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

#include <chrono>

/**
 * Instantiate a class of this type for each phase of execution for which
 * you want to keep time statistics
 */
class TimeStatAccumulator
{
public:

    TimeStatAccumulator() { reset(); }

    /**
     * Call this at the beginning of the phase you want to profile
     */
    void begin()
    {
        start = std::chrono::steady_clock::now();
    }

    /**
     * Call this at the end of the phase you want to profile
     */
    void end()
    {
        accumulatedTime += (std::chrono::steady_clock::now() - start);
    }

    /**
     * \return the sum of the time spent in each begin-end pair.
     */
    double time();

    /**
     * Reset the time accumulator
     */
    void reset();

    TimeStatAccumulator(const TimeStatAccumulator&)=delete;
    TimeStatAccumulator& operator=(const TimeStatAccumulator&)=delete;

private:
    std::chrono::steady_clock::time_point start;
    std::chrono::nanoseconds accumulatedTime;
};

/**
 * Scoped lock class that calls begin()/end() on an accumulator
 */
class TimeStatScope
{
public:

    TimeStatScope(TimeStatAccumulator& acc) : acc(acc) { acc.begin(); }

    ~TimeStatScope() { acc.end(); }

private:
    TimeStatAccumulator& acc;
};
