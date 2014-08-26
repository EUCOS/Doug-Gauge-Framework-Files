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
 * @file Datatypes/NVector.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 02.26.2012
 *
 * @brief The implementation of the Gauge::NVector datatype, a vector of
 * integers utilized for the generation of gauge basis vector sets.
 */

// C Headers
#include <cstdlib>

// Gauge Framework Headers
#include <Datatypes/NVector.h>

Gauge::NVector::NVector(int size) {
  this->size = size;
  base = new int[this->size];
  for (int index = 0; index < this->size; ++index) base[index] = 0;
}
/*!
 * The copy constructor simply copies the provided Gauge::NVector
 * to @c *this using the data copy method.
 */
Gauge::NVector::NVector(const Gauge::NVector& other) {
  size = other.size;
  base = new int[size];
  for (int index = 0; index < size; ++index) base[index] = other.base[index];
}
/*!
 * The assignment operator checks that we aren't reassigning to the
 * same instence. If we aren't then we perform a copy in much the same
 * way that the copy constructor does.
 */
Gauge::NVector& Gauge::NVector::operator=(const Gauge::NVector& other) {
  if (this != &other) {
    if (base != NULL && size != other.size) delete [] base;
    if (base == NULL) base = new int[other.size];
    size = other.size;
    for (int index = 0; index < size; ++index) base[index] = other.base[index];
  }
  return *this;
}
/*!
 * We simply compare the Gauge::NVector instances element-by-element.
 */
bool Gauge::NVector::operator==(const Gauge::NVector &other) const {
  if (size != other.size) return false;
  for (int index = 0; index < size; ++index)
    if (base[index] != other.base[index]) return false;
  return true;
}

// Printable Interface
void Gauge::NVector::PrintTo(std::ostream *out) const {
  *out << "[ ";
  if (size >= 1) {
    *out << base[0];
    if (size > 1) {
      for (int index = 1; index < size; ++index) {
        *out << " " << base[index];
      }
    }
  }
  *out << " ]";
}

// Serializable Interface
void Gauge::NVector::SerializeWith(Gauge::Serializer *serializer) const {
  // We cannot compress any of our data because we can't guarantee it won't
  // overflow that a smaller type.
  serializer->Write<int>(size);
  serializer->Write<int>(base, base + size);
}

void Gauge::NVector::DeserializeWith(Gauge::Serializer *serializer) {
  serializer->Read<int>(&size);

  if (base != NULL) delete [] base;
  base = new int[size];

  serializer->Read<int>(base, base + size);
}
