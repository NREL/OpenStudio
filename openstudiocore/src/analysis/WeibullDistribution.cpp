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

#include "WeibullDistribution.hpp"
#include "UncertaintyDescription_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace analysis {

WeibullDistribution::WeibullDistribution() 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(WeibullDistribution::type())))
{}

WeibullDistribution::WeibullDistribution(double alpha, double beta) 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(WeibullDistribution::type())))
{
  setAlpha(alpha);
  setBeta(beta);
}

UncertaintyDescriptionType WeibullDistribution::type() {
  return UncertaintyDescriptionType(UncertaintyDescriptionType::weibull_uncertain);
}

double WeibullDistribution::alpha() const {
  return impl()->getAttribute("alphas",false).valueAsDouble();
}

double WeibullDistribution::beta() const {
  return impl()->getAttribute("betas",false).valueAsDouble();
}

bool WeibullDistribution::setAlpha(double value) {
  if (value > 0) {
    impl()->setAttribute(Attribute("alphas",value),false);
    return true;
  }
  else {
    LOG(Warn,"The alphas of a weibull distribution must be positive.");
    return false;
  }
}

bool WeibullDistribution::setBeta(double value) {
  if (value > 0) {
    impl()->setAttribute(Attribute("betas",value),false);
    return true;
  }
  else {
    LOG(Warn,"The betas of a weibull distribution must be positive.");
    return false;
  }  
}

WeibullDistribution::WeibullDistribution(std::shared_ptr<detail::UncertaintyDescription_Impl> impl)
  : UncertaintyDescription(impl)
{
  OS_ASSERT(type() == WeibullDistribution::type());
}

} // analysis
} // openstudio
