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
 * @file src/Datatypes/BasisVector.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 05.10.2012
 *
 * @brief The implementation of the Gauge::BasisVector datatype, a vector
 * rational numbers.
 */

// C Headers
#include <cassert>

// Gauge Framework Headers
#include <Datatypes/BasisVector.h>

Gauge::BasisVector &Gauge::BasisVector::operator=(
    const Gauge::BasisVector &other) {
  if (this != &other) {
    if (base != NULL && size != other.size) delete [] base;
    size = other.size;
    leading = size;
    trailing = 0;
    if (other.base != NULL) {
      if (base == NULL) base = new int[size];
      bool found_leading = false;
      for (int index = 0; index < size; ++index) {
        base[index] = other.base[index];
        if (base[index] != 0) {
          trailing = index + 1;
          if (!found_leading) {
            leading = index;
            found_leading = true;
          }
        }
      }
    } else if (size == 0) {
      base = NULL;
    } else {
      assert(false);
    }
    den = other.den;

    begin = base;
    end = base + size;
  }

  return *this;
}
