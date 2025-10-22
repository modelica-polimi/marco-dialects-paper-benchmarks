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

#include <functional>

class Event
{
public:

    Event();

    Event(std::function<bool(double)> condition, std::function<void()> action);

    void evaluateCondition(double time);

private:
    std::function<bool(double)> condition;
    std::function<void()> action;
};

class WhenSample
{
public:

    WhenSample(double start, double step, double epsilon);

    bool hasToFire(double time);

private:
    double start;
    double step;
    double epsilon;
    unsigned int index = 0;
};

class WhenSampleEvent: public Event
{
public:

    WhenSampleEvent(double start, double step, double epsilon, std::function<void()> action);

private:
    WhenSample whenSample;
};
