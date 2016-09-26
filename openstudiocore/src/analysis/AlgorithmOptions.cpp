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

#include "AlgorithmOptions.hpp"
#include "AlgorithmOptions_Impl.hpp"

#include "../utilities/core/Assert.hpp"
#include "../utilities/core/Compare.hpp"
#include "../utilities/core/Finder.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  AlgorithmOptions_Impl::AlgorithmOptions_Impl()
  {}

  AlgorithmOptions_Impl::AlgorithmOptions_Impl(const std::vector<Attribute>& options)
    : m_options(options)
  {}

  AlgorithmOptions_Impl::AlgorithmOptions_Impl(const AlgorithmOptions_Impl& other)
  {
    for (const Attribute& option : other.m_options) {
      Attribute optionClone = option.clone();
      m_options.push_back(optionClone);
    }
  }

  AlgorithmOptions AlgorithmOptions_Impl::clone() const {
    std::shared_ptr<AlgorithmOptions_Impl> impl(new AlgorithmOptions_Impl(*this));
    return AlgorithmOptions(impl);
  }

  boost::optional<int> AlgorithmOptions_Impl::maxIter() const {
    OptionalInt result;
    if (OptionalAttribute maxIter = getOption("maxIter")) {
      result = maxIter->valueAsInteger();
    }
    return result;
  }

  boost::optional<int> AlgorithmOptions_Impl::maxSims() const {
    OptionalInt result;
    if (OptionalAttribute maxSims = getOption("maxSims")) {
      result = maxSims->valueAsInteger();
    }
    return result;
  }

  std::vector<Attribute> AlgorithmOptions_Impl::options() const {
    return m_options;
  }

  boost::optional<Attribute> AlgorithmOptions_Impl::getOption(const std::string& name) const {
    return findByName<Attribute>(m_options,name,true);
  }

  void AlgorithmOptions_Impl::setMaxIter(int maxIter) {
    OptionalAttribute maxIterA = getOption("maxIter");
    if (maxIterA) {
      if (OptionalAlgorithm algorithm = this->algorithm()) {
        int oldValue = maxIterA->valueAsInteger();
        if (maxIter > oldValue) {
          algorithm->resetComplete();
        }
      }
      maxIterA->setValue(maxIter);
    }
    else {
      maxIterA = Attribute("maxIter",maxIter);
      saveOption(*maxIterA);
    }
  }

  void AlgorithmOptions_Impl::setMaxSims(int maxSims) {
    OptionalAttribute maxSimsA = getOption("maxSims");
    if (maxSimsA) {
      if (OptionalAlgorithm algorithm = this->algorithm()) {
        int oldValue = maxSimsA->valueAsInteger();
        if (maxSims > oldValue) {
          algorithm->resetComplete();
        }
      }
      maxSimsA->setValue(maxSims);
    }
    else {
      maxSimsA = Attribute("maxSims",maxSims);
      saveOption(*maxSimsA);
    }
  }

  void AlgorithmOptions_Impl::saveOption(const Attribute& attribute) {
    OS_ASSERT(!getOption(attribute.name()));
    m_options.push_back(attribute);
  }

  void AlgorithmOptions_Impl::clearMaxIter() {
    if (OptionalAlgorithm algorithm = this->algorithm()) {
      algorithm->resetComplete();
    }
    clearOption("maxIter");
  }

  void AlgorithmOptions_Impl::clearMaxSims() {
    if (OptionalAlgorithm algorithm = this->algorithm()) {
      algorithm->resetComplete();
    }
    clearOption("maxSims");
  }

  void AlgorithmOptions_Impl::clearOption(const std::string& name) {
    auto it = findIteratorByName(m_options,name,true);
    if (it != m_options.end()) {
      m_options.erase(it);
    }
  }

  boost::optional<Algorithm> AlgorithmOptions_Impl::algorithm() const {
    return m_algorithm;
  }

  void AlgorithmOptions_Impl::setAlgorithm(const Algorithm& algorithm) {
    m_algorithm = algorithm;
  }

  QVariant AlgorithmOptions_Impl::toVariant() const {
    QVariantMap map;

    QVariantList attributesList;
    for (const Attribute& option : options()) {
      attributesList.push_back(openstudio::detail::toVariant(option));
    }
    map["attributes"] = attributesList;

    return QVariant(map);
  }

} // detail

AlgorithmOptions::AlgorithmOptions()
  : m_impl(std::shared_ptr<detail::AlgorithmOptions_Impl>(
        new detail::AlgorithmOptions_Impl()))
{}

AlgorithmOptions AlgorithmOptions::clone() const {
  return m_impl->clone();
}

boost::optional<int> AlgorithmOptions::maxIter() const {
  return getImpl<detail::AlgorithmOptions_Impl>()->maxIter();
}

boost::optional<int> AlgorithmOptions::maxSims() const {
  return getImpl<detail::AlgorithmOptions_Impl>()->maxSims();
}

std::vector<Attribute> AlgorithmOptions::options() const {
  return getImpl<detail::AlgorithmOptions_Impl>()->options();
}

void AlgorithmOptions::setMaxIter(int maxIter) {
  getImpl<detail::AlgorithmOptions_Impl>()->setMaxIter(maxIter);
}

void AlgorithmOptions::setMaxSims(int maxSims) {
  getImpl<detail::AlgorithmOptions_Impl>()->setMaxSims(maxSims);
}

void AlgorithmOptions::clearMaxIter() {
  getImpl<detail::AlgorithmOptions_Impl>()->clearMaxIter();
}

void AlgorithmOptions::clearMaxSims() {
  getImpl<detail::AlgorithmOptions_Impl>()->clearMaxSims();
}

/// @cond
AlgorithmOptions::AlgorithmOptions(std::shared_ptr<detail::AlgorithmOptions_Impl> impl)
  : m_impl(impl)
{}
/// @endcond

void AlgorithmOptions::setAlgorithm(const Algorithm& algorithm) {
  getImpl<detail::AlgorithmOptions_Impl>()->setAlgorithm(algorithm);
}

QVariant AlgorithmOptions::toVariant() const {
  return getImpl<detail::AlgorithmOptions_Impl>()->toVariant();
}

} // analysis
} // openstudio

