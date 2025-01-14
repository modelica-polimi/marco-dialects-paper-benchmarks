package ThermalChipOO
  package Types
    type Temperature = Real(unit = "K", nominal = 500);
    type Power = Real(unit = "W");
    type ThermalConductivity = Real(unit = "W/(m.K)");
    type ThermalConductance = Real(unit = "W/K");
    type SpecificHeatCapacity = Real(unit = "J/(kg.K)");
    type ThermalCapacitance = Real(unit = "J/K");
    type Density = Real(unit = "kg/m3");
    type Length = Real(unit = "m");
    type Time = Real(unit = "s");
    connector PowerInput = input Power;
  end Types;

  package Interfaces
    connector HeatPort
      Types.Temperature T;
      flow Types.Power Q;
    end HeatPort;
  end Interfaces;

  package Models

    model Volume
      parameter Types.ThermalConductivity lambda = 148 "Thermal conductivity of silicon" annotation(
        Evaluate = true);
      parameter Types.Density rho = 2329 "Density of silicon" annotation(
        Evaluate = true);
      parameter Types.SpecificHeatCapacity c = 700 "Specific heat capacity of silicon" annotation(
        Evaluate = true);
      parameter Types.Temperature Tstart = 273.15 + 40;
      parameter Types.ThermalCapacitance C "Thermal capacitance of a volume";
      parameter Types.ThermalConductance Gx "Thermal conductance of half a volume,x direction";
      parameter Types.ThermalConductance Gy "Thermal conductance of half a volume,y direction";
      parameter Types.ThermalConductance Gz "Thermal conductance of half a volume,z 
    direction";
    
      Interfaces.HeatPort upper "Upper surface thermal port";
      Interfaces.HeatPort lower "Lower surface thermal port";
      Interfaces.HeatPort left "Left surface thermal port";
      Interfaces.HeatPort right "Right surface thermal port";
      Interfaces.HeatPort top "Top surface thermal port";
      Interfaces.HeatPort bottom "Bottom surface thermal port";
      Interfaces.HeatPort center "Volume center thermal port";
      
      Types.Temperature T(start = Tstart, fixed = true) "Volume temperature";
    equation
      C*der(T) = upper.Q + lower.Q + left.Q + right.Q + top.Q + bottom.Q + center.Q;
    
      upper.Q  = Gx*(upper.T  - T);
      lower.Q  = Gx*(lower.T  - T);
      left.Q   = Gy*(left.T   - T);
      right.Q  = Gy*(right.T  - T);
      top.Q    = Gz*(top.T    - T);
      bottom.Q = Gz*(bottom.T - T);
      center.T = T;
    end Volume;

    model TemperatureSource
      Interfaces.HeatPort port;
      Types.Temperature T = 298.15 "Source temperature";
    equation
  port.T = T;
    end TemperatureSource;
    
    model PowerSource
      Interfaces.HeatPort port;
      Types.PowerInput Q "Source thermal power leaving the port";
    equation
      port.Q = -Q;
    end PowerSource;
    partial model BaseThermalChip
      parameter Integer N = 66 "Number of volumesin the x direction";
      parameter Integer M = 66 "Number of volumesin the y direction";
      parameter Integer P = 22 "Number of volumesin the z direction";
      parameter Types.Length L = 12e-3 "Chip lengthin the x direction" annotation(
        Evaluate = true);
      parameter Types.Length W = 12e-3 "Chip widthin the y direction" annotation(
        Evaluate = true);
      parameter Types.Length H = 4e-3 "Chip heightin the z direction" annotation(
        Evaluate = true);
      parameter Types.ThermalConductivity lambda = 148 "Thermal conductivity of silicon" annotation(
        Evaluate = true);
      parameter Types.Density rho = 2329 "Density of silicon" annotation(
        Evaluate = true);
      parameter Types.SpecificHeatCapacity c = 700 "Specific heat capacity of silicon" annotation(
        Evaluate = true);
      parameter Types.Temperature Tstart = 273.15 + 40;
      final parameter Types.Length l = L / N "Chip lengthin the x direction";
      final parameter Types.Length w = W / M "Chip widthin the y direction";
      final parameter Types.Length h = H / P "Chip heightin the z direction";
      parameter Types.Temperature Tt = 273.15 + 40 "Prescribed temperature of the top surface" annotation(
        Evaluate = true);
      final parameter Types.ThermalCapacitance C = rho*c*l*w*h "Thermal capacitance of a volume";
      final parameter Types.ThermalConductance Gx = lambda*w*h/l "Thermal conductance of a volume,x direction";
      final parameter Types.ThermalConductance Gy = lambda*l*h/w "Thermal conductance of a volume,y direction";
      final parameter Types.ThermalConductance Gz = lambda*l*w/h "Thermal conductance of a volume,z direction";
    
      Volume vol[N,M,P](each T(start = Tstart, fixed = true),
                        each C = C,
                        each Gx = 2*Gx, each Gy = 2*Gy, each Gz = 2*Gz);
      TemperatureSource Tsource[N,M](each T = Tt);
    
      output Types.Temperature Tct1 = vol[1,1,1].T "Top layer corner volume temperature";
      output Types.Temperature Tct2 = vol[1,N,1].T "Top layer corner volume temperature";
      output Types.Temperature Tct3 = vol[N,N,1].T "Top layer corner volume temperature";
      output Types.Temperature Tct4 = vol[N,1,1].T "Top layer corner volume temperature";
      output Types.Temperature Tcb1 = vol[1,1,P].T "Bottom layer corner volume temperature";
      output Types.Temperature Tcb2 = vol[1,N,P].T "Bottom layer corner volume temperature";
      output Types.Temperature Tcb3 = vol[N,N,P].T "Bottom layer corner volume temperature";
      output Types.Temperature Tcb4 = vol[N,1,P].T "Bottom layer corner volume temperature";
    equation
      // Connections in the z direction
      for i in 1:N loop
        for j in 1:M loop
          connect(vol[i,j,1].top, Tsource[i,j].port);
          for k in 1:P-1 loop
            connect(vol[i,j,k].bottom, vol[i,j,k+1].top);
          end for;
        end for;
      end for;
      
      // Connections in the y direction
      for i in 1:N loop
        for k in 1:P loop
          for j in 1:M-1 loop
            connect(vol[i,j,k].right, vol[i,j+1,k].left);
          end for;
        end for;
      end for;
    
      // Connections in the x direction
      for j in 1:M loop
        for k in 1:P loop
           for i in 1:N-1 loop
             connect(vol[i,j,k].lower, vol[i+1,j,k].upper);
           end for;
         end for;
       end for;
    end BaseThermalChip;

    model ThermalChipSimpleBoundary "Thermal chip model written by explicit ODEs, constant power on half of the bottom surface"
      extends BaseThermalChip;
      parameter Types.Power Ptot = 100 "Total power consumption";
      final parameter Types.Power Pv = Ptot / (N * M / 2) "Power dissipated in a single volume";
      PowerSource Qsource[N,div(M,2)](each Q = Pv);
    equation
      connect(Qsource.port, vol[:, 1:div(M,2), P].center);
    end ThermalChipSimpleBoundary;
  end Models;

  package Test
    model ThermalChipSimpleBoundary
      extends Models.ThermalChipSimpleBoundary;
      annotation(
        experiment(StartTime = 0, StopTime = 0.6, Tolerance = 1e-9, Interval = 0.0001),
        __OpenModelica_simulationFlags(lv = "LOG_STATS", s = "dassl"));
    end ThermalChipSimpleBoundary;
  end Test;
end ThermalChipOO;
