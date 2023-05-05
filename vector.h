/*
 *  vector.h
 *  Created by Matthias Kesenheimer on 19.06.22.
 *  Copyright 2022. All rights reserved.
 *  More information about the Eigen library at http://eigen.tuxfamily.org/dox/index.html
 */

#pragma once
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
    /// vector class
    /// Verknuepft die Daten gespeichert in m_data mit Eigen::matrix. Dadurch koennen Rechenoperationen einfacher und schneller durchgefuehrt werden.
    /// </summary>
    template <class _T>
    class vector {
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
        using eigen_type = Eigen::Matrix<_T, Eigen::Dynamic, 1>;
        using map_type = Eigen::Map<eigen_type>; // map the data structure to Eigen::matrix

        // Notiz zum Zaehlen der Referenzen:
        // - Beim Anlegen eines Objekts muss der Referenzzaehler immer 1 sein, d.h. auch wenn der move-constructor 
        //   vector(vector&& other) verwendet wird. Es wird ein neues Objekt erzeugt, auf die bisher nur eine Referenz zeigt.
        //   Es spielt keine Rolle, ob das Objekt durch eine Move-Operation oder durch den Standardkonstruktor erzeugt wird.
        //   Beim Move-Konstruktor sollen ja nur die Daten verschoben werden. Die Referenzen zum alten Objekt sollen trotzdem bestehen
        //   bleiben.
        // - Bei den Kopier- bzw. Move-Operatoren const vector<_T>& operator=(const vector<_T>& rhs) und vector<_T>& operator=(const vector<_T>&& rhs)
        //   werden ausserdem auch nur die Daten kopiert bzw. verschoben. Die urspruenglichen Referenzen duerfen von dieser Aktion nicht beeinflusst werden.
        //   Deshalb werden in diesen Faellen die Referenzzaehler nicht veraendert.

        /// <summary>
        /// construct a dynamic-size vector with size 'size'
        /// </summary>
        vector(size_type s)
            : m_data(s), m_eigen(data(), size(), 1), m_refCount(1) {}

        /// <summary>
        /// construct an object from a std::vector
        /// </summary>
        vector(const data_type& v)
            : m_data(v.begin(), v.end()), m_eigen(data(), size(), 1), m_refCount(1) {}

        /// <summary>
        /// construct a dynamic-size vector with size 'size' and default value 'defaultValue'
        /// </summary>
        vector(size_type s, const value_type& defaultValue)
            : m_data(s, defaultValue), m_eigen(data(), size(), 1), m_refCount(1) {}

        /// <summary>
        /// construct a dynamic-size empty vector
        /// </summary>
        vector()
            : m_data(), m_eigen(data(), size(), 1), m_refCount(1) {}

        /// <summary>
        /// copy constructor
        /// </summary>
        vector(const vector& other)
            : m_data(other.m_data), m_eigen(data(), size(), 1), m_refCount(1) {}

        /// <summary>
        /// move constructor
        /// </summary>
        vector(vector&& other) noexcept
            : m_data(std::move(other.m_data)), m_eigen(data(), size(), 1), m_refCount(1) {}

        /// <summary>
        /// initializing by initializer list
        /// </summary>
        vector(std::initializer_list<value_type> IList)
            : m_data(IList), m_eigen(data(), size(), 1), m_refCount(1) {}

        /// <summary>
        /// construct from Eigen::matrix
        /// </summary>
        vector(const eigen_type& eigvec)
            : m_data(eigvec.rows()), m_eigen(data(), size(), 1), m_refCount(1) {
            m_eigen = eigvec;
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
        /// size of the data container
        /// </summary>
        const size_type size() const {
            return m_data.size();
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
        map_type& eigen() {
            return m_eigen;
        }

        /// <summary>
        /// returns the underlying data structure
        /// </summary>
        const map_type& eigen() const {
            return m_eigen;
        }

        /// <summary>
        /// is the vector empty?
        /// </summary>
        const bool empty() const {
            return m_data.empty();
        }

        /// <summary>
        /// add new value
        /// </summary>
        void push_back(const value_type& value) {
            m_data.push_back(value);
            update();
        }

        /// <summary>
        /// add new value
        /// </summary>
        void push_back(value_type&& Val) {
            m_data.push_back(Val);
            update();
        }

        /// <summary>
        /// append a vector
        /// </summary>
        void append(const vector<value_type>& toAppend) {
            m_data.insert(m_data.end(), toAppend.begin(), toAppend.end());
            update();
        }

        /// <summary>
        /// delete entries, leaving the container with a size of 0.
        /// </summary>
        void clear() {
            m_data.clear();
            update();
        }

        /// <summary>
        /// clear without change of size
        /// </summary>
        void reset() {
            size_type oldSize = size();
            m_data.clear();
            m_data.resize(oldSize);
            update();
        }

        /// <summary>
        /// assign a new size and new values to the vector
        /// </summary>
        void assign(size_type size, const value_type& defaultValue) {
            m_data.assign(size, defaultValue);
            update();
        }

        /// <summary>
        /// resize the vector
        /// </summary>
        void resize(size_type newSize) {
            m_data.resize(newSize);
            update();
        }

        /// <summary>
        /// reserve memory (does not change size, but subsequent push_back()'s are more efficient)
        /// </summary>
        void reserve(size_type size) {
            m_data.reserve(size);
        }

        /// <summary>
        /// erase with iterator
        /// </summary>
        void erase(iterator it) {
            m_data.erase(it);
            update();
        }

        /// <summary>
        /// erase elements in iterator range
        /// </summary>
        void erase(iterator it1, iterator it2) {
            m_data.erase(it1, it2);
            update();
        }

        /// <summary>
        /// accessing elements
        /// </summary>
        reference at(const size_type i) {
            return m_data.at(i);
        }

        /// <summary>
        /// accessing elements
        /// </summary>
        const_reference at(const size_type i) const {
            return m_data.at(i);
        }

        /// <summary>
        /// first element
        /// </summary>
        const_reference front() const {
            return m_data.front();
        }

        /// <summary>
        /// first element
        /// </summary>
        reference front() {
            return m_data.front();
        }

        /// <summary>
        /// last element
        /// </summary>
        const_reference back() const {
            return m_data.back();
        }

        /// <summary>
        /// last element
        /// </summary>
        reference back() {
            return m_data.back();
        }

        /// <summary>
        /// accessing elements
        /// </summary>
        const_reference operator[] (size_type const i) const {
#if defined(DEBUG) || defined(_DEBUG)
            return m_data.at(i);
#else
            return m_data[i];
#endif
        }

        /// <summary>
        /// accessing elements
        /// </summary>
        reference operator[] (size_type const i) {
#if defined(DEBUG) || defined(_DEBUG)
            return m_data.at(i);
#else
            return m_data[i];
#endif
        }

        /// <summary>
        /// assignment operator
        /// </summary>
        const vector<_T>& operator=(const vector<_T>& rhs) {
            if (this != &rhs) {
                m_data = rhs.m_data;
                update(); // this operator could change the size, therefor update the eigen data structure
            }
            return *this;
        }

        /// <summary>
        /// assignment operator
        /// </summary>
        vector<_T>& operator=(vector<_T>&& rhs) noexcept {
            if (this != &rhs) {
                m_data = std::move(rhs.m_data);
                update(); // this operator could change the size, therefor update the eigen data structure
            }
            return *this;
        }

        // Interface methods
        /// <summary>
        /// size of the data container
        /// </summary>
        size_t size() {
            return m_data.size();
        }

        /// <summary>
        /// 
        /// </summary>
        void addRef() {
            ++m_refCount;
        }

        /// <summary>
        /// 
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
                new (&m_eigen) map_type(data(), size(), 1);
            }

            /// <summary>
            /// private/underlying data structure
            /// </summary>
            data_type m_data;
            map_type m_eigen;
            std::atomic<int> m_refCount;
    };
}