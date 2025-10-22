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
#include "BaseThermalChip.hpp"

class ThermalChipSimpleBoundaryEulerExp: public BaseThermalChipEulerExp
{
public:

    ThermalChipSimpleBoundaryEulerExp(size_t nx, size_t ny, size_t nz, real_t stop_time, int num_intervals);

private:

    void outputDefinition();

    void initializationProblem();

    const real_t Ptot = 100;
    const real_t Pv = Ptot / (nx * ny / 2);
};
