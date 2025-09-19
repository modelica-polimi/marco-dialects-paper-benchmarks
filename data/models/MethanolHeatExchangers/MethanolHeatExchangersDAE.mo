package MethanolHeatExchangersDAE
  package Types
    type Temperature = Real(unit = "K", displayUnit = "degC", nominal = 500);
    type Power = Real(unit = "W");
    type ThermalConductivity = Real(unit = "W/(m.K)");
    type ThermalConductance = Real(unit = "W/K");
    type SpecificEnthalpy = Real(unit = "J/kg");
    type SpecificHeatCapacity = Real(unit = "J/(kg.K)");
    type ThermalCapacitance = Real(unit = "J/K");
    type CoefficientOfHeatTransfer = Real(unit = "W/(m2.K)");
    type Density = Real(unit = "kg/m3");
    type MassFlowRate = Real(unit = "kg/s");
    type Length = Real(unit = "m");
    type Time = Real(unit = "s");
    type PerUnit = Real(unit = "1");
    type Frequency = Real(unit = "Hz");
    type Volume = Real(unit = "m3");
    type HeatCapacity = Real(unit = "J/K");
    type Pressure = Real(unit = "Pa");
  end Types;

  package Models
    model MethanolHeatExchangers
      parameter Integer Nu = 3 "Number of units in series";
      parameter Integer Nh = 4 "Number of heat exchangers in parallel for each unit";
      parameter Integer Nv = 6 "Number of finite volumes for each heat exchanger";
      parameter Types.MassFlowRate w_nom = 1 "Nominal mass flow rate";
      parameter Types.Power Q_nom = 500e3 "Nominal unit power";
      parameter Types.Frequency f_w = 1/30 "Period of mass flow rate fluctuations";
      parameter Types.Frequency f_Q = 1/100 "Period of thermal power fluctuations";
      parameter Types.Temperature T0 = 493.15 "Reference temperature";
      parameter Types.Volume V = 1 "Total volume of the fluid in the system";
      parameter Types.PerUnit beta = 0.01 "Fraction of the total volume taken by mixing volumes";
      parameter Types.ThermalConductance UA_nom = 10000 "Nominal UA value of each unit";
      parameter Types.PerUnit alpha = 0.8 "Coefficient of mass flow rate in the UA value";
      parameter Types.HeatCapacity Cw = 10000 "Total heat capacity of the heat exchanger walls of each unit";
      parameter Types.Pressure p_nom = 20e5 "Nominal fluid pressure";
      final parameter Types.Volume V_v = V*(1-beta)/(Nu*Nh*Nv) "Fluid volume of each finite volume";
      final parameter Types.Volume V_m = V*beta/Nu;
      final parameter Types.HeatCapacity C_wv = Cw/(Nu*Nh*Nv) "Heat capacity of the walls for each finite volume";
      constant Real pi = 3.14159265359;
      Types.MassFlowRate w "Total mass flow rate flowing through the system";
      Types.MassFlowRate w_h "Mass flow rate flowing through each individual heat exchanger";
      Types.Power Q[Nh] "Thermal power to each heat exchanger in a unit";
      Types.Power Q_c[Nu,Nh,Nv] "Convective heat transfer on each heat exchanger volume";
      Types.Temperature T[Nu,Nh,Nv+1] "Temperatures at the heat exchanger volume boundaries";
      Types.SpecificEnthalpy h[Nu,Nh,Nv+1] "Specific enthalpies at the heat exchanger volume boundaries";
      Types.SpecificEnthalpy h_m[Nu] "Specific enthalpies of the mixers at the unit outlets";
      Types.Temperature T_tilde[Nu,Nh,Nv](each start = T0, each fixed = true) "Fluid temperature states";
      Types.Density rho[Nu,Nh,Nv] "Density in each heat exchanger volume";
      Types.Density rho_m[Nu] "Density in each mixer at the unit outlets";
      Types.SpecificHeatCapacity cv[Nu,Nh,Nv] "cv in each heat exchanger volume";
      Types.SpecificHeatCapacity cv_m[Nu] "cv in mixer at the unit outlets";
      Types.Temperature T_w[Nu,Nh,Nv](each start = T0, each fixed = true) "Wall temperatures";
      output Types.Temperature T_m[Nu](each start = T0, each fixed = true) "Temperatures of the mixers at each unit outlet";
    equation
      w = w_nom * (1 + 0.2*sin(2*pi*f_w*time));
      w_h = w / Nh;
      for j in 1:Nh loop
        Q[j] = Q_nom/(Nu*Nh)*(1 + sin(2*pi*f_Q*time + 2*pi*j/Nh));
      end for;
      for j in 1:Nh loop
        T[1,j,1] = T0 "Inlet temperatures of heat exchangers in first unit";
      end for;
      for i in 2:Nu loop
        for j in 1:Nh loop
          T[i,j,1] = T_m[i-1] "Inlet temperatures of heat exhangers of units 2:Nu";
        end for;
      end for;
      T_tilde = T[:,:,2:Nv + 1] "Volume outlet temperatures";
      for i in 1:Nu loop
        // Energy balance equation for the mixers at the unit outlets
        V_m*rho_m[i]*cv_m[i]*der(T_m[i]) = w_h*sum(h[i,j,Nv+1] for j in 1:Nh) - w*h_m[i];
        for j in 1:Nh loop
          for k in 1:Nv loop
            // Energy balance equations for the heat exchangers
            V_v*rho[i,j,k]*cv[i,j,k]*der(T_tilde[i,j,k]) = w_h*(h[i,j,k] - h[i,j,k+1]) + Q_c[i,j,k];
            C_wv*der(T_w[i, j, k]) = Q[j]/Nv - Q_c[i,j,k];
            Q_c[i,j,k] = UA_nom/(Nu*Nh*Nv)*(w/w_nom)^alpha*(T_w[i,j,k] - T_tilde[i,j,k]);
          end for;
        end for;
      end for;
  // Fluid property computations
      for i in 1:Nu loop
        rho_m[i] = p_nom /(Methanol.R*T_m[i]);
        h_m[i] = Methanol.h_T(T_m[i]);
        cv_m[i] = Methanol.cv_T(T_m[i]);
        for j in 1:Nh loop
          for k in 1:Nv loop
            rho[i,j,k] = p_nom /(Methanol.R*T_tilde[i,j,k]);
            cv[i,j,k] = Methanol.cv_T(T_tilde[i,j,k]);
          end for;
          for k in 1:Nv+1 loop
            h[i,j,k] = Methanol.h_T(T[i,j,k]);
          end for;
        end for;
      end for;
      annotation(
        experiment(StartTime = 0, StopTime = 100, Tolerance = 1e-06, Interval = 0.01),
        __OpenModelica_simulationFlags(lv = "LOG_STATS", s = "euler"));
    end MethanolHeatExchangers;

    package Methanol
      constant Types.SpecificHeatCapacity R = 8.314462 / 32.04e-3;
      constant Types.Temperature Tc = 512.64;
      constant Types.PerUnit f[8] = {3.90086, 10.9929, 18.3371, -16.3663, -6.22334, 2.80358, 1.07783, 0.96967};
      constant Types.PerUnit g[8] = {0.0, 4.12575, 3.26973, 3.77492, 2.93574, 8.23747, 10.3312, 0.53326};

      function cp_T
        input Types.Temperature T;
        output Types.SpecificHeatCapacity cp;
      protected
        Types.PerUnit tau;
        Types.PerUnit u[8];
        Types.PerUnit x;
      algorithm
        tau := Tc / T;
        u := g * tau;
        x := f[1];
        for i in 2:8 loop
          x := x + f[i] * u[i] ^ 2 * exp(u[i]) / (exp(u[i]) - 1) ^ 2;
        end for;
        cp := x * R;
      end cp_T;

      function cv_T
        input Types.Temperature T;
        output Types.SpecificHeatCapacity cv;
      algorithm
        cv := cp_T(T) - R;
      end cv_T;

      function h_T
        input Types.Temperature T;
        output Types.SpecificEnthalpy h;
      protected
        Types.PerUnit tau;
        Types.PerUnit u[8];
        Types.PerUnit x;
      algorithm
        tau := Tc / T;
        u := g * tau;
        x := f[1] / tau;
        for i in 2:8 loop
          x := x + f[i] * g[i] / (exp(u[i]) - 1);
        end for;
        h := R * T * tau * x - 1361.810 * tau / Tc;
      end h_T;

      model Test
        parameter Types.Time Tone = 1;
        Types.Temperature T;
        Types.SpecificEnthalpy h;
        Types.SpecificHeatCapacity cp;
        Types.SpecificHeatCapacity cp_check;
      equation
        T = 500 + 200 * time / Tone;
        h = h_T(T);
        cp = cp_T(T);
        der(h) / der(T) = cp_check;
      end Test;

      model Test2
        parameter Types.Time Tone = 1;
        Types.Temperature T;
        Types.SpecificHeatCapacity cp;
      equation
        T = 500 + 200 * time / Tone;
        cp = cp_T(T);
      end Test2;
    end Methanol;
  end Models;

  package Benchmarks
  end Benchmarks;
end MethanolHeatExchangersDAE;
