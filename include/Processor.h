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
 * @date 10.02.2012
 *
 * @brief The Gauge::Processor abstract class is defined to provided the
 * framework with a uniform means of processing models.
 */

#pragma once

#include <cassert>

#include <Datatypes/Model.h>
#include <Interfaces/Serializable.h>

namespace Gauge {
  class Processor : public Serializable {
    public:
      Processor() : finalized(false) {}
      Processor(const Processor &other) : finalized(other.finalized) {}
      virtual ~Processor() {}

      virtual void Process(const Gauge::Model &model) =  0;
      virtual void Finalize() = 0;
      virtual void Merge(const Gauge::Processor &other) = 0;

      virtual Gauge::Processor *LocalProcessor() const = 0;

    protected:
      bool finalized;
  };
}
