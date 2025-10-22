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

#include "matrix.hpp"

matrix2d_t matrix2d_create(point2d_t dims)
{
    size_t size = dims.x * dims.y;
    matrix2d_t result;
    result.data = new real_t[size];
    result.dims = dims;
    return result;
}

void matrix2d_destroy(matrix2d_t matrix)
{
    delete[] matrix.data;
}

matrix3d_t matrix3d_create(point3d_t dims)
{
    size_t size = dims.x * dims.y * dims.z;
    matrix3d_t result;
    result.data = new real_t[size];
    result.dims = dims;
    return result;
}

void matrix3d_destroy(matrix3d_t matrix)
{
    delete[] matrix.data;
}

matrix4d_t matrix4d_create(point4d_t dims)
{
    size_t size = dims.x * dims.y * dims.z * dims.t;
    matrix4d_t result;
    result.data = new real_t[size];
    result.dims = dims;
    return result;
}

void matrix4d_destroy(matrix4d_t matrix)
{
    delete[] matrix.data;
}
