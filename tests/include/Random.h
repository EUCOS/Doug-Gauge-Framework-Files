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
 * @file tests/include/Random.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 09.07.2012
 *
 * @brief This simple library gives us randomization for the primative types to
 * be used in testing.
 */

#pragma once

// C Headers
#include <cassert>
#include <cstdlib>
// C++ Headers
#include <algorithm>
#include <numeric>
// Gauge Framework Headers
#include <Datatypes/Basis.h>
#include <Datatypes/BasisVector.h>
#include <Datatypes/Input.h>
#include <Datatypes/GSOMatrix.h>
#include <Datatypes/Geometry.h>
#include <Datatypes/Group.h>
#include <Datatypes/Model.h>
#include <Datatypes/NVector.h>
#include <Datatypes/Rational.h>
#include <Datatypes/Raw.h>
#include <Datatypes/Sector.h>
#include <Datatypes/State.h>
#include <Datatypes/Vector.h>

namespace Random {
  inline void Seed() { srand(time(NULL)); }

  inline int Int(int min, int max) {
    assert(min < max);
    return rand() % (max - min) + min;
  }

  inline char Char(char min, char max) {
    assert(min < max);
    return static_cast<char>(rand() % (max - min) + min);
  }

  inline int *IntArray(int min, int max, size_t size) {
    int *base = new int[size];
    for (int *iter = base; iter != base + size; ++iter)
      *iter = Random::Int(min,max);
    return base;
  }

  inline char *CharArray(char min, char max, size_t size) {
    char *base = new char[size];
    for (char *iter = base; iter != base + size; ++iter)
      *iter = Random::Char(min,max);
    return base;
  }

  inline Gauge::NVector *NVector(int size) {
    Gauge::NVector *vector = new Gauge::NVector();
    vector->size = size;
    vector->base = Random::IntArray(-100, 100, size);
    return vector;
  }

  inline Gauge::Math::Rational *Rational() {
    int num = Random::Int(-100,100);
    int den = Random::Int(1,100);
    return new Gauge::Math::Rational(num, den);
  }

  inline Gauge::Raw *Raw(int size) {
    Gauge::Raw *raw = new Gauge::Raw();
    raw->size = size;
    raw->data = Random::CharArray(' ','~', size);
    return raw;
  }

  inline Gauge::Vector *Vector(int size) {
    int den = Random::Int(2,10);
    Gauge::Vector *vector = new Gauge::Vector(size, den);
    for(int *iter = vector->begin; iter != vector->end; ++iter)
      *iter = Random::Int(2,100);
    for (int index = 0; index < vector->size; ++index)
      if (vector->base[index] != 0) {
        vector->leading = index;
        break;
      }
    for (int index = vector->size; index > 0; --index)
      if (vector->base[index-1] != 0) {
        vector->trailing = index;
        break;
      }
    return vector;
  }

  inline Gauge::State *State(int size) {
    int den = Random::Int(2,10);
    Gauge::State *vector = new Gauge::State(size, den);
    for(int *iter = vector->begin; iter != vector->end; ++iter)
      *iter = Random::Int(2,100);
    for (int index = 0; index < vector->size; ++index)
      if (vector->base[index] != 0) {
        vector->leading = index;
        break;
      }
    for (int index = vector->size; index > 0; --index)
      if (vector->base[index-1] != 0) {
        vector->trailing = index;
        break;
      }
    return vector;
  }

  inline Gauge::Sector *Sector(int size) {
    int den = Random::Int(2,10);
    Gauge::Sector *vector = new Gauge::Sector(size, den);
    for(int *iter = vector->begin; iter != vector->end; ++iter)
      *iter = Random::Int(2,100);
    for (int index = 0; index < vector->size; ++index)
      if (vector->base[index] != 0) {
        vector->leading = index;
        break;
      }
    for (int index = vector->size; index > 0; --index)
      if (vector->base[index-1] != 0) {
        vector->trailing = index;
        break;
      }
    return vector;
  }

  inline Gauge::BasisVector *BasisVector(int size) {
    int den = Random::Int(2,10);
    Gauge::BasisVector *vector = new Gauge::BasisVector(size, den);
    for(int *iter = vector->begin; iter != vector->end; ++iter)
      *iter = Random::Int(2,100);
    for (int index = 0; index < vector->size; ++index)
      if (vector->base[index] != 0) {
        vector->leading = index;
        break;
      }
    for (int index = vector->size; index > 0; --index)
      if (vector->base[index-1] != 0) {
        vector->trailing = index;
        break;
      }
    return vector;
  }

  inline Gauge::Basis *Basis(int size, int width) {
    Gauge::Basis *basis = new Gauge::Basis(size);
    Gauge::BasisVector *holder;
    for (int index = 0; index < size; ++index) {
      holder = Random::BasisVector(width);
      basis->base[index] = *holder;
      delete holder;
    }

    return basis;
  }

  inline Gauge::Input *Input() {
    Gauge::Input *input = new Gauge::Input();
    input->dimensions = Random::Int(2,5) * 2;
    input->layers = Random::Int(1,20);
    input->orders = Random::IntArray(2,22,input->layers);
    input->susy_type = static_cast<Gauge::Input::SUSYType>(Random::Int(0,3));
    return input;
  }

  inline Gauge::Group::Factor *GroupFactor() {
    Gauge::Group::Factor *factor = new Gauge::Group::Factor();
    factor->cls = Random::Char('A','G');
    factor->rank = Random::Int(1,22);
    return factor;
  }

  inline Gauge::Group *Group() {
    Gauge::Group *group = new Gauge::Group();
    int size = Random::Int(1,10);
    Gauge::Group::Factor *factor;
    while (size > 0) {
      factor = Random::GroupFactor();
      if (factor->rank + group->rank > 127) {
        delete factor;
        break;
      }
      group->factors.insert(factor);
      group->rank += factor->rank;
      --size;
    }
    return group;
  }

  inline Gauge::GSOMatrix *GSOMatrix(int size) {
    Gauge::GSOMatrix *gsomatrix = new Gauge::GSOMatrix(size);
    for (int row = 0; row < size; ++row)
      for (int col = 0; col < size; ++col) {
        Gauge::Math::Rational *rat = Random::Rational();
        gsomatrix->base[row][col] = *rat;
        delete rat;
      }
    return gsomatrix;
  }

  inline Gauge::Geometry *Geometry() {
    int size = Random::Int(2,22);
    int width = Random::Int(2,22);
    Gauge::Basis *basis = Random::Basis(size, width);
    Gauge::GSOMatrix *gso_matrix = Random::GSOMatrix(size);
    Gauge::Geometry *geometry = new Gauge::Geometry(*basis, *gso_matrix);

    delete gso_matrix;
    delete basis;
    return geometry;
  }

  inline Gauge::Model *Model() {
    Gauge::Model *model = new Gauge::Model();
    model->geometry = Random::Geometry();
    model->group = Random::Group();
    model->susy = Random::Int(0,4);
    return model;
  }
}
