/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "GasLayer.hpp"
#include "GasLayer_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  GasLayer_Impl::GasLayer_Impl(const IdfObject& idfObject,Model_Impl* model, bool keepHandle)
    : FenestrationMaterial_Impl(idfObject, model, keepHandle)
  {}

  GasLayer_Impl::GasLayer_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                               Model_Impl* model,
                               bool keepHandle)
    : FenestrationMaterial_Impl(other, model, keepHandle)
  {}

  GasLayer_Impl::GasLayer_Impl(const GasLayer_Impl& other,Model_Impl* model,bool keepHandle)
    : FenestrationMaterial_Impl(other, model, keepHandle)
  {}

  OptionalDouble GasLayer_Impl::getVisibleTransmittance() const {
    OptionalDouble od(1.0);
    return od;
  }

  boost::optional<double> GasLayer_Impl::interiorVisibleAbsorptance() const {
    return 0.0;
  }

  boost::optional<double> GasLayer_Impl::exteriorVisibleAbsorptance() const {
    return 0.0;
  }

} // detail

/// @cond
GasLayer::GasLayer(IddObjectType type,const Model& model)
  : FenestrationMaterial(type,model)
{
  OS_ASSERT(getImpl<detail::GasLayer_Impl>());
}

GasLayer::GasLayer(std::shared_ptr<detail::GasLayer_Impl> impl)
  : FenestrationMaterial(std::move(impl))
{}
/// @endcond

} // model
} // openstudio
