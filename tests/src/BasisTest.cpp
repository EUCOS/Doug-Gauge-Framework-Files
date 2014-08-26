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
 * @file tests/src/BasisTest.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 09.07.2012
 *
 * @brief This unittest is designed to define the operational parameters of the
 * Gauge::Basis class.
 */

#include <gtest/gtest.h>
#include <Random.h>

TEST(Constructors, Default) {
  Random::Seed();
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Basis *basis = new Gauge::Basis();

    EXPECT_EQ(0, basis->size);
    EXPECT_EQ(NULL, basis->base);

    delete basis;
  }
}

TEST(Constructors, Integer) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    Gauge::Basis *basis = new Gauge::Basis(size);

    Gauge::BasisVector empty;

    EXPECT_EQ(size, basis->size);
    for (int index = 0; index < basis->size; ++index)
      EXPECT_EQ(empty, basis->base[index]);

    delete basis;
  }
}

TEST(Constructors, BiInteger) {
  for (int trial = 0; trial < 1; ++trial) {
    int size = Random::Int(2,5);
    int width = Random::Int(2,100);
    Gauge::Basis *basis = new Gauge::Basis(size, width);
    Gauge::BasisVector fixed(width);

    EXPECT_EQ(size, basis->size);
    for (int index = 0; index < basis->size; ++index)
      EXPECT_EQ(fixed, basis->base[index]);

    delete basis;
  }
}

TEST(Constructors, Copy) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    int width = Random::Int(2,100);
    Gauge::Basis *basis = Random::Basis(size, width);
    Gauge::Basis *copy = new Gauge::Basis(*basis);

    EXPECT_TRUE(*basis == *copy);

    delete basis;
    delete copy;
  }
}

TEST(Operators, Assignment) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    int width = Random::Int(2,100);
    Gauge::Basis *basis = Random::Basis(size, width);
    Gauge::Basis copy = *basis;

    EXPECT_TRUE(*basis == copy);

    delete basis;
  }
}

TEST(Operators, Equal) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,10);
    int width = Random::Int(2,10);
    Gauge::Basis *lhs = Random::Basis(size, width);
    Gauge::Basis *rhs = Random::Basis(size, width);

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

TEST(Operators, NotEqual) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,10);
    int width = Random::Int(2,10);
    Gauge::Basis *lhs = Random::Basis(size, width);
    Gauge::Basis *rhs = Random::Basis(size, width);

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
    int width = Random::Int(2,100);
    Gauge::Basis *input = Random::Basis(size, width);
    Gauge::Raw *raw_input = input->Serialize();

    Gauge::Basis *output = Random::Basis(Random::Int(2,100), Random::Int(2,100));
    output->Deserialize(raw_input);

    EXPECT_EQ(input->size, output->size);
    for (int index = 0; index < input->size; ++index)
      EXPECT_EQ(input->base[index], output->base[index]);

    EXPECT_EQ(0, raw_input->size);
    EXPECT_EQ(NULL, raw_input->data);

    delete output;
    delete raw_input;
    delete input;
  }
}
