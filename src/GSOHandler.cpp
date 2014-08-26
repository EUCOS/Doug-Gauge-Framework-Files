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
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * The Gauge Framework. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * @file src/GSOHandler.cpp
 * @author D. More <douglas_moore1@baylor.edu>
 * @date 04.05.2012
 *
 * @brief The Gauge::GSOHandler class is responsible for constructing the GSO
 * projection matrix (Gauge::GSOMatrix) from the basis vectors (Gauge::Basis).
 * In addition, a static method will take care of determining if a Gauge::Charge
 * is a physical state or not.
 *
 * In its current state, the Gauge::GSOHandler class can only initialize
 * Gauge::GSOMatrix instances.
 */

#include <GSOHandler.h>
#include <Math.h>

Gauge::GSOHandler::GSOHandler() {
  first_ = true;
  orders_ = NULL;
  products_ = NULL;
  setup_ = false;
  susy_type_ = Gauge::Input::kSUSY;
  extra_layers_ = 2;
}

Gauge::GSOHandler::~GSOHandler() {
  ClearOrders();
  ClearProducts();
}

void Gauge::GSOHandler::Setup(const Gauge::Basis &basis,
                              Gauge::Input::SUSYType susy_type) {
  ClearOrders();
  ClearProducts();

  susy_type_ = susy_type;

  extra_layers_ = (susy_type != Gauge::Input::kNonSUSY) ? 2 : 1;
  kij_ = Gauge::GSOMatrix(basis.size + extra_layers_);
  orders_ = new int[basis.size + extra_layers_];
  products_ = new Gauge::Math::Rational*[basis.size + extra_layers_];
  for (int row = 0; row < basis.size + extra_layers_; ++row) {
    if (row < extra_layers_) {
      orders_[row] = 2;
    } else {
      orders_[row] = basis.base[row - extra_layers_].order;
    }
    products_[row] = new Gauge::Math::Rational[row+1];
  }

  ComputeProducts(basis);
  first_ = true;
  setup_ = true;
}

bool Gauge::GSOHandler::NextGSOMatrix() {
  assert(setup_);
  return (first_ && FirstGSOMatrix()) || Next();
}

bool Gauge::GSOHandler::Project(const Gauge::Geometry &geometry,
                                const Gauge::State &state,
                                const int *coefficients) {
  using namespace Gauge::Math;
  const Gauge::Basis &basis = geometry.basis;
  const Gauge::GSOMatrix &gso = geometry.gso_matrix;
  Gauge::Math::Rational value;

  int extra_layers = gso.size - basis.size;

  // All Periodic Basis Vector
  value = Product(state, Gauge::kPeriodicBasisVector);
  if (!PassesProjection(&value, gso, 0, coefficients, extra_layers)) {
    return false;
  }

  // SUSY Basis Vector
  if (extra_layers == 2) {
    value = Rational(0);
    if (!PassesProjection(&value, gso, 1, coefficients, extra_layers)) {
      return false;
    }
  }

  // The Rest of the Basis
  for (int vector = 0; vector < basis.size; ++vector) {
    value = Product(state, basis.base[vector]);
    if (!PassesProjection(&value, gso, vector + extra_layers,
                          coefficients, extra_layers)) {
      return false;
    }
  }
  return true;
}

void Gauge::GSOHandler::ClearOrders() {
  if (orders_ != NULL) {
    delete [] orders_;
    orders_ = NULL;
  }
}

void Gauge::GSOHandler::ClearProducts() {
  if (products_ != NULL) {
    for (int i = 0; i < kij_.size; ++i) {
      if (products_[i] != NULL) {
        delete [] products_[i];
        products_[i] = NULL;
      }
    }
    delete [] products_;
    products_ = NULL;
  }
}

void Gauge::GSOHandler::ComputeProducts(const Gauge::Basis &basis) {
  for (int row = 0; row < basis.size + extra_layers_; ++row) {
    for (int column = 0; column <= row; ++column) {
      if (column == 0) {
        if (row == 0) {
          products_[row][column] =
            Gauge::Math::Rational(basis.base[0].size);
        } else if (row >= extra_layers_) {
          products_[row][column] =
              Gauge::Math::Product(basis.base[row - extra_layers_],
                                   Gauge::kPeriodicBasisVector);
          Reduce(&products_[row][column]);
        }
      } else if (column >= extra_layers_) {
          products_[row][column] =
              Gauge::Math::Product(basis.base[row - extra_layers_],
                                   basis.base[column - extra_layers_]);
          Reduce(&products_[row][column]);
      } else {
        products_[row][column] = Gauge::Math::Rational(0);
      }
    }
  }
}

bool Gauge::GSOHandler::FirstGSOMatrix() {
  for (int row = 0; row < kij_.size; ++row) {
    for (int column = 0; column < row; ++column) {
      assert(MinimizeElement(row,column));
    }
  }
  first_ = false;
  return Validate();
}

bool Gauge::GSOHandler::MinimizeElement(int row, int column) {
  int order = orders_[column];
  Gauge::Math::Rational element(-2 * (order / 2 - 1 + (order & 1)),order);
  Reduce(&element);
  while (!SetElement(row, column, element)) {
    Reduce(Add(&element, Gauge::Math::Rational(2,order)));
    if (element > 1) return false;
  }
  return true;
}

bool Gauge::GSOHandler::SetElement(int row, int column,
                                   const Gauge::Math::Rational &element) {
  if (column == 1 && susy_type_ == Gauge::Input::kFullSUSY) {
    if (element != 0) return false;
  }
  Gauge::Math::Rational tnemele, diagonal;
  if (!ComputeOffDiagonal(tnemele, row, column, element)) return false;
  if (column == 0 && !ComputeDiagonal(diagonal, row, element)) return false;
  kij_.base[row][column] = element;
  kij_.base[column][row] = tnemele;
  if (column == 0) kij_.base[row][row] = diagonal;
  return true;
}

bool Gauge::GSOHandler::ComputeOffDiagonal(Gauge::Math::Rational &tnemele,
    int row, int column, const Gauge::Math::Rational &element) {

  tnemele = Divide(products_[row][column],2);
  Reduce(Mod(Subtract(&tnemele, element), 2));

  int lcm = abs(Gauge::Math::LCM(tnemele.den, orders_[row]));
  if (lcm != orders_[row]) return false;
  int common = lcm / tnemele.den;
  Multiply(&tnemele, Gauge::Math::Rational(common, common));
  if (tnemele.num & 1) return false;
  Reduce(Cycle(&tnemele));
  return true;
}

bool Gauge::GSOHandler::ComputeDiagonal(Gauge::Math::Rational &diagonal,
    int row, const Gauge::Math::Rational &element) {
  diagonal = Divide(products_[row][row], 4);
  Reduce(Mod(Subtract(&diagonal, element), 2));

  int lcm = abs(Gauge::Math::LCM(diagonal.den, orders_[row]));
  if (lcm != orders_[row]) return false;
  int common = lcm / diagonal.den;
  Multiply(&diagonal, Gauge::Math::Rational(common, common));
  if (diagonal.num & 1) return false;
  Reduce(Cycle(&diagonal));
  return true;
}

bool Gauge::GSOHandler::Next() {
  for (int row = 2; row < kij_.size; ++row) {
    for (int column = 1; column < row; ++column) {
      if (IncrementElement(row,column)) {
        if (!Validate()) return Next();
        return true;
      }
      assert(MinimizeElement(row,column));
    }
  }
  return false;
}

bool Gauge::GSOHandler::IncrementElement(int row, int column) {
  int order = orders_[column];
  Gauge::Math::Rational element = kij_.base[row][column];
  Reduce(Add(&element, Gauge::Math::Rational(2,order)));
  if (element > 1) return false;
  while (!SetElement(row, column, element)) {
    Reduce(Add(&element, Gauge::Math::Rational(2,order)));
    if (element > 1) return false;
  }
  return true;
}

bool Gauge::GSOHandler::PassesProjection(Gauge::Math::Rational *value,
                                         const Gauge::GSOMatrix &gso, int row,
                                         const int *coeff, int extra) {
  const Gauge::Math::Rational *kij;
  int num = value->num, den = value->den;
  int gso_num, gso_den;
  for (int index = 0; index < gso.size; ++index) {
    kij = &gso.base[row][index];
    gso_num = kij->num;
    gso_den = kij->den;

    num = num * gso_den - coeff[index] * gso_num * den;
    den *= gso_den;
  }
  int gcd = Gauge::Math::GCD(num,den);
  return (num/gcd) % (2 * den/gcd) == 0;
}

bool Gauge::GSOHandler::Validate() const {
  if (susy_type_ != Gauge::Input::kReducedSUSY) {
    return true;
  }
  for (int row = 2; row < kij_.size; ++row) {
    if (kij_.base[row][1] == 1) return true;
  }
  return false;
}
