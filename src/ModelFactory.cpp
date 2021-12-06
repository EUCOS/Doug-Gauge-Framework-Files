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
 * @file src/ModelFactory.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 04.14.2012
 *
 * @brief This file provides an implementation of the Gauge::ModelFactory class.
 */

#include <algorithm>
#include <array>
#include <functional>
#include <iterator>
#include <map>
#include <sstream>
#include <vector>

#include <GSOHandler.h>
#include <Math.h>
#include <ModelFactory.h>

Gauge::ModelFactory::ModelFactory() {
  built_ = false;
  coefficients_ = NULL;
  layer_ = 0;
  number_of_sectors_ = 0;
  orders_ = NULL;
  setup_ = false;
  width_ = 0;
}

Gauge::ModelFactory::~ModelFactory() {
  ClearOrders();
  ClearSectors();
  ClearCoefficients();
  ClearStates();
  ClearGroups();
}

void Gauge::ModelFactory::Setup(const Gauge::Geometry *geometry) {
  assert(geometry != NULL);

  ClearOrders();
  ClearSectors();
  ClearCoefficients();
  ClearStates();
  ClearGroups();

  if (model_.geometry != NULL) delete model_.geometry;
  model_.geometry = new Gauge::Geometry(*geometry);

  const Gauge::Basis &basis = model_.geometry->basis;
  layer_ = basis.size;
  if (layer_ > 0) {
    width_ = basis.base[0].size;
    orders_ = new int[layer_];
    number_of_sectors_ = 1;
    for (int index = 0; index < layer_; ++index) {
      orders_[index] = basis.base[index].order;
      number_of_sectors_ *= orders_[index];
    }
  }
  if (width_ == 16 && model_.geometry->gso_matrix.size - layer_ == 2)
    number_of_sectors_ = 2 * number_of_sectors_ - 1;

  setup_ = true;
  built_ = false;
}

bool Gauge::ModelFactory::Build() {
  assert(setup_);
  if (built_) return built_;
  if (!ConstructSectors()) {
    built_ = true;
    return false;
  }

  ConstructStates();
  ResolveGroups();
  SetSUSY();

  return true;
}

std::string Gauge::ModelFactory::Group() const {
  std::ostringstream stream;
  model_.group->PrintTo(&stream);
  return stream.str();
}

void Gauge::ModelFactory::ClearCoefficients() {
  if(coefficients_ != NULL) {
    for (int index = 0; index < number_of_sectors_; ++index) {
      if (coefficients_[index] != NULL) delete [] coefficients_[index];
    }
    delete [] coefficients_;
  }
  coefficients_ = NULL;
}

void Gauge::ModelFactory::ClearGroups() {
  if (model_.group != NULL) delete model_.group;
  model_.group = NULL;
}

void Gauge::ModelFactory::ClearOrders() {
  if (orders_ != NULL) delete [] orders_;
  orders_ = NULL;
}

void Gauge::ModelFactory::ClearSectors() {
  sectors_.clear();
}

void Gauge::ModelFactory::ClearStates() {
  model_.states.clear();
}

void Gauge::ModelFactory::ConstructCoefficients() {
  ClearCoefficients();

  int extra_layers = model_.geometry->gso_matrix.size - layer_;
  bool ten_dimensions_special = width_ == 16 && extra_layers == 2;

  int length;
  if (ten_dimensions_special) {
    length = (number_of_sectors_ + 1) / 2;
  } else {
    length = number_of_sectors_;
  }

  int **coefficients = new int*[number_of_sectors_];
  for (int index = 0; index < number_of_sectors_; ++index) {
    coefficients[index] = new int[extra_layers + layer_];
    for (int jndex = 0; jndex < extra_layers + layer_; ++jndex) {
      coefficients[index][jndex] = 0;
    }
  }

  int width = model_.geometry->gso_matrix.size;
  for (int row = 1; row < length; ++row) {
    bool raised = false;
    for (int col = extra_layers; col < width; ++col) {
      if (raised) {
        coefficients[row][col] = coefficients[row-1][col];
        continue;
      }
      if (coefficients[row-1][col] == orders_[col - extra_layers] - 1) {
        coefficients[row][col] = 0;
      } else {
        raised = true;
        coefficients[row][col] = coefficients[row-1][col] + 1;
      }
    }
  }

  if (ten_dimensions_special) {
    for (int row = length; row < number_of_sectors_; ++row) {
      coefficients[row][0] = 1;
      coefficients[row][1] = 1;
      for (int col = 2; col < extra_layers + layer_; ++col)
        coefficients[row][col] = coefficients[1 + row - length][col];
    }
  }

  coefficients_ = const_cast<const int**>(coefficients);
}

bool Gauge::ModelFactory::ConstructSectors() {
  assert(model_.geometry != NULL);
  ClearSectors();

  ConstructCoefficients();

  int extra_layers = model_.geometry->gso_matrix.size - layer_;
  int width = model_.geometry->gso_matrix.size;

  sectors_.reserve(number_of_sectors_);
  sectors_.emplace(end(sectors_), new Gauge::Sector(width_, 0, 1));

  for (int row = 1; row < number_of_sectors_; ++row) {
    Gauge::Sector* sector;
    if (coefficients_[row][0] != 0) {
      sector = new Gauge::Sector(width_, 2, 2);
    } else {
      sector = new Gauge::Sector(width_, 0, 1);
    }

    for (int col = extra_layers; col < width; ++col) {
      const Gauge::BasisVector &bv =
        model_.geometry->basis.base[col - extra_layers];

      Gauge::BasisVector *argument =
        Gauge::Math::Multiply(bv, coefficients_[row][col]);

      Gauge::Math::Cycle(Gauge::Math::Add(sector, *argument));
      delete argument;
    }
    sectors_.emplace(end(sectors_), sector);

    for (int kndex = 0; kndex < width_; ++kndex) {
      if (sectors_[row]->base[kndex] != 0) break;
      if (kndex == width_ - 1) {
        ClearSectors();
        ClearCoefficients();
        return false;
      }
    }
  }

  return true;
}

void Gauge::ModelFactory::ConstructStates() {
  model_.states.BySector() =
      std::vector<std::list<Gauge::State*>>(number_of_sectors_);
  for (int index = 0; index < number_of_sectors_; ++index) {
    Gauge::State *state = new Gauge::State(*sectors_[index]);
    Gauge::Math::Rational mag = Gauge::Math::Magnitude(*state);
    SelectF(0, state, mag.num, mag.den, index);
  }
}

Gauge::Group::Factor *Gauge::ModelFactory::IdentifyGroup(
    const std::list<Gauge::State *> &states) {
  int size = states.size();
  std::array<int,3> classes = {{'A', 'D', 'E'}};
  std::array<int,3> ranks = {{RankA(size), RankD(size), RankE(size)}};
  bool degenerate = std::count_if(begin(ranks), end(ranks),
                                  [](int &x) { return x != 0; }) > 1;
  if (degenerate) {
    int rank = ResolveRank(states);
    bool succeed = std::count_if(begin(ranks), end(ranks),
                                 [rank](int &x) { return rank == x; }) == 1;

    assert(succeed);

    std::transform(begin(ranks), end(ranks), begin(ranks),
                   [rank](int &x) { if (x != rank) return 0; return rank; });
  }

  for (size_t index = 0; index < 3; ++index)
    if (ranks[index] != 0)
      return new Gauge::Group::Factor(classes[index], ranks[index]);

  return new Gauge::Group::Factor();
}

void Gauge::ModelFactory::InitializeSector(int index, int base_index,
                                           int layer) {
  const Gauge::BasisVector *bv;
  if (layer == -1) {
    bv = &Gauge::kPeriodicBasisVector;
  } else {
    bv = &model_.geometry->basis.base[layer];
  }
  std::unique_ptr<Gauge::Sector>& base = sectors_[base_index];

  int den = Gauge::Math::LCM(base->den, bv->order);
  int gcd = Gauge::Math::GCD(base->den, bv->order);

  sectors_.emplace(begin(sectors_) + index, new Gauge::Sector(*base));

  int *start = sectors_[index]->base;
  const int *bv_iter = bv->base;
  for (int *val = start; val < start + width_; ++val, ++bv_iter) {
    *val = ((*val) * bv->order + (*bv_iter) * sectors_[index]->den) / gcd;
  }

  sectors_[index]->den = den;

  for (int *val = start; val < start + width_; ++val) {
    if (*val == -den) {
      *val *= -1;
      continue;
    }
    while (*val < -den) *val += 2 * den;
    while (*val > den) *val -= 2 * den;
  }
}

void Gauge::ModelFactory::LowerState(int index, const Gauge::State *state,
                                     int n, int d, int sector) {
  Gauge::State *new_state = new Gauge::State(*state);
  new_state->base[index] -= new_state->den;
  SelectF(index + 1, new_state, n, d, sector);
}

void Gauge::ModelFactory::RaiseState(int index, const Gauge::State *state,
                                     int n, int d, int sector) {
  Gauge::State *new_state = new Gauge::State(*state);
  new_state->base[index] += new_state->den;
  if (index < state->leading) new_state->leading = index;
  SelectF(index + 1, new_state, n, d, sector);
}

int Gauge::ModelFactory::RankA(int size) const {
  if (size < 1 || size > 253) return 0;
  double rank_double = (-1 + pow(1 + 8 * size, 0.5)) / 2;
  int rank_integer = static_cast<int>(floor(rank_double));
  if (rank_integer == rank_double) return rank_integer;
  return 0;
}

int Gauge::ModelFactory::RankD(int size) const {
  if (size < 12 || size > 462) return 0;
  double rank_double = (1 + pow(1 + 4 * size, 0.5)) / 2;
  int rank_integer = static_cast<int>(floor(rank_double));
  if (rank_integer == rank_double) return rank_integer;
  return 0;
}

int Gauge::ModelFactory::RankE(int size) const {
  switch(size) {
    case  36 : return 6; break;
    case  63 : return 7; break;
    case 120 : return 8; break;
    default  : return 0;
  }
  return 0;
}

int Gauge::ModelFactory::ResolveRank(const std::list<Gauge::State *> &states) const {
  typedef std::function<bool(const Gauge::State*,const Gauge::State*)> Compare;
  typedef std::map<Gauge::State *, bool, Compare> maptype;

  int count = states.size();
  maptype nonsimple(less);
  for (std::list<Gauge::State *>::const_iterator p = states.begin();
       p != states.end(); ++p) {
    nonsimple[*p] = false;
  }
  Gauge::State *sum;
  for (std::list<Gauge::State *>::const_iterator p = states.begin();
       p != states.end(); ++p) {
    for (std::list<Gauge::State *>::const_iterator q = std::next(p);
         q != states.end(); ++q) {
      sum = Gauge::Math::Add(**p,**q);
      if (Gauge::Math::Magnitude(*sum) == 2) {
        typename maptype::iterator location = nonsimple.find(sum);

        if (location != nonsimple.end() && !location->second) {
          --count;
          location->second = true;
        }
      }
      delete sum;
    }
  }
  return count;
}

void Gauge::ModelFactory::ResolveGroups() {
  typedef std::list<std::list<Gauge::State*>>::iterator iterator;
  std::list<std::list<Gauge::State*>>& adjoints = model_.states.ByGroup();

  ClearGroups();
  model_.group = new Gauge::Group();
  for (iterator roots = adjoints.begin(); roots != adjoints.end(); ++roots)
    model_.group->factors.insert(IdentifyGroup(*roots));
}

void Gauge::ModelFactory::SelectF(int index, Gauge::State *state,
                                  int n, int d, int sector) {
  int leading_index = state->leading;
  int leading_value =
    (leading_index < width_) ? state->base[state->leading] : 0;

  if (index > leading_index &&
      leading_index < width_ &&
      leading_value <= 0) {
    delete state;
    return;
  } else if (index < state->size) {
    int alpha = 2 * state->base[index];
    int beta = state->den;

    int raised_n = n * beta + d * beta + d * alpha;
    int lowered_n = n * beta + d * beta - d * alpha;
    int den = d * beta;

    bool can_raise = raised_n <= 2 * den;
    bool can_lower = lowered_n <= 2 * den;

    int gcd;
    if (can_raise) {
      gcd = Gauge::Math::GCD(raised_n, den);
      RaiseState(index, state, raised_n/gcd, den/gcd, sector);
    }

    if (can_lower && index > leading_index) {
      gcd = Gauge::Math::GCD(lowered_n, den);
      LowerState(index, state, lowered_n/gcd, den/gcd, sector);
    }

    SelectF(index + 1, state, n, d, sector);
    return;
  } else if (n == 2*d) {
    assert(leading_index != width_ && leading_value > 0);
    int trailing = width_ - 1;
    while(trailing > -1 && state->base[trailing] == 0) --trailing;
    ++trailing;
    state->trailing = trailing;
    if (Gauge::GSOHandler::Project(*model_.geometry, *state,
                                   coefficients_[sector])) {
      model_.states.insert(state, sector);
      return;
    }
  }
  delete state;
}

void Gauge::ModelFactory::SetSUSY() {
  const Gauge::Geometry &geometry = *model_.geometry;
  int extra = geometry.gso_matrix.size - geometry.basis.size;
  if (extra == 2) {
    for (int index = extra; index < geometry.gso_matrix.size; ++index) {
      if (geometry.gso_matrix.base[index][1] == 1) {
        model_.susy = 0;
        return;
      }
    }
    int dim = 26 - geometry.basis.base[geometry.basis.size-1].size;
    if (dim & 1)
      model_.susy = 3;
    else if (dim == 10)
      model_.susy = 1;
    else if (dim == 4)
      model_.susy = 4;
    else
      model_.susy = 2;
  } else {
    model_.susy = 0;
  }
}
