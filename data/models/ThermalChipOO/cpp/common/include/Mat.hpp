/***************************************************************************
 *   Copyright (C) 2019-2022 by Politecnico di Milano                      *
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
#include <cmath>
#include <cstdint>
#include <algorithm>
#include <utility>


/** 2D and 3D dense matrices */


template <typename ElemT>
class Mat2D
{
public:
    Mat2D(size_t x_sz, size_t y_sz) : x_sz(x_sz), y_sz(y_sz)
    {
        buffer = new ElemT[x_sz*y_sz];
    }
    Mat2D(Mat2D<ElemT>& other)
    {
        x_sz = other.x_sz;
        y_sz = other.y_sz;
        buffer = new ElemT[x_sz*y_sz];
        std::copy(other.begin(), other.end(), buffer);
    }

    ~Mat2D()
    {
        delete buffer;
    }

    inline size_t xSize() const { return x_sz; }
    inline size_t ySize() const { return y_sz; }

    inline ElemT *begin() { return buffer; }
    inline ElemT *end() { return buffer + x_sz * y_sz; }

    inline ElemT& operator()(size_t x, size_t y)
    {
        return buffer[x*y_sz + y];
    }

    Mat2D<ElemT>& operator=(const Mat2D<ElemT>& other)
    {
        if (this == &other)
            return *this;
        delete buffer;
        x_sz = other.x_sz;
        y_sz = other.y_sz;
        buffer = new ElemT[x_sz*y_sz];
        std::copy(other.begin(), other.end(), buffer);
        return *this;
    }

    Mat2D<ElemT>& operator=(const Mat2D<ElemT>&& other)
    {
        if (this == &other)
            return *this;
        delete buffer;
        x_sz = std::exchange(other.x_sz, 0);
        y_sz = std::exchange(other.y_sz, 0);
        buffer = std::exchange(other.buffer, nullptr);
        return *this;
    }

private:
    ElemT *buffer;
    size_t x_sz, y_sz;
};


template <typename ElemT>
class Mat3D
{
public:
    Mat3D(size_t x_sz, size_t y_sz, size_t z_sz)
        : x_sz(x_sz), y_sz(y_sz), z_sz(z_sz)
    {
        buffer = new ElemT[x_sz*y_sz*z_sz];
    }
    Mat3D(Mat2D<ElemT>& other)
    {
        x_sz = other.x_sz;
        y_sz = other.y_sz;
        z_sz = other.z_sz;
        buffer = new ElemT[x_sz*y_sz*z_sz];
        std::copy(other.begin(), other.end(), buffer);
    }

    ~Mat3D()
    {
        delete buffer;
    }

    inline size_t xSize() const { return x_sz; }
    inline size_t ySize() const { return y_sz; }
    inline size_t zSize() const { return z_sz; }

    inline ElemT *begin() { return buffer; }
    inline ElemT *end() { return buffer + x_sz * y_sz * z_sz; }

    inline ElemT& operator()(size_t x, size_t y, size_t z)
    {
        return buffer[x*y_sz*z_sz + y*z_sz + z];
    }

    Mat3D<ElemT>& operator=(const Mat3D<ElemT>& other)
    {
        if (this == &other)
            return *this;
        delete buffer;
        x_sz = other.x_sz;
        y_sz = other.y_sz;
        z_sz = other.z_sz;
        buffer = new ElemT[x_sz*y_sz*z_sz];
        std::copy(other.begin(), other.end(), buffer);
        return *this;
    }

    Mat3D<ElemT>& operator=(const Mat3D<ElemT>&& other)
    {
        if (this == &other)
            return *this;
        delete buffer;
        x_sz = std::exchange(other.x_sz, 0);
        y_sz = std::exchange(other.y_sz, 0);
        z_sz = std::exchange(other.z_sz, 0);
        buffer = std::exchange(other.buffer, nullptr);
        return *this;
    }

private:
    ElemT *buffer;
    size_t x_sz, y_sz, z_sz;
};


