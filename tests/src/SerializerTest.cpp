/*
 * This file is part of The Gauge Framework.
 *
 * The Gauge Framework is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * The Gauge Framework is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with The Gauge Framework. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * @file test/src/SerializerTest.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 09.04.2012
 *
 * @brief This unittest is designed to define the operational parameters of the
 * Gauge::Serializer class.
 */

#include <gtest/gtest.h>
#include <Random.h>
#include <Serializer.h>

TEST(Constructors, DefaultConstructor) {
  Random::Seed();
  Gauge::Serializer serializer;
  Gauge::Raw *raw = serializer.Flush();
  EXPECT_EQ(0, raw->size);
  EXPECT_EQ(NULL, raw->data);
  delete raw;
}

TEST(Constructors, RawConstructor) {
  for (int trial = 0; trial < 1000; ++trial) {
    int size = Random::Int(1,1000);
    Gauge::Raw *raw = Random::Raw(size);
    Gauge::Raw *old = new Gauge::Raw(*raw);
    Gauge::Serializer serializer(raw);

    Gauge::Raw *serialized = serializer.Flush();
    EXPECT_EQ(raw, serialized);
    EXPECT_EQ(old->size, serialized->size);
    for (int index = 0; index < size; ++index)
      EXPECT_EQ(old->data[index], serialized->data[index]);

    delete raw;
    delete old;
  }
}

TEST(Deserialize, WriteReadInvariance_Primative) {
  for (int trial = 0; trial < 1; ++trial) {
    Gauge::Serializer input;
    int size = Random::Int(1,1000);
    int *random_integers = Random::IntArray(-32000, 32000, size);
    for (int index = 0; index < size; ++index)
      input.Write<int>(random_integers[index]);
    Gauge::Raw *raw = input.Flush();

    Gauge::Serializer output(raw);
    int value = 0;
    for (int index = 0; index < size; ++index) {
      output.Read<int>(&value);
      EXPECT_EQ(random_integers[index], value);
    }
    output.Flush();

    EXPECT_EQ(0, raw->size);
    EXPECT_EQ(NULL, raw->data);

    delete [] random_integers;
    delete raw;
  }
}

TEST(Deserialize, WriteReadInvariance_Array) {
  for (int trial = 0; trial < 1; ++trial) {
    Gauge::Serializer input;
    int size = Random::Int(1,1000);
    int *random_integers = Random::IntArray(-32000, 32000, size);
    input.Write<int>(random_integers, random_integers + size);
    Gauge::Raw *raw = input.Flush();

    Gauge::Serializer output(raw);
    int *value = new int[size];
    output.Read<int>(value, value + size);
    for (int index = 0; index < size; ++index) {
      EXPECT_EQ(random_integers[index], value[index]);
    }
    output.Flush();

    EXPECT_EQ(0, raw->size);
    EXPECT_EQ(NULL, raw->data);

    delete [] random_integers;
    delete [] value;
    delete raw;
  }
}

TEST(Compression, WriteReadInvariance_Primative) {
  for (int trial = 0; trial < 1; ++trial) {
    Gauge::Serializer input;
    int size = Random::Int(1,1000);
    int *random_integers = Random::IntArray(-127, 127, size);
    for (int index = 0; index < size; ++index)
      input.Write<char>(random_integers[index]);
    Gauge::Raw *raw = input.Flush();

    Gauge::Serializer output(raw);
    int value = 0;
    for (int index = 0; index < size; ++index) {
      output.Read<char>(&value);
      EXPECT_EQ(random_integers[index], value);
    }
    output.Flush();

    EXPECT_EQ(0, raw->size);
    EXPECT_EQ(NULL, raw->data);

    delete [] random_integers;
    delete raw;
  }
}

TEST(Decompression, WriteReadInvariance_Array) {
  for (int trial = 0; trial < 1; ++trial) {
    Gauge::Serializer input;
    int size = Random::Int(1,1000);
    int *random_integers = Random::IntArray(-127, 127, size);
    input.Write<char>(random_integers, random_integers + size);
    Gauge::Raw *raw = input.Flush();

    Gauge::Serializer output(raw);
    int *value = new int[size];
    output.Read<char>(value, value + size);
    for (int index = 0; index < size; ++index) {
      EXPECT_EQ(random_integers[index], value[index]);
    }
    output.Flush();

    EXPECT_EQ(0, raw->size);
    EXPECT_EQ(NULL, raw->data);

    delete [] random_integers;
    delete [] value;
    delete raw;
  }
}
