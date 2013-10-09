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

#include <analysis/ParameterStudyAlgorithmOptions.hpp>
#include <analysis/ParameterStudyAlgorithmOptions_Impl.hpp>

#include <analysis/Problem.hpp>

#include <utilities/core/Json.hpp>

namespace openstudio {
namespace analysis {

namespace detail {

  ParameterStudyAlgorithmOptions_Impl::ParameterStudyAlgorithmOptions_Impl(
      const ParameterStudyAlgorithmType& algorithmType)
    : DakotaAlgorithmOptions_Impl(), m_algorithmType(algorithmType)
  {}
  
  ParameterStudyAlgorithmOptions_Impl::ParameterStudyAlgorithmOptions_Impl(
      const ParameterStudyAlgorithmType& algorithmType,
      const std::vector<Attribute>& options)
    : DakotaAlgorithmOptions_Impl(options),
      m_algorithmType(algorithmType)
  {}

  AlgorithmOptions ParameterStudyAlgorithmOptions_Impl::clone() const {
    boost::shared_ptr<ParameterStudyAlgorithmOptions_Impl> impl(
        new ParameterStudyAlgorithmOptions_Impl(*this));
    return ParameterStudyAlgorithmOptions(impl);
  }

  ParameterStudyAlgorithmType ParameterStudyAlgorithmOptions_Impl::algorithmType() const {
    return m_algorithmType;
  }

  std::vector<double> ParameterStudyAlgorithmOptions_Impl::finalPoint() const {
    std::vector<double> result;
    if (OptionalAttribute option = getOption("finalPoint")) {
      result = getDoubleVectorFromAttribute(*option);
    }
    return result;
  }

  std::vector<double> ParameterStudyAlgorithmOptions_Impl::stepVector() const {
    std::vector<double> result;
    if (OptionalAttribute option = getOption("stepVector")) {
      result = getDoubleVectorFromAttribute(*option);
    }
    return result;
  }

  boost::optional<int> ParameterStudyAlgorithmOptions_Impl::numSteps() const {
    OptionalInt result;
    if (OptionalAttribute option = getOption("numSteps")) {
      result = option->valueAsInteger();
    }
    return result;
  }

  std::vector<double> ParameterStudyAlgorithmOptions_Impl::listOfPoints() const {
    std::vector<double> result;
    if (OptionalAttribute option = getOption("listOfPoints")) {
      result = getDoubleVectorFromAttribute(*option);
    }
    return result;
  }

  std::vector<int> ParameterStudyAlgorithmOptions_Impl::stepsPerVariable() const {
    std::vector<int> result;
    if (OptionalAttribute option = getOption("stepsPerVariable")) {
      result = getIntVectorFromAttribute(*option);
    }
    return result;
  }

  std::vector<int> ParameterStudyAlgorithmOptions_Impl::partitions() const {
    std::vector<int> result;
    if (OptionalAttribute option = getOption("partitions")) {
      result = getIntVectorFromAttribute(*option);
    }
    return result;
  }

  void ParameterStudyAlgorithmOptions_Impl::setFinalPoint(const std::vector<double>& value) {
    Attribute option = createAttributeFromVector("finalPoint",value);
    if (OptionalAttribute currentOption = getOption("finalPoint")) {
      currentOption->setValue(option.valueAsAttributeVector());
    }
    else {
      saveOption(option);
    }
  }

  void ParameterStudyAlgorithmOptions_Impl::setStepVector(const std::vector<double>& value) {
    Attribute option = createAttributeFromVector("stepVector",value);
    if (OptionalAttribute currentOption = getOption("stepVector")) {
      currentOption->setValue(option.valueAsAttributeVector());
    }
    else {
      saveOption(option);
    }
  }

  bool ParameterStudyAlgorithmOptions_Impl::setNumSteps(int value) {
	  if (value < 1) {
      LOG(Warn,"Cannot set ParameterStudyAlgorithmOptions numSteps to a value less than one.");
      return false;
	  }
    OptionalAttribute option;
    if ((option = getOption("numSteps"))) {
      option->setValue(value);
    }
    else {
      option = Attribute("numSteps",value);
      saveOption(*option);
    }
    return true;
  }

  void ParameterStudyAlgorithmOptions_Impl::setListOfPoints(const std::vector<double>& value) {
    Attribute option = createAttributeFromVector("listOfPoints",value);
    if (OptionalAttribute currentOption = getOption("listOfPoints")) {
      currentOption->setValue(option.valueAsAttributeVector());
    }
    else {
      saveOption(option);
    }
  }

  bool ParameterStudyAlgorithmOptions_Impl::setStepsPerVariable(const std::vector<int>& value) {
    int minValue = *std::min_element(value.begin(), value.end());
    if (minValue < 0) {
      LOG(Warn,"Cannot set ParameterStudyAlgorithmOptions stepsPerVariable to a value less than zero.");
      return false;
	  }
    Attribute option = createAttributeFromVector("stepsPerVariable",value);
    if (OptionalAttribute currentOption = getOption("stepsPerVariable")) {
      currentOption->setValue(option.valueAsAttributeVector());
    }
    else {
      saveOption(option);
    }
    return true;
  }

  bool ParameterStudyAlgorithmOptions_Impl::setPartitions(const std::vector<int>& value) {
    int minValue = *std::min_element(value.begin(), value.end());
    if (minValue < 0) {
      LOG(Warn,"Cannot set ParameterStudyAlgorithmOptions partitions to a value less than zero.");
      return false;
	  }
    Attribute option = createAttributeFromVector("partitions",value);
    if (OptionalAttribute currentOption = getOption("partitions")) {
      currentOption->setValue(option.valueAsAttributeVector());
    }
    else {
      saveOption(option);
    }
    return true;
  }

  void ParameterStudyAlgorithmOptions_Impl::clearFinalPoint() {
    clearOption("finalPoint");
  }

  void ParameterStudyAlgorithmOptions_Impl::clearStepVector() {
    clearOption("stepVector");
  }

  void ParameterStudyAlgorithmOptions_Impl::clearNumSteps() {
    clearOption("numSteps");
  }

  void ParameterStudyAlgorithmOptions_Impl::clearListOfPoints() {
    clearOption("listOfPoints");
  }

  void ParameterStudyAlgorithmOptions_Impl::clearStepsPerVariable() {
    clearOption("stepsPerVariable");
  }

  void ParameterStudyAlgorithmOptions_Impl::clearPartitions() {
    clearOption("partitions");
  }

  QVariant ParameterStudyAlgorithmOptions_Impl::toVariant() const {
    QVariantMap map = AlgorithmOptions_Impl::toVariant().toMap();

    map["parameter_study_algorithm_type"] = toQString(algorithmType().valueName());

    return QVariant(map);
  }

  ParameterStudyAlgorithmOptions ParameterStudyAlgorithmOptions_Impl::fromVariant(const QVariant& variant,
                                                                                  const VersionString& version)
  {
    QVariantMap map = variant.toMap();
    AttributeVector attributes = deserializeUnorderedVector(
          map["attributes"].toList(),
          boost::function<Attribute (const QVariant&)>(boost::bind(openstudio::detail::toAttribute,_1,version)));
    return ParameterStudyAlgorithmOptions(map["parameter_study_algorithm_type"].toString().toStdString(),
                                          attributes);
  }

} // detail


ParameterStudyAlgorithmOptions::ParameterStudyAlgorithmOptions(
    const ParameterStudyAlgorithmType& algorithmType)
  : DakotaAlgorithmOptions(boost::shared_ptr<detail::ParameterStudyAlgorithmOptions_Impl>(
        new detail::ParameterStudyAlgorithmOptions_Impl(algorithmType)))
{}

ParameterStudyAlgorithmOptions::ParameterStudyAlgorithmOptions(
    const ParameterStudyAlgorithmType& algorithmType,
    const std::vector<Attribute>& options)
  : DakotaAlgorithmOptions(boost::shared_ptr<detail::ParameterStudyAlgorithmOptions_Impl> (
        new detail::ParameterStudyAlgorithmOptions_Impl(algorithmType,options)))
{}

ParameterStudyAlgorithmType ParameterStudyAlgorithmOptions::algorithmType() const {
  return getImpl<detail::ParameterStudyAlgorithmOptions_Impl>()->algorithmType();
}

std::vector<double> ParameterStudyAlgorithmOptions::finalPoint() const {
  return getImpl<detail::ParameterStudyAlgorithmOptions_Impl>()->finalPoint();
}

std::vector<double> ParameterStudyAlgorithmOptions::stepVector() const {
  return getImpl<detail::ParameterStudyAlgorithmOptions_Impl>()->stepVector();
}

boost::optional<int> ParameterStudyAlgorithmOptions::numSteps() const {
  return getImpl<detail::ParameterStudyAlgorithmOptions_Impl>()->numSteps();
}

std::vector<double> ParameterStudyAlgorithmOptions::listOfPoints() const {
  return getImpl<detail::ParameterStudyAlgorithmOptions_Impl>()->listOfPoints();
}

std::vector<int> ParameterStudyAlgorithmOptions::stepsPerVariable() const {
  return getImpl<detail::ParameterStudyAlgorithmOptions_Impl>()->stepsPerVariable();
}

std::vector<int> ParameterStudyAlgorithmOptions::partitions() const {
  return getImpl<detail::ParameterStudyAlgorithmOptions_Impl>()->partitions();
}

void ParameterStudyAlgorithmOptions::setFinalPoint(const std::vector<double>& value) {
  return getImpl<detail::ParameterStudyAlgorithmOptions_Impl>()->setFinalPoint(value);
}

void ParameterStudyAlgorithmOptions::setStepVector(const std::vector<double>& value) {
  return getImpl<detail::ParameterStudyAlgorithmOptions_Impl>()->setStepVector(value);
}

bool ParameterStudyAlgorithmOptions::setNumSteps(int value) {
  return getImpl<detail::ParameterStudyAlgorithmOptions_Impl>()->setNumSteps(value);
}

void ParameterStudyAlgorithmOptions::setListOfPoints(const std::vector<double>& value) {
  return getImpl<detail::ParameterStudyAlgorithmOptions_Impl>()->setListOfPoints(value);
}

bool ParameterStudyAlgorithmOptions::setStepsPerVariable(const std::vector<int>& value) {
  return getImpl<detail::ParameterStudyAlgorithmOptions_Impl>()->setStepsPerVariable(value);
}

bool ParameterStudyAlgorithmOptions::setPartitions(const std::vector<int>& value) {
  return getImpl<detail::ParameterStudyAlgorithmOptions_Impl>()->setPartitions(value);
}

void ParameterStudyAlgorithmOptions::clearFinalPoint() {
  return getImpl<detail::ParameterStudyAlgorithmOptions_Impl>()->clearFinalPoint();
}

void ParameterStudyAlgorithmOptions::clearStepVector() {
  return getImpl<detail::ParameterStudyAlgorithmOptions_Impl>()->clearStepVector();
}

void ParameterStudyAlgorithmOptions::clearNumSteps() {
  return getImpl<detail::ParameterStudyAlgorithmOptions_Impl>()->clearNumSteps();
}

void ParameterStudyAlgorithmOptions::clearListOfPoints() {
  return getImpl<detail::ParameterStudyAlgorithmOptions_Impl>()->clearListOfPoints();
}

void ParameterStudyAlgorithmOptions::clearStepsPerVariable() {
  return getImpl<detail::ParameterStudyAlgorithmOptions_Impl>()->clearStepsPerVariable();
}

void ParameterStudyAlgorithmOptions::clearPartitions() {
  return getImpl<detail::ParameterStudyAlgorithmOptions_Impl>()->clearPartitions();
}

/// @cond
ParameterStudyAlgorithmOptions::ParameterStudyAlgorithmOptions(boost::shared_ptr<detail::ParameterStudyAlgorithmOptions_Impl> impl)
  : DakotaAlgorithmOptions(impl)
{}
/// @endcond

} // analysis
} // openstudio

