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
 * @file include/Interfaces/Serializable.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 09.04.2012
 * @brief The Gauge::Serializable interface is defined to signify that an object
 * can be serialized.
 */

#pragma once

#include <Serializer.h>

namespace Gauge {
  /*!
   * Any class that inherits from the Gauge::Serializable class is will be,
   * well, serializable. This facilitates the both the saving of state, printing
   * of checkpoint files, message passing via MPI, etc.
   */
  class Serializable {
    public:
      /*!
       * The Gauge::Serializable interface has not internal state so there is
       * nothing to destruct.
       */
      virtual ~Serializable() {}
      /*!
       * The Gauge::Serializable::Serialize method provides a mechanism for
       * serializing an instance. The process should be constant and return a
       * pointer to a Gauge::Raw instance.
       *
       * @return A pointer to the Gauge::Raw instance containing the serilized
       * data.
       */
      Gauge::Raw *Serialize() const {
        Gauge::Serializer serializer;
        SerializeWith(&serializer);
        return serializer.Flush();
      }
      /*!
       * The Gauge::Serializable::Deserialize abstract provides a mechanism for
       * deserializing an instance. The process should be constant and return a
       * pointer to a Gauge::Raw instance.
       *
       * @param[in,out] raw The Gauge::Raw from which to deserialize.
       * @return A Gauge::Raw pointer to the remaining deserialized data.
       */
      Gauge::Raw *Deserialize(Gauge::Raw *raw) {
        Gauge::Serializer serializer(raw);
        DeserializeWith(&serializer);
        return serializer.Flush();
      }
      /*!
       * Often we will need to serialize complex objects. This method provides a
       * mechanism with which to "chain-serialize" several objects with the same
       * Gauge::Serializer.
       *
       * @param[in,out] serializer The Gauge::Serializer to use for serialization.
       */
      virtual void SerializeWith(Gauge::Serializer *serializer) const = 0;
      /*!
       * Often we will need to deserialize complex objects. This method provides
       * a mechanism with which to "chain-deserialize" several objects with the
       * same Gauge::Serializer.
       *
       * @param[in,out] serializer The Gauge::Serializer to use for serialization.
       */
      virtual void DeserializeWith(Gauge::Serializer *serializer) = 0;
  };
}
