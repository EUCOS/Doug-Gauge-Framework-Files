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
 * @file src/Processor/Statistics.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 10.02.2012
 *
 * @brief The Gauge::Process::Statistics class is implemented below.
 */

#include <fstream>
#include <iomanip>

#include <Processor/Statistics.h>
#include <Utility/Directory.h>

void Gauge::Process::Statistics::Process(const Gauge::Model &model) {
  assert(!finalized);
  int susy = model.susy;
  std::string order = OrderString(model);
  std::string group = GroupString(model);

  if (stats.find(susy) != stats.end()) {
    if (stats[susy].find(order) != stats[susy].end())
      if (stats[susy][order].find(group) != stats[susy][order].end())
        stats[susy][order][group] += 1;
      else
        stats[susy][order][group] = 1;
    else
      stats[susy][order].insert(GroupEntry(group,1));
  } else {
    stats[susy].insert(OrderEntry(order, GroupMap()));
    stats[susy][order].insert(GroupEntry(group,1));
  }
}


void Gauge::Process::Statistics::Finalize() {
  if (*(--root.end()) == '/')
    root = root.substr(0,root.size()-1);

  susy_iterator susy_iter;
  order_iterator order_iter;
  group_iterator group_iter;

  for (susy_iter = stats.begin(); susy_iter != stats.end(); ++susy_iter) {
    std::string susydir = root + "/" + "N=" + toString(susy_iter->first);
    if (!Utility::Dir::Create(susydir)) exit(1);
    for (order_iter = susy_iter->second.begin();
        order_iter != susy_iter->second.end(); ++order_iter) {
      std::string filename = susydir + "/" + order_iter->first + ".txt";
      std::ofstream filestream(filename.c_str());
      for (group_iter = order_iter->second.begin();
          group_iter != order_iter->second.end(); ++group_iter) {
        filestream << std::setw(90) << std::left << group_iter->first;
        filestream << " " << group_iter->second << std::endl;
      }
      filestream.close();
    }
  }

  finalized = true;
}

void Gauge::Process::Statistics::Merge(const Gauge::Processor &other) {
  assert(!finalized);
  const Gauge::Process::Statistics* that =
    static_cast<const Gauge::Process::Statistics*>(&other);
  const StatsMap &s = that->stats;
  csusy_iterator susy_iter;
  corder_iterator order_iter;
  cgroup_iterator group_iter;

  for (susy_iter = s.cbegin(); susy_iter != s.cend(); ++susy_iter) {
    const int &susy = susy_iter->first;
    const OrderMap &orders = susy_iter->second;
    if (stats.find(susy) != stats.end()) {
      for (order_iter = orders.cbegin();
          order_iter != orders.cend();
          ++order_iter) {
        const std::string &order = order_iter->first;
        const GroupMap &groups = order_iter->second;
        if (stats[susy].find(order) != stats[susy].end()) {
          for (group_iter = groups.cbegin();
              group_iter != groups.cend();
              ++group_iter) {
            const std::string &group = group_iter->first;
            const uint64_t &count = group_iter->second;
            if (stats[susy][order].find(group) != stats[susy][order].end()) {
              stats[susy][order][group] += count;
            } else {
              stats[susy][order][group] = count;
            }
          }
        } else {
          stats[susy][order].insert(groups.begin(), groups.end());
        }
      }
    } else {
      stats[susy].insert(orders.begin(), orders.end());
    }
  }
}

void Gauge::Process::Statistics::SerializeWith(
    Gauge::Serializer *serializer) const {
  serializer->Write<int>(root.size());
  serializer->Write<char>(root.begin(), root.end());
  serializer->Write<int>(stats.size());
  for (csusy_iterator susy = stats.begin(); susy != stats.end(); ++susy) {
    serializer->Write<char>(susy->first);
    const OrderMap &orders = susy->second;
    serializer->Write<int>(orders.size());
    for (corder_iterator order = orders.begin(); order != orders.end(); ++order) {
      serializer->Write<int>(order->first.size());
      serializer->Write<char>(order->first.begin(), order->first.end());
      const GroupMap &groups = order->second;
      serializer->Write<int>(groups.size());
      for (cgroup_iterator group = groups.begin(); group != groups.end(); ++group) {
        serializer->Write<int>(group->first.size());
        serializer->Write<char>(group->first.begin(), group->first.end());
        serializer->Write<uint64_t>(group->second);
      }
    }
  }
}

void Gauge::Process::Statistics::DeserializeWith(
    Gauge::Serializer *serializer) {
  int size;
  serializer->Read<int>(&size);
  root = std::string(size,'a');
  serializer->Read<char>(root.begin(), root.end());

  stats.clear();

  serializer->Read<int>(&size);
  for (int index = size; index > 0; --index) {
    int susy;
    serializer->Read<char>(&susy);
    stats[susy] = OrderMap();
    serializer->Read<int>(&size);
    for (int index = size; index > 0; --index) {
      serializer->Read<int>(&size);
      std::string order(size,'a');
      serializer->Read<char>(order.begin(), order.end());
      stats[susy][order] = GroupMap();
      serializer->Read<int>(&size);
      for (int index = size; index > 0; --index) {
        serializer->Read<int>(&size);
        std::string group(size,'a');
        serializer->Read<char>(group.begin(),group.end());
        uint64_t count;
        serializer->Read<uint64_t>(&count);
        stats[susy][order][group] = count;
      }
    }
  }
}
