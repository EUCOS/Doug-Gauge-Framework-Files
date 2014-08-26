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
 * @file Datatypes/Basis.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 03.18.2012
 *
 * @brief The implementation of the Gauge::NVector datatype, a vector of
 * integers utilized for the generation of gauge basis vector sets.
 */

// System Headers
#include <cassert>
#include <cstdlib>

// Framework Headers
#include <Datatypes/Rational.h>
#include <Math.h>

Gauge::Math::Rational &Gauge::Math::Rational::operator=(const Rational &other) {
  if (this != &other) {
    num = other.num;
    den = other.den;
  }
  return *this;
}

// Printable Interface
void Gauge::Math::Rational::PrintTo(std::ostream *out) const {
  if (num == 0) {
    *out << 0;
  } else if (den == 1) {
    *out << num;
  } else if (den == 0) {
    *out << "INF";
  } else {
    int num_abs = abs(num), den_abs = abs(den);
    int sign = (num / num_abs) * (den / den_abs);
    *out << ((sign > 0) ? "" : "-");
    *out << num_abs << "/" << den_abs;
  }
}

// Serializable Interface
void Gauge::Math::Rational::SerializeWith(Gauge::Serializer *serializer) const {
  // Our numerator and denominator are limited to about 120 so we can compress
  // to char.
  serializer->Write<char>(den);
  serializer->Write<char>(num);
}

void Gauge::Math::Rational::DeserializeWith(Gauge::Serializer *serializer) {
  serializer->Read<char>(&den);
  serializer->Read<char>(&num);
}

Gauge::Math::Rational Gauge::Math::Abs(const Rational &alpha) {
  return Gauge::Math::Rational(abs(alpha.num), abs(alpha.den));
}

Gauge::Math::Rational *Gauge::Math::Abs(Gauge::Math::Rational *alpha) {
  alpha->num = abs(alpha->num);
  alpha->den = abs(alpha->den);
  return alpha;
}

Gauge::Math::Rational Gauge::Math::Add(const Gauge::Math::Rational &alpha,
                                       const Gauge::Math::Rational &beta) {
  assert(alpha.den != 0 && beta.den != 0);
  int num = alpha.num * beta.den + alpha.den * beta.num;
  int den = alpha.den * beta.den;
  int sign = (den < 0) ? -1 : 1;
  return Gauge::Math::Rational(sign * num, sign * den);
}

Gauge::Math::Rational *Gauge::Math::Add(Gauge::Math::Rational *alpha,
                                        const Gauge::Math::Rational &beta) {
  assert(alpha->den != 0 && beta.den != 0);
  int den = alpha->den * beta.den;
  int sign = (den < 0) ? -1 : 1;

  alpha->num = sign * (alpha->num * beta.den + alpha->den * beta.num);
  alpha->den = sign * den;

  return alpha;
}


Gauge::Math::Rational Gauge::Math::Subtract(const Gauge::Math::Rational &alpha,
                                            const Gauge::Math::Rational &beta) {
  assert(alpha.den != 0 && beta.den != 0);
  int num = alpha.num * beta.den - alpha.den * beta.num;
  int den = alpha.den * beta.den;
  int sign = (den < 0) ? -1 : 1;
  return Gauge::Math::Rational(sign * num, sign * den);
}

Gauge::Math::Rational *Gauge::Math::Subtract(Gauge::Math::Rational *alpha,
    const Gauge::Math::Rational &beta) {
  assert(alpha->den != 0 && beta.den != 0);
  int den = alpha->den * beta.den;
  int sign = (den < 0) ? -1 : 1;

  alpha->num = sign * (alpha->num * beta.den - alpha->den * beta.num);
  alpha->den = sign * den;

  return alpha;
}

Gauge::Math::Rational Gauge::Math::Multiply(const Gauge::Math::Rational &alpha,
                                            const Gauge::Math::Rational &beta) {
  assert(alpha.den != 0 && beta.den != 0);
  int sign = (alpha.den * beta.den < 0) ? -1 : 1;
  return Gauge::Math::Rational(sign * alpha.num * beta.num,
                               sign * alpha.den * beta.den);
}

Gauge::Math::Rational *Gauge::Math::Multiply(Gauge::Math::Rational *alpha,
    const Gauge::Math::Rational &beta) {
  assert(alpha->den != 0 && beta.den != 0);
  int sign = (alpha->den * beta.den < 0) ? -1 : 1;
  alpha->num *= sign * beta.num;
  alpha->den *= sign * beta.den;

  return alpha;
}

Gauge::Math::Rational Gauge::Math::Divide(const Gauge::Math::Rational &alpha,
                                          const Gauge::Math::Rational &beta) {
  assert(alpha.den != 0 && beta.num != 0);
  int sign = (alpha.den * beta.num < 0) ? -1 : 1;
  return Gauge::Math::Rational(sign * alpha.num * beta.den,
                               sign * alpha.den * beta.num);
}

Gauge::Math::Rational *Gauge::Math::Divide(Gauge::Math::Rational *alpha,
                                           const Gauge::Math::Rational &beta) {
  assert(alpha->den != 0 && beta.num != 0);

  int sign = (alpha->den * beta.num < 0) ? -1 : 1;
  alpha->num *= sign * beta.den;
  alpha->den *= sign * beta.num;

  return alpha;
}

Gauge::Math::Rational Gauge::Math::Mod(const Gauge::Math::Rational &alpha,
                                       const Gauge::Math::Rational &beta) {
  assert(alpha.den != 0 && beta.num != 0);

  int sign = (alpha.den * alpha.num < 0) ? -1 : 1;
  return Gauge::Math::Rational(
      sign * alpha.num * beta.den % (sign * alpha.den * beta.num),
      sign * alpha.den * beta.den);
}

Gauge::Math::Rational *Gauge::Math::Mod(Gauge::Math::Rational *alpha,
                                        const Gauge::Math::Rational &beta) {
  assert(alpha->den != 0 && beta.num != 0);

  int sign = (alpha->den * alpha->num < 0) ? -1 : 1;
  alpha->num = (sign * beta.den * alpha->num) % (sign * alpha->den * beta.num);
  alpha->den *= sign * beta.den;

  return alpha;
}

Gauge::Math::Rational Gauge::Math::Reduce(const Gauge::Math::Rational &alpha) {
  int gcd = Gauge::Math::GCD(alpha.num, alpha.den);
  int sign = (alpha.den < 0) ? -1 : 1;
  if (gcd == 1 && sign == 1) return alpha;
  return Gauge::Math::Rational(sign * alpha.num / gcd, sign * alpha.den / gcd);
}

Gauge::Math::Rational *Gauge::Math::Reduce(Gauge::Math::Rational *alpha) {
  int gcd = Gauge::Math::GCD(alpha->num, alpha->den);
  int sign = (alpha->den < 0) ? -1 : 1;
  if (gcd != 1 || sign != 1) {
    alpha->num /= (sign * gcd);
    alpha->den /= (sign * gcd);
  }

  return alpha;
}

Gauge::Math::Rational Gauge::Math::Cycle(const Gauge::Math::Rational &alpha) {
  Gauge::Math::Rational tmp = alpha;
  Cycle(&tmp);
  return tmp;
}

Gauge::Math::Rational *Gauge::Math::Cycle(Gauge::Math::Rational *alpha) {
  if (*alpha == Gauge::Math::Rational(-1)) {
    Multiply(alpha, -1);
  }

  while (*alpha < Gauge::Math::Rational(-1)) {
    Add(alpha, Gauge::Math::Rational(2*alpha->den, alpha->den));
  }

  while (*alpha > Gauge::Math::Rational(1)) {
    Subtract(alpha, Gauge::Math::Rational(2*alpha->den, alpha->den));
  }

  return alpha;
}
