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

#include "PSUADEDaceAlgorithmOptions.hpp"
#include "PSUADEDaceAlgorithmOptions_Impl.hpp"

#include "../runmanager/lib/JSON.hpp"

#include "../utilities/core/Json.hpp"
#include "../utilities/core/Optional.hpp"

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
    std::shared_ptr<PSUADEDaceAlgorithmOptions_Impl> impl(new PSUADEDaceAlgorithmOptions_Impl(*this));
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
          std::function<Attribute (const QVariant&)>(std::bind(openstudio::detail::toAttribute,std::placeholders::_1,version)));
    return PSUADEDaceAlgorithmOptions(attributes);
  }

} // detail

PSUADEDaceAlgorithmOptions::PSUADEDaceAlgorithmOptions()
  : DakotaAlgorithmOptions(std::shared_ptr<detail::PSUADEDaceAlgorithmOptions_Impl>(
        new detail::PSUADEDaceAlgorithmOptions_Impl()))
{}

PSUADEDaceAlgorithmOptions::PSUADEDaceAlgorithmOptions(const std::vector<Attribute>& options)
  : DakotaAlgorithmOptions(std::shared_ptr<detail::PSUADEDaceAlgorithmOptions_Impl>(
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
PSUADEDaceAlgorithmOptions::PSUADEDaceAlgorithmOptions(std::shared_ptr<detail::PSUADEDaceAlgorithmOptions_Impl> impl)
  : DakotaAlgorithmOptions(impl)
{}

/// @endcond

} // analysis
} // openstudio

