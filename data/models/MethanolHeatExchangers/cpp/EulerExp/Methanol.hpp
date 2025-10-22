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
#include "types.hpp"

#include <vector>
#include <cmath>
#include <iostream>
#include <iomanip>
#include <cassert>

namespace Methanol
{
    // model parameters
    const static real_t R = 8.314462 / 32.04e-3;
    const static real_t Tc = 512.64;
    const static std::vector<real_t> f = {3.90086, 10.9929, 18.3371, -16.3663, -6.22334, 2.80358, 1.07783, 0.96967};
    const static std::vector<real_t> g = {0.0, 4.12575, 3.26973, 3.77492, 2.93574, 8.23747, 10.3312, 0.53326};

    // model functions
    static real_t cp_T(real_t T)
    {
        real_t cp, tau, x;
        real_t u[8], exp_u[8];

        tau = Tc / T;
        x = f[0];

        // algorithm
        for (size_t i = 1; i < f.size(); i++)
        {
            u[i] = g[i] * tau;
            exp_u[i] = exp(u[i]);
            x += f[i] * u[i]*u[i] * exp_u[i] / ((exp_u[i] - 1)*(exp_u[i] - 1));
        }
        cp = x * R;

        return cp;
    }

    static real_t cv_T(real_t T)
    {
        real_t cv;

        // algorithm
        cv = cp_T(T) - R;

        return cv;
    }

    static real_t h_T(real_t T)
    {
        real_t h, tau, x;
        real_t u[8];

        tau = Tc / T;
        x = f[0] / tau;

        // algorithm
        for (size_t i = 1; i < f.size(); i++)
        {
            u[i] = g[i] * tau;
            x += f[i] * g[i] / (exp(u[i]) - 1);
        }
        h = R * T * tau * x - 1361.810 * tau / Tc;

        return h;
    }

    void Test()
    {
        real_t Tone, T, h, cp, cp_check, prev_h, prev_T;

        Tone = 1;
        T = prev_T = 500 + 200 * 0 / Tone;
        h = prev_h = h_T(T);
        cp = cp_T(T);

        for (real_t time = 0.01; time <= 100.00; time += 0.01)
        {
            T = 500 + 200 * time / Tone;
            h = h_T(T);
            cp = cp_T(T);
            cp_check = (h - prev_h) / (T - prev_T);
            prev_h = h;
            prev_T = T;

            std::cout << std::setprecision(2) << std::fixed;
            std::cout << "Timestep: " << time << ", ";
            std::cout << std::scientific;
            std::cout << "cp: " << cp << ", ";
            std::cout << "cp_check: " << cp_check << ", ";
            std::cout << "error: " << cp - cp_check << std::endl;
        }
        assert(cp - cp_check < 1e-003);
    }
}
