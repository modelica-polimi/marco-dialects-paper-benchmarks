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

#include "Event.hpp"
#include <cstdio>

Event::Event() : condition([](double time){return false;}), action([](){}) {}

Event::Event(std::function<bool(double)> condition, std::function<void()> action)
    : condition(condition), action(action) {}

void Event::evaluateCondition(double time)
{
    if(condition(time))
        action();
}

WhenSample::WhenSample(double start, double step, double epsilon)
    : start(start), step(step), epsilon(epsilon) {}

bool WhenSample::hasToFire(double time)
{
    if(time > start+index*step-epsilon)
    {
        //printf("%f\n",time); //Print event firing times
        index=index+1;
        return true;
    }

    return false;
}

WhenSampleEvent::WhenSampleEvent(double start, double step, double epsilon, std::function<void()> action)
    : Event([=](double time){ return whenSample.hasToFire(time); }, action), whenSample(start, step, epsilon) {}
