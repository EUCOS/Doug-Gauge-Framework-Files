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
 * @file src/Datatypes/Group.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 05.23.2012
 * @brief The Gauge::Group::Factor class is a datatype representing an A,D or E
 * class group factor.
 */

#include <Datatypes/Group.h>

Gauge::Group::Factor::Factor(const Gauge::Group::Factor &other) {
  cls = other.cls;
  rank = other.rank;
}

Gauge::Group::Factor &Gauge::Group::Factor::operator=(
    const Gauge::Group::Factor &other) {
  if (this != &other) {
    cls = other.cls;
    rank = other.rank;
  }
  return *this;
}

void Gauge::Group::Factor::PrintTo(std::ostream *out) const {
  *out << cls << rank;
}

void Gauge::Group::Factor::SerializeWith(Gauge::Serializer *serializer) const {
  serializer->Write<char>(cls);
  // Since the rank of a given group is bounded by 22, so we can compress to
  // char.
  serializer->Write<int>(rank);
}

void Gauge::Group::Factor::DeserializeWith(Gauge::Serializer *serializer) {
  serializer->Read<char,char>(&cls);
  serializer->Read<int,int>(&rank);
}

Gauge::Group::Group(const Gauge::Group &other) {
  rank = 0;
  Gauge::Group::iterator iter = other.begin();
  for (; iter != other.end(); ++iter) {
    factors.insert(new Gauge::Group::Factor(**iter));
    rank += (*iter)->rank;
  }
}

Gauge::Group::~Group() {
  for (Gauge::Group::iterator iter = begin(); iter != end(); ++iter)
    if (*iter != NULL) delete *iter;
}

Gauge::Group &Gauge::Group::operator=(const Gauge::Group &other) {
  if (this != &other) {
    rank = other.rank;
    Gauge::Group::iterator iter = other.begin();
    while (iter != other.end()) {
      factors.insert(new Gauge::Group::Factor(**iter));
    }
  }
  return *this;
}

bool Gauge::Group::operator==(const Gauge::Group &other) const {
  if (rank != other.rank) return false;
  if (factors.size() != other.factors.size()) return false;
  Gauge::Group::iterator lit = begin();
  Gauge::Group::iterator rit = other.begin();
  for (; lit != end(); ++lit, ++rit)
    if (**lit != **rit) return false;
  return true;
}

bool Gauge::Group::operator< (const Gauge::Group &other) const {
  int bound = static_cast<int>(std::min(factors.size(), other.factors.size()));
  Gauge::Group::iterator lit = begin();
  Gauge::Group::iterator rit = other.begin();
  for (int counter = 0; counter < bound; ++bound, ++lit, ++rit) {
    if (**lit < **rit) return true;
    if (**lit > **rit) return false;
  }
  return false;
}

void Gauge::Group::PrintTo(std::ostream *out) const {
  Gauge::Group::const_iterator iter = begin();
  if (iter != end()) (*iter)->PrintTo(out);
  ++iter;
  while (iter != end()) {
    *out << " " << **iter;
    ++iter;
  }
}

void Gauge::Group::SerializeWith(Gauge::Serializer *serializer) const {
  // Our ranks max out at 22, so we can compress to char.
  serializer->Write<char>(rank);
  // Since the rank cannot exceed 22 we can compress to char.
  serializer->Write<int>(static_cast<int>(factors.size()));
  for (Gauge::Group::iterator iter = begin(); iter != end(); ++iter)
    serializer->WriteObject<Gauge::Group::Factor>(**iter);
}

void Gauge::Group::DeserializeWith(Gauge::Serializer *serializer) {
  int size;
  for (Gauge::Group::iterator iter = begin(); iter != end(); ++iter)
    delete *iter;
  factors.clear();
  serializer->Read<char>(&rank);
  serializer->Read<int>(&size);
  for (; size > 0; --size) {
    Gauge::Group::Factor *factor = new Gauge::Group::Factor();
    serializer->ReadObject<Gauge::Group::Factor>(factor);
    factors.insert(factor);
  }
}
