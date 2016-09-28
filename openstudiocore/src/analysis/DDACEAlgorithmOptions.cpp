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

#include "DDACEAlgorithmOptions.hpp"
#include "DDACEAlgorithmOptions_Impl.hpp"

#include "Problem.hpp"

#include "../utilities/math/Primes.hpp"

#include "../utilities/core/Json.hpp"
#include "../utilities/core/Optional.hpp"

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
    std::shared_ptr<DDACEAlgorithmOptions_Impl> impl(new DDACEAlgorithmOptions_Impl(*this));
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
          std::function<Attribute (const QVariant&)>(std::bind(openstudio::detail::toAttribute,std::placeholders::_1,version)));
    return DDACEAlgorithmOptions(map["ddace_algorithm_type"].toString().toStdString(),
                                 attributes);
  }

} // detail

DDACEAlgorithmOptions::DDACEAlgorithmOptions(const DDACEAlgorithmType& algorithmType)
  : DakotaAlgorithmOptions(std::shared_ptr<detail::DDACEAlgorithmOptions_Impl>(
        new detail::DDACEAlgorithmOptions_Impl(algorithmType)))
{}

DDACEAlgorithmOptions::DDACEAlgorithmOptions(const DDACEAlgorithmType& algorithmType,
                                             const std::vector<Attribute>& options)
  : DakotaAlgorithmOptions(std::shared_ptr<detail::DDACEAlgorithmOptions_Impl>(
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
DDACEAlgorithmOptions::DDACEAlgorithmOptions(std::shared_ptr<detail::DDACEAlgorithmOptions_Impl> impl)
  : DakotaAlgorithmOptions(impl)
{}
/// @endcond

} // analysis
} // openstudio

