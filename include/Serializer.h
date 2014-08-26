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
 * @file include/Serializer.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 09.04.2012
 *
 * @brief The Gauge::Serializer class is defined.
 */

#pragma once

// C Headers
#include <cassert>
// C++ Headers
#include <sstream>
// Gauge Framework Headers
#include <Datatypes/Raw.h>

namespace Gauge {
  /*!
   * The Gauge::Serializer class is designed to provide a serialization
   * mechanism via an STL stringstream.
   *
   * The class allows reading to and writing from the data stream. When provided
   * a Gauge::Raw to begin with, the raw data is read into the stream;
   * otherwise, a new Gauge::Raw is created.  Subsequent read and write calls
   * alter the stream. No changes are made to the underlying Gauge::Raw are made
   * until a call to Gauge::Serializer::Flush is made. When this occurs the
   * stream is emptied.
   */
  class Serializer {
    public:
      /*!
       * The default constructor initializes the underlying Gauge::Raw to @c
       * NULL, sets the stream width to @c 0 and constructs an empty stream.
       */
      Serializer();
      /*!
       * The Gauge::Raw constructor takes a pointer to a Gauge::Raw, reads the
       * contents of the raw into the stream, sets the stream width to the width
       * of the Gauge::Raw and sets the internal Gauge::Raw pointer to the
       * provided location.
       *
       * @param[in,out] date The Gauge::Raw pointer to the initial values.
       */
      explicit Serializer(Gauge::Raw *data);
      /*!
       * The destructor does nothing. We don't want to deallocate the memory
       * allocated for the Gauge::Raw because we need access to it outside the
       * class after destruction of the Gauge:Serializer object.
       */
      virtual ~Serializer() {}
      /*!
       * The Gauge::Serializer::Write(const U&) method writes a primative or
       * contiguous type to the stream without compression.
       *
       * This method should work with primative types (@c int, @c float, etc...)
       * or contiguous types (structs or classes with only primative members).
       *
       * @param[in] input A constant reference to the data to be serialized.
       */
      template <typename T, typename U>
      void Write(const U &input);
      /*!
       * The Gauge::Serializer::WriteObject(const T&) method writes a generic
       * object to the stream without compression under the assumption that it
       * is adheres to the Gauge::Serializable interface.
       *
       * @param[in] input A constant reference to the data to be serialized.
       */
      template <typename T>
      void WriteObject(const T &input);
      /*!
       * The Gauge::Serializer::Write(T, T) method writes a range
       * of values to the stream. This is designed for writing iterable
       * objects.
       *
       * @param[in] begin A constant pointer to the start of the memory block.
       * @param[in] end   A constant pointer to the end of the memory block to
       * be serialized.
       */
      template <typename T, typename Iter>
      void Write(Iter begin, Iter end);
      /*!
       * The Gauge::Serializer::WriteObject(Iter, Iter) method writes a
       * range of values to the stream under the assumption that the objects in
       * the range adhere to the Gauge::Serializable interface.
       *
       * @param[in] begin An iterator to the start of the memory block.
       * @param[in] end   An iterator to the end of the memory block to
       * be serialized.
       */
      template <typename Iter>
      void WriteObject(Iter begin,Iter end);
      /*!
       * The Gauge::Serializer::Read(U*) method reads a primative or contiguous
       * type from a serialization without decompression.
       *
       * This method should work with primative types (@c int, @c float, etc...)
       * or contiguous types (structs or classes with only primative members).
       *
       * @param[out] output A pointer to the location into which the
       * unserialized data will be read.
       */
      template <typename T, typename U>
      void Read(U *output);
      /*!
       * The Gauge::Serializer::ReadObject(T*) method reads a generic object to
       * the stream without compression under the assumption that it is adheres
       * to the Gauge::Serializable interface.
       *
       * @param[out] output A pointer to the location into which the
       * unserialized data will be read.
       */
      template <typename T>
      void ReadObject(T *output);
      /*!
       * The Gauge::Serializer::Read(U,U) method reads a range of values from
       * the stream.  This is designed for reading arrays, specifically those
       * dynamically allocated.
       *
       * @param[out] begin  An iterator to the start of the memory block.
       * @param[out] end    An iterator to the end of the memory block to be
       * deserialized.
       */
      template <typename T, typename Iter>
      void Read(Iter begin, Iter end);
      /*!
       * The Gauge::Serializer::ReadObject(U, U) method reads a
       * range of values to the stream under the assumption that the objects in
       * the range adhere to the Gauge::Serializable interface.
       *
       * @param[in] begin An iterator to the start of the memory block.
       * @param[in] end   An iterator to the end of the memory block to be
       * deserialized.
       */
      template <typename Iter>
      void ReadObject(Iter begin, Iter end);
      /*!
       * The Gauge::Serializer::Flush writes the contents of the underlying
       * stream to the Gauge::Raw, clearing the stream in the process.
       *
       * @return A pointer to the internal Gauge::Raw instance. This may,
       * infact, have been declared and allocated outside of the
       * Gauge::Serializer.
       */
      Gauge::Raw *Flush();

    private:
      std::stringstream stream; /*!< The underlying stream for serialization. */
      int stream_width;         /*!< The byte-width of the stream.            */
      Gauge::Raw *raw_data;     /*!< The Gauge::Raw to which the serialized data
                                     will be written.                         */

      /*!
       * The copy constructor is defined privately. This ensures that
       * Gauge::Serializer instances cannot be efficiently copied.
       *
       * @param[in] other The Gauge::Serializer from which to copy.
       */
      Serializer(const Gauge::Serializer &other) {}
      /*!
       * The assignment operator is defined privately. This ensures that
       * Gauge::Serializer instances cannot be efficiently copied.
       *
       * @param[in] other The Gauge::Serializer from which to copy.
       *
       * @return The resulting Gauge::Serializer.
       */
      Gauge::Serializer &operator=(const Gauge::Serializer &other) {
        return *this;
      }
  };
  /*!
   * The Gauge::Serializer::Write(const U&) method simply casts the data into an
   * character array, writes the array to the stream and increases the
   * stream_width accordingly.
   */
  template <typename T, typename U>
  void Gauge::Serializer::Write(const U &input) {
    T holder = static_cast<T>(input);
    int size = sizeof(holder);
    stream.write(reinterpret_cast<const char *>(&holder), size);
    stream_width += size;
  }
  /*!
   * The Gauge::Serializer::WriteObject(const T&) method simply makes an
   * interface call to the Gauge::Serializable::SerializeWith method using @c
   * this as the argument.
   */
  template <typename T>
  void Gauge::Serializer::WriteObject(const T &input) {
    input.SerializeWith(this);
  }
  /*!
   * The Gauge::Serializer::Write(const T*, const T*) method first ensures that
   * the begin pointer and the end pointer are valid (the end pointer follows
   * the begin pointer). If this is the case we write the array data to the
   * stream and increase the stream_width accordingly.
   *
   * If the bounds are invalid then the program terminates with a C-assertion.
   */
  template <typename T, typename Iter>
  void Gauge::Serializer::Write(Iter begin, Iter end) {
    for (Iter iter = begin; iter != end; ++iter)
      Write<T>(*iter);
  }
  /*!
   * The Gauge::Serializer::WriteObject(T, T) method first ensures
   * that the begin pointer and the end pointer are valid (the end pointer
   * follows the begin pointer). If this is the case, we simply make an
   * interface call to the Gauge::Serializable::SerializeWith method using @c
   * this as the argument for each element in the range.
   *
   * If the bounds are invalid then the program terminates with a C-assertion.
   */
  template <typename T>
  void Gauge::Serializer::WriteObject(T begin, T end) {
    for (T iter = begin; iter != end; ++iter)
      iter->SerializeWith(this);
  }
  /*!
   * The Gauge::Serializer::Read(U*) method first checks to ensure that you are
   * not attempting to read more data from the stream than is actually in the
   * stream. If this is the case the program will terminate via C-assertion.
   *
   * If we can read from the stream, we write the data to the location provided
   * and we reduce the stream_width accordingly.
   */
  template <typename T, typename U>
  void Gauge::Serializer::Read(U *output) {
    T holder;
    int size = sizeof(holder);
    assert(stream_width >= size);
    stream.read(reinterpret_cast<char *>(&holder), size);
    stream_width -= size;
    *output = static_cast<U>(holder);
  }
  /*!
   * The Gauge::Serializer::ReadObject(T*) method simply makes an interface call
   * to the Gauge::Serializable::DeserializeWith method using @c
   * this as the argument.
   */
  template <typename T>
  void Gauge::Serializer::ReadObject(T *output) {
    output->DeserializeWith(this);
  }
  /*!
   * The Gauge::Serializer::Read(Iter, Iter) method first ensures that the begin
   * pointer and the end pointer are valid (the end pointer follows the begin
   * pointer). If this is the case we read the array data from the stream and
   * decrease the stream_width accordingly.
   *
   * If the bounds are invalid then the program terminates with a C-assertion.
   */
  template <typename T, typename Iter>
  void Gauge::Serializer::Read(Iter begin, Iter end) {
    for (Iter iter = begin; iter != end; ++iter)
      Read<T>(&*iter);
  }
  /*!
   * The Gauge::Serializer::ReadObject(Iter, Iter) method first ensures
   * that the begin pointer and the end pointer are valid (the end pointer
   * follows the begin pointer). If this is the case, we simply make an
   * interface call to the Gauge::Serializable::DeserializeWith method using @c
   * this as the argument for each element in the range.
   *
   * If the bounds are invalid then the program terminates with a C-assertion.
   */
  template <typename Iter>
  void Gauge::Serializer::ReadObject(Iter begin, Iter end) {
    for (Iter iter = begin; iter != end; ++iter)
      iter->DeserializeWith(this);
  }
}
