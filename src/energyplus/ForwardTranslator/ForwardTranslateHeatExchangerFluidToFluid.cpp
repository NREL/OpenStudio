/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/HeatExchangerFluidToFluid.hpp"
#include "../../model/HeatExchangerFluidToFluid_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/SetpointManager.hpp"
#include "../../model/SetpointManager_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/core/Logger.hpp"
#include <utilities/idd/HeatExchanger_FluidToFluid_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateHeatExchangerFluidToFluid(HeatExchangerFluidToFluid& modelObject) {
    OptionalString s;
    OptionalDouble d;
    OptionalModelObject mo;
    boost::optional<IdfObject> idfo;

    IdfObject idfObject(IddObjectType::HeatExchanger_FluidToFluid);

    m_idfObjects.push_back(idfObject);

    // Name
    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    // AvailabilityScheduleName
    if (boost::optional<Schedule> sched = modelObject.availabilitySchedule()) {
      if ((idfo = translateAndMapModelObject(sched.get()))) {
        idfObject.setString(HeatExchanger_FluidToFluidFields::AvailabilityScheduleName, idfo->name().get());
      }
    }

    // LoopDemandSideInletNode
    mo = modelObject.demandInletModelObject();
    if (mo) {
      idfObject.setString(HeatExchanger_FluidToFluidFields::LoopDemandSideInletNodeName, mo->name().get());
    }

    // LoopDemandSideOutletNode
    mo = modelObject.demandOutletModelObject();
    if (mo) {
      idfObject.setString(HeatExchanger_FluidToFluidFields::LoopDemandSideOutletNodeName, mo->name().get());
    }

    // LoopDemandSideDesignFlowRate
    if (modelObject.isLoopDemandSideDesignFlowRateAutosized()) {
      idfObject.setString(HeatExchanger_FluidToFluidFields::LoopDemandSideDesignFlowRate, "Autosize");
    } else {
      if ((d = modelObject.loopDemandSideDesignFlowRate())) {
        idfObject.setDouble(HeatExchanger_FluidToFluidFields::LoopDemandSideDesignFlowRate, d.get());
      }
    }

    // LoopSupplySideInletNode
    mo = modelObject.supplyInletModelObject();
    if (mo) {
      idfObject.setString(HeatExchanger_FluidToFluidFields::LoopSupplySideInletNodeName, mo->name().get());
    }

    // LoopSupplySideOutletNode
    mo = modelObject.supplyOutletModelObject();
    if (mo) {
      idfObject.setString(HeatExchanger_FluidToFluidFields::LoopSupplySideOutletNodeName, mo->name().get());
    }

    // LoopSupplySideDesignFlowRate
    if (modelObject.isLoopSupplySideDesignFlowRateAutosized()) {
      idfObject.setString(HeatExchanger_FluidToFluidFields::LoopSupplySideDesignFlowRate, "Autosize");
    } else {
      if ((d = modelObject.loopSupplySideDesignFlowRate())) {
        idfObject.setDouble(HeatExchanger_FluidToFluidFields::LoopSupplySideDesignFlowRate, d.get());
      }
    }

    // HeatExchangeModelType
    if ((s = modelObject.heatExchangeModelType())) {
      idfObject.setString(HeatExchanger_FluidToFluidFields::HeatExchangeModelType, s.get());
    }

    // HeatExchangerUFactorTimesAreaValue
    if (modelObject.isHeatExchangerUFactorTimesAreaValueAutosized()) {
      idfObject.setString(HeatExchanger_FluidToFluidFields::HeatExchangerUFactorTimesAreaValue, "Autosize");
    } else {
      if ((d = modelObject.heatExchangerUFactorTimesAreaValue())) {
        idfObject.setDouble(HeatExchanger_FluidToFluidFields::HeatExchangerUFactorTimesAreaValue, d.get());
      }
    }

    // ControlType
    if ((s = modelObject.controlType())) {
      idfObject.setString(HeatExchanger_FluidToFluidFields::ControlType, s.get());
    }

    // HeatExchangerSetpointNodeName
    if ((mo = modelObject.supplyOutletModelObject())) {
      auto node = mo->optionalCast<Node>();
      OS_ASSERT(node);
      if (!node->setpointManagers().empty()) {
        idfObject.setString(HeatExchanger_FluidToFluidFields::HeatExchangerSetpointNodeName, mo->name().get());
      } else {
        auto plant = node->plantLoop();
        OS_ASSERT(plant);
        auto supplyOutletModelObject = plant->supplyOutletNode();
        idfObject.setString(HeatExchanger_FluidToFluidFields::HeatExchangerSetpointNodeName, supplyOutletModelObject.name().get());
      }
    }

    // MinimumTemperatureDifferencetoActivateHeatExchanger
    if ((d = modelObject.minimumTemperatureDifferencetoActivateHeatExchanger())) {
      idfObject.setDouble(HeatExchanger_FluidToFluidFields::MinimumTemperatureDifferencetoActivateHeatExchanger, d.get());
    }

    // HeatTransferMeteringEndUseType
    if ((s = modelObject.heatTransferMeteringEndUseType())) {
      idfObject.setString(HeatExchanger_FluidToFluidFields::HeatTransferMeteringEndUseType, s.get());
    }

    // ComponentOverrideLoopSupplySideInletNode
    if ((mo = modelObject.componentOverrideLoopSupplySideInletNode())) {
      idfObject.setString(HeatExchanger_FluidToFluidFields::ComponentOverrideLoopSupplySideInletNodeName, mo->name().get());
    }

    // ComponentOverrideLoopDemandSideInletNode
    if ((mo = modelObject.componentOverrideLoopDemandSideInletNode())) {
      idfObject.setString(HeatExchanger_FluidToFluidFields::ComponentOverrideLoopDemandSideInletNodeName, mo->name().get());
    }

    // ComponentOverrideCoolingControlTemperatureMode
    if ((s = modelObject.componentOverrideCoolingControlTemperatureMode())) {
      idfObject.setString(HeatExchanger_FluidToFluidFields::ComponentOverrideCoolingControlTemperatureMode, s.get());
    }

    // SizingFactor
    if ((d = modelObject.sizingFactor())) {
      idfObject.setDouble(HeatExchanger_FluidToFluidFields::SizingFactor, d.get());
    }

    // OperationMinimumTemperatureLimit
    if ((d = modelObject.operationMinimumTemperatureLimit())) {
      idfObject.setDouble(HeatExchanger_FluidToFluidFields::OperationMinimumTemperatureLimit, d.get());
    }

    // OperationMaximumTemperatureLimit
    if ((d = modelObject.operationMaximumTemperatureLimit())) {
      idfObject.setDouble(HeatExchanger_FluidToFluidFields::OperationMaximumTemperatureLimit, d.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
