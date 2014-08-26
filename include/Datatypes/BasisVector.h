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
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * The Gauge Framework. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * @file Datatypes/BasisVector.h
 * @author Douglas Moore <douglas_moore1@baylor.edu>
 * @date 03.16.2012
 * @brief The Gauge::BasisVector class is defined.
 *
 * The Gauge::BasisVector class provides a data container for FFHS Gauge basis
 * vectors. These basis vectors represent the boundary conditions of
 * free-fermions on the string world sheet under parallel transport about
 * non-constractable loops.
 */

#ifndef GAUGE_FRAMEWORK_BASISVECTOR_H
#define GAUGE_FRAMEWORK_BASISVECTOR_H

#include <Datatypes/NVector.h>
#include <Datatypes/Vector.h>

namespace Gauge {
  /*!
   * @brief
   * The Gauge::BasisVector is a vector of rational numbers representing the
   * boundary conditions on the free fermions under parallel transport about
   * non-contractable loops on the world sheet.
   *
   * By convention we we find the smallest common denominator such that each of
   * the numerators is even.
   */
  struct BasisVector : public Gauge::Vector {
    int &order; /*!< A reference to the denominator of the basis vector. */

    /*!
     * The default constructor does basic initialization.
     */
    BasisVector() : Gauge::Vector(), order(den) {}
    /*!
     * The integer constructor constructs a Gauge::BasisVector of the size
     * provided as an argument.
     *
     * @param[in] size This is an integer representation of the size of the
     * vector.
     */
    explicit BasisVector(int size) : Vector(size), order(den) {}
    /*!
     * The bi-integer constructor constructs a vector of the requested size with
     * the denominator provided and the numerators initialized to 0.
     *
     * @param[in] size The size of the basis vector.
     * @param[in] order The common denominator of the vector.
     */
    BasisVector(int size, int order) : Gauge::Vector(size, order), order(den) {}
    /*!
     * The tri-integer constructor constructs a vector of the provided size with
     * the possibly non-trivial initial numerator and denominator provided.
     *
     * @param[in] size The size of the basis vector.
     * @param[in] numerator The common numerator for the vector.
     * @param[in] order The common denominator of the vector.
     */
    BasisVector(int size, int num, int order) :
      Gauge::Vector(size, num, order), order(den) {}
    /*!
     * The Gauge::BasisVector copy constructor copies the contents of the
     * Gauge::BasisVector reference provided.
     *
     * @param[in] other The Gauge::BasisVector to be copied.
     */
    explicit BasisVector(const Gauge::Vector &other) :
      Gauge::Vector(other), order(den) {}
    /*!
     * The Gauge::BasisVector copy constructor copies the contents of the
     * Gauge::BasisVector reference provided.
     *
     * @param[in] other The Gauge::BasisVector to be copied.
     */
    BasisVector(const Gauge::BasisVector &other) :
      Gauge::Vector(other), order(den) {}
    /*!
     * The Gauge::BasisVector utilizes non-trivial dynamic memory allocation.
     * Consequently, we have a non-trival destructor.
     *
     * We make the destructor virtual because several classes will inherit from
     * Gauge::BasisVector.
     */
    virtual ~BasisVector() {};
    /*!
     * This Gauge::BasisVector assignment operator takes care of the necessary
     * reallocation of the base array and assignment of values from the provided
     * Gauge::BasisVector instance.
     *
     * @param[in] other The Gauge::BasisVector instance to be copied.
     *
     * @return A reference to @c this.
     */
    BasisVector &operator=(const Gauge::BasisVector &other);
  };

  /*!
   * The All-Periodic basis vector is used so often that a static version of it
   * might come in handy.
   */
  static const Gauge::BasisVector kPeriodicBasisVector(22,2,2);

  /*!
   * The SUSY basis vector is used so often that a static version of it
   * might come in handy.
   */
  static const Gauge::BasisVector kSUSYBasisVector(22,0,2);
}

#endif
