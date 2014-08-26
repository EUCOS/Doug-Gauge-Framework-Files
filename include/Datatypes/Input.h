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
 * @file include/Inputs.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 04.11.2012
 *
 * @brief The Gauge::Inputs class is a mechanism for providing input information
 * to the Gauge::GeometryFactory class.
 */

#ifndef GAUGE_FRAMEWORK_INPUT_H
#define GAUGE_FRAMEWORK_INPUT_H

#include <cassert>
#include <cstdlib>

#include <Interfaces/Printable.h>
#include <Interfaces/Serializable.h>

namespace Gauge {
  /*!
   * @brief
   * The Gauge::Inputs class is a way of generating the inputs for
   * Gauge::GeometryFactory instances.
   *
   * The idea being that the programmer can provide a simple means of
   * determining what types of models need to be generated and allows the
   * Gauge::Input class to do the heavy-lifting. More details to come.
   */
  struct Input : public Gauge::Printable, public Gauge::Serializable {
    /*!
     * The Gauge::Input::SUSYType is an enumeration type that is used to
     * specify what supersymmetry properties generated models should have.
     */
    enum SUSYType {
      kSUSY,          /*!< Gauge::Input::kSUSY signifies that models should
                        have the SUSY sector, but can be constructed with
                        either the reduced @f$ N=0 @f$ or the full @f$ N=4 @f$
                        SUSY. */
      kFullSUSY,      /*!< Gauge::Input::kFullSUSY signifies that models
                        should have the full @f$ N = 4 @f$ SUSY. */
      kNonSUSY,       /*!< Gauge::Input::kNonSUSY signifies that the model
                        should be built from a Gauge::Basis that does not
                        include the SUSY sector and, thus, has @f$ N=0 @f$
                        SUSY. */
      kReducedSUSY    /*!< Gauge::Input::kReducedSUSY signifies that the model
                        generated should be built from a Gauge::Basis
                        including the SUSY sector, but have @f$ N = 0 @f$ SUSY
                        as a result of the GSO projection.*/
    };

    int dimensions;     /*!< The number of large spacetime dimensions. */
    int layers;         /*!< The number of layers the model should have. */
    int *orders;        /*!< An array of orders with which to build the model.*/
    SUSYType susy_type; /*!< The type of SUSY the model should have. */

    /*!
     * Our default constructor initializes sets the number of large spacetime
     * dimensions to @c 4, the number of layers to @c 0, the orders pointer to
     * @c NULL and the SUSY type to all supersymmetries.
     */
    Input() : dimensions(4), layers(0), orders(NULL), susy_type(kSUSY) {}

    /*!
     * Our primary constructor initializes the orders array to that provided as
     * well as the number of layers, large spacetime dimensions and the SUSY
     * type.
     *
     * @param[in] orders The array of orders.
     * @param[in] layers The number of layers (a.k.a. the number of orders).
     * @param[in] dimensions The number of large spacetime dimensions.
     * @param[in] susy_type The Gauge::Input::SUSYType of the models.
     */
    Input(const int *orders, int layers, int dimensions, SUSYType susy_type) :
        dimensions(dimensions), layers(layers), susy_type(susy_type) {
      assert(dimensions % 2 == 0 && 4 <= dimensions && dimensions <= 10);
      assert(layers >= 1);
      this->orders = new int[this->layers];
      for (int index = 0; index < this->layers; ++index) {
        assert(2 <= orders[index]);
        this->orders[index] = orders[index];
      }
    }

    /*!
     * The Copy constructor copies the contents of the provided Gauge::Input
     * instance to a new instance.
     *
     * @param[in] other The Gauge::Input to copy.
     */
    Input(const Gauge::Input &other) {
      dimensions = other.dimensions;
      layers = other.layers;
      orders = new int[layers];
      for (int index = 0; index < layers; ++index)
        orders[index] = other.orders[index];
      susy_type = other.susy_type;
    }

    /*!
     * Our destructor deletes the dynamically allocated memory used to store the
     * array of orders.
     */
    virtual ~Input() { if (orders != NULL) delete [] orders; }

    /*!
     * The assignment operator assigns the content of the provided Gauge::Input
     * instance to @c this.
     *
     * @param[in] other The Gauge::Input instance to copy.
     *
     * @return A reference to @c this.
     */
    Gauge::Input &operator=(const Gauge::Input &other) {
      if (this != &other) {
        if (orders != NULL && layers != other.layers) delete [] orders;
        if (orders == NULL) orders = new int[other.layers];

        dimensions = other.dimensions;
        layers = other.layers;
        for (int index = 0; index < layers; ++index)
          orders[index] = other.orders[index];
        susy_type = other.susy_type;
      }
      return *this;
    }
    /*!
     * The equality operator determines the equality of the Gauge::Input
     * instances.
     *
     * @param[in] other The Gauge::Input to which to compare @this.
     * @return A boolean signifying equality.
     */
    bool operator==(const Gauge::Input &other) const {
      if (dimensions != other.dimensions) return false;
      if (layers != other.layers) return false;
      if (susy_type != other.susy_type) return false;
      for (int index = 0; index < layers; ++index)
        if (orders[index] != other.orders[index]) return false;
      return true;
    }
    /*!
     * The non-equality operator determines whether two Gauge::Input
     * instances are not equal.
     *
     * @param[in] other The Gauge::Input to which to compare @this.
     * @return A boolean signifying that the instances are not equal.
     */
    bool operator!=(const Gauge::Input &other) const {
      return !(*this == other);
    }

    virtual void PrintTo(std::ostream *out) const {
      *out << "[ ";
      for (int index = 0; index < layers; ++index)
        *out << orders[index] << " ";

      *out << "] " << "D=" << dimensions << " ";
      switch (susy_type) {
        case kSUSY: *out << "Full and Reduced SUSY"; break;
        case kNonSUSY: *out << "NonSUSY"; break;
        case kReducedSUSY: *out << "Reduced SUSY"; break;
        case kFullSUSY: *out << "Full SUSY"; break;
        default: assert(false);
      };
    }

    // Serializable Interface
    virtual void SerializeWith(Gauge::Serializer *serializer) const {
      // Since the dimension does not exceed 10, we can compress to a char.
      serializer->Write<char>(dimensions);
      // Since we are only considering layers up to about 20, we can compress to
      // a char.
      serializer->Write<char>(layers);
      // Our orders are limited by time to approximately 50 so we can compress
      // to char.
      serializer->Write<char>(orders, orders + layers);
      // There are only 4 susy types, again we can compress to char.
      serializer->Write<char>(susy_type);
    }

    virtual void DeserializeWith(Gauge::Serializer *serializer) {
      serializer->Read<char>(&dimensions);
      serializer->Read<char>(&layers);

      if (orders != NULL) delete [] orders;
      orders = new int[layers];

      serializer->Read<char>(orders, orders + layers);
      serializer->Read<char>(&susy_type);
    }
  };
}

#endif
