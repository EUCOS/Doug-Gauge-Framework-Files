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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with The Gauge Framework.  If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * @file NVectorHandler.cpp
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 02.27.2012
 *
 * @brief An implementation of the Gauge::NVectorHandler class.
 *
 * The Gauge::NVectorHandler class deals with simple NVector generation.
 */

// C Headers
#include <cassert>
#include <cstdio>
#include <cstdlib>

// Gauge Framework Headers
#include <Math.h>
#include <NVectorHandler.h>

/*!
 * We initialize Gauge::NVectorHandler::solution_ to @c NULL.
 */
Gauge::NVectorHandler::NVectorHandler() {
  amatrix_ = NULL;
  avalue_ = 0;
  barriers_ = NULL;
  conjugates_ = NULL;
  constraints_ = NULL;
  current_ = 0;
  holders_ = NULL;
  layer_ = 0;
  moduli_ = NULL;
  multiplicity_ = NULL;
  orders_ = NULL;
  replication_ = NULL;
  setup_ = false;
  size_ = 0;
  solution_ = NULL;
  solvers_ = NULL;
}

/*!
 * The destructor deallocates the following dynamically allocated pointers:
 *  - Gauge::NVectorHandler::amatrix_
 *  - Gauge::NVectorHandler::barriers_
 *  - Gauge::NVectorHandler::conjugates_
 *  - Gauge::NVectorHandler::constraints_
 *  - Gauge::NVectorHandler::holders_
 *  - Gauge::NVectorHandler::moduli_
 *  - Gauge::NVectorHandler::multiplicity_
 *  - Gauge::NVectorHandler::orders_
 *  - Gauge::NVectorHandler::replication_
 *  - Gauge::NVectorHandler::solution_
 *  - Gauge::NvectorHandler::solvers_
 */
Gauge::NVectorHandler::~NVectorHandler() {
  if (amatrix_ != NULL) {
    for (int index = 0; index < layer_; ++index) {
      if (amatrix_[index] != NULL) delete [] amatrix_[index];
    }
    delete [] amatrix_;
  }
  if (barriers_ != NULL) delete [] barriers_;
  if (conjugates_ != NULL) delete [] conjugates_;
  if (constraints_ != NULL) {
    int number_of_constraints = layer_*(layer_-1)/2;
    for (int index = 0; index < number_of_constraints; ++index) {
      if (constraints_[index] != NULL) delete [] constraints_[index];
    }
    delete [] constraints_;
  }
  if (holders_ != NULL) delete [] holders_;
  if (moduli_ != NULL) delete [] moduli_;
  if (multiplicity_ != NULL) delete [] multiplicity_;
  if (orders_ != NULL) delete [] orders_;
  if (replication_ != NULL) delete [] replication_;
  if (solution_ != NULL) delete solution_;
  if (solvers_ != NULL) delete [] solvers_;
}

/*!
 * This method is simply a public interface to the
 * Gauge::NVectorHandler::FindNextSolution. Essentially, it removes the need to
 * provide a boolean argument specifying the need to distribute the solution.
 */
bool Gauge::NVectorHandler::NextSolution() {
  assert(setup_);
  return FindNextSolution(holders_[current_] != NULL);
}

/*!
 * The Setup method makes calls to private, working methods to setup the
 * Gauge::NVectorHandler for use. The following steps are taken:
 *  - Calculate the multiplicity and replication.
 *      (Gauge::NVectorHandler::multiplicity_,
 *       Gauge::NVectorHandler::replication_,
 *       Gauge::NVectorHandler::SetupMultiplicities)
 *  - Construct the a-matrix.
 *      (Gauge::NVectorHandler::amatrix_,
 *       Gauge::NVectorHandler::SetupAMatrix)
 *  - Find the barriers.
 *      (Gauge::NVectorHandler::barriers_)
 *  - Setup the moduli.
 *      (Gauge::NVectorHandler::moduli_)
 *  - Setup the mixed constraints.
 *      (Gauge::NVectorHandler::constraints_,
 *       Gauge::NVectorHandler::SetupConstraints)
 *  - Resolve the n-values that are conjugates.
 *      (Gauge::NVectorHandler::conjugates_,
 *       Gauge::NVectorHandler::SetupConjugates)
 *  - Setup the solvers.
 *      (Gauge::NVectorHandler::solvers_,
 *       Gauge::NVectorHandler::SetupSolvers)
 *  - Set the current layer to @c 0.
 *      (Gauge::NVectorHandler::current_)
 *  - Initialize the current solution.
 *      (Gauge::NVectorHandler::solution_)
 *  - Setup the current equation.
 *      (Gauge::NVectorHandler::SetupEquation)
 *  - Set the flag specifying that we have completed the Setup.
 *      (Gauge::NVectorHandler::setup_)
 */
void Gauge::NVectorHandler::Setup(const int *orders, int layers, int size) {
  assert(layers > 0);
  assert(size <= 22 && size >= 16);
  for (int i = 0; i < layers; ++i) assert(orders[i] >= 2 && orders[i] < 100);

  // Copy the provided array of orders to the internal storage.
  if (orders_ != NULL) delete [] orders_;
  orders_ = new int[layers];
  for (int index = 0; index < layers; ++index) orders_[index] = orders[index];

  // Set @c this->layer_ from input.
  layer_ = layers;

  // Set @c this->size_ from input.
  size_ = size;

  // Calculate the avalue (product of orders less one).
  avalue_ = Gauge::Math::Product(orders, orders+layers, 1) - 1;

  SetupMultiplicities();  // Setup the multiplicity and replication arrays.
  SetupAMatrix();         // Setup the a-matrix and barriers.
  SetupConstraints();     // Setup the moduli and modular invariance constraints
  if (layer_ != 1) {
    SetupConjugates();    // Determine which n-values are conjugates.
  }
  SetupSolvers();         // Setup the layer one solvers.

  current_ = 0;
  if (solution_ != NULL) delete solution_;
  solution_ = new Gauge::NVector(avalue_);
  SetupEquation();

  setup_ = true;
}

/*!
 * The default constructor simply initializes the
 * Gauge::NVectorHandler::Solver::coefficients_ array to @c NULL and sets the
 * Gauge::NVectorHandler::Solver::setup_ flag to @c false.
 */
Gauge::NVectorHandler::Solver::Solver() {
  coefficients_ = NULL;
  setup_ = false;
}

/*!
 * The destructor deallocates the following dynamically allocated pointers:
 *  - Gauge::NVectorHandler::coefficients_;
 */
Gauge::NVectorHandler::Solver::~Solver() {
  if (coefficients_ != NULL) delete [] coefficients_;
}

/*!
 * We perform the following setup of the class:
 *    -# Calculate the width of the solution and coefficients.
 *        (Gauge::NVectorHandler::Solver::solution_,
 *         Gauge::NVectorHandler::Solver::coefficients_)
 *    -# Allocate the memory for the coefficients.
 *        (Gauge::NVectorHandler::Solver::coefficients_)
 *    -# Initalize the coefficients.
 *        (Gauge::NVectorHandler::Solver::coefficients_)
 *    -# Initialize the flag signifying that we have not found yet solutions.
 *        (Gauge::NVectorHandler::Solver::first_)
 *    -# Initalize the maximum size.
 *        (Gauge::NVectorHandler::Solver::maximum_size_)
 *    -# Initalize the minimum total.
 *        (Gauge::NVectorHandler::Solver::minimum_total_)
 *    -# Initialize the modulus.
 *        (Gauge::NVectorHandler::Solver::modulus_).
 *    -# Initialize the flag signifying that we have setup the solver.
 *        (Gauge::NVectorHandler::Solver::setup_)
 *    -# Initialize the size of the current solution to @c 0.
 *        (Gauge::NVectorHandler::Solver::size_)
 *    -# Initalize the internal solution.
 *        (Gauge::NVectorHandler::Solver::solution_)
 *    -# Initialize the total of the current solution to @c 0.
 *        (Gauge::NVectorHandler::Solver::total_)
 */
void Gauge::NVectorHandler::Solver::Setup(int order, int maximum_size, int
    minimum_total) {
  int width = order/2;
  if (coefficients_ != NULL) delete [] coefficients_;
  coefficients_ = new int[width];
  for (int i = 0; i < width; ++i) coefficients_[i] = (width - i) * (width - i);
  first_ = true;
  maximum_size_ = maximum_size;
  minimum_total_ = minimum_total;
  modulus_ = (2 - (order & 1)) * order;
  setup_ = true;
  size_ = 0;
  solution_ = Gauge::NVector(width);
  total_ = 0;
}

bool Gauge::NVectorHandler::Solver::NextSolution() {
  /*!
   * We begin by ensuring that Gauge::NVectorHandler::Solver::Setup has been run
   * via a C assert check of the Gauge::NVectorHandler::Solver::setup_ flag.
   */
  assert(setup_);
  /*!
   * Now, because all of the methods that are called here return booleans and we
   * only want to stop when any one of them returns @c true, we use the
   * short-circuited logical "or" operator to make the following calls in order:
   *  - Gauge::NVectorHandler::Solver::FirstSolution
   *  - Gauge::NVectorHandler::Solver::Decompose
   *  - Gauge::NVectorHandler::Solver::Maximize
   *
   * and return the result. Thus, if they all fail we return @c false and @c
   * true otherwise.
   */
  return (first_ && FirstSolution()) || Decompose() || Maximize();
}

/*!
 * We, very simply, set the new Gauge::NVectorHandler::Solver::minimum_total_,
 * Gauge::NVectorHandler::Solver::first_ and the
 * Gauge::NVectorHandler::Solver::solution_.
 */
void Gauge::NVectorHandler::Solver::Reset(int minimum_total) {
  minimum_total_ = minimum_total;
  first_ = true;
  solution_ = Gauge::NVector(solution_.size);
  total_ = 0;
  size_ = 0;
}

/*!
 * To decompose the solution we loop from the next to last element to the first
 * and attempt to decompose that element to all possible orders beginning with
 * zeroth order.
 *
 * As we do the looping we keep track of the running size and the total this we
 * we don't have to recalculate it each time nor do we have to do the
 * calculation in Gauge::NVectorHandler::Solver::DecomposeElement.
 *
 * We return @c true if Gauge::NVectorHandler::Solver::DecomposeElement ever
 * returns @c true. Otherwise we return @c false.
 */
bool Gauge::NVectorHandler::Solver::Decompose() {
  int running_size = size_;
  int delta_total = 0;
  for (int i = solution_.size - 2; i >= 0; --i) {
    running_size -= solution_.base[i+1];
    delta_total += solution_.base[i+1] * coefficients_[i+1];
    for (int j = 0; j < solution_.base[i]; ++j) {
      if (DecomposeElement(i,j,running_size, delta_total)) {
        return true;
      }
    }
  }
  return false;
}

/*!
 * We begin my adjusting the running_size and delta_total provided because the
 * values provided don't take into account the order of the current
 * decomposition. This cannot really be handled nicely in
 * Gauge::NVectorHandler::Solver::Decompose.
 *
 * After making the adjustments we maximize the end-segment beginning with the
 * element following the current with the target total as delta_total and
 * maximum size as @c maximum_size_-running_size
 *
 * If we can maximize the end-segment we decrement the current element by the
 * order (plus one), set the Gauge::NVectorHandler::Solver::size_ to the new
 * value and return @c true.
 *
 * It we cannot maximize the end-segment we return @c false.
 *
 * @see Gauge::NVectorHandler::Solver::MaximizeSegment
 */
bool Gauge::NVectorHandler::Solver::DecomposeElement(int index, int order,
    int running_size, int delta_total) {
  running_size -= (1+order);
  delta_total += (1+order) * coefficients_[index];
  int size = MaximizeSegment(solution_, index + 1, delta_total,
      maximum_size_-running_size);

  if (size != -1) {
    solution_.base[index] -= (1+order);
    size_ = running_size + size;
    return true;
  }
  return false;
}

/*!
 * We start off by setting the Gauge::NVectorHandler::Solver::first_ flag to
 * @c false and attempting to maximize the entire solution with the
 * Gauge::NVectorHandler::Solver::minimum_total_.
 *
 * In the case that this succeeds we set
 * Gauge::NVectorHandler::Solver::total_ and
 * Gauge::NVectorHandler::Solver::size_ to their appropriate values and return
 * @c true.
 *
 * Otherwise we return @c false.
 */
bool Gauge::NVectorHandler::Solver::FirstSolution() {
  //first_ = false;
  int size = MaximizeSegment(solution_, 0, minimum_total_, maximum_size_);
  if (size != -1) {
    first_ = false;
    total_ = minimum_total_;
    size_ = size;
    return true;
  }
  return false;
}

/*!
 * We start by creating a few dummy variables to hold the
 * Gauge::NVectorHandler::Solver::total_ incremented by the
 * Gauge::NVectorHandler::Solver::modulus_ an one to hold the maximum_total that
 * an nvector can represent.
 *
 * We then loop, incrementing our dummy total by the modulus each time,
 * attempting to maximize the entire solution to the dummy total.
 *
 * If any maximization succeeds, we set the internal
 * Gauge::NVectorHandler::Solver:total_ to dummy total, set the
 * Gauge::NVectorHandler::Solver::size_ to the size returned by
 * Gauge::NVectorHandler::Solver::MaximizeSegment and return @c true.
 *
 * If we never succeed at maximization, we return @c false.
 */
bool Gauge::NVectorHandler::Solver::Maximize() {
  int total = modulus_;
  if (first_) {
    total += minimum_total_;
    first_ = false;
  } else {
    total += total_;
  }
  int maximum_total = modulus_ *
    static_cast<int>(floor(22 * coefficients_[0] / modulus_));
  int size;
  while (total <= maximum_total) {
    size = MaximizeSegment(solution_, 0, total, maximum_size_);
    if (size != -1) {
      total_ = total;
      size_ = size;
      return true;
    }
    total += modulus_;
  }
  return false;
}

int Gauge::NVectorHandler::Solver::MaximizeSegment(Gauge::NVector &nvector,
    int index, int target_total, int maximum_size) {
  /*! We start by bounds checking our index. */
  if (index >= nvector.size) return -1;

  /*!
   * Next we store the current element's value so we can reset it if fail to
   * maximize the segment.
   */
  int previous_value = nvector.base[index];

  /*!
   * We now calculate the maximum value that the current element can hold.
   * If it will cause us to exceed the maximum size, we exit. Otherwise we set
   * the element's value.
   */
  int delta = target_total/coefficients_[index];
  if (maximum_size < delta) return -1;
  nvector.base[index] = delta;

  /*!
   * In the case that this is the last element of the solution, we return its
   * value.
   */
  if (index == nvector.size - 1) return nvector.base[index];

  /*!
   * We now loop through attempting to maximize the end-segment that follows the
   * current element. If we can't we decrement the current element's value and
   * try again with the adjusted total.
   *
   * If we succeed at maximizing the subsequent end-segment we add the current
   * element's value to the returned size and return that value.
   */
  int next_total = target_total - (nvector.base[index] * coefficients_[index]);
  int next_size = maximum_size - nvector.base[index];
  int size = 0;
  if (index < nvector.size - 1) {
    while (nvector.base[index] > 0) {
      size = MaximizeSegment(nvector, index + 1, next_total, next_size);
      if (size != -1) return size + nvector.base[index];
      --nvector.base[index];
      next_total += coefficients_[index];
      ++next_size;
    }
  }

  /*!
   * In the case that we have yet to succeed at maximizing the end-segment we
   * attempt once more with the current element set to zero. In the case of
   * success we return the new size otherwise we undo our hard work and return
   * @c -1.
   */
  size = MaximizeSegment(nvector,index+1, target_total, maximum_size);

  if (size == -1) nvector.base[index] = previous_value;

  return size;
}

/*!
 * DistributeSolution distributes the values of the current solution over
 * all possible distributions of values and tests that the distrubution
 * satisfies the constraints.
 *
 * If a distribution is found that satisfies the constraints, the process halts
 * and returns true. Otherwise it continues until all possibilities have been
 * exhausted and then returns false.
 */
bool Gauge::NVectorHandler::DistributeSolution() {
  if (layer_ == 1) return false;
  int number_of_segments = orders_[current_]/2;
  for (int segment = number_of_segments - 1; segment >= 0; --segment) {
    if (DistributeInSegment(current_, segment)) {
      return true;
    }
  }
  return false;
}

bool Gauge::NVectorHandler::DistributeInSegment(int equation, int segment) {
  int order = orders_[equation];
  bool has_partner = !((order & 1) == 0 && segment == 0);
  int multiplicity = multiplicity_[equation];
  int size = (has_partner) ? 2 * multiplicity : multiplicity;

  int left = (equation == 0) ? 0 : barriers_[equation - 1];
  int start = left + segment * multiplicity;
  int midpoint = left + (order/2) * multiplicity;
  int start2 = midpoint + segment * multiplicity;
  if ((order & 1) == 0) start2 -= multiplicity;

  int *container = new int[size];
  for (int i = 0; i < multiplicity; ++i) {
    container[i] = solution_->base[start + i];
    if (has_partner) container[multiplicity + i] = solution_->base[start2 + i];
  }

  while (DistributeArray(container, size)) {
    for (int i = 0; i < multiplicity; ++i) {
      solution_->base[start + i] = container[i];
      if (has_partner) solution_->base[start2 + i] = container[multiplicity + i];
    }
    if (!IsValid(equation)) continue;
    for (int i = 0; i < avalue_; ++i) {
      if (conjugates_[i] > i &&
          solution_->base[i] < solution_->base[conjugates_[i]]) {
        goto step;
      }
    }
    delete [] container;
    return true;
    step:;
  }
  for (int i = 0; i < multiplicity; ++i) {
    solution_->base[start + i] = container[i];
    if (has_partner) solution_->base[start2 + i] = container[multiplicity + i];
  }
  delete [] container;
  return false;
}

bool Gauge::NVectorHandler::DistributeArray(int *array, int size) {
  for (int i = size - 1; i >= 0; --i) {
    if (array[i] != 0 && i == size - 1) {
      int j = i - 1;
      while (j >= 0) {
        if (array[j] != 0) {
          array[j + 1] = array[i] + 1;
          if (j != i - 1) array[i] = 0;
          --array[j];
          break;
        }
        --j;
      }
      if (j >= 0) return true;
      for (int i = 1; i < size; ++i) {
        array[0] += array[i];
        array[i] = 0;
      }
      return false;
    } else if (array[i] != 0) {
      --array[i];
      ++array[i + 1];
      return true;
    }
  }
  return false;
}

bool Gauge::NVectorHandler::IsValid(int equation) {
  for (int i = equation + 1; i < layer_; ++i) {
    int index = equation + i + layer_ - 1;
    int value = 0;
    for (int j = 0; j < barriers_[equation]; ++j) {
      value += solution_->base[j] * amatrix_[equation][j] * amatrix_[i][j];
    }
    if (value % moduli_[index] != 0) return false;
  }
  return true;
}

void Gauge::NVectorHandler::FillSolution() {
  int factor = multiplicity_[current_];
  for (int i = 0; i < holders_[current_]->size; ++i) {
    if (current_ == 0) {
      solution_->base[factor * i] = holders_[current_]->base[i];
    } else {
      solution_->base[factor * i + barriers_[current_ - 1]] =
        holders_[current_]->base[i];
    }
  }
}

bool Gauge::NVectorHandler::FindNextSolution(bool distribute) {
  if (distribute) {
    if (DistributeSolution()) {
      if (current_ == layer_ - 1) {
        return true;
      } else {
        ++current_;
        SetupEquation();
      }
    }
  }

  if (NextLayerOneSolution()) {
    FillSolution();
    if (IsValid(current_) && current_ < (layer_ - 1)) {
      ++current_;
      SetupEquation();
      return FindNextSolution(false);
    } else if (current_ < layer_ - 1) {
      if (DistributeSolution()) {
        ++current_;
        SetupEquation();
        return FindNextSolution(false);
      } else if (solvers_[current_].Sum() == 0) {
        return FindNextSolution(false);
      }
      --current_;
      return FindNextSolution(true);
    }
    return true;
  } else if (current_ > 0) {
    --current_;
    return FindNextSolution(true);
  }
  return false;
}

bool Gauge::NVectorHandler::NextLayerOneSolution() {
  if (holders_[current_] == NULL) {
    holders_[current_] = &solvers_[current_].Solution();
  }
  return solvers_[current_].NextSolution();
}

void Gauge::NVectorHandler::SetupAMatrix() {
  if (barriers_ != NULL) delete [] barriers_;
  if (amatrix_ != NULL) {
    for (int index = 0; index < layer_; ++index) {
      if (amatrix_[index] != NULL) delete [] amatrix_[index];
    }
    delete [] amatrix_;
  }
  /*!
   * We begin by allocating the required memory to store the barriers and
   * the amatrix.
   */
  barriers_ = new int[layer_];
  amatrix_ = new int*[layer_];
  for (int i = 0; i < layer_; ++i) {
    amatrix_[i] = new int[avalue_];
    for (int j = 0; j < avalue_; ++j) amatrix_[i][j] = 0;
  }
  int multiplicity, replication, *coefficients, length, index;
  /*! Then, for each row we: */
  for (int i = 0; i < layer_; ++i) {
    /*! - Get the multiplicity and replication for the current equation. */
    multiplicity = multiplicity_[i];
    replication = replication_[i];
    /*! - Calculate the coefficients for that row. */
    coefficients = NULL;
    Coefficients(i, &coefficients, &length);
    index = 0;
    /*!
     * - Then fill the a-matrix up to to the next-to-last replication.
     *   (The last replication will be short by one coefficient).
     */
    for (int rep = 0; rep < replication - 1; ++rep) {
      for (int *coeff = coefficients; coeff != coefficients+length; ++coeff) {
        for (int mult = 0; mult < multiplicity; ++mult) {
          amatrix_[i][index] = *coeff;
          ++index;
        }
      }
    }
    /*! - Fill the amatrix up to the next-to-last coefficient. */
    for (int *coeff = coefficients; coeff != coefficients+length-1; ++coeff) {
      for (int mult = 0; mult < multiplicity; ++mult) {
        amatrix_[i][index] = *coeff;
        ++index;
      }
    }
    /*! - Set the barrier for that row to the current index. */
    barriers_[i] = index;
    /*!
     * - Finally fill in the rest of the row. This coefficient will be
     *   @f$ 0 @f$, thus the setting of the barrier in the last step.
     */
    for (int mult = 0; mult < multiplicity - 1; ++mult) {
      amatrix_[i][index] = *(coefficients + length - 1);
      ++index;
    }
    /*!
     * - Make sure we release the memory that was allocate for the
     *   coefficients before we repeat this process for the next row.
     */
    if (coefficients != NULL) delete [] coefficients;
  }
}

/*!
 * In order to determine the conjugate we:
 *   -# Allocate the memory to hold the mapping.
 *   -# Initialize the array as the identity
 *      @c (Gauge::NVectorHandler::conjugates_[N] = N).
 *   -# Allocate the memory to hold an array representing the sum of columns
 *   -# Sum each of the columns of the a-matrix and constraints matrix and place
 *      them in the sum array.
 *   -# A pair of columns are determined to be conjugates if, and only if,
 *      for every row,
 *     -# The values of the a-matrix in those columns are equal, up to a sign
 *     -# The coefficients in those columns of the mixed constraints are equal
 *
 * We only check two columns for equality if they have the same sum. This
 * reduces the complexity of the algorithm from @f$ O(N^3) @f$ to
 * @f$ O(N^2) @f$.
 */
void Gauge::NVectorHandler::SetupConjugates() {
  if (conjugates_ != NULL) delete [] conjugates_;
  int height = static_cast<int>(layer_ * (layer_ - 1) / 2);
  conjugates_ = new int[avalue_];
  for (int index = 0; index < avalue_; ++index) conjugates_[index] = index;
  int *sum = new int[avalue_];
  for (int column = 0; column < avalue_; ++column) {
    sum[column] = 2;
    for (int row = 0; row < layer_; ++row) {
      sum[column] += abs(amatrix_[row][column]);
    }
    for (int row = 0; row < height; ++row) {
      sum[column] += constraints_[row][column];
    }
  }
  for (int column_1 = 0; column_1 < avalue_ - 1; ++column_1) {
    if (conjugates_[column_1] != column_1) continue;
    for (int column_2 = column_1 + 1; column_2 < avalue_; ++column_2) {
      if (conjugates_[column_2] != column_2) continue;
      if (sum[column_1] == sum[column_2]) {
        int row = 0;
        for (; row < layer_; ++row) {
          if (abs(amatrix_[row][column_1]) != abs(amatrix_[row][column_2])) {
            break;
          }
        }
        if (row == layer_) {
          row = 0;
          for (; row < height; ++row) {
            if (constraints_[row][column_1] != constraints_[row][column_2]) {
              break;
            }
          }
          if (row == height) {
            conjugates_[column_1] = column_2;
            conjugates_[column_2] = column_1;
            break;
          }
        }
      }
    }
  }
  delete [] sum;
}

/*!
 * To determine the moduli we:
 *   - Determine the number of constraints: @f$ \frac{L(L+1)}{2} @f$.
 *   - Allocate enough memory to store the moduli.
 *   - Then, for each row we
 *     - Calculate the modulus
 *     - Calculate the modulus of the mixed constraints.
 *     - Initialize the coefficients of the mixed constraints.
 */
void Gauge::NVectorHandler::SetupConstraints() {
  if (constraints_ != NULL) delete [] constraints_;
  if (moduli_ != NULL) delete [] moduli_;
  int number_of_constraints = static_cast<int>(layer_ * (layer_ + 1) / 2);
  if (number_of_constraints > layer_) {
    constraints_ = new int*[number_of_constraints - layer_];
    for(int row = 0; row < number_of_constraints - layer_; ++row) {
      constraints_[row] = new int[avalue_];
    }
  } else {
    constraints_ = NULL;
  }

  moduli_ = new int[number_of_constraints];
  for (int mom = 0; mom < layer_; ++ mom) {
    moduli_[mom] = (2 - (orders_[mom] & 1)) * orders_[mom];
    for (int dad = mom + 1; dad < layer_; ++dad) {
      int index = mom + dad + layer_ - 1;
      moduli_[index] = Gauge::Math::GCD(orders_[mom],orders_[dad]);
      for (int column = 0; column < avalue_; ++column) {
        constraints_[index - layer_][column] =
          amatrix_[mom][column] * amatrix_[dad][column];
      }
    }
  }
}

void Gauge::NVectorHandler::SetupEquation() {
  /*!
   * We first determine what the maximum size and minimum total for the equation
   * in question.
   */
  int left_barrier = (current_ == 0) ? 0 : barriers_[current_ - 1];
  int maximum_size = size_;
  int minimum_total = 0;
  int coeff;
  for (int i = 0; i < left_barrier; ++i) {
    maximum_size -= solution_->base[i];
    if (amatrix_[current_][i] != 0) {
      coeff = amatrix_[current_][i] * amatrix_[current_][i];
      minimum_total += coeff * solution_->base[i];
    }
  }

  int order = orders_[current_];
  int modulus = moduli_[current_];
  int minimum_modulus =
    modulus * static_cast<int>(ceil(1.0 * minimum_total / modulus));

  /*!
   * Then, based on the previously initialized values, we setup the
   * Gauge::NVectorHandler::Solver in question.
   */
  if (minimum_total == 0) {
    solvers_[current_].Setup(order, maximum_size, modulus);
  } else if (minimum_total % modulus == 0) {
    solvers_[current_].Setup(order, maximum_size, 0);
  } else {
    solvers_[current_].Setup(order, maximum_size,
        minimum_modulus - minimum_total);
  }
  if (holders_[current_] != NULL) holders_[current_] = NULL;
}

/*!
 * This method simply calculates the multiplicites and replications for each
 * layer.
 */
void Gauge::NVectorHandler::SetupMultiplicities() {
  if (multiplicity_ != NULL) delete [] multiplicity_;
  if (replication_  != NULL) delete [] replication_;
  multiplicity_ = new int[layer_];
  replication_  = new int[layer_];

  for (int l = 0; l < layer_; ++l) {
    multiplicity_[l] = Gauge::Math::Product(orders_+l+1, orders_+layer_, 1);
    replication_[l]  = Gauge::Math::Product(orders_, orders_+l, 1);
  }
}

/*!
 * We allocate the memory for the Gauge::NVectorHandler::holders_ and
 * Gauge::NVectorHandler::solvers_, one for each layer.
 *
 * We then initialize each of the Gauge::NVectorHandler::holders_ to
 * a pointer to the internal solution state of the corresponding
 * Gauge::NVectorHandler::Solver.
 */
void Gauge::NVectorHandler::SetupSolvers() {
  if (holders_ != NULL) delete [] holders_;
  if (solvers_  != NULL) delete [] solvers_;
  holders_ = new const Gauge::NVector*[layer_];
  solvers_ = new Gauge::NVectorHandler::Solver[layer_];
  for (int l = 0; l < layer_; ++l) {
    holders_[l] = NULL;
  }
}

void Gauge::NVectorHandler::Coefficients(int layer, int **coeff, int *length) {
  if (*coeff != NULL) delete [] *coeff;
  *length = orders_[layer];
  *coeff = new int[*length];
  int sign = 1;
  int max = *length/2;
  int index = 0;
  for (int j = max; j >= 0; --j) {
    (*coeff)[index] = sign * j;
    ++index;
    if (j == 1 && sign == 1) {
      j = (orders_[layer] % 2 == 0) ? max : max + 1;
      sign = -1;
    }
  }
}
