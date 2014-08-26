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
 * @file src/Math.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 04.09.2012
 *
 * @brief We defined the non-template methods found in the Gauge::Math
 * namespace.
 */

// System Headers
#include <algorithm>

// Framework Headers
#include <Math.h>

int Gauge::Math::GCD(int alpha, int beta) {
  if (beta == 0) return alpha;
  return Gauge::Math::GCD(beta, alpha % beta);
}

int Gauge::Math::LCM(int alpha, int beta) {
  return alpha * beta / Gauge::Math::GCD(alpha,beta);
}

int Gauge::Math::ISqrt(int alpha) {
  return static_cast<int>(floor(sqrt(alpha)));
}
