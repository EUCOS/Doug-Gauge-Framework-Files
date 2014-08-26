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
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * The Gauge Framework. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * @file NVectorHandler.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 02.26.2012
 *
 * @brief The Gauge::NVectorHandler is an object for systematically generating
 * NVectors.
 *
 * The idea of this class is that, using NVectors, we can reduce some of the
 * redundancy in the process of generating basis vectors. The
 * Gauge::NVectorHandler simply consolidates the logic used for generating such
 * vectors of integers.
 */

#ifndef GAUGE_FRAMEWORK_NVECTORHANDLER_H
#define GAUGE_FRAMEWORK_NVECTORHANDLER_H

#include <Datatypes/NVector.h>

namespace Gauge {
  /*!
   * @brief The Gauge::NVectorHandler systematically constructs NVectors that
   * guaruntee modular invariant basis vector sets.
   */
  class NVectorHandler {
    public:
      // Constructors
      /*!
       * The default constructor initializes ensures that the internal variables
       * are all initialized to the expected values based on their types.
       */
      NVectorHandler();
      // Destructor
      /*!
       * We have a nontrivial destructor because we utilize multiple dynamically
       * allocated arrays.
       */
      ~NVectorHandler();
      // Interface
      /*!
       * This method is a const accessor for the current solution.
       *
       * @return A const pointer to the solution stored internally.
       */
      const Gauge::NVector *CurrentSolution() const { return solution_; }
      /*!
       * NextSolution generates the next nvector that satisfies all of the
       * modular invariance constraints.
       *
       * If this method is called before any calls to
       * Gauge::NVectorHandler::Setup, the program will terminate via
       * a C assertion.
       *
       * @return A boolean flag specifying whether a new solution was found.
       */
      bool NextSolution();
      /*!
       * Setup does the nontrivial initialization and setup required to
       * correctly generate models.
       *
       * If this method is not called before any calls to
       * Gauge::NVectorHandler::NextSolution the program will terminate via
       * a C assertion.
       *
       * @param[in] orders An array of order.
       * @param[in] layers The length of the orders array, that is the layer.
       */
      void Setup(const int *orders, int layers, int size);
      /*!
       * This accessor provides constant access to the underlying A matrix
       * representing the un-squared and un-mixed modular invariance
       * constraints.
       *
       * @return A constant double pointer to the underlying
       * Gauge::NVector::amatrix_.
       */
      const int **amatrix() const { return const_cast<const int**>(amatrix_); }
      /*!
       * This accessor provides constant access to the conjugacy mapping.
       *
       * @return A constant pointer to the underlying
       * Gauge::NVector::conjugates_.
       */
      const int *conjugates() const {
        return const_cast<const int*>(conjugates_);
      }
      /*!
       * This accessor provideds an constant reference to the width of the
       * Gauge::NVectorHandler::amatrix_ and Gauge::NVectorHandler::conjugates_.
       *
       * @return A constant reference to the Gauge::NVectorHandler::avalue_.
       */
      const int& avalue() const { return avalue_; }
    private:
      /*!
       * @brief The Gauge::NVectorHandler::Solver class is designed to
       * encapsulate the algorithm for solving the layer 1 modular invariance
       * constraints.
       */
      class Solver {
        public:
          /*!
           * The default constructor simply initializes the coefficients
           * array to @c NULL.
           */
          Solver();
          /*!
           * The coefficients array is dynamcially allocated so we must manually
           * free that memory.
           */
          ~Solver();
          /*!
           * This method simply returns a const reference to the current
           * solution.
           */
          const Gauge::NVector& Solution() const { return solution_; }
          /*!
           * This method is a working method designed to construct the next
           * solution to the current, layer 1, modular invariance equation.
           *
           * If this method is called before
           * Gauge::NVectorHandler::Solver::Setup, then the program will
           * terminate via C assertion.
           */
          bool NextSolution();
          /*!
           * This method resets all of the internal variables back to their
           * initial states. This is useful during the complete process of
           * solving the multi-layer modular invariance constraints.
           *
           * @param[in] minimum_total The minimum allowed dot-product that the
           * n-vector and the coefficients array can have. This will change
           * based on the state of the other layer 1 solvers, thus we need to
           * take it as an input.
           */
          void Reset(int minimum_total);
          /*!
           * Setup runs the nontrivial initialization and setup requried to
           * correctly find solutions.
           *
           * If this method is not called before any calls to
           * Gauge::NVectorHandler::Solver::NextSolution, the program will
           * terminate via a C assertion.
           *
           * @param[in] order The order of the modular invariance to be solved.
           * @param[in] maximum_size The maximum value the elements of the
           * n-vector solution should sum to.
           * @param[in] minimum_total The smallest allowed dot-product between
           * the n-vector solution and the coefficients array.
           */
          void Setup(int order, int maximim_size, int minimum_total);
          /*!
           * Sum simply returns the sum of the values of the current solution.
           * This value is computed and stored dynamically in the
           * Gauge::NVectorHandler::Solver::size_ value.
           *
           * @return A constant reference to the sum of the values of the
           * current solution.
           */
          const int& Sum() const { return size_; }

        private:
          /*! The copy constructor is private to prevent copying. */
          Solver(const Gauge::NVectorHandler::Solver& other) {}

          /*!
           * The assignment operator, as with the copy constructor, is private.
           */
          Solver &operator=(const Gauge::NVectorHandler::Solver& other) {
            return *this;
          }

          int *coefficients_;       /*!< A dynamically allocated array of
                                      coefficients. */
          bool first_;              /*!< A boolean flag specifying whether it is
                                      the first time the
                                      Gauge::NVectorHandler::Solver has been
                                      run. */
          int maximum_size_;        /*!< The maximum allowed sum the values of
                                      of the current solution. */
          int minimum_total_;       /*!< The smallest dot-product of the current
                                      solution with the coefficients array. */
          int modulus_;             /*!< The modulus of the modular invariance
                                      constraint in question. */
          bool setup_;              /*!< A boolean flag specifying whether the
                                      Gauge::NVectorHandler::Solver::Setup
                                      method has been called. */
          Gauge::NVector solution_; /*!< The current solution. */
          int size_;                /*!< The current sum of the values of the
                                      current solution. */
          int total_;               /*!< The current dot-product between the
                                      current solution and the coefficients
                                      array. */
          /*!
           * This method takes the current solution and attempts to find the
           * next decomposition. This amounts to determining if any element can
           * be decomposed.
           *
           * A call to this method should never change
           * Gauge::NVectorHandler::Solver::total_. If it does, an exception
           * will be thrown by C assertion. However, it will always result in
           * a change in Gauge::NVectorHandler::Solver::size_, provided it
           * succeeds.
           *
           * @see Gauge::NVectorHandler::Solver::DecomposeElement
           * @see Gauge::NVectorHandler::Solver::NextSolution
           * @see Gauge::NVectorHandler::Solver::size_
           * @see Gauge::NVectorHandler::Solver::total_
           *
           * @return A boolean flag specifying whether any element could be
           * decomposed.
           */
          bool Decompose();
          /*!
           * This method attempts to decompose the element at the provided
           * location.
           *
           * Sometimes elements require "higher-order" decompositions,
           * that is they need to be decremented more than once to find
           * a subsequent solution; hence, we take the order as an argument.
           *
           * Additionally, rather than computing the size of the segment
           * preceeding and the total following the element in question, we
           * require those as arguments. This improves performance.
           *
           * @see Gauge::NVectorHandler::Solver::Decompose
           *
           * @param[in] index The index of the element to decompose.
           * @param[in] order The order to which we would like to decompose this
           * element.
           * @param[in] running_size The size of the segment of the nvector that
           * preceeds the element in question.
           * @param[in] delta_total The total of the segment following the
           * element in question.
           *
           * @return A boolean flag specifying whether a decomposition of the
           * provided order could be found.
           */
          bool DecomposeElement(int index, int order, int running_size,
              int delta_total);
          /*!
           * The logic required the first time
           * Gauge::NVectorHandler::Solver::NextSolution is run is slightly
           * different from subsequent runs. Hence, we encapsulate that logic in
           * this method.
           *
           * @see Gauge::NVectorHandler::Solver::NextSolution
           * @see Gauge::NVectorHandler::Solver::first_
           *
           * @return A boolean flag specifying whether the solution we typically
           * accept as the first is acutally a solution. (This is not
           * guarunteed, but does usually occur.)
           */
          bool FirstSolution();
          /*!
           * This method takes the current solution and attempts to find the
           * the next maximal solution. That is, it increases the
           * Gauge::NVectorHandler::Solver::total_ by a multiple of
           * Gauge::NVectorHandler::Solver::modulus_ and attempts to find
           * a valid solution.
           *
           * If this method succeeds, Gauge::NVectorHandler::Solver::total_ will
           * change and, more often than not,
           * Gauge::NVectorHandler::Solver::size_ will too. If
           * Gauge::NVectorHandler::Solver::total_ does not change, execution is
           * terminated via C assertion.
           *
           * @see Gauge::NVectorHandler::Solver::MaximizeSegment
           * @see Gauge::NVectorHandler::Solver::NextSolution
           * @see Gauge::NVectorHandler::Solver::size_
           * @see Gauge::NVectorHandler::Solver::total_
           *
           * @return A boolean flag specifying whether a maximal solution could
           * be found.
           */
          bool Maximize();
          /*!
           * This method attempts to find the maximal value for the end-segment
           * of the provided Gauge::NVector starting with the element at the
           * provided index with the total and maximum size provided.
           *
           * Note: This is the single most called method in the
           * Gauge::NVectorHandler::Solver class and, as such, is the best
           * target for optimiztion.
           *
           * @see Gauge::NVectorHandler::Solver::Maximize
           *
           * @param[in] nvector The Gauge::NVector in question.
           * @param[in] index The index to the first element of the segment to
           * maximize.
           * @param[in] target_total The total that the segment should have.
           * @param[in] maximum_size The maximum size that the segment can have
           * and still be valid.
           *
           * @return An integer representing the size of the segment. If the
           * value is @f$ -1 @f$ then a valid maximal segment could not be
           * found.
           */
          int MaximizeSegment(Gauge::NVector &nvector, int index,
              int target_total, int maximum_size);
      };
      /*!
       * The Gauge::NVectorHandler copy constructor is both trivial and private.
       * This is intentional. It prevents excessive copying. If, in the future,
       * it becomes necessary to use the copy constructor, make it public.
       */
      NVectorHandler(const NVectorHandler &other) {}
      /*!
       * The assignment operator is intentionally make private to prevent its
       * use. This ensures that memory copying is made explicit.
       */
      NVectorHandler &operator=(const NVectorHandler &other) { return *this; }

      int **amatrix_;           /*!< A dynamically allocated, 2D array
                                  representing the unsquared coefficients of the
                                  modular invariance constraints. */
      int avalue_;              /*!< An integer representation of the avalue,
                                  that is @f$ A = \prod_{i = 1}^L N_i @f$ */
      int *barriers_;           /*!< A dynamically allocated array representing
                                  the column of the matrix such it, and all
                                  following columns, are @f$ 0 @f$. The index is
                                  the row of the amatrix_ in question. */
      int *conjugates_;         /*!< A dynamically allocated array specifying
                                  which columns of the a-matrix are conjugates
                                  (i.e.  the coefficients of the modular
                                  invariance constraints specified by those
                                  columns are the same). */
      int **constraints_;       /*!< A dynamically allocated, 2D array
                                  representing the coefficients of the mixed
                                  modular invariance constraints. */
      int current_;             /*!< An integer representing the current layer
                                  we are solving. */
      const NVector **holders_; /*!< A dynamically allocated array of
                                  Gauge::NVector pointers. These pointers point
                                  the the internal solutions in the
                                  corresponding Gauge::NVectorHandler::Solver.*/
      int layer_;               /*!< An integer representation of the layer of
                                  the model to be build. */
      int *moduli_;             /*!< A dynamically allocated array representing
                                  the moduli of the modular invariance
                                  constraint equations. */
      int *multiplicity_;       /*!< A dynamically allocated array containing
                                  the multiplicities for each layer. */
      int *orders_;             /*!< A dynamically allocated array representing
                                  the order of the models build. (Provided at
                                  Setup) */
      int *replication_;        /*!< A dynamically allocated array representing
                                  the replication for each layer. */
      bool setup_;              /*!< A boolean flag specifying whether the
                                  Gauge::NVectorHandler is setup. */
      int size_;                /*!< An integer representation of the size
                                  Gauge::BasisVector vectors that will be
                                  constructed with the generated
                                  Gauge::NVectors. Specifically,
                                  @f$ N = 26 - D @f$. */
      NVector *solution_;       /*!< A dynamically allocated pointer to the
                                  current solution state. */
      Solver *solvers_;         /*!< A dynamically allocated array of
                                  Gauge::NVectorHandler::Solver, one for each
                                  layer. */


      /*!
       * This method does takes care of distributing the current solution
       * appropriately.
       *
       * @see Gauge::NVectorHandler::DistributeInSegment
       * @see Gauge::NVectorHandler::IsValid
       *
       * @return A boolean flag signifying whether a distributed solution was
       * found (@c true) or not.
       */
      bool DistributeSolution();
      /*!
       * This method takes care of distributing the solution within segments;
       * the layer-one solutions can be delt with seperately.
       *
       * @see Gauge::NVectorHandler::DistributeSolution
       * @see Gauge::NVectorHandler::DistributeArray
       *
       * @param[in] equation The index of the constraint whose segment is in
       * question.
       * @param[in] segment The segment we are distributing within.
       *
       * @return A boolean flag specifying whether a valid distribution was
       * found (@c true) or not.
       */
      bool DistributeInSegment(int equation, int segment);
      /*!
       * This method takes care of the actual distribution of values in an
       * array.
       *
       * Note that the array that this method takes is NOT a solution,
       * but rather is a "compress" version of the segement begin delt with by
       * DistributeInSegment.
       *
       * @param[in] array The array whose values are being distributed.
       * @param[in] size  The size of the array provided.
       *
       * @return A boolean flag specifying whether a distribution was found (@c
       * true) or not.
       */
      bool DistributeArray(int *array, int size);
      /*!
       * Once we have solutions that satify the un-mixed modular invariance
       * constraints, this method determines if it satisfies the mixed
       * constraints.
       *
       * @param[in] equation The layer-one equation whose mixed constraints are
       * to be validated.
       *
       * @return A boolean specifying where the current solution satisfies the
       * mixed constraints in question.
       */
      bool IsValid(int equation);
      /*!
       * When a new layer one solution is found, we need to fill the current
       * solution with its values. This method takes care of that.
       */
      void FillSolution();
      /*!
       * This method does exactly what one would expect: it generates the next
       * valid NVector solution to the modular invariance constraints.
       *
       * @param[in] distribute Whether or not to distribute the solution
       *
       * @return A boolean flag signifying success (@c true) or failure.
       */
      bool FindNextSolution(bool distribute);
      /*!
       * Because of the way the algorithm was designed, we solve the layer one
       * constraints independently and this allows us to construct the
       * higher-layer constraints.
       *
       * This method solves the layer one constraint whose index is
       * Gauge::NVectorHandler::current_.
       *
       * @return A boolean flag specifying whether a layer one solution was
       * found (@c true) or not.
       */
      bool NextLayerOneSolution();
      /*!
       * SetupAMatrix initializes the Gauge::NVectorHandler::amatrix_ and
       * Gauge::NVectorHandler::barriers_ arrays to their required values
       * based on the order and layer provided to Gauge::NVectorHandler::Setup
       *
       * @see Gauge::NVectorHandler::amatrix_
       * @see Gauge::NVectorHandler::barriers_
       * @see Gauge::NVectorHandler::Setup
       */
      void SetupAMatrix();
      /*!
       * We determine which n-values are conjugate to one another. That is, we
       * which n-values correspond to equal coefficients in the modular
       * invariance constraints.
       *
       * @see Gauge::NVectorHandler::conjugates_
       * @see Gauge::NVectorHandler::Setup
       */
      void SetupConjugates();
      /*!
       * This method uses the Gauge::NVectorHandler::orders_ array to determine
       * what the moduli for the modular invariance constraint equations are as
       * well as initializing the coefficients for the mixed constriants.
       *
       * @see Gauge::NVectorHandler::constraints_
       * @see Gauge::NVectorHandler::moduli_
       * @see Gauge::NVectorHandler::orders_
       * @see Gauge::NVectorHandler::Setup
       */
      void SetupConstraints();
      /*!
       * This method does the nontrivial setup of the current solver. This
       * method should be called each time Gauge::NVectorHandler::current_ is
       * changed.
       *
       * @see Gauge::NVectroHandler::current_
       * @see Gauge::NVectorHandler::Setup
       */
      void SetupEquation();
      /*!
       * This method allocates the memory for and initializes the
       * Gauge::NVectorHandler::multiplicity_ and
       * Gauge::NVectorHandler::replication_ arrays.
       *
       * @see Gauge::NVectorHandler::multiplicity_
       * @see Gauge::NVectorHandler::replicaiton_
       * @see Gauge::NVectorHandler::Setup
       */
      void SetupMultiplicities();
      /*!
       * This method initializes the Gauge::NVectorHandler::solvers_ array.
       * Essentially, each element is a Gauge::NVectorHandler::Solver, one for
       * each layer.
       *
       * @see Gauge::NVectorHandler::solvers_
       * @see Gauge::NVectorHandler::Setup
       */
      void SetupSolvers();
      /*!
       * This method determines what the coefficients of the
       * Gauge::NVectorHandler::amatrix_ should be based on the row.
       *
       * @param[in] layer The row of the amatrix in question
       * @param[out] coeff An integer array to hold the calculated coefficients
       * @param[out] length An integer pointer to hold the length of the
       * coefficients array.
       */
      void Coefficients(int layer, int **coeff, int *length);
  };
}

#endif
