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

#include <list>
#include <cstddef>
#include "types.hpp"

struct point2d_t
{
    size_t x;
    size_t y;
};

struct point3d_t
{
    size_t x;
    size_t y;
    size_t z;
};

struct point4d_t
{
    size_t x;
    size_t y;
    size_t z;
    size_t t;
};

struct matrix2d_t
{
    real_t *data;
    point2d_t dims;
};

struct matrix3d_t
{
    real_t *data;
    point3d_t dims;
};

struct matrix4d_t
{
    real_t *data;
    point4d_t dims;
};

matrix2d_t matrix2d_create(point2d_t dims);

void matrix2d_destroy(matrix2d_t matrix);

inline real_t matrix2d_get(matrix2d_t matrix, point2d_t idxs)
{
    size_t idx = idxs.x * matrix.dims.x + idxs.y;
    return matrix.data[idx];
}

inline void matrix2d_set(real_t value, matrix2d_t matrix, point2d_t idxs)
{
    size_t idx = idxs.x * matrix.dims.y + idxs.y;
    matrix.data[idx] = value;
}

matrix3d_t matrix3d_create(point3d_t dims);

void matrix3d_destroy(matrix3d_t matrix);

inline real_t matrix3d_get(matrix3d_t matrix, point3d_t idxs)
{
    size_t idx = idxs.x * matrix.dims.y * matrix.dims.z 
        + idxs.y * matrix.dims.z 
        + idxs.z;
    return matrix.data[idx];
}

inline void matrix3d_set(real_t value, matrix3d_t matrix, point3d_t idxs)
{
    size_t idx = idxs.x * matrix.dims.y * matrix.dims.z 
        + idxs.y * matrix.dims.z 
        + idxs.z;
    matrix.data[idx] = value;
}

matrix4d_t matrix4d_create(point4d_t dims);

void matrix4d_destroy(matrix4d_t matrix);

inline real_t matrix4d_get(matrix4d_t matrix, point4d_t idxs)
{
    size_t idx = idxs.x * matrix.dims.y * matrix.dims.z * matrix.dims.t 
        + idxs.y * matrix.dims.z * matrix.dims.t 
        + idxs.z * matrix.dims.t
        + idxs.t;
    return matrix.data[idx];
}

inline void matrix4d_set(real_t value, matrix4d_t matrix, point4d_t idxs)
{
    size_t idx = idxs.x * matrix.dims.y * matrix.dims.z * matrix.dims.t 
        + idxs.y * matrix.dims.z * matrix.dims.t 
        + idxs.z * matrix.dims.t
        + idxs.t;
    matrix.data[idx] = value;
}
