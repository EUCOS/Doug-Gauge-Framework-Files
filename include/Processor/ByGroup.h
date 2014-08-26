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
 * @file include/Processor/ByGroup.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 12.08.2012
 *
 * @brief The Gauge::Processor::ByGroup class is defined to provide the
 * framework with a uniform means of printing model geometries by gauge
 * group. This allows us to consider the gauge group degeneracies.
 */

#pragma once

#include <cstdlib>
#include <ctime>
#include <map>

#include <Processor.h>

namespace Gauge {
  namespace Process {
    class ByGroup : public Processor {
      public:
        bool print_gso;
        std::string root;
        std::string local;
        std::map<std::string, std::ofstream*> files;

        ByGroup() : ByGroup(".", true) {}
        ByGroup(bool print_gso) : ByGroup(".", print_gso) {}
        ByGroup(const std::string &root, bool print_gso);
        ByGroup(const char *root, bool print_gso) :
            ByGroup(std::string(root), print_gso) {}
        virtual ~ByGroup();

        virtual void Process(const Gauge::Model &model);
        virtual void Finalize();
        virtual void Merge(const Gauge::Processor &other);

        virtual Gauge::Processor* LocalProcessor() const {
          return new Gauge::Process::ByGroup(this->root, this->print_gso);
        }

        virtual void SerializeWith(Gauge::Serializer *serializer) const;
        virtual void DeserializeWith(Gauge::Serializer *serializer);

      private:
        ByGroup(const ByGroup &other) {}
        const ByGroup &operator=(const ByGroup &other) { return *this; }

        std::string GroupString(const Gauge::Model &model) {
          std::stringstream stream;
          model.group->PrintTo(&stream);
          return stream.str();
        }
    };
  }
}
