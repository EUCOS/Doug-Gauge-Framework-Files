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
 * @file include/Datatypes/Raw.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 09.04.2012
 * @brief This datatype represents raw data.
 *
 * The Gauge::Raw datatype is a wrapper for a char array used to represent pure,
 * unstructured data.
 */

#pragma once

#include <Interfaces/Printable.h>

namespace Gauge {
  /*!
   * @brief
   * The Gauge::Raw struct is simply an array struct to hold unstructured data
   * for serialization.
   *
   * A Gauge::Raw instance can be used to write binary data to file or provided
   * compressed data for message passing via OpenMPI.
   */
  struct Raw : public Gauge::Printable {
    char *data; /*!< A dynamically allocated array of characters (bytes). */
    int size;   /*!< An integer representation of the size of the buffer. */

    /*!
     * The default constructor initializes the Gauge::Raw::data field to @c NULL
     * and the size to @c to 0.
     */
    Raw() : data(NULL), size(0) {}
    /*!
     * The integer constructor initializes the Gauge::Raw to the provided size.
     *
     * @param[in] size The size of the raw data in bytes.
     */
    explicit Raw(int size);
    /*!
     * The copy constructor copies the data from a Gauge::Raw instance to a new
     * instance of the Gauge::Raw class.
     *
     * @param[in] other The Gauge::Raw instance to be copied.
     */
    Raw(const Gauge::Raw &other);
    /*!
     * The destructor deallocates the dynamically allocated Gauge::Raw::data.
     *
     * Note: We made the destructor virtual, just in case.
     */
    virtual ~Raw() { if (data != NULL) delete [] data; }
    /*!
     * The assignment operator copies the contents of a Gauge::Raw instance to
     * @c this.
     *
     * @param[in] other The Gauge::Raw to assign to @c this.
     */
    Gauge::Raw &operator=(const Gauge::Raw &other);
    /*!
     * The equality operator compares two Gauge::Raw instances
     * element-by-element for equality.
     *
     * @param[in] other The Gauge::Raw instance to which to compare @c this.
     * @return A boolean signifying that the two Gauge::Raw instances are equal.
     */
    bool operator==(const Gauge::Raw &other) const;
    /*!
     * The "not equals" operator compares two Gauge::Raw instances
     * element-by-element for nonequality.
     *
     * @param[in] other The Gauge::Raw instance to which to compare @c this.
     * @return A boolean signifying that the two Gauge::Raw instances are not
     * equal.
     */
    bool operator!=(const Gauge::Raw &other) const { return !(*this == other); }

    // Printable Interface
    /*!
     * By defining Gauge::Raw::PrintTo and inheriting from Gauge::Printable, we
     * are given the necessary machinery to write a Gauge::Raw to an
     * std::ostream which may be a filestream, stringstream, etc.
     *
     * @param[in,out] out The output stream to which to write.
     */
    virtual void PrintTo(std::ostream *out) const { out->write(data, size); }
  };
}
