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
 * @file src/Serializer.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 09.04.2012
 *
 * @brief The Gauge::Serializer class is defined.
 */

#include <Serializer.h>

/*!
 * The default constructor does nothing but initialize the stream_width to @c 0
 * and allocates a new Gauge::Raw instance.
 */
Gauge::Serializer::Serializer() {
  stream_width = 0;
  raw_data = new Gauge::Raw();
}
/*!
 * The Gauge::Raw constructor will read the data into the stream if the provided
 * pointer isn't @c NULL. If it is, then we initialize the pointer to a new,
 * empty Gauge::Raw instance.
 */
Gauge::Serializer::Serializer(Gauge::Raw *data) {
  if (data == NULL) {
    data = new Gauge::Raw();
    raw_data = data;
    stream_width = 0;
  } else {
    raw_data = data;
    stream_width = raw_data->size;
    if (raw_data->size != 0)
      stream.write(reinterpret_cast<char *>(raw_data->data), raw_data->size);
  }
}
/*!
 * If the stream is not empty we write the contents of the stream to the
 * underlying Gauge::Raw instance. If it is empty, we simply deallocate the
 * Gauge::Raw::data array.
 */
Gauge::Raw *Gauge::Serializer::Flush() {
  if (raw_data->data != NULL) {
    delete [] raw_data->data;
    raw_data->data = NULL;
  }
  raw_data->size = stream_width;
  if (raw_data->size != 0) {
    raw_data->data = new char[raw_data->size];
    stream.read(raw_data->data, stream_width);
  }
  stream_width = 0;
  return raw_data;
}
