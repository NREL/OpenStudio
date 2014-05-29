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

#include "HistogramBinDistribution.hpp"
#include "UncertaintyDescription_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace analysis {

HistogramBinDistribution::HistogramBinDistribution() 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(HistogramBinDistribution::type())))
{}

HistogramBinDistribution::HistogramBinDistribution(std::vector<double> abscissas, std::vector<double> counts) 
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(HistogramBinDistribution::type())))
{
  setAbscissas(abscissas);
  setCounts(counts);
}

UncertaintyDescriptionType HistogramBinDistribution::type() {
  return UncertaintyDescriptionType(UncertaintyDescriptionType::histogram_bin_uncertain);
}

std::vector<double> HistogramBinDistribution::abscissas() const {
  Attribute attribute = impl()->getAttribute("abscissas",false);
  return getDoubleVectorFromAttribute(attribute);
}

std::vector<double> HistogramBinDistribution::counts() const {
  Attribute attribute = impl()->getAttribute("counts",false);
  return getDoubleVectorFromAttribute(attribute);
}

std::vector<double> HistogramBinDistribution::ordinates() const {
  Attribute attribute = impl()->getAttribute("ordinates",false);
  return getDoubleVectorFromAttribute(attribute);
}

boost::optional<int> HistogramBinDistribution::numPairs() const {
  if (impl()->isSet("num_pairs")) {
    return impl()->getAttribute("num_pairs",false).valueAsInteger();
  }
  return boost::none;
}

bool HistogramBinDistribution::setAbscissas(const std::vector<double>& value) {
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

bool HistogramBinDistribution::setCounts(const std::vector<double>& value) {
  int minValue = *std::min_element(value.begin(), value.end() - 1);
  if (value.back() != 0.0) {
    LOG(Warn,"The last value of counts must be zero.");
    return false;
  }
  else if (minValue < 0) {
    LOG(Warn,"All values of counts must be positive (except for the last value, which should be zero).");
    return false;
  }
  else {
    impl()->setAttribute(createAttributeFromVector("counts",value),false);
    int numPairs = value.size();
    impl()->setAttribute(Attribute("num_pairs",numPairs),false);
    resetOrdinates();
    return true;
  }
}

void HistogramBinDistribution::resetCounts() {
  impl()->clearAttribute("counts",false);
}

bool HistogramBinDistribution::setOrdinates(const std::vector<double>& value) {
  int minValue = *std::min_element(value.begin(), value.end() - 1);
  if (value.back() != 0.0) {
    LOG(Warn,"The last value of ordinates must be zero.");
    return false;
  }
  else if (minValue <= 0) {
    LOG(Warn,"All values of ordinates must be positive (except for the last value, which should be zero).");
    return false;
  }
  else {
    impl()->setAttribute(createAttributeFromVector("ordinates",value),false);
    int numPairs = value.size();
    impl()->setAttribute(Attribute("num_pairs",numPairs),false);
    resetCounts();
    return true;
  }
}

void HistogramBinDistribution::resetOrdinates() {
  impl()->clearAttribute("ordinates",false);
}

HistogramBinDistribution::HistogramBinDistribution(std::shared_ptr<detail::UncertaintyDescription_Impl> impl)
  : UncertaintyDescription(impl)
{
  OS_ASSERT(type() == HistogramBinDistribution::type());
}

} // analysis
} // openstudio
