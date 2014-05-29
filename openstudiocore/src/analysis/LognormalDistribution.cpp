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

#include "LognormalDistribution.hpp"
#include "UncertaintyDescription_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace analysis {

LognormalDistribution::LognormalDistribution()
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
  new detail::UncertaintyDescription_Impl(LognormalDistribution::type())))
{}

UncertaintyDescriptionType LognormalDistribution::type() {
  return UncertaintyDescriptionType(UncertaintyDescriptionType::lognormal_uncertain);
}

boost::optional<double> LognormalDistribution::mean() const {
  if (impl()->isSet("means")) {
    return impl()->getAttribute("means",false).valueAsDouble();
  }
  return boost::none;
}

boost::optional<double> LognormalDistribution::standardDeviation() const {
  if (impl()->isSet("std_deviations")) {
    return impl()->getAttribute("std_deviations",false).valueAsDouble();
  }
  return boost::none;
}

boost::optional<double> LognormalDistribution::errorFactor() const {
  if (impl()->isSet("error_factors")) {
    return impl()->getAttribute("error_factors",false).valueAsDouble();
  }
  return boost::none;
}

boost::optional<double> LognormalDistribution::lambda() const {
  if (impl()->isSet("lambdas")) {
    return impl()->getAttribute("lambdas",false).valueAsDouble();
  }
  return boost::none;
}

boost::optional<double> LognormalDistribution::zeta() const {
  if (impl()->isSet("zetas")) {
    return impl()->getAttribute("zetas",false).valueAsDouble();
  }
  return boost::none;
}

boost::optional<double> LognormalDistribution::lowerBound() const {
  if (impl()->isSet("lower_bounds")) {
    return impl()->getAttribute("lower_bounds",false).valueAsDouble();
  }
  return boost::none;
}

boost::optional<double> LognormalDistribution::upperBound() const {
  if (impl()->isSet("upper_bounds")) {
    return impl()->getAttribute("upper_bounds",false).valueAsDouble();
  }
  return boost::none;
}

bool LognormalDistribution::setMean(double value) {
  if (value > 0) {
    impl()->setAttribute(Attribute("means",value),false);
    return true;
  }
  else {
    LOG(Warn,"The mean of a lognormal distribution must be positive.");
    return false;
  }
}

void LognormalDistribution::resetMean() {
  impl()->clearAttribute("means",false);
}

bool LognormalDistribution::setStandardDeviation(double value) {
  if (value > 0) {
    impl()->setAttribute(Attribute("std_deviations",value),false);
    return true;
  }
  else {
    LOG(Warn,"The standard deviation of a lognormal distribution must be positive.");
    return false;
  }
}

void LognormalDistribution::resetStandardDeviation() {
  impl()->clearAttribute("std_deviations",false);
}

bool LognormalDistribution::setErrorFactor(double value) {
  if (value > 1) {
    impl()->setAttribute(Attribute("error_factors",value),false);
    return true;
  }
  else {
    LOG(Warn,"The error factors of a lognormal distribution must be greater than 1.");
    return false;
  } 
}

void LognormalDistribution::resetErrorFactor() {
  impl()->clearAttribute("error_factors",false);
}

void LognormalDistribution::setLambda(double value) {
  impl()->setAttribute(Attribute("lambdas",value),false);
}

void LognormalDistribution::resetLambda() {
  impl()->clearAttribute("lambdas",false);
}

bool LognormalDistribution::setZeta(double value) {
  if (value > 0) {
    impl()->setAttribute(Attribute("zetas",value),false);
    return true;
  }
  else {
    LOG(Warn,"The zetas of a lognormal distribution must be positive.");
    return false;
  }
}

void LognormalDistribution::resetZeta() {
  impl()->clearAttribute("zetas",false);
}

bool LognormalDistribution::setLowerBound(double value) {
  if (value > 0) {
    impl()->setAttribute(Attribute("lower_bounds",value),false);
    return true;
  }
  else {
    LOG(Warn,"The lower bounds of a lognormal distribution must be positive.");
    return false;
  }
}

void LognormalDistribution::resetLowerBound() {
  impl()->clearAttribute("lower_bounds",false);
}

void LognormalDistribution::setUpperBound(double value) {
  impl()->setAttribute(Attribute("upper_bounds",value),false);
}

void LognormalDistribution::resetUpperBound() {
  impl()->clearAttribute("upper_bounds",false);
}

LognormalDistribution::LognormalDistribution(std::shared_ptr<detail::UncertaintyDescription_Impl> impl)
  : UncertaintyDescription(impl)
{
  OS_ASSERT(type() == LognormalDistribution::type());
}

} // analysis
} // openstudio
