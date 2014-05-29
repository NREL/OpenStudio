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

#include "UniformDistribution.hpp"
#include "UncertaintyDescription_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace analysis {

UniformDistribution::UniformDistribution()
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
  new detail::UncertaintyDescription_Impl(UniformDistribution::type())))
{}

UniformDistribution::UniformDistribution(double lowerBound, double upperBound) 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(UniformDistribution::type())))
{
  setLowerBound(lowerBound);
  setUpperBound(upperBound);
}

UncertaintyDescriptionType UniformDistribution::type() {
  return UncertaintyDescriptionType(UncertaintyDescriptionType::uniform_uncertain);
}

double UniformDistribution::lowerBound() const {
  return impl()->getAttribute("lower_bounds",false).valueAsDouble();
}

double UniformDistribution::upperBound() const {
  return impl()->getAttribute("upper_bounds",false).valueAsDouble();
}

void UniformDistribution::setLowerBound(double value) {
  impl()->setAttribute(Attribute("lower_bounds",value),false);
}

void UniformDistribution::setUpperBound(double value) {
  impl()->setAttribute(Attribute("upper_bounds",value),false);
}

UniformDistribution::UniformDistribution(std::shared_ptr<detail::UncertaintyDescription_Impl> impl)
  : UncertaintyDescription(impl)
{
  OS_ASSERT(type() == UniformDistribution::type());
}

} // analysis
} // openstudio
