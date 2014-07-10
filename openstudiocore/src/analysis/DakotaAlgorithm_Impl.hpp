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

#ifndef ANALYSIS_DAKOTAALGORITHM_IMPL_HPP
#define ANALYSIS_DAKOTAALGORITHM_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "Algorithm_Impl.hpp"

#include "../runmanager/lib/Job.hpp"

#include "../utilities/core/FileReference.hpp"

namespace openstudio {
namespace analysis {

class Analysis;
class DakotaAlgorithmOptions;
class DataPoint;
class DakotaParametersFile;
class UncertaintyDescriptionType;

namespace detail {

  /** DakotaAlgorithm_Impl is a Algorithm_Impl that is the implementation class for
   *  DakotaAlgorithm. */
  class ANALYSIS_API DakotaAlgorithm_Impl : public Algorithm_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    DakotaAlgorithm_Impl(const std::string& name, const DakotaAlgorithmOptions& options);

    // TODO: Add other items necessary for construction.
    DakotaAlgorithm_Impl(const UUID& uuid,
                         const UUID& versionUUID,
                         const std::string& name,
                         const std::string& displayName,
                         const std::string& description,
                         bool complete,
                         bool failed,
                         int iter,
                         const DakotaAlgorithmOptions& options,
                         const boost::optional<FileReference>& restartFileReference,
                         const boost::optional<FileReference>& outFileReference, 
                         const boost::optional<openstudio::runmanager::Job>& job);

    DakotaAlgorithm_Impl(const DakotaAlgorithm_Impl& other);

    virtual ~DakotaAlgorithm_Impl() {}

    //@}
    /** @name Getters and Queries */
    //@{

    /** Returns true if the algorithm requires continuous variables. The Problem class uses this
     *  method to determine whether to represent OpenStudio DiscreteVariables as continuous
     *  variables when communicating with DAKOTA. If the desired behavior is to fail, rather than
     *  to fake continuity, then isCompatibleProblemType should return false if Problem contains
     *  non-trivial (> 1 perturbation) discrete variables. */
    virtual bool requiresContinuousVariables() const;

    virtual bool isCompatible(const UncertaintyDescriptionType& udescType) const;

    /** Returns a reference to the DAKOTA restart file, if possible. Should exist if this algorithm
     *  is used by an analysis that has been run using the AnalysisDriver. */
    boost::optional<FileReference> restartFileReference() const;

    /** Returns a reference to the DAKOTA out file, if possible. Should exist if this algorithm
     *  is used by an analysis that has been run using the AnalysisDriver. */
    boost::optional<FileReference> outFileReference() const;

    /** Returns the RunManager job if possible. */
    boost::optional<runmanager::Job> job() const;

    //@}
    /** @name Actions */
    //@{

    virtual std::string dakotaInFileDescription() const = 0;

    /** Tries to create a new data point from dakotaParametersFile, returning the new point, or an
     *  equivalent point already present in analysis. Also updates this algorithm's internal iter
     *  counter. */
    virtual boost::optional<DataPoint> createNextDataPoint(Analysis& analysis,
                                                           const DakotaParametersFile& params);

    //@}
    /** @name Protected in or Absent from Public Class */
    //@{

    virtual void reset();

    /** Called after Dakota job is constructed. As a side effect, sets iter to 0. */
    void setRestartFileReference(const FileReference& file);

    /** Called after Dakota job is constructed. As a side effect, sets iter to 0. */
    void setOutFileReference(const FileReference& file);

    /** Sets the RunManager job. */
    void setJob(const runmanager::Job& job);

    virtual QVariant toVariant() const;

    //@}
   private:
    boost::optional<FileReference> m_restartFileReference;
    boost::optional<FileReference> m_outFileReference;
    boost::optional<runmanager::Job> m_job; 

    REGISTER_LOGGER("openstudio.analysis.DakotaAlgorithm");
  };

} // detail

} // analysis
} // openstudio

#endif // ANALYSIS_DAKOTAALGORITHM_IMPL_HPP

