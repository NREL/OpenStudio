/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef ANALYSIS_FSUDACEALGORITHM_IMPL_HPP
#define ANALYSIS_FSUDACEALGORITHM_IMPL_HPP

#include <analysis/AnalysisAPI.hpp>
#include <analysis/DakotaAlgorithm_Impl.hpp>

namespace openstudio {
namespace analysis {

class FSUDaceAlgorithm;
class FSUDaceAlgorithmOptions;

namespace detail {

  /** FSUDaceAlgorithm_Impl is a DakotaAlgorithm_Impl that is the implementation class for FSUDaceAlgorithm.*/
  class ANALYSIS_API FSUDaceAlgorithm_Impl : public DakotaAlgorithm_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    FSUDaceAlgorithm_Impl(const FSUDaceAlgorithmOptions& options);

    FSUDaceAlgorithm_Impl(const UUID& uuid,
                          const UUID& versionUUID,
                          const std::string& displayName,
                          const std::string& description,
                          bool complete,
                          bool failed,
                          int iter,
                          const FSUDaceAlgorithmOptions& options,
                          const boost::optional<FileReference>& restartFileReference,
                          const boost::optional<FileReference>& outFileReference,
                          const boost::optional<openstudio::runmanager::Job>& job);

    /** Constructor provided for deserialization; not for general use. */
    FSUDaceAlgorithm_Impl(const FSUDaceAlgorithm_Impl& other);

    virtual ~FSUDaceAlgorithm_Impl() {}

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

    FSUDaceAlgorithmOptions fsudaceAlgorithmOptions() const;

    //@}
    /** @name Absent or Protected in Public Class */
    //@{

    virtual QVariant toVariant() const;

    static FSUDaceAlgorithm fromVariant(const QVariant& variant, const VersionString& version);

    //@}
   private:
    REGISTER_LOGGER("openstudio.analysis.FSUDaceAlgorithm");
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_FSUDACEALGORITHM_IMPL_HPP

