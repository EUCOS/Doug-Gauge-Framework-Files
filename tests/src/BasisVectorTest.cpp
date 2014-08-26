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
 * @file tests/src/BasisVectorTest.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 09.07.2012
 *
 * @brief This unittest is designed to define the operational parameters of the
 * Gauge::BasisVector class.
 */

#include <gtest/gtest.h>
#include <Random.h>

TEST(Constructors, Default) {
  Random::Seed();
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::BasisVector *vector = new Gauge::BasisVector();
    EXPECT_EQ(1, vector->den);
    EXPECT_EQ(vector->den, vector->order);
    EXPECT_EQ(0, vector->size);
    EXPECT_EQ(NULL, vector->begin);
    EXPECT_EQ(NULL, vector->end);
    EXPECT_EQ(NULL, vector->base);
    EXPECT_EQ(0, vector->leading);
    EXPECT_EQ(0, vector->trailing);

    delete vector;
  }
}

TEST(Constructors, Integer) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    Gauge::BasisVector *vector = new Gauge::BasisVector(size);
    EXPECT_EQ(1, vector->den);
    EXPECT_EQ(vector->den, vector->order);
    EXPECT_EQ(size, vector->size);
    ASSERT_EQ(vector->base, vector->begin);
    ASSERT_EQ(vector->base + size, vector->end);
    for (int *iter = vector->begin; iter != vector->end; ++iter) {
      EXPECT_EQ(0, *iter);
    }
    EXPECT_EQ(size, vector->leading);
    EXPECT_EQ(0, vector->trailing);
    delete vector;
  }
}

TEST(Constructors, BiInteger) {
  for (int trial = 0; trial < 100; ++trial) {
    int den  = Random::Int(2,100);
    int size = Random::Int(2,100);
    Gauge::BasisVector *vector = new Gauge::BasisVector(size, den);
    EXPECT_EQ(den, vector->den);
    EXPECT_EQ(vector->den, vector->order);
    EXPECT_EQ(size, vector->size);
    ASSERT_EQ(vector->base, vector->begin);
    ASSERT_EQ(vector->base + size, vector->end);
    for (int *iter = vector->begin; iter != vector->end; ++iter) {
      EXPECT_EQ(0, *iter);
    }
    EXPECT_EQ(size, vector->leading);
    EXPECT_EQ(0, vector->trailing);
    delete vector;
  }
}

TEST(Constructors, TriInteger) {
  for (int trial = 0; trial < 100; ++trial) {
    int num  = Random::Int(2,100);
    int den  = Random::Int(2,100);
    int size = Random::Int(2,100);
    Gauge::BasisVector *vector = new Gauge::BasisVector(size, num, den);
    EXPECT_EQ(den, vector->den);
    EXPECT_EQ(vector->den, vector->order);
    EXPECT_EQ(size, vector->size);
    ASSERT_EQ(vector->base, vector->begin);
    ASSERT_EQ(vector->base + size, vector->end);
    for (int *iter = vector->begin; iter != vector->end; ++iter) {
      EXPECT_EQ(num, *iter);
    }
    EXPECT_EQ(0, vector->leading);
    EXPECT_EQ(size, vector->trailing);
    delete vector;
  }
}

TEST(Constructors, Copy) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    Gauge::BasisVector *vector = Random::BasisVector(size);
    Gauge::BasisVector *copy = new Gauge::BasisVector(*vector);

    EXPECT_EQ(*vector, *copy);

    delete vector;
    delete copy;
  }
}

TEST(Operators, Assignment) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    Gauge::BasisVector *vector = Random::BasisVector(size);
    Gauge::BasisVector copy = *vector;

    EXPECT_EQ(*vector, copy);

    delete vector;
  }
}

TEST(SerialiableInterface, WriteReadInvariance) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    Gauge::BasisVector *input = Random::BasisVector(size);
    Gauge::Raw *raw_input = input->Serialize();

    Gauge::BasisVector *output = Random::BasisVector(Random::Int(2,100));
    output->Deserialize(raw_input);

    EXPECT_EQ(*input, *output);

    EXPECT_EQ(0, raw_input->size);
    EXPECT_EQ(NULL, raw_input->data);

    delete output;
    delete raw_input;
    delete input;
  }
}
