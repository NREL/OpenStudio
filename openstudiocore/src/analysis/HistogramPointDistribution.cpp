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

#include "HistogramPointDistribution.hpp"
#include "UncertaintyDescription_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace analysis {

HistogramPointDistribution::HistogramPointDistribution() 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(HistogramPointDistribution::type())))
{}

HistogramPointDistribution::HistogramPointDistribution(std::vector<double> abscissas, std::vector<double> counts) 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(HistogramPointDistribution::type())))
{
  setAbscissas(abscissas);
  setCounts(counts);
}

UncertaintyDescriptionType HistogramPointDistribution::type() {
  return UncertaintyDescriptionType(UncertaintyDescriptionType::histogram_point_uncertain);
}

std::vector<double> HistogramPointDistribution::abscissas() const {
  Attribute attribute = impl()->getAttribute("abscissas",false);
  return getDoubleVectorFromAttribute(attribute);
}

std::vector<double> HistogramPointDistribution::counts() const {
  Attribute attribute = impl()->getAttribute("counts",false);
  return getDoubleVectorFromAttribute(attribute);
}

boost::optional<int> HistogramPointDistribution::numPairs() const {
  if (impl()->isSet("num_pairs")) {
    return impl()->getAttribute("num_pairs",false).valueAsInteger();
  }
  return boost::none;
}

bool HistogramPointDistribution::setAbscissas(const std::vector<double>& value) {
  for(std::vector<double>::size_type i = 1; i != value.size(); i++) {
    double previousValue = value[i-1];
    double currentValue = value[i];
    if (previousValue >= currentValue) {
      LOG(Warn,"Abscissas values must be strictly increasing.");
      return false;
    }
  }
  impl()->setAttribute(createAttributeFromVector("abscissas",value),false);
  return true;
}

bool HistogramPointDistribution::setCounts(const std::vector<double>& value) {
  double minValue = *std::min_element(value.begin(), value.end());
  if (minValue <= 0) {
    LOG(Warn,"All values of counts must be positive.");
    return false;
  }
  else {
    impl()->setAttribute(createAttributeFromVector("counts",value),false);
    int numPairs = value.size();
    impl()->setAttribute(Attribute("num_pairs",numPairs),false);
    return true;
  }
}

HistogramPointDistribution::HistogramPointDistribution(std::shared_ptr<detail::UncertaintyDescription_Impl> impl)
  : UncertaintyDescription(impl)
{
  OS_ASSERT(type() == HistogramPointDistribution::type());
}

} // analysis
} // openstudio
