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

#include <cstddef>
#include <cmath>

// #define USE_FLOAT_AS_REAL

#ifdef USE_FLOAT_AS_REAL
using real_t = float;
#else
using real_t = double;
#endif

using index_t = std::size_t;

#ifdef USE_NARROW_STATE_INDEX
using s_index_t = int;
#else
using s_index_t = long int;
#endif

//provide a power function that works whether real_t is double or float
inline float hipermod_power(float base, float expon) { return powf(base, expon); }
inline double hipermod_power(double base, double expon) { return pow(base, expon); }

// re0define std::accumulate to solve the sum at compile time
template<typename tpl_sum_t, size_t N>
inline constexpr tpl_sum_t hipermod_sum(tpl_sum_t const (&A)[N])
{
    tpl_sum_t sum(tpl_sum_t(0));
    for(size_t i = 0; i < N; ++i)
    {
        sum += A[i];
    }
    return sum;
}
