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
 * @file tests/src/GeometryTest.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 09.09.2012
 *
 * @brief This unittest is designed to define the operational parameters of the
 * Gauge::Geometry class.
 */

#include <gtest/gtest.h>
#include <Random.h>

TEST(Constructors, Default) {
  Random::Seed();
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Geometry *geometry = new Gauge::Geometry();

    EXPECT_EQ(Gauge::Basis(), geometry->basis);
    EXPECT_EQ(Gauge::GSOMatrix(), geometry->gso_matrix);

    delete geometry;
  }
}

TEST(Constructors, Full) {
  for (int trial = 0; trial < 100; ++trial) {
    int size  = Random::Int(2,22);
    int width = Random::Int(2,22);
    Gauge::Basis *basis = Random::Basis(size, width);
    Gauge::GSOMatrix *gsomatrix = Random::GSOMatrix(size);
    Gauge::Geometry *geometry = new Gauge::Geometry(*basis, *gsomatrix);

    EXPECT_EQ(*basis, geometry->basis);
    EXPECT_EQ(*gsomatrix, geometry->gso_matrix);

    delete geometry;
    delete gsomatrix;
    delete basis;
  }
}

TEST(Constructor, Copy) {
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Geometry *geometry = Random::Geometry();
    Gauge::Geometry *copy = new Gauge::Geometry(*geometry);

    EXPECT_EQ(*geometry, *copy);

    delete geometry;
    delete copy;
  }
}

TEST(Operators, Assignment) {
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Geometry *geometry = Random::Geometry();
    Gauge::Geometry copy = *geometry;

    EXPECT_EQ(*geometry, copy);

    delete geometry;
  }
}

TEST(Operators, Equal) {
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Geometry *lhs = Random::Geometry();
    Gauge::Geometry *rhs = Random::Geometry();

    if (lhs->basis == rhs->basis && lhs->gso_matrix == rhs->gso_matrix)
      EXPECT_TRUE(*lhs == *rhs);
    else
      EXPECT_FALSE(*lhs == *rhs);

    delete rhs;
    delete lhs;
  }
}

TEST(Operators, NotEqual) {
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Geometry *lhs = Random::Geometry();
    Gauge::Geometry *rhs = Random::Geometry();

    if (lhs->basis == rhs->basis && lhs->gso_matrix == rhs->gso_matrix)
      EXPECT_TRUE(*lhs == *rhs);
    else
      EXPECT_FALSE(*lhs == *rhs);

    delete rhs;
    delete lhs;
  }
}

TEST(SerialiableInterface, WriteReadInvariance) {
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Geometry *input = Random::Geometry();
    Gauge::Raw *raw_input = input->Serialize();

    Gauge::Geometry *output = Random::Geometry();
    output->Deserialize(raw_input);

    EXPECT_EQ(*input, *output);

    EXPECT_EQ(0, raw_input->size);
    EXPECT_EQ(NULL, raw_input->data);

    delete output;
    delete raw_input;
    delete input;
  }
}
