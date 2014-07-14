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

#include "BetaDistribution.hpp"
#include "UncertaintyDescription_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace analysis {

BetaDistribution::BetaDistribution() 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(BetaDistribution::type())))
{}

BetaDistribution::BetaDistribution(double alpha, double beta, double lowerBound, double upperBound) 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(BetaDistribution::type())))
{
  setAlpha(alpha);
  setBeta(beta);
  setLowerBound(lowerBound);
  setUpperBound(upperBound);
}

UncertaintyDescriptionType BetaDistribution::type() {
  return UncertaintyDescriptionType(UncertaintyDescriptionType::beta_uncertain);
}

double BetaDistribution::alpha() const {
  return impl()->getAttribute("alphas",false).valueAsDouble();
}

double BetaDistribution::beta() const {
  return impl()->getAttribute("betas",false).valueAsDouble();
}

double BetaDistribution::lowerBound() const {
  return impl()->getAttribute("lower_bounds",false).valueAsDouble();
}

double BetaDistribution::upperBound() const {
  return impl()->getAttribute("upper_bounds",false).valueAsDouble();
}

void BetaDistribution::setAlpha(double value) {
  impl()->setAttribute(Attribute("alphas",value),false);
}

void BetaDistribution::setBeta(double value) {
  impl()->setAttribute(Attribute("betas",value),false);
}

void BetaDistribution::setLowerBound(double value) {
  impl()->setAttribute(Attribute("lower_bounds",value),false);
}

void BetaDistribution::setUpperBound(double value) {
  impl()->setAttribute(Attribute("upper_bounds",value),false);
}

BetaDistribution::BetaDistribution(std::shared_ptr<detail::UncertaintyDescription_Impl> impl)
  : UncertaintyDescription(impl)
{
  OS_ASSERT(type() == BetaDistribution::type());
}

} // analysis
} // openstudio
