/*
 * This file is part of The Gauge Framework.
 *
 * The Gauge Framework is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * The Gauge Framework is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * The Gauge Framework. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * @file tests/src/RationalTest.cpp
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
    Gauge::Math::Rational *rat = new Gauge::Math::Rational();
    EXPECT_EQ(0, rat->num);
    EXPECT_EQ(1, rat->den);
    delete rat;
  }
}

TEST(Constructors, Integer) {
  for (int trial = 0; trial < 100; ++trial) {
    int num = Random::Int(-100,100);
    Gauge::Math::Rational *rat = new Gauge::Math::Rational(num);
    EXPECT_EQ(num, rat->num);
    EXPECT_EQ(1, rat->den);
    delete rat;
  }
}

TEST(Constructors, BiInteger) {
  for (int trial = 0; trial < 100; ++trial) {
    int num = Random::Int(-100,100);
    int den = Random::Int(1,100);
    Gauge::Math::Rational *rat = new Gauge::Math::Rational(num, den);
    EXPECT_EQ(num, rat->num);
    EXPECT_EQ(den, rat->den);
    delete rat;
  }
}

TEST(Constructors, Copy) {
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Math::Rational *rat = Random::Rational();
    Gauge::Math::Rational *copy = new Gauge::Math::Rational(*rat);

    EXPECT_EQ(*rat, *copy);

    delete copy;
    delete rat;
  }
}

TEST(Operators, Assignment) {
  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Math::Rational *rat = Random::Rational();
    Gauge::Math::Rational copy = *rat;

    EXPECT_EQ(*rat, copy);

    delete rat;
  }
}

TEST(SerialiableInterface, WriteReadInvariance) {
  for (int trial = 0; trial < 1; ++trial) {
    Gauge::Math::Rational *input = Random::Rational();
    Gauge::Raw *raw_input = input->Serialize();

    Gauge::Math::Rational *output = Random::Rational();
    output->Deserialize(raw_input);

    EXPECT_EQ(*input, *output);

    EXPECT_EQ(0, raw_input->size);
    EXPECT_EQ(NULL, raw_input->data);

    delete output;
    delete raw_input;
    delete input;
  }
}
