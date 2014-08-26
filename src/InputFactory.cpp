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
 * @file src/InputFactory.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 05.14.2012
 *
 * @brief An implementation of the Gauge::InputFactory class.
 */

// System Headers
#include <cassert>

// Framework Headers
#include <InputFactory.h>

Gauge::InputFactory::Single::Single(const int *orders, int layers,
    int dimensions, Gauge::Input::SUSYType susy_type) {
  input = Gauge::Input(orders, layers, dimensions, susy_type);
  first = true;
}

bool Gauge::InputFactory::Single::Next() {
  if (first) {
    first = false;
    return true;
  }
  return false;
}

Gauge::InputFactory::Range::Range(const int *lower_bound,
    const int *upper_bound, int layers, int dimensions,
    Gauge::Input::SUSYType susy_type) {
  assert(1 <= layers);
  assert(2 <= dimensions && dimensions <= 10);
  //assert(dimensions % 2 == 0 || susy_type == Gauge::Input::kNonSUSY);
  input.layers = layers;
  input.dimensions = dimensions;
  input.susy_type = susy_type;
  this->lower_bound = new int[layers];
  this->upper_bound = new int[layers];
  input.orders = new int[layers];
  for (int index = 0; index < layers; ++index) {
    assert(2 <= lower_bound[index]);
    assert(2 <= upper_bound[index]);
    if (lower_bound[index] < upper_bound[index]) {
      this->lower_bound[index] = lower_bound[index];
      this->upper_bound[index] = upper_bound[index];
    } else {
      this->lower_bound[index] = upper_bound[index];
      this->upper_bound[index] = lower_bound[index];
    }
    input.orders[index] = lower_bound[index];
  }
  first = true;
}

Gauge::InputFactory::Range::~Range() {
  if (lower_bound != NULL) delete [] lower_bound;
  if (upper_bound != NULL) delete [] upper_bound;
}

bool Gauge::InputFactory::Range::Next() {
  if (first) {
    first = false;
    return true;
  }

  for (int index = 0; index < input.layers; ++index) {
    if (input.orders[index] < upper_bound[index]) {
      ++input.orders[index];
      return true;
    }
    input.orders[index] = lower_bound[index];
  }

  return false;
}
