/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#include "CoilSystemCoolingDXHeatExchangerAssisted.hpp"
#include "CoilSystemCoolingDXHeatExchangerAssisted_Impl.hpp"
#include "CoilCoolingDXSingleSpeed.hpp"
#include "CoilCoolingDXSingleSpeed_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include <utilities/idd/OS_CoilSystem_Cooling_DX_HeatExchangerAssisted_FieldEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilSystemCoolingDXHeatExchangerAssisted_Impl::CoilSystemCoolingDXHeatExchangerAssisted_Impl(const IdfObject& idfObject,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilSystemCoolingDXHeatExchangerAssisted::iddObjectType());
  }

  CoilSystemCoolingDXHeatExchangerAssisted_Impl::CoilSystemCoolingDXHeatExchangerAssisted_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilSystemCoolingDXHeatExchangerAssisted::iddObjectType());
  }

  CoilSystemCoolingDXHeatExchangerAssisted_Impl::CoilSystemCoolingDXHeatExchangerAssisted_Impl(const CoilSystemCoolingDXHeatExchangerAssisted_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilSystemCoolingDXHeatExchangerAssisted_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilSystemCoolingDXHeatExchangerAssisted_Impl::iddObjectType() const {
    return CoilSystemCoolingDXHeatExchangerAssisted::iddObjectType();
  }

  AirToAirComponent CoilSystemCoolingDXHeatExchangerAssisted_Impl::heatExchanger() const {
    boost::optional<AirToAirComponent> value = optionalHeatExchanger();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heat Exchanger attached.");
    }
    return value.get();
  }

  StraightComponent CoilSystemCoolingDXHeatExchangerAssisted_Impl::coolingCoil() const {
    boost::optional<StraightComponent> value = optionalCoolingCoil();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Cooling Coil attached.");
    }
    return value.get();
  }

  bool CoilSystemCoolingDXHeatExchangerAssisted_Impl::setHeatExchanger(const AirToAirComponent& heatExchanger) {
    bool result = setPointer(OS_CoilSystem_Cooling_DX_HeatExchangerAssistedFields::HeatExchanger, heatExchanger.handle());
    return result;
  }

  bool CoilSystemCoolingDXHeatExchangerAssisted_Impl::setCoolingCoil(const StraightComponent& coolingCoil) {
    bool result = setPointer(OS_CoilSystem_Cooling_DX_HeatExchangerAssistedFields::CoolingCoil, coolingCoil.handle());
    return result;
  }

  boost::optional<AirToAirComponent> CoilSystemCoolingDXHeatExchangerAssisted_Impl::optionalHeatExchanger() const {
    return getObject<ModelObject>().getModelObjectTarget<AirToAirComponent>(OS_CoilSystem_Cooling_DX_HeatExchangerAssistedFields::HeatExchanger);
  }

  boost::optional<StraightComponent> CoilSystemCoolingDXHeatExchangerAssisted_Impl::optionalCoolingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<StraightComponent>(OS_CoilSystem_Cooling_DX_HeatExchangerAssistedFields::CoolingCoil);
  }

  unsigned CoilSystemCoolingDXHeatExchangerAssisted_Impl::inletPort() {
    return OS_CoilSystem_Cooling_DX_HeatExchangerAssistedFields::AirInletNodeName;
  }

  unsigned CoilSystemCoolingDXHeatExchangerAssisted_Impl::outletPort() {
    return OS_CoilSystem_Cooling_DX_HeatExchangerAssistedFields::AirOutletNodeName;
  }

  bool CoilSystemCoolingDXHeatExchangerAssisted_Impl::addToNode(Node & node)
  {
    if( boost::optional<AirLoopHVAC> airLoop = node.airLoopHVAC() ) {
      if( ! airLoop->demandComponent(node.handle()) ) {
        return StraightComponent_Impl::addToNode( node );
      }
    }

    return false;
  }

  ModelObject CoilSystemCoolingDXHeatExchangerAssisted_Impl::clone(Model model) const
  {
    auto newCoil = StraightComponent_Impl::clone(model).cast<CoilSystemCoolingDXHeatExchangerAssisted>();

    {
      auto mo = heatExchanger().clone(model).cast<AirToAirComponent>();
      newCoil.setHeatExchanger(mo);
    }

    {
      auto mo = coolingCoil().clone(model).cast<StraightComponent>();
      newCoil.setCoolingCoil(mo);
    }

    return newCoil;
  }

  std::vector<ModelObject> CoilSystemCoolingDXHeatExchangerAssisted_Impl::children() const
  {
    std::vector<ModelObject> result;

    result.push_back(heatExchanger());
    result.push_back(coolingCoil());

    return result;
  }

} // detail

CoilSystemCoolingDXHeatExchangerAssisted::CoilSystemCoolingDXHeatExchangerAssisted(const Model& model)
  : StraightComponent(CoilSystemCoolingDXHeatExchangerAssisted::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilSystemCoolingDXHeatExchangerAssisted_Impl>());

  CoilCoolingDXSingleSpeed coolingCoil(model);
  setCoolingCoil(coolingCoil);

  HeatExchangerAirToAirSensibleAndLatent heatExchanger(model);
  heatExchanger.setSupplyAirOutletTemperatureControl(false);
  setHeatExchanger(heatExchanger);
}

IddObjectType CoilSystemCoolingDXHeatExchangerAssisted::iddObjectType() {
  return IddObjectType(IddObjectType::OS_CoilSystem_Cooling_DX_HeatExchangerAssisted);
}

AirToAirComponent CoilSystemCoolingDXHeatExchangerAssisted::heatExchanger() const {
  return getImpl<detail::CoilSystemCoolingDXHeatExchangerAssisted_Impl>()->heatExchanger();
}

StraightComponent CoilSystemCoolingDXHeatExchangerAssisted::coolingCoil() const {
  return getImpl<detail::CoilSystemCoolingDXHeatExchangerAssisted_Impl>()->coolingCoil();
}

bool CoilSystemCoolingDXHeatExchangerAssisted::setHeatExchanger(const AirToAirComponent& heatExchanger) {
  return getImpl<detail::CoilSystemCoolingDXHeatExchangerAssisted_Impl>()->setHeatExchanger(heatExchanger);
}

bool CoilSystemCoolingDXHeatExchangerAssisted::setCoolingCoil(const StraightComponent& coolingCoil) {
  return getImpl<detail::CoilSystemCoolingDXHeatExchangerAssisted_Impl>()->setCoolingCoil(coolingCoil);
}

/// @cond
CoilSystemCoolingDXHeatExchangerAssisted::CoilSystemCoolingDXHeatExchangerAssisted(std::shared_ptr<detail::CoilSystemCoolingDXHeatExchangerAssisted_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

