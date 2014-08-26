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
 * @file Datatypes/State.h
 * @author Douglas Moore <douglas_moore1@baylor.edu>
 * @date 04.14.2012
 * @brief The Gauge::State class is defined.
 *
 * The Gauge::State class represents FFHS charges or states. These are
 * representations of the particles in the low-energy spectrum.
 */

#ifndef GAUGE_FRAMEWORK_STATE_H
#define GAUGE_FRAMEWORK_STATE_H

#include <Datatypes/Sector.h>
#include <Datatypes/Vector.h>

namespace Gauge {
  /*!
   * @brief
   * The Gauge::State is a vector of rational numbers representing FFHS gauge
   * states or charges.
   *
   * By convention we we find the smallest common denominator such that each of
   * the numerators is even.
   */
  struct State : public Gauge::Vector {
    /*!
     * The default constructor does basic initialization.
     */
    State() : Gauge::Vector() {}
    /*!
     * The integer constructor constructs a Gauge::State of the size provided as
     * an argument.
     *
     * @param[in] size This is an integer representation of the size of the
     * vector.
     */
    explicit State(int size) : Vector(size) {}
    /*!
     * The bi-integer constructor constructs a vector of the requested size with
     * the denominator provided and the numerators initialized to 0.
     *
     * @param[in] size The size of the basis vector.
     * @param[in] den The common denominator of the vector.
     */
    State(int size, int den) : Gauge::Vector(size, den) {}
    /*!
     * The tri-integer constructor constructs a vector of the provided size with
     * the possibly non-trivial initial numerator and denominator provided.
     *
     * @param[in] size The size of the state.
     * @param[in] num The common numerator for the vector.
     * @param[in] den The common denominator of the vector.
     */
    State(int size, int num, int den) : Gauge::Vector(size, num, den) {}
    /*!
     * The Gauge::Sector constructor constructs a vector of the provided size
     * with the possibly non-trivial initial numerator and denominator provided.
     *
     * @param[in] sector The Gauge::Sector from which to construct the state.
     */
    explicit State(const Gauge::Sector &sector) : Gauge::Vector(sector) {
      den *= 2;
    }
    /*!
     * The Gauge::State copy constructor copies the contents of the Gauge::State
     * reference provided.
     *
     * @param[in] other The Gauge::State to be copied.
     */
    explicit State(const Gauge::Vector &other) : Gauge::Vector(other) {}
    /*!
     * The Gauge::State copy constructor copies the contents of the Gauge::State
     * reference provided.
     *
     * @param[in] other The Gauge::State to be copied.
     */
    State(const Gauge::State &other) : Gauge::Vector(other) {}
    /*!
     * The Gauge::State utilizes non-trivial dynamic memory allocation.
     * Consequently, we have a non-trival destructor.
     *
     * We make the destructor virtual because several classes will inherit from
     * Gauge::State.
     */
    virtual ~State() {};
  };
}

#endif
