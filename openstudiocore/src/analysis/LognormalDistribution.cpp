/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
