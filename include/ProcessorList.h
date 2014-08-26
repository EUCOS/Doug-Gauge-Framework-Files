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
 * @file include/Processor.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 12.05.2012
 *
 * @brief The Gauge::ProcessorList provides an encapsulated vector of
 * Gauge::Processor instances.
 */

#pragma once

#include <cassert>
#include <vector>

#include <Processor.h>

namespace Gauge {
  class ProcessorList : public Serializable {
    public:
      ProcessorList() : finalized(false) {}
      ProcessorList(std::initializer_list<Processor*> p): processors(p) {
        finalized = false;
      }
      ~ProcessorList();

      void Process(const Gauge::Model &model);
      void Finalize();
      void Merge(const Gauge::ProcessorList &other);

      void Add(Gauge::Processor *processor);

      Gauge::ProcessorList *LocalList() const;

      virtual void SerializeWith(Gauge::Serializer *serializer) const;
      virtual void DeserializeWith(Gauge::Serializer *serializer);

    private:
      std::vector<Processor*> processors;
      bool finalized;
  };
}
