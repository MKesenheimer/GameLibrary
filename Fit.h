/*
 *  Fit.h
 *  Created by Matthias Kesenheimer on 20.06.2022.
 *  Copyright 2022. All rights reserved.
 */
#pragma once
#include "vector.h"
#include "matrix.h"

namespace math::utilities
{
    /// <summary>
    /// 
    /// </summary>
    class Fit
    {
    public:
        /// <summary>
        /// Linfit loest ein ueberbestimmtes LGS (numberOfEquations > numberOfUnknowns). Das LGS ist durch b und A
        /// gegeben.
        /// Die Loesung des LGS wird ueber eine QR-Zerlegung gemacht. Sind die
        /// QR-Matrizen vorhanden werden durch Multiplikation von der linken Seite mit Q-transponiert und dann
        /// durch rueckeinsetzen der Diagonalmatrix R die Unbekannten berechnet.
        /// Zurueckgegeben wird der Vektor x aus der Gleichung b = A * x.
        /// </summary>
        template<typename T>
        static vector<T> linFit(const vector<T>& b, const matrix<T>& A) {
            vector<T> x;
            linFit(b, A, x);

            // returns empty vector() if fit failed
            return x;
        }

        template<typename T>
        static int linFit(const vector<T>& b, const matrix<T>& A, vector<T>& x) {
            if (A.rows() == 0 || A.cols() == 0 || b.size() == 0)
                return -1;

            x = vector<T>(A.eigen().colPivHouseholderQr().solve(b.eigen()));
            return 0;
        }

        /// <summary>
        /// Fit a polynom of degree "degree" to a given number of points.
        /// The matrix "points" can be generated as follows, for example:
        /// math::vector<double> p1 = {0, 0};
        /// math::vector<double> p2 = {1, 1};
        /// math::vector<double> p3 = {2, 4};
        /// math::vector<double> p4 = {3, 9};
        /// math::matrix<double> points;
        /// points.push_back(p1);
        /// points.push_back(p2);
        /// points.push_back(p3);
        /// points.push_back(p4);
        /// </summary>
        /// <return>
        /// Returns a vector c = {c0, c1, c2, ..., cn} of coefficients that define the polynomial:
        /// P_n(x) = c0 + c1 * x + c2 * x^2 + ...
        /// where n is the degree of the polynomial.
        /// </return>
        template<typename T>
        static math::vector<T> polyFit(const matrix<T>& points, size_t degree) {
            const size_t numberOfPoints = points.rows();

             // A is the coefficient matrix of the polynom
            math::matrix<double> A(numberOfPoints, degree + 1);
            math::vector<double> b(numberOfPoints);
            for (size_t i = 0; i < numberOfPoints; ++i) { // for every vector
                for (size_t j = 0; j <= degree; ++j) { // for every coefficient (degree)
                    A(i, j) = std::pow(points(i, 0), j);
                }
                b[i] = points(i, 1);
            }

            // Solve the equation b = A * x for x -> gives the coefficient of the polynomial 
            // P_n(x) = c0 + c1 * x + c2 * x^2 + ...
            return linFit<T>(b, A);
        }
    };
}