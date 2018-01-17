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

#include "NegativeBinomialDistribution.hpp"
#include "UncertaintyDescription_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace analysis {

NegativeBinomialDistribution::NegativeBinomialDistribution() 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(NegativeBinomialDistribution::type())))
{}

NegativeBinomialDistribution::NegativeBinomialDistribution(double probabilityPerTrial, int numTrials) 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(NegativeBinomialDistribution::type())))
{
  setProbabilityPerTrial(probabilityPerTrial);
  setNumTrials(numTrials);
}

UncertaintyDescriptionType NegativeBinomialDistribution::type() {
  return UncertaintyDescriptionType(UncertaintyDescriptionType::negative_binomial_uncertain);
}

double NegativeBinomialDistribution::probabilityPerTrial() const {
  return impl()->getAttribute("prob_per_trial",false).valueAsDouble();
}

int NegativeBinomialDistribution::numTrials() const {
  return impl()->getAttribute("num_trials",false).valueAsInteger();
}

bool NegativeBinomialDistribution::setProbabilityPerTrial(double value) {
  if (value < 0 || value > 1) {
    LOG(Warn,"Probability must be between 0 and 1.");
    return false;
  }
  else {
    impl()->setAttribute(Attribute("prob_per_trial",value),false);
    return true;
  }
}

bool NegativeBinomialDistribution::setNumTrials(int value) {
  if (value < 0) {
    LOG(Warn,"Number of trials (num_trials) must be nonnegative.");
    return false;
  }
  else {
    impl()->setAttribute(Attribute("num_trials",value),false);
    return true;
  }
}

NegativeBinomialDistribution::NegativeBinomialDistribution(std::shared_ptr<detail::UncertaintyDescription_Impl> impl)
  : UncertaintyDescription(impl)
{
  OS_ASSERT(type() == NegativeBinomialDistribution::type());
}

} // analysis
} // openstudio
