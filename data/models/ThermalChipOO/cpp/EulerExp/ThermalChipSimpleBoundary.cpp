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

#include "ThermalChipSimpleBoundary.hpp"
#include <iostream>

using namespace std;

ThermalChipSimpleBoundaryEulerExp::ThermalChipSimpleBoundaryEulerExp(size_t nx, size_t ny, size_t nz, real_t stop_time, int num_intervals)
    : BaseThermalChipEulerExp(nx, ny, nz, stop_time, num_intervals)
{
    cout<<"Running simulation with parameters:\n"
        <<"model=ThermalChipSimpleBoundaryEulerExp nx="<<nx<<" ny="<<ny<<" nz="<<nz
        <<" stop_time="<<stop_time<<" num_intervals="<<num_intervals<<endl;

    outputDefinition();
    initializationProblem();
}

void ThermalChipSimpleBoundaryEulerExp::initializationProblem()
{
    for(size_t i=0; i<nx; i++)
    {
        for(size_t j=0; j<ny/2; j++)
        {
            matrix2d_set(Pv, input->Qb, {i,j});
        }

        for(size_t j=ny/2; j<ny; j++)
        {
            matrix2d_set(0, input->Qb, {i,j});
        }
    }
}

void ThermalChipSimpleBoundaryEulerExp::outputDefinition()
{
    output.T.push_back({0,0,0});
    output.T.push_back({0,0,(nz-1)/2});
    output.T.push_back({0,0,nz-1});
    output.T.push_back({0,ny-1,0});
    output.T.push_back({0,ny-1,(nz-1)/2});
    output.T.push_back({0,ny-1,nz-1});
    output.T.push_back({(nx-1)/2,(ny-1)/2,0});
    output.T.push_back({(nx-1)/2,(ny-1)/2,(nz-1)/2});
    output.T.push_back({(nx-1)/2,(ny-1)/2,nz-1});
    output.T.push_back({nx-1,0,0});
    output.T.push_back({nx-1,0,(nz-1)/2});
    output.T.push_back({nx-1,0,nz-1});
    output.T.push_back({nx-1,ny-1,0});
    output.T.push_back({nx-1,ny-1,(nz-1)/2});
    output.T.push_back({nx-1,ny-1,nz-1});
}
