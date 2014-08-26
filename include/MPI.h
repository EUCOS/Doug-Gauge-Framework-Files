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
 * @file include/MPI.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 05.23.2012
 * @brief The Gauge::MPI namespace is defined.
 */

#ifndef GAUGE_FRAMEWORK_MPI_H
#define GAUGE_FRAMEWORK_MPI_H

#include <cstdlib>
#include "mpi.h"

#include <Datatypes/Raw.h>
#include <Interfaces/Serializable.h>

namespace Gauge {
  namespace MPI {
    void Acquaint(int process, MPI_Comm comm = MPI_COMM_WORLD);
    int Acquaint(MPI_Comm comm = MPI_COMM_WORLD);

    void SendTag(int processor, int exit_tag, MPI_Comm comm = MPI_COMM_WORLD);
    int ReceiveTag(int processor, MPI_Comm comm = MPI_COMM_WORLD);

    void SendRaw(Gauge::Raw &raw, int process, int tag);
    Gauge::Raw *ReceiveRaw(int process, int tag);

    template <class T>
    void Send(const T &message, int process, int tag) {
      Gauge::Raw *data = message.Serialize();
      SendRaw(*data, process, tag);
      delete data;
    }

    template <class T>
    bool Receive(int process, int tag, T *message) {
      Gauge::Raw *data = ReceiveRaw(process, tag);
      if (data == NULL) return false;
      message->Deserialize(data);
      if (data->data != NULL) {
        delete data;
        return false;
      }
      return true;
    }
  }
}

#endif
