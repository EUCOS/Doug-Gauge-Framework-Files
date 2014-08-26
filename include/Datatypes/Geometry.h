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
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * The Gauge Framework.  If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * @file include/Datatypes/Geometry.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 03.18.2012
 * @brief The Gauge::Geometry class is a datatype that encapsulates a pair of
 * Gauge::Basis and Gauge::GSOMatrix instances. This forms an input into the
 * Gauge::ModelHandler class and will be passed, via MPI.
 *
 * The Gauge::Geometry is the basic input into the WCFFHS Gauge model building
 * process.
 */

#ifndef GAUGE_FRAMEWORK_GEOMETRY_H
#define GAUGE_FRAMEWORK_GEOMETRY_H

#include <Datatypes/Basis.h>
#include <Datatypes/GSOMatrix.h>
#include <Interfaces/Printable.h>
#include <Interfaces/Serializable.h>

namespace Gauge {
  /*!
   * @brief
   * Gauge::Geometry class provides encapsulation of the two basic inputs of
   * a WCFFHS model, the basis vector and the GSO projection matrix. In the case
   * of the Gauge Framework, these come in the form of Gauge::Basis and
   * Gauge::GSOMatrix objects.
   */
  struct Geometry : public Gauge::Printable, public Gauge::Serializable {
    Gauge::Basis basis;           /*!< The basis describing the space. */
    Gauge::GSOMatrix gso_matrix;  /*!< The GSO projection matrix. */
    /*!
     * Our default constructor is trivial; everything is handled by other
     * constructors.
     */
    Geometry() {}
    /*!
     * This constructor uses the Gauge::Basis and Gauge::GSOMatrix instance
     * provided to construct the Gauge::Geometry.
     *
     * @param[in] basis The Gauge::Basis to be copied.
     * @param[in] gso_matrix The Gauge::GSOMatrix to be copied.
     */
    Geometry(const Gauge::Basis &basis, const Gauge::GSOMatrix &gso_matrix) {
      this->basis = basis;
      this->gso_matrix = gso_matrix;
    }
    /*!
     * The Gauge::Geometry copy constructor copies the internal state of the
     * provided Gauge::Geometry to @c this.
     *
     * @param[in] other The Gague::Geometry to copy.
     */
    Geometry(const Gauge::Geometry &other) {
      basis = other.basis;
      gso_matrix = other.gso_matrix;
    }
    /*!
     * Our destructor is trivial; there is no dynamic memory allocation here.
     *
     * Note: We made the destructor virtual just in case.
     */
    virtual ~Geometry() {}
    /*!
     * The assignment operator does the necessary work to assign
     * a Gauge::Geometry to @c this.
     *
     * Note: Generally we want to check for self-assignment, but in this case,
     * self-assignment is handled by the Gauge::Geometry::data method, so we
     * don't do it here.
     *
     * @param[in] other The Gauge::Geometry to assign to @c this.
     */
    Gauge::Geometry &operator=(const Gauge::Geometry &other) {
      if (this != &other) {
        basis = other.basis;
        gso_matrix = other.gso_matrix;
      }
      return *this;
    }
    /*!
     * The equality operator compares two Gauge::Geometry instances for
     * equality.
     *
     * @param[in] other The Gauge::Geometry instance to which to compare @c
     * this.
     * @return A boolean signifying equality.
     */
    bool operator==(const Gauge::Geometry &other) const {
      return basis == other.basis && gso_matrix == other.gso_matrix;
    }
    /*!
     * The non-equality operator compares two Gauge::Geometry instances for
     * non-equality.
     *
     * @param[in] other The Gauge::Geometry instance to which to compare @c
     * this.
     * @return A boolean signifying that the instances are not equal.
     */
    bool operator!=(const Gauge::Geometry &other) const {
      return basis != other.basis || gso_matrix != other.gso_matrix;
    }

    // Printable Interface
    /*!
     * By defining Gauge::Geometry::PrintTo and inheriting from
     * Gauge::Printable, we are given the Gauge::Printable::operator<< and
     * Gauge::Printable::PrintTo(const Printable&, std::ostream*).
     */
    virtual void PrintTo(std::ostream *out) const {
      *out << basis << std::endl << gso_matrix << std::endl;
    }

    // Serializable Interface
    virtual void SerializeWith(Gauge::Serializer *serializer) const {
      serializer->WriteObject(basis);
      serializer->WriteObject(gso_matrix);
    }
    virtual void DeserializeWith(Gauge::Serializer *serializer) {
      serializer->ReadObject(&basis);
      serializer->ReadObject(&gso_matrix);
    }
  };
}

#endif
