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
 * @file include/Processor/PrintToScreen.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 10.02.2012
 *
 * @brief The Gauge::Processor::PrintToScreen class is defined to provide the
 * framework with the ability to print models directly to standard output.
 * Note that this is not thread-safe so you should not use it in a parallelized
 * situation.
 */

#pragma once

#include <iostream>

#include <Processor.h>

namespace Gauge {
  namespace Process {
    class PrintToScreen : public Processor {
      public:
        PrintToScreen() {}

        virtual void Process(const Gauge::Model &model) {
          assert(!finalized);
          std::cout << model << std::endl;
        }
        virtual void Finalize() { finalized = true; }
        virtual void Merge(const Gauge::Processor &other) { assert(!finalized); }
        virtual Gauge::Processor* LocalProcessor() const {
          return new Gauge::Process::PrintToScreen();
        }

        virtual void SerializeWith(Gauge::Serializer *serializer) const {}
        virtual void DeserializeWith(Gauge::Serializer *serializer) {}
    };
  }
}
