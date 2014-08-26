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
 * @file include/GeometryFactory.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 04.05.2012
 * @brief We declare the Gauge::GeometryFactory.
 *
 * The Gauge::GeometryFactory is used to generate the Gauge::Geometries that are
 * fed in as inputs to the model building process.
 */

#ifndef GAUGE_FRAMEWORK_GEOMETRYFACTORY_H
#define GAUGE_FRAMEWORK_GEOMETRYFACTORY_H

#include <Datatypes/Geometry.h>
#include <Datatypes/Input.h>
#include <BasisHandler.h>
#include <InputFactory.h>
#include <GSOHandler.h>

namespace Gauge {
  /*!
   * @brief
   * To generate Gauge::Geometry instances in an algorithm-agnostic manner, we
   * define the Gauge::GeometryFactory.
   *
   * We need to be able to generate Gauge::Geometry instances in a way that is
   * independent of the actual algorithm used. That is to say, the interface for
   * the construction of basis sets and GSO projection matricies shouldn't
   * change, even if the algorithm does. To that end, we define the
   * Gauge::GeometryFactory class to facility this generic generation algorithm.
   *
   * This class favors the use of static factory methods over constructors. This
   * is due to their improved expressivness. It does, however, incure the
   * responsibility of deallocating the memory, a burden we bare nobly.
   */
  class GeometryFactory {
    public:
      /*!
       * We will be dynamically allocating memory to contain our
       * Gauge::BasisHandler, Gauge::GSOHandler and Gauge::Geometry instances.
       * Thus, we need to deallocate them.
       */
      ~GeometryFactory();
      // Accessors
      /*!
       * This constant accessor provideds a constant pointer to the
       * Gauge::Geometry stored within @c this.
       *
       * @return A constant pointer to the Gauge::Geometry most recently
       * created.
       */
      const Gauge::Geometry *Geometry() const { return geometry_; }
      // Interface
      /*!
       * This static method provided a clean way of constructing generic
       * geometry factories in an expressive way. It takes a pointer to
       * a Gauge::BasisHandler and a pointer to a Gauge::GSOHandler,
       * constructs a Gauge::GeometryFactory instance, and returns a pointer to
       * the newly constructed Gauge::GeometryFactory.
       *
       * NOTE: It is the end user's job to deallocate the memory allocated for
       * the return Gauge::GeometryFactory.
       *
       * @param[in,out] basis_handler The Gauge::BasisHandler the constructed
       * Gauge::GeometryFactory will use.
       * @param[in,out] gso_handler The Gauge::GSOHandler that constructed
       * Gauge::GeometryFactory will use.
       *
       * @return A pointer to the newly allocated Gauge::GeometryFactory.
       */
      static GeometryFactory *GenericFactory(Gauge::BasisHandler *basis_handler,
                                             Gauge::GSOHandler *gso_handler);
      /*!
       * Once a call to Gauge::GeometryFactory::Setup has been made, subsequent
       * calls to this method will construct the next Gauge::Geometry and return
       * a boolean signifying success.
       *
       * If this method is called before any calls to
       * Gauge::GeometryFactory::Setup, the program will terminate via
       * C-assertion.
       *
       * @return A boolean flag signifying that a new Gauge::Geometry has been
       * constructor, @c true, or not.
       */
      bool NextGeometry();
      /*!
       * This setup method allows the user to specify what the Gauge::Input
       * should be.
       *
       * @param[in] input The orders of the basis vectors to be generated.
       * generate.
       */
      void Setup(Gauge::InputFactory::Generic *input_factory);
      /*!
       * This static method provided a clean way of constructing generic
       * geometry factories in an expressive way. It constructs
       * a Gauge::GeometryFactory using a Gauge::BasisHandler and
       * Gauge::GSOHandler by default. Currently, this method just provides
       * a simplified mechanism for creating Gauge::GeometryFactories. In the
       * future, when there is more than just one type of Gauge::BasisHandler
       * or Gauge::GSOHandler, this method will be much more applicable.
       *
       * NOTE: It is the end user's job to deallocate the memory allocated for
       * the return Gauge::GeometryFactory.
       *
       * @return A pointer to the newly allocated Gauge::GeometryFactory.
       */
      static GeometryFactory *SystematicFactory();

    private:
      /*!
       * The default constructor does nothing and is private, so the only way to
       * us it is via a static factory method.
       */
      GeometryFactory() {}
      /*!
       * To discourage copying we have a trivial, private copy constructor.
       */
      GeometryFactory(const Gauge::GeometryFactory &other) {}
      /*!
       * Again, we are discouraging copying, thus a private, trivial assignment
       * operator.
       */
      GeometryFactory &operator=(const Gauge::GeometryFactory &other) {
        return *this;
      }
      /*!
       * This constructor is used internally to construct new
       * Gauge::GeometryFactory instances. It takes a pointer to
       * a Gauge::BasisHandler and a pointer to a Gauge::GSOHandler, constructs
       * a Gauge::GeometryFactory instance.
       *
       * @param[in,out] basis_handler The Gauge::BasisHandler the constructed
       * Gauge::GeometryFactory will use.
       * @param[in,out] gso_handler The Gauge::GSOHandler that constructed
       * Gauge::GeometryFactory will use.
       */
      GeometryFactory(Gauge::BasisHandler *basis_handler,
                      Gauge::GSOHandler *gso_handler);

      Gauge::BasisHandler *basis_handler_;  /*!< The Gauge::BasisHandler to be
                                              used for the construction of
                                              Gauge::Geometry instances. */
      Gauge::GSOHandler *gso_handler_;      /*!< The Gauge::GSOHandler to be
                                              used in the construction of
                                              Gauge::Geometry instances. */
      Gauge::Geometry *geometry_;           /*!< The Gauge::Geometry that was
                                              most recently generated. */

      Gauge::InputFactory::Generic *input_factory_;
                                            /*!< The Gauge::InputFactory that
                                              should be used to construct the
                                              Gauge::Input instances. */

      bool first_;                          /*!< This is a boolean flag used to
                                              signify that there have been no
                                              calls to
                                              GeometryFactory::NextGeometry. */
      bool setup_;                          /*!< This flag signifies that the
                                              Gauge::GeometryFactory::Setup
                                              method has been called. */
      const Gauge::Input *input_;           /*!< The Gauge::Input specifying
                                              what models should be generated.*/
  };
}

#endif
