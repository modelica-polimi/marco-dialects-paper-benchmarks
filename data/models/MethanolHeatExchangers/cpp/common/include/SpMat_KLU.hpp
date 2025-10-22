/***************************************************************************
 *   Copyright (C) 2019-2023 by Politecnico di Milano                      *
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

#include "types.hpp"
#include <Eigen/Sparse>
#include <klu.h>
#include <string>
#include <iostream>

#if defined(MAT_LIB_TYPE_KLU) && defined(USE_FLOAT_AS_REAL)
#error "KLU only supports double precision"
#endif

namespace klu_sp_mat
{

class SpMat;
class Vector;
class Solver;
class Triplet;

class Vector
{
public:
    inline Vector(const size_t size) : impl(size) {}

    inline Vector(const Vector& rhs) : impl(rhs.impl) {}

    inline size_t size() const
    {
        return impl.rows();
    }

    inline void setElem(const size_t pos, const real_t value)
    {
        impl[pos] = value;
    }

    inline real_t getElem(const size_t pos) const
    {
        return impl[pos];
    }

    inline Vector operator+(const Vector& rhs)
    {
        Vector tmp(impl.size());
        tmp.impl = impl + rhs.impl;
        return tmp;
    }

private:
    friend class SpMat;
    friend class Solver;
    Eigen::Matrix<real_t, Eigen::Dynamic, 1> impl;
};

class Triplet
{
public:
    Triplet(size_t x, size_t y, real_t v) : t(x, y, v) {}
private:
    template<class InputIt> friend class TripletIteratorConverter;
    Eigen::Triplet<real_t,s_index_t> t;
};

class SpMat
{
public:
    // square sparse matrix
    inline SpMat(const size_t size) : info(size, size) {}

    // rectangular sparse matrix
    inline SpMat(const size_t nrow, const size_t ncol) : info(nrow, ncol) {}

    template<class InputIt>
    inline void setFromTriplets(const InputIt& begin, const InputIt& end);

    inline size_t rows() const
    {
        return info.rows();
    }

    inline size_t cols() const
    {
        return info.cols();
    }

    inline void setElem(const size_t r, const size_t c, const real_t v)
    {
        info.coeffRef(r, c) = v;
        valueChange = true;
    }

    inline real_t getElem(const size_t row, const size_t col)
    {
        return info.coeff(row, col);
    }

    inline Vector operator*(const Vector& rhs)
    {
        Vector tmp(rhs.impl.size());
        tmp.impl = info * rhs.impl;
        return tmp;
    }

private:
    friend class Solver;

    using matrix_impl = Eigen::SparseMatrix<real_t,Eigen::StorageOptions::ColMajor,s_index_t>;
    matrix_impl info;
    bool valueChange;
    bool nnzChange;

    inline bool hasValueChanged() const { return valueChange; }
    inline bool hasStructureChanged() const { return nnzChange; }

    inline void resetFlagValues() { valueChange = false; }
    inline void resetFlagStructure() { nnzChange = false; }

    inline void makeCompressed() { info.makeCompressed(); }
    
    inline const real_t *getXArray() const { return info.valuePtr(); }
    inline const s_index_t *getIArray() const { return info.innerIndexPtr(); }
    inline const s_index_t *getPArray() const { return info.outerIndexPtr(); }
};

class Solver
{
public:
    inline Solver();

    inline void setMatrix(SpMat* sparse_matrix)
    {
        if (sparse_matrix->rows() != sparse_matrix->cols())
            throw std::runtime_error("KLU requires A matrix to be square");
        mat = sparse_matrix;
    }

    inline Vector solve(const Vector& bVector);

    double analysisTime() { return tAnalysis.time(); }
    double factorizationTime() { return tFactorization.time(); }
    double solvingTime() { return tSolving.time(); }
private:
    SpMat* mat;
    #ifdef USE_NARROW_STATE_INDEX
    klu_common common;
    klu_symbolic *symbolic;
    klu_numeric *numeric;
    #else
    klu_l_common common;
    klu_l_symbolic *symbolic = nullptr;
    klu_l_numeric *numeric = nullptr;
    #endif
    TimeStatAccumulator tAnalysis;
    TimeStatAccumulator tFactorization;
    TimeStatAccumulator tSolving;
};

// Eigen wants an iterator over its own Triplet type, while we have an
// iterator over our own Triplet which wraps around it. This class resolves the
// problem using template magic
template <class InputIt>
class TripletIteratorConverter: public InputIt
{
public:
    TripletIteratorConverter(InputIt it) : InputIt(it) {}
    Eigen::Triplet<real_t,s_index_t>& operator*() const
    {
        return InputIt::operator*()->t;
    }
    Eigen::Triplet<real_t,s_index_t> *operator->() const
    {
        return &((InputIt::operator->())->t);
    }
    Eigen::Triplet<real_t,s_index_t>& operator[](const size_t& n) const
    {
        return (InputIt::operator[](n))->t;
    }
};

template<class InputIt>
void SpMat::setFromTriplets(const InputIt& begin, const InputIt& end)
{
    info.setFromTriplets(
        TripletIteratorConverter<InputIt>(begin), 
        TripletIteratorConverter<InputIt>(end));
    valueChange = true;
    nnzChange = true;
    return;
}

Solver::Solver()
{
    #ifdef USE_NARROW_STATE_INDEX
    klu_defaults(&common);
    #else
    klu_l_defaults(&common);
    #endif
}

Vector Solver::solve(const Vector& bVector)
{
    if (!mat)
        throw std::runtime_error("called Solver::solve() without setting the matrix");

    double *ax = const_cast<double *>(mat->getXArray());
    #ifdef USE_NARROW_STATE_INDEX
    int n = mat->rows();
    int *ap = const_cast<int *>(mat->getPArray());
    int *ai = const_cast<int *>(mat->getIArray());
    #else
    SuiteSparse_long n = mat->rows();
    SuiteSparse_long *ap = const_cast<SuiteSparse_long *>(mat->getPArray());
    SuiteSparse_long *ai = const_cast<SuiteSparse_long *>(mat->getIArray());
    #endif

    if (!symbolic || mat->hasStructureChanged()) {
        TimeStatScope timer(tAnalysis);
        // Eigen docs say:
        // "The input matrix A should be in a compressed and column-major form.
        // Otherwise an expensive copy will be made."
        mat->makeCompressed();

        #ifdef USE_NARROW_STATE_INDEX
        if (symbolic)
            klu_free_symbolic(&symbolic, &common);
        symbolic = klu_analyze(n, ap, ai, &common);
        #else
        if (symbolic)
            klu_l_free_symbolic(&symbolic, &common);
        symbolic = klu_l_analyze(n, ap, ai, &common);
        #endif
        if (!symbolic)
            throw std::runtime_error("Solver::analyze() failed");
    }

    if (!numeric || mat->hasValueChanged() || mat->hasStructureChanged()) {
        TimeStatScope timer(tFactorization);
        bool ok;
        if (!numeric || mat->hasStructureChanged()) {
            #ifdef USE_NARROW_STATE_INDEX
            if (numeric)
                klu_free_numeric(&numeric, &common);
            numeric = klu_factor(ap, ai, ax, symbolic, &common);
            #else
            if (numeric)
                klu_l_free_numeric(&numeric, &common);
            numeric = klu_l_factor(ap, ai, ax, symbolic, &common);
            #endif
            ok = !!numeric;
        } else {
            #ifdef USE_NARROW_STATE_INDEX
            ok = !!klu_refactor(ap, ai, ax, symbolic, numeric, &common);
            #else
            ok = !!klu_l_refactor(ap, ai, ax, symbolic, numeric, &common);
            #endif
        }
        if (!ok)
            throw std::runtime_error("factorization failed");
    }

    mat->resetFlagStructure();
    mat->resetFlagValues();
    Vector tmp(bVector);

    bool ok;
    {
        TimeStatScope timer(tSolving);
        #ifdef USE_NARROW_STATE_INDEX
        ok = klu_solve(symbolic, numeric, n, 1, tmp.impl.data(), &common);
        #else
        ok = klu_l_solve(symbolic, numeric, n, 1, tmp.impl.data(), &common);
        #endif
    }
    if (!ok)
        throw std::runtime_error("solve failed");
    
    return tmp;
}

}
