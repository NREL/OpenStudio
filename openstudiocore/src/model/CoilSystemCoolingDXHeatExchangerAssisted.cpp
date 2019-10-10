/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

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
#include "AirLoopHVACOutdoorAirSystem.hpp"
#include "AirLoopHVACOutdoorAirSystem_Impl.hpp"
#include "HeatExchangerAirToAirSensibleAndLatent.hpp"
#include "HeatExchangerAirToAirSensibleAndLatent_Impl.hpp"

#include "ZoneHVACPackagedTerminalHeatPump.hpp"
#include "ZoneHVACPackagedTerminalHeatPump_Impl.hpp"
#include "AirLoopHVACUnitarySystem.hpp"
#include "AirLoopHVACUnitarySystem_Impl.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir.hpp"
#include "AirLoopHVACUnitaryHeatPumpAirToAir_Impl.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass.hpp"
#include "AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass_Impl.hpp"

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
      // Not appropriate: no specific output, there are at the coil and HX level
    return result;
  }

  IddObjectType CoilSystemCoolingDXHeatExchangerAssisted_Impl::iddObjectType() const {
    return CoilSystemCoolingDXHeatExchangerAssisted::iddObjectType();
  }


  std::vector<ModelObject> CoilSystemCoolingDXHeatExchangerAssisted_Impl::children() const
  {
    std::vector<ModelObject> result;

    result.push_back(coolingCoil());
    result.push_back(heatExchanger());

    return result;
  }

  ModelObject CoilSystemCoolingDXHeatExchangerAssisted_Impl::clone(Model model) const
  {
    auto newCoilSystem = StraightComponent_Impl::clone(model).cast<CoilSystemCoolingDXHeatExchangerAssisted>();

    {
      auto mo = coolingCoil().clone(model).cast<StraightComponent>();
      newCoilSystem.setCoolingCoil(mo);
    }

    {
      auto mo = heatExchanger().clone(model).cast<AirToAirComponent>();
      newCoilSystem.setHeatExchanger(mo);
    }

    return newCoilSystem;
  }

  boost::optional<HVACComponent> CoilSystemCoolingDXHeatExchangerAssisted_Impl::containingHVACComponent() const
  {
     // AirLoopHVACUnitarySystem
    std::vector<AirLoopHVACUnitarySystem> airLoopHVACUnitarySystems = this->model().getConcreteModelObjects<AirLoopHVACUnitarySystem>();

    for( const auto & airLoopHVACUnitarySystem : airLoopHVACUnitarySystems )
    {
      if( boost::optional<HVACComponent> coolingCoil = airLoopHVACUnitarySystem.coolingCoil() )
      {
        if( coolingCoil->handle() == this->handle() )
        {
          return airLoopHVACUnitarySystem;
        }
      }
    }

    // AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass
    std::vector<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass> bypassSystems = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatCoolVAVChangeoverBypass>();

    for( const auto & bypassSystem : bypassSystems )
    {
      if( boost::optional<HVACComponent> coolingCoil = bypassSystem.coolingCoil() )
      {
        if( coolingCoil->handle() == this->handle() )
        {
          return bypassSystem;
        }
      }
    }

    // AirLoopHVACUnitaryHeatPumpAirToAir

    std::vector<AirLoopHVACUnitaryHeatPumpAirToAir> airLoopHVACUnitaryHeatPumpAirToAirs;

    airLoopHVACUnitaryHeatPumpAirToAirs = this->model().getConcreteModelObjects<AirLoopHVACUnitaryHeatPumpAirToAir>();

    for( const auto & airLoopHVACUnitaryHeatPumpAirToAir : airLoopHVACUnitaryHeatPumpAirToAirs )
    {
      if( boost::optional<HVACComponent> coil = airLoopHVACUnitaryHeatPumpAirToAir.coolingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return airLoopHVACUnitaryHeatPumpAirToAir;
        }
      }
    }   return boost::none;
  }

  boost::optional<ZoneHVACComponent> CoilSystemCoolingDXHeatExchangerAssisted_Impl::containingZoneHVACComponent() const
  {

    // ZoneHVACPackagedTerminalHeatPump

    std::vector<ZoneHVACPackagedTerminalHeatPump> zoneHVACPackagedTerminalHeatPumps;

    zoneHVACPackagedTerminalHeatPumps = this->model().getConcreteModelObjects<ZoneHVACPackagedTerminalHeatPump>();

    for( const auto & zoneHVACPackagedTerminalHeatPump : zoneHVACPackagedTerminalHeatPumps )
    {
      if( boost::optional<HVACComponent> coil = zoneHVACPackagedTerminalHeatPump.coolingCoil() )
      {
        if( coil->handle() == this->handle() )
        {
          return zoneHVACPackagedTerminalHeatPump;
        }
      }
    }

    // ZoneHVAC:WindowAirConditioner not wrapped

    return boost::none;
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

  unsigned CoilSystemCoolingDXHeatExchangerAssisted_Impl::inletPort() const {
    return OS_CoilSystem_Cooling_DX_HeatExchangerAssistedFields::AirInletNodeName;
  }

  unsigned CoilSystemCoolingDXHeatExchangerAssisted_Impl::outletPort() const {
    return OS_CoilSystem_Cooling_DX_HeatExchangerAssistedFields::AirOutletNodeName;
  }

  bool CoilSystemCoolingDXHeatExchangerAssisted_Impl::addToNode(Node & node)
  {

    /**
     * Note JM 2019-03-13: At this point in time
     * CoilSystemCoolingDXHeatExchangerAssisted is **NOT** allowed on a Branch directly and should be placed inside one of the Unitary systems
     * cf https://github.com/NREL/EnergyPlus/issues/7222
     * This method returns false and does nothing as a result
     */

    // TODO: uncomment this if it becomes allowed
    //if( boost::optional<AirLoopHVAC> airLoop = node.airLoopHVAC() ) {
      //if( ! airLoop->demandComponent(node.handle()) ) {
        //return StraightComponent_Impl::addToNode( node );
      //}
    //}

    //if ( auto oa = node.airLoopHVACOutdoorAirSystem() ) {
      //return StraightComponent_Impl::addToNode( node );
    //}

    return false;
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
  : StraightComponent(std::move(impl))
{}
/// @endcond

} // model
} // openstudio

