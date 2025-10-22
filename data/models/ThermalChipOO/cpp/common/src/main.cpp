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

#include "ModelSolutionI.hpp"
#include "CsvEmitter.hpp"
#include "TimeStatAccumulator.hpp"
#include "string"

#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    TimeStatAccumulator statSim, statEmit, statTotal;
    {
        TimeStatScope ts(statTotal);
        if (argc < 2)
        {
            cerr << "first parameter must be csv file" << endl;
            return 2;
        }

        ModelSolutionI * model_sol = ModelSolutionI::create(argc, argv);
        const string filename = argv[1];
        std::remove(filename.c_str());
        const string csv_separator = ",";
        Emitter<CSV_emitter>* csv_emit;
        csv_emit = new Emitter<CSV_emitter>(filename, csv_separator);

        model_sol->emitDataTo(csv_emit);

        auto doStep=[&](ModelSolutionI *ms)->bool
        {
            TimeStatScope ts(statSim);
            return ms->step();
        };

        while(doStep(model_sol))
        {
            TimeStatScope ts(statEmit);
            model_sol->emitDataTo(csv_emit);
        }

        TimeStatScope tsemit(statEmit);
        model_sol->emitDataTo(csv_emit);

        delete csv_emit;
        delete model_sol;
    }

    double total=statTotal.time(), sim=statSim.time(), emit=statEmit.time();
    cout << "Total time          " << total << " s" << endl
         << "Simulation time     " << sim << " s" << endl
         << "Output writing time " << emit << " s" << endl
         << "Overhead            " << total-sim-emit << " s" << endl;

    return 0;
}
