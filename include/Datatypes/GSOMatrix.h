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
 * @file Datatypes/GSOMatrix.h
 * @author Douglas Moore <douglas_moore1@baylor.edu>
 * @date 03.16.2012
 * @brief The Gauge::GSOMatrix is a datatype for storing the GSO projection
 * matrix coefficients.
 *
 * The underlying data of the Gauge::GSOMatrix is an array that represents
 * a matrix. Note that the GSO projection matrix is always square, so we need
 * only store one of the dimensions.
 */

#ifndef GAUGE_FRAMEWORK_GSOMATRIX_H
#define GAUGE_FRAMEWORK_GSOMATRIX_H

#include <Datatypes/Rational.h>
#include <Interfaces/Printable.h>
#include <Interfaces/Serializable.h>

namespace Gauge {
  /*!
   * @breif
   * The Gauge::GSOMatrix class is a datatype for storing the GSO projection
   * matrix, one of the two inputs need to specify a Gauge::Model.
   *
   * The GSO projection matrix is a square matrix of height equal to the number
   * of layers in the model. The elements of the matrix are
   * Gauge::Math::Rational objects.
   */
  struct GSOMatrix : public Gauge::Printable, public Gauge::Serializable {
    Gauge::Math::Rational **base; /*!< A dynamically allocated array of
                                    Gauge::Math::Rational values. */
    int size;                     /*!< An integer field representing the size of
                                    the Gauge::GSOMatrix. */
    /*!
     * The default constructor initializes the internal array to @c NULL and the
     * size to @c.
     */
    GSOMatrix() : base(NULL), size(0) {}
    /*!
     * Our integer constructor inializes the base array to a length equal to the
     * square of the provided argument with each element set to @c 0.
     *
     * @param[in] size The dimenson of the matrix. Because it is square, the
     * height and width are the same.
     */
    GSOMatrix(int size);
    /*!
     * The copy constructor copies the contents of the provided instance of
     * Gauge::GSOMatrix to @c this.
     *
     * @param[in] other The Gauge::GSOMatrix to copy.
     */
    GSOMatrix(const Gauge::GSOMatrix &other);
    /*!
     * Because we will be doing some dynamic memory allocation, we need the a
     * non-trivial destructor.
     */
    virtual ~GSOMatrix();
    /*!
     * The assignment operator copies the contents of the provided instance of
     * Gauge::GSOMatrix to @c this.
     *
     * @param[in] other The Gauge::GSOMatrix to copy.
     */
    GSOMatrix &operator=(const Gauge::GSOMatrix &other);
    /*!
     * The equality operator compares two Gauge::GSOMatrix instance for
     * equality.
     *
     * @param[in] other The Gauge::GSOMatrix instance to which to compare @c
     * this.
     * @return A boolean signifying equality.
     */
    bool operator==(const Gauge::GSOMatrix &other) const;
    /*!
     * The non-equality operator compares two Gauge::GSOMatrix instance for
     * equality.
     *
     * @param[in] other The Gauge::GSOMatrix instance to which to compare @c
     * this.
     * @return A boolean signifying that the instances are not equal.
     */
    bool operator!=(const Gauge::GSOMatrix &other) const {
      return !(*this == other);
    }

    // Printable Interface
    /*!
     * By defining Gauge::GSOMatrix::PrintTo and inheriting from
     * Gauge::Printable, we are given the Gauge::Printable::operator<< and
     * Gauge::Printable::PrintTo(const Printable&, std::ostream*).
     */
    virtual void PrintTo(std::ostream *out) const;

    // Serializable Interface
    virtual void SerializeWith(Gauge::Serializer *serializer) const;
    virtual void DeserializeWith(Gauge::Serializer *serializer);
  };
}

#endif
