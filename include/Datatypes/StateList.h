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
 * @file include/Datatypes/StateList.h
 * @author Douglas Moore <douglas_moore1@baylor.edu>
 * @date 10.26.2012
 * @brief The Gauge::StateList class is defined.
 *
 * The Gauge::StateList class provides a uniform means of representing
 * collections of states with multiple representations. For example, one might
 * want a full list of all states, or a list partitioned by sector, or possibly
 * by group.
 */

#pragma once

#include <list>
#include <vector>

#include <Datatypes/State.h>

namespace Gauge {
  class StateList {
    public:
      StateList() : full_list_out_dated(false), by_group_out_dated(false) {}
      StateList(const Gauge::StateList& other) = delete;
      ~StateList();

      StateList& operator=(const Gauge::StateList& other) = delete;

      void insert(Gauge::State *state, size_t sector);

      void clear();

      std::list<Gauge::State*>& FullList();
      std::vector<std::list<Gauge::State*>>& BySector();
      std::list<std::list<Gauge::State*>>& ByGroup();

    private:
      std::list<Gauge::State*> full_list;
      std::vector<std::list<Gauge::State*>> by_sector;
      std::list<std::list<Gauge::State*>> by_group;

      bool full_list_out_dated;
      bool by_group_out_dated;

      void BuildFullList();
      void BuildByGroup();
  };
}
