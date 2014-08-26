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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with The Gauge Framework.  If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * @file src/GSOMatrix.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 04.04.2012
 *
 * @brief An implementation of the Gauge::GSOMatrix class.
 *
 */

#include <cassert>
#include <Datatypes/GSOMatrix.h>

/*!
 * The integer constructor sets the size to the provided value, allocates the
 * memory for the internal array, and initializes the elements of the array to
 * @c 0.
 */
Gauge::GSOMatrix::GSOMatrix(int size) {
  this->size = size;
  base = new Gauge::Math::Rational*[this->size];
  for (int row = 0; row < this->size; ++row) {
    base[row] = new Gauge::Math::Rational[this->size];
    for (int column = 0; column < this->size; ++column) {
      base[row][column] = Gauge::Math::Rational(0);
    }
  }
}
/*!
 * The copy constructor works much as the
 * Gauge::GSOMatrix::GSOMatrix(const Gauge::GSOMatrix::Data&) constructor works,
 * except we must make a call to Gauge::GSOMatrix::data() to get the underlying
 * data from the provided Gauge::GSOMatrix instance.
 *
 * @see Gauge::GSOMatrix::data()
 * @see Gauge::GSOMatrix::data(const Gauge::GSOMatrix::Data&)
 */
Gauge::GSOMatrix::GSOMatrix(const Gauge::GSOMatrix &other) {
  size = other.size;
  base = new Gauge::Math::Rational*[size];
  for (int row = 0; row < size; ++row) {
    base[row] = new Gauge::Math::Rational[size];
    for (int column = 0; column < size; ++column) {
      base[row][column] = other.base[row][column];
    }
  }
}
Gauge::GSOMatrix::~GSOMatrix() {
  if (base != NULL) {
    for (int row = 0; row < size; ++row) {
      if (base[row] != NULL) delete [] base[row];
    }
    delete [] base;
  }
}
/*!
 * The assignment operator makes a call to
 * Gauge::GSOMatrix::data(const Gauge::GSOMatrix::Data&) to copy the contents of
 * the provided Gauge::GSOMatrix instance. Note that we test for
 * self-assignment.
 */
Gauge::GSOMatrix &Gauge::GSOMatrix::operator=(const Gauge::GSOMatrix &other) {
  if (this != &other) {
    if (base != NULL && size != other.size) {
      for (int row = 0; row < size; ++row) {
        if (base[row] != NULL) delete [] base[row];
        base[row] = NULL;
      }
      delete [] base;
      base = NULL;
    }

    size = other.size;
    if (base == NULL) {
      base = new Gauge::Math::Rational*[size];
      for (int row = 0; row < size; ++row) {
        base[row] = new Gauge::Math::Rational[size];
      }
    }

    for (int row = 0; row < size; ++row)
      for (int column = 0; column < size; ++column)
        base[row][column] = other.base[row][column];
  }
  return *this;
}
bool Gauge::GSOMatrix::operator==(const Gauge::GSOMatrix &other) const {
  if (size != other.size) return false;
  for (int row = 0; row < size; ++row)
    for (int col = 0; col < size; ++col)
      if (base[row][col] != other.base[row][col]) return false;
  return true;
}

/*!
 * Gauge::GSOMatrix::PrintTo prints the underlying matrix structure in
 * a 2-dimensional array.
 */
void Gauge::GSOMatrix::PrintTo(std::ostream *out) const {
  if (size > 0) {
    for (int row = 0; row < size; ++row) {
      *out << "| " << base[row][0];
      if (size > 1) {
        for (int column = 1; column < size; ++column) {
          *out << " " << base[row][column];
        }
      }
      *out << " |" << std::endl;
    }
  }
}

void Gauge::GSOMatrix::SerializeWith(Gauge::Serializer *serializer) const {
  serializer->Write<int>(size);
  for (int row = 0; row < size; ++row)
    serializer->WriteObject(base[row], base[row] + size);
}

void Gauge::GSOMatrix::DeserializeWith(Gauge::Serializer *serializer) {
  if (base != NULL) {
    for (int row = 0; row < size; ++row) delete [] base[row];
    delete [] base;
  }

  serializer->Read<int>(&size);

  base = new Gauge::Math::Rational*[size];
  for (int row = 0; row < size; ++row) {
    base[row] = new Gauge::Math::Rational[size];
    serializer->ReadObject(base[row], base[row] + size);
  }
}
