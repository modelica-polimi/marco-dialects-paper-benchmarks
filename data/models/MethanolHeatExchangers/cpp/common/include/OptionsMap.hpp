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

#pragma once

#include <string>
#include <map>
#include <stdexcept>

/**
 * Ignore options starting with -, and put in a map all options of the
 * type key=value
 */
class OptionsMap
{
public:
    OptionsMap(int argc, char *argv[]);
    
    bool has(const std::string& opt) const
    {
        return options.find(opt) != options.end();
    }
    
    std::string get(const std::string& opt) const
    {
        auto it = options.find(opt);
        if(it == options.end())
            throw std::runtime_error(std::string("Missing option: ")+opt);
        return it->second;
    }
    
private:
    std::map<std::string,std::string> options;
};
