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
 * @file tests/src/VectorTest.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 09.07.2012
 *
 * @brief This unittest is designed to define the operational parameters of the
 * Gauge::Vector class.
 */

#include <gtest/gtest.h>
#include <Random.h>

TEST(Constructors, Default) {
  Random::Seed();

  for (int trial = 0; trial < 100; ++trial) {
    Gauge::Vector *vector = new Gauge::Vector();
    EXPECT_EQ(1, vector->den);
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
    Gauge::Vector *vector = new Gauge::Vector(size);
    EXPECT_EQ(1, vector->den);
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
    Gauge::Vector *vector = new Gauge::Vector(size, den);
    EXPECT_EQ(den, vector->den);
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
    Gauge::Vector *vector = new Gauge::Vector(size, num, den);
    EXPECT_EQ(den, vector->den);
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
    Gauge::Vector *vector = Random::Vector(size);
    Gauge::Vector *copy = new Gauge::Vector(*vector);

    EXPECT_EQ(*vector, *copy);

    delete vector;
    delete copy;
  }
}

TEST(Operators, Assignment) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    Gauge::Vector *vector = Random::Vector(size);
    Gauge::Vector copy = *vector;

    EXPECT_EQ(*vector, copy);

    delete vector;
  }
}

TEST(Operators, Equal) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    Gauge::Vector *vector1 = Random::Vector(size);
    Gauge::Vector *vector2 = Random::Vector(size);

    EXPECT_TRUE(*vector1 == *vector1);
    EXPECT_TRUE(*vector2 == *vector2);

    bool are_equal = vector1->size == vector2->size;
    are_equal = are_equal && vector1->leading == vector2->leading;
    are_equal = are_equal && vector1->trailing == vector2->trailing;
    for(int index = 0; are_equal && index < vector1->size; ++index)
      are_equal = vector2->den * vector1->base[index] ==
                  vector1->den * vector2->base[index];

    EXPECT_EQ(are_equal, *vector1 == *vector2);

    delete vector1;
    delete vector2;
  }
}

TEST(Operators, LessThan) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    Gauge::Vector *vector1 = Random::Vector(size);
    Gauge::Vector *vector2 = Random::Vector(size);

    EXPECT_FALSE(*vector1 < *vector1);
    EXPECT_FALSE(*vector2 < *vector2);

    if (vector1->leading > vector2->leading)
      EXPECT_TRUE(*vector1 < *vector2);
    else if (vector1->leading < vector2->leading)
      EXPECT_FALSE(*vector1 < *vector2);
    else {
      int index = 0;
      for (; index < size; ++index) {
        int lhs = vector1->base[index] * vector2->den;
        int rhs = vector2->base[index] * vector1->den;
        if (lhs < rhs) {
          EXPECT_TRUE(*vector1 < *vector2);
          break;
        } else if (lhs > rhs) {
          EXPECT_FALSE(*vector1 < *vector2);
          break;
        }
      }
      if (index == size) EXPECT_FALSE(*vector1 < *vector2);
    }

    delete vector1;
    delete vector2;
  }
}

TEST(Operators, NotEqual) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    Gauge::Vector *vector1 = Random::Vector(size);
    Gauge::Vector *vector2 = Random::Vector(size);

    if (*vector1 == *vector2)
      EXPECT_FALSE(*vector1 != *vector2);
    else
      EXPECT_TRUE(*vector1 != *vector2);

    delete vector1;
    delete vector2;
  }
}

TEST(Operators, LessOrEqual) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    Gauge::Vector *vector1 = Random::Vector(size);
    Gauge::Vector *vector2 = Random::Vector(size);

    if (*vector1 < *vector2 || *vector1 == *vector2)
      EXPECT_TRUE(*vector1 <= *vector2);
    else
      EXPECT_FALSE(*vector1 <= *vector2);

    delete vector1;
    delete vector2;
  }
}

TEST(Operators, GreaterThan) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    Gauge::Vector *vector1 = Random::Vector(size);
    Gauge::Vector *vector2 = Random::Vector(size);

    if (*vector1 <= *vector2)
      EXPECT_FALSE(*vector1 > *vector2);
    else
      EXPECT_TRUE(*vector1 > *vector2);

    delete vector1;
    delete vector2;
  }
}

TEST(Operators, GreaterOrEqual) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    Gauge::Vector *vector1 = Random::Vector(size);
    Gauge::Vector *vector2 = Random::Vector(size);

    if (*vector1 < *vector2)
      EXPECT_FALSE(*vector1 >= *vector2);
    else
      EXPECT_TRUE(*vector1 >= *vector2);

    delete vector1;
    delete vector2;
  }
}

TEST(SerialiableInterface, WriteReadInvariance) {
  for (int trial = 0; trial < 100; ++trial) {
    int size = Random::Int(2,100);
    Gauge::Vector *input = Random::Vector(size);
    Gauge::Raw *raw_input = input->Serialize();

    Gauge::Vector *output = Random::Vector(Random::Int(2,100));
    output->Deserialize(raw_input);

    EXPECT_EQ(*input, *output);

    EXPECT_EQ(0, raw_input->size);
    EXPECT_EQ(NULL, raw_input->data);

    delete output;
    delete raw_input;
    delete input;
  }
}
