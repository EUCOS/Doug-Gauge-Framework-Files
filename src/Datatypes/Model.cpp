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
 * @file src/Datatypes/Model.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 05.20.2012
 * @brief The Gauge::Model datatype is defined.
 */

#include <Datatypes/Model.h>

Gauge::Model::~Model() {
  if (geometry != NULL) delete geometry;
  if (group != NULL) delete group;
}

bool Gauge::Model::operator==(const Gauge::Model &other) const {
  return (*geometry == *other.geometry) &&
         (*group == *other.group) &&
         (susy == other.susy);
}

void Gauge::Model::PrintTo(std::ostream *out) const {
  *out << "Geometry:";
  if (geometry == NULL) {
    *out << " NULL" << std::endl;
  } else {
    *out << std::endl;
    geometry->PrintTo(out);
  }
  *out << "N = " << susy << std::endl;
  *out << "Group: ";
  if (group == NULL) {
    *out << "NULL";
  } else {
    group->PrintTo(out);
  }
}

void Gauge::Model::SerializeWith(Gauge::Serializer *serializer) const {
  serializer->WriteObject(*group);
  serializer->WriteObject(*geometry);
  // We can have at most N = 4, so we can compress to char.
  serializer->Write<char>(susy);
}

void Gauge::Model::DeserializeWith(Gauge::Serializer *serializer) {
  serializer->ReadObject(group);
  serializer->ReadObject(geometry);
  serializer->Read<char>(&susy);
}
