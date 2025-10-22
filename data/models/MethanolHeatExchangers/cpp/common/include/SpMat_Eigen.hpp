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
#include "TimeStatAccumulator.hpp"
#include <Eigen/Sparse>
#include <string>

namespace eigen_sp_mat
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
    inline SpMat(const size_t size);  // square sparse matrix

    inline SpMat(const size_t size1, const size_t size2); // rectangular sparse matrix

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

    inline real_t getElem(const size_t row, const size_t col) const
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
};

class Solver
{
public:
    inline Solver() {};

    inline void setMatrix(SpMat* sparse_matrix) { mat = sparse_matrix; }

    inline Vector solve(const Vector& vec);

    double analysisTime() { return tAnalysis.time(); }
    double factorizationTime() { return tFactorization.time(); }
    double solvingTime() { return tSolving.time(); }
private:
    SpMat* mat;
    Eigen::SparseLU<SpMat::matrix_impl> info;
    TimeStatAccumulator tAnalysis;
    TimeStatAccumulator tFactorization;
    TimeStatAccumulator tSolving;
};

SpMat::SpMat(const size_t size) : info (size, size) {}

SpMat::SpMat(const size_t size1, const size_t size2) : info (size1, size2) {}

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

Vector Solver::solve(const Vector& vec)
{
    if (!mat)
        throw std::runtime_error("called Solver::solve() without setting the matrix");

    if (mat->hasStructureChanged()) {
        TimeStatScope timer(tAnalysis);
        // Eigen docs say:
        // "The input matrix A should be in a compressed and column-major form.
        // Otherwise an expensive copy will be made."
        mat->makeCompressed();
        info.analyzePattern(mat->info);
    }
    
    if (mat->hasStructureChanged() || mat->hasValueChanged()) {
        TimeStatScope timer(tFactorization);
        info.factorize(mat->info);
        if (info.info() != Eigen::ComputationInfo::Success)
            throw std::runtime_error("factorization failed");
    }

    mat->resetFlagStructure();
    mat->resetFlagValues();
    Vector tmp(vec.size());

    {
        TimeStatScope timer(tSolving);
        tmp.impl = info.solve(vec.impl);
    }

    // check errors
    if (info.info() != Eigen::ComputationInfo::Success)
        throw std::runtime_error("solve failed");
    return tmp;
}

}
