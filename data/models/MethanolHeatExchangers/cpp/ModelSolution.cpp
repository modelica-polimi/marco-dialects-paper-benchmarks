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

#include "ModelSolutionI.hpp"
#include "EulerExp/MethanolHeatExchangers.hpp"
#include "OptionsMap.hpp"

#include <iostream>
#include <string>

using namespace std;

ModelSolutionI* ModelSolutionI::create(int argc, char *argv[])
{
    OptionsMap opts(argc,argv);
    
    size_t Nu = opts.has("Nu") ? stoi(opts.get("Nu")) : 3;
    size_t Nh = opts.has("Nh") ? stoi(opts.get("Nh")) : 4;
    size_t Nv = opts.has("Nv") ? stoi(opts.get("Nv")) : 6;
    double stop_time = opts.has("stop_time") ? stod(opts.get("stop_time")) : 100.0;
    int num_intervals = opts.has("num_intervals") ? stoi(opts.get("num_intervals")) : 10000;
    
    cout<<"Running simulation with parameters:\nNu="<<Nu<<" Nh="<<Nh<<" Nv="
        <<Nv<<" stop_time="<<stop_time<<" num_intervals="<<num_intervals<<endl;
    
    return new MethanolHeatExchangersEulerExp(Nu, Nh, Nv, stop_time, num_intervals);
}
