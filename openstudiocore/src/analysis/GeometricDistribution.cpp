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

#include "GeometricDistribution.hpp"
#include "UncertaintyDescription_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace analysis {

GeometricDistribution::GeometricDistribution() 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(GeometricDistribution::type())))
{}

GeometricDistribution::GeometricDistribution(double probabilityPerTrial) 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(GeometricDistribution::type())))
{
  setProbabilityPerTrial(probabilityPerTrial);
}

UncertaintyDescriptionType GeometricDistribution::type() {
  return UncertaintyDescriptionType(UncertaintyDescriptionType::geometric_uncertain);
}

double GeometricDistribution::probabilityPerTrial() const {
  return impl()->getAttribute("prob_per_trial",false).valueAsDouble();
}

bool GeometricDistribution::setProbabilityPerTrial(double value) {
  if (value < 0 || value > 1) {
    LOG(Warn,"Probability must be between 0 and 1.");
    return false;
  }
  else {
    impl()->setAttribute(Attribute("prob_per_trial",value),false);
    return true;
  }
}

GeometricDistribution::GeometricDistribution(std::shared_ptr<detail::UncertaintyDescription_Impl> impl)
  : UncertaintyDescription(impl)
{
  OS_ASSERT(type() == GeometricDistribution::type());
}

} // analysis
} // openstudio
