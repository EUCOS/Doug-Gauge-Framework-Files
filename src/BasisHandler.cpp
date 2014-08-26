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
 * @file src/BasisHandler.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 04.03.2012
 *
 * @brief An implementation of the Gauge::BasisHandler class.
 *
 * The Gauge::BasisHandler class deals with the creation of Gauge::Basis sets.
 * This is necessary before the creation of Gauge::Kij objects by the
 * Gauge::GeometryFactory and, subsequently, the generation of Gauge::Geometry
 * objects.
 */

// System Headers
#include <cassert>

// Framework Headers
#include <BasisHandler.h>

/*!
 * We simply setup the Gauge::BasisHandler::nvector_handler_ using the inputs
 * provided, initialize the Gauge::BasisHandler::basis_, and set the orders.
 *
 * @see Gauge::NVectorHandler::Setup
 * @see Gauge::Basis::Basis(int,int)
 * @see Gauge::Basis::At
 * @see Gauge::BasisVector.SetOrder
 */
void Gauge::BasisHandler::Setup(const Gauge::Input &input) {
  nvector_handler_.Setup(input.orders, input.layers, 26 - input.dimensions);
  basis_ = Basis(input.layers, 26 - input.dimensions);
  for (int index = 0; index < input.layers; ++index) {
    basis_.base[index].order = input.orders[index];
  }
}

/*!
 * All Gauge::BasisHandler::NextBasis does is make a call to
 * Gauge::NVectorHandler::NextSolution and, if that method returns true
 * indicating a new Gauge::NVector has been found, it calls
 * Gauge::BasisHandler::FillBasis.
 *
 * @see Gauge::NVectorHandler::NextSolution
 * @see Gauge::BasisHandler::FillBasis
 */
bool Gauge::BasisHandler::NextBasis() {
  if (nvector_handler_.NextSolution()) {
    FillBasis();
    return true;
  }
  return false;
}

/*!
 * This method uses the a-value, and a-matrix computed by Gauge::NVectorHandler,
 * as well as the current Gauge::NVector to fill in the basis.
 *
 * @see Gauge::NVectorHandler::avalue
 * @see Gauge::NVectorHandler::amatrix
 * @see Gauge::Basis::Size
 * @see Gauge::NVectorHandler::CurrentSolution
 * @see Gauge::Basis::At
 * @see Gauge::BasisVector::Set
 */
void Gauge::BasisHandler::FillBasis() {
  const int& avalue = nvector_handler_.avalue();
  const int& layer  = basis_.size;
  const int** amatrix = nvector_handler_.amatrix();

  const Gauge::NVector* nvector = nvector_handler_.CurrentSolution();

  for (int vector = 0; vector < layer; ++vector) {
    int index = 0;
    bool found = false;
    for (int n = 0; n < avalue; ++n) {
      if (!found && amatrix[vector][n] != 0) {
        basis_.base[vector].leading = index;
        found = true;
      }
      for (int i = 0; i < nvector->base[n]; ++i, ++index) {
        basis_.base[vector].base[index] = 2*amatrix[vector][n];
      }
    }
    basis_.base[vector].trailing = index;
    for (; index < basis_.base[vector].size; ++index) {
      basis_.base[vector].base[index] = 0;
    }
  }
}
