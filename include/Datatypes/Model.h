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
 * @file include/Datatypes/Model.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 05.20.2012
 * @brief The Gauge::Model datatype is declared.
 */

#ifndef GAUGE_FRAMEWORK_MODEL_H
#define GAUGE_FRAMEWORK_MODEL_H

#include <Datatypes/Geometry.h>
#include <Datatypes/Group.h>
#include <Datatypes/StateList.h>

#include <Interfaces/Printable.h>
#include <Interfaces/Serializable.h>

namespace Gauge {
  /*!
   * @brief
   * The Gauge::Model data structure represents a physical model of the
   * universe.
   *
   * The Gauge::Model includes the geometry of the compactified spacetime
   * dimensions, the gauge group and the number of spacetime supersymmetries.
   */
  struct Model : public Gauge::Printable, public Gauge::Serializable {
    Gauge::Geometry *geometry;  /*!< A pointer to the geometry object.  */
    Gauge::Group *group;        /*!< A list of Gauge::Group instances.  */
    int susy;                   /*!< The number of supersymmetries.     */
    Gauge::StateList states;    /*!< The low energy states.             */

    /*!
     * The default constructor initializes the geometry pointer to @c NULL, and
     * the number of supersymmetries to @c 0.
     */
    Model() : geometry(NULL), group(NULL), susy(0), states() {}
    /*!
     * The copy constructor does a straight-up copy of the contents of a
     * Gauge::Model instance into a new instance.
     *
     * @param[in] other The Gauge::Model instance to copy.
     */
    Model(const Gauge::Model &other) = delete;
    /*!
     * The destructor destructs the Gauge::Geometry and the list of Gauge::Group
     * instances.
     */
    ~Model();
    /*!
     * The assignment operator does a straight-up copy of the contents of a
     * Gauge::Model instance into @c this.
     *
     * @param[in] other The Gauge::Model instance to copy.
     * @return A reference to @c this.
     */
    Gauge::Model &operator=(const Gauge::Model &other) = delete;
    /*!
     * The equality operator compares to Gauge::Model instances to determine if
     * they are equal.
     *
     * @param[in] other The Gauge::Model to which to compare to @c this
     * @return A boolean signifying that the instances are equal.
     */
    bool operator==(const Gauge::Model &other) const;
    /*!
     * The "not equals" operator compares to Gauge::Model instances to determine
     * if they are not equal.
     *
     * @param[in] other The Gauge::Model to which to compare to @c this
     * @return A boolean signifying that the instances are not equal.
     */
    bool operator!=(const Gauge::Model &other) const {
      return !(*this == other);
    }

    /* Printable Interface */
    virtual void PrintTo(std::ostream *out) const;

    // Serializable Interface
    virtual void SerializeWith(Gauge::Serializer *serializer) const;
    virtual void DeserializeWith(Gauge::Serializer *serializer);
  };
}

#endif
