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
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * The Gauge Framework. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * @file src/MPI.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 05.23.2012
 * @brief The Gauge::MPI namespace is defined.
 */

#include <MPI.h>

void Gauge::MPI::Acquaint(int process, MPI_Comm comm) {
  MPI_Send(&process, 1, MPI_INT, process, 0, comm);
}

int Gauge::MPI::Acquaint(MPI_Comm comm) {
  MPI_Status status;
  int process;
  MPI_Recv(&process, 1, MPI_INT, MPI_ANY_SOURCE, 0, comm, &status);
  return status.MPI_SOURCE;
}

void Gauge::MPI::SendTag(int process, int tag, MPI_Comm comm) {
  MPI_Send(NULL, 0, MPI_INT, process, tag, comm);
}

int Gauge::MPI::ReceiveTag(int process, MPI_Comm comm) {
  MPI_Status status;
  MPI_Recv(NULL, 0, MPI_INT, process, MPI_ANY_TAG, comm, &status);
  return status.MPI_TAG;
}

void Gauge::MPI::SendRaw(Gauge::Raw &raw, int process, int tag) {
  int *size_ptr = const_cast<int *>(&raw.size);
  char *data_ptr = const_cast<char *>(raw.data);
  MPI_Send(size_ptr, 1, MPI_INT, process, tag, MPI_COMM_WORLD);
  MPI_Send(data_ptr, raw.size, MPI_CHAR, process, tag, MPI_COMM_WORLD);
}

Gauge::Raw *Gauge::MPI::ReceiveRaw(int process, int exit_tag) {
  Gauge::Raw *raw = new Gauge::Raw();

  MPI_Status status;
  MPI_Recv(&raw->size, 1, MPI_INT, process, MPI_ANY_TAG, MPI_COMM_WORLD,
      &status);
  if (status.MPI_TAG == exit_tag) {
    delete raw;
    return NULL;
  }
  raw->data = new char[raw->size];
  MPI_Recv(raw->data, raw->size, MPI_CHAR, process, MPI_ANY_TAG, MPI_COMM_WORLD,
      &status);
  if (status.MPI_TAG == exit_tag) delete raw;
  return raw;
}
