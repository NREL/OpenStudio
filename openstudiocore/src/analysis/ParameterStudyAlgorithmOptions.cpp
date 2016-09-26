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

#include "ParameterStudyAlgorithmOptions.hpp"
#include "ParameterStudyAlgorithmOptions_Impl.hpp"

#include "Problem.hpp"

#include "../utilities/core/Json.hpp"

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
    std::shared_ptr<ParameterStudyAlgorithmOptions_Impl> impl(
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
          std::function<Attribute (const QVariant&)>(std::bind(openstudio::detail::toAttribute,std::placeholders::_1,version)));
    return ParameterStudyAlgorithmOptions(map["parameter_study_algorithm_type"].toString().toStdString(),
                                          attributes);
  }

} // detail


ParameterStudyAlgorithmOptions::ParameterStudyAlgorithmOptions(
    const ParameterStudyAlgorithmType& algorithmType)
  : DakotaAlgorithmOptions(std::shared_ptr<detail::ParameterStudyAlgorithmOptions_Impl>(
        new detail::ParameterStudyAlgorithmOptions_Impl(algorithmType)))
{}

ParameterStudyAlgorithmOptions::ParameterStudyAlgorithmOptions(
    const ParameterStudyAlgorithmType& algorithmType,
    const std::vector<Attribute>& options)
  : DakotaAlgorithmOptions(std::shared_ptr<detail::ParameterStudyAlgorithmOptions_Impl> (
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
ParameterStudyAlgorithmOptions::ParameterStudyAlgorithmOptions(std::shared_ptr<detail::ParameterStudyAlgorithmOptions_Impl> impl)
  : DakotaAlgorithmOptions(impl)
{}
/// @endcond

} // analysis
} // openstudio

