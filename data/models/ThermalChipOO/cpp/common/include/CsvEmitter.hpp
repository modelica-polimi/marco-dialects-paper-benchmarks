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

#include "Emitter.hpp"
#include "types.hpp"

#include <fstream>
#include <string>

//TODO: remove template parameter and use a base class so as to support other emit formats
template<>
class Emitter<CSV_emitter>
{
public:
    Emitter(const std::string filename, const std::string separator)
    : ofs(filename, std::ofstream::out | std::ofstream::app), separator(separator)
    {
        ofs.precision(sizeof(real_t)==8 ? 15 : 7); //Adjust precision based on real type
    }

    template<typename T>
    void pushValue(const T& val) { ofs << val; }
    
    //TODO: Make separator pushing automatic and remove from API
    void pushSeparator() { ofs << separator; }

    void endline() { ofs << "\n"; }
    
    void setPrecision(int precision)
    {
        ofs.precision(precision);
    }

private:
    std::ofstream ofs;
    std::string separator;
};
