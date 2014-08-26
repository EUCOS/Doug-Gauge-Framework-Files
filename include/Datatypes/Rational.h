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
 * The Gauge Framework. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * @file Datatypes/Rational.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 04.04.2012
 * @brief The Gauge::Rational class represents rational numbers.
 *
 * The Gauge::Rational is simply a pair of integers, numerator and denominator.
 */

#ifndef GAUGE_FRAMEWORK_RATIONAL_H
#define GAUGE_FRAMEWORK_RATIONAL_H

#include <Interfaces/Printable.h>
#include <Interfaces/Serializable.h>

namespace Gauge {
  namespace Math {
    /*!
     * @brief
     * The Gauge::Math::Rational class is a datatype representing rational
     * numbers.
     *
     * The underlying data is a struct (Gauge::Math::Rational:Data) containing
     * to integers, the numerator (Gauge::Math::Rational::Data::numerator) and
     * denominator (Gauge::Math::Rational::Data::denominator).
     */
    struct Rational : public Gauge::Printable, public Gauge::Serializable {
      int num;  /*!< An integer representation of the numerator. */
      int den;  /*!< An integer representation of the denominator.*/

      /*!
       * Our default constructor sets the numerator to @c 0 and denominator to
       * @c 1.
       */
      Rational() : num(0), den(1) {}
      /*!
       * Our integer constructor sets the numerator to the provided value and
       * denominator to @c 1.
       *
       * @param[in] num The value to which to set the numerator.
       */
      Rational(const int &num) : num(num), den(1) {}
      /*!
       * Our di-integer constructor sets the numerator and the denominator to
       * the values provided.
       *
       * @param[in] num The value to which to set the numerator.
       * @param[in] den The value to which to set the denominator.
       */
      Rational(const int &num, const int &den) : num(num), den(den) {}
      /*!
       * The copy constructor simply copies the contents of the provided
       * Gauge::Math::Rational to @c this,
       *
       * @param[in] other The Gauge::Math::Rational to be copied.
       */
      Rational(const Gauge::Math::Rational &other)
        : num(other.num), den(other.den) {}
      /*!
       * No memory is dynamically allocated in the class so we have a trivial
       * destructor.
       */
      virtual ~Rational() {}
      /*!
       * The assignment operator simply copies the contents of the provided
       * Gauge::Math::Rational to @c this;
       *
       * @param[in] other The Gauge::Math::Rational to be copied.
       */
      Gauge::Math::Rational &operator=(const Gauge::Math::Rational &other);
      /*!
       * The equality operator tests for equality between @c this and the
       * provided Gauge::Math::Rational.
       */
      bool operator==(const Gauge::Math::Rational &other) const {
        return num * other.den == den * other.num;
      }
      /*!
       * The equality operator tests for equality between @c this and the
       * provided integer.
       */
      bool operator==(const int &other) const { return num == other * den; }
      /*!
       * The less-than operator tests for less-than between @c this and the
       * provided Gauge::Math::Rational.
       */
      bool operator< (const Gauge::Math::Rational &other) const {
        return num * other.den < den * other.num;
      }
      /*!
       * The less-than operator tests for less-than between @c this and the
       * provided integer.
       */
      bool operator< (const int &other) const { return num < other * den; }
      /*!
       * The inequality operator tests for equality between @c this and the
       * provided Gauge::Math::Rational.
       */
      bool operator!=(const Gauge::Math::Rational &other) const {
        return !(*this == other);
      }
      /*!
       * The inequality operator tests for equality between @c this and the
       * provided integer.
       */
      bool operator!=(const int &other) const { return !(*this == other); }
      /*!
       * The greater-than operator tests for less-than between @c this and the
       * provided Gauge::Math::Rational.
       */
      bool operator> (const Gauge::Math::Rational &other) const {
        return !(*this == other || *this < other);
      }
      /*!
       * The greater-than operator tests for less-than between @c this and the
       * provided integer.
       */
      bool operator> (const int &other) const {
        return !(*this == other || *this < other);
      }
      /*!
       * The less-than-or-equal operator tests for equality between @c this and
       * the provided Gauge::Math::Rational.
       */
      bool operator<=(const Gauge::Math::Rational &other) const {
        return !(*this > other);
      }
      /*!
       * The less-than-or-equal operator tests for equality between @c this
       * and the provided integer.
       */
      bool operator<=(const int &other) const { return !(*this > other); }
      /*!
       * The greater-than-or-equal operator tests for less-than between @c
       * this and the provided Gauge::Math::Rational.
       */
      bool operator>=(const Gauge::Math::Rational &other) const {
        return !(*this < other);
      }
      /*!
       * The greater-than-or-equal operator tests for less-than between @c
       * this and the provided integer.
       */
      bool operator>=(const int &other) const { return !(*this < other); }

      // Printable Interface
      /*!
       * By defining Gauge::Math::Rational::PrintTo and inheriting from
       * Gauge::Printable, we are given the Gauge::Printable::operator<< and
       * Gauge::Printable::PrintTo(const Printable&, std::ostream*).
       */
      virtual void PrintTo(std::ostream *out) const;

      // Serializable Interface
      virtual void SerializeWith(Gauge::Serializer *serializer) const;
      virtual void DeserializeWith(Gauge::Serializer *serializer);
    };

    /*!
     * This method returns the absolute value of the provided rational number.
     *
     * @param[in] alpha The rational number whose absolute value is required.
     *
     * @return The absolute value of the provided Gauge::Math::Rational.
     */
    Rational Abs(const Rational &alpha);
    /*!
     * This method computes the absolute value of a Gauge::Math::Rational in
     * place.
     *
     * @param[in,out] alpha A pointer to the rational number whose absolute
     * value is desired.
     *
     * @return A pointer to the rational number that was modified.
     */
    Rational *Abs(Rational *alpha);
    /*!
     * This method adds two Gauge::Math::Rational instances and returns the
     * value.
     *
     * @param[in] alpha The first rational number.
     * @param[in] beta  The second rational number.
     *
     * @return The sum of the two provided Gauge::Math::Rational instances.
     */
    Rational Add(const Rational &alpha, const Rational &beta);
    /*!
     * This method adds two Gauge::Math::Rational instances in place of the
     * first. That is, the contents of the first rational number is replaced by
     * the sum.
     *
     * @param[in,out] alpha A pointer to the first rational number.
     * @param[in] beta  The second rational number.
     *
     * @return A pointer to the rational number that was modified.
     */
    Rational *Add(Rational *alpha, const Rational &beta);
    /*!
     * This method subtracts two Gauge::Math::Rational instances and returns the
     * value.
     *
     * @param[in] alpha The first rational number.
     * @param[in] beta  The second rational number.
     *
     * @return The difference of the two provided Gauge::Math::Rational
     * instances.
     */
    Rational Subtract(const Rational &alpha, const Rational &beta);
    /*!
     * This method subtracts two Gauge::Math::Rational instances in place of the
     * first. That is, the contents of the first rational number is replaced by
     * the difference.
     *
     * @param[in,out] alpha A pointer to the first rational number.
     * @param[in] beta  The second rational number.
     *
     * @return A pointer to the rational number that was modified.
     */
    Rational *Subtract(Rational *alpha, const Rational &beta);
    /*!
     * This method computes the product of two Gauge::Math::Rational instances.
     *
     * @param[in] alpha The first rational number.
     * @param[in] beta  The second rational number.
     *
     * @return The product of the two Gauge::Math::Rational instances.
     */
    Rational Multiply(const Rational &alpha, const Rational &beta);
    /*!
     * This method computes the product of two Gauge::Math::Rational instances
     * in place of the first argument.
     *
     * @param[in,out] alpha A pointer to the first rational number.
     * @param[in] beta  The second rational number.
     *
     * @return a pointer to the rational number that was modified.
     */
    Rational *Multiply(Rational *alpha, const Rational &beta);
    /*!
     * We can also divide Gauge::Math::Rational instance. A C-assertion will
     * terminate the program if you divide by @c 0.
     *
     * @param[in] alpha The first rational number.
     * @param[in] beta The second rational number.
     *
     * @return The ratio of the two rational numbers.
     */
    Rational Divide(const Rational &alpha, const Rational &beta);
    /*!
     * This method divides two Gauge::Math::Rational instances in place of the
     * first. A C-assertion will terminate the program if you divide by @c 0.
     *
     * @param[in,out] alpha The first rational number.
     * @param[in] beta The second rational number.
     *
     * @return A pointer to the rational number that was modified.
     */
    Rational *Divide(Rational *alpha, const Rational &beta);
    /*!
     * Of course, we can compute the mod of two rational numbers.
     *
     * @param[in] alpha A pointer to the first rational number.
     * @param[in] beta The second rational number.
     *
     * @return The mod of the two numbers.
     */
    Rational Mod(const Rational &alpha, const Rational &beta);
    /*!
     * Here we perform the mod of two rational number in place of the first.
     *
     * @param[in] alpha The first rational number.
     * @param[in] beta The second rational number.
     *
     * @return A pointer to the rational number that was modified.
     */
    Rational *Mod(Rational *alpha, const Rational &beta);
    /*!
     * Sometimes it would be nice to reduce a Gauge::Math::Rational. This method
     * returns a reduced version of the provided value.
     *
     * @param[in] alpha The rational number to reduce.
     *
     * @return The reduced rational.
     */
    Rational Reduce(const Rational &alpha);
    /*!
     * Sometimes it would be nice to reduce a Gauge::Math::Rational in place.
     *
     * @param[in,out] alpha A pointer to the rational number to reduce in place.
     *
     * @return A pointer to the rational number that was modified.
     */
    Rational *Reduce(Rational *alpha);
    /*!
     * Often we need our rational numbers to be between @c 1 and @c -1. We can
     * enforce that with this method.
     *
     * @param[in] alpha The rational number to cylce.
     * @return The a copy of the Gauge::Rational cycled.
     */
    Rational Cycle(const Rational &alpha);
    /*!
     * Often we need our rational numbers to be between @c 1 and @c -1. We can
     * enforce that with this method. This is performed in place.
     *
     * @param[in,out] alpha The rational number to cylce.
     *
     * @return A pointer to the rational number that was modified.
     */
    Rational *Cycle(Rational *alpha);
  }
}

#endif
