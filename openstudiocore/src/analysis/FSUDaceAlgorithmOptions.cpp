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

#include <analysis/FSUDaceAlgorithmOptions.hpp>
#include <analysis/FSUDaceAlgorithmOptions_Impl.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Json.hpp>
#include <utilities/core/Optional.hpp>

#include <boost/bind.hpp>

#include <stdlib.h>

namespace openstudio {
namespace analysis {

namespace detail {

  FSUDaceAlgorithmOptions_Impl::FSUDaceAlgorithmOptions_Impl(
    const FSUDaceAlgorithmType& algorithmType) : DakotaAlgorithmOptions_Impl(),
      m_algorithmType(algorithmType)
  {
    saveOption(Attribute("samples",10));
    saveOption(Attribute("fixedSequence",false));
    saveOption(Attribute("latinize",false));
    saveOption(Attribute("varianceBasedDecomp",false));
    saveOption(Attribute("qualityMetrics",false));
    saveOption(Attribute("fixedSeed",false));
    setSeed(std::max<int>(rand(),1));
  }

  FSUDaceAlgorithmOptions_Impl::FSUDaceAlgorithmOptions_Impl(
      const FSUDaceAlgorithmType& algorithmType,
      const boost::optional<FSUDaceCvtTrialType>& trialType,
      const std::vector<Attribute>& options)
    : DakotaAlgorithmOptions_Impl(options),
      m_algorithmType(algorithmType),
      m_trialType(trialType)
  {}

  AlgorithmOptions FSUDaceAlgorithmOptions_Impl::clone() const {
    boost::shared_ptr<FSUDaceAlgorithmOptions_Impl> impl(new FSUDaceAlgorithmOptions_Impl(*this));
    return FSUDaceAlgorithmOptions(impl);
  }

  FSUDaceAlgorithmType FSUDaceAlgorithmOptions_Impl::algorithmType() const {
    return m_algorithmType;
  }

  int FSUDaceAlgorithmOptions_Impl::samples() const {
    int result;
    OptionalAttribute option = getOption("samples");
    OS_ASSERT(option);
    result = option->valueAsInteger();
    return result;
  }

  std::vector<int> FSUDaceAlgorithmOptions_Impl::sequenceStart() const {
    std::vector<int> result;
    if (OptionalAttribute option = getOption("sequenceStart")) {
      result = getIntVectorFromAttribute(*option);
    }
    return result;
  }

  std::vector<int> FSUDaceAlgorithmOptions_Impl::sequenceLeap() const {
    std::vector<int> result;
    if (OptionalAttribute option = getOption("sequenceLeap")) {
      result = getIntVectorFromAttribute(*option);
    }
    return result;
  }

  bool FSUDaceAlgorithmOptions_Impl::fixedSequence() const {
    bool result;
    OptionalAttribute option = getOption("fixedSequence");
    OS_ASSERT(option);
    result = option->valueAsBoolean();
    return result;
  }

  bool FSUDaceAlgorithmOptions_Impl::latinize() const {
    bool result;
    OptionalAttribute option = getOption("latinize");
    OS_ASSERT(option);
    result = option->valueAsBoolean();
    return result;
  }

  bool FSUDaceAlgorithmOptions_Impl::varianceBasedDecomp() const {
    bool result;
    OptionalAttribute option = getOption("varianceBasedDecomp");
    OS_ASSERT(option);
    result = option->valueAsBoolean();
    return result;
  }

  bool FSUDaceAlgorithmOptions_Impl::qualityMetrics() const {
    bool result;
    OptionalAttribute option = getOption("qualityMetrics");
    OS_ASSERT(option);
    result = option->valueAsBoolean();
    return result;
  }

  boost::optional<int> FSUDaceAlgorithmOptions_Impl::seed() const {
    OptionalInt result;
    if (OptionalAttribute option = getOption("seed")) {
      result = option->valueAsInteger();
    }
    return result;
  }

  boost::optional<int> FSUDaceAlgorithmOptions_Impl::numTrials() const {
    OptionalInt result;
    if (OptionalAttribute option = getOption("numTrials")) {
      result = option->valueAsInteger();
    }
    return result;
  }

  bool FSUDaceAlgorithmOptions_Impl::fixedSeed() const {
    bool result;
    OptionalAttribute option = getOption("fixedSeed");
    OS_ASSERT(option);
    result = option->valueAsBoolean();
    return result;
  }

  boost::optional<FSUDaceCvtTrialType> FSUDaceAlgorithmOptions_Impl::trialType() const {
    return m_trialType;
  }

  bool FSUDaceAlgorithmOptions_Impl::setSamples(int value) {
	  if (value < 1) {
      LOG(Warn,"Cannot set FSUDaceAlgorithmOptions samples to a value less than one.");
      return false;
	  }
    OptionalAttribute option = getOption("samples");
    OS_ASSERT(option);
    option->setValue(value);
    return true;
  }

  bool FSUDaceAlgorithmOptions_Impl::setSequenceStart(const std::vector<int>& value) {
    int minValue = *std::min_element(value.begin(), value.end());
    if (minValue < 0) {
      LOG(Warn,"Cannot set FSUDaceAlgorithmOptions sequence start to a value less than zero.");
      return false;
	}
    Attribute option = createAttributeFromVector("sequenceStart",value);
    if (OptionalAttribute currentOption = getOption("sequenceStart")) {
      currentOption->setValue(option.valueAsAttributeVector());
    }
    else {
      saveOption(option);
    }
    return true;
  }

  bool FSUDaceAlgorithmOptions_Impl::setSequenceLeap(const std::vector<int>& value) {
    int minValue = *std::min_element(value.begin(), value.end());
    if (minValue < 1) {
      LOG(Warn,"Cannot set FSUDaceAlgorithmOptions sequence leap to a value less than one.");
      return false;
	}
    Attribute option = createAttributeFromVector("sequenceLeap",value);
    if (OptionalAttribute currentOption = getOption("sequenceLeap")) {
      currentOption->setValue(option.valueAsAttributeVector());
    }
    else {
      saveOption(option);
    }
    return true;
  }

  void FSUDaceAlgorithmOptions_Impl::setFixedSequence(bool value) {
    OptionalAttribute option = getOption("fixedSequence");
    OS_ASSERT(option);
    option->setValue(value);
  }

  void FSUDaceAlgorithmOptions_Impl::setLatinize(bool value) {
    OptionalAttribute option = getOption("latinize");
    OS_ASSERT(option);
    option->setValue(value);
  }

  void FSUDaceAlgorithmOptions_Impl::setVarianceBasedDecomp(bool value) {
    OptionalAttribute option = getOption("varianceBasedDecomp");
    OS_ASSERT(option);
    option->setValue(value);
  }

  void FSUDaceAlgorithmOptions_Impl::setQualityMetrics(bool value) {
    OptionalAttribute option = getOption("qualityMetrics");
    OS_ASSERT(option);
    option->setValue(value);
  }

  bool FSUDaceAlgorithmOptions_Impl::setSeed(int value) {
	if (value < 1) {
      LOG(Warn,"Cannot set FSUDaceAlgorithmOptions seed to a value less than one.");
      return false;
	}
    OptionalAttribute option;
    if ((option = getOption("seed"))) {
      option->setValue(value);
    }
    else {
      option = Attribute("seed",value);
      saveOption(*option);
    }
    return true;
  }

  bool FSUDaceAlgorithmOptions_Impl::setNumTrials(int value) {
	if (value < 1) {
      LOG(Warn,"Cannot set FSUDaceAlgorithmOptions number of trials to a value less than one.");
	return false;
	  }
    OptionalAttribute option;
    if ((option = getOption("numTrials"))) {
      option->setValue(value);
    }
    else {
      option = Attribute("numTrials",value);
      saveOption(*option);
    }
    return true;
  }

  void FSUDaceAlgorithmOptions_Impl::setFixedSeed(bool value) {
    OptionalAttribute option = getOption("fixedSeed");
    OS_ASSERT(option);
    option->setValue(value);
  }

  void FSUDaceAlgorithmOptions_Impl::setTrialType(FSUDaceCvtTrialType value) {
    m_trialType = value;
  }

  void FSUDaceAlgorithmOptions_Impl::clearSequenceStart() {
    clearOption("sequenceStart");
  }

  void FSUDaceAlgorithmOptions_Impl::clearSequenceLeap() {
    clearOption("sequenceLeap");
  }

  void FSUDaceAlgorithmOptions_Impl::clearSeed() {
    clearOption("seed");
  }

  void FSUDaceAlgorithmOptions_Impl::clearNumTrials() {
    clearOption("numTrials");
  }

  void FSUDaceAlgorithmOptions_Impl::clearTrialType() {
    m_trialType.reset();
  }

  QVariant FSUDaceAlgorithmOptions_Impl::toVariant() const {
    QVariantMap map = AlgorithmOptions_Impl::toVariant().toMap();

    map["fsu_dace_algorithm_type"] = toQString(algorithmType().valueName());
    if (OptionalFSUDaceCvtTrialType tt = trialType()) {
      map["trial_type"] = toQString(tt->valueName());
    }

    return QVariant(map);
  }

  FSUDaceAlgorithmOptions FSUDaceAlgorithmOptions_Impl::fromVariant(const QVariant& variant,
                                                                    const VersionString& version)
  {
    QVariantMap map = variant.toMap();
    AttributeVector attributes = deserializeUnorderedVector(
          map["attributes"].toList(),
          boost::function<Attribute (const QVariant&)>(boost::bind(openstudio::detail::toAttribute,_1,version)));
    return FSUDaceAlgorithmOptions(
          map["fsu_dace_algorithm_type"].toString().toStdString(),
          map.contains("trial_type") ? FSUDaceCvtTrialType(map["trial_type"].toString().toStdString()) : OptionalFSUDaceCvtTrialType(),
          attributes);
  }

} // detail

FSUDaceAlgorithmOptions::FSUDaceAlgorithmOptions(const FSUDaceAlgorithmType& algorithmType)
  : DakotaAlgorithmOptions(boost::shared_ptr<detail::FSUDaceAlgorithmOptions_Impl>(
        new detail::FSUDaceAlgorithmOptions_Impl(algorithmType)))
{}

FSUDaceAlgorithmOptions::FSUDaceAlgorithmOptions(const FSUDaceAlgorithmType& algorithmType,
                                                 const boost::optional<FSUDaceCvtTrialType>& trialType,
                                                 const std::vector<Attribute>& options)
  : DakotaAlgorithmOptions(boost::shared_ptr<detail::FSUDaceAlgorithmOptions_Impl>(
        new detail::FSUDaceAlgorithmOptions_Impl(algorithmType, trialType, options)))
{}

FSUDaceAlgorithmType FSUDaceAlgorithmOptions::algorithmType() const {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->algorithmType();
}

int FSUDaceAlgorithmOptions::samples() const {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->samples();
}

std::vector<int> FSUDaceAlgorithmOptions::sequenceStart() const {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->sequenceStart();
}

std::vector<int> FSUDaceAlgorithmOptions::sequenceLeap() const {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->sequenceLeap();
}

bool FSUDaceAlgorithmOptions::fixedSequence() const {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->fixedSequence();
}

bool FSUDaceAlgorithmOptions::latinize() const {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->latinize();
}
bool FSUDaceAlgorithmOptions::varianceBasedDecomp() const {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->varianceBasedDecomp();
}

bool FSUDaceAlgorithmOptions::qualityMetrics() const {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->qualityMetrics();
}

boost::optional<int> FSUDaceAlgorithmOptions::seed() const {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->seed();
}

boost::optional<int> FSUDaceAlgorithmOptions::numTrials() const {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->numTrials();
}

bool FSUDaceAlgorithmOptions::fixedSeed() const {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->fixedSeed();
}

boost::optional<FSUDaceCvtTrialType> FSUDaceAlgorithmOptions::trialType() const {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->trialType();
}

bool FSUDaceAlgorithmOptions::setSamples(int value) {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->setSamples(value);
}

bool FSUDaceAlgorithmOptions::setSequenceStart(const std::vector<int>& value) {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->setSequenceStart(value);
}

bool FSUDaceAlgorithmOptions::setSequenceLeap(const std::vector<int>& value) {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->setSequenceLeap(value);
}

void FSUDaceAlgorithmOptions::setFixedSequence(bool value) {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->setFixedSequence(value);
}

void FSUDaceAlgorithmOptions::setLatinize(bool value) {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->setLatinize(value);
}

void FSUDaceAlgorithmOptions::setVarianceBasedDecomp(bool value) {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->setVarianceBasedDecomp(value);
}

void FSUDaceAlgorithmOptions::setQualityMetrics(bool value) {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->setQualityMetrics(value);
}

bool FSUDaceAlgorithmOptions::setSeed(int value) {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->setSeed(value);
}

bool FSUDaceAlgorithmOptions::setNumTrials(int value) {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->setNumTrials(value);
}

void FSUDaceAlgorithmOptions::setFixedSeed(bool value) {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->setFixedSeed(value);
}

void FSUDaceAlgorithmOptions::setTrialType(FSUDaceCvtTrialType value) {
  getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->setTrialType(value);
}

void FSUDaceAlgorithmOptions::clearSequenceStart() {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->clearSequenceStart();
}

void FSUDaceAlgorithmOptions::clearSequenceLeap() {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->clearSequenceLeap();
}

void FSUDaceAlgorithmOptions::clearSeed() {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->clearSeed();
}

void FSUDaceAlgorithmOptions::clearNumTrials() {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->clearNumTrials();
}

void FSUDaceAlgorithmOptions::clearTrialType() {
  return getImpl<detail::FSUDaceAlgorithmOptions_Impl>()->clearTrialType();
}


/// @cond
FSUDaceAlgorithmOptions::FSUDaceAlgorithmOptions(boost::shared_ptr<detail::FSUDaceAlgorithmOptions_Impl> impl)
  : DakotaAlgorithmOptions(impl)
{}

/// @endcond

} // analysis
} // openstudio

