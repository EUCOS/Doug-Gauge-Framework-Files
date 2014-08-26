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
 * @file Datatypes/Vector.h
 * @author Douglas Moore <douglas_moore1@baylor.edu>
 * @date 03.16.2012
 * @brief The Gauge::Vector class is defined.
 *
 * The Gauge::Vector class is a superclass for the three main types of vectors
 * in the Gauge Framework:
 *  - Gauge::BasisVector
 *  - Gauge::Charge
 *  - Gauge::Sector.
 */

#ifndef GAUGE_FRAMEWORK_VECTOR_H
#define GAUGE_FRAMEWORK_VECTOR_H

#include <cassert>

#include <Datatypes/Rational.h>

#include <Interfaces/Printable.h>
#include <Interfaces/Serializable.h>

namespace Gauge {
  /*!
   * @brief
   * The Gauge::Vector class provides the bulk of the code required to implement
   * all of the main vector in the Gauge Framework.
   */
  struct Vector : public Gauge::Printable, public Gauge::Serializable {
    int* base;    /*!< An array to hold the elements of the vector. */
    int den;      /*!< An integer representing the common denominator of the
                    elements of the vector. */
    int size;     /*!< An integer representing the size of the vector.*/

    int *begin;   /*!< A pointer to the beginning of the vector. */
    int *end;     /*!< A pointer to the element after the end of the vector. */

    int leading;  /*!< The index of the first non-zero element. */
    int trailing; /*!< The index after that of the last non-zero element. */

    /*!
     * The default constructor does basic initialization.
     */
    Vector();
    /*!
     * The integer constructor constructs a Gauge::Vector of the size provided
     * as an argument.
     *
     * @param[in] size This is an integer representation of the size of the
     * vector.
     */
    explicit Vector(int size);
    /*!
     * The bi-integer constructor constructs a vector of the requested size with
     * the denominator provided and the numerators initialized to 0.
     *
     * @param[in] size The size of the vector.
     * @param[in] den The common denominator of the vector.
     *
     */
    Vector(int size, int den);
    /*!
     * The tri-integer constructor constructs a vector of the provided size with
     * the possibly non-trivial initial numerator and denominator provided.
     *
     * @param[in] size The size of the vector.
     * @param[in] num The common numerator for the vector.
     * @param[in] den The common denominator of the vector.
     */
    Vector(int size, int num, int den);
    /*!
     * The Gauge::Vector copy constructor copies the contents of the
     * Gauge::Vector reference provided.
     *
     * @param[in] other The Gauge::Vector to be copied.
     */
    Vector(const Gauge::Vector &other);
    /*!
     * The Gauge::Vector utilizes non-trivial dynamic memory allocation.
     * Consequently, we have a non-trival destructor.
     *
     * We make the destructor virtual because several classes will inherit from
     * Gauge::Vector.
     */
    virtual ~Vector() { if (base != NULL) delete [] base; }
    /*!
     * The assignment operator copies the contents of the Gauge::Vector
     * provided into the current context.
     *
     * @param[in] other The Gauge::Vector to be copied.
     * @return Returns a reference to this.
     */
    Vector& operator=(const Gauge::Vector& other);
    /*!
     * The equality operator tests for equality between @c this and the provided
     * Gauge::Vector.
     */
    bool operator==(const Gauge::Vector &other) const;
    /*!
     * The less-than operator tests for less-than between @c this and the
     * provided Gauge::Vector.
     */
    bool operator< (const Gauge::Vector &other) const;
    /*!
     * The inequality operator tests for equality between @c this and the
     * provided Gauge::Vector.
     */
    bool operator!=(const Gauge::Vector &other) const {
      return !(*this == other);
    }
    /*!
     * The greater-than operator tests for less-than between @c this and the
     * provided Gauge::Vector.
     */
    bool operator> (const Gauge::Vector &other) const {
      return !(*this == other || *this < other);
    }
    /*!
     * The less-than-or-equal operator tests for equality between @c this and
     * the provided Gauge::Vector.
     */
    bool operator<=(const Gauge::Vector &other) const {
      return !(*this > other);
    }
    /*!
     * The greater-than-or-equal operator tests for less-than between @c this
     * and the provided Gauge::Vector.
     */
    bool operator>=(const Gauge::Vector &other) const {
      return !(*this < other);
    }

    // Printable Interface
    virtual void PrintTo(std::ostream *out) const;

    // Serializable Interface
    virtual void SerializeWith(Gauge::Serializer *serializer) const;
    virtual void DeserializeWith(Gauge::Serializer *serializer);
  };

  namespace Math {
    /*!
     * This method computes the sum of two vector-like objects (Gauge::Vector)
     * and returns the result as a rational number (Gauge::Math::Rational).
     *
     * Note: If the two vectors are of different size then the program will
     * terminate via C-assertion.
     *
     * @param[in] alpha The first vector.
     * @param[in] beta The second vector.
     *
     * @return The accumulated sum.
     */
    template <class T, class U>
    T *Add(const T &alpha, const U &beta) {
      assert(alpha.size == beta.size);
      T *vector = new T(alpha.size);
      int start = std::min(alpha.leading, beta.leading);
      int stop = std::max(alpha.trailing, beta.trailing);
      if (start >= stop) return vector;
      for (int index = start; index < stop; ++index) {
        vector->base[index] =
          alpha.base[index] * beta.den + alpha.den * beta.base[index];
        if (vector->base[index] != 0) vector->trailing = index + 1;
      }
      vector->den = alpha.den * beta.den;
      vector->leading = start;

      return vector;
    }
    /*!
     * This method computes the sum of two vector-like objects (Gauge::Vector)
     * in place and returns the result as a rational number
     * (Gauge::Math::Rational).
     *
     * Note: If the two vectors are of different size then the program will
     * terminate via C-assertion.
     *
     * @param[in] alpha A pointer to the vector to add to in place.
     * @param[in] beta The second vector.
     */
    template <class T, class U>
    T *Add(T *alpha, const U &beta) {
      assert(alpha->size == beta.size);
      int start = std::min(alpha->leading, beta.leading);
      int stop = std::max(alpha->trailing, beta.trailing);
      if (start >= stop) return alpha;
      for (int index = start; index < stop; ++index) {
        alpha->base[index] =
          alpha->base[index] * beta.den + alpha->den * beta.base[index];
        if (alpha->base[index] != 0) alpha->trailing = index + 1;
      }
      alpha->den = alpha->den * beta.den;
      alpha->leading = start;
      return alpha;
    }
    /*!
     * This method computes the difference of two vector-like objects
     * (Gauge::Vector) and returns the result as a rational number
     * (Gauge::Math::Rational).
     *
     * Note: If the two vectors are of different size then the program will
     * terminate via C-assertion.
     *
     * @param[in] alpha The first vector.
     * @param[in] beta The second vector.
     *
     * @return The accumulated difference.
     */
    template <class T, class U>
    T *Subtract(const T &alpha, const U &beta) {
      assert(alpha.size == beta.size);
      T *vector = new T(alpha.size);
      int start = std::min(alpha.leading, beta.leading);
      int stop = std::max(alpha.trailing, beta.trailing);
      if (start >= stop) return vector;
      bool found = false;
      for (int index = start; index < stop; ++index) {
        vector->base[index] =
          alpha.base[index] * beta.den - alpha.den * beta.base[index];
        if (vector->base[index] != 0) {
          vector->trailing = index + 1;
          if (!found) {
            vector->leading = start;
            found = true;
          }
        }
      }
      vector->den = alpha.den * beta.den;

      return vector;
    }
    /*!
     * This method computes the difference of two vector-like objects
     * (Gauge::Vector) in place and returns the result as a rational number
     * (Gauge::Math::Rational).
     *
     * Note: If the two vectors are of different size then the program will
     * terminate via C-assertion.
     *
     * @param[in] alpha A pointer to the vector to add to in place.
     * @param[in] beta The second vector.
     *
     * @return The accumulated difference.
     */
    template <class T, class U>
    T *Subtract(const T *alpha, const U &beta) {
      assert(alpha.size == beta.size);
      int start = std::min(alpha.leading, beta.leading);
      int stop = std::max(alpha.trailing, beta.trailing);
      if (start >= stop) return;
      bool found = false;
      for (int index = start; index < stop; ++index) {
        alpha->base[index] =
          alpha.base[index] * beta.den - alpha.den * beta.base[index];
        if (alpha->base[index] != 0) {
          alpha->trailing = index + 1;
          if (!found) {
            alpha->leading = start;
            found = true;
          }
        }
      }
      alpha->den = alpha.den * beta.den;
      return alpha;
    }
    /*!
     * This function multiplies the provided vector-like object by the provided
     * rational number.
     *
     * @param[in] alpha The vector.
     * @param[in] beta The Gauge::Math::Rational value by which to multiply.
     *
     * @return A pointer to the new vector instance.
     */
    template <class T>
    T *Multiply(const T &alpha, const Gauge::Math::Rational &beta) {
      T *vector = new T(alpha.size);
      if (alpha.leading >= alpha.trailing) return vector;
      for (int index = alpha.leading; index < alpha.trailing; ++index) {
        vector->base[index] = alpha.base[index] * beta.num;
      }
      vector->den = alpha.den * beta.den;
      vector->leading = alpha.leading;
      vector->trailing = alpha.trailing;

      return vector;
    }
    /*!
     * This function multiplies the provided vector-like object by the provided
     * rational number IN PLACE.
     *
     * @param[in] alpha A pointer to the vector.
     * @param[in] beta The Gauge::Math::Rational value to multiply by.
     *
     * @return A pointer to the new vector instance.
     */
    template <class T, class U>
    T *Mulitply(T *alpha, const Gauge::Math::Rational &beta) {
      if (alpha.leading >= alpha.trailing) return alpha;
      for (int index = alpha.leading; index < alpha.trailing; ++index) {
        alpha->base[index] *= beta.num;
      }
      alpha->den *= beta.den;

      return alpha;
    }
    /*!
     * Some vectors have values that are restricted to the range @f$ (-1,1] @f$.
     * This function cycles the vector's values into that range and returns a
     * pointer to the new vector.
     *
     * @param[in] alpha The vector to cycle.
     *
     * @return A pointer to the new vector.
     */
    template <class T>
    T *Cycle(const T &alpha) {
      T *vector = new T(alpha.size);
      vector->den = alpha.den;
      int den = vector->den;
      for (int index = alpha.leading; index < alpha.trailing; ++index) {
        int *val = vector->begin + index;
        *val = alpha.base[index];
        if (*val == -den) {
          *val *= -1;
        }
        while (*val < -den) *val += 2 * den;
        while (*val > den) *val -= 2 * den;
      }
      bool leading = false, trailing = false;
      for (int i = 0; i < vector->size && !leading && !trailing; ++i) {
        if (!leading && vector->base[i] != 0) {
          vector->leading = i;
          leading = true;
        }
        if (!trailing && vector->base[vector->size - i - 1] != 0) {
          vector->trailing = vector->size - i;
        }
      }
      return vector;
    }
    /*!
     * Some vectors have values that are restricted to the range @f$ (-1,1] @f$.
     * This function cycles the vector's values into that range in place and
     * returns a pointer to the vector.
     *
     * @param[in] alpha The vector to cycle.
     *
     * @return A pointer to the vector.
     */
    template <class T>
    T *Cycle(T *alpha) {
      int den = alpha->den;
      for (int index = alpha->leading; index < alpha->trailing; ++index) {
        int *val = alpha->begin + index;
        if (*val == -den) {
          *val *= -1;
        }
        while (*val < -den) *val += 2 * den;
        while (*val > den) *val -= 2 * den;
      }
      bool leading = false, trailing = false;
      for (int i = 0; i < alpha->size && !leading && !trailing; ++i) {
        if (!leading && alpha->base[i] != 0) {
          alpha->leading = i;
          leading = true;
        }
        if (!trailing && alpha->base[alpha->size - i - 1] != 0) {
          alpha->trailing = alpha->size - i;
          trailing = true;
        }
      }
      return alpha;
    }
    /*!
     * This method computes the dot product of two vector-like objects
     * (Gauge::Vector) and returns the result as a rational number
     * (Gauge::Math::Rational).
     *
     * There is no problem taking the dot-product between two different sized
     * vectors; we simply stop when we reach the end of the shortest vector.
     *
     * We then initialize an accumulation variable of type Gauge::Math::Rational
     * and then cycle through the two vectors performing a dot product as usual.
     *
     * @param[in] alpha The first vector.
     * @param[in] beta The second vector.
     *
     * @return The accumulated dot product.
     */
    template <class T, class U>
    Gauge::Math::Rational Product(const T &alpha, const U &beta) {
      int start = std::max(alpha.leading, beta.leading);
      int stop = std::min(alpha.trailing, beta.trailing);
      if (start >= stop) return Gauge::Math::Rational(0);
      const int *alpha_iter = alpha.begin + start;
      const int *beta_iter = beta.begin + start;
      int num = 0, den = alpha.den * beta.den;
      for (; alpha_iter != alpha.begin + stop; ++alpha_iter, ++beta_iter) {
        num += (*alpha_iter) * (*beta_iter);
      }
      return Gauge::Math::Rational(num,den);
    }
    /*!
     * This method computes the magnitude of two vector-like instances.
     *
     * @param[in] alpha The vector.
     *
     * @return The magniuted of the provided vector.
     */
    template <class T>
    Gauge::Math::Rational Magnitude(const T &alpha) {
      if (alpha.leading >= alpha.trailing) return 0;
      int num = 0, den = alpha.den * alpha.den;
      const int *start = alpha.begin + alpha.leading;
      const int *stop = alpha.begin + alpha.trailing;
      for (const int *iter = start; iter != stop; ++iter)
        num += (*iter) * (*iter);
      return Gauge::Math::Rational(num,den);
    }
  }
}

#endif
