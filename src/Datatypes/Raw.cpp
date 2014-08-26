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
 * @file src/Datatypes/Raw.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 09.04.2012
 * @brief This datatype represents raw data.
 */

#include <Datatypes/Raw.h>

Gauge::Raw::Raw(int size) {
  this->size = size;
  this->data = new char[size];
  for (char *iter = data; iter != data + size; ++iter) *iter = '\0';
}

Gauge::Raw::Raw(const Gauge::Raw &other) {
  this->size = other.size;
  this->data = new char[size];

  char *l = this->data;
  char *r = other.data;
  for (; l != data + size; ++l, ++r) *l = *r;
}

Gauge::Raw &Gauge::Raw::operator=(const Gauge::Raw &other) {
  if (this != &other) {
    if (data != NULL) delete [] data;
    this->size = other.size;
    this->data = new char[size];

    char *l = this->data;
    char *r = other.data;
    for (; l != data + size; ++l, ++r) *l = *r;
  }
  return *this;
}

bool Gauge::Raw::operator==(const Gauge::Raw &other) const {
  if (size != other.size) return false;
  for (int index = 0; index < size; ++index)
    if (data[index] != other.data[index]) return false;
  return true;
}
