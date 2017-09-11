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
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

