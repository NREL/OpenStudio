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

#include "HistogramPointDistribution.hpp"
#include "UncertaintyDescription_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace analysis {

HistogramPointDistribution::HistogramPointDistribution() 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(HistogramPointDistribution::type())))
{}

HistogramPointDistribution::HistogramPointDistribution(std::vector<double> abscissas, std::vector<double> counts) 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(HistogramPointDistribution::type())))
{
  setAbscissas(abscissas);
  setCounts(counts);
}

UncertaintyDescriptionType HistogramPointDistribution::type() {
  return UncertaintyDescriptionType(UncertaintyDescriptionType::histogram_point_uncertain);
}

std::vector<double> HistogramPointDistribution::abscissas() const {
  Attribute attribute = impl()->getAttribute("abscissas",false);
  return getDoubleVectorFromAttribute(attribute);
}

std::vector<double> HistogramPointDistribution::counts() const {
  Attribute attribute = impl()->getAttribute("counts",false);
  return getDoubleVectorFromAttribute(attribute);
}

boost::optional<int> HistogramPointDistribution::numPairs() const {
  if (impl()->isSet("num_pairs")) {
    return impl()->getAttribute("num_pairs",false).valueAsInteger();
  }
  return boost::none;
}

bool HistogramPointDistribution::setAbscissas(const std::vector<double>& value) {
  for(std::vector<double>::size_type i = 1; i != value.size(); i++) {
    double previousValue = value[i-1];
    double currentValue = value[i];
    if (previousValue >= currentValue) {
      LOG(Warn,"Abscissas values must be strictly increasing.");
      return false;
    }
  }
  impl()->setAttribute(createAttributeFromVector("abscissas",value),false);
  return true;
}

bool HistogramPointDistribution::setCounts(const std::vector<double>& value) {
  double minValue = *std::min_element(value.begin(), value.end());
  if (minValue <= 0) {
    LOG(Warn,"All values of counts must be positive.");
    return false;
  }
  else {
    impl()->setAttribute(createAttributeFromVector("counts",value),false);
    int numPairs = value.size();
    impl()->setAttribute(Attribute("num_pairs",numPairs),false);
    return true;
  }
}

HistogramPointDistribution::HistogramPointDistribution(std::shared_ptr<detail::UncertaintyDescription_Impl> impl)
  : UncertaintyDescription(impl)
{
  OS_ASSERT(type() == HistogramPointDistribution::type());
}

} // analysis
} // openstudio
