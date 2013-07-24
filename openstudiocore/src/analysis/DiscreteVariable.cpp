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

#include <analysis/DiscreteVariable.hpp>
#include <analysis/DiscreteVariable_Impl.hpp>

#include <analysis/DataPoint.hpp>
#include <analysis/NullMeasure.hpp>
#include <analysis/NullMeasure_Impl.hpp>
#include <analysis/Problem.hpp>
#include <analysis/WorkflowStep.hpp>
#include <analysis/WorkflowStep_Impl.hpp>

#include <runmanager/lib/WorkItem.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Compare.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/core/FileReference.hpp>
#include <utilities/core/Optional.hpp>

#include <boost/foreach.hpp>
#include <boost/bind.hpp>

namespace openstudio {
namespace analysis {

namespace detail {

  DiscreteVariable_Impl::DiscreteVariable_Impl(const std::string& name,
                                               const std::vector<Measure>& measures)
    : InputVariable_Impl(name),
      m_measures(measures)
  {
    // get null measures straightened out
    bool hasNull(false);
    bool isSelected(false);
    MeasureVector::iterator it = m_measures.begin();
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
                    << "DiscreteVariable '" << name << "' are inconsistent.");
    }
    if (!outType.first) {
      LOG_AND_THROW("Output file types of measures used to attempt to construct "
                    << "DiscreteVariable '" << name << "' are inconsistent.");
    }
    if (hasNull && inType.second && outType.second &&
        (inType.second.get() != outType.second.get()))
    {
      LOG_AND_THROW("Unable to construct DiscreteVariable '" << name <<
                    "', because at least one measure changes the file type between input and " <<
                    "output, and there is also a NullMeasure (which cannot change the file type).");
    }
    BOOST_FOREACH(Measure& measure,m_measures) {
      measure.onChange();
      connectChild(measure,false);
    }
  }

  DiscreteVariable_Impl::DiscreteVariable_Impl(
      const UUID& uuid,
      const UUID& versionUUID,
      const std::string& name,
      const std::string& displayName,
      const std::string& description,
      const boost::optional<UncertaintyDescription>& udesc,
      const std::vector<Measure>& measures)
    : InputVariable_Impl(uuid,versionUUID,name,displayName,description,udesc),
      m_measures(measures)
  {
    BOOST_FOREACH(Measure& measure,m_measures) {
      connectChild(measure,false);
    }
  }

  DiscreteVariable_Impl::DiscreteVariable_Impl(const DiscreteVariable_Impl &other)
    : InputVariable_Impl(other)
  {
    BOOST_FOREACH(const Measure& pert,other.measures(false)) {
      m_measures.push_back(pert.clone().cast<Measure>());
      connectChild(m_measures.back(),false);
    }
  }

  AnalysisObject DiscreteVariable_Impl::clone() const {
    boost::shared_ptr<DiscreteVariable_Impl> impl(new DiscreteVariable_Impl(*this));
    DiscreteVariable result(impl);
    MeasureVector measures = result.measures(false);
    BOOST_FOREACH(Measure& measure,measures) {
      measure.setParent(result);
    }
    return result;
  }

  double DiscreteVariable_Impl::getValue(const DataPoint& dataPoint) const {
    OptionalInt index = dataPoint.problem().getVariableIndexByUUID(uuid());
    BOOST_ASSERT(index);
    return dataPoint.variableValues()[*index].toDouble();
  }

  boost::optional<FileReferenceType> DiscreteVariable_Impl::inputFileType() const {
    std::pair<bool,OptionalFileReferenceType> intermediate = detail::inputFileType(measures(false));
    BOOST_ASSERT(intermediate.first);
    return intermediate.second;
  }

  boost::optional<FileReferenceType> DiscreteVariable_Impl::outputFileType() const {
    std::pair<bool,OptionalFileReferenceType> intermediate = detail::outputFileType(measures(false));
    BOOST_ASSERT(intermediate.first);
    return intermediate.second;
  }

  bool DiscreteVariable_Impl::isValid(const QVariant& value) const {
    if (!((value.type() == QVariant::Int) || (value.type() == QVariant::UInt))) {
      LOG(Warn,"Unexpected QVariant value type sent into isValid method of DiscreteVariable '"
          << name() << "'. Expected an integer, but was passed a " << value.typeName() << ".");
      return false;
    }
    int index = value.toInt();
    if ((index < 0) || (index >= int(numMeasures(false)))) {
      LOG(Warn,"Unexpected QVariant value type sent into isValid method of DiscreteVariable '"
          << name() << "'. Expected an index into this variable's measure vector, but index = " << index
          << ", even though expected an integer in the range [0," << numMeasures(false) << ").");
      return false;
    }
    // okay if measure is not selected--isSelected used by Algorithm, but user may choose to
    // run a "not selected" point.
    return true;
  }

  bool DiscreteVariable_Impl::isValid(const UncertaintyDescription& udesc) const {
    UncertaintyDescriptionTypeVector validTypes = UncertaintyDescription::validTypes(VariableValueType::Discrete);
    return (std::find(validTypes.begin(),validTypes.end(),udesc.type()) != validTypes.end());
  }

  runmanager::WorkItem DiscreteVariable_Impl::createWorkItem(
      const QVariant& value,
      const openstudio::path& rubyIncludeDirectory) const
  {
    if (!isValid(value)) {
      LOG_AND_THROW("Invalid value sent into the createJob method of DiscreteVariable '"
          << name() << "'.");
    }

    int index = value.toInt();

    return m_measures[index].createWorkItem(rubyIncludeDirectory);
  }

  std::vector<Measure> DiscreteVariable_Impl::measures(
      bool selectedMeasuresOnly) const
  {
    if (selectedMeasuresOnly) {
      MeasureVector result;
      BOOST_FOREACH(const Measure& measure,m_measures) {
        if (measure.isSelected()) {
          result.push_back(measure);
        }
      }
      return result;
    }

    return m_measures;
  }

  Measure DiscreteVariable_Impl::getMeasure(int index) const {
    if (index >= int(m_measures.size())) {
      LOG_AND_THROW("DiscreteVariable '" << name() << "' only has " << m_measures.size()
          << " measures; measure " << index << " does not exist.");
    }
    Measure result = m_measures[index];
    return result;
  }

  boost::optional<Measure> DiscreteVariable_Impl::getMeasureByUUID(
      const UUID& uuid) const
  {
    OptionalMeasure result;
    MeasureVector::const_iterator it = std::find_if(
        m_measures.begin(),
        m_measures.end(),
        boost::bind(uuidEquals<Measure,UUID>,_1,uuid));
    if (it != m_measures.end()) {
      result = *it;
    }
    return result;
  }

  unsigned DiscreteVariable_Impl::numMeasures(bool selectedMeasuresOnly) const {
    return measures(selectedMeasuresOnly).size();
  }

  boost::optional<int> DiscreteVariable_Impl::getIndexByUUID(
      const Measure& measure) const
  {
    OptionalInt result;
    MeasureVector::const_iterator it = std::find_if(
          m_measures.begin(),
          m_measures.end(),
          boost::bind(uuidEquals<Measure,UUID>,_1,measure.uuid()));
    if (it != m_measures.end()) {
      result = int(it - m_measures.begin());
    }
    return result;
  }

  Measure DiscreteVariable_Impl::getMeasure(const DataPoint& dataPoint) const {
    OptionalInt index = dataPoint.problem().getVariableIndexByUUID(uuid());
    BOOST_ASSERT(index);
    return getMeasure(dataPoint.variableValues()[*index].toInt());
  }

  bool DiscreteVariable_Impl::push(const Measure& measure) {
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

  bool DiscreteVariable_Impl::insert(int index, const Measure& measure) {
    if ((index < 0) || (index >= int(numMeasures(false)))) {
      return false;
    }
    // position does not matter for compatibility checking
    MeasureVector candidates = measures(false);
    candidates.push_back(measure);
    if (!measuresAreCompatible(candidates)) {
      return false;
    }

    MeasureVector::iterator it = m_measures.begin();
    for (int count = 0; count < index; ++count, ++it);
    it = m_measures.insert(it,measure);
    for (int i = index, n = int(m_measures.size()); i < n; ++i) {
      m_measures[i].onChange();
    }
    connectChild(m_measures[index],true);
    onChange(AnalysisObject_Impl::InvalidatesDataPoints);
    return true;
  }

  bool DiscreteVariable_Impl::erase(const Measure& measure) {
    MeasureVector::iterator it = std::find_if(
        m_measures.begin(),
        m_measures.end(),
        boost::bind(uuidsEqual<Measure,Measure>,_1,measure));
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

  bool DiscreteVariable_Impl::swap(const Measure& measure1,
                                   const Measure& measure2)
  {
    MeasureVector::iterator it1 = std::find_if(
        m_measures.begin(),
        m_measures.end(),
        boost::bind(uuidsEqual<Measure,Measure>,_1,measure1));
    MeasureVector::iterator it2 = std::find_if(
        m_measures.begin(),
        m_measures.end(),
        boost::bind(uuidsEqual<Measure,Measure>,_1,measure2));
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

  bool DiscreteVariable_Impl::setMeasures(const std::vector<Measure>& measures) {
    if (!measuresAreCompatible(measures)) {
      return false;
    }

    BOOST_FOREACH(Measure& measure,m_measures) {
      disconnectChild(measure);
    }
    m_measures = measures;
    BOOST_FOREACH(Measure& measure,m_measures) {
      measure.onChange();
      connectChild(measure,true);
    }
    onChange(AnalysisObject_Impl::InvalidatesDataPoints);
    return true;
  }

  void DiscreteVariable_Impl::clearMeasures() {
    BOOST_FOREACH(Measure& measure,m_measures) {
      disconnectChild(measure);
    }
    m_measures.clear();
    onChange(AnalysisObject_Impl::InvalidatesDataPoints);
  }

  bool DiscreteVariable_Impl::fileTypesAreCompatible(
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
    MeasureVector::iterator it = std::find(pv.begin(),pv.end(),childMeasure);
    BOOST_ASSERT(it != pv.end());
    pv.erase(it);

    std::pair<bool,OptionalFileReferenceType> inputFileTypeResult = detail::inputFileType(pv);
    BOOST_ASSERT(inputFileTypeResult.first);
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

  bool DiscreteVariable_Impl::measuresAreCompatible(
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

  std::pair<bool,boost::optional<FileReferenceType> > inputFileType(
      const std::vector<Measure>& measures)
  {
    std::pair<bool,OptionalFileReferenceType> result(true,boost::none);
    BOOST_FOREACH(const Measure& measure,measures) {
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
    BOOST_FOREACH(const Measure& measure,measures) {
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

DiscreteVariable::DiscreteVariable(const std::string& name,
                                   const std::vector<Measure>& measures)
  : InputVariable(boost::shared_ptr<detail::DiscreteVariable_Impl>(
        new detail::DiscreteVariable_Impl(name,measures)))
{
  DiscreteVariable copyOfThis(getImpl<detail::DiscreteVariable_Impl>());
  BOOST_FOREACH(const Measure& measure,measures) {
    measure.setParent(copyOfThis);
  }
}

DiscreteVariable::DiscreteVariable(const UUID& uuid,
                                   const UUID& versionUUID,
                                   const std::string& name,
                                   const std::string& displayName,
                                   const std::string& description,
                                   const boost::optional<UncertaintyDescription>& udesc,
                                   const std::vector<Measure>& measures)
  : InputVariable(boost::shared_ptr<detail::DiscreteVariable_Impl>(
        new detail::DiscreteVariable_Impl(uuid,
                                          versionUUID,
                                          name,
                                          displayName,
                                          description,
                                          udesc,
                                          measures)))
{
  DiscreteVariable copyOfThis(getImpl<detail::DiscreteVariable_Impl>());
  BOOST_FOREACH(const Measure& measure,measures) {
    measure.setParent(copyOfThis);
  }
}

std::vector<Measure> DiscreteVariable::measures(bool selectedMeasuresOnly) const
{
  return getImpl<detail::DiscreteVariable_Impl>()->measures(selectedMeasuresOnly);
}

std::vector<Measure> DiscreteVariable::perturbations(bool selectedMeasuresOnly) const {
  return getImpl<detail::DiscreteVariable_Impl>()->measures(selectedMeasuresOnly);
}

Measure DiscreteVariable::getMeasure(int index) const {
  return getImpl<detail::DiscreteVariable_Impl>()->getMeasure(index);
}

Measure DiscreteVariable::getPerturbation(int index) const {
  return getImpl<detail::DiscreteVariable_Impl>()->getMeasure(index);
}

boost::optional<Measure> DiscreteVariable::getMeasureByUUID(const UUID& uuid) const {
  return getImpl<detail::DiscreteVariable_Impl>()->getMeasureByUUID(uuid);
}

boost::optional<Measure> DiscreteVariable::getPerturbationByUUID(const UUID& uuid) const {
  return getImpl<detail::DiscreteVariable_Impl>()->getMeasureByUUID(uuid);
}

unsigned DiscreteVariable::numMeasures(bool selectedMeasuresOnly) const {
  return getImpl<detail::DiscreteVariable_Impl>()->numMeasures(selectedMeasuresOnly);
}

unsigned DiscreteVariable::numPerturbations(bool selectedMeasuresOnly) const {
  return getImpl<detail::DiscreteVariable_Impl>()->numMeasures(selectedMeasuresOnly);
}

boost::optional<int> DiscreteVariable::getIndexByUUID(const Measure& measure) const {
  return getImpl<detail::DiscreteVariable_Impl>()->getIndexByUUID(measure);
}

Measure DiscreteVariable::getMeasure(const DataPoint& dataPoint) const {
  return getImpl<detail::DiscreteVariable_Impl>()->getMeasure(dataPoint);
}

Measure DiscreteVariable::getPerturbation(const DataPoint& dataPoint) const {
  return getImpl<detail::DiscreteVariable_Impl>()->getMeasure(dataPoint);
}

bool DiscreteVariable::push(const Measure& measure) {
  return getImpl<detail::DiscreteVariable_Impl>()->push(measure);
}

bool DiscreteVariable::insert(int index, const Measure& measure) {
  return getImpl<detail::DiscreteVariable_Impl>()->insert(index,measure);
}

bool DiscreteVariable::erase(const Measure& measure) {
  return getImpl<detail::DiscreteVariable_Impl>()->erase(measure);
}

bool DiscreteVariable::swap(const Measure& measure1,
                            const Measure& measure2)
{
  return getImpl<detail::DiscreteVariable_Impl>()->swap(measure1,measure2);
}

bool DiscreteVariable::setMeasures(const std::vector<Measure>& measures) {
  return getImpl<detail::DiscreteVariable_Impl>()->setMeasures(measures);
}

bool DiscreteVariable::setPerturbations(const std::vector<Measure>& measures) {
  return getImpl<detail::DiscreteVariable_Impl>()->setMeasures(measures);
}

void DiscreteVariable::clearMeasures() {
  return getImpl<detail::DiscreteVariable_Impl>()->clearMeasures();
}

void DiscreteVariable::clearPerturbations() {
  return getImpl<detail::DiscreteVariable_Impl>()->clearMeasures();
}

/// @cond
DiscreteVariable::DiscreteVariable(boost::shared_ptr<detail::DiscreteVariable_Impl> impl)
  : InputVariable(impl)
{}

bool DiscreteVariable::fileTypesAreCompatible(const Measure& childMeasure,
                                              const FileReferenceType& proposedInputFileType,
                                              const FileReferenceType& proposedOutputFileType) const
{
  return getImpl<detail::DiscreteVariable_Impl>()->fileTypesAreCompatible(childMeasure,
                                                                          proposedInputFileType,
                                                                          proposedOutputFileType);
}
/// @endcond

} // analysis
} // openstudio

