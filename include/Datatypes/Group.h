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
 * @file include/Datatypes/Group.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 04.20.2012
 * @brief The Gauge::Group::Factor class is a datatype representing an A,D or E
 * class group factor.
 */

#ifndef GAUGE_FRAMEWORK_GROUP_H
#define GAUGE_FRAMEWORK_GROUP_H

#include <set>

#include <Interfaces/Printable.h>
#include <Interfaces/Serializable.h>

namespace Gauge {
  struct Group : public Gauge::Printable, public Gauge::Serializable {
    /*!
     * @brief A Gauge::Group is effectively just a character-integer pair, the
     * Cartan class and the rank.
     *
     * We DO NOT store any information about the root structure beyond what can
     * be obtained by knowledge of the group name.
     */
    struct Factor : public Gauge::Printable, public Gauge::Serializable {
      char cls; /*!< The character representation of the Cartan class. */
      int rank; /*!< The rank of the group as an integer. */
      /*!
       * Our default constructor initializes our class to @c 'N' and rank to @c
       * 0.
       */
      Factor() : cls('N'), rank(0) {}
      /*!
       * The primary constructor takes a character representation of the group
       * and an integer representation of the rank and constructs the
       * Gauge::Group instance.
       *
       * @param[in] cls The character representation of the Cartan class.
       * @param[in] rank The integer representation of the rank.
       */
      Factor(char cls, int rank) : cls(cls), rank(rank) {}
      /*!
       * The copy constructor simply copies the provided data within the
       * Gauge::Group instance provided to the internal value.
       *
       * @param[in] other The Gauge::Group to be copied.
       */
      Factor(const Gauge::Group::Factor &other);
      /*!
       * We do not dynamic allocation so we have a trivial destructor.
       */
      virtual ~Factor() {}
      /*!
       * The assignment operator simply copies the provided data within the
       * Gauge::Group instance provided to the internal value.
       *
       * @param[in] other The Gauge::Group::Factor to be copied.
       *
       * @return A reference to @c this.
       */
      Gauge::Group::Factor &operator=(const Gauge::Group::Factor &other);

      // Comparison Operators
      /*!
       * The equality operator determines if two Gauge::Group::Factor instances
       * are equivalent.
       *
       * @param[in] other The other Gauge::Group::Factor instance.
       *
       * @return @c true if @c this is equal to the other instance and @c false
       * otherwise.
       */
      bool operator==(const Gauge::Group::Factor &other) const {
        return (cls == other.cls && rank == other.rank);
      }
      /*!
       * The less-than operator determines if one Gauge::Group instance is less
       * than another.
       *
       * @param[in] other The other Gauge::Group instance.
       *
       * @return @c true if @c this is less than the other instance and @c false
       * otherwise.
       */
      bool operator< (const Gauge::Group::Factor &other) const {
        return (cls < other.cls || (cls == other.cls && rank < other.rank));
      }
      /*!
       * The inequality operator determines if two Gauge::Group::Factor
       * instances are not equivalent.
       *
       * @param[in] other The other Gauge::Group::Factor instance.
       *
       * @return @c true if @c this is not equal to the other instance and @c
       * false otherwise.
       */
      bool operator!=(const Gauge::Group::Factor &other) const {
        return !(*this == other);
      }
      /*!
       * The greater-than operator determines if one Gauge::Group::Factor
       * instance is greater than another.
       *
       * @param[in] other The other Gauge::Group::Factor instance.
       *
       * @return @c true if @c this is greater than the other instance and @c
       * false otherwise.
       */
      bool operator> (const Gauge::Group::Factor &other) const {
        return !(*this == other || *this < other);
      }
      /*!
       * The less-than-or-equal operator determines if one Gauge::Group::Factor
       * instance is less than or equal to another.
       *
       * @param[in] other The other Gauge::Group::Factor instance.
       *
       * @return @c true if @c this is less than or equal to the other instance
       * and @c false otherwise.
       */
      bool operator<=(const Gauge::Group::Factor &other) const {
        return !(*this > other);
      }
      /*!
       * The greater-than-or-equal operator determines if one
       * Gauge::Group::Factor instance is greater than or equal to another.
       *
       * @param[in] other The other Gauge::Group::Factor instance.
       *
       * @return @c true if @c this is greater than or equal to the other
       * instance and @c false otherwise.
       */
      bool operator>=(const Gauge::Group::Factor &other) const {
        return !(*this < other);
      }

      // Printable Interface
      /*!
       * By defining Gauge::Group::Factor::PrintTo and inheriting from
       * Gauge::Printable, we are given the Gauge::Printable::operator<< and
       * Gauge::Printable::PrintTo(const Printable&, std::ostream*).
       */
      virtual void PrintTo(std::ostream *out) const;

      // Serializeable Interface
      virtual void SerializeWith(Gauge::Serializer *serializer) const;
      virtual void DeserializeWith(Gauge::Serializer *serializer);

      /*!
       * The Gauge::Group::Factor::Compare structure provides a mechanism for
       * comparing two Gauge::Group::Factor pointers.
       */
      struct Compare {
        bool operator()(const Gauge::Group::Factor *alpha,
            const Gauge::Group::Factor *beta) {
          return *alpha < *beta;
        }
      };
    };

    /*!
     * The Gauge::Group::iterator is exactly that, an iterator for the
     * Gauge::Group object.
     */
    typedef std::multiset<Gauge::Group::Factor*>::iterator iterator;
    /*!
     * The Gauge::Group::const_iterator is exactly that, a constant iterator
     * for the Gauge::Group object.
     */
    typedef std::multiset<Gauge::Group::Factor*>::const_iterator
      const_iterator;
    /*! A multiset containing the Gauge::Group::Factors. */
    std::multiset<Gauge::Group::Factor*, Gauge::Group::Factor::Compare> factors;
    int rank;   /*!< The rank of the Gauge::Group. */
    /*!
     * The default constructor simply initializes the rank to @c 0.
     */
    Group() : rank(0) {}
    /*!
     * The copy constructor copies the content of the provided Gauge::Group to
     * a new Gauge::Group instance.
     *
     * @param[in] other The Gauge::Group to be copied.
     */
    Group(const Gauge::Group &other);
    /*!
     * The destructor deletes each of the Gauge::Group::Factor pointers stored
     * in the Gauge::Group::factors field.
     */
    ~Group();
    /*!
     * The assignment operator provides a mechanism for copying a Gauge::Group
     * instance to @c this and returns a reference to @c this.
     *
     * @param[in] other The Gauge::Group instance to copy.
     *
     * @return A reference to @c this.
     */
    Gauge::Group &operator=(const Gauge::Group &other);
    /*!
     * The equality operator compares two Gauge::Group instances for equality.
     *
     * @param[in] other The Gauge::Group instance to which to compare @c this.
     * @return A boolean signifying equality.
     */
    bool operator==(const Gauge::Group &other) const;
    /*!
     * The less-than operator compares two Gauge::Group instances for inequality.
     *
     * @param[in] other The Gauge::Group instance to which to compare @c this.
     * @return A boolean signifying that @c this is less than @c other.
     */
    bool operator< (const Gauge::Group &other) const;
    /*!
     * The non-equality operator compares two Gauge::Group instances for
     * non-equality.
     *
     * @param[in] other The Gauge::Group instance to which to compare @c this.
     * @return A boolean signifying that the instances are not equal.
     */
    bool operator!=(const Gauge::Group &other) const {
      return !(*this == other);
    }
    /*!
     * The less-than-or-equal operator compares two Gauge::Group instances to
     * determine if the left is less than or equal to the right.
     *
     * @param[in] other The Gauge::Group instance to which to compare @c this.
     * @return A boolean signifying that @c this is less than or equal to @c
     * other.
     */
    bool operator<=(const Gauge::Group &other) const {
      return (*this == other) || (*this < other);
    }
    /*!
     * The greater-than operator compares two Gauge::Group instances for
     * inequality.
     *
     * @param[in] other The Gauge::Group instance to which to compare @c this.
     * @return A boolean signifying that @c this is greater than @c other.
     */
    bool operator> (const Gauge::Group &other) const {
      return !(*this <= other);
    }
    /*!
     * The greater-than-or-equal operator compares two Gauge::Group instances
     * for inequality.
     *
     * @param[in] other The Gauge::Group instance to which to compare @c this.
     * @return A boolean signifying that @c this is greater than @c other.
     */
    bool operator>=(const Gauge::Group &other) const {
      return !(*this < other);
    }
    /*!
     * This method returns a Gauge::Group::iterator to the beginning of the
     * Gauge::Group::factors multiset.
     */
    Gauge::Group::iterator begin() { return factors.begin(); }
    /*!
     * This method returns a Gauge::Group::iterator to the end of the
     * Gauge::Group::factors multiset.
     */
    Gauge::Group::iterator end() { return factors.end(); }
    /*!
     * This method returns a Gauge::Group::const_iterator to the beginning of
     * the Gauge::Group::factors multiset.
     */
    Gauge::Group::const_iterator begin() const { return factors.begin(); }
    /*!
     * This method returns a Gauge::Group::const_iterator to the end of the
     * Gauge::Group::factors multiset.
     */
    Gauge::Group::const_iterator end() const { return factors.end(); }

    /* Printable Interface */
    virtual void PrintTo(std::ostream *out) const;

    // Serializeable Interface
    virtual void SerializeWith(Gauge::Serializer *serializer) const;
    virtual void DeserializeWith(Gauge::Serializer *serializer);
  };
}

#endif
