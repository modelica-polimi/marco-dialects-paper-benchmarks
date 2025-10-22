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

#include "MethanolHeatExchangers.hpp"
#include "Methanol.hpp"

MethanolHeatExchangersEulerExp::MethanolHeatExchangersEulerExp(size_t Nu, size_t Nh, size_t Nv , double stop_time, int num_intervals)
    : Nu(Nu), Nh(Nh), Nv(Nv), stop_time(stop_time), num_intervals(num_intervals)
{
    // state allocation
    state = new state_t();
    input = new input_t();
    aux = new aux_t();
    methanol = new methanol_t();

    state->T_tilde = matrix3d_create({Nu, Nh, Nv});
    state->T_w = matrix3d_create({Nu, Nh, Nv});
    state->T_m = new real_t[Nu];
    input->Q = new real_t[Nh];
    aux->Q_c = matrix3d_create({Nu, Nh, Nv});
    aux->T = matrix3d_create({Nu, Nh, Nv+1});
    methanol->h = matrix3d_create({Nu, Nh, Nv+1});
    methanol->rho = matrix3d_create({Nu, Nh, Nv});
    methanol->cv = matrix3d_create({Nu, Nh, Nv});
    methanol->h_m = new real_t[Nu];
    methanol->rho_m = new real_t[Nu];
    methanol->cv_m = new real_t[Nu];

    // initialization
    initializationProblem();

    // definition of states to be observed
    outputDefinition();
}

MethanolHeatExchangersEulerExp::~MethanolHeatExchangersEulerExp()
{
    matrix3d_destroy(state->T_tilde);
    matrix3d_destroy(state->T_w);
    matrix3d_destroy(aux->Q_c);
    matrix3d_destroy(aux->T);
    matrix3d_destroy(methanol->h);
    matrix3d_destroy(methanol->rho);
    matrix3d_destroy(methanol->cv);

    delete state;
    delete input;
    delete methanol;
    delete aux;
}

bool MethanolHeatExchangersEulerExp::step()
{
    updateInput();
    updateAux();
    updateMethanol();
    updateState();

    _cur_interval += 1.0;
    _time = start_time + _cur_interval * _h;
    return (_time < stop_time - _epsilon);
}

void MethanolHeatExchangersEulerExp::emitDataTo(Emitter<CSV_emitter> *outputStream)
{
    // Initialize output
    outputStream->pushValue(_time);
    outputStream->pushSeparator();

    // Output of temperature T_m
    for (size_t i = 0; i < output.T_m.size() - 1; ++i)
    {
        size_t index = output.T_m[i];
        outputStream->pushValue(state->T_m[index]);
        outputStream->pushSeparator();
    }
    if (output.T_m.size() > 0)
    {
        size_t last_index = output.T_m[output.T_m.size() - 1];
        outputStream->pushValue(state->T_m[last_index]);
    }

    // Finalize output
    outputStream->endline();
}

void MethanolHeatExchangersEulerExp::initializationProblem()
{
    // time
    _time = start_time;

    // state
    for (size_t i = 0; i < Nu; ++i) {
        for (size_t j = 0; j < Nh; ++j) {
            for (size_t k = 0; k < Nv; ++k) {
                matrix3d_set(T0, state->T_tilde, {i, j, k});
                matrix3d_set(T0, state->T_w, {i, j, k});
            }
        }
        state->T_m[i] = T0;
    }
}

void MethanolHeatExchangersEulerExp::outputDefinition()
{
    for (size_t i = 0; i < Nu; ++i)
    {
        output.T_m.push_back(i);
    }
}

void MethanolHeatExchangersEulerExp::updateState()
{
    for (size_t i = 0; i < Nu; ++i)
    {
        real_t h_sum = 0;
        for (size_t j = 0; j < Nh; ++j)
        {
            for (size_t k = 0; k < Nv; ++k)
            {
                // Energy balance equations for the heat exchangers
                matrix3d_set(matrix3d_get(state->T_tilde, {i, j, k})
                    + (input->w_h*(matrix3d_get(methanol->h, {i, j, k})-matrix3d_get(methanol->h, {i, j, k+1}))
                    + matrix3d_get(aux->Q_c, {i, j, k}))
                    * _h / (V_v*matrix3d_get(methanol->rho, {i, j, k})*matrix3d_get(methanol->cv, {i, j, k})),
                    state->T_tilde, {i, j, k});

                matrix3d_set(matrix3d_get(state->T_w, {i, j, k})
                    + (input->Q[j]/Nv - matrix3d_get(aux->Q_c, {i, j, k}))
                    * _h / C_wv,
                    state->T_w, {i, j, k});
            }
            h_sum += matrix3d_get(methanol->h, {i, j, Nv});
        }
        // Energy balance equation for the mixers at the unit outlets
        state->T_m[i] +=
            (input->w_h*h_sum - input->w*methanol->h_m[i])
            * _h / (V_m*methanol->rho_m[i]*methanol->cv_m[i]);
    }
}

void MethanolHeatExchangersEulerExp::updateInput()
{
    input->w = w_nom * (1 + 0.2*sin(2*pi*f_w*_time));
    input->w_h = input->w / Nh;
    for (size_t j = 0; j < Nh; ++j)
    {
        input->Q[j] = Q_nom/(Nu*Nh)*(1 + sin(2*pi*f_Q*_time + 2*pi*j/Nh));
    }
}

void MethanolHeatExchangersEulerExp::updateAux()
{
    // Inlet temperatures of heat exchangers in first unit
    for (size_t j = 0; j < Nh; ++j)
    {
        matrix3d_set(T0, aux->T, {0, j, 0});
    }

    // Inlet temperatures of heat exchangers of units 2:Nu
    for (size_t i = 1; i < Nu; ++i)
    {
        for (size_t j = 0; j < Nh; ++j)
        {
            matrix3d_set(state->T_m[i-1], aux->T, {i, j, 0});
        }
    }

    // Volume outlet temperatures
    for (size_t i = 0; i < Nu; ++i)
    {
        for (size_t j = 0; j < Nh; ++j)
        {
            for (size_t k = 1; k <= Nv; ++k)
            {
                matrix3d_set(matrix3d_get(state->T_tilde, {i, j, k-1}), aux->T, {i, j, k});
            }
        }
    }

    // Convective heat transfer on each heat exchanger volume
    for (size_t i = 0; i < Nu; ++i)
    {
        for (size_t j = 0; j < Nh; ++j)
        {
            for (size_t k = 0; k < Nv; ++k)
            {
                matrix3d_set(UA_nom/(Nu*Nh*Nv)*hipermod_power((input->w/w_nom), alpha)
                    * (matrix3d_get(state->T_w, {i, j, k}) - matrix3d_get(state->T_tilde, {i, j, k})),
                    aux->Q_c, {i, j, k});
            }
        }
    }
}

// Fluid property computations
void MethanolHeatExchangersEulerExp::updateMethanol()
{
    for (size_t i = 0; i < Nu; ++i)
    {
        methanol->rho_m[i] = p_nom /(Methanol::R*state->T_m[i]);
        methanol->h_m[i] = Methanol::h_T(state->T_m[i]);
        methanol->cv_m[i] = Methanol::cv_T(state->T_m[i]);

        for (size_t j = 0; j < Nh; ++j)
        {
            for (size_t k = 0; k < Nv; ++k)
            {
                matrix3d_set(p_nom / (Methanol::R*matrix3d_get(state->T_tilde, {i, j, k})),
                    methanol->rho, {i, j, k});
                matrix3d_set(Methanol::cv_T(matrix3d_get(state->T_tilde, {i, j, k})),
                    methanol->cv, {i, j, k});
                matrix3d_set(Methanol::h_T(matrix3d_get(aux->T, {i, j, k})),
                    methanol->h, {i, j, k});
            }
            matrix3d_set(Methanol::h_T(matrix3d_get(aux->T, {i, j, Nv})),
                methanol->h, {i, j, Nv});
        }
    }
}
