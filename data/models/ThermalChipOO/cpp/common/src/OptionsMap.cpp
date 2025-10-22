/***************************************************************************
 *   Copyright (C) 2022 by Politecnico di Milano                           *
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

#include "OptionsMap.hpp"
#include <iostream>

using namespace std;

OptionsMap::OptionsMap(int argc, char *argv[])
{
    for(int i=0;i<argc;i++)
    {
        string opt=argv[i];
        if(opt.empty() || opt.at(0)=='-') continue;
        auto eq=opt.find('=');
        if(eq==string::npos) continue;
        options[opt.substr(0,eq)]=opt.substr(eq+1);
    }
    //for(auto& e : options) cout<<e.first<<" "<<e.second<<endl;
}
