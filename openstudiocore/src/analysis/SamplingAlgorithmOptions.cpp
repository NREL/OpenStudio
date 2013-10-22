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

#include <analysis/SamplingAlgorithmOptions.hpp>
#include <analysis/SamplingAlgorithmOptions_Impl.hpp>

#include <utilities/core/Assert.hpp>
#include <utilities/core/Json.hpp>
#include <utilities/core/Optional.hpp>

#include <boost/bind.hpp>

namespace openstudio {
namespace analysis {

namespace detail {

  SamplingAlgorithmOptions_Impl::SamplingAlgorithmOptions_Impl() : DakotaAlgorithmOptions_Impl()
  {
    saveOption(Attribute("samples",5));
    saveOption(Attribute("allVariables",false));
    saveOption(Attribute("varianceBasedDecomp",false));
    saveOption(Attribute("dropTolerance",false));
    saveOption(Attribute("fixedSeed",false));
    setSeed(std::max<int>(rand(),1));
  }

  SamplingAlgorithmOptions_Impl::SamplingAlgorithmOptions_Impl(
      const boost::optional<SamplingAlgorithmSampleType>& sampleType,
      const boost::optional<SamplingAlgorithmRNGType>& rngType,
      const std::vector<Attribute>& options)
    : DakotaAlgorithmOptions_Impl(options),
      m_sampleType(sampleType),
      m_rngType(rngType)
  {}

  AlgorithmOptions SamplingAlgorithmOptions_Impl::clone() const {
    boost::shared_ptr<SamplingAlgorithmOptions_Impl> impl(new SamplingAlgorithmOptions_Impl(*this));
    return SamplingAlgorithmOptions(impl);
  }

  boost::optional<SamplingAlgorithmSampleType> SamplingAlgorithmOptions_Impl::sampleType() const {
    return m_sampleType;
  }

  int SamplingAlgorithmOptions_Impl::samples() const {
    int result;
    OptionalAttribute option = getOption("samples");
    OS_ASSERT(option);
    result = option->valueAsInteger();
    return result;
  }

  bool SamplingAlgorithmOptions_Impl::allVariables() const {
    bool result;
    OptionalAttribute option = getOption("allVariables");
    OS_ASSERT(option);
    result = option->valueAsBoolean();
    return result;
  }

  bool SamplingAlgorithmOptions_Impl::varianceBasedDecomp() const {
    bool result;
    OptionalAttribute option = getOption("varianceBasedDecomp");
    OS_ASSERT(option);
    result = option->valueAsBoolean();
    return result;
  }

  bool SamplingAlgorithmOptions_Impl::dropTolerance() const {
    bool result;
    OptionalAttribute option = getOption("dropTolerance");
    OS_ASSERT(option);
    result = option->valueAsBoolean();
    return result;
  }

  boost::optional<int> SamplingAlgorithmOptions_Impl::seed() const {
    OptionalInt result;
    if (OptionalAttribute option = getOption("seed")) {
      result = option->valueAsInteger();
    }
    return result;
  }

  bool SamplingAlgorithmOptions_Impl::fixedSeed() const {
    bool result;
    OptionalAttribute option = getOption("fixedSeed");
    OS_ASSERT(option);
    result = option->valueAsBoolean();
    return result;
  }

  boost::optional<SamplingAlgorithmRNGType> SamplingAlgorithmOptions_Impl::rngType() const {
    return m_rngType;
  }

  bool SamplingAlgorithmOptions_Impl::setSamples(int value) {
	  if (value < 1) {
      LOG(Warn,"Cannot set SamplingAlgorithmOptions samples to a value less than one.");
      return false;
	  }
    OptionalAttribute option = getOption("samples");
    OS_ASSERT(option);
    option->setValue(value);
    return true;
  }

  void SamplingAlgorithmOptions_Impl::setAllVariables(bool value) {
    OptionalAttribute option = getOption("allVariables");
    OS_ASSERT(option);
    option->setValue(value);
  }

  void SamplingAlgorithmOptions_Impl::setVarianceBasedDecomp(bool value) {
    OptionalAttribute option = getOption("varianceBasedDecomp");
    OS_ASSERT(option);
    option->setValue(value);
  }

  void SamplingAlgorithmOptions_Impl::setDropTolerance(bool value) {
    OptionalAttribute option = getOption("dropTolerance");
    OS_ASSERT(option);
    option->setValue(value);
  }

  bool SamplingAlgorithmOptions_Impl::setSeed(int value) {
	if (value < 1) {
      LOG(Warn,"Cannot set SamplingAlgorithmOptions seed to a value less than one.");
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

  void SamplingAlgorithmOptions_Impl::setFixedSeed(bool value) {
    OptionalAttribute option = getOption("fixedSeed");
    OS_ASSERT(option);
    option->setValue(value);
  }

  void SamplingAlgorithmOptions_Impl::setSampleType(SamplingAlgorithmSampleType value) {
    m_sampleType = value;
  }

  void SamplingAlgorithmOptions_Impl::setRNGType(SamplingAlgorithmRNGType value) {
    m_rngType = value;
  }

  void SamplingAlgorithmOptions_Impl::clearSeed() {
    clearOption("seed");
  }

  void SamplingAlgorithmOptions_Impl::clearSampleType() {
    m_sampleType.reset();
  }

  void SamplingAlgorithmOptions_Impl::clearRNGType() {
    m_rngType.reset();
  }

  QVariant SamplingAlgorithmOptions_Impl::toVariant() const {
    QVariantMap map = AlgorithmOptions_Impl::toVariant().toMap();

    if (OptionalSamplingAlgorithmSampleType st = sampleType()) {
      map["sample_type"] = toQString(st->valueName());
    }
    if (OptionalSamplingAlgorithmRNGType rt = rngType()) {
      map["rng_type"] = toQString(rt->valueName());
    }

    return QVariant(map);
  }

  SamplingAlgorithmOptions SamplingAlgorithmOptions_Impl::fromVariant(const QVariant& variant,
                                                                      const VersionString& version)
  {
    QVariantMap map = variant.toMap();
    AttributeVector attributes = deserializeUnorderedVector(
          map["attributes"].toList(),
          boost::function<Attribute (const QVariant&)>(boost::bind(openstudio::detail::toAttribute,_1,version)));
    return SamplingAlgorithmOptions(
          map.contains("sample_type") ? SamplingAlgorithmSampleType(map["sample_type"].toString().toStdString()) : OptionalSamplingAlgorithmSampleType(),
          map.contains("rng_type") ? SamplingAlgorithmRNGType(map["rng_type"].toString().toStdString()) : OptionalSamplingAlgorithmRNGType(),
          attributes);
  }

} // detail

SamplingAlgorithmOptions::SamplingAlgorithmOptions()
  : DakotaAlgorithmOptions(boost::shared_ptr<detail::SamplingAlgorithmOptions_Impl>(
        new detail::SamplingAlgorithmOptions_Impl()))
{}

SamplingAlgorithmOptions::SamplingAlgorithmOptions(const boost::optional<SamplingAlgorithmSampleType>& sampleType,
                                                   const boost::optional<SamplingAlgorithmRNGType>& rngType,
                                                   const std::vector<Attribute>& options)
  : DakotaAlgorithmOptions(boost::shared_ptr<detail::SamplingAlgorithmOptions_Impl>(
        new detail::SamplingAlgorithmOptions_Impl(sampleType,rngType,options)))
{}

boost::optional<SamplingAlgorithmSampleType> SamplingAlgorithmOptions::sampleType() const {
  return getImpl<detail::SamplingAlgorithmOptions_Impl>()->sampleType();
}

int SamplingAlgorithmOptions::samples() const {
  return getImpl<detail::SamplingAlgorithmOptions_Impl>()->samples();
}

bool SamplingAlgorithmOptions::allVariables() const {
  return getImpl<detail::SamplingAlgorithmOptions_Impl>()->allVariables();
}

bool SamplingAlgorithmOptions::varianceBasedDecomp() const {
  return getImpl<detail::SamplingAlgorithmOptions_Impl>()->varianceBasedDecomp();
}

bool SamplingAlgorithmOptions::dropTolerance() const {
  return getImpl<detail::SamplingAlgorithmOptions_Impl>()->dropTolerance();
}

boost::optional<int> SamplingAlgorithmOptions::seed() const {
  return getImpl<detail::SamplingAlgorithmOptions_Impl>()->seed();
}

bool SamplingAlgorithmOptions::fixedSeed() const {
  return getImpl<detail::SamplingAlgorithmOptions_Impl>()->fixedSeed();
}

boost::optional<SamplingAlgorithmRNGType> SamplingAlgorithmOptions::rngType() const {
  return getImpl<detail::SamplingAlgorithmOptions_Impl>()->rngType();
}

bool SamplingAlgorithmOptions::setSamples(int value) {
  return getImpl<detail::SamplingAlgorithmOptions_Impl>()->setSamples(value);
}

void SamplingAlgorithmOptions::setAllVariables(bool value) {
  return getImpl<detail::SamplingAlgorithmOptions_Impl>()->setAllVariables(value);
}


void SamplingAlgorithmOptions::setVarianceBasedDecomp(bool value) {
  return getImpl<detail::SamplingAlgorithmOptions_Impl>()->setVarianceBasedDecomp(value);
}

void SamplingAlgorithmOptions::setDropTolerance(bool value) {
  return getImpl<detail::SamplingAlgorithmOptions_Impl>()->setDropTolerance(value);
}

bool SamplingAlgorithmOptions::setSeed(int value) {
  return getImpl<detail::SamplingAlgorithmOptions_Impl>()->setSeed(value);
}

void SamplingAlgorithmOptions::setFixedSeed(bool value) {
  return getImpl<detail::SamplingAlgorithmOptions_Impl>()->setFixedSeed(value);
}

void SamplingAlgorithmOptions::setSampleType(SamplingAlgorithmSampleType value) {
  getImpl<detail::SamplingAlgorithmOptions_Impl>()->setSampleType(value);
}

void SamplingAlgorithmOptions::setRNGType(SamplingAlgorithmRNGType value) {
  getImpl<detail::SamplingAlgorithmOptions_Impl>()->setRNGType(value);
}

void SamplingAlgorithmOptions::clearSeed() {
  return getImpl<detail::SamplingAlgorithmOptions_Impl>()->clearSeed();
}

void SamplingAlgorithmOptions::clearRNGType() {
  return getImpl<detail::SamplingAlgorithmOptions_Impl>()->clearRNGType();
}

void SamplingAlgorithmOptions::clearSampleType() {
  return getImpl<detail::SamplingAlgorithmOptions_Impl>()->clearSampleType();
}

/// @cond
SamplingAlgorithmOptions::SamplingAlgorithmOptions(boost::shared_ptr<detail::SamplingAlgorithmOptions_Impl> impl)
  : DakotaAlgorithmOptions(impl)
{}

/// @endcond

} // analysis
} // openstudio

