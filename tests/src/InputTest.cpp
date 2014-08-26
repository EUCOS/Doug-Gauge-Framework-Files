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
 * @file tests/src/InputTest.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 09.08.2012
 *
 * @brief This unittest is designed to define the operational parameters of the
 * Gauge::Input class.
 */

#include <gtest/gtest.h>
#include <Random.h>

TEST(Constructors, Default) {
  Random::Seed();
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Input *input = new Gauge::Input();

    EXPECT_EQ(4, input->dimensions);
    EXPECT_EQ(0, input->layers);
    EXPECT_EQ(NULL, input->orders);
    EXPECT_EQ(Gauge::Input::kSUSY, input->susy_type);

    delete input;
  }
}

TEST(Constructors, Full) {
  for (int trial = 0; trial < 100; ++trial) {
    int layers = Random::Int(1,20);
    int *orders = Random::IntArray(2,22,layers);
    int dimensions = Random::Int(2,5) * 2;
    Gauge::Input::SUSYType susy_type =
        static_cast<Gauge::Input::SUSYType>(Random::Int(0,3));
    Gauge::Input *input = new Gauge::Input(orders, layers, dimensions, susy_type);

    EXPECT_EQ(dimensions, input->dimensions);
    EXPECT_EQ(layers, input->layers);
    EXPECT_EQ(susy_type, input->susy_type);
    for (int index = 0; index < input->layers; ++index)
      EXPECT_EQ(orders[index], input->orders[index]);

    delete input;
    delete [] orders;
  }
}

TEST(Constructors, Copy) {
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Input *input = Random::Input();
    Gauge::Input *copy = new Gauge::Input(*input);

    EXPECT_EQ(*input, *copy);

    delete copy;
    delete input;
  }
}

TEST(Operators, Assignment) {
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Input *input = Random::Input();
    Gauge::Input copy = *input;

    EXPECT_EQ(*input, copy);

    delete input;
  }
}

TEST(Operators, Equal) {
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Input *lhs = Random::Input();
    Gauge::Input *rhs = Random::Input();

    bool equals = lhs->dimensions == rhs->dimensions;
    equals = equals && lhs->layers == rhs->layers;
    equals = equals && lhs->susy_type == rhs->susy_type;
    for (int index = 0; equals && index < lhs->layers; ++index)
      equals = lhs->orders[index] == rhs->orders[index];

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
    Gauge::Input *lhs = Random::Input();
    Gauge::Input *rhs = Random::Input();

    bool equals = lhs->dimensions == rhs->dimensions;
    equals = equals && lhs->layers == rhs->layers;
    equals = equals && lhs->susy_type == rhs->susy_type;
    for (int index = 0; equals && index < lhs->layers; ++index)
      equals = lhs->orders[index] == rhs->orders[index];

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
    Gauge::Input *input = Random::Input();
    Gauge::Raw *raw_input = input->Serialize();

    Gauge::Input *output = Random::Input();
    output->Deserialize(raw_input);

    EXPECT_EQ(input->dimensions, output->dimensions);
    EXPECT_EQ(input->layers, output->layers);
    EXPECT_EQ(input->susy_type, output->susy_type);
    for (int index = 0; index < input->layers; ++index)
      EXPECT_EQ(input->orders[index], output->orders[index]);

    EXPECT_EQ(0, raw_input->size);
    EXPECT_EQ(NULL, raw_input->data);

    delete output;
    delete raw_input;
    delete input;
  }
}
