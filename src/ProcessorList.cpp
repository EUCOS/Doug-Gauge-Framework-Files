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
 * @file src/ProcessorList.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 12.05.2012
 *
 * @brief The Gauge::ProcessorList provides an encapsulated vector of
 * Gauge::Processor instances.
 */

#include <ProcessorList.h>

Gauge::ProcessorList::~ProcessorList() {
  for (Gauge::Processor *processor: processors)
    delete processor;
}

void Gauge::ProcessorList::Process(const Gauge::Model &model) {
  assert(!finalized);
  for (Gauge::Processor *processor: processors)
    processor->Process(model);
}

void Gauge::ProcessorList::Finalize() {
  for (Gauge::Processor *processor: processors)
    processor->Finalize();
  finalized = true;
}

void Gauge::ProcessorList::Merge(const Gauge::ProcessorList &other) {
  assert(!finalized);
  auto x = begin(processors);
  auto y = begin(other.processors);
  while (x != end(processors) && y != end(other.processors)) {
    (*x)->Merge(**y);
    ++x;
    ++y;
  }
}

void Gauge::ProcessorList::Add(Gauge::Processor *processor) {
  processors.push_back(processor);
}

Gauge::ProcessorList *Gauge::ProcessorList::LocalList() const {
  Gauge::ProcessorList *local = new ProcessorList({});
  for(auto *processor: processors)
    local->processors.emplace_back(processor->LocalProcessor());
  return local;
}

void Gauge::ProcessorList::SerializeWith(Gauge::Serializer *serializer) const {
  serializer->Write<size_t>(processors.size());
  for (auto *processor: processors)
    processor->SerializeWith(serializer);
  serializer->Write<bool>(finalized);
}

void Gauge::ProcessorList::DeserializeWith(Gauge::Serializer *serializer) {
  size_t size;
  serializer->Read<size_t>(&size);
  assert(size == processors.size());
  for (auto *processor: processors)
    processor->DeserializeWith(serializer);
  serializer->Read<bool>(&finalized);
}
