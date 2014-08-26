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
 * @file tests/src/GSOMatrixTest.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 09.08.2012
 *
 * @brief This unittest is designed to define the operational parameters of the
 * Gauge::GSOMatrix class.
 */

#include <gtest/gtest.h>
#include <Random.h>

TEST(Constructors, Default) {
  Random::Seed();
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::GSOMatrix *gsomatrix = new Gauge::GSOMatrix();

    EXPECT_EQ(0, gsomatrix->size);
    EXPECT_EQ(NULL, gsomatrix->base);

    delete gsomatrix;
  }
}

TEST(Constructors, Integer) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(1,100);
    Gauge::GSOMatrix *gsomatrix = new Gauge::GSOMatrix(size);

    EXPECT_EQ(size, gsomatrix->size);
    for (int row = 0; row < size; ++row)
      for (int col = 0; col < size; ++col)
        EXPECT_EQ(Gauge::Math::Rational(0), gsomatrix->base[row][col]);

    delete gsomatrix;
  }
}

TEST(Constructors, Copy) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(1,100);
    Gauge::GSOMatrix *gsomatrix = Random::GSOMatrix(size);
    Gauge::GSOMatrix *copy = new Gauge::GSOMatrix(*gsomatrix);

    EXPECT_EQ(*gsomatrix, *copy);

    delete copy;
    delete gsomatrix;
  }
}

TEST(Operators, Assignment) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(1,100);
    Gauge::GSOMatrix *gsomatrix = Random::GSOMatrix(size);
    Gauge::GSOMatrix copy = *gsomatrix;

    EXPECT_EQ(*gsomatrix, copy);

    delete gsomatrix;
  }
}

TEST(Operators, Equal) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(1,100);
    Gauge::GSOMatrix *lhs = Random::GSOMatrix(size);
    Gauge::GSOMatrix *rhs = Random::GSOMatrix(size);

    bool equals = lhs->size == rhs->size;
    for (int row = 0; equals && row < size; ++row)
      for (int col = 0; equals && col < size; ++col)
        equals = lhs->base[row][col] == rhs->base[row][col];

    if (equals)
      EXPECT_TRUE(*lhs == *rhs);
    else
      EXPECT_FALSE(*lhs == *rhs);

    delete rhs;
    delete lhs;
  }
}

TEST(Operators, NotEqual) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(1,100);
    Gauge::GSOMatrix *lhs = Random::GSOMatrix(size);
    Gauge::GSOMatrix *rhs = Random::GSOMatrix(size);

    bool equals = lhs->size == rhs->size;
    for (int row = 0; equals && row < size; ++row)
      for (int col = 0; equals && col < size; ++col)
        equals = lhs->base[row][col] == rhs->base[row][col];

    if (equals)
      EXPECT_FALSE(*lhs != *rhs);
    else
      EXPECT_TRUE(*lhs != *rhs);

    delete rhs;
    delete lhs;
  }
}

TEST(SerialiableInterface, WriteReadInvariance) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(1,100);
    Gauge::GSOMatrix *input = Random::GSOMatrix(size);
    Gauge::Raw *raw_input = input->Serialize();

    Gauge::GSOMatrix *output = Random::GSOMatrix(Random::Int(1,100));
    output->Deserialize(raw_input);

    EXPECT_EQ(*input, *output);

    EXPECT_EQ(0, raw_input->size);
    EXPECT_EQ(NULL, raw_input->data);

    delete output;
    delete raw_input;
    delete input;
  }
}
