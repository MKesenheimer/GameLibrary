/*
 *  vector.h
 *  Created by Matthias Kesenheimer on 19.06.22.
 *  Copyright 2022. All rights reserved.
 *  More information about the Eigen library at http://eigen.tuxfamily.org/dox/index.html
 */

#pragma once
#include "vector.h"
#include <Eigen/Dense>
#include <Eigen/StdVector>
#include <initializer_list>
#include <vector>
#include <atomic>

#ifdef _DEBUG
#include <iostream>
#endif

namespace math {
    /// <summary>
    /// matrix class
    /// Verknuepft die Daten gespeichert in m_data mit Eigen::matrix. Dadurch koennen Rechenoperationen einfacher und schneller durchgefuehrt werden.
    /// </summary>
    template <class _T> 
    class matrix {
    public:
        /// <summary>
        /// typedefs
        /// </summary>
        using data_type = std::vector<_T>;
        using value_type = typename data_type::value_type;
        using size_type = typename data_type::size_type;
        using reference = typename data_type::reference;
        using const_reference = typename data_type::const_reference;
        using const_iterator = typename data_type::const_iterator;
        using const_reverse_iterator = typename data_type::const_reverse_iterator;
        using reverse_iterator = typename data_type::reverse_iterator;
        using iterator = typename data_type::iterator;
        using eigen_type = Eigen::Matrix<_T, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>;
        using map_type = Eigen::Map<eigen_type>; // map the data structure to Eigen::matrix
        //using interface_item_type = InterfaceItemType_t<_T>;

        /// <summary>
        /// construct a dynamic-size matrix with size 'rows'x'cols'
        /// </summary>
        matrix(size_type r, size_type c)
            : m_cols(c), m_rows(r), m_data(r * c), m_eigen(data(), rows(), cols()), m_refCount(1) {}

        /// <summary>
        /// construct a dynamic-size matrix with number of rows 'rows' and default column vectors 'vector'
        /// </summary>
        matrix(size_type r, const data_type& v)
            : m_data(matrixFromVector(r, v)), m_eigen(data(), rows(), cols()), m_refCount(1) {}

        /// <summary>
        /// construct a dynamic-size matrix with number of rows 'rows' and default column vectors 'vector'
        /// </summary>
        matrix(size_type r, const vector<_T>& v)
            : m_data(matrixFromVector(r, v)), m_eigen(data(), rows(), cols()), m_refCount(1) {}

        /// <summary>
        /// construct a dynamic-size matrix with size 'cols * rows' and default value 'defaultValue'
        /// </summary>
        matrix(size_type r, size_type c, const value_type& defaultValue)
            : m_cols(c), m_rows(r), m_data(r * c, defaultValue), m_eigen(data(), rows(), cols()), m_refCount(1) {}

        /// <summary>
        /// construct a dynamic-size matrix from nested vector's
        /// </summary>
        matrix(const vector<vector<_T>>& matrix)
            : m_data(matrixFromVectors(matrix)), m_eigen(data(), rows(), cols()), m_refCount(1) {}

        /// <summary>
        /// construct a dynamic-size empty matrix
        /// </summary>
        matrix()
            : m_cols(0), m_rows(0), m_data(), m_eigen(data(), cols(), rows()), m_refCount(1) {}

        /// <summary>
        /// copy constructor
        /// </summary>
        matrix(const matrix& other)
            : m_cols(other.m_cols), m_rows(other.m_rows), m_data(other.m_data), m_eigen(data(), rows(), cols()), m_refCount(1) {}

        /// <summary>
        /// move constructor
        /// </summary>
        matrix(matrix&& other) noexcept
            : m_cols(other.m_cols), m_rows(other.m_rows), m_data(std::move(other.m_data)), m_eigen(data(), rows(), cols()), m_refCount(1) {}

        /// <summary>
        /// initialization by initializer list
        /// </summary>
        matrix(std::initializer_list<std::initializer_list<_T>> IList)
            : m_data(matrixFromVectors(IList)), m_eigen(data(), rows(), cols()), m_refCount(1) {}

        /// <summary>
        /// initialization by initializer list
        /// </summary>
        matrix(std::initializer_list<vector<_T>> IList)
            : m_data(matrixFromVectors(IList)), m_eigen(data(), rows(), cols()), m_refCount(1) {}

        /// <summary>
        /// construct from eigen type
        /// </summary>
        matrix(const eigen_type& eigenmat)
            : m_cols(eigenmat.cols()), m_rows(eigenmat.rows()), m_data(rows() * cols()), m_eigen(data(), rows(), cols()), m_refCount(1) {
            m_eigen = eigenmat;
        }

        /// <summary>
        /// begin of data container
        /// returns a const iterator
        /// </summary>
        const_iterator begin() const {
            return m_data.begin();
        }

        /// <summary>
        /// begin of data container
        /// returns an iterator
        /// </summary>
        iterator begin() {
            return m_data.begin();
        }

        /// <summary>
        /// rbegin of data container
        /// returns a const iterator
        /// </summary>
        const_reverse_iterator rbegin() const {
            return m_data.rbegin();
        }

        /// <summary>
        /// rbegin of data container
        /// returns an iterator
        /// </summary>
        reverse_iterator rbegin() {
            return m_data.rbegin();
        }

        /// <summary>
        /// end of data container
        /// returns a const iterator
        /// </summary>
        const_iterator end() const {
            return m_data.end();
        }

        /// <summary>
        /// end of data container
        /// returns an iterator
        /// </summary>
        iterator end() {
            return m_data.end();
        }

        /// <summary>
        /// rend of data container
        /// returns a const iterator
        /// </summary>
        const_reverse_iterator rend() const {
            return m_data.rend();
        }

        /// <summary>
        /// rend of data container
        /// returns an iterator
        /// </summary>
        reverse_iterator rend() {
            return m_data.rend();
        }

        /// <summary>
        /// number of rows of the matrix
        /// </summary>
        size_type rows() const {
            return m_rows;
        }

        /// <summary>
        /// number of columns of the matrix
        /// </summary>
        size_type cols() const {
            return m_cols;
        }

        /// <summary>
        /// returns the underlying data structure
        /// </summary>
        value_type* data() {
            return m_data.data();
        }

        /// <summary>
        /// returns the underlying data structure
        /// </summary>
        const value_type* data() const {
            return m_data.data();
        }

        /// <summary>
        /// returns the underlying data structure
        /// </summary>
        map_type& eigen()
        {
            return m_eigen;
        }

        /// <summary>
        /// returns the underlying data structure
        /// </summary>
        const map_type& eigen() const {
            return m_eigen;
        }

        /// <summary>
        /// add new row vector
        /// </summary>
        template <class _Vec>
        void push_back(const _Vec& vector) {
            if (m_cols == 0)
                m_cols = vector.size();

#if defined(_DEBUG) || defined(DEBUG)
            if (vector.size() != m_cols) {
                std::cout << "Warning: matrix::push_back: size of vector does not match the matrix layout." << std::endl;
                std::cout << "vector.size() = " << vector.size() << ", matrix.cols() = " << m_cols << std::endl;
            }
#endif
            m_rows++;
            m_data.insert(m_data.end(), vector.begin(), vector.end());
            update();
        }

        /// <summary>
        /// delete entries, leaving the container with a size of 0.
        /// </summary>
        void clear() {
            m_rows = 0;
            m_cols = 0;
            m_data.clear();
            update();
        }

        /// <summary>
        /// clear without change of size
        /// </summary>
        void reset() {
            size_type oldRows = rows();
            size_type oldCols = cols();
            m_data.clear();
            m_data.resize(oldRows * oldCols);
            update();
        }

        /// <summary>
        /// assign a new size and new values to the vector
        /// </summary>
        void assign(size_type r, size_type c, const value_type& defaultValue) {
            m_rows = r;
            m_cols = c;
            m_data.assign(r * c, defaultValue);
            update();
        }

        /// <summary>
        /// resize
        /// </summary>
        void resize(size_type r, size_type c) {
            m_rows = r;
            m_cols = c;
            m_data.resize(r * c);
            update();
        }

        /// <summary>
        /// reserve memory for pushing back row vectors
        /// (does not change size, but subsequent push_back()'s are more efficient)
        /// </summary>
        void reserve(size_type r, size_type c) {
            m_data.reserve(r * c);
        }
        
        /// <summary>
        /// accessing elements
        /// </summary>
        value_type& at(const size_type r, const size_type c) {
            return m_eigen(r, c);
        }

        /// <summary>
        /// accessing elements
        /// </summary>
        const value_type& at(const size_type r, const size_type c) const {
            return m_eigen(r, c);
        }

        /// <summary>
        /// bracket operator
        /// </summary>
        value_type& operator()(const size_type r, const size_type c) {
            return m_eigen(r, c);
        }

        /// <summary>
        /// bracket operator
        /// </summary>
        const value_type& operator()(const size_type r, const size_type c) const {
            return m_eigen(r, c);
        }

        /// <summary>
        /// assignment operator
        /// </summary>
        const matrix<_T>& operator=(const matrix<_T>& rhs) {
            if (this != &rhs) {
                m_rows = rhs.m_rows;
                m_cols = rhs.m_cols;
                m_data = rhs.m_data;
                update(); // this operator could change the size, therefor update the eigen data structure
            }
            return *this;
        }

        /// <summary>
        /// assignment operator
        /// </summary>
        matrix<_T>& operator=(matrix<_T>&& rhs) {
            if (this != &rhs) {
                m_rows = std::move(rhs.m_rows);
                m_cols = std::move(rhs.m_cols);
                m_data = std::move(rhs.m_data);
                update(); // this operator could change the size, therefor update the eigen data structure
            }
            return *this;
        }

        // Interface methods
        /// <summary>
        /// number of rows
        /// </summary>
        size_t rows() {
            return m_rows;
        }

        /// <summary>
        /// number of columns
        /// </summary>
        size_t cols() {
            return m_cols;
        }

        /// <summary>
        /// increase the reference count
        /// </summary>
        void addRef() {
            ++m_refCount;
        }

        /// <summary>
        /// decrese the reference count
        /// </summary>
        void release() {
            if (!--m_refCount)
                delete this;
        }

    private:
        /// <summary>
        /// update the eigen data structure
        /// </summary>
        void update() {
            // Despite appearances, this does not invoke the memory allocator, 
            // because the syntax specifies the location for storing the result.
            // ref to https://eigen.tuxfamily.org/dox/group__TutorialMapClass.html
            new (&m_eigen) map_type(data(), rows(), cols());
        }

        /// <summary>
        /// generate a matrix from a single vector
        /// </summary>
        template <class _Vec>
        const data_type matrixFromVector(size_type rows, const _Vec& v) {
            m_rows = rows;
            m_cols = v.size();
            data_type matrix(m_rows * m_cols);
            size_type it = 0;
            for (size_type r = 0; r < m_rows; ++r)
                for (size_type c = 0; c < m_cols; ++c)
                    matrix[it++] = v[c];
            return matrix;
        }

        /// <summary>
        /// generate a matrix from multiple vectors
        /// </summary>
        template <class _Vec>
        const data_type matrixFromVectors(const std::initializer_list<_Vec>& list) {
            m_rows = list.size();
#ifdef _DEBUG
            m_cols = (list.size() > 0 ? list.begin()->size() : 0);
#else
            m_cols = list.begin()->size();
#endif
            data_type matrix(m_rows * m_cols);
            size_type it = 0;
            for (const auto& vec : list) // rows
                for (const auto& value : vec) // columns
                    matrix[it++] = value;
            return matrix;
        }

        /// <summary>
        /// generate a matrix from multiple vectors
        /// </summary>
        const data_type matrixFromVectors(const vector<vector<_T>>& mat) {
            m_rows = mat.size();
#ifdef _DEBUG
            m_cols = (mat.size() > 0 ? mat.begin()->size() : 0);
#else
            m_cols = mat.front().size();
#endif
            data_type matrix(m_rows * m_cols);
            size_type it = 0;
            for (const auto& vec : mat) // rows
                for (const auto& value : vec) // columns
                    matrix[it++] = value;
            return matrix;
        }

        /// <summary>
        /// private/underlying data structure
        /// </summary>
        size_type m_cols, m_rows;
        data_type m_data;
        map_type m_eigen;
        std::atomic<int> m_refCount;
    };
}