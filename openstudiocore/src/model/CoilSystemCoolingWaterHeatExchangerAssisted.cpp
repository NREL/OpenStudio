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

#include "CoilSystemCoolingWaterHeatExchangerAssisted.hpp"
#include "CoilSystemCoolingWaterHeatExchangerAssisted_Impl.hpp"
#include "AirToAirComponent.hpp"
#include "AirToAirComponent_Impl.hpp"
#include "ControllerWaterCoil.hpp"
#include "ControllerWaterCoil_Impl.hpp"
#include "WaterToAirComponent.hpp"
#include "WaterToAirComponent_Impl.hpp"
#include "CoilCoolingWater.hpp"
#include "CoilCoolingWater_Impl.hpp"
#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Node.hpp"
#include "Node_Impl.hpp"
#include "AirLoopHVAC.hpp"
#include "AirLoopHVAC_Impl.hpp"
#include "HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"
#include <utilities/idd/OS_CoilSystem_Cooling_Water_HeatExchangerAssisted_FieldEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  CoilSystemCoolingWaterHeatExchangerAssisted_Impl::CoilSystemCoolingWaterHeatExchangerAssisted_Impl(const IdfObject& idfObject,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : StraightComponent_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == CoilSystemCoolingWaterHeatExchangerAssisted::iddObjectType());
  }

  CoilSystemCoolingWaterHeatExchangerAssisted_Impl::CoilSystemCoolingWaterHeatExchangerAssisted_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == CoilSystemCoolingWaterHeatExchangerAssisted::iddObjectType());
  }

  CoilSystemCoolingWaterHeatExchangerAssisted_Impl::CoilSystemCoolingWaterHeatExchangerAssisted_Impl(const CoilSystemCoolingWaterHeatExchangerAssisted_Impl& other,
                                                                                                     Model_Impl* model,
                                                                                                     bool keepHandle)
    : StraightComponent_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& CoilSystemCoolingWaterHeatExchangerAssisted_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType CoilSystemCoolingWaterHeatExchangerAssisted_Impl::iddObjectType() const {
    return CoilSystemCoolingWaterHeatExchangerAssisted::iddObjectType();
  }

  AirToAirComponent CoilSystemCoolingWaterHeatExchangerAssisted_Impl::heatExchanger() const {
    boost::optional<AirToAirComponent> value = optionalHeatExchanger();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Heat Exchanger attached.");
    }
    return value.get();
  }

  WaterToAirComponent CoilSystemCoolingWaterHeatExchangerAssisted_Impl::coolingCoil() const {
    auto value = optionalCoolingCoil();
    if (!value) {
      LOG_AND_THROW(briefDescription() << " does not have an Cooling Coil attached.");
    }
    return value.get();
  }

  bool CoilSystemCoolingWaterHeatExchangerAssisted_Impl::setHeatExchanger(const AirToAirComponent& heatExchanger) {
    bool result = setPointer(OS_CoilSystem_Cooling_Water_HeatExchangerAssistedFields::HeatExchanger, heatExchanger.handle());
    return result;
  }

  bool CoilSystemCoolingWaterHeatExchangerAssisted_Impl::setCoolingCoil(const WaterToAirComponent& coolingCoil) {
    bool result = setPointer(OS_CoilSystem_Cooling_Water_HeatExchangerAssistedFields::CoolingCoil, coolingCoil.handle());
    return result;
  }

  boost::optional<AirToAirComponent> CoilSystemCoolingWaterHeatExchangerAssisted_Impl::optionalHeatExchanger() const {
    return getObject<ModelObject>().getModelObjectTarget<AirToAirComponent>(OS_CoilSystem_Cooling_Water_HeatExchangerAssistedFields::HeatExchanger);
  }

  boost::optional<WaterToAirComponent> CoilSystemCoolingWaterHeatExchangerAssisted_Impl::optionalCoolingCoil() const {
    return getObject<ModelObject>().getModelObjectTarget<WaterToAirComponent>(OS_CoilSystem_Cooling_Water_HeatExchangerAssistedFields::CoolingCoil);
  }

  unsigned CoilSystemCoolingWaterHeatExchangerAssisted_Impl::inletPort() {
    return OS_CoilSystem_Cooling_Water_HeatExchangerAssistedFields::AirInletNodeName;
  }

  unsigned CoilSystemCoolingWaterHeatExchangerAssisted_Impl::outletPort() {
    return OS_CoilSystem_Cooling_Water_HeatExchangerAssistedFields::AirOutletNodeName;
  }

  bool CoilSystemCoolingWaterHeatExchangerAssisted_Impl::addToNode(Node & node)
  {
    bool result = false;

    if( boost::optional<AirLoopHVAC> airLoop = node.airLoopHVAC() ) {
      if( ! airLoop->demandComponent(node.handle()) ) {
        result = StraightComponent_Impl::addToNode( node );
        if( result ) {
          auto t_coolingCoil = coolingCoil();
          if( auto waterInletModelObject = t_coolingCoil.waterInletModelObject() ) {

            if( auto coilCoolingWater = t_coolingCoil.optionalCast<CoilCoolingWater>() ) {
              if( auto oldController = coilCoolingWater->controllerWaterCoil() ) {
                oldController->remove();
              }
            }

            auto t_model = model();
            ControllerWaterCoil controller(t_model);

            auto coilWaterInletNode = waterInletModelObject->optionalCast<Node>();
            OS_ASSERT(coilWaterInletNode);
            controller.setActuatorNode(coilWaterInletNode.get());
            // sensor node will be established in translator since that node does not yet exist

            controller.setAction("Reverse");
          }
        }
      }
    }

    return result;
  }
} // detail

CoilSystemCoolingWaterHeatExchangerAssisted::CoilSystemCoolingWaterHeatExchangerAssisted(const Model& model)
  : StraightComponent(CoilSystemCoolingWaterHeatExchangerAssisted::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::CoilSystemCoolingWaterHeatExchangerAssisted_Impl>());

  CoilCoolingWater coolingCoil(model);
  setCoolingCoil(coolingCoil);

  HeatExchangerAirToAirSensibleAndLatent heatExchanger(model);
  heatExchanger.setSupplyAirOutletTemperatureControl(false);
  setHeatExchanger(heatExchanger);
}

IddObjectType CoilSystemCoolingWaterHeatExchangerAssisted::iddObjectType() {
  return IddObjectType(IddObjectType::OS_CoilSystem_Cooling_Water_HeatExchangerAssisted);
}

AirToAirComponent CoilSystemCoolingWaterHeatExchangerAssisted::heatExchanger() const {
  return getImpl<detail::CoilSystemCoolingWaterHeatExchangerAssisted_Impl>()->heatExchanger();
}

WaterToAirComponent CoilSystemCoolingWaterHeatExchangerAssisted::coolingCoil() const {
  return getImpl<detail::CoilSystemCoolingWaterHeatExchangerAssisted_Impl>()->coolingCoil();
}

bool CoilSystemCoolingWaterHeatExchangerAssisted::setHeatExchanger(const AirToAirComponent& heatExchanger) {
  return getImpl<detail::CoilSystemCoolingWaterHeatExchangerAssisted_Impl>()->setHeatExchanger(heatExchanger);
}

bool CoilSystemCoolingWaterHeatExchangerAssisted::setCoolingCoil(const WaterToAirComponent& coolingCoil) {
  return getImpl<detail::CoilSystemCoolingWaterHeatExchangerAssisted_Impl>()->setCoolingCoil(coolingCoil);
}

/// @cond
CoilSystemCoolingWaterHeatExchangerAssisted::CoilSystemCoolingWaterHeatExchangerAssisted(std::shared_ptr<detail::CoilSystemCoolingWaterHeatExchangerAssisted_Impl> impl)
  : StraightComponent(impl)
{}
/// @endcond

} // model
} // openstudio

