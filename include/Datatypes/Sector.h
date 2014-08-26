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
 * @file Datatypes/Sector.h
 * @author Douglas Moore <douglas_moore1@baylor.edu>
 * @date 04.14.2012
 * @brief The Gauge::Sector class is defined.
 *
 * The Gauge::Sector class represents FFHS sectors, the phases the free fermions
 * pick up under parallel transport.
 */

#ifndef GAUGE_FRAMEWORK_SECTOR_H
#define GAUGE_FRAMEWORK_SECTOR_H

#include <Datatypes/Vector.h>

namespace Gauge {
  /*!
   * @brief
   * The Gauge::Sector is a vector of rational numbers representing the phases
   * that free fermions pick up under parallel transport about non-contractible
   * loops.
   *
   * By convention we we find the smallest common denominator such that each of
   * the numerators is even.
   */
  struct Sector : public Gauge::Vector {
    /*!
     * The default constructor does basic initialization.
     */
    Sector() : Gauge::Vector() {}
    /*!
     * The integer constructor constructs a Gauge::Sector of the size provided
     * as an argument.
     *
     * @param[in] size This is an integer representation of the size of the
     * vector.
     */
    explicit Sector(int size) : Vector(size) {}
    /*!
     * The bi-integer constructor constructs a vector of the requested size with
     * the denominator provided and the numerators initialized to 0.
     *
     * @param[in] size The size of the vector.
     * @param[in] den The common denominator of the vector.
     */
    Sector(int size, int den) : Gauge::Vector(size, den) {}
    /*!
     * The tri-integer constructor constructs a vector of the provided size with
     * the possibly non-trivial initial numerator and denominator provided.
     *
     * @param[in] size The size of the vector.
     * @param[in] num The common numerator for the vector.
     * @param[in] den The common denominator of the vector.
     */
    Sector(int size, int num, int den) : Gauge::Vector(size, num, den) {}
    /*!
     * The Gauge::Sector copy constructor copies the contents of the
     * Gauge::Sector reference provided.
     *
     * @param[in] other The Gauge::Sector to be copied.
     */
    explicit Sector(const Gauge::Vector &other) : Gauge::Vector(other) {}
    /*!
     * The Gauge::Sector copy constructor copies the contents of the
     * Gauge::Sector reference provided.
     *
     * @param[in] other The Gauge::Sector to be copied.
     */
    Sector(const Gauge::Sector &other) : Gauge::Vector(other) {}
    /*!
     * The Gauge::Sector utilizes non-trivial dynamic memory allocation.
     * Consequently, we have a non-trival destructor.
     *
     * We make the destructor virtual because several classes will inherit from
     * Gauge::Sector.
     */
    virtual ~Sector() {};
  };
}

#endif
