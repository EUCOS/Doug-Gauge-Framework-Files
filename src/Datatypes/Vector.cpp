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
 * @file Datatypes/Vector.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 03.16.2012
 *
 * @brief The implementation of the Gauge::NVector datatype, a vector of
 * integers utilized for the generation of gauge basis vector sets.
 */

// C Headers
#include <cassert>
#include <cstdlib>

// Gauge Framework Headers
#include <Datatypes/Vector.h>

/*!
 * This constructor initializes the base array to @c NULL, sets the denominator
 * to @c 1 and the size to @c 0.
 *
 * Note that the default constructor does not dynamically allocate any memory.
 * That will be handled by Gauge::Vector::operator= or Gauge::Vector::data when,
 * and if, they are called.
 */
Gauge::Vector::Vector() {
  base = NULL;
  den  = 1;
  size = 0;

  begin = base;
  end = begin + size;

  leading = 0;
  trailing = 0;
}

/*!
 * This constructor initializes the size to the argument provided, sets the
 * denominator to @c 1, allocates the memory for the base array and initializes
 * the allocated base to @c 0.
 */
Gauge::Vector::Vector(int size) {
  this->size = size;
  den  = 1;
  base = new int[this->size];
  for (int index = 0; index < this->size; ++index) base[index] = 0;

  begin = base;
  end = begin + this->size;

  leading = this->size;
  trailing = 0;
}

/*!
 * This constructor initializes the size to the value of the first argument,
 * sets the denominator to the value of the second arugment, allocates the
 * memory for the base array and initializes the allocated base to the @c 0.
 */
Gauge::Vector::Vector(int size, int denominator) {
  this->size = size;
  den  = denominator;
  base = new int[this->size];
  for (int index = 0; index < this->size; ++index) base[index] = 0;

  begin = base;
  end = begin + this->size;

  leading = this->size;
  trailing = 0;
}

/*!
 * This constructor initializes the size to the value of the first argument,
 * sets the denominator to the value of the third arugment, allocates the memory
 * for the base array and initializes the allocated base to the value of the
 * second argument.
 */
Gauge::Vector::Vector(int size, int numerator, int denominator) {
  this->size = size;
  den  = denominator;
  base = new int[this->size];
  for (int index = 0; index < this->size; ++index) base[index] = numerator;

  begin = base;
  end = begin + this->size;

  leading = 0;
  trailing = size;
}

/*!
 * This constructor copies the contents of the provided Gauge::Vector via
 * a call to the Gauge::Vector::data(const Gauge::Vector::Data&).
 *
 * @see Gauge::Vector::data()
 * @see Gauge::Vector::data(const Gauge::Vector::Data&)
 */
Gauge::Vector::Vector(const Gauge::Vector& other) {
  size = other.size;
  leading = size;
  trailing = 0;
  if (other.base != NULL) {
    base = new int[size];
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

/*!
 * The assignment operator makes a call to
 * Gauge::Vector::data(const Gauge::Vector::Data&) to copy the contents of the
 * provided Gauge::Vector.
 *
 * @see Gauge::Vector::data()
 * @see Gauge::Vector::data(const Gauge::Vector::Data&)
 *
 */
Gauge::Vector& Gauge::Vector::operator=(const Gauge::Vector& other) {
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

bool Gauge::Vector::operator==(const Gauge::Vector &other) const {
  if (size != other.size ||
      leading != other.leading || trailing != other.trailing) {
    return false;
  }
  if (leading == size) return true;
  int left_den = den;
  int right_den = other.den;
  const int *left_i = begin + leading;
  const int *right_i = other.begin + leading;
  const int *end = begin + trailing;
  for (; left_i != end; ++left_i, ++right_i) {
    if (*left_i * right_den != *right_i * left_den) return false;
  }
  return true;
}

bool Gauge::Vector::operator< (const Gauge::Vector &other) const {
  if (size < other.size || leading > other.leading) {
    return true;
  } else if (size > other.size || leading < other.leading) {
    return false;
  }

  int left_den = den;
  int right_den = other.den;
  const int *left_i = begin + leading;
  const int *right_i = other.begin + leading;
  const int *end = begin + trailing;
  int left, right;
  for (; left_i != end; ++left_i, ++right_i) {
    left = *left_i * right_den;
    right = *right_i * left_den;
    if (left < right) return true;
    if (left > right) return false;
  }
  return false;
}

// Printable Interface
void Gauge::Vector::PrintTo(std::ostream *out) const {
  *out << "[ ";
  if (size > 0) {
    *out << *begin;
    if (size > 1)
      for (const int *iter = begin + 1; iter != end; ++iter)
        *out << " " << *iter;
  }
  *out << " ] (" << den << ")";
}

void Gauge::Vector::SerializeWith(Gauge::Serializer *serializer) const {
  // Our vectors don't exceed about 22 elements in length, so we can compress to
  // char.
  serializer->Write<char>(size);
  // We don't expect our numerators to exeed about 50 so we can compress to
  // char.
  serializer->Write<char>(begin,end);
  // Our orders do not exceed 50, so we can compress to char.
  serializer->Write<char>(den);
  // See the comment for the size.
  serializer->Write<char>(leading);
  serializer->Write<char>(trailing);
}

void Gauge::Vector::DeserializeWith(Gauge::Serializer *serializer) {
  if (base != NULL) delete [] base;
  serializer->Read<char>(&size);

  base = new int[size];
  begin = base;
  end = begin + size;

  serializer->Read<char>(begin,end);
  serializer->Read<char>(&den);
  serializer->Read<char>(&leading);
  serializer->Read<char>(&trailing);
}
