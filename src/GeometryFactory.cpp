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
 * @file src/GeometryFactory.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 04.10.2012
 */

#include <cstdlib>
#include <cassert>

#include <GeometryFactory.h>

Gauge::GeometryFactory::GeometryFactory(Gauge::BasisHandler *basis_handler,
                                        Gauge::GSOHandler *gso_handler) {
  this->input_factory_ = NULL;
  this->basis_handler_ = basis_handler;
  this->gso_handler_ = gso_handler;
  this->geometry_ = new Gauge::Geometry();

  first_ = true;
  setup_ = false;
}

Gauge::GeometryFactory::~GeometryFactory() {
  if (input_factory_ != NULL) delete input_factory_;
  if (basis_handler_ != NULL) delete basis_handler_;
  if (gso_handler_ != NULL) delete gso_handler_;
  if (geometry_ != NULL) delete geometry_;
}

Gauge::GeometryFactory *Gauge::GeometryFactory::GenericFactory(
    Gauge::BasisHandler *basis_handler, Gauge::GSOHandler *gso_handler) {
  return new GeometryFactory(basis_handler, gso_handler);
}

bool Gauge::GeometryFactory::NextGeometry() {
  assert(setup_ && input_factory_ != NULL &&
         basis_handler_ != NULL && gso_handler_ != NULL);

  if (first_) {
    if (input_factory_->Next()) {
      input_ = &input_factory_->Input();
      basis_handler_->Setup(*input_);
    } else {
      return false;
    }
  }

  if (first_ || !gso_handler_->NextGSOMatrix()) {
    first_ = false;
    while (basis_handler_->NextBasis()) {
      gso_handler_->Setup(basis_handler_->basis(), input_->susy_type);
      if (gso_handler_->NextGSOMatrix()) {
        geometry_->basis = basis_handler_->basis();
        geometry_->gso_matrix = gso_handler_->GSOMatrix();
        return true;
      }
    }
    first_ = true;
    return NextGeometry();
  }
  geometry_->gso_matrix = gso_handler_->GSOMatrix();
  return true;
}

void Gauge::GeometryFactory::Setup(Gauge::InputFactory::Generic *factory) {
  input_factory_ = factory;
  first_ = true;
  setup_ = true;
}

Gauge::GeometryFactory *Gauge::GeometryFactory::SystematicFactory() {
  return new GeometryFactory(new Gauge::BasisHandler(),
                             new Gauge::GSOHandler());
}
