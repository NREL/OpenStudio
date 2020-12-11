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
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/FanOnOff.hpp"
#include "../../model/FanOnOff_Impl.hpp"
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/FanConstantVolume.hpp"
#include "../../model/FanConstantVolume_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ZoneHVACTerminalUnitVariableRefrigerantFlow.hpp"
#include "../../model/ZoneHVACTerminalUnitVariableRefrigerantFlow_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/CurveBiquadratic.hpp"
#include "../../model/CurveBiquadratic_Impl.hpp"
#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"
#include "../../model/CoilCoolingDXVariableRefrigerantFlow.hpp"
#include "../../model/CoilCoolingDXVariableRefrigerantFlow_Impl.hpp"
#include "../../model/CoilHeatingDXVariableRefrigerantFlow.hpp"
#include "../../model/CoilHeatingDXVariableRefrigerantFlow_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include "../../utilities/math/FloatCompare.hpp"
#include <utilities/idd/ZoneHVAC_TerminalUnit_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/Fan_OnOff_FieldEnums.hxx>
#include <utilities/idd/Fan_SystemModel_FieldEnums.hxx>
#include <utilities/idd/Fan_ConstantVolume_FieldEnums.hxx>
#include <utilities/idd/OutdoorAir_Mixer_FieldEnums.hxx>
#include <utilities/idd/Coil_Cooling_DX_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_DX_VariableRefrigerantFlow_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Fuel_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Electric_FieldEnums.hxx>
#include <utilities/idd/Coil_Heating_Water_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateZoneHVACTerminalUnitVariableRefrigerantFlow(ZoneHVACTerminalUnitVariableRefrigerantFlow& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    IdfObject idfObject(IddObjectType::ZoneHVAC_TerminalUnit_VariableRefrigerantFlow);

    m_idfObjects.push_back(idfObject);

    // Name

    s = modelObject.name();
    if (s) {
      idfObject.setName(*s);
    }

    auto t_airLoopHVAC = modelObject.airLoopHVAC();

    // TerminalUnitAvailabilityschedule

    if (boost::optional<model::Schedule> schedule = modelObject.terminalUnitAvailabilityschedule()) {
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAvailabilitySchedule, _schedule->name().get());
      }
    }

    // SupplyAirFlowRateDuringCoolingOperation

    if (modelObject.isSupplyAirFlowRateDuringCoolingOperationAutosized()) {
      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingSupplyAirFlowRate, "Autosize");
    } else if ((value = modelObject.supplyAirFlowRateDuringCoolingOperation())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingSupplyAirFlowRate, value.get());
    }

    // SupplyAirFlowRateWhenNoCoolingisNeeded

    if (modelObject.isSupplyAirFlowRateWhenNoCoolingisNeededAutosized()) {
      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::NoCoolingSupplyAirFlowRate, "Autosize");
    } else if ((value = modelObject.supplyAirFlowRateWhenNoCoolingisNeeded())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::NoCoolingSupplyAirFlowRate, value.get());
    }

    // SupplyAirFlowRateDuringHeatingOperation

    if (modelObject.isSupplyAirFlowRateDuringHeatingOperationAutosized()) {
      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingSupplyAirFlowRate, "Autosize");
    } else if ((value = modelObject.supplyAirFlowRateDuringHeatingOperation())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingSupplyAirFlowRate, value.get());
    }

    // SupplyAirFlowRateWhenNoHeatingisNeeded

    if (modelObject.isSupplyAirFlowRateWhenNoHeatingisNeededAutosized()) {
      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::NoHeatingSupplyAirFlowRate, "Autosize");
    } else if ((value = modelObject.supplyAirFlowRateWhenNoHeatingisNeeded())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::NoHeatingSupplyAirFlowRate, value.get());
    }

    // OutdoorAirFlowRateDuringCoolingOperation

    if (modelObject.isOutdoorAirFlowRateDuringCoolingOperationAutosized()) {
      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingOutdoorAirFlowRate, "Autosize");
    } else if ((value = modelObject.outdoorAirFlowRateDuringCoolingOperation())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingOutdoorAirFlowRate, value.get());
    }

    // OutdoorAirFlowRateDuringHeatingOperation

    if (modelObject.isOutdoorAirFlowRateDuringHeatingOperationAutosized()) {
      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingOutdoorAirFlowRate, "Autosize");
    } else if ((value = modelObject.outdoorAirFlowRateDuringHeatingOperation())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingOutdoorAirFlowRate, value.get());
    }

    // OutdoorAirFlowRateWhenNoCoolingorHeatingisNeeded

    if (modelObject.isOutdoorAirFlowRateWhenNoCoolingorHeatingisNeededAutosized()) {
      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::NoLoadOutdoorAirFlowRate, "Autosize");
    } else if ((value = modelObject.outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::NoLoadOutdoorAirFlowRate, value.get());
    }

    // SupplyAirFanOperatingModeScheduleName

    if (boost::optional<model::Schedule> schedule = modelObject.supplyAirFanOperatingModeSchedule()) {
      if (boost::optional<IdfObject> _schedule = translateAndMapModelObject(schedule.get())) {
        idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanOperatingModeScheduleName, _schedule->name().get());
      }
    }

    // SupplyAirFanplacement

    idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanPlacement, "DrawThrough");

    model::ModelObject fan = modelObject.supplyAirFan();

    // Node Names
    // Currently we default the Fan to DrawThrough, which means the components are ordered as such:
    // InletNode ----- (Mixer) ---- CoolingCoil ----- HeatingCoil ----- Fan ---- (Supplemental Heating Coil) -------- OutletNode

    std::string inletNodeName;
    std::string outletNodeName;
    std::string mixerOutletNodeName;
    std::string coolOutletNodeName;
    std::string heatOutletNodeName;
    std::string fanOutletNodeName;
    // std::string supplementalOutletNodeName;
    std::string oaNodeName;

    auto coolingCoil = modelObject.coolingCoil();
    auto heatingCoil = modelObject.heatingCoil();

    if (boost::optional<model::Node> node = modelObject.inletNode()) {
      inletNodeName = node->name().get();
    }

    if (boost::optional<model::Node> node = modelObject.outletNode()) {
      outletNodeName = node->name().get();
    }

    mixerOutletNodeName = modelObject.name().get() + " Mixer Outlet Node";

    coolOutletNodeName = modelObject.name().get() + " Cooling Coil Outlet Node";

    heatOutletNodeName = modelObject.name().get() + " Heating Coil Outlet Node";

    fanOutletNodeName = modelObject.name().get() + " Fan Outlet Node";  // Will be overriden to outletNodeName if no supplemental Heating coil

    // supplementalOutletNodeName = modelObject.name().get() + " Supplemental Heating Coil Outlet Node";

    oaNodeName = modelObject.name().get() + " Outdoor Air Node";

    // TerminalUnitAirInletNodeName

    idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAirInletNodeName, inletNodeName);

    // TerminalUnitAirOutletNodeName

    idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::TerminalUnitAirOutletNodeName, outletNodeName);

    auto translateMixer = [&]() {
      auto t_airLoopHVAC = modelObject.airLoopHVAC();
      if (t_airLoopHVAC) {
        return false;
      }

      bool zeroOA = false;
      if ((value = modelObject.outdoorAirFlowRateDuringCoolingOperation())) {
        zeroOA = equal(value.get(), 0.0);
      }
      if ((value = modelObject.outdoorAirFlowRateDuringHeatingOperation())) {
        zeroOA = (zeroOA && equal(value.get(), 0.0));
      }
      if ((value = modelObject.outdoorAirFlowRateWhenNoCoolingorHeatingisNeeded())) {
        zeroOA = (zeroOA && equal(value.get(), 0.0));
      }

      if (zeroOA) return false;

      return true;
    };

    // OutdoorAirMixer
    if (translateMixer()) {
      IdfObject _outdoorAirMixer(IddObjectType::OutdoorAir_Mixer);
      _outdoorAirMixer.setName(modelObject.name().get() + " OA Mixer");
      m_idfObjects.push_back(_outdoorAirMixer);

      _outdoorAirMixer.setString(OutdoorAir_MixerFields::MixedAirNodeName, mixerOutletNodeName);

      _outdoorAirMixer.setString(OutdoorAir_MixerFields::ReturnAirStreamNodeName, inletNodeName);

      IdfObject _oaNodeList(openstudio::IddObjectType::OutdoorAir_NodeList);
      _oaNodeList.setString(0, oaNodeName);
      m_idfObjects.push_back(_oaNodeList);

      _outdoorAirMixer.setString(OutdoorAir_MixerFields::ReliefAirStreamNodeName, modelObject.name().get() + " Relief Node Name");

      _outdoorAirMixer.setString(OutdoorAir_MixerFields::OutdoorAirStreamNodeName, oaNodeName);

      // OutsideAirMixerObjectType

      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutsideAirMixerObjectType, _outdoorAirMixer.iddObject().name());

      // OutsideAirMixerObjectName

      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::OutsideAirMixerObjectName, _outdoorAirMixer.name().get());
    }

    if (coolingCoil) {
      if (boost::optional<IdfObject> _coolingCoil = translateAndMapModelObject(coolingCoil.get())) {
        // CoolingCoilObjectType

        idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingCoilObjectType, _coolingCoil->iddObject().name());

        // CoolingCoilObjectName

        idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::CoolingCoilObjectName, _coolingCoil->name().get());

        std::string coolingCoilInletNodeName;
        if (translateMixer()) {
          coolingCoilInletNodeName = mixerOutletNodeName;
        } else {
          coolingCoilInletNodeName = inletNodeName;
        }

        _coolingCoil->setString(Coil_Cooling_DX_VariableRefrigerantFlowFields::CoilAirInletNode, coolingCoilInletNodeName);

        _coolingCoil->setString(Coil_Cooling_DX_VariableRefrigerantFlowFields::CoilAirOutletNode, coolOutletNodeName);
      }
    }

    if (heatingCoil) {
      if (boost::optional<IdfObject> _heatingCoil = translateAndMapModelObject(heatingCoil.get())) {
        // HeatingCoilObjectType

        idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingCoilObjectType, _heatingCoil->iddObject().name());

        // HeatingCoilObjectName

        idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::HeatingCoilObjectName, _heatingCoil->name().get());

        if (coolingCoil) {
          _heatingCoil->setString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirInletNode, coolOutletNodeName);
        } else if (translateMixer()) {
          _heatingCoil->setString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirInletNode, mixerOutletNodeName);
        } else {
          _heatingCoil->setString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirInletNode, inletNodeName);
        }

        _heatingCoil->setString(Coil_Heating_DX_VariableRefrigerantFlowFields::CoilAirOutletNode, heatOutletNodeName);
      }
    }

    boost::optional<HVACComponent> _mo_supplementalHeatingCoil = modelObject.supplementalHeatingCoil();

    if (boost::optional<IdfObject> _fan = translateAndMapModelObject(fan)) {
      // SupplyAirFanObjectType

      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanObjectType, _fan->iddObject().name());

      // SupplyAirFanObjectName

      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplyAirFanObjectName, _fan->name().get());

      std::string fanInletNodeName;

      if (heatingCoil) {
        fanInletNodeName = heatOutletNodeName;
      } else if (coolingCoil) {
        fanInletNodeName = coolOutletNodeName;
      } else if (translateMixer()) {
        fanInletNodeName = mixerOutletNodeName;
      } else {
        fanInletNodeName = inletNodeName;
      }

      // If no supplemental coil, then directly the outletNodeName, otherwise it has its own name constructed above
      if (!_mo_supplementalHeatingCoil) {
        fanOutletNodeName = outletNodeName;
      }

      if (fan.iddObject().type() == IddObjectType::OS_Fan_OnOff) {
        _fan->setString(Fan_OnOffFields::AirInletNodeName, fanInletNodeName);
        _fan->setString(Fan_OnOffFields::AirOutletNodeName, fanOutletNodeName);
      } else if (fan.iddObject().type() == IddObjectType::OS_Fan_ConstantVolume) {
        _fan->setString(Fan_ConstantVolumeFields::AirInletNodeName, fanInletNodeName);
        _fan->setString(Fan_ConstantVolumeFields::AirOutletNodeName, fanOutletNodeName);
      } else if (fan.iddObject().type() == IddObjectType::OS_Fan_SystemModel) {
        _fan->setString(Fan_SystemModelFields::AirInletNodeName, fanInletNodeName);
        _fan->setString(Fan_SystemModelFields::AirOutletNodeName, fanOutletNodeName);
      } else {
        LOG(Error, "VRF named " << modelObject.name().get() << " uses an unsupported fan type.");
      }
    }

    if (_mo_supplementalHeatingCoil) {
      if (boost::optional<IdfObject> _supplementalHeatingCoil = translateAndMapModelObject(_mo_supplementalHeatingCoil.get())) {
        // SupplementalHeatingCoilObjectType
        idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplementalHeatingCoilObjectType,
                            _supplementalHeatingCoil->iddObject().name());

        // SupplementalHeatingCoilObjectName
        idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::SupplementalHeatingCoilName, _supplementalHeatingCoil->name().get());

        if (_supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Fuel) {
          _supplementalHeatingCoil->setString(Coil_Heating_FuelFields::AirInletNodeName, fanOutletNodeName);
          _supplementalHeatingCoil->setString(Coil_Heating_FuelFields::AirOutletNodeName, outletNodeName);
        } else if (_supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Electric) {
          _supplementalHeatingCoil->setString(Coil_Heating_ElectricFields::AirInletNodeName, fanOutletNodeName);
          _supplementalHeatingCoil->setString(Coil_Heating_ElectricFields::AirOutletNodeName, outletNodeName);
        } else if (_supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Water) {
          _supplementalHeatingCoil->setString(Coil_Heating_WaterFields::AirInletNodeName, fanOutletNodeName);
          _supplementalHeatingCoil->setString(Coil_Heating_WaterFields::AirOutletNodeName, outletNodeName);
        } else if (_supplementalHeatingCoil->iddObject().type() == IddObjectType::Coil_Heating_Steam) {
          // Not yet supported in OS
          OS_ASSERT(false);
        } else {
          LOG(Error, "Unsupported supplemental heating Coil type for " << modelObject.briefDescription());
          OS_ASSERT(false);
        }
      }
    }

    // ZoneTerminalUnitOnParasiticElectricEnergyUse

    if ((value = modelObject.zoneTerminalUnitOnParasiticElectricEnergyUse())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::ZoneTerminalUnitOnParasiticElectricEnergyUse, value.get());
    }

    // ZoneTerminalUnitOffParasiticElectricEnergyUse

    if ((value = modelObject.zoneTerminalUnitOffParasiticElectricEnergyUse())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::ZoneTerminalUnitOffParasiticElectricEnergyUse, value.get());
    }

    // RatedTotalHeatingCapacitySizingRatio

    if ((value = modelObject.ratedTotalHeatingCapacitySizingRatio())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::RatedHeatingCapacitySizingRatio, value.get());
    }

    // \field Availability Manager List Name
    // \field Design Specification ZoneHVAC Sizing Object Name

    //  Maximum Supply Air Temperature from Supplemental Heater (autosized)
    if (modelObject.isMaximumSupplyAirTemperaturefromSupplementalHeaterAutosized()) {
      idfObject.setString(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::MaximumSupplyAirTemperaturefromSupplementalHeater, "Autosize");
    } else if ((value = modelObject.maximumSupplyAirTemperaturefromSupplementalHeater())) {
      idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::MaximumSupplyAirTemperaturefromSupplementalHeater, value.get());
    }

    // \field Maximum Outdoor Dry-Bulb Temperature for Supplemental Heater Operation (default 21C)
    idfObject.setDouble(ZoneHVAC_TerminalUnit_VariableRefrigerantFlowFields::MaximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation,
                        modelObject.maximumOutdoorDryBulbTemperatureforSupplementalHeaterOperation());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
