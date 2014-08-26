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

#include <cstring>
#include <inttypes.h>

#include <fstream>

#include <GeometryFactory.h>
#include <MPI.h>

using namespace Gauge;
using namespace Gauge::InputFactory;

template <class T>
std::string toString(T value) {
  std::ostringstream stream;
  stream << value;
  return stream.str();
}

std::string OutputFilename(int rank) {
  std::stringstream stream;
  if (rank < 10)
    stream << 0 << rank;
  else
    stream << rank;
  stream << ".txt";
  std::string filename = "/data/moored/MPI/" + stream.str();
  return filename;
}

std::ofstream *OpenOutput(int rank) {
  return new std::ofstream(OutputFilename(rank).c_str(), std::ios::trunc);
}

int main(int argc, char **argv) {
  const int good_tag = 73, exit_tag = 81, root = 0;

  const int D = 9, L = 2;
  const int lower[] = {2,10}, upper[] = {2,10};
  const Input::SUSYType susy_type = Input::kSUSY;

  const std::string root_dir = "/data/moored/MPI";
  const std::string output_dir = root_dir + "/D=" + toString(D) + "/";
  const std::string log_file = root_dir + "/D=" + toString(D) + ".log";

  MPI_Init(&argc, &argv);
  int rank, num_procs;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);

  assert(2 < num_procs);

  if (rank == root) {
    time_t rawtime;

    std::ofstream status(log_file.c_str());

    time(&rawtime);
    char *cur_time = asctime(localtime(&rawtime));
    cur_time[strlen(cur_time) - 1] = '\0';
    status << "Process started at " << cur_time << std::endl;
    status << cur_time << " -> " << num_procs << " builders started." << std::endl;

    GeometryFactory *factory = GeometryFactory::SystematicFactory();
    factory->Setup(new Range(lower, upper, L, D, susy_type));
    int process = 2;
    uint64_t count = 0;
    while (factory->NextGeometry()) {
      if (count % 10000 == 0) {
        time(&rawtime);
        cur_time = asctime(localtime(&rawtime));
        cur_time[strlen(cur_time) - 1] = '\0';
        status << cur_time << " -> " << (count + 1) << " geometries built" << std::endl;
      }
      ++count;
      std::ofstream *datafile = OpenOutput(process);
      factory->Geometry()->PrintTo(datafile);
      delete datafile;

      Gauge::MPI::Send(*factory->Geometry(), process, good_tag);
      process = (process + 1) % (num_procs - 2) + 2;
    }
    delete factory;

    for (process = 0; process < num_procs; ++process)
      if (process != root)
        Gauge::MPI::SendTag(process, exit_tag);

    std::cout << "Geometries Constructed: " << count << std::endl;

    time(&rawtime);
    cur_time = asctime(localtime(&rawtime));
    cur_time[strlen(cur_time) - 1] = '\0';

    status << cur_time << " -> Geometries Constructed: " << count << std::endl;
  } else {
    Gauge::Geometry geometry;
    std::string filename = OutputFilename(rank);
    while (Gauge::MPI::Receive(root, exit_tag, &geometry))
      remove(filename.c_str());
  }

  MPI_Finalize();

  return 0;
}
