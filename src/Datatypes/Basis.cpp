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
 * @file Datatypes/Basis.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 03.18.2012
 *
 * @brief The implementation of the Gauge::NVector datatype, a vector of
 * integers utilized for the generation of gauge basis vector sets.
 */

// C Headers
#include <cstdlib>

// Gauge Framework Headers
#include <Datatypes/Basis.h>

/*!
 * We begin by setting the size. We then allocate the required memory for the
 * array and then initialize the elements of the array with using the
 * Gauge::BasisVector::BasisVector() constructor.
 */
Gauge::Basis::Basis(int size) {
  this->size = size;
  this->base = new Gauge::BasisVector[this->size];
  for (int index = 0; index < size; ++index) {
    this->base[index] = Gauge::BasisVector();
  }
}
/*!
 * We begin by setting the size. We then allocate the required memory for the
 * array and then initialize the elements of the array with using the
 * Gauge::BasisVector::BasisVector(int) constructor.
 */
Gauge::Basis::Basis(int size, int width) {
  this->size = size;
  this->base = new Gauge::BasisVector[this->size];
  for (int index = 0; index < this->size; ++index) {
    this->base[index] = Gauge::BasisVector(width);
  }
}
/*!
 * The copy constructor simply copies the provided Gauge::Basis
 * to @c *this using the data copy method.
 */
Gauge::Basis::Basis(const Gauge::Basis& other) {
  size = other.size;
  base = new Gauge::BasisVector[size];
  for (int index = 0; index < size; ++index) base[index] = other.base[index];
}
/*!
 * The assignment operator checks that we aren't reassigning to the
 * same instence. If we aren't then we perform a copy in much the same
 * way that the copy constructor does.
 */
Gauge::Basis& Gauge::Basis::operator=(const Gauge::Basis& other) {
  if (this != &other) {
    if (base != NULL && size != other.size) delete [] base;
    if (base == NULL) base = new Gauge::BasisVector[other.size];
    size = other.size;
    for (int index = 0; index < size; ++index)
      base[index] = other.base[index];
  }
  return *this;
}
/*!
 * The equality operator simply checks each basis vector in the set for
 * equality taking a strict interpretation of equality: order matters.
 *
 * Physically the order of the basis vectors is irrelevant, but we will be using
 * the equality to perform instance comparisons, not physical comparisons. For
 * example, to ensure that the copy constructor copies correctly.
 */
bool Gauge::Basis::operator==(const Gauge::Basis &other) const {
  if (size != other.size) return false;
  for (int index = 0; index < size; ++index)
    if (base[index] != other.base[index]) return false;
  return true;
}

// Printable Interface
void Gauge::Basis::PrintTo(std::ostream *out) const {
  for (int index = 0; index < size; ++index) {
    *out << base[index] << std::endl;
  }
}

void Gauge::Basis::SerializeWith(Gauge::Serializer *serializer) const {
  // The size of our basis is simply the layer and is thus limited to about 20,
  // so we can compress to char.
  serializer->Write<char>(size);
  serializer->WriteObject(base, base + size);
}

void Gauge::Basis::DeserializeWith(Gauge::Serializer *serializer) {
  if (base != NULL) delete [] base;
  serializer->Read<char>(&size);
  base = new Gauge::BasisVector[size];
  serializer->ReadObject(base, base + size);
}
