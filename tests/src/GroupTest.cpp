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
 * @file tests/src/GroupTest.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 09.08.2012
 *
 * @brief This unittest is designed to define the operational parameters of the
 * Gauge::Group class.
 */

#include <gtest/gtest.h>
#include <Random.h>

TEST(Constructors, Default) {
  Random::Seed();
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Group *group = new Gauge::Group();

    EXPECT_EQ(0, group->rank);
    EXPECT_EQ(0, static_cast<int>(group->factors.size()));

    delete group;
  }
}

TEST(Constructors, Copy) {
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Group *group = Random::Group();
    Gauge::Group *copy = new Gauge::Group(*group);

    EXPECT_EQ(*group, *copy);

    delete copy;
    delete group;
  }
}

TEST(Operators, Assignment) {
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Group *group = Random::Group();
    Gauge::Group copy = *group;

    EXPECT_EQ(*group, copy);

    delete group;
  }
}

TEST(Operators, Equal) {
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Group *lhs = Random::Group();
    Gauge::Group *rhs = Random::Group();

    bool equal = lhs->rank == rhs->rank;
    equal = equal && lhs->factors.size() == rhs->factors.size();

    Gauge::Group::iterator lhs_iter = lhs->begin();
    Gauge::Group::iterator rhs_iter = rhs->begin();
    for (; equal && lhs_iter != lhs->end(); ++lhs_iter, ++rhs_iter)
      equal = **lhs_iter == **rhs_iter;

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
    Gauge::Group *lhs = Random::Group();
    Gauge::Group *rhs = Random::Group();

    bool equal = lhs->rank == rhs->rank;
    equal = equal && lhs->factors.size() == rhs->factors.size();

    Gauge::Group::iterator lhs_iter = lhs->begin();
    Gauge::Group::iterator rhs_iter = rhs->begin();
    for (; equal && lhs_iter != lhs->end(); ++lhs_iter, ++rhs_iter)
      equal = **lhs_iter == **rhs_iter;

    if (equal)
      EXPECT_FALSE(*lhs != *rhs);
    else
      EXPECT_TRUE(*lhs != *rhs);

    delete rhs;
    delete lhs;
  }
}

TEST(SerializableInterface, WriteReadInvariance) {
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Group *input = Random::Group();
    Gauge::Raw *raw_input = input->Serialize();

    Gauge::Group *output = Random::Group();
    output->Deserialize(raw_input);

    EXPECT_EQ(*input, *output);

    EXPECT_EQ(0, raw_input->size);
    EXPECT_EQ(NULL, raw_input->data);

    delete output;
    delete raw_input;
    delete input;
  }
}
