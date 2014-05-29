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

#include "BinomialDistribution.hpp"
#include "UncertaintyDescription_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace analysis {

BinomialDistribution::BinomialDistribution() 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(BinomialDistribution::type())))
{}

BinomialDistribution::BinomialDistribution(double probabilityPerTrial, int numTrials) 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(BinomialDistribution::type())))
{
  setProbabilityPerTrial(probabilityPerTrial);
  setNumTrials(numTrials);
}

UncertaintyDescriptionType BinomialDistribution::type() {
  return UncertaintyDescriptionType(UncertaintyDescriptionType::binomial_uncertain);
}

double BinomialDistribution::probabilityPerTrial() const {
  return impl()->getAttribute("prob_per_trial",false).valueAsDouble();
}

int BinomialDistribution::numTrials() const {
  return impl()->getAttribute("num_trials",false).valueAsInteger();
}

bool BinomialDistribution::setProbabilityPerTrial(double value) {
  if (value < 0 || value > 1) {
    LOG(Warn,"Probability must be between 0 and 1.");
    return false;
  }
  else {
    impl()->setAttribute(Attribute("prob_per_trial",value),false);
    return true;
  }
}

bool BinomialDistribution::setNumTrials(int value) {
  if (value < 0) {
    LOG(Warn,"Number of trials (num_trials) must be nonnegative.");
    return false;
  }
  else {
    impl()->setAttribute(Attribute("num_trials",value),false);
    return true;
  }
}

BinomialDistribution::BinomialDistribution(std::shared_ptr<detail::UncertaintyDescription_Impl> impl)
  : UncertaintyDescription(impl)
{
  OS_ASSERT(type() == BinomialDistribution::type());
}

} // analysis
} // openstudio
