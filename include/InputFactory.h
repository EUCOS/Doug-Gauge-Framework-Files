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
 * @file include/InputFactory.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 05.14.2012
 * @brief We declare the Gauge::InputFactory as well as several subclasses.
 */

#ifndef GAUGE_FRAMEWORK_INPUTFACTORY_H
#define GAUGE_FRAMEWORK_INPUTFACTORY_H

#include <Datatypes/Input.h>

namespace Gauge {
  namespace InputFactory {
    /*!
     * @brief The Gauge::InputFactory::Generic class is an abstract base class
     * from which all InputFactory classes must derive.
     *
     * The Gauge::GeometryFactory utilizes instances of derived classes to
     * improve the looping mechanism that generates the Gauge::Geometries.
     */
    class Generic {
      public:
        /*!
         * The default construct is trivial.
         */
        Generic() {}
        /*!
         * The destructor is trivial.
         */
        virtual ~Generic() {}
        /*!
         * This method returns the current Gauge::Input instance.
         *
         * @return A constant reference to the current Gauge::Input instance.
         */
        virtual const Gauge::Input &Input() const { return input; }
        /*!
         * This pure method is the mechanism with which the next Gauge::Input is
         * generated.
         *
         * @return A boolean flag signifying that a new Gauge::Input was
         * generated.
         */
        virtual bool Next() = 0;

      protected:
        /*!
         * The copy constructor is trivial and private to discourage copying.
         *
         * @param[in] other The Gauge::InputFactory::Generic instance to copy.
         */
        Generic(const Generic &other) {}
        /*!
         * The assignment operator is trivial and private to discourage copying.
         *
         * @param[in] other The Gauge::InputFactory::Generic instance to copy.
         *
         * @return A reference to @c this.
         */
        Generic &operator=(const Generic &other) { return *this; }

        Gauge::Input input;                       /*!< The current input */
    };

    /*!
     * @brief The Gauge::InputFactory::Single is generates a single Gauge::Input
     * instance.
     *
     * This class is designed to provide a uniform API.
     */
    class Single : public Gauge::InputFactory::Generic {
      public:
        /*!
         * The constructor takes integer array of orders, the number of layers,
         * the number of large spacetime dimensions and the type of
         * supersymmetry and constructs an input factory that generates that
         * single Gauge::Input.
         *
         * @param[in] orders An integer array of orders.
         * @param[in] layers The number of layers.
         * @param[in] dimensions The number of large spacetime dimensions.
         * @param[in] susy_type The type of SUSY the models should have.
         */
        Single(const int *orders, int layers, int dimensions,
            Gauge::Input::SUSYType susy_type);
        /*!
         * We have a trivial destructor.
         */
        virtual ~Single() {}
        /*!
         * This method is the mechanism with which the next Gauge::Input is
         * generated.
         *
         * @return A boolean flag signifying that a new Gauge::Input was
         * generated.
         */
        virtual bool Next();

      private:
        /*!
         * We have a trivial and private default constructor. We want only one
         * way to create a Gauge::InputFactory::Single instance.
         */
        Single() : Gauge::InputFactory::Generic() {}
        /*!
         * The copy constructor is trivial and private to discourage copying.
         *
         * @param[in] other The Gauge::InputFactory::Single instance to copy.
         */
        Single(const Single &other) : Gauge::InputFactory::Generic(other) {}
        /*!
         * The assignment operator is trivial and private to discourage copying.
         *
         * @param[in] other The Gauge::InputFactory::Single instance to copy.
         *
         * @return A reference to @c this.
         */
        Single &operator=(const Single &other) { return *this; }

        bool first; /*!< A flag signifying that Next hasn't been called yet. */
    };

    /*!
     * @brief The Gauge::InputFactory::Range class allows the user to define a
     * range of inputs rather than a single input.
     *
     * This class is constructed from a pair of arrays of orders. Each possible
     * set of orders between the values provided is generated.
     */
    class Range : public Gauge::InputFactory::Generic {
      public:
        /*!
         * The constructor takes two integer arrays of orders (the lower and
         * upper bounds), the number of layers, the number of large spacetime
         * dimensions and the type of supersymmetry and constructs an input
         * factory that generates each Gauge::Input between those two bounds.
         *
         * @param[in] lower_bound The lower bound of orders.
         * @param[in] upper_bound The upper bound of orders.
         * @param[in] layers The number of layers.
         * @param[in] dimensions The number of large spacetime dimensions.
         * @param[in] susy_type The type of SUSY the models should have.
         */
        Range(const int *lower_bound, const int *upper_bound, int layers,
              int dimensions, Gauge::Input::SUSYType susy_type);
        /*!
         * We have non-trivial memory allocation so we must manually delete the
         * Gauge::InputFactory::Range::lower_bound and
         * Gauge::InputFactory::Range::upper_bound arrays.
         */
        virtual ~Range();
        /*!
         * This method is the mechanism with which the next Gauge::Input is
         * generated.
         *
         * @return A boolean flag signifying that a new Gauge::Input was
         * generated.
         */
        virtual bool Next();

      private:
        /*!
         * We have a trivial and private default constructor. We want only one
         * way to create a Gauge::InputFactory::Range instance.
         */
        Range() : Gauge::InputFactory::Generic() {}
        /*!
         * The copy constructor is trivial and private to discourage copying.
         *
         * @param[in] other The Gauge::InputFactory::Range instance to copy.
         */
        Range(const Range &other) : Gauge::InputFactory::Generic(other) {}
        /*!
         * The assignment operator is trivial and private to discourage copying.
         *
         * @param[in] other The Gauge::InputFactory::Range instance to copy.
         *
         * @return A reference to @c this.
         */
        Range &operator=(const Range &other) { return *this; }

        bool first;       /*!< A flag signifying that Next hasn't been called.*/
        int *lower_bound; /*!< An array of orders bounding below. */
        int *upper_bound; /*!< An array of orders bounding above. */
    };
  }
}

#endif
