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
#include <analysis/NullPerturbation.hpp>
#include <analysis/NullPerturbation_Impl.hpp>
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

  DiscreteVariable_Impl::DiscreteVariable_Impl(
      const std::string& name,
      const std::vector<DiscretePerturbation>& perturbations)
    : InputVariable_Impl(name),
      m_perturbations(perturbations)
  {
    // get null perturbations straightened out
    bool hasNull(false);
    bool isSelected(false);
    DiscretePerturbationVector::iterator it = m_perturbations.begin();
    while (it != m_perturbations.end()) {
      if (OptionalNullPerturbation oNull = it->optionalCast<NullPerturbation>()) {
        if (isSelected && oNull->isSelected()) {
          it = m_perturbations.erase(it); // only let there be one NullPerturbation that is selected.
          continue;
        }
        hasNull = true;
        if (!isSelected) {
          isSelected = oNull->isSelected();
        }
      }
      ++it;
    }
    // throw if perturbation file types are inconsistent.
    std::pair<bool,OptionalFileReferenceType> inType = detail::inputFileType(perturbations);
    std::pair<bool,OptionalFileReferenceType> outType = detail::outputFileType(perturbations);
    if (!inType.first) {
      LOG_AND_THROW("Input file types of perturbations used to attempt to construct "
                    << "DiscreteVariable '" << name << "' are inconsistent.");
    }
    if (!outType.first) {
      LOG_AND_THROW("Output file types of perturbations used to attempt to construct "
                    << "DiscreteVariable '" << name << "' are inconsistent.");
    }
    if (hasNull && inType.second && outType.second &&
        (inType.second.get() != outType.second.get()))
    {
      LOG_AND_THROW("Unable to construct DiscreteVariable '" << name <<
                    "', because at least one perturbation changes the file type between input and " <<
                    "output, and there is also a NullPerturbation (which cannot change the file type).");
    }
    BOOST_FOREACH(DiscretePerturbation& perturbation,m_perturbations) {
      perturbation.onChange();
      connectChild(perturbation,false);
    }
  }

  DiscreteVariable_Impl::DiscreteVariable_Impl(
      const UUID& uuid,
      const UUID& versionUUID,
      const std::string& name,
      const std::string& displayName,
      const std::string& description,
      const boost::optional<UncertaintyDescription>& udesc,
      const std::vector<DiscretePerturbation>& perturbations)
    : InputVariable_Impl(uuid,versionUUID,name,displayName,description,udesc),
      m_perturbations(perturbations)
  {
    BOOST_FOREACH(DiscretePerturbation& perturbation,m_perturbations) {
      connectChild(perturbation,false);
    }
  }

  DiscreteVariable_Impl::DiscreteVariable_Impl(const DiscreteVariable_Impl &other)
    : InputVariable_Impl(other)
  {
    BOOST_FOREACH(const DiscretePerturbation& pert,other.perturbations(false)) {
      m_perturbations.push_back(pert.clone().cast<DiscretePerturbation>());
      connectChild(m_perturbations.back(),false);
    }
  }

  AnalysisObject DiscreteVariable_Impl::clone() const {
    boost::shared_ptr<DiscreteVariable_Impl> impl(new DiscreteVariable_Impl(*this));
    DiscreteVariable result(impl);
    DiscretePerturbationVector perturbations = result.perturbations(false);
    BOOST_FOREACH(DiscretePerturbation& perturbation,perturbations) {
      perturbation.setParent(result);
    }
    return result;
  }

  double DiscreteVariable_Impl::getValue(const DataPoint& dataPoint) const {
    OptionalInt index = dataPoint.problem().getVariableIndexByUUID(uuid());
    BOOST_ASSERT(index);
    return dataPoint.variableValues()[*index].toDouble();
  }

  boost::optional<FileReferenceType> DiscreteVariable_Impl::inputFileType() const {
    std::pair<bool,OptionalFileReferenceType> intermediate = detail::inputFileType(perturbations(false));
    BOOST_ASSERT(intermediate.first);
    return intermediate.second;
  }

  boost::optional<FileReferenceType> DiscreteVariable_Impl::outputFileType() const {
    std::pair<bool,OptionalFileReferenceType> intermediate = detail::outputFileType(perturbations(false));
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
    if ((index < 0) || (index >= int(numPerturbations(false)))) {
      LOG(Warn,"Unexpected QVariant value type sent into isValid method of DiscreteVariable '"
          << name() << "'. Expected an index into this variable's perturbation vector, but index = " << index
          << ", even though expected an integer in the range [0," << numPerturbations(false) << ").");
      return false;
    }
    // okay if perturbation is not selected--isSelected used by Algorithm, but user may choose to
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

    return m_perturbations[index].createWorkItem(rubyIncludeDirectory);
  }

  std::vector<DiscretePerturbation> DiscreteVariable_Impl::perturbations(
      bool selectedPerturbationsOnly) const
  {
    if (selectedPerturbationsOnly) {
      DiscretePerturbationVector result;
      BOOST_FOREACH(const DiscretePerturbation& perturbation,m_perturbations) {
        if (perturbation.isSelected()) {
          result.push_back(perturbation);
        }
      }
      return result;
    }

    return m_perturbations;
  }

  DiscretePerturbation DiscreteVariable_Impl::getPerturbation(int index) const {
    if (index >= int(m_perturbations.size())) {
      LOG_AND_THROW("DiscreteVariable '" << name() << "' only has " << m_perturbations.size()
          << " perturbations; perturbation " << index << " does not exist.");
    }
    DiscretePerturbation result = m_perturbations[index];
    return result;
  }

  boost::optional<DiscretePerturbation> DiscreteVariable_Impl::getPerturbationByUUID(
      const UUID& uuid) const
  {
    OptionalDiscretePerturbation result;
    DiscretePerturbationVector::const_iterator it = std::find_if(
        m_perturbations.begin(),
        m_perturbations.end(),
        boost::bind(uuidEquals<DiscretePerturbation,UUID>,_1,uuid));
    if (it != m_perturbations.end()) {
      result = *it;
    }
    return result;
  }

  unsigned DiscreteVariable_Impl::numPerturbations(bool selectedPerturbationsOnly) const {
    return perturbations(selectedPerturbationsOnly).size();
  }

  boost::optional<int> DiscreteVariable_Impl::getIndexByUUID(
      const DiscretePerturbation& perturbation) const
  {
    OptionalInt result;
    DiscretePerturbationVector::const_iterator it = std::find_if(
          m_perturbations.begin(),
          m_perturbations.end(),
          boost::bind(uuidEquals<DiscretePerturbation,UUID>,_1,perturbation.uuid()));
    if (it != m_perturbations.end()) {
      result = int(it - m_perturbations.begin());
    }
    return result;
  }

  DiscretePerturbation DiscreteVariable_Impl::getPerturbation(const DataPoint& dataPoint) const {
    OptionalInt index = dataPoint.problem().getVariableIndexByUUID(uuid());
    BOOST_ASSERT(index);
    return getPerturbation(dataPoint.variableValues()[*index].toInt());
  }

  bool DiscreteVariable_Impl::push(const DiscretePerturbation& perturbation) {
    DiscretePerturbationVector candidates = perturbations(false);
    candidates.push_back(perturbation);
    if (!perturbationsAreCompatible(candidates)) {
      return false;
    }

    m_perturbations.push_back(perturbation);
    m_perturbations.back().onChange();
    connectChild(m_perturbations.back(),true);
    onChange(AnalysisObject_Impl::Benign);
    return true;
  }

  bool DiscreteVariable_Impl::insert(int index, const DiscretePerturbation& perturbation) {
    if ((index < 0) || (index >= int(numPerturbations(false)))) {
      return false;
    }
    // position does not matter for compatibility checking
    DiscretePerturbationVector candidates = perturbations(false);
    candidates.push_back(perturbation);
    if (!perturbationsAreCompatible(candidates)) {
      return false;
    }

    DiscretePerturbationVector::iterator it = m_perturbations.begin();
    for (int count = 0; count < index; ++count, ++it);
    it = m_perturbations.insert(it,perturbation);
    for (int i = index, n = int(m_perturbations.size()); i < n; ++i) {
      m_perturbations[i].onChange();
    }
    connectChild(m_perturbations[index],true);
    onChange(AnalysisObject_Impl::InvalidatesDataPoints);
    return true;
  }

  bool DiscreteVariable_Impl::erase(const DiscretePerturbation& perturbation) {
    DiscretePerturbationVector::iterator it = std::find_if(
        m_perturbations.begin(),
        m_perturbations.end(),
        boost::bind(uuidsEqual<DiscretePerturbation,DiscretePerturbation>,_1,perturbation));
    if (it == m_perturbations.end()) {
      return false;
    }
    int index = int(it - m_perturbations.begin());
    disconnectChild(*it);
    m_perturbations.erase(it);
    for (int i = index, n = int(m_perturbations.size()); i < n; ++i) {
      m_perturbations[i].onChange();
    }
    onChange(AnalysisObject_Impl::InvalidatesDataPoints);
    return true;
  }

  bool DiscreteVariable_Impl::swap(const DiscretePerturbation& perturbation1,
                                   const DiscretePerturbation& perturbation2)
  {
    DiscretePerturbationVector::iterator it1 = std::find_if(
        m_perturbations.begin(),
        m_perturbations.end(),
        boost::bind(uuidsEqual<DiscretePerturbation,DiscretePerturbation>,_1,perturbation1));
    DiscretePerturbationVector::iterator it2 = std::find_if(
        m_perturbations.begin(),
        m_perturbations.end(),
        boost::bind(uuidsEqual<DiscretePerturbation,DiscretePerturbation>,_1,perturbation2));
    if ((it1 == m_perturbations.end()) || (it2 == m_perturbations.end())) {
      return false;
    }
    DiscretePerturbation temp = *it1;
    int index1 = int(it1 - m_perturbations.begin());
    int index2 = int(it2 - m_perturbations.begin());
    m_perturbations.at(index1) = *it2;
    m_perturbations.at(index2) = temp;
    m_perturbations[index1].onChange();
    m_perturbations[index2].onChange();
    onChange(AnalysisObject_Impl::InvalidatesDataPoints);
    return true;
  }

  bool DiscreteVariable_Impl::setPerturbations(const std::vector<DiscretePerturbation>& perturbations) {
    if (!perturbationsAreCompatible(perturbations)) {
      return false;
    }

    BOOST_FOREACH(DiscretePerturbation& perturbation,m_perturbations) {
      disconnectChild(perturbation);
    }
    m_perturbations = perturbations;
    BOOST_FOREACH(DiscretePerturbation& perturbation,m_perturbations) {
      perturbation.onChange();
      connectChild(perturbation,true);
    }
    onChange(AnalysisObject_Impl::InvalidatesDataPoints);
    return true;
  }

  void DiscreteVariable_Impl::clearPerturbations() {
    BOOST_FOREACH(DiscretePerturbation& perturbation,m_perturbations) {
      disconnectChild(perturbation);
    }
    m_perturbations.clear();
    onChange(AnalysisObject_Impl::InvalidatesDataPoints);
  }

  bool DiscreteVariable_Impl::fileTypesAreCompatible(
      const DiscretePerturbation& childPerturbation,
      const FileReferenceType& proposedInputFileType,
      const FileReferenceType& proposedOutputFileType) const
  {
    // can only change file types if no null perturbations
    if (proposedInputFileType != proposedOutputFileType) {
      NullPerturbationVector nullPerturbations = subsetCastVector<NullPerturbation>(m_perturbations);
      if (!nullPerturbations.empty()) {
        return false;
      }
    }

    // check proposals against file types of other perturbations
    DiscretePerturbationVector pv = perturbations(false);
    DiscretePerturbationVector::iterator it = std::find(pv.begin(),pv.end(),childPerturbation);
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

  bool DiscreteVariable_Impl::perturbationsAreCompatible(
      const std::vector<DiscretePerturbation>& perturbations) const
  {
    // perturbations must be internally consistent
    std::pair<bool,OptionalFileReferenceType> inputFileTypeResult = detail::inputFileType(perturbations);
    if (!inputFileTypeResult.first) {
      return false;
    }
    std::pair<bool,OptionalFileReferenceType> outputFileTypeResult = detail::outputFileType(perturbations);
    if (!outputFileTypeResult.first) {
      return false;
    }

    // perturbations cannot change file type and contain a null
    if (inputFileTypeResult.second && outputFileTypeResult.second &&
        (inputFileTypeResult.second.get() != outputFileTypeResult.second.get()))
    {
      NullPerturbationVector nullPerturbations = subsetCastVector<NullPerturbation>(perturbations);
      if (!nullPerturbations.empty()) {
        return false;
      }
    }

    // perturbations must be a-ok with workflow step
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
      const std::vector<DiscretePerturbation>& perturbations)
  {
    std::pair<bool,OptionalFileReferenceType> result(true,boost::none);
    BOOST_FOREACH(const DiscretePerturbation& perturbation,perturbations) {
      OptionalFileReferenceType temp = perturbation.inputFileType();
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
      const std::vector<DiscretePerturbation>& perturbations)
  {
    std::pair<bool,OptionalFileReferenceType> result(true,boost::none);
    BOOST_FOREACH(const DiscretePerturbation& perturbation,perturbations) {
      OptionalFileReferenceType temp = perturbation.outputFileType();
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
                                   const std::vector<DiscretePerturbation>& perturbations)
  : InputVariable(boost::shared_ptr<detail::DiscreteVariable_Impl>(
        new detail::DiscreteVariable_Impl(name,perturbations)))
{
  DiscreteVariable copyOfThis(getImpl<detail::DiscreteVariable_Impl>());
  BOOST_FOREACH(const DiscretePerturbation& perturbation,perturbations) {
    perturbation.setParent(copyOfThis);
  }
}

DiscreteVariable::DiscreteVariable(const UUID& uuid,
                                   const UUID& versionUUID,
                                   const std::string& name,
                                   const std::string& displayName,
                                   const std::string& description,
                                   const boost::optional<UncertaintyDescription>& udesc,
                                   const std::vector<DiscretePerturbation>& perturbations)
  : InputVariable(boost::shared_ptr<detail::DiscreteVariable_Impl>(
        new detail::DiscreteVariable_Impl(uuid,
                                          versionUUID,
                                          name,
                                          displayName,
                                          description,
                                          udesc,
                                          perturbations)))
{
  DiscreteVariable copyOfThis(getImpl<detail::DiscreteVariable_Impl>());
  BOOST_FOREACH(const DiscretePerturbation& perturbation,perturbations) {
    perturbation.setParent(copyOfThis);
  }
}

std::vector<DiscretePerturbation> DiscreteVariable::perturbations(bool selectedPerturbationsOnly) const
{
  return getImpl<detail::DiscreteVariable_Impl>()->perturbations(selectedPerturbationsOnly);
}

DiscretePerturbation DiscreteVariable::getPerturbation(int index) const {
  return getImpl<detail::DiscreteVariable_Impl>()->getPerturbation(index);
}

boost::optional<DiscretePerturbation> DiscreteVariable::getPerturbationByUUID(const UUID& uuid) const {
  return getImpl<detail::DiscreteVariable_Impl>()->getPerturbationByUUID(uuid);
}

unsigned DiscreteVariable::numPerturbations(bool selectedPerturbationsOnly) const {
  return getImpl<detail::DiscreteVariable_Impl>()->numPerturbations(selectedPerturbationsOnly);
}

boost::optional<int> DiscreteVariable::getIndexByUUID(const DiscretePerturbation& perturbation) const {
  return getImpl<detail::DiscreteVariable_Impl>()->getIndexByUUID(perturbation);
}

DiscretePerturbation DiscreteVariable::getPerturbation(const DataPoint& dataPoint) const {
  return getImpl<detail::DiscreteVariable_Impl>()->getPerturbation(dataPoint);
}

bool DiscreteVariable::push(const DiscretePerturbation& perturbation) {
  return getImpl<detail::DiscreteVariable_Impl>()->push(perturbation);
}

bool DiscreteVariable::insert(int index, const DiscretePerturbation& perturbation) {
  return getImpl<detail::DiscreteVariable_Impl>()->insert(index,perturbation);
}

bool DiscreteVariable::erase(const DiscretePerturbation& perturbation) {
  return getImpl<detail::DiscreteVariable_Impl>()->erase(perturbation);
}

bool DiscreteVariable::swap(const DiscretePerturbation& perturbation1,
                            const DiscretePerturbation& perturbation2)
{
  return getImpl<detail::DiscreteVariable_Impl>()->swap(perturbation1,perturbation2);
}

bool DiscreteVariable::setPerturbations(const std::vector<DiscretePerturbation>& perturbations) {
  return getImpl<detail::DiscreteVariable_Impl>()->setPerturbations(perturbations);
}

void DiscreteVariable::clearPerturbations() {
  return getImpl<detail::DiscreteVariable_Impl>()->clearPerturbations();
}

/// @cond
DiscreteVariable::DiscreteVariable(boost::shared_ptr<detail::DiscreteVariable_Impl> impl)
  : InputVariable(impl)
{}

bool DiscreteVariable::fileTypesAreCompatible(const DiscretePerturbation& childPerturbation,
                                              const FileReferenceType& proposedInputFileType,
                                              const FileReferenceType& proposedOutputFileType) const
{
  return getImpl<detail::DiscreteVariable_Impl>()->fileTypesAreCompatible(childPerturbation,
                                                                          proposedInputFileType,
                                                                          proposedOutputFileType);
}
/// @endcond

} // analysis
} // openstudio

