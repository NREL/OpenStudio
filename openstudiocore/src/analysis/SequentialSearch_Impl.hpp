/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
*  All rights reserved.
*
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#ifndef ANALYSIS_SEQUENTIALSEARCH_IMPL_HPP
#define ANALYSIS_SEQUENTIALSEARCH_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "OpenStudioAlgorithm_Impl.hpp"

namespace openstudio {

namespace analysis {

class SequentialSearch;
class SequentialSearchOptions;
class DataPoint;
class OptimizationDataPoint;

namespace detail {

  /** SequentialSearch_Impl is a Algorithm_Impl that is the implementation class for SequentialSearch.*/
  class ANALYSIS_API SequentialSearch_Impl : public OpenStudioAlgorithm_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    SequentialSearch_Impl(const SequentialSearchOptions& options);

    /** Constructor provided for deserialization; not for general use. */
    SequentialSearch_Impl(const UUID& uuid,
                          const UUID& versionUUID,
                          const std::string& displayName,
                          const std::string& description,
                          bool complete,
                          bool failed,
                          int iter,
                          const SequentialSearchOptions& options);

    SequentialSearch_Impl(const SequentialSearch_Impl& other);

    virtual ~SequentialSearch_Impl() {}

    virtual AnalysisObject clone() const;

    //@}
    /** @name Virtual Methods */
    //@{

    /** Returns true if Algorithm can operate on problem. For example, DesignOfExperiments can work
     *  on all \link Problem Problems \endlink, but SequentialSearch requires an OptimizationProblem
     *  with two objective functions and \link DiscreteVariable DiscreteVariables\endlink. */
    virtual bool isCompatibleProblemType(const Problem& problem) const;

    /** Create the next iteration of work for analysis in the form of constructed but incomplete
     *  \link DataPoint DataPoints \endlink. Throws openstudio::Exception if analysis.algorithm()
     *  != *this. */
    virtual int createNextIteration(Analysis& analysis);

    //@}
    /** @name Getters and Queries */
    //@{

    SequentialSearchOptions sequentialSearchOptions() const;

    //@}
    /** @name Actions */
    //@{

    /** Get the "swoosh" curve relative to objective function i, i == 0, or i == 1. Throws an
     *  openstudio::Exception if i is not equal to 0 or 1, or if analysis.algorithm() != *this. */
    std::vector<OptimizationDataPoint> getMinimumCurve(unsigned i,
                                                       Analysis& analysis) const;

    /** Get the Pareto front (set of non-dominated points). Throws an openstudio::Exception if
     *  analysis.algorithm() != *this. */
    std::vector<OptimizationDataPoint> getParetoFront(Analysis& analysis) const;

    std::vector< std::vector<QVariant> > getCandidateCombinations(const DataPoint& dataPoint) const;

    //@}
    /** @name Absent or Protected in Public Class */
    //@{

    virtual QVariant toVariant() const;

    static SequentialSearch fromVariant(const QVariant& variant, const VersionString& version);

    //@}
   private:
    REGISTER_LOGGER("openstudio.analysis.SequentialSearch");
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_SEQUENTIALSEARCH_IMPL_HPP

