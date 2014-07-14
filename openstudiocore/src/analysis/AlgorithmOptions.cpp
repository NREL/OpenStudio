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

