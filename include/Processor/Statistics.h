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
 * @file include/Processor/Statistics.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 10.02.2012
 *
 * @brief The Gauge::Processor::Statistics class is defined to provided the
 * framework with a uniform means of processing models for basic occurance
 * statistics.
 */

#pragma once

#include <map>

#include <Processor.h>

namespace Gauge {
  namespace Process {
    class Statistics : public Processor {
      public:
        std::string root;

        Statistics () : Statistics(".") {}
        Statistics (const std::string &root) : root(root) {}
        Statistics (const char *root) : Statistics(std::string(root)) {}

        bool operator==(const Statistics &other) const {
          return stats == other.stats;
        }
        bool operator!=(const Statistics &other) const {
          return !this->operator==(other);
        }

        virtual void Process(const Gauge::Model &model);
        virtual void Finalize();
        virtual void Merge(const Gauge::Processor &other);

        virtual Gauge::Processor* LocalProcessor() const {
          return new Gauge::Process::Statistics(this->root);
        }

        virtual void SerializeWith(Gauge::Serializer *serializer) const;
        virtual void DeserializeWith(Gauge::Serializer *serializer);

      private:
        typedef std::map<std::string, uint64_t> GroupMap;
        typedef std::map<std::string, GroupMap> OrderMap;
        typedef std::map<int, OrderMap> StatsMap;

        typedef std::pair<std::string, uint64_t> GroupEntry;
        typedef std::pair<std::string, GroupMap> OrderEntry;
        typedef std::pair<int, OrderMap> SUSYEntry;

        typedef std::map<std::string, uint64_t>::iterator group_iterator;
        typedef std::map<std::string, GroupMap>::iterator order_iterator;
        typedef std::map<int, OrderMap>::iterator susy_iterator;

        typedef std::map<std::string, uint64_t>::const_iterator cgroup_iterator;
        typedef std::map<std::string, GroupMap>::const_iterator corder_iterator;
        typedef std::map<int, OrderMap>::const_iterator csusy_iterator;

        StatsMap stats;

        Statistics(const Statistics &other) {}
        const Statistics &operator=(const Statistics &other) { return *this; }

        template <class T>
        std::string toString(T value) {
          std::ostringstream stream;
          stream << value;
          return stream.str();
        }

        std::string OrderString(const Gauge::Model &model) {
          Gauge::Basis &basis = model.geometry->basis;
          std::stringstream stream;
          if (basis.size > 0) {
            if (basis.base[0].order < 10) stream << "0";
            stream << basis.base[0].order;
          }
          for (int index = 1; index < basis.size; ++index) {
            stream << ((basis.base[index].order < 10) ? "x0" : "x");
            stream << basis.base[index].order;
          }
          return stream.str();
        }

        std::string GroupString(const Gauge::Model &model) {
          std::stringstream stream;
          model.group->PrintTo(&stream);
          return stream.str();
        }

    };
  }
}
