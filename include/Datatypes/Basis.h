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
 * @file Datatypes/Basis.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 03.18.2012
 * @brief The Gauge::Basis is the set of basis vector used to construct
 * a particular gauge model.
 *
 * The Gauge::Basis is essentially an array of Gauge::BasisVector instances.
 */

#ifndef GAUGE_FRAMEWORK_BASIS_H
#define GAUGE_FRAMEWORK_BASIS_H

#include <Datatypes/BasisVector.h>
#include <Interfaces/Printable.h>
#include <Interfaces/Serializable.h>

namespace Gauge {
  /*!
   * @brief
   * The Gauge::Basis class is an array class containing the boundary
   * conditions necessary for specifying gauge models.
   *
   * The underlying data is a struct (Gauge::Basis:Data) containing a
   * dynamically allocated array of Gauge::BasisVector instances.
   */
  struct Basis : public Gauge::Printable, public Gauge::Serializable {
    Gauge::BasisVector *base; /*!< A dynamically allocated array of
                                Gauge::BasisVector objects. */
    int size;                 /*!< An integer representation of the size of the
                                Basis. */
    /*!
     * The default constructor initializes the Gauge::Basis::basis field to @c
     * NULL and the size to @c 0.
     */
    Basis() : base(NULL), size(0) {}
    /*!
     * The integer constructor initializes the Gauge::Basis to the provided
     * size.
     *
     * @param[in] size The number of basis vectors in the basis.
     */
    explicit Basis(int size);
    /*!
     * The integer constructor initializes the Gauge::Basis to the provided
     * size with basis vectors of the requested width.
     *
     * @param[in] size The number of basis vectors in the basis.
     * @param[in] width The width of the basis vectors in the basis.
     */
    Basis(int size, int width);
    /*!
     * The Gauge::Basis copy constructor copies the internal state of the
     * provided Gauge::Basis to @c this.
     *
     * @param[in] other The Gague::Basis to copy.
     */
    Basis(const Gauge::Basis &other);
    /*!
     * Because of the non-trivial memory allocation required for construction
     * of Gauge::Basis objects, we have to have a non-trivial destructor.
     *
     * Note: We made the destructor virtual just in case.
     */
    virtual ~Basis() { if (base != NULL) delete [] base; }
    /*!
     * The assignment operator does the necessary work to assign
     * a Gauge::Basis to @c this.
     *
     * @param[in] other The Gauge::Basis to assign to @c this.
     */
    Gauge::Basis &operator=(const Gauge::Basis &other);
    /*!
     * The equality operator determines the equality of the Gauge::Basis
     * instances.
     *
     * @param[in] other The Gauge::Basis to which to compare @this.
     * @return A boolean signifying equality.
     */
    bool operator==(const Gauge::Basis &other) const;
    /*!
     * The equality operator determines the equality of the Gauge::Basis
     * instances.
     *
     * The "not equals" operator is simply the logical negation of the equality
     * operator. To see implementation details, see the
     * Gauge::Basis::operator==(const Gauge::Basis &) operator.
     *
     * @see Gauge::Basis::operator==(const Gauge::Basis &)
     *
     * @param[in] other The Gauge::Basis to which to compare @this.
     * @return A boolean signifying nonequality.
     */
    bool operator!=(const Gauge::Basis &other) const {
      return !(*this == other);
    }

    // Printable Interface
    /*!
     * By defining Gauge::Basis::PrintTo and inheriting from Gauge::Printable,
     * we are given the Gauge::Printable::operator<< and
     * Gauge::Printable::PrintTo(const Printable&, std::ostream*).
     */
    virtual void PrintTo(std::ostream *out) const;

    // Serializable Interface
    virtual void SerializeWith(Gauge::Serializer *serializer) const;
    virtual void DeserializeWith(Gauge::Serializer *serializer);
  };
}

#endif
