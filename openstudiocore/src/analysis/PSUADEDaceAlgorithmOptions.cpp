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

#include <analysis/PSUADEDaceAlgorithmOptions.hpp>
#include <analysis/PSUADEDaceAlgorithmOptions_Impl.hpp>

#include <runmanager/lib/JSON.hpp>

#include <utilities/core/Json.hpp>
#include <utilities/core/Optional.hpp>

#include <boost/bind.hpp>

namespace openstudio {
namespace analysis {

namespace detail {

  PSUADEDaceAlgorithmOptions_Impl::PSUADEDaceAlgorithmOptions_Impl()
    : DakotaAlgorithmOptions_Impl()
  {}

  PSUADEDaceAlgorithmOptions_Impl::PSUADEDaceAlgorithmOptions_Impl(
      const std::vector<Attribute>& options)
    : DakotaAlgorithmOptions_Impl(options)
  {}

  AlgorithmOptions PSUADEDaceAlgorithmOptions_Impl::clone() const {
    boost::shared_ptr<PSUADEDaceAlgorithmOptions_Impl> impl(new PSUADEDaceAlgorithmOptions_Impl(*this));
    return PSUADEDaceAlgorithmOptions(impl);
  }

  boost::optional<int> PSUADEDaceAlgorithmOptions_Impl::seed() const {
    OptionalInt result;
    if (OptionalAttribute option = getOption("seed")) {
      result = option->valueAsInteger();
    }
    return result;
  }

  boost::optional<int> PSUADEDaceAlgorithmOptions_Impl::samples() const {
    OptionalInt result;
    if (OptionalAttribute option = getOption("samples")) {
      result = option->valueAsInteger();
    }
    return result;
  }

  boost::optional<int> PSUADEDaceAlgorithmOptions_Impl::partitions() const {
    OptionalInt result;
    if (OptionalAttribute option = getOption("partitions")) {
      result = option->valueAsInteger();
    }
    return result;
  }

  bool PSUADEDaceAlgorithmOptions_Impl::setSeed(int value) {
	  if (value < 1) {
      LOG(Warn,"Cannot set PSUADEDaceAlgorithmOptions seed to a value less than one.");
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

  bool PSUADEDaceAlgorithmOptions_Impl::setSamples(int value) {
	  if (value < 1) {
      LOG(Warn,"Cannot set PSUADEDaceAlgorithmOptions samples to a value less than one.");
      return false;
	  }
    OptionalAttribute option;
    if ((option = getOption("samples"))) {
      option->setValue(value);
    }
    else {
      option = Attribute("samples",value);
      saveOption(*option);
    }
    return true;
  }

  bool PSUADEDaceAlgorithmOptions_Impl::setPartitions(int value) {
	  if (value < 1) {
      LOG(Warn,"Cannot set PSUADEDaceAlgorithmOptions partitions to a value less than one.");
      return false;
	  }
    OptionalAttribute option;
    if ((option = getOption("partitions"))) {
      option->setValue(value);
    }
    else {
      option = Attribute("partitions",value);
      saveOption(*option);
    }
    return true;
  }  

  void PSUADEDaceAlgorithmOptions_Impl::clearSeed() {
    clearOption("seed");
  }

  void PSUADEDaceAlgorithmOptions_Impl::clearSamples() {
    clearOption("samples");
  }

  void PSUADEDaceAlgorithmOptions_Impl::clearPartitions() {
    clearOption("partitions");
  }

  PSUADEDaceAlgorithmOptions PSUADEDaceAlgorithmOptions_Impl::fromVariant(const QVariant& variant,
                                                                          const VersionString& version)
  {
    QVariantMap map = variant.toMap();
    AttributeVector attributes = deserializeUnorderedVector(
          map["attributes"].toList(),
          boost::function<Attribute (const QVariant&)>(boost::bind(openstudio::detail::toAttribute,_1,version)));
    return PSUADEDaceAlgorithmOptions(attributes);
  }

} // detail

PSUADEDaceAlgorithmOptions::PSUADEDaceAlgorithmOptions()
  : DakotaAlgorithmOptions(boost::shared_ptr<detail::PSUADEDaceAlgorithmOptions_Impl>(
        new detail::PSUADEDaceAlgorithmOptions_Impl()))
{}

PSUADEDaceAlgorithmOptions::PSUADEDaceAlgorithmOptions(const std::vector<Attribute>& options)
  : DakotaAlgorithmOptions(boost::shared_ptr<detail::PSUADEDaceAlgorithmOptions_Impl>(
        new detail::PSUADEDaceAlgorithmOptions_Impl(options)))
{}

boost::optional<int> PSUADEDaceAlgorithmOptions::seed() const {
  return getImpl<detail::PSUADEDaceAlgorithmOptions_Impl>()->seed();
}

boost::optional<int> PSUADEDaceAlgorithmOptions::samples() const {
  return getImpl<detail::PSUADEDaceAlgorithmOptions_Impl>()->samples();
}

boost::optional<int> PSUADEDaceAlgorithmOptions::partitions() const {
  return getImpl<detail::PSUADEDaceAlgorithmOptions_Impl>()->partitions();
}

bool PSUADEDaceAlgorithmOptions::setSeed(int value) {
  return getImpl<detail::PSUADEDaceAlgorithmOptions_Impl>()->setSeed(value);
}

bool PSUADEDaceAlgorithmOptions::setSamples(int value) {
  return getImpl<detail::PSUADEDaceAlgorithmOptions_Impl>()->setSamples(value);
}

bool PSUADEDaceAlgorithmOptions::setPartitions(int value) {
  return getImpl<detail::PSUADEDaceAlgorithmOptions_Impl>()->setPartitions(value);
}

void PSUADEDaceAlgorithmOptions::clearSeed() {
  return getImpl<detail::PSUADEDaceAlgorithmOptions_Impl>()->clearSeed();
}

void PSUADEDaceAlgorithmOptions::clearSamples() {
  return getImpl<detail::PSUADEDaceAlgorithmOptions_Impl>()->clearSamples();
}

void PSUADEDaceAlgorithmOptions::clearPartitions() {
  return getImpl<detail::PSUADEDaceAlgorithmOptions_Impl>()->clearPartitions();
}

/// @cond
PSUADEDaceAlgorithmOptions::PSUADEDaceAlgorithmOptions(boost::shared_ptr<detail::PSUADEDaceAlgorithmOptions_Impl> impl)
  : DakotaAlgorithmOptions(impl)
{}

/// @endcond

} // analysis
} // openstudio

