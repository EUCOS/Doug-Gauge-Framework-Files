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
 * @file src/Datatypes/StateList.h
 * @author Douglas Moore <douglas_moore1@baylor.edu>
 * @date 10.27.2012
 * @brief The Gauge::StateList class is defined.
 *
 * The Gauge::StateList class provides a uniform means of representing
 * collections of states with multiple representations. For example, one might
 * want a full list of all states, or a list partitioned by sector, or possibly
 * by group.
 */

#include <Datatypes/StateList.h>

Gauge::StateList::~StateList() {
  this->clear();
}

void Gauge::StateList::insert(Gauge::State *state, size_t sector) {
  by_sector[sector].emplace_back(state);
  full_list_out_dated = true;
  by_group_out_dated  = true;
}

void Gauge::StateList::clear() {
  Gauge::State* state = nullptr;
  for (size_t index = 0; index < by_sector.size(); ++index) {
    for (auto iter = begin(by_sector[index]);
         iter != end(by_sector[index]); ++iter)
      if (*iter != nullptr) {
        state = *iter;
        delete state;
      }
    by_sector[index].clear();
  }
  by_sector.clear();

  full_list.clear();
  by_group.clear();

  full_list_out_dated = false;
  by_group_out_dated  = false;
}

std::list<Gauge::State*>& Gauge::StateList::FullList() {
  if (full_list_out_dated) {
    BuildFullList();
    full_list_out_dated = false;
  }
  return full_list;
}

std::vector<std::list<Gauge::State*>>& Gauge::StateList::BySector() {
  return by_sector;
}

std::list<std::list<Gauge::State*>>& Gauge::StateList::ByGroup() {
  if (by_group_out_dated) {
    BuildByGroup();
    by_group_out_dated = false;
  }
  return by_group;
}

void Gauge::StateList::BuildFullList() {
  full_list.clear();
  for (auto sector: by_sector)
    full_list.insert(end(full_list), begin(sector), end(sector));
}

void Gauge::StateList::BuildByGroup() {
  by_group.clear();
  if (full_list_out_dated) BuildFullList();
  std::list<Gauge::State*> all_states(begin(full_list), end(full_list));

  typename std::list<std::list<Gauge::State*>>::iterator group;
  while (all_states.size() != 0) {
    by_group.push_back(std::list<Gauge::State *>(1, all_states.front()));
    all_states.pop_front();
    group = (--by_group.end());
    for (std::list<Gauge::State*>::iterator p = group->begin();
        p != group->end(); ++p) {
      for (std::list<Gauge::State*>::iterator q = all_states.begin();
          q != all_states.end();) {
        if (Gauge::Math::Product(**p, **q) == 0) {
          ++q;
        } else {
          group->push_back(*q);
          q = all_states.erase(q);
        }
      }
    }
  }
}
