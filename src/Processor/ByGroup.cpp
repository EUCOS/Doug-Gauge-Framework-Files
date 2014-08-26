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
 * @file src/Processor/ByGroup.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 12.08.2012
 *
 * @brief The Gauge::Process::ByGroup class is implemented below.
 */

#include <cstdio>
#include <fstream>

#include <Processor/ByGroup.h>
#include <Utility/Directory.h>

Gauge::Process::ByGroup::ByGroup(const std::string &root, bool print_gso) {
  this->print_gso = print_gso;
  this->root = root;
  this->local = "";
  if (root.back() != '/') this->root += "/";
  Utility::Dir::Create(this->root);
}

Gauge::Process::ByGroup::~ByGroup() {
  for (auto &entry: files) {
    if (entry.second != NULL) delete entry.second;
    remove((local + entry.first + ".txt").c_str());
  }
  remove(local.c_str());
}

void Gauge::Process::ByGroup::Process(const Gauge::Model &model) {
  assert(!finalized);
  if (files.empty() && local == "") local = Utility::Dir::Temporary(root);
  std::string group = GroupString(model);
  if (files.find(group) == end(files) || files[group] == NULL)
    files[group] = new std::ofstream(local + group + ".txt");

  if (print_gso)
    *files[group] << *model.geometry << std::endl;
  else
    *files[group] << model.geometry->basis << std::endl;
}

void Gauge::Process::ByGroup::Finalize() {
  assert(!finalized);

  for (auto entry: files) {
    if (entry.second != NULL) delete entry.second;
    rename((local + entry.first + ".txt").c_str(),
           (root + entry.first + ".txt").c_str());
  }
  files.clear();
  finalized = true;
}

void Gauge::Process::ByGroup::Merge(const Gauge::Processor &other) {
  assert(!finalized);
  if (files.empty() && local == "") local = Utility::Dir::Temporary(root);

  const Gauge::Process::ByGroup *that =
    static_cast<const Gauge::Process::ByGroup*>(&other);

  for (auto entry: that->files) {
    if (files.find(entry.first) == end(files) || files[entry.first] == NULL)
      files[entry.first] =
        new std::ofstream(local + entry.first + ".txt", std::ios_base::app);
    std::ifstream old_file(that->local + entry.first + ".txt");
    if (old_file.is_open()) {
      std::string line;
      while (old_file.good()) {
        getline(old_file, line);
        *files[entry.first] << line << std::endl;
      }
      old_file.close();
    }
  }
}

void Gauge::Process::ByGroup::SerializeWith(
    Gauge::Serializer *serializer) const {
  serializer->Write<size_t>(root.size());
  serializer->Write<char>(begin(root), end(root));
  serializer->Write<size_t>(local.size());
  serializer->Write<char>(begin(local), end(local));
  serializer->Write<size_t>(files.size());
  for (auto &entry: files) {
    serializer->Write<size_t>(entry.first.size());
    serializer->Write<char>(begin(entry.first), end(entry.first));
  }
}

void Gauge::Process::ByGroup::DeserializeWith(
    Gauge::Serializer *serializer) {
  size_t size, group_size;
  std::string group;

  serializer->Read<size_t>(&size);
  root = std::string(size, 'a');
  serializer->Read<char>(begin(root), end(root));

  serializer->Read<size_t>(&size);
  local = std::string(size, 'a');
  serializer->Read<char>(begin(local), end(local));

  serializer->Read<size_t>(&size);
  while (size > 0) {
    serializer->Read<size_t>(&group_size);
    group = std::string(group_size, 'a');
    serializer->Read<char>(begin(group), end(group));
    if (files.find(group) == end(files))
      delete files[group];
    files[group] = NULL;
    --size;
  }
}
