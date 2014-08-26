/*
 * This file is part of The Gauge Framework.
 *
 * The Gauge Framework is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * The Gauge Framework is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * The Gauge Framework. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * @file Math.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 02.27.2012
 *
 * @brief The Math namespace is declared.
 */

#ifndef GAUGE_FRAMEWORK_MATH_H
#define GAUGE_FRAMEWORK_MATH_H

// System Headers
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <math.h>

namespace Gauge {
  /*!
   * The Math namespace contains generic definitions of mathematically related
   * methods. These are methods that do not really belong in a datatype class
   * but are too general for a single handler class. This includes things like
   * functions to determine the length-squared of a vector or the product of
   * terms in an array-like object, etc...
   */
  namespace Math {
    /*!
     * One of the most common mathematical operations we must perform is the
     * determination of the greatest common divisor of two integers. To that
     * end we require as input two integers and output their GCD.
     *
     * @param[in] alpha the first integer
     * @param[in] beta  the second integer
     *
     * @return the greatest common divisor of the provided integers.
     */
    int GCD(int alpha, int beta);
    /*!
     * We use this Gauge::Math::LCM function to calculate the least-common
     * multiple of two integers.
     *
     * Recall that @f$ lcm(a,b) = \frac{a*b}{gcd(a,b)} @f$, so that is what we
     * do here.
     *
     * @param[in] alpha The first integer.
     * @param[in] beta The second integer.
     *
     * @return The least common multiple of the provided integers.
     */
    int LCM(int alpha, int beta);
    /*!
     * This method calculates the square root of an integer and rounds down to
     * the integer.
     *
     * @param[in] alpha The value to square root.
     *
     * @return The rounded-down square root.
     */
    int ISqrt(int alpha);
    /*!
     * This method computes the product of a list of values accumulating in
     * a provided type.
     *
     * We, very simply, use the provided iterators to iterate through the
     * array-like structure and accumulate the product as we go.
     *
     * @param[in] begin an iterator to the first value
     * @param[in] end   an iterator to the value following the last
     * @param[in] acc   an accumulator of the type desired at return
     *
     * @return the accumulated product.
     */
    template <class Iter, class T>
    T Product(Iter begin, Iter end, T acc) {
      for (; begin != end; ++begin) acc *= (*begin);
      return acc;
    }
  }
}

#endif
