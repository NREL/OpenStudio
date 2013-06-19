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

#ifndef ANALYSIS_OPTIMIZATIONDATAPOINT_IMPL_HPP
#define ANALYSIS_OPTIMIZATIONDATAPOINT_IMPL_HPP

#include <analysis/AnalysisAPI.hpp>
#include <analysis/DataPoint_Impl.hpp>

namespace openstudio {
namespace analysis {

class OptimizationDataPoint;
class OptimizationProblem;

namespace detail {

  /** OptimizationDataPoint_Impl is a DataPoint_Impl that is the implementation class for OptimizationDataPoint.*/
  class ANALYSIS_API OptimizationDataPoint_Impl : public DataPoint_Impl {
    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    /** Constructor from variableValues. Called by Algorithm, may also be called directly by a
     *  user. After construction isComplete() == false. */
    OptimizationDataPoint_Impl(const OptimizationProblem& optimizationProblem,
                               const std::vector<QVariant>& variableValues);

    /** Constructor provided for deserialization; not for general use. */
    OptimizationDataPoint_Impl(const UUID& uuid,
                               const UUID& versionUUID,
                               const std::string& name,
                               const std::string& displayName,
                               const std::string& description,
                               bool complete,
                               bool failed,
                               const OptimizationProblem& optimizationProblem,
                               const std::vector<double>& objectiveValues,
                               const std::vector<QVariant>& variableValues,
                               const std::vector<double>& responseValues,
                               const openstudio::path& directory,
                               const boost::optional<FileReference>& osmInputData,
                               const boost::optional<FileReference>& idfInputData,
                               const boost::optional<FileReference>& sqlOutputData,
                               const boost::optional<FileReference>& xmlOutputData,
                               const std::vector<Tag>& tags,
                               const boost::optional<runmanager::Job>& topLevelJob,
                               const std::vector<openstudio::path>& dakotaParametersFiles);

    OptimizationDataPoint_Impl(const OptimizationDataPoint_Impl& other);

    virtual ~OptimizationDataPoint_Impl() {}

    virtual AnalysisObject clone() const;

    //@}
    /** @name Getters */
    //@{

    /** Equivalent to .problem().cast<OptimizationProblem>(). Provided for convenience. */
    OptimizationProblem optimizationProblem() const;

    std::vector<double> objectiveValues() const;

    //@}
    /** @name Setters */
    //@{

    virtual void clearResults();

    //@}
    /** @name Protected in Public Class */
    //@{

    void setObjectiveValues(const std::vector<double> values);

    //@}
   protected:
    std::vector<double> m_objectiveValues;
   private:
    REGISTER_LOGGER("openstudio.analysis.OptimizationDataPoint");
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_OPTIMIZATIONDATAPOINT_IMPL_HPP

