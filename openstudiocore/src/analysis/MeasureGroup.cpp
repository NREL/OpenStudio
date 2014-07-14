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

#include "MeasureGroup.hpp"
#include "MeasureGroup_Impl.hpp"

#include "DataPoint.hpp"
#include "NullMeasure.hpp"
#include "NullMeasure_Impl.hpp"
#include "Problem.hpp"
#include "WorkflowStep.hpp"
#include "WorkflowStep_Impl.hpp"

#include "../runmanager/lib/WorkItem.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Containers.hpp"
#include "../utilities/core/FileReference.hpp"
#include "../utilities/core/Json.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  MeasureGroup_Impl::MeasureGroup_Impl(const std::string& name,
                                       const std::vector<Measure>& measures)
    : DiscreteVariable_Impl(name),
      m_measures(measures)
  {
    // get null measures straightened out
    bool hasNull(false);
    bool isSelected(false);
    auto it = m_measures.begin();
    while (it != m_measures.end()) {
      if (OptionalNullMeasure oNull = it->optionalCast<NullMeasure>()) {
        if (isSelected && oNull->isSelected()) {
          it = m_measures.erase(it); // only let there be one NullMeasure that is selected.
          continue;
        }
        hasNull = true;
        if (!isSelected) {
          isSelected = oNull->isSelected();
        }
      }
      ++it;
    }
    // throw if measure file types are inconsistent.
    std::pair<bool,OptionalFileReferenceType> inType = detail::inputFileType(measures);
    std::pair<bool,OptionalFileReferenceType> outType = detail::outputFileType(measures);
    if (!inType.first) {
      LOG_AND_THROW("Input file types of measures used to attempt to construct "
                    << "MeasureGroup '" << name << "' are inconsistent.");
    }
    if (!outType.first) {
      LOG_AND_THROW("Output file types of measures used to attempt to construct "
                    << "MeasureGroup '" << name << "' are inconsistent.");
    }
    if (hasNull && inType.second && outType.second &&
        (inType.second.get() != outType.second.get()))
    {
      LOG_AND_THROW("Unable to construct MeasureGroup '" << name <<
                    "', because at least one measure changes the file type between input and " <<
                    "output, and there is also a NullMeasure (which cannot change the file type).");
    }
    for (Measure& measure : m_measures) {
      measure.onChange();
      connectChild(measure,false);
    }
  }

  MeasureGroup_Impl::MeasureGroup_Impl(
      const UUID& uuid,
      const UUID& versionUUID,
      const std::string& name,
      const std::string& displayName,
      const std::string& description,
      const boost::optional<UncertaintyDescription>& udesc,
      const std::vector<Measure>& measures)
    : DiscreteVariable_Impl(uuid,versionUUID,name,displayName,description,udesc),
      m_measures(measures)
  {
    for (Measure& measure : m_measures) {
      connectChild(measure,false);
    }
  }

  MeasureGroup_Impl::MeasureGroup_Impl(const MeasureGroup_Impl &other)
    : DiscreteVariable_Impl(other)
  {
    for (const Measure& pert : other.measures(false)) {
      m_measures.push_back(pert.clone().cast<Measure>());
      connectChild(m_measures.back(),false);
    }
  }

  AnalysisObject MeasureGroup_Impl::clone() const {
    std::shared_ptr<MeasureGroup_Impl> impl(new MeasureGroup_Impl(*this));
    MeasureGroup result(impl);
    MeasureVector measures = result.measures(false);
    for (Measure& measure : measures) {
      measure.setParent(result);
    }
    return result;
  }

  boost::optional<FileReferenceType> MeasureGroup_Impl::inputFileType() const {
    std::pair<bool,OptionalFileReferenceType> intermediate = detail::inputFileType(measures(false));
    OS_ASSERT(intermediate.first);
    return intermediate.second;
  }

  boost::optional<FileReferenceType> MeasureGroup_Impl::outputFileType() const {
    std::pair<bool,OptionalFileReferenceType> intermediate = detail::outputFileType(measures(false));
    OS_ASSERT(intermediate.first);
    return intermediate.second;
  }

  bool MeasureGroup_Impl::isValid(const QVariant& value) const {
    if (!((value.type() == QVariant::Int) || (value.type() == QVariant::UInt))) {
      LOG(Warn,"Unexpected QVariant value type sent into isValid method of MeasureGroup '"
          << name() << "'. Expected an integer, but was passed a " << value.typeName() << ".");
      return false;
    }
    int index = value.toInt();
    if ((index < 0) || (index >= int(numMeasures(false)))) {
      LOG(Warn,"Unexpected QVariant value type sent into isValid method of MeasureGroup '"
          << name() << "'. Expected an index into this variable's measure vector, but index = " << index
          << ", even though expected an integer in the range [0," << numMeasures(false) << ").");
      return false;
    }
    // okay if measure is not selected--isSelected used by Algorithm, but user may choose to
    // run a "not selected" point.
    return true;
  }

  runmanager::WorkItem MeasureGroup_Impl::createWorkItem(
      const QVariant& value,
      const openstudio::path& rubyIncludeDirectory) const
  {
    if (!isValid(value)) {
      LOG_AND_THROW("Invalid value sent into the createJob method of MeasureGroup '"
          << name() << "'.");
    }

    int index = value.toInt();

    return m_measures[index].createWorkItem(rubyIncludeDirectory);
  }

  std::vector<int> MeasureGroup_Impl::validValues(bool selectedOnly) const {
    IntVector result;

    if (selectedOnly) {
      int index(0);
      for (const Measure& measure : measures(false)) {
        if (measure.isSelected()) {
          result.push_back(index);
        }
        ++index;
      }
    }
    else {
      for (int i = 0, n = numMeasures(false); i < n; ++i) {
        result.push_back(i);
      }
    }

    return result;
  }

  std::vector<Measure> MeasureGroup_Impl::measures(
      bool selectedMeasuresOnly) const
  {
    if (selectedMeasuresOnly) {
      MeasureVector result;
      for (const Measure& measure : m_measures) {
        if (measure.isSelected()) {
          result.push_back(measure);
        }
      }
      return result;
    }

    return m_measures;
  }

  Measure MeasureGroup_Impl::getMeasure(int index) const {
    if (index >= int(m_measures.size())) {
      LOG_AND_THROW("MeasureGroup '" << name() << "' only has " << m_measures.size()
          << " measures; measure " << index << " does not exist.");
    }
    Measure result = m_measures[index];
    return result;
  }

  boost::optional<Measure> MeasureGroup_Impl::getMeasureByUUID(
      const UUID& uuid) const
  {
    OptionalMeasure result;
    auto it = std::find_if(
        m_measures.begin(),
        m_measures.end(),
        std::bind(uuidEquals<Measure,UUID>,std::placeholders::_1,uuid));
    if (it != m_measures.end()) {
      result = *it;
    }
    return result;
  }

  unsigned MeasureGroup_Impl::numMeasures(bool selectedMeasuresOnly) const {
    return measures(selectedMeasuresOnly).size();
  }

  boost::optional<int> MeasureGroup_Impl::getIndexByUUID(
      const Measure& measure) const
  {
    OptionalInt result;
    auto it = std::find_if(
          m_measures.begin(),
          m_measures.end(),
          std::bind(uuidEquals<Measure,UUID>,std::placeholders::_1,measure.uuid()));
    if (it != m_measures.end()) {
      result = int(it - m_measures.begin());
    }
    return result;
  }

  Measure MeasureGroup_Impl::getMeasure(const DataPoint& dataPoint) const {
    OptionalInt index = dataPoint.problem().getVariableIndexByUUID(uuid());
    OS_ASSERT(index);
    return getMeasure(dataPoint.variableValues()[*index].toInt());
  }

  bool MeasureGroup_Impl::push(const Measure& measure) {
    MeasureVector candidates = measures(false);
    candidates.push_back(measure);
    if (!measuresAreCompatible(candidates)) {
      return false;
    }

    m_measures.push_back(measure);
    m_measures.back().onChange();
    connectChild(m_measures.back(),true);
    onChange(AnalysisObject_Impl::Benign);
    return true;
  }

  bool MeasureGroup_Impl::insert(int index, const Measure& measure) {
    if ((index < 0) || (index > int(numMeasures(false)))) {
      return false;
    }

    if (index == int(numMeasures(false))) {
      return push(measure);
    }

    // position does not matter for compatibility checking
    MeasureVector candidates = measures(false);
    candidates.push_back(measure);
    if (!measuresAreCompatible(candidates)) {
      return false;
    }

    auto it = m_measures.begin();
    for (int count = 0; count < index; ++count, ++it);
    it = m_measures.insert(it,measure);
    for (int i = index, n = int(m_measures.size()); i < n; ++i) {
      m_measures[i].onChange();
    }
    connectChild(m_measures[index],true);
    onChange(AnalysisObject_Impl::InvalidatesDataPoints);
    return true;
  }

  bool MeasureGroup_Impl::erase(const Measure& measure) {
    auto it = std::find_if(
        m_measures.begin(),
        m_measures.end(),
        std::bind(uuidsEqual<Measure,Measure>,std::placeholders::_1,measure));
    if (it == m_measures.end()) {
      return false;
    }
    int index = int(it - m_measures.begin());
    disconnectChild(*it);
    m_measures.erase(it);
    for (int i = index, n = int(m_measures.size()); i < n; ++i) {
      m_measures[i].onChange();
    }
    onChange(AnalysisObject_Impl::InvalidatesDataPoints);
    return true;
  }

  bool MeasureGroup_Impl::swap(const Measure& measure1,
                                   const Measure& measure2)
  {
    auto it1 = std::find_if(
        m_measures.begin(),
        m_measures.end(),
        std::bind(uuidsEqual<Measure,Measure>,std::placeholders::_1,measure1));
    auto it2 = std::find_if(
        m_measures.begin(),
        m_measures.end(),
        std::bind(uuidsEqual<Measure,Measure>,std::placeholders::_1,measure2));
    if ((it1 == m_measures.end()) || (it2 == m_measures.end())) {
      return false;
    }
    Measure temp = *it1;
    int index1 = int(it1 - m_measures.begin());
    int index2 = int(it2 - m_measures.begin());
    m_measures.at(index1) = *it2;
    m_measures.at(index2) = temp;
    m_measures[index1].onChange();
    m_measures[index2].onChange();
    onChange(AnalysisObject_Impl::InvalidatesDataPoints);
    return true;
  }

  bool MeasureGroup_Impl::setMeasures(const std::vector<Measure>& measures) {
    if (!measuresAreCompatible(measures)) {
      return false;
    }

    for (Measure& measure : m_measures) {
      disconnectChild(measure);
    }
    m_measures = measures;
    for (Measure& measure : m_measures) {
      measure.onChange();
      connectChild(measure,true);
    }
    onChange(AnalysisObject_Impl::InvalidatesDataPoints);
    return true;
  }

  void MeasureGroup_Impl::clearMeasures() {
    for (Measure& measure : m_measures) {
      disconnectChild(measure);
    }
    m_measures.clear();
    onChange(AnalysisObject_Impl::InvalidatesDataPoints);
  }

  bool MeasureGroup_Impl::fileTypesAreCompatible(
      const Measure& childMeasure,
      const FileReferenceType& proposedInputFileType,
      const FileReferenceType& proposedOutputFileType) const
  {
    // can only change file types if no null measures
    if (proposedInputFileType != proposedOutputFileType) {
      NullMeasureVector nullMeasures = subsetCastVector<NullMeasure>(m_measures);
      if (!nullMeasures.empty()) {
        return false;
      }
    }

    // check proposals against file types of other measures
    MeasureVector pv = measures(false);
    auto it = std::find(pv.begin(),pv.end(),childMeasure);
    OS_ASSERT(it != pv.end());
    pv.erase(it);

    std::pair<bool,OptionalFileReferenceType> inputFileTypeResult = detail::inputFileType(pv);
    OS_ASSERT(inputFileTypeResult.first);
    if (inputFileTypeResult.second && (proposedInputFileType != *(inputFileTypeResult.second))) {
      return false;
    }

    std::pair<bool,OptionalFileReferenceType> outputFileTypeResult = detail::outputFileType(pv);
    if (outputFileTypeResult.second && (proposedOutputFileType != *(outputFileTypeResult.second))) {
      return false;
    }

    // either proposals match current file types, or at least one current file type is null.
    // if matches current, should be a-okay.
    // otherwise, proposing a new file type on input or output side for this variable, so
    // ask problem if ok.
    if (OptionalAnalysisObject parent = this->parent()) {
      if (!inputFileTypeResult.second || !outputFileTypeResult.second) {
        if (!(parent->cast<WorkflowStep>().fileTypesAreCompatible(proposedInputFileType,
                                                                  proposedOutputFileType)))
        {
          return false;
        }
      }
    }
    return true;
  }

  bool MeasureGroup_Impl::measuresAreCompatible(
      const std::vector<Measure>& measures) const
  {
    // measures must be internally consistent
    std::pair<bool,OptionalFileReferenceType> inputFileTypeResult = detail::inputFileType(measures);
    if (!inputFileTypeResult.first) {
      return false;
    }
    std::pair<bool,OptionalFileReferenceType> outputFileTypeResult = detail::outputFileType(measures);
    if (!outputFileTypeResult.first) {
      return false;
    }

    // measures cannot change file type and contain a null
    if (inputFileTypeResult.second && outputFileTypeResult.second &&
        (inputFileTypeResult.second.get() != outputFileTypeResult.second.get()))
    {
      NullMeasureVector nullMeasures = subsetCastVector<NullMeasure>(measures);
      if (!nullMeasures.empty()) {
        return false;
      }
    }

    // measures must be a-ok with workflow step
    if (OptionalAnalysisObject parent = this->parent()) {
      if (!(parent->cast<WorkflowStep>().fileTypesAreCompatible(inputFileTypeResult.second,
                                                                outputFileTypeResult.second)))
      {
        return false;
      }
    }

    return true;
  }

  QVariant MeasureGroup_Impl::toVariant() const {
    QVariantMap measureGroupData = InputVariable_Impl::toVariant().toMap();

    measureGroupData["workflow_step_type"] = "MeasureGroup";

    int index(0);
    QVariantList measuresList;
    for (const Measure& measure : measures(false)) {
      QVariantMap measureData = measure.toVariant().toMap();
      measureData["measure_group_index"] = index;
      measuresList.push_back(measureData);
      ++index;
    }
    measureGroupData["measures"] = measuresList;

    return QVariant(measureGroupData);
  }

  MeasureGroup MeasureGroup_Impl::fromVariant(const QVariant& variant, const VersionString& version) {
    QVariantMap map = variant.toMap();

    MeasureVector measures = deserializeOrderedVector(
          map["measures"].toList(),
          "measure_group_index",
          std::function<Measure (const QVariant&)>(std::bind(Measure_Impl::factoryFromVariant,std::placeholders::_1,version)));

    return MeasureGroup(toUUID(map["uuid"].toString().toStdString()),
                        toUUID(map["version_uuid"].toString().toStdString()),
                        map.contains("name") ? map["name"].toString().toStdString() : std::string(),
                        map.contains("display_name") ? map["display_name"].toString().toStdString() : std::string(),
                        map.contains("description") ? map["description"].toString().toStdString() : std::string(),
                        map.contains("uncertainty_description") ? analysis::detail::toUncertaintyDescription(map["uncertainty_description"],version) : OptionalUncertaintyDescription(),
                        measures);
  }

  void MeasureGroup_Impl::updateInputPathData(const openstudio::path& originalBase,
                                              const openstudio::path& newBase)
  {
    MeasureVector measures = this->measures(false);
    for (Measure& measure : measures) {
      measure.getImpl<detail::Measure_Impl>()->updateInputPathData(originalBase,newBase);
    }
  }

  std::pair<bool,boost::optional<FileReferenceType> > inputFileType(
      const std::vector<Measure>& measures)
  {
    std::pair<bool,OptionalFileReferenceType> result(true,boost::none);
    for (const Measure& measure : measures) {
      OptionalFileReferenceType temp = measure.inputFileType();
      if (temp) {
        if (result.second && (*temp != *(result.second))) {
          result.first = false;
          return result;
        }
        result.second = temp;
      }
    }
    return result;
  }

  std::pair<bool,boost::optional<FileReferenceType> > outputFileType(
      const std::vector<Measure>& measures)
  {
    std::pair<bool,OptionalFileReferenceType> result(true,boost::none);
    for (const Measure& measure : measures) {
      OptionalFileReferenceType temp = measure.outputFileType();
      if (temp) {
        if (result.second && (*temp != *(result.second))) {
          result.first = false;
          return result;
        }
        result.second = temp;
      }
    }
    return result;
  }

} // detail

MeasureGroup::MeasureGroup(const std::string& name,
                                   const std::vector<Measure>& measures)
  : DiscreteVariable(std::shared_ptr<detail::MeasureGroup_Impl>(
        new detail::MeasureGroup_Impl(name,measures)))
{
  MeasureGroup copyOfThis(getImpl<detail::MeasureGroup_Impl>());
  for (const Measure& measure : measures) {
    measure.setParent(copyOfThis);
  }
}

MeasureGroup::MeasureGroup(const UUID& uuid,
                                   const UUID& versionUUID,
                                   const std::string& name,
                                   const std::string& displayName,
                                   const std::string& description,
                                   const boost::optional<UncertaintyDescription>& udesc,
                                   const std::vector<Measure>& measures)
  : DiscreteVariable(std::shared_ptr<detail::MeasureGroup_Impl>(
        new detail::MeasureGroup_Impl(uuid,
                                      versionUUID,
                                      name,
                                      displayName,
                                      description,
                                      udesc,
                                      measures)))
{
  MeasureGroup copyOfThis(getImpl<detail::MeasureGroup_Impl>());
  for (const Measure& measure : measures) {
    measure.setParent(copyOfThis);
  }
}

std::vector<Measure> MeasureGroup::measures(bool selectedMeasuresOnly) const
{
  return getImpl<detail::MeasureGroup_Impl>()->measures(selectedMeasuresOnly);
}

std::vector<Measure> MeasureGroup::perturbations(bool selectedMeasuresOnly) const {
  return getImpl<detail::MeasureGroup_Impl>()->measures(selectedMeasuresOnly);
}

Measure MeasureGroup::getMeasure(int index) const {
  return getImpl<detail::MeasureGroup_Impl>()->getMeasure(index);
}

Measure MeasureGroup::getPerturbation(int index) const {
  return getImpl<detail::MeasureGroup_Impl>()->getMeasure(index);
}

boost::optional<Measure> MeasureGroup::getMeasureByUUID(const UUID& uuid) const {
  return getImpl<detail::MeasureGroup_Impl>()->getMeasureByUUID(uuid);
}

boost::optional<Measure> MeasureGroup::getPerturbationByUUID(const UUID& uuid) const {
  return getImpl<detail::MeasureGroup_Impl>()->getMeasureByUUID(uuid);
}

unsigned MeasureGroup::numMeasures(bool selectedMeasuresOnly) const {
  return getImpl<detail::MeasureGroup_Impl>()->numMeasures(selectedMeasuresOnly);
}

unsigned MeasureGroup::numPerturbations(bool selectedMeasuresOnly) const {
  return getImpl<detail::MeasureGroup_Impl>()->numMeasures(selectedMeasuresOnly);
}

boost::optional<int> MeasureGroup::getIndexByUUID(const Measure& measure) const {
  return getImpl<detail::MeasureGroup_Impl>()->getIndexByUUID(measure);
}

Measure MeasureGroup::getMeasure(const DataPoint& dataPoint) const {
  return getImpl<detail::MeasureGroup_Impl>()->getMeasure(dataPoint);
}

Measure MeasureGroup::getPerturbation(const DataPoint& dataPoint) const {
  return getImpl<detail::MeasureGroup_Impl>()->getMeasure(dataPoint);
}

bool MeasureGroup::push(const Measure& measure) {
  return getImpl<detail::MeasureGroup_Impl>()->push(measure);
}

bool MeasureGroup::insert(int index, const Measure& measure) {
  return getImpl<detail::MeasureGroup_Impl>()->insert(index,measure);
}

bool MeasureGroup::erase(const Measure& measure) {
  return getImpl<detail::MeasureGroup_Impl>()->erase(measure);
}

bool MeasureGroup::swap(const Measure& measure1,
                            const Measure& measure2)
{
  return getImpl<detail::MeasureGroup_Impl>()->swap(measure1,measure2);
}

bool MeasureGroup::setMeasures(const std::vector<Measure>& measures) {
  return getImpl<detail::MeasureGroup_Impl>()->setMeasures(measures);
}

bool MeasureGroup::setPerturbations(const std::vector<Measure>& measures) {
  return getImpl<detail::MeasureGroup_Impl>()->setMeasures(measures);
}

void MeasureGroup::clearMeasures() {
  return getImpl<detail::MeasureGroup_Impl>()->clearMeasures();
}

void MeasureGroup::clearPerturbations() {
  return getImpl<detail::MeasureGroup_Impl>()->clearMeasures();
}

/// @cond
MeasureGroup::MeasureGroup(std::shared_ptr<detail::MeasureGroup_Impl> impl)
  : DiscreteVariable(impl)
{}

bool MeasureGroup::fileTypesAreCompatible(const Measure& childMeasure,
                                              const FileReferenceType& proposedInputFileType,
                                              const FileReferenceType& proposedOutputFileType) const
{
  return getImpl<detail::MeasureGroup_Impl>()->fileTypesAreCompatible(childMeasure,
                                                                          proposedInputFileType,
                                                                          proposedOutputFileType);
}
/// @endcond

} // analysis
} // openstudio

