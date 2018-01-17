/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef ANALYSIS_OPTIMIZATIONDATAPOINT_IMPL_HPP
#define ANALYSIS_OPTIMIZATIONDATAPOINT_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "DataPoint_Impl.hpp"

namespace openstudio {
namespace analysis {

class OptimizationDataPoint;
class OptimizationProblem;

namespace detail {

  /** OptimizationDataPoint_Impl is a DataPoint_Impl that is the implementation class for OptimizationDataPoint.*/
  class ANALYSIS_API OptimizationDataPoint_Impl : public DataPoint_Impl {
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
                               const boost::optional<runmanager::Job>& topLevelJob,
                               const std::vector<openstudio::path>& dakotaParametersFiles,
                               const std::vector<Tag>& tags,
                               const std::vector<Attribute>& outputAttributes);

    OptimizationDataPoint_Impl(const OptimizationDataPoint_Impl& other);

    virtual ~OptimizationDataPoint_Impl() {}

    virtual AnalysisObject clone() const override;

    //@}
    /** @name Getters */
    //@{

    /** Equivalent to .problem().cast<OptimizationProblem>(). Provided for convenience. */
    OptimizationProblem optimizationProblem() const;

    std::vector<double> objectiveValues() const;

    //@}
    /** @name Setters */
    //@{

    virtual void clearResults() override;

    //@}
    /** @name Actions */
    //@{

    /** Update high level results from json. */
    virtual bool updateFromJSON(const AnalysisJSONLoadResult& loadResult, 
                                boost::optional<runmanager::RunManager>& runManager) override;

    //@}
    /** @name Protected in or Absent from Public Class */
    //@{

    void setObjectiveValues(const std::vector<double> values);

    virtual QVariant toVariant() const override;

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

