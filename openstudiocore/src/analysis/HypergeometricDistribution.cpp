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
