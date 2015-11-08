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
 * @file tests/src/ModelTest.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 09.09.2012
 *
 * @brief This unittest is designed to define the operational parameters of the
 * Gauge::Model class.
 */

#include <gtest/gtest.h>
#include <Random.h>

TEST(Constructors, Default) {
  Random::Seed();
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Model *model = new Gauge::Model();

    EXPECT_EQ(0, model->susy);
    EXPECT_EQ(NULL, model->geometry);
    EXPECT_EQ(NULL, model->group);

    delete model;
  }
}

TEST(Operators, Equal) {
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Model *lhs = Random::Model();
    Gauge::Model *rhs = Random::Model();

    bool equal = lhs->susy == rhs->susy;
    equal = equal && *lhs->geometry == *rhs->geometry;
    equal = equal && *lhs->group == *rhs->group;

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
    Gauge::Model *lhs = Random::Model();
    Gauge::Model *rhs = Random::Model();

    bool equal = lhs->susy == rhs->susy;
    equal = equal && *lhs->geometry == *rhs->geometry;
    equal = equal && *lhs->group == *rhs->group;

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
    Gauge::Model *input = Random::Model();
    Gauge::Raw *raw_input = input->Serialize();

    Gauge::Model *output = Random::Model();
    output->Deserialize(raw_input);

    EXPECT_EQ(*input, *output);

    EXPECT_EQ(0, raw_input->size);
    EXPECT_EQ(NULL, raw_input->data);

    delete output;
    delete raw_input;
    delete input;
  }
}
