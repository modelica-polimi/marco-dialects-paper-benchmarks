/***************************************************************************
 *   Copyright (C) 2019-2020 by Politecnico di Milano                      *
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
#include "types.hpp"
#include "Event.hpp"
#include "matrix.hpp"

#include <cstdlib>
#include <vector>


class BaseThermalChipEulerExp : public ModelSolutionI
{
public:

    BaseThermalChipEulerExp(size_t nx, size_t ny, size_t nz, real_t stop_time, int num_intervals);

    ~BaseThermalChipEulerExp();

    bool step();

    void emitDataTo(Emitter<CSV_emitter> *outputStream);

protected:

    void initializationProblem();

    void outputDefinition(){}

    virtual void updateState();

    virtual void updateInput(){}

    const size_t nx;
    const size_t ny;
    const size_t nz;

    struct input_t
    {
        real_t Tt;
        matrix2d_t Qb;
    };

    struct matrix_index_t
    {
        size_t x, y, z;
    };

    struct output_state_index_t
    {
        std::vector<matrix_index_t> T;
        std::vector<std::pair<size_t, size_t> > Q;
    };

    input_t* input;
    output_state_index_t output;

    // induction step
    const double start_time = 0;
    const double stop_time;
    const int num_intervals;
    double _cur_interval = 0;
    double _time;
    const double _h = (stop_time - start_time) / num_intervals;
    const double _epsilon = _h / 100;

private:
    // here we only need to construct this object and perform a deep copy of it.
    struct state_t
    {
        matrix3d_t T;
    };

    //data
    state_t *state;
    state_t *prev_state;

    // model parameters
    const real_t L = 12e-3;     // Chip length in the x direction
    const real_t W = 12e-3;     // Chip width in the y direction
    const real_t H = 4e-3;      // Chip height in the z direction

    const real_t rho = 2329;    // Density of Silicon
    const real_t lambda = 148;  // Thermal conductivity of silicon
    const real_t c = 700;       // Specific heat capacity of silicon

    const real_t l = L / nx;    // Chip length in the x direction
    const real_t w = W / ny;    // Chip width in the y direction
    const real_t h = H / nz;    // Chip height in the z direction

    const real_t Tstart = 273.15 + 40;
    const real_t Tt = 273.15 + 40;
    const real_t C = rho * c * l * w * h; // Thermal capacitance of a volume
    const real_t Gx = lambda * w * h / l; // Thermal conductance of a volume, x direction
    const real_t Gy = lambda * l * h / w; // Thermal conductance of a volume, y direction
    const real_t Gz = lambda * l * w / h; // Thermal conductance of a volume, z direction
};
