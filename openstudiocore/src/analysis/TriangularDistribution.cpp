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

#include "TriangularDistribution.hpp"
#include "UncertaintyDescription_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace analysis {

TriangularDistribution::TriangularDistribution() 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(TriangularDistribution::type())))
{}

TriangularDistribution::TriangularDistribution(double mode, double lowerBound, double upperBound) 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(TriangularDistribution::type())))
{
  setMode(mode);
  setLowerBound(lowerBound);
  setUpperBound(upperBound);
}

UncertaintyDescriptionType TriangularDistribution::type() {
  return UncertaintyDescriptionType(UncertaintyDescriptionType::triangular_uncertain);
}

double TriangularDistribution::mode() const {
  return impl()->getAttribute("modes",false).valueAsDouble();
}

double TriangularDistribution::lowerBound() const {
  return impl()->getAttribute("lower_bounds",false).valueAsDouble();
}

double TriangularDistribution::upperBound() const {
  return impl()->getAttribute("upper_bounds",false).valueAsDouble();
}

void TriangularDistribution::setMode(double value) {
  impl()->setAttribute(Attribute("modes",value),false);
}

void TriangularDistribution::setLowerBound(double value) {
  impl()->setAttribute(Attribute("lower_bounds",value),false);
}

void TriangularDistribution::setUpperBound(double value) {
  impl()->setAttribute(Attribute("upper_bounds",value),false);
}

TriangularDistribution::TriangularDistribution(std::shared_ptr<detail::UncertaintyDescription_Impl> impl)
  : UncertaintyDescription(impl)
{
  OS_ASSERT(type() == TriangularDistribution::type());
}

} // analysis
} // openstudio
