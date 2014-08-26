/*
 * This file is part of The Gauge Framework.
 *
 * The Gauge Framework is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Gauge Framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with The Gauge Framework. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * @file Datatypes/NVector.h
 * @author Douglas Moore <douglas_moore1@baylor.edu>
 * @date 02.26.2012
 * @brief The NVector class declaration is defined.
 *
 * NVector is a datatype that represents an integer array utilized in the
 * systematic generation of gauge basis vector sets.
 */

#ifndef GAUGE_FRAMEWORK_NVECTOR_H
#define GAUGE_FRAMEWORK_NVECTOR_H

#include <Interfaces/Printable.h>
#include <Interfaces/Serializable.h>

namespace Gauge {
  /*!
   * @brief
   * The Gauge::NVector class is a datatype for storing integer arrays
   * representing the n-values that are used for systematically generating basis
   * vectors.
   *
   * It has an underlying data struct, Gauge::NVector::Data, that contains the
   * base array and an integer representing the size of the array.
   */
  struct NVector : public Gauge::Printable, public Gauge::Serializable {
    int *base;  //!< An array to hold the elements of the n-vector.
    int size;   //!< An integer representing the size of the n-vector.

    /*!
     * The default constructor does exactly nothing.
     */
    NVector() : base(NULL), size(0) {}
    /*!
     * The integer constructor takes an integer representation of the size of
     * the n-vector and initializes the array to that size.
     *
     * @param[in] size This is an integer representation of the size of the
     * nvector.
     */
    NVector(int size);
    /*!
     * The copy constructor takes a const-reference to a Gauge::NVector and
     * copies the referenced object's contents into this.
     *
     * @param[in] other The Gauge::NVector to be copied.
     */
    NVector(const Gauge::NVector &other);
    // Destructor
    /*!
     * Because the underlying data structure has a dynamically allocated array
     * at its heart, we much manually deallocate that memory. Thus, this
     * destructor ensures that the memory management occurs properly.
     *
     * Note that, because we cannot foresee all future use of the classes of
     * this framework, we have made the destructor virtual.
     */
    virtual ~NVector() { if (base != NULL) delete [] base; }
    /*!
     * The assignment operator takes a const-reference to a Gauge::NVector,
     * copies the referenced object's contents into this, and returns a
     * reference to this.
     *
     * @param[in] other The Gauge::NVector to be copied.
     * @return Returns a reference to this.
     */
    NVector &operator=(const NVector &other);
    /*!
     * The equality operator determines the equality of two Gauge::NVector
     * instances.
     *
     * @param[in] other The Gauge::NVector instance to which to compare @c this.
     * @return A boolean signifying equality.
     */
    bool operator==(const Gauge::NVector &other) const;
    /*!
     * The non-equality operator determines the equality of two Gauge::NVector
     * instances.
     *
     * @param[in] other The Gauge::NVector instance to which to compare @c this.
     * @return A boolean signifying that the instances are not equal.
     */
    bool operator!=(const Gauge::NVector &other) const {
      return !(*this == other);
    }

    // Printable Interface
    virtual void PrintTo(std::ostream *out) const;

    // Serializable Interface
    virtual void SerializeWith(Gauge::Serializer *serializer) const;
    virtual void DeserializeWith(Gauge::Serializer *serializer);
  };
}

#endif
