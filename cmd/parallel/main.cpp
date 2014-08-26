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
 * @file cmd/parallel/main.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 05.08.2012
 */

#include <Processor/ByGroup.h>
#include <Survey.h>
#include <Utility.h>

int main(int argc, char **argv) {
  const int D = 10, L = 1;
  const int lower[L] = {2}, upper[L] = {26};

  const std::string root_dir = "/data/moored/test/L=" + std::to_string(L) + "/";
  Utility::Dir::Create(root_dir);

  Gauge::Survey::Parallel(
      // Required for MPI
      argc, argv,
      // Processors
      { new Gauge::Process::ByGroup(root_dir + "D=" + std::to_string(D) + "/", false) },
      // Geometry Factory,
      Gauge::GeometryFactory::SystematicFactory(),
      // Input Factory
      new Gauge::InputFactory::Range(lower, upper, L, D, Gauge::Input::kSUSY),
      // Log File
      root_dir + "D=" + std::to_string(D) + ".log"
    );

  return 0;
}
