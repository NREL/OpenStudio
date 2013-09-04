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
                               const OptimizationProblem& optimizationProblem,
                               bool complete,
                               bool failed,
                               bool selected,
                               DataPointRunType runType,
                               const std::vector<QVariant>& variableValues,
                               const std::vector<double>& responseValues,
                               const std::vector<double>& objectiveValues,
                               const openstudio::path& directory,
                               const boost::optional<FileReference>& osmInputData,
                               const boost::optional<FileReference>& idfInputData,
                               const boost::optional<FileReference>& sqlOutputData,
                               const std::vector<FileReference>& xmlOutputData,
                               const boost::optional<runmanager::Job>& topLevelJob,
                               const std::vector<openstudio::path>& dakotaParametersFiles,
                               const std::vector<Tag>& tags,
                               const std::vector<Attribute>& outputAttributes);

    /** Constructor provided for deserialization; not for general use. */
    OptimizationDataPoint_Impl(const UUID& uuid,
                               const UUID& versionUUID,
                               const std::string& name,
                               const std::string& displayName,
                               const std::string& description,
                               const UUID& problemUUID,
                               const boost::optional<UUID>& analysisUUID,
                               bool complete,
                               bool failed,
                               bool selected,
                               DataPointRunType runType,
                               const std::vector<QVariant>& variableValues,
                               const std::vector<double>& responseValues,
                               const std::vector<double>& objectiveValues,
                               const openstudio::path& directory,
                               const boost::optional<FileReference>& osmInputData,
                               const boost::optional<FileReference>& idfInputData,
                               const boost::optional<FileReference>& sqlOutputData,
                               const std::vector<FileReference>& xmlOutputData,
                               const boost::optional<runmanager::Job>& topLevelJob,
                               const std::vector<openstudio::path>& dakotaParametersFiles,
                               const std::vector<Tag>& tags,
                               const std::vector<Attribute>& outputAttributes);

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
    /** @name Actions */
    //@{

    /** Update high level results from json. */
    virtual bool updateFromJSON(const AnalysisJSONLoadResult& loadResult, 
                                boost::optional<runmanager::RunManager>& runManager);

    //@}
    /** @name Protected in or Absent from Public Class */
    //@{

    void setObjectiveValues(const std::vector<double> values);

    virtual QVariant toVariant() const;

    static OptimizationDataPoint fromVariant(const QVariant& variant, const VersionString& version);

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

