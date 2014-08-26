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
 * @file Interfaces/Printable.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 03.17.2012
 * @brief The Gauge::Printable interface is defined.
 */

#ifndef GAUGE_FRAMEWORK_PRINTABLE_H
#define GAUGE_FRAMEWORK_PRINTABLE_H

#include <iostream>

namespace Gauge {
  class Printable {
    public:
      virtual ~Printable() {}
      virtual void PrintTo(std::ostream *out) const = 0;
      friend std::ostream& operator<<(std::ostream& out, const Printable& p) {
        p.PrintTo(&out);
        return out;
      }
     friend void PrintTo(const Printable& p, std::ostream *out) {
       p.PrintTo(out);
     }
  };
}

#endif
