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
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * The Gauge Framework. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * @file include/ModelFactory.h
 * @author D. Moore <douglas_moore1@baylor.edu>
 * @date 04.14.2012
 *
 * @brief The Gauge::ModelFactory class is defined.
 */

#ifndef GAUGE_FRAMEWORK_MODELFACTORY_H
#define GAUGE_FRAMEWORK_MODELFACTORY_H

#include <list>
#include <memory>
#include <string>
#include <vector>

#include <Datatypes/Model.h>
#include <Datatypes/Sector.h>
#include <Datatypes/StateList.h>

namespace Gauge {
  class ModelFactory {
    public:
      /*!
       * Our default constructor is also our primary constructor. It does all of
       * our basic initialization. Most importantly, it deals with setting all
       * internal pointers to @c NULL.
       */
      ModelFactory();
      /*!
       * Because we generously dynamically allocate memory, we need
       * a non-trivial destructor.
       */
      ~ModelFactory();
      /*!
       * A call to this method constructs the model specified by the
       * Gauge::Geometry provided to the Gauge::ModelFactory::Setup method. It
       * returns a boolean flag signifying that the model was constructed, @c
       * true, or that it was lower-order redundant, @c false.
       *
       * @return A boolean flag signifying success or failure.
       */
      bool Build();
      /*!
       * This method returns a string representation of the Gauge::Group.
       *
       * @return A string representation of the group.
       */
      std::string Group() const;
      /*!
       * This method returns a constant reference to the model.
       *
       * @return A reference to the model.
       */
      const Gauge::Model &Model() const { return model_; }
      /*!
       * This method prints the contents of the Gauge::ModelHandler class, that
       * is, the contents of the model constructed. This includes the
       * Gauge::Geometry, Gauge::Sector and Gauge::State instances stored
       * within.
       */
      void PrintAll() const;
      /*!
       * This method prints the contents of the Gauge::ModelFactory::geometry_.
       */
      void PrintGeometry() const;
      /*!
       * This method prints the contents of the Gauge::ModelFactory::adjoints_
       * as well as the names of the groups and the number of non-zero positive
       * roots.
       *
       * It finishes it off with the name of the entire gauge group, sans @f$
       * U(1) @f$.
       */
      void PrintGroups() const;
      /*!
       * This method prints the contents of the internal Gauge::Model instance.
       */
      void PrintModel() const;
      /*!
       * This method prints the contents of the Gauge::ModelFactory::sectors_ as
       * well as the coefficients used to generate them.
       */
      void PrintSectors() const;
      /*!
       * This method prints the contents of the Gauge::ModelFactory::states_
       * by sector along with the number of states in each list.
       */
      void PrintStates() const;
      /*!
       * The setup does all of the non-trival setup for the class. It takes
       * a Gauge::Geometry pointer and, from it, fills all of the internal
       * variables of relevance.
       *
       * @param[in] geometry A pointer to the Gauge::Geometry to build from.
       */
      void Setup(const Gauge::Geometry *geometry);

    private:
      /*!
       * This is a private working method used to compare to pointers in there
       * value that they point to rather than the their actual value. That is,
       * it compares two Gauge::State instances that are each pointed to by
       * either of the provided arguments.
       *
       * NOTE: This method was made static so it can be used in the std::sort
       * algorithm as a comparison operator.
       *
       * @param[in] alpha A pointer to the first Gauge::State instance.
       * @param[in] beta A pointer to the second Gauge::State instance.
       *
       * @return A boolean value signifying that the first instance is less
       * thatn the second.
       */
      static bool less(const Gauge::State *alpha, const Gauge::State *beta) {
        return *alpha < *beta;
      }
      /*!
       * We have made the copy constructor trival and private to prevent
       * copying.
       */
      ModelFactory(const Gauge::ModelFactory &other) {}
      /*!
       * We have made the assignment operator trival and private to prevent
       * copying.
       */
      Gauge::ModelFactory &operator=(const Gauge::ModelFactory &other) {
        return *this;
      }
      bool built_;                            /*!< A flag signifying that the
                                                model has been built whether
                                                successfully or not. */
      const int **coefficients_;              /*!< A dnnamically allocated array
                                                of integers representing the
                                                coefficients used to construct
                                                the Gauge::Sector instances. */
      int layer_;                             /*!< The number of layers. */
      Gauge::Model model_;                    /*!< The constructed model. */
      int number_of_sectors_;                 /*!< An integer representation of
                                                the number of sectors */
      int *orders_;                           /*!< A dynamically allocated array
                                                of integer representations of
                                                the orders. */
      std::vector<std::unique_ptr<Gauge::Sector>> sectors_;
                                              /*!< A dynamically allocated array
                                                of sectors. */
      bool setup_;                            /*!< A flag signifying that the
                                                factory has been setup. */
      int width_;                             /*!< The number of complex
                                                fermions. */
      /*!
       * This method does the garbage collection on the
       * Gauge::ModelFactory::coefficients_ array.
       */
      void ClearCoefficients();
      /*!
       * This method does the garbage collection on the
       * Gauge::ModelFactory::groups_ list.
       */
      void ClearGroups();
      /*!
       * This method does the garbage collection on the
       * Gauge::ModelFactory::orders_ array.
       */
      void ClearOrders();
      /*!
       * This method does the garbage collection on the
       * Gauge::ModelFactory::sectors_ array.
       */
      void ClearSectors();
      /*!
       * This method does the garbage collection on the
       * Gauge::ModelFactory::states_ array.
       */
      void ClearStates();
      /*!
       * This method constructs the coefficients that of the sectors.
       */
      void ConstructCoefficients();
      /*!
       * This method constructs the sectors of the model from the Gauge::Basis
       * provided. Recall that the sectors are simply integer linear
       * combinations of the basis vectors with the restriction that the i-th
       * coefficient be * @f$ 0 \leq m_i \leq N_i-1 @f$, with @f$ N_i @f$ the
       * i-th basis vector.
       */
      bool ConstructSectors();
      /*!
       * This method does the dirty work of actually constructing the
       * Gauge::States.
       */
      void ConstructStates();
      /*!
       * This method takes a list of Gauge::State pointers and determines what
       * gauge group the fit into.
       *
       * @param[in] states The method list of non-zero positive roots.
       *
       * @return A c-style string representing the gauge group name (in Cartan
       * notation). @c "N00" is the result if the gauge group cannot be
       * determined.
       */
      Gauge::Group::Factor *IdentifyGroup(const std::list<Gauge::State*> &states);
      /*!
       * This method takes care of the initialization of the sectors. To improve
       * efficiency we reuse as much work as possible by copying previously
       * constructed sectors.
       *
       * @param[in] index The index of the sector to be initialized.
       * @param[in] base_index The index of the sector to use as a "prototype"
       * for the new sector.
       * @param[in] layer The basis vector that will be extending the base
       * sector.
       */
      void InitializeSector(int index, int base_index, int layer);
      /*!
       * This method makes a copy of the provided Gauge::State and lowers the
       * value of new state at the provided index.
       *
       * @param[in] index The index to lower.
       * @param[in] state The state to copy.
       * @param[in] n The numerator of the magnitude of the state.
       * @param[in] d The denominator of the magnitude of the state.
       * @param[in] sector The index of the sector from which the state was
       * constructed.
       */
      void LowerState(int index, const Gauge::State *state,
                      int n, int d, int sector);
      /*!
       * This method makes a copy of the provided Gauge::State and raises the
       * value of new state at the provided index.
       *
       * @param[in] index The index to raise.
       * @param[in] state The state to copy.
       * @param[in] n The numerator of the magnitude of the state.
       * @param[in] d The denominator of the magnitude of the state.
       * @param[in] sector The index of the sector from which the state was
       * constructed.
       */
      void RaiseState(int index, const Gauge::State *state,
                      int n, int d, int sector);
      /*!
       * Provided the number of non-zero positive roots, this method determines
       * the rank of the group if it were an 'A' class group.
       *
       * @param[in] size The number of non-zero positive roots.
       *
       * @return The rank of the group or @c 0 if it cannot be an 'A' class
       * group.
       */
      int RankA(int size) const;
      /*!
       * Provided the number of non-zero positive roots, this method determines
       * the rank of the group if it were an 'D' class group.
       *
       * @param[in] size The number of non-zero positive roots.
       *
       * @return The rank of the group or @c 0 if it cannot be an 'D' class
       * group.
       */
      int RankD(int size) const;
      /*!
       * Provided the number of non-zero positive roots, this method determines
       * the rank of the group if it were an 'E' class group.
       *
       * @param[in] size The number of non-zero positive roots.
       *
       * @return The rank of the group or @c 0 if it cannot be an 'E' class
       * group.
       */
      int RankE(int size) const;
      /*!
       * This method takes care of determining the what the gauge groups are
       * from the generated Gauge::States.
       */
      void ResolveGroups();
      /*!
       * The number of non-zero positive roots in the adjoint representation is
       * degenerate, that is there is more than one group with that number of
       * non-zero positive roots, this method distils the roots down to the
       * simple roots and returns the number.
       *
       * @param[in] states The list of non-zero positive roots.
       *
       * @return The rank of the algebra.
       */
      int ResolveRank(const std::list<Gauge::State *> &states) const;
      /*!
       * This method determines whether to raise, lower to keep the provided
       * state.
       *
       * @param[in] index The index to be either raised or lowered.
       * @param[in] state The state to copy.
       * @param[in] n The numerator of the magnitude of the state.
       * @param[in] d The denominator of the magnitude of the state.
       * @param[in] sector The index of the sector from which the state was
       * constructed.
       */
      void SelectF(int index, Gauge::State *state, int n, int d, int sector);
      /*!
       * This method sets the number of supersymmetries.
       */
      void SetSUSY();
  };
}

#endif
