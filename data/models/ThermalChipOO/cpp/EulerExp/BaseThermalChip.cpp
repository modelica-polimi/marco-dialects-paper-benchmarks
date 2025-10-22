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

#include "BaseThermalChip.hpp"

BaseThermalChipEulerExp::BaseThermalChipEulerExp(size_t nx, size_t ny, size_t nz , real_t stop_time, int num_intervals)
    : nx(nx), ny(ny), nz(nz), stop_time(stop_time), num_intervals(num_intervals)
{
    // state allocation
    state = new state_t();
    prev_state = new state_t();
    input = new input_t();

    state->T = matrix3d_create({nx, ny, nz});
    prev_state->T = matrix3d_create({nx, ny, nz});
    input->Qb = matrix2d_create({nx, ny});

    // initialization
    initializationProblem();
    outputDefinition();
}

BaseThermalChipEulerExp::~BaseThermalChipEulerExp()
{
    matrix3d_destroy(state->T);
    matrix3d_destroy(prev_state->T);
    matrix2d_destroy(input->Qb);

    delete state;
    delete prev_state;
    delete input;
}

bool BaseThermalChipEulerExp::step()
{
    updateInput();
    updateState();

    _cur_interval += 1.0;
    _time = start_time + _cur_interval * _h;
    return (_time < stop_time - _epsilon);
}

void BaseThermalChipEulerExp::emitDataTo(Emitter<CSV_emitter> *outputStream)
{
    outputStream->pushValue(_time);
    outputStream->pushSeparator();

    // keep track of line status
    // depends only on size() or empty() -- reduce runtime checks
    // When the selection of data to be emitted is done at code generation time
    // this value can be optimized out by the compiler
    bool empty_line = true;

    // Output of temperature T
    if(!output.T.empty())
    {
        for(size_t i = 0; i < output.T.size() - 1; i++)
        {
            const auto& index = output.T[i];
            outputStream->pushValue(matrix3d_get(state->T, {index.x,index.y,index.z}));
            outputStream->pushSeparator();
        }

        if(output.T.size() > 0)
        {
            const auto last_index = output.T[output.T.size() - 1];
            outputStream->pushValue(matrix3d_get(state->T, {last_index.x,last_index.y,last_index.z}));
            empty_line = false;
        }
    }

    // Output of power Q
    if(!output.Q.empty())
    {
        if(!empty_line)
        {
            outputStream->pushSeparator();
        }

        const auto& first_Q_index = output.Q[0];
        outputStream->pushValue(matrix2d_get(input->Qb, {first_Q_index.first,first_Q_index.second}));
        empty_line = false;
    }

    for(size_t i = 1; i < output.Q.size(); i++)
    {
        outputStream->pushSeparator();
        const auto& index = output.Q[i];
        outputStream->pushValue(matrix2d_get(input->Qb, {index.first,index.second}));
    }

    // Finalize output
    outputStream->endline();
}

void BaseThermalChipEulerExp::initializationProblem()
{
    // time
    _time = start_time;

    // state
    for(size_t i=0; i<nx; i++)
    {
        for(size_t j=0; j<ny; j++)
        {
            for(size_t k=0; k<nz; k++)
            {
                matrix3d_set(Tstart, state->T, {i,j,k});
            }
        }
    }

    // input
    input->Tt=Tt;
    return;
}

void BaseThermalChipEulerExp::updateState()
{
    std::swap(state, prev_state);

    // corners
    matrix3d_set((1.0 - (Gx + Gy + 3 * Gz) * _h / C) * matrix3d_get(prev_state->T, {0,0,0})
        + (Gx * _h / C) * matrix3d_get(prev_state->T, {1,0,0})
        + (Gy * _h / C) * matrix3d_get(prev_state->T, {0,1,0})
        + (Gz * _h / C) * matrix3d_get(prev_state->T, {0,0,1})
        + (2 * Gz / C) * input->Tt *_h, 
        state->T,
        {0,0,0});

    matrix3d_set((1.0 - (Gx + Gy + 3 * Gz) * _h / C) * matrix3d_get(prev_state->T, {nx-1, 0, 0}) 
        + (Gx * _h / C) * matrix3d_get(prev_state->T, {nx-2,0,0})
        + (Gy * _h / C) * matrix3d_get(prev_state->T, {nx-1,1,0})
        + (Gz * _h / C) * matrix3d_get(prev_state->T, {nx-1,0,1})
        + (2 * Gz / C) * input->Tt *_h, 
        state->T,
        {nx-1, 0, 0});

    matrix3d_set((1.0 - (Gx + Gy + 3 * Gz) * _h / C) * matrix3d_get(prev_state->T, {0,ny-1,0})
        + (Gx * _h / C) * matrix3d_get(prev_state->T, {1,ny-1,0})
        + (Gy * _h / C) * matrix3d_get(prev_state->T, {0,ny-2,0})
        + (Gz * _h / C) * matrix3d_get(prev_state->T, {0,ny-1,1})
        + (2 * Gz / C) * input->Tt *_h,
        state->T,
        {0, ny-1, 0});

    matrix3d_set((1.0 - (Gx + Gy + 3 * Gz) * _h / C) * matrix3d_get(prev_state->T, {nx-1,ny-1,0})
        + (Gx * _h / C) * matrix3d_get(prev_state->T, {nx-2,ny-1,0})
        + (Gy * _h / C) * matrix3d_get(prev_state->T, {nx-1,ny-2,0})
        + (Gz * _h / C) * matrix3d_get(prev_state->T, {nx-1,ny-1,1})
        + (2 * Gz / C) * input->Tt *_h,
        state->T,
        {nx-1, ny-1, 0});

    matrix3d_set((1.0 - (Gx + Gy + Gz) * _h / C) * matrix3d_get(prev_state->T, {0,0,nz-1})
        + (Gx * _h / C) * matrix3d_get(prev_state->T, {1,0,nz-1})
        + (Gy * _h / C) * matrix3d_get(prev_state->T, {0,1,nz-1})
        + (Gz * _h / C) * matrix3d_get(prev_state->T, {0,0,nz-2})
        + matrix2d_get(input->Qb, {0,0}) / C *_h,
        state->T,
        {0,0,nz-1});

    matrix3d_set((1.0 - (Gx + Gy + Gz) * _h / C) * matrix3d_get(prev_state->T, {nx-1,0,nz-1})
        + (Gx * _h / C) * matrix3d_get(prev_state->T, {nx-2,0,nz-1})
        + (Gy * _h / C) * matrix3d_get(prev_state->T, {nx-1,1,nz-1})
        + (Gz * _h / C) * matrix3d_get(prev_state->T, {nx-1,0,nz-2})
        + matrix2d_get(input->Qb, {nx-1,0}) / C *_h,
        state->T,
        {nx-1, 0, nz-1});

    matrix3d_set((1.0 - (Gx + Gy + Gz) * _h / C) * matrix3d_get(prev_state->T, {0,ny-1,nz-1})
        + (Gx * _h / C) * matrix3d_get(prev_state->T, {1,ny-1,nz-1})
        + (Gy * _h / C) * matrix3d_get(prev_state->T, {0,ny-2,nz-1})
        + (Gz * _h / C) * matrix3d_get(prev_state->T, {0,ny-1,nz-2})
        + matrix2d_get(input->Qb, {0,ny-1}) / C *_h,
        state->T,
        {0, ny-1, nz-1});

    matrix3d_set((1.0 - (Gx + Gy + Gz) * _h / C) * matrix3d_get(prev_state->T, {nx-1,ny-1,nz-1})
        + (Gx * _h / C) * matrix3d_get(prev_state->T, {nx-2,ny-1,nz-1})
        + (Gy * _h / C) * matrix3d_get(prev_state->T, {nx-1,ny-2,nz-1})
        + (Gz * _h / C) * matrix3d_get(prev_state->T, {nx-1,ny-1,nz-2})
        + matrix2d_get(input->Qb, {nx-1,ny-1}) / C *_h,
        state->T,
        {nx-1, ny-1, nz-1});
    // end of corners

    // edges
    for(size_t x = 1; x < nx - 1; ++x)
    {
        matrix3d_set((1.0 - (2 * Gx + Gy + 3 * Gz) * _h / C) * matrix3d_get(prev_state->T, {x,0,0})
            + (Gx * _h / C) * matrix3d_get(prev_state->T, {x-1, 0, 0}) 
            + (Gx * _h / C) * matrix3d_get(prev_state->T, {x+1, 0, 0}) 
            + (Gy * _h / C) * matrix3d_get(prev_state->T, {x, 1, 0}) 
            + (Gz * _h / C) * matrix3d_get(prev_state->T, {x, 0, 1}) 
            + (2 * Gz / C) * input->Tt *_h,
            state->T,
            {x, 0, 0});

        matrix3d_set((1.0 - (2 * Gx + Gy + 3 * Gz) * _h / C) * matrix3d_get(prev_state->T, {x, ny-1, 0}) 
            + (Gx * _h / C) * matrix3d_get(prev_state->T, {x-1, ny-1, 0}) 
            + (Gx * _h / C) * matrix3d_get(prev_state->T, {x+1, ny-1, 0}) 
            + (Gy * _h / C) * matrix3d_get(prev_state->T, {x, ny-2, 0}) 
            + (Gz * _h / C) * matrix3d_get(prev_state->T, {x, ny-1, 1}) 
            + (2 * Gz / C) * input->Tt *_h,
            state->T,
            {x, ny-1, 0});

        matrix3d_set((1.0 - (2 * Gx + Gy + Gz) * _h / C) * matrix3d_get(prev_state->T, {x, 0, nz-1}) 
            + (Gx * _h / C) * matrix3d_get(prev_state->T, {x-1, 0, nz-1}) 
            + (Gx * _h / C) * matrix3d_get(prev_state->T, {x+1, 0, nz-1}) 
            + (Gy * _h / C) * matrix3d_get(prev_state->T, {x, 1, nz-1}) 
            + (Gz * _h / C) * matrix3d_get(prev_state->T, {x, 0, nz-2}) 
            + matrix2d_get(input->Qb, {x, 0})  / C *_h,
            state->T,
            {x, 0, nz-1});

        matrix3d_set((1.0 - (2 * Gx + Gy + Gz) * _h / C) * matrix3d_get(prev_state->T, {x, ny-1, nz-1}) 
            + (Gx * _h / C) * matrix3d_get(prev_state->T, {x-1, ny-1, nz-1}) 
            + (Gx * _h / C) * matrix3d_get(prev_state->T, {x+1, ny-1, nz-1}) 
            + (Gy * _h / C) * matrix3d_get(prev_state->T, {x, ny-2, nz-1}) 
            + (Gz * _h / C) * matrix3d_get(prev_state->T, {x, ny-1, nz-2}) 
            + matrix2d_get(input->Qb, {x, ny-1})  / C *_h,
            state->T,
            {x, ny-1, nz-1});
    }

    for(size_t y = 1; y < ny - 1; ++y)
    {
        matrix3d_set((1.0 - (Gx + 2 * Gy + 3 * Gz) * _h / C) * matrix3d_get(prev_state->T, {0, y, 0}) 
            + (Gx * _h / C) * matrix3d_get(prev_state->T, {1, y, 0}) 
            + (Gy * _h / C) * matrix3d_get(prev_state->T, {0, y-1, 0}) 
            + (Gy * _h / C) * matrix3d_get(prev_state->T, {0, y+1, 0}) 
            + (Gz * _h / C) * matrix3d_get(prev_state->T, {0, y, 1}) 
            + (2 * Gz / C) * input->Tt *_h,
            state->T,
            {0, y, 0});

        matrix3d_set((1.0 - (Gx + 2 * Gy + 3 * Gz) * _h / C) * matrix3d_get(prev_state->T, {nx-1, y, 0}) 
            + (Gx * _h / C) * matrix3d_get(prev_state->T, {nx-2, y, 0}) 
            + (Gy * _h / C) * matrix3d_get(prev_state->T, {nx-1, y-1, 0}) 
            + (Gy * _h / C) * matrix3d_get(prev_state->T, {nx-1, y+1, 0}) 
            + (Gz * _h / C) * matrix3d_get(prev_state->T, {nx-1, y, 1}) 
            + (2 * Gz / C) * input->Tt *_h,
            state->T,
            {nx-1, y, 0});

        matrix3d_set((1.0 - (Gx + 2 * Gy + Gz) * _h / C) * matrix3d_get(prev_state->T, {0, y, nz-1}) 
            + (Gx * _h / C) * matrix3d_get(prev_state->T, {1, y, nz-1}) 
            + (Gy * _h / C) * matrix3d_get(prev_state->T, {0, y-1, nz-1}) 
            + (Gy * _h / C) * matrix3d_get(prev_state->T, {0, y+1, nz-1}) 
            + (Gz * _h / C) * matrix3d_get(prev_state->T, {0, y, nz-2}) 
            + matrix2d_get(input->Qb, {0, y})  / C *_h,
            state->T,
            {0, y, nz-1});

        matrix3d_set((1.0 - (Gx + 2 * Gy + Gz) * _h / C) * matrix3d_get(prev_state->T, {nx-1, y, nz-1}) 
            + (Gx * _h / C) * matrix3d_get(prev_state->T, {nx-2, y, nz-1}) 
            + (Gy * _h / C) * matrix3d_get(prev_state->T, {nx-1, y-1, nz-1}) 
            + (Gy * _h / C) * matrix3d_get(prev_state->T, {nx-1, y+1, nz-1}) 
            + (Gz * _h / C) * matrix3d_get(prev_state->T, {nx-1, y, nz-2}) 
            + matrix2d_get(input->Qb, {nx-1, y})  / C *_h,
            state->T,
            {nx-1, y, nz-1});
    }

    for(size_t z = 1; z < nz - 1; ++z)
    {
        matrix3d_set((1.0 - (Gx + Gy + 2 * Gz) * _h / C) * matrix3d_get(prev_state->T, {0, 0, z}) 
            + (Gx * _h / C) * matrix3d_get(prev_state->T, {1, 0, z}) 
            + (Gy * _h / C) * matrix3d_get(prev_state->T, {0, 1, z}) 
            + (Gz * _h / C) * matrix3d_get(prev_state->T, {0, 0, z-1}) 
            + (Gz * _h / C) * matrix3d_get(prev_state->T, {0, 0, z+1}),
            state->T,
            {0,0,z});

        matrix3d_set((1.0 - (Gx + Gy + 2 * Gz) * _h / C) * matrix3d_get(prev_state->T, {nx-1, 0, z}) 
            + (Gx * _h / C) * matrix3d_get(prev_state->T, {nx-2, 0, z}) 
            + (Gy * _h / C) * matrix3d_get(prev_state->T, {nx-1, 1, z}) 
            + (Gz * _h / C) * matrix3d_get(prev_state->T, {nx-1, 0, z-1}) 
            + (Gz * _h / C) * matrix3d_get(prev_state->T, {nx-1, 0, z+1}),
            state->T,
            {nx-1, 0, z});

        matrix3d_set((1.0 - (Gx + Gy + 2 * Gz) * _h / C) * matrix3d_get(prev_state->T, {0, ny-1, z}) 
            + (Gx * _h / C) * matrix3d_get(prev_state->T, {1, ny-1, z}) 
            + (Gy * _h / C) * matrix3d_get(prev_state->T, {0, ny-2, z}) 
            + (Gz * _h / C) * matrix3d_get(prev_state->T, {0, ny-1, z-1}) 
            + (Gz * _h / C) * matrix3d_get(prev_state->T, {0, ny-1, z+1}),
            state->T,
            {0, ny-1, z});

        matrix3d_set((1.0 - (Gx + Gy + 2 * Gz) * _h / C) * matrix3d_get(prev_state->T, {nx-1, ny-1, z}) 
            + (Gx * _h / C) * matrix3d_get(prev_state->T, {nx-2, ny-1, z}) 
            + (Gy * _h / C) * matrix3d_get(prev_state->T, {nx-1, ny-2, z}) 
            + (Gz * _h / C) * matrix3d_get(prev_state->T, {nx-1, ny-1, z-1}) 
            + (Gz * _h / C) * matrix3d_get(prev_state->T, {nx-1, ny-1, z+1}),
            state->T,
            {nx-1, ny-1, z});
    }
    // end edges

    // faces
    for(size_t x = 1; x < nx-1; ++x)
    {
        for(size_t y = 1; y < ny-1; ++y)
        {
            matrix3d_set((1.0 - (2 * Gx + 2 * Gy + 3 * Gz) * _h / C) * matrix3d_get(prev_state->T, {x, y, 0}) 
                + (Gx * _h / C) * matrix3d_get(prev_state->T, {x-1, y, 0}) 
                + (Gx * _h / C) * matrix3d_get(prev_state->T, {x+1, y, 0}) 
                + (Gy * _h / C) * matrix3d_get(prev_state->T, {x, y-1, 0}) 
                + (Gy * _h / C) * matrix3d_get(prev_state->T, {x, y+1, 0}) 
                + (Gz * _h / C) * matrix3d_get(prev_state->T, {x, y, 1}) 
                + (2 * Gz / C) * input->Tt *_h,
                state->T,
                {x, y, 0});

            matrix3d_set((1.0 - (2 * Gx + 2 * Gy + Gz) * _h / C) * matrix3d_get(prev_state->T, {x, y, nz-1}) 
                + (Gx * _h / C) * matrix3d_get(prev_state->T, {x-1, y, nz-1}) 
                + (Gx * _h / C) * matrix3d_get(prev_state->T, {x+1, y, nz-1}) 
                + (Gy * _h / C) * matrix3d_get(prev_state->T, {x, y-1, nz-1}) 
                + (Gy * _h / C) * matrix3d_get(prev_state->T, {x, y+1, nz-1}) 
                + (Gz * _h / C) * matrix3d_get(prev_state->T, {x, y, nz-2}) 
                + matrix2d_get(input->Qb, {x, y})  / C *_h,
                state->T,
                {x, y, nz-1});
        }
    }

    for(size_t x = 1; x < nx-1; ++x)
    {
        for(size_t z = 1; z < nz-1; ++z)
        {
            matrix3d_set((1.0 - (2 * Gx + Gy + 2 * Gz) * _h / C) * matrix3d_get(prev_state->T, {x, 0, z}) 
                + (Gx * _h / C) * matrix3d_get(prev_state->T, {x-1, 0, z}) 
                + (Gx * _h / C) * matrix3d_get(prev_state->T, {x+1, 0, z}) 
                + (Gy * _h / C) * matrix3d_get(prev_state->T, {x, 1, z}) 
                + (Gz * _h / C) * matrix3d_get(prev_state->T, {x, 0, z-1}) 
                + (Gz * _h / C) * matrix3d_get(prev_state->T, {x, 0, z+1}),
                state->T,
                {x, 0, z});

            matrix3d_set((1.0 - (2 * Gx + Gy + 2 * Gz) * _h / C) * matrix3d_get(prev_state->T, {x, ny-1, z}) 
                + (Gx * _h / C) * matrix3d_get(prev_state->T, {x-1, ny-1, z}) 
                + (Gx * _h / C) * matrix3d_get(prev_state->T, {x+1, ny-1, z}) 
                + (Gy * _h / C) * matrix3d_get(prev_state->T, {x, ny-2, z}) 
                + (Gz * _h / C) * matrix3d_get(prev_state->T, {x, ny-1, z-1}) 
                + (Gz * _h / C) * matrix3d_get(prev_state->T, {x, ny-1, z+1}),
                state->T,
                {x, ny-1, z});
        }
    }

    for(size_t y = 1; y < ny-1; ++y)
    {
        for(size_t z = 1; z < nz-1; ++z)
        {
            matrix3d_set((1.0 - (Gx + 2 * Gy + 2 * Gz) * _h / C) * matrix3d_get(prev_state->T, {0, y, z}) 
                + (Gx * _h / C) * matrix3d_get(prev_state->T, {1, y, z}) 
                + (Gy * _h / C) * matrix3d_get(prev_state->T, {0, y-1, z}) 
                + (Gy * _h / C) * matrix3d_get(prev_state->T, {0, y+1, z}) 
                + (Gz * _h / C) * matrix3d_get(prev_state->T, {0, y, z-1}) 
                + (Gz * _h / C) * matrix3d_get(prev_state->T, {0, y, z+1}),
                state->T,
                {0, y, z});

            matrix3d_set((1.0 - (Gx + 2 * Gy + 2 * Gz) * _h / C) * matrix3d_get(prev_state->T, {nx-1, y, z}) 
                + (Gx * _h / C) * matrix3d_get(prev_state->T, {nx-2, y, z}) 
                + (Gy * _h / C) * matrix3d_get(prev_state->T, {nx-1, y-1, z}) 
                + (Gy * _h / C) * matrix3d_get(prev_state->T, {nx-1, y+1, z}) 
                + (Gz * _h / C) * matrix3d_get(prev_state->T, {nx-1, y, z-1}) 
                + (Gz * _h / C) * matrix3d_get(prev_state->T, {nx-1, y, z+1}),
                state->T,
                {nx-1, y, z});
        }
    }
    // end faces

    // internal volume
    for(size_t x = 1; x < nx-1; ++x)
    {
        for(size_t y = 1; y < ny-1; ++y)
        {
            for(size_t z = 1; z < nz-1; ++z)
            {
                matrix3d_set((1.0 - (2 * Gx + 2 * Gy + 2 * Gz) * _h / C) * matrix3d_get(prev_state->T, {x, y, z}) 
                    + (Gx * _h / C) * matrix3d_get(prev_state->T, {x-1, y, z}) 
                    + (Gx * _h / C) * matrix3d_get(prev_state->T, {x+1, y, z}) 
                    + (Gy * _h / C) * matrix3d_get(prev_state->T, {x, y-1, z}) 
                    + (Gy * _h / C) * matrix3d_get(prev_state->T, {x, y+1, z}) 
                    + (Gz * _h / C) * matrix3d_get(prev_state->T, {x, y, z-1}) 
                    + (Gz * _h / C) * matrix3d_get(prev_state->T, {x, y, z+1}),
                    state->T,
                    {x, y, z});
            }
        }
    }
    // end internal volume
}
