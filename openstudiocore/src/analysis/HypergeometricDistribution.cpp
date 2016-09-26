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

#include "HypergeometricDistribution.hpp"
#include "UncertaintyDescription_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace analysis {

HypergeometricDistribution::HypergeometricDistribution() 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(HypergeometricDistribution::type())))
{}

HypergeometricDistribution::HypergeometricDistribution(
  int totalPopulation, int selectedPopulation, int numDrawn) 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(HypergeometricDistribution::type())))
{
  setTotalPopulation(totalPopulation);
  setSelectedPopulation(selectedPopulation);
  setNumDrawn(numDrawn);
}

UncertaintyDescriptionType HypergeometricDistribution::type() {
  return UncertaintyDescriptionType(UncertaintyDescriptionType::hypergeometric_uncertain);
}

int HypergeometricDistribution::totalPopulation() const {
  return impl()->getAttribute("total_population",false).valueAsInteger();
}

int HypergeometricDistribution::selectedPopulation() const {
  return impl()->getAttribute("selected_population",false).valueAsInteger();
}

int HypergeometricDistribution::numDrawn() const {
  return impl()->getAttribute("num_drawn",false).valueAsInteger();
}

bool HypergeometricDistribution::setTotalPopulation(int value) {
  if (value < 0) {
    LOG(Warn,"Total population must be nonnegative.");
    return false;
  }
  else {
    impl()->setAttribute(Attribute("total_population",value),false);
    return true;
  }
}

bool HypergeometricDistribution::setSelectedPopulation(int value) {
  if (value < 0) {
    LOG(Warn,"Selected population must be nonnegative.");
    return false;
  }
  else {
    impl()->setAttribute(Attribute("selected_population",value),false);
    return true;
  }
}

bool HypergeometricDistribution::setNumDrawn(int value) {
  if (value < 0) {
    LOG(Warn,"Number drawn (num_drawn) must be nonnegative.");
    return false;
  }
  else {
    impl()->setAttribute(Attribute("num_drawn",value),false);
    return true;
  }
}

HypergeometricDistribution::HypergeometricDistribution(std::shared_ptr<detail::UncertaintyDescription_Impl> impl)
  : UncertaintyDescription(impl)
{
  OS_ASSERT(type() == HypergeometricDistribution::type());
}

} // analysis
} // openstudio
