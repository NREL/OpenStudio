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

#ifndef ANALYSIS_PARAMETERSTUDYALGORITHM_IMPL_HPP
#define ANALYSIS_PARAMETERSTUDYALGORITHM_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "DakotaAlgorithm_Impl.hpp"

namespace openstudio {
namespace analysis {

class ParameterStudyAlgorithm;
class ParameterStudyAlgorithmOptions;

namespace detail {

  /** ParameterStudyAlgorithm_Impl is a DakotaAlgorithm_Impl that is the implementation class for ParameterStudyAlgorithm.*/

  class ANALYSIS_API ParameterStudyAlgorithm_Impl : public DakotaAlgorithm_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    // TODO: Add other items necessary for construction.
    ParameterStudyAlgorithm_Impl(const ParameterStudyAlgorithmOptions& options);

    // TODO: Add other items necessary for construction.
    ParameterStudyAlgorithm_Impl(const UUID& uuid,
                                 const UUID& versionUUID,
                                 const std::string& displayName,
                                 const std::string& description,
                                 bool complete,
                                 bool failed,
                                 int iter,
                                 const ParameterStudyAlgorithmOptions& options,
                                 const boost::optional<FileReference>& restartFileReference,
                                 const boost::optional<FileReference>& outFileReference,
                                 const boost::optional<openstudio::runmanager::Job>& job);

    ParameterStudyAlgorithm_Impl(const ParameterStudyAlgorithm_Impl& other);

    virtual ~ParameterStudyAlgorithm_Impl() {}

    virtual AnalysisObject clone() const;

    //@}

    /** @name Virtual Methods */
    //@{

    /** Returns true if Algorithm can operate on problem. For example, DesignOfExperiments can work
     *  on all \link Problem Problems \endlink, but SequentialSearch requires an OptimizationProblem
     *  with two objective functions and \link DiscreteVariable DiscreteVariables\endlink. */
    virtual bool isCompatibleProblemType(const Problem& problem) const;

    /** Returns true if the algorithm requires continuous variables. The Problem class uses this
     *  method to determine whether to represent OpenStudio DiscreteVariables as continuous
     *  variables when communicating with DAKOTA. If the desired behavior is to fail, rather than
     *  to fake continuity, then isCompatibleProblemType should return false if Problem contains
     *  non-trivial (> 1 perturbation) discrete variables. */
    virtual bool requiresContinuousVariables() const;

    virtual std::string dakotaInFileDescription() const;

    //@}
    /** @name Getters and Queries */
    //@{

    ParameterStudyAlgorithmOptions parameterStudyAlgorithmOptions() const;

    //@}
    /** @name Absent or Protected in Public Class */
    //@{

    virtual QVariant toVariant() const;

    static ParameterStudyAlgorithm fromVariant(const QVariant& variant, const VersionString& version);

    //@}
   private:
    REGISTER_LOGGER("openstudio.analysis.ParameterStudyAlgorithm");
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_PARAMETERSTUDYALGORITHM_IMPL_HPP

