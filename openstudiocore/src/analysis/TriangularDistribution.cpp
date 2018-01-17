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
