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
 * @file tests/src/NVectorTest.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 09.07.2012
 *
 * @brief This unittest is designed to define the operational parameters of the
 * Gauge::NVector class.
 */

#include <gtest/gtest.h>
#include <Random.h>

TEST(Constructors, Default) {
  Random::Seed();
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::NVector *vector = new Gauge::NVector();
    EXPECT_EQ(0, vector->size);
    EXPECT_EQ(NULL, vector->base);
    delete vector;
  }
}

TEST(Constructors, Integer) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    Gauge::NVector *vector = new Gauge::NVector(size);
    EXPECT_EQ(size, vector->size);
    for (int *iter = vector->base; iter != vector->base + vector->size; ++iter) {
      EXPECT_EQ(0, *iter);
    }
    delete vector;
  }
}

TEST(Constructors, Copy) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    Gauge::NVector *vector = Random::NVector(size);
    Gauge::NVector *copy = new Gauge::NVector(*vector);

    EXPECT_EQ(*vector, *copy);

    delete vector;
    delete copy;
  }
}

TEST(Operators, Assignment) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    Gauge::NVector *vector = Random::NVector(size);
    Gauge::NVector copy = *vector;

    EXPECT_EQ(*vector, copy);

    delete vector;
  }
}

TEST(Operators, Equals) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    Gauge::NVector *lhs = Random::NVector(size);
    Gauge::NVector *rhs = Random::NVector(size);

    bool equal = lhs->size == rhs->size;
    for (int index = 0; equal && index < lhs->size; ++index)
      equal = lhs->base[index] == rhs->base[index];

    if (equal)
      EXPECT_TRUE(*lhs == *rhs);
    else
      EXPECT_FALSE(*lhs == *rhs);

    delete rhs;
    delete lhs;
  }
}

TEST(Operators, NotEquals) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    Gauge::NVector *lhs = Random::NVector(size);
    Gauge::NVector *rhs = Random::NVector(size);

    bool equal = lhs->size == rhs->size;
    for (int index = 0; equal && index < lhs->size; ++index)
      equal = lhs->base[index] == rhs->base[index];

    if (equal)
      EXPECT_FALSE(*lhs != *rhs);
    else
      EXPECT_TRUE(*lhs != *rhs);

    delete rhs;
    delete lhs;
  }
}

TEST(SerialiableInterface, WriteReadInvariance) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    Gauge::NVector *input = Random::NVector(size);
    Gauge::Raw *raw_input = input->Serialize();

    Gauge::NVector *output = Random::NVector(Random::Int(2,100));
    output->Deserialize(raw_input);

    EXPECT_EQ(*input, *output);

    EXPECT_EQ(0, raw_input->size);
    EXPECT_EQ(NULL, raw_input->data);

    delete output;
    delete raw_input;
    delete input;
  }
}
