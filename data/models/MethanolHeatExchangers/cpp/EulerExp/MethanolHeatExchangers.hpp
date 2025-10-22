/***************************************************************************
 *   Copyright (C) 2020-2021 by Politecnico di Milano                      *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU Lesser General Public License as        *
 *   published by the Free Software Foundation; either version 3 of the    *
 *   License, or (at your option) any later version.                       *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU Lesser General Public License for more details.                   *
 *                                                                         *
 *   You should have received a copy of the GNU Lesser General Public      *
 *   License along with this program; if not, see                          *
 *   <http://www.gnu.org/licenses/>                                        *
 ***************************************************************************/

#pragma once
#include "CsvEmitter.hpp"
#include "ModelSolutionI.hpp"
#include "matrix.hpp"
#include "types.hpp"

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

class MethanolHeatExchangersEulerExp : public ModelSolutionI
{
public:
    MethanolHeatExchangersEulerExp(size_t Nu, size_t Nh, size_t Nv, double stop_time, int num_intervals);

    ~MethanolHeatExchangersEulerExp();

    bool step();

    void emitDataTo(Emitter<CSV_emitter> *outputStream);

private:
    // structs
    const size_t Nu; // Number of units in series
    const size_t Nh; // Number of heat exchangers in parallel for each unit
    const size_t Nv; // Number of finite volumes for each heat exchanger

    struct state_t {
        matrix3d_t T_tilde; // Fluid temperature states
        matrix3d_t T_w; // Wall temperatures
        real_t *T_m; // Temperatures of the mixers at each unit outlet
    };

    struct input_t {
        real_t w; // Total mass flow rate flowing through the system
        real_t w_h; // Mass flow rate flowing through each individual heat exchanger
        real_t *Q; // Thermal power to each heat exchanger in a unit
    };

    struct aux_t {
        matrix3d_t Q_c; // Convective heat transfer on each heat exchanger volume
        matrix3d_t T; // Temperatures at the heat exchanger volume boundaries
    };

    struct methanol_t {
        matrix3d_t h; // Specific enthalpies at the heat exchanger volume boundaries
        matrix3d_t rho; // Density in each heat exchanger volume
        matrix3d_t cv; // cv in each heat exchanger volume
        real_t *h_m; // Specific enthalpies of the mixers at the unit outlets
        real_t *rho_m; // Density in each mixer at the unit outlets
        real_t *cv_m; // cv in mixer at the unit outlets
    };

    struct output_state_index_t {
        std::vector<size_t> T_m;
    };

private:
    // methods
    void initializationProblem();
    void outputDefinition();
    void updateInput();
    void updateState();
    void updateAux();
    void updateMethanol();

private:
    // data
    state_t *state;
    input_t *input;
    methanol_t *methanol;
    aux_t *aux;
    output_state_index_t output;

    // induction step
    const double start_time = 0;
    const double stop_time;
    const int num_intervals;
    const real_t _h = (real_t)(stop_time - start_time) / (real_t)num_intervals;
    const double _epsilon = 1e-06;
    double _cur_interval = 0; // Integer
    double _time;

    // model parameters
    const real_t w_nom = 1; // Nominal mass flow rate
    const real_t Q_nom = 500e3; // Nominal unit power
    const real_t f_w = 1.0/30.0; // Period of mass flow rate fluctuations
    const real_t f_Q = 1.0/100.0; // Period of thermal power fluctuations
    const real_t T0 = 493.15; // Reference temperature
    const real_t V = 1; // Total volume of the fluid in the system
    const real_t beta = 0.01; // Fraction of the total volume taken by mixing volumes
    const real_t UA_nom = 10000; // Nominal UA value of each unit
    const real_t alpha = 0.8; // Coefficient of mass flow rate in the UA value
    const real_t Cw = 10000; // Total heat capacity of the heat exchanger walls of each unit
    const real_t p_nom = 20e5; // Nominal fluid pressure
    const real_t V_v = V*(1-beta)/(Nu*Nh*Nv); // Fluid volume of each finite volume
    const real_t V_m = V*beta/Nu;
    const real_t C_wv = Cw/(Nu*Nh*Nv); // Heat capacity of the walls for each finite volume
    const real_t pi = M_PI;
};
