/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "Inverter.hpp"
#include "Inverter_Impl.hpp"
#include "ParentObject.hpp"
#include "ParentObject_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "ElectricLoadCenterDistribution.hpp"
#include "ElectricLoadCenterDistribution_Impl.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {

namespace model {

namespace detail {

Inverter_Impl::Inverter_Impl(IddObjectType type, Model_Impl* model)
  : ParentObject_Impl(type,model)
{
}

Inverter_Impl::Inverter_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
  : ParentObject_Impl(idfObject, model, keepHandle)
{
}

Inverter_Impl::Inverter_Impl(
    const openstudio::detail::WorkspaceObject_Impl& other,
    Model_Impl* model,
    bool keepHandle)
 : ParentObject_Impl(other, model, keepHandle)
{
}

Inverter_Impl::Inverter_Impl(const Inverter_Impl& other,
                             Model_Impl* model,
                             bool keepHandles)
 : ParentObject_Impl(other, model, keepHandles)
{
}

boost::optional<ElectricLoadCenterDistribution> Inverter_Impl::electricLoadCenterDistribution() const
{
  auto elcds = getObject<ModelObject>().getModelObjectSources<ElectricLoadCenterDistribution>(ElectricLoadCenterDistribution::iddObjectType());
  if (elcds.empty()){
    // no error
  } else if (elcds.size() == 1u){
    return elcds[0];
  } else{
    // error
  }

  return boost::none;
}

} // detail

Inverter::Inverter(IddObjectType type,const Model& model)
  : ParentObject(type, model)
{
  OS_ASSERT(getImpl<detail::Inverter_Impl>());
}

Inverter::Inverter(std::shared_ptr<detail::Inverter_Impl> p)
  : ParentObject(std::move(p))
{}

boost::optional<ElectricLoadCenterDistribution> Inverter::electricLoadCenterDistribution() const
{
  return getImpl<detail::Inverter_Impl>()->electricLoadCenterDistribution();
}

boost::optional<ThermalZone> Inverter::thermalZone() const {
  return getImpl<detail::Inverter_Impl>()->thermalZone();
}

bool Inverter::setThermalZone(const ThermalZone& thermalZone) {
  return getImpl<detail::Inverter_Impl>()->setThermalZone(thermalZone);
}

void Inverter::resetThermalZone() {
  getImpl<detail::Inverter_Impl>()->resetThermalZone();
}

} // model

} // openstudio

