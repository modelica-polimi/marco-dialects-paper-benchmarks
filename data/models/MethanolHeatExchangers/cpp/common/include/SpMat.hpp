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

#include "types.hpp"
#include "SpMat_Eigen.hpp"
#include "SpMat_KLU.hpp"
#include <cmath>

#if !defined(MAT_LIB_TYPE_EIGEN) && !defined(MAT_LIB_TYPE_KLU)
#define MAT_LIB_TYPE_KLU
#endif

#if defined(MAT_LIB_TYPE_EIGEN)
using Triplet = eigen_sp_mat::Triplet;
using SpMat = eigen_sp_mat::SpMat;
using InputVector = eigen_sp_mat::Vector;
using StateVector = eigen_sp_mat::Vector;
using Solver = eigen_sp_mat::Solver;
#elif defined(MAT_LIB_TYPE_KLU)
using Triplet = klu_sp_mat::Triplet;
using SpMat = klu_sp_mat::SpMat;
using InputVector = klu_sp_mat::Vector;
using StateVector = klu_sp_mat::Vector;
using Solver = klu_sp_mat::Solver;
#else
#error "Unspecified sparse matrix factorization library"
#endif
