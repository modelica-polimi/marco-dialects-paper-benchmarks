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

#include <iostream>
#include <string>
#include <map>
#include <functional>
#include "ModelSolutionI.hpp"
#include "OptionsMap.hpp"
#include "EulerExp/ThermalChipSimpleBoundary.hpp"

using namespace std;

ModelSolutionI* ModelSolutionI::create(int argc, char *argv[])
{
    OptionsMap opts(argc,argv);
    
    string model        = opts.has("model")           ? opts.get("model")                 : "";
    size_t nx           = opts.has("nx")              ? stoi(opts.get("nx"))              : 4;
    size_t ny           = opts.has("ny")              ? stoi(opts.get("ny"))              : 4;
    size_t nz           = opts.has("nz")              ? stoi(opts.get("nz"))              : 4;
    size_t Nr           = opts.has("Nr")              ? stoi(opts.get("Nr"))              : 1;
    size_t Pr           = opts.has("Pr")              ? stoi(opts.get("Pr"))              : 1;
    real_t stop_time    = opts.has("stop_time")       ? stod(opts.get("stop_time"))       : 0.01;
    int num_intervals   = opts.has("num_intervals")   ? stoi(opts.get("num_intervals"))   : 5000;
    int equid_time_grid = opts.has("equid_time_grid") ? stoi(opts.get("equid_time_grid")) : 0;
    real_t tol          = opts.has("tol")             ? stod(opts.get("tol"))             : 1e-12;
    
    map<string,function<ModelSolutionI* ()>> models=
    {
        { "ThermalChipSimpleBoundaryEulerExp", [=](){ return new ThermalChipSimpleBoundaryEulerExp(nx, ny, nz, stop_time, num_intervals); } }
    };
    
    auto it = models.find(model);
    if(it == models.end())
    {
        cerr << "Unsupported or missing model=<model name> parameter" << endl;
        cerr << "Options are:" << endl;
        for(auto m : models) cerr << "- " << m.first << endl;
        exit(1);
    }

    return it->second();
}
