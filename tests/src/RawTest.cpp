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
    Gauge::Raw *raw = new Gauge::Raw();
    EXPECT_EQ(0, raw->size);
    EXPECT_EQ(NULL, raw->data);
    delete raw;
  }
}

TEST(Constructors, Integer) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    Gauge::Raw *raw = new Gauge::Raw(size);
    EXPECT_EQ(size, raw->size);
    for (char *iter = raw->data; iter != raw->data + raw->size; ++iter)
      EXPECT_EQ('\0', *iter);
    delete raw;
  }
}

TEST(Constructors, Copy) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,1000);
    Gauge::Raw *raw = Random::Raw(size);
    Gauge::Raw *copy = new Gauge::Raw(*raw);

    EXPECT_EQ(*raw, *copy);

    delete copy;
    delete raw;
  }
}

TEST(Operators, Assignment) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,1000);
    Gauge::Raw *raw = Random::Raw(size);
    Gauge::Raw copy = *raw;

    EXPECT_EQ(*raw, copy);

    delete raw;
  }
}

TEST(Operators, Equals) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,1000);
    Gauge::Raw *lhs = Random::Raw(size);
    Gauge::Raw *rhs = Random::Raw(size);

    bool equals = lhs->size == rhs->size;
    for (int index = 0; equals && index < size; ++index)
      equals = lhs->data[index] == rhs->data[index];

    if (equals)
      EXPECT_TRUE(*lhs == *rhs);
    else
      EXPECT_FALSE(*lhs == *rhs);

    delete rhs;
    delete lhs;
  }
}

TEST(Operators, NotEquals) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,1000);
    Gauge::Raw *lhs = Random::Raw(size);
    Gauge::Raw *rhs = Random::Raw(size);

    bool equals = lhs->size == rhs->size;
    for (int index = 0; equals && index < size; ++index)
      equals = lhs->data[index] == rhs->data[index];

    if (equals)
      EXPECT_FALSE(*lhs != *rhs);
    else
      EXPECT_TRUE(*lhs != *rhs);

    delete rhs;
    delete lhs;
  }
}
