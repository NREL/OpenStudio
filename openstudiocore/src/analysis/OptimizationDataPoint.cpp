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

#include <analysis/OptimizationDataPoint.hpp>
#include <analysis/OptimizationDataPoint_Impl.hpp>

#include <analysis/OptimizationProblem.hpp>
#include <analysis/OptimizationProblem_Impl.hpp>

#include <utilities/core/Containers.hpp>
#include <utilities/core/Json.hpp>

#include <boost/function.hpp>
#include <boost/bind.hpp>

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
      const std::vector<FileReference>& xmlOutputData,
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
                     xmlOutputData,
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
      const std::vector<FileReference>& xmlOutputData,
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
                     xmlOutputData,
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
    boost::shared_ptr<OptimizationDataPoint_Impl> impl(new OptimizationDataPoint_Impl(*this));
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
      Q_FOREACH(double value,objectiveValues()) {
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
          boost::function<double (QVariant*)>(boost::bind(&QVariant::toDouble,_1,&ok)));

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
                                 slice.xmlOutputData(),
                                 slice.topLevelJob(),
                                 slice.dakotaParametersFiles(),
                                 slice.tags(),
                                 slice.outputAttributes());
  }

} // detail

OptimizationDataPoint::OptimizationDataPoint(const OptimizationProblem& optimizationProblem,
                                             const std::vector<QVariant>& variableValues)
  : DataPoint(boost::shared_ptr<detail::OptimizationDataPoint_Impl>(
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
                                             const std::vector<FileReference>& xmlOutputData,
                                             const boost::optional<runmanager::Job>& topLevelJob,
                                             const std::vector<openstudio::path>& dakotaParametersFiles,
                                             const std::vector<Tag>& tags,
                                             const std::vector<Attribute>& outputAttributes)
  : DataPoint(boost::shared_ptr<detail::OptimizationDataPoint_Impl>(
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
                                                         xmlOutputData,
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
                                             const std::vector<FileReference>& xmlOutputData,
                                             const boost::optional<runmanager::Job>& topLevelJob,
                                             const std::vector<openstudio::path>& dakotaParametersFiles,
                                             const std::vector<Tag>& tags,
                                             const std::vector<Attribute>& outputAttributes)
  : DataPoint(boost::shared_ptr<detail::OptimizationDataPoint_Impl>(
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
                                                         xmlOutputData,
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
OptimizationDataPoint::OptimizationDataPoint(boost::shared_ptr<detail::OptimizationDataPoint_Impl> impl)
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

