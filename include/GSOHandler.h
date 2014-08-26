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
 * @file include/GSOHandler.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 04.05.2012
 *
 * @brief The Gauge::GSOHandler class is defined.
 */

#ifndef GAUGE_FRAMEWORK_GSOHANDLER_H
#define GAUGE_FRAMEWORK_GSOHANDLER_H

#include <Datatypes/Geometry.h>
#include <Datatypes/Input.h>
#include <Datatypes/Rational.h>
#include <Datatypes/State.h>

namespace Gauge {
  /*!
   * @brief
   * The Gauge::GSOHandler class deals with the initialization and generation of
   * modular invariant Gauge::GSOMatrix instances.
   */
  class GSOHandler {
    public:
      /*!
       * The default constructor inializes the Gauge::GSOHandler::products_ and
       * Gauge::GSOHandler::orders_ to @c NULL.
       */
      GSOHandler();
      /*!
       * Because we are dynamically allocating arrays, i.e.
       * Gauge::GSOHandler::products_ and Gauge::GSOHandler::orders_, we need to
       * deallocate them.
       */
      ~GSOHandler();
      /*!
       * Gauge::GSOHandler::GSOMatrix returns the current working
       * Gauge::GSOMatrix solution.
       *
       * @return The current Gauge::GSOMatrix.
       */
      const Gauge::GSOMatrix &GSOMatrix() const { return kij_; }
      /*!
       * Gauge::GSOHandler::NextGSOMatrix is called to generate the next
       * Gauge::GSOMatrix.
       *
       * @return A boolean flag specifying whether a new Gauge::GSOMatrix was
       * created @c true, or @c false otherwise.
       */
      bool NextGSOMatrix();
      /*!
       * This static method determines whether a state passes the GSO projection
       * or not.
       *
       * @param[in] geometry The Gauge::Geometry in which the Gauge::State was
       * build.
       * @param[in] state The Gauge::State being tested.
       * @param[in] coefficients The coefficients used to contruct the
       * Gauge::Sector that the Gauge::State was built from.
       *
       * @return @c true if the Gauge::State survives, and @c false otherwise.
       */
      static bool Project(const Gauge::Geometry &geometry,
                          const Gauge::State &state,
                          const int *coefficients);
      /*!
       * Gauge::GSOHandler::Setup does all of the non-trivial setup required to
       * actually generate Gauge::GSOMatrix instances.
       *
       * @param[in] basis The Gauge::Basis that will be used to create the
       * @param[in] susy_type The type of susy the model should have.
       * Gauge::GSOMatrix instances.
       */
      void Setup(const Gauge::Basis &basis, Gauge::Input::SUSYType susy_type);

    private:
      int extra_layers_;                  /*!< The number of extra layers that
                                            are atomatically added. */
      bool first_;                        /*!< A boolean flag specifying that
                                            this is the first run since a call
                                            to Gauge::GSOHandler::Setup. */
      Gauge::GSOMatrix kij_;              /*!< The current working
                                            Gauge::GSOMatrix. */
      int *orders_;                       /*!< The dynamically allocated array
                                            of orders. */
      Gauge::Math::Rational **products_;  /*!< The dynamically allocated matrix
                                            of dot products between basis
                                            vectors. */
      bool setup_;                        /*!< A boolean flag specifying that
                                            a call has been made to
                                            Gauge::GSOHandler::Setup. */
      Gauge::Input::SUSYType susy_type_;  /*!< A Gauge::Intput::SUSYType
                                            signifying what type of SUSY the
                                            models should have. */
      /*!
       * This method handles deallocating the memory allocated for the orders.
       */
      void ClearOrders();
      /*!
       * This method clears the memory allocated for the products array.
       */
      void ClearProducts();
      /*!
       * When setting the lower triangle of the Gauge::GSOMatrix, we must ensure
       * that the upper triangle can be set to a valid value. This method
       * computes the modular invariant value and returns @c true, or @c false
       * if one can't be found.
       *
       * @param[in,out] tnemele A reference to the upper-triangle element.
       * @param[in] row The index of the lower-triangle element.
       * @param[in] column The index of the lower-triangle element.
       * @param[in] element A constant reference to the lower-triangle element.
       *
       * @return A boolean flag signifying whether a valid upper-triangle was
       * found, @c true, or @c false otherwise.
       */
      bool ComputeOffDiagonal(Gauge::Math::Rational &tnemele,
                              int row, int column,
                              const Gauge::Math::Rational &element);
      /*!
       * When setting the lower triangle of the Gauge::GSOMatrix, we must ensure
       * that the diagonal value can be set to a valid value, if the lower
       * triangle element is in the first column. This method computes the
       * modular invariant value and returns @c true, or @c false if one can't
       * be found.
       *
       * @param[in,out] tnemele A reference to the upper-triangle element.
       * @param[in] row The index of the lower-triangle element.
       * @param[in] element A constant reference to the lower-triangle element.
       *
       * @return A boolean flag signifying whether a valid upper-triangle was
       * found, @c true, or @c false otherwise.
       */
      bool ComputeDiagonal(Gauge::Math::Rational &diagonal,
                           int row, const Gauge::Math::Rational &element);
      /*!
       * This method computes the products between each of the basis vectors in
       * the provided basis.
       *
       * @param[in] basis The basis that will be paired with the generated
       * GSO projection matrix.
       */
      void ComputeProducts(const Gauge::Basis &basis);
      /*!
       * Gauge::GSOHandler::FirstGSOMatrix minimizes each of the lower triangle
       * elements and returns @c true if it does so sucessfully.
       *
       * @return A boolean flag specifying whether a "minimal" GSO projection
       * matrix was found, @c true, or not.
       */
      bool FirstGSOMatrix();
      /*!
       * Gauge::IncrementElement increments the lower triangle element at the
       * provided location and sets the upper-triangle and diagonal via a call
       * to Gauge::GSOHandler::SetElement if necessary.
       *
       * @param[in] row The row of the lower-triangle element to be incremented.
       * @param[in] column The column of the lower-triangle element to be
       * incremented.
       *
       * @return A boolean flag specifying whether the incrementation
       * succeeded, @c true, or not.
       */
      bool IncrementElement(int row, int column);
      /*!
       * This method takes care of minimizing a single lower-triangle element at
       * the provided location and returns success.
       *
       * @param[in] row The row of the lower-triangle element to be incremented.
       * @param[in] column The column of the lower-triangle element to be
       * incremented.
       *
       * @return A boolean flag specifying whether the minimization
       * succeeded, @c true, or not.
       */
      bool MinimizeElement(int row, int column);
      /*!
       * This method makes calls to Gauge::GSOHandler::IncrementElement and
       * Gauge::GSOHandler::MinimizeElement so as to find the next
       * Gauge::GSOMatrix.
       *
       * @return A boolean flag signifiying whether a new Gauge::GSOMatrix was
       * found.
       */
      bool Next();
      /*!
       * This method does most of the heavy lifting. It set the lower-triangle,
       * upper-triangle and, if the element is in the first column, the diagonal
       * elements.
       *
       * @param[in] row The row of the lower-triangle element.
       * @param[in] column The column of the lower-triangle element.
       * @param[in] element The value of the lower-triangle element.
       *
       * @return A boolean flag signifying that the elements were set without
       * problem.
       */
      bool SetElement(int row, int column,
                      const Gauge::Math::Rational &element);
      /*!
       * From the dot-product of a state with a basis vector, this method
       * determines if the state survives the GSO projection from that basis
       * vector.
       *
       * @param[in] value The value of the dot product between the state and the
       * basis vector.
       * @param[in] gso The Gauge::GSOMatrix specifying the GSO projection.
       * @param[in] row The row of the GSO projection matrix corresponding to
       * the basis vector.
       * @param[in] coeff The coefficients used to construct the Gauge::State.
       * @param[in] extra The number of extra layers that were used to construct
       * the Gauge::GSOMatrix but are not in the Gauge::Basis.
       *
       * @return @c true if the state passes the projection and @c false
       * otherwise.
       */
      static bool PassesProjection(Gauge::Math::Rational *value,
                                   const Gauge::GSOMatrix &gso, int row,
                                   const int *coeff, int extra);
      /*!
       * This method simply determines if the GSO projection matrix generated
       * satisfies all of the required SUSY properties.
       *
       * @return A boolean signifying that the GSO projection matrix is valid.
       */
      bool Validate() const;
  };
}

#endif
