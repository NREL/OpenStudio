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

#include <analysis/DDACEAlgorithmOptions.hpp>
#include <analysis/DDACEAlgorithmOptions_Impl.hpp>

#include <analysis/Problem.hpp>

#include <utilities/math/Primes.hpp>

#include <utilities/core/Json.hpp>
#include <utilities/core/Optional.hpp>

#include <stdlib.h>

namespace openstudio {
namespace analysis {

namespace detail {

  DDACEAlgorithmOptions_Impl::DDACEAlgorithmOptions_Impl(const DDACEAlgorithmType& algorithmType)
    : DakotaAlgorithmOptions_Impl(), m_algorithmType(algorithmType)
  {
    setSeed(std::max<int>(rand(),1));
  }

  DDACEAlgorithmOptions_Impl::DDACEAlgorithmOptions_Impl(const DDACEAlgorithmType& algorithmType,
                                                         const std::vector<Attribute>& options)
    : DakotaAlgorithmOptions_Impl(options), m_algorithmType(algorithmType)
  {}

  AlgorithmOptions DDACEAlgorithmOptions_Impl::clone() const {
    boost::shared_ptr<DDACEAlgorithmOptions_Impl> impl(new DDACEAlgorithmOptions_Impl(*this));
    return DDACEAlgorithmOptions(impl);
  }

  DDACEAlgorithmType DDACEAlgorithmOptions_Impl::algorithmType() const {
    return m_algorithmType;
  }

  boost::optional<int> DDACEAlgorithmOptions_Impl::seed() const{
    OptionalInt result;
    if (OptionalAttribute option = getOption("seed")) {
      result = option->valueAsInteger();
    }
    return result;
  }

  boost::optional<int> DDACEAlgorithmOptions_Impl::samples() const {
    OptionalInt result;
    if (OptionalAttribute option = getOption("samples")) {
      result = option->valueAsInteger();
    }
    return result;
  }

  boost::optional<int> DDACEAlgorithmOptions_Impl::symbols() const {
    OptionalInt result;
    if (OptionalAttribute option = getOption("symbols")) {
      result = option->valueAsInteger();
    }
    return result;
  }

  void DDACEAlgorithmOptions_Impl::setSeed(int value){
    OptionalAttribute option;
    if ((option = getOption("seed"))) {
      option->setValue(value);
    }
    else {
      option = Attribute("seed",value);
      saveOption(*option);
    }
  }

  bool DDACEAlgorithmOptions_Impl::setSamples(int value) {
	  if (value < 0) {
      LOG(Warn,"Cannot set DDACEAlgorithmOptions samples to a value less than zero.");
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

  bool DDACEAlgorithmOptions_Impl::setSymbols(int value) {
    OptionalAttribute option;
    if ((option = getOption("symbols"))) {
      option->setValue(value);
    }
    else {
      option = Attribute("symbols",value);
      saveOption(*option);
    }
    return true;
  }

  bool DDACEAlgorithmOptions_Impl::setSamplesForGrid(int numGridPartitions,const Problem& problem) {
    if (algorithmType() != DDACEAlgorithmType::grid) {
      LOG(Info,"This method only applies to the grid algorithm type, but '" << algorithmType() 
          << "' is selected.");
      return false;
    }
    if (!problem.allVariablesAreContinuousOrStaticTransformations()) {
      LOG(Info,"Problem '" << problem.name() << "' has un-ignorable discrete variables, which "
          << "means that the grid algorithm cannot be applied.");
      return false;
    }
    bool result = setSymbols(numGridPartitions);
    result = result && setSamples(static_cast<int>(std::pow((double)numGridPartitions,problem.numContinuousVariables())));
    return result;
  }

  bool DDACEAlgorithmOptions_Impl::setSamplesAndSymbolsForOrthogonalArray(int symbols,
                                                                          int multiplier)
  {
    if (!((algorithmType() == DDACEAlgorithmType::oas) || 
          (algorithmType() == DDACEAlgorithmType::oa_lhs))) 
    {
      LOG(Info,"This method only applies to the oas and oas_lhs algorithm types, but '" 
          << algorithmType() << "' is selected.");
      return false;
    }

    bool result(false);

    if (!((symbols == 4) || isPrime(symbols))) {
      LOG(Info,"For orthogonal array sampling, symbols must be prime or equal to 4.");
      return result;
    }

    OptionalInt oldSymbols = this->symbols();
    result = setSymbols(symbols);

    if (result) {
      result = setSamples(multiplier * static_cast<int>(std::pow((double)symbols,2)));
      if (!result) {
        if (oldSymbols) {
          setSymbols(*oldSymbols);
        }
        else {
          clearSymbols();
        }
      }
    }

    return result;
  }

  void DDACEAlgorithmOptions_Impl::clearSeed() {
    clearOption("seed");
  }

  void DDACEAlgorithmOptions_Impl::clearSamples() {
    clearOption("samples");
  }

  void DDACEAlgorithmOptions_Impl::clearSymbols() {
    clearOption("symbols");
  }

  QVariant DDACEAlgorithmOptions_Impl::toVariant() const {
    QVariantMap map = AlgorithmOptions_Impl::toVariant().toMap();

    map["ddace_algorithm_type"] = toQString(algorithmType().valueName());

    return QVariant(map);
  }

  DDACEAlgorithmOptions DDACEAlgorithmOptions_Impl::fromVariant(const QVariant& variant,
                                                                const VersionString& version)
  {
    QVariantMap map = variant.toMap();
    AttributeVector attributes = deserializeUnorderedVector(
          map["attributes"].toList(),
          boost::function<Attribute (const QVariant&)>(boost::bind(openstudio::detail::toAttribute,_1,version)));
    return DDACEAlgorithmOptions(map["ddace_algorithm_type"].toString().toStdString(),
                                 attributes);
  }

} // detail

DDACEAlgorithmOptions::DDACEAlgorithmOptions(const DDACEAlgorithmType& algorithmType)
  : DakotaAlgorithmOptions(boost::shared_ptr<detail::DDACEAlgorithmOptions_Impl>(
        new detail::DDACEAlgorithmOptions_Impl(algorithmType)))
{}

DDACEAlgorithmOptions::DDACEAlgorithmOptions(const DDACEAlgorithmType& algorithmType,
                                             const std::vector<Attribute>& options)
  : DakotaAlgorithmOptions(boost::shared_ptr<detail::DDACEAlgorithmOptions_Impl>(
        new detail::DDACEAlgorithmOptions_Impl(algorithmType, options)))
{}

int DDACEAlgorithmOptions::samplesForCentralComposite(const Problem& problem) {
  if (!problem.allVariablesAreContinuousOrStaticTransformations()) {
    LOG(Info,"Problem '" << problem.name() << "' has un-ignorable discrete variables, which "
        << "means that the central composite algorithm cannot be applied.");
    return 0;
  }
  int n = problem.numContinuousVariables();
  return 1 + 2*n + static_cast<int>(std::pow(2.0,n));
}

int DDACEAlgorithmOptions::samplesForBoxBehnken(const Problem& problem) {
  if (!problem.allVariablesAreContinuousOrStaticTransformations()) {
    LOG(Info,"Problem '" << problem.name() << "' has un-ignorable discrete variables, which "
        << "means that the box-behnken algorithm cannot be applied.");
    return 0;
  }
  int n = problem.numContinuousVariables();
  return 1 + n * static_cast<int>(std::pow(2.0,n-1));
}

DDACEAlgorithmType DDACEAlgorithmOptions::algorithmType() const {
  return getImpl<detail::DDACEAlgorithmOptions_Impl>()->algorithmType();
}

boost::optional<int> DDACEAlgorithmOptions::seed() const {
  return getImpl<detail::DDACEAlgorithmOptions_Impl>()->seed();
}

boost::optional<int> DDACEAlgorithmOptions::samples() const {
  return getImpl<detail::DDACEAlgorithmOptions_Impl>()->samples();
}

boost::optional<int> DDACEAlgorithmOptions::symbols() const {
  return getImpl<detail::DDACEAlgorithmOptions_Impl>()->symbols();
}

void DDACEAlgorithmOptions::setSeed(int value) {
  getImpl<detail::DDACEAlgorithmOptions_Impl>()->setSeed(value);
}

bool DDACEAlgorithmOptions::setSamples(int value) {
  return getImpl<detail::DDACEAlgorithmOptions_Impl>()->setSamples(value);
}

bool DDACEAlgorithmOptions::setSymbols(int value) {
  return getImpl<detail::DDACEAlgorithmOptions_Impl>()->setSymbols(value);
}

bool DDACEAlgorithmOptions::setSamplesForGrid(int numGridPartitions,const Problem& problem) {
  return getImpl<detail::DDACEAlgorithmOptions_Impl>()->setSamplesForGrid(numGridPartitions,problem);
}

bool DDACEAlgorithmOptions::setSamplesAndSymbolsForOrthogonalArray(int symbols, int multiplier) {
  return getImpl<detail::DDACEAlgorithmOptions_Impl>()->setSamplesAndSymbolsForOrthogonalArray(symbols,multiplier);
}

void DDACEAlgorithmOptions::clearSeed() {
  getImpl<detail::DDACEAlgorithmOptions_Impl>()->clearSeed();
}

void DDACEAlgorithmOptions::clearSamples() {
  getImpl<detail::DDACEAlgorithmOptions_Impl>()->clearSamples();
}

void DDACEAlgorithmOptions::clearSymbols() {
  getImpl<detail::DDACEAlgorithmOptions_Impl>()->clearSymbols();
}

/// @cond
DDACEAlgorithmOptions::DDACEAlgorithmOptions(boost::shared_ptr<detail::DDACEAlgorithmOptions_Impl> impl)
  : DakotaAlgorithmOptions(impl)
{}
/// @endcond

} // analysis
} // openstudio

