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

#include "OptimizationDataPoint.hpp"
#include "OptimizationDataPoint_Impl.hpp"

#include "OptimizationProblem.hpp"
#include "OptimizationProblem_Impl.hpp"

#include "../utilities/core/Containers.hpp"
#include "../utilities/core/Json.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  OptimizationDataPoint_Impl::OptimizationDataPoint_Impl(
      const OptimizationProblem& optimizationProblem,
      const std::vector<QVariant>& variableValues)
    : DataPoint_Impl(optimizationProblem,variableValues)
  {}

  OptimizationDataPoint_Impl::OptimizationDataPoint_Impl(
      const UUID& uuid,
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
      const std::vector<Attribute>& outputAttributes)
    : DataPoint_Impl(uuid,
                     versionUUID,
                     name,
                     displayName,
                     description,
                     optimizationProblem,
                     complete,
                     failed,
                     selected,
                     runType,
                     variableValues,
                     responseValues,
                     directory,
                     osmInputData,
                     idfInputData,
                     sqlOutputData,
                     topLevelJob,
                     dakotaParametersFiles,
                     tags,
                     outputAttributes),
      m_objectiveValues(objectiveValues)
  {}

  OptimizationDataPoint_Impl::OptimizationDataPoint_Impl(
      const UUID& uuid,
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
      const std::vector<Attribute>& outputAttributes)
    : DataPoint_Impl(uuid,
                     versionUUID,
                     name,
                     displayName,
                     description,
                     problemUUID,
                     analysisUUID,
                     complete,
                     failed,
                     selected,
                     runType,
                     variableValues,
                     responseValues,
                     directory,
                     osmInputData,
                     idfInputData,
                     sqlOutputData,
                     topLevelJob,
                     dakotaParametersFiles,
                     tags,
                     outputAttributes),
      m_objectiveValues(objectiveValues)
  {}

  OptimizationDataPoint_Impl::OptimizationDataPoint_Impl(const OptimizationDataPoint_Impl &other)
    : DataPoint_Impl(other),
      m_objectiveValues(other.objectiveValues())
  {}

  AnalysisObject OptimizationDataPoint_Impl::clone() const {
    std::shared_ptr<OptimizationDataPoint_Impl> impl(new OptimizationDataPoint_Impl(*this));
    return OptimizationDataPoint(impl);
  }

  OptimizationProblem OptimizationDataPoint_Impl::optimizationProblem() const {
    return problem().cast<OptimizationProblem>();
  }

  std::vector<double> OptimizationDataPoint_Impl::objectiveValues() const {
    return m_objectiveValues;
  }

  void OptimizationDataPoint_Impl::clearResults() {
    DataPoint_Impl::clearResults();
    m_objectiveValues.clear();
  }

  bool OptimizationDataPoint_Impl::updateFromJSON(const AnalysisJSONLoadResult& loadResult, boost::optional<runmanager::RunManager>& runManager) {
    if (loadResult.analysisObject) {
      if (OptionalOptimizationDataPoint loaded = loadResult.analysisObject->optionalCast<OptimizationDataPoint>()) {
        bool result = DataPoint_Impl::updateFromJSON(loadResult,runManager);
        if (result) {
          m_objectiveValues = loaded->objectiveValues();
        }
        return result;
      }
      else {
        LOG(Info,"Cannot update OptimizationDataPoint because the AnalysisObject loaded from JSON is not an OptimizationDataPoint.");
      }
    }
    else {
      LOG(Info,"Cannot update DataPoint from JSON because the JSON string could not be loaded.");
    }

    return false;
  }

  void OptimizationDataPoint_Impl::setObjectiveValues(const std::vector<double> values) {
    m_objectiveValues = values;
    onChange(AnalysisObject_Impl::Benign);
  }

  QVariant OptimizationDataPoint_Impl::toVariant() const {
    QVariantMap dataPointData = DataPoint_Impl::toVariant().toMap();

    dataPointData["data_point_type"] = QString("OptimizationDataPoint");

    if (!objectiveValues().empty()) {
      QVariantList objectiveValuesList;
      int index(0);
      for (double value : objectiveValues()) {
        QVariantMap objectiveMap;
        objectiveMap["objective_value_index"] = QVariant(index);
        objectiveMap["value"] = QVariant(value);
        objectiveValuesList.push_back(objectiveMap);
        ++index;
      }
      dataPointData["objective_values"] = objectiveValuesList;
    }

    return QVariant(dataPointData);
  }

  OptimizationDataPoint OptimizationDataPoint_Impl::fromVariant(const QVariant& variant, const VersionString& version) {
    DataPoint slice = DataPoint_Impl::fromVariant(variant,version);

    QVariantMap map = variant.toMap();

    bool ok(false);
    DoubleVector objectiveValues = deserializeOrderedVector<double>(
          map["objective_values"].toList(),
          "value",
          "objective_value_index",
          std::function<double (QVariant*)>(std::bind(&QVariant::toDouble,std::placeholders::_1,&ok)));

    return OptimizationDataPoint(slice.uuid(),
                                 slice.versionUUID(),
                                 slice.name(),
                                 slice.displayName(),
                                 slice.description(),
                                 slice.problemUUID(),
                                 slice.analysisUUID(),
                                 slice.isComplete(),
                                 slice.failed(),
                                 slice.selected(),
                                 slice.runType(),
                                 slice.variableValues(),
                                 slice.responseValues(),
                                 objectiveValues,
                                 slice.directory(),
                                 slice.osmInputData(),
                                 slice.idfInputData(),
                                 slice.sqlOutputData(),
                                 slice.topLevelJob(),
                                 slice.dakotaParametersFiles(),
                                 slice.tags(),
                                 slice.outputAttributes());
  }

} // detail

OptimizationDataPoint::OptimizationDataPoint(const OptimizationProblem& optimizationProblem,
                                             const std::vector<QVariant>& variableValues)
  : DataPoint(std::shared_ptr<detail::OptimizationDataPoint_Impl>(
                  new detail::OptimizationDataPoint_Impl(optimizationProblem,variableValues)))
{}

OptimizationDataPoint::OptimizationDataPoint(const UUID& uuid,
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
                                             const std::vector<Attribute>& outputAttributes)
  : DataPoint(std::shared_ptr<detail::OptimizationDataPoint_Impl>(
                  new detail::OptimizationDataPoint_Impl(uuid,
                                                         versionUUID,
                                                         name,
                                                         displayName,
                                                         description,
                                                         optimizationProblem,
                                                         complete,
                                                         failed,
                                                         selected,
                                                         runType,
                                                         variableValues,
                                                         responseValues,
                                                         objectiveValues,
                                                         directory,
                                                         osmInputData,
                                                         idfInputData,
                                                         sqlOutputData,
                                                         topLevelJob,
                                                         dakotaParametersFiles,
                                                         tags,
                                                         outputAttributes)))
{}

OptimizationDataPoint::OptimizationDataPoint(const UUID& uuid,
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
                                             const std::vector<Attribute>& outputAttributes)
  : DataPoint(std::shared_ptr<detail::OptimizationDataPoint_Impl>(
                  new detail::OptimizationDataPoint_Impl(uuid,
                                                         versionUUID,
                                                         name,
                                                         displayName,
                                                         description,
                                                         problemUUID,
                                                         analysisUUID,
                                                         complete,
                                                         failed,
                                                         selected,
                                                         runType,
                                                         variableValues,
                                                         responseValues,
                                                         objectiveValues,
                                                         directory,
                                                         osmInputData,
                                                         idfInputData,
                                                         sqlOutputData,
                                                         topLevelJob,
                                                         dakotaParametersFiles,
                                                         tags,
                                                         outputAttributes)))
{}

OptimizationProblem OptimizationDataPoint::optimizationProblem() const {
  return getImpl<detail::OptimizationDataPoint_Impl>()->optimizationProblem();
}

std::vector<double> OptimizationDataPoint::objectiveValues() const {
  return getImpl<detail::OptimizationDataPoint_Impl>()->objectiveValues();
}

/// @cond
OptimizationDataPoint::OptimizationDataPoint(std::shared_ptr<detail::OptimizationDataPoint_Impl> impl)
  : DataPoint(impl)
{}
/// @endcond

void OptimizationDataPoint::setObjectiveValues(const std::vector<double> values) {
  getImpl<detail::OptimizationDataPoint_Impl>()->setObjectiveValues(values);
}

OptimizationDataPointObjectiveFunctionLess::OptimizationDataPointObjectiveFunctionLess(unsigned index)
  : m_index(index)
{}

bool OptimizationDataPointObjectiveFunctionLess::operator()(
    const OptimizationDataPoint& left,
    const OptimizationDataPoint& right) const
{
  DoubleVector leftValues = left.objectiveValues();
  DoubleVector rightValues = right.objectiveValues();
  if ((leftValues.size() > m_index) && (rightValues.size() > m_index)) {
    return leftValues[m_index] < rightValues[m_index];
  }
  return false;
}

} // analysis
} // openstudio

