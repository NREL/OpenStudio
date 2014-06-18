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

#include "NormalDistribution.hpp"
#include "UncertaintyDescription_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace analysis {

NormalDistribution::NormalDistribution()
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
  new detail::UncertaintyDescription_Impl(NormalDistribution::type())))
{}

NormalDistribution::NormalDistribution(double mean, double standardDeviation) 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(NormalDistribution::type())))
{
  setMean(mean);
  setStandardDeviation(standardDeviation);
}

UncertaintyDescriptionType NormalDistribution::type() {
  return UncertaintyDescriptionType(UncertaintyDescriptionType::normal_uncertain);
}

double NormalDistribution::mean() const {
  return impl()->getAttribute("means",false).valueAsDouble();
}

double NormalDistribution::standardDeviation() const {
  return impl()->getAttribute("std_deviations",false).valueAsDouble();
}

boost::optional<double> NormalDistribution::lowerBound() const {
  if (impl()->isSet("lower_bounds")) {
    return impl()->getAttribute("lower_bounds",false).valueAsDouble();
  }
  return boost::none;
}

boost::optional<double> NormalDistribution::upperBound() const {
  if (impl()->isSet("upper_bounds")) {
    return impl()->getAttribute("upper_bounds",false).valueAsDouble();
  }
  return boost::none;
}

void NormalDistribution::setMean(double value) {
  impl()->setAttribute(Attribute("means",value),false);
}

bool NormalDistribution::setStandardDeviation(double value) {
  if (value > 0) {
    impl()->setAttribute(Attribute("std_deviations",value),false);
    return true;
  }
  else {
    LOG(Warn,"The standard deviation of a normal distribution must be positive.");
    return false;
  }
}

void NormalDistribution::setLowerBound(double value) {
  impl()->setAttribute(Attribute("lower_bounds",value),false);
}

void NormalDistribution::resetLowerBound() {
  impl()->clearAttribute("lower_bounds",false);
}

void NormalDistribution::setUpperBound(double value) {
  impl()->setAttribute(Attribute("upper_bounds",value),false);
}

void NormalDistribution::resetUpperBound() {
  impl()->clearAttribute("upper_bounds",false);
}

NormalDistribution::NormalDistribution(std::shared_ptr<detail::UncertaintyDescription_Impl> impl)
  : UncertaintyDescription(impl)
{
  OS_ASSERT(type() == NormalDistribution::type());
}

} // analysis
} // openstudio
