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
#include "../../model/AirLoopHVAC.hpp"
#include "../../model/AirLoopHVAC_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SizingZone.hpp"
#include "../../model/SizingZone_Impl.hpp"
#include "../../model/ControllerMechanicalVentilation.hpp"
#include "../../model/ControllerMechanicalVentilation_Impl.hpp"
#include "../../model/ControllerOutdoorAir.hpp"
#include "../../model/ControllerOutdoorAir_Impl.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/Sizing_Zone_FieldEnums.hxx>
#include <utilities/idd/DesignSpecification_ZoneAirDistribution_FieldEnums.hxx>
#include <utilities/idd/Controller_MechanicalVentilation_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSizingZone(SizingZone& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    // ZoneorZoneListName
    model::ThermalZone thermalZone = modelObject.thermalZone();
    boost::optional<IdfObject> _thermalZone = translateAndMapModelObject(thermalZone);
    if (!_thermalZone) {
      // This shouldn't even happen, but in any case, there's no point translating a Sizing:Zone for no Zone...
      return boost::none;
    }

    IdfObject idfObject(IddObjectType::Sizing_Zone);
    m_idfObjects.push_back(idfObject);

    std::string name = _thermalZone->nameString();
    { idfObject.setString(Sizing_ZoneFields::ZoneorZoneListName, name); }

    // ZoneCoolingDesignSupplyAirTemperatureInputMethod
    {
      s = modelObject.zoneCoolingDesignSupplyAirTemperatureInputMethod();
      idfObject.setString(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureInputMethod, s.get());
    }

    // ZoneCoolingDesignSupplyAirTemperature
    value = modelObject.zoneCoolingDesignSupplyAirTemperature();
    if (value) {
      idfObject.setDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperature, value.get());
    }

    // zoneCoolingDesignSupplyAirTemperatureDifference
    value = modelObject.zoneCoolingDesignSupplyAirTemperatureDifference();
    if (value) {
      idfObject.setDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureDifference, value.get());
    }

    // ZoneHeatingDesignSupplyAirTemperatureInputMethod
    {
      s = modelObject.zoneHeatingDesignSupplyAirTemperatureInputMethod();
      idfObject.setString(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperatureInputMethod, s.get());
    }

    // ZoneHeatingDesignSupplyAirTemperature
    value = modelObject.zoneHeatingDesignSupplyAirTemperature();
    if (value) {
      idfObject.setDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperature, value.get());
    }

    // ZoneHeatingDesignSupplyAirTemperatureDifference
    value = modelObject.zoneHeatingDesignSupplyAirTemperatureDifference();
    if (value) {
      idfObject.setDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperatureDifference, value.get());
    }

    // ZoneCoolingDesignSupplyAirHumidityRatio

    value = modelObject.zoneCoolingDesignSupplyAirHumidityRatio();
    if (value) {
      idfObject.setDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirHumidityRatio, value.get());
    }

    // ZoneHeatingDesignSupplyAirHumidityRatio

    value = modelObject.zoneHeatingDesignSupplyAirHumidityRatio();
    if (value) {
      idfObject.setDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirHumidityRatio, value.get());
    }

    //  ((DesignSpecificationOutdoorAirObjectName)(Design Specification Outdoor Air Object Name))

    // ZoneHeatingSizingFactor

    value = modelObject.zoneHeatingSizingFactor();
    if (value) {
      idfObject.setDouble(Sizing_ZoneFields::ZoneHeatingSizingFactor, value.get());
    }

    // ZoneCoolingSizingFactor

    value = modelObject.zoneCoolingSizingFactor();
    if (value) {
      idfObject.setDouble(Sizing_ZoneFields::ZoneCoolingSizingFactor, value.get());
    }

    // CoolingDesignAirFlowMethod

    s = modelObject.coolingDesignAirFlowMethod();
    if (s) {
      idfObject.setString(Sizing_ZoneFields::CoolingDesignAirFlowMethod, s.get());
    }

    // CoolingDesignAirFlowRate

    value = modelObject.coolingDesignAirFlowRate();
    if (value) {
      idfObject.setDouble(Sizing_ZoneFields::CoolingDesignAirFlowRate, value.get());
    }

    // CoolingMinimumAirFlowperZoneFloorArea

    value = modelObject.coolingMinimumAirFlowperZoneFloorArea();
    if (value) {
      idfObject.setDouble(Sizing_ZoneFields::CoolingMinimumAirFlowperZoneFloorArea, value.get());
    }

    // CoolingMinimumAirFlow

    value = modelObject.coolingMinimumAirFlow();
    if (value) {
      idfObject.setDouble(Sizing_ZoneFields::CoolingMinimumAirFlow, value.get());
    }

    // CoolingMinimumAirFlowFraction

    value = modelObject.coolingMinimumAirFlowFraction();
    if (value) {
      idfObject.setDouble(Sizing_ZoneFields::CoolingMinimumAirFlowFraction, value.get());
    }

    // HeatingDesignAirFlowMethod

    s = modelObject.heatingDesignAirFlowMethod();
    if (s) {
      idfObject.setString(Sizing_ZoneFields::HeatingDesignAirFlowMethod, s.get());
    }

    // HeatingDesignAirFlowRate

    value = modelObject.heatingDesignAirFlowRate();
    if (value) {
      idfObject.setDouble(Sizing_ZoneFields::HeatingDesignAirFlowRate, value.get());
    }

    // HeatingMaximumAirFlowperZoneFloorArea

    value = modelObject.heatingMaximumAirFlowperZoneFloorArea();
    if (value) {
      idfObject.setDouble(Sizing_ZoneFields::HeatingMaximumAirFlowperZoneFloorArea, value.get());
    }

    // HeatingMaximumAirFlow

    value = modelObject.heatingMaximumAirFlow();
    if (value) {
      idfObject.setDouble(Sizing_ZoneFields::HeatingMaximumAirFlow, value.get());
    }

    // HeatingMaximumAirFlowFraction

    value = modelObject.heatingMaximumAirFlowFraction();
    if (value) {
      idfObject.setDouble(Sizing_ZoneFields::HeatingMaximumAirFlowFraction, value.get());
    }

    // These fields are onto OS:Sizing:Zonne but they are on DesignSpecification:ZoneAirDistribution in E+:
    // * Design Zone Air Distribution Effectiveness in Cooling Mode
    // * Design Zone Air Distribution Effectiveness in Heating Mode
    // * Design Zone Secondary Recirculation Fraction
    // * Design Minimum Zone Ventilation Efficiency

    // If any of the DSZAD fields is non-default, then it's worth it to translate it. Otherwise it has no effect.
    bool isDSZADTranslated =
      !(modelObject.isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted()
        && modelObject.isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted()
        && modelObject.isDesignZoneSecondaryRecirculationFractionDefaulted() && modelObject.isDesignMinimumZoneVentilationEfficiencyDefaulted());

    // Have to declare it here for scoping, so that we can access it when trying to set it for the Controller:MechanicalVentilation
    std::string dSZADName;

    if (isDSZADTranslated) {
      IdfObject dSZAD(IddObjectType::DesignSpecification_ZoneAirDistribution);

      dSZADName = name + " Design Spec Zone Air Dist";
      dSZAD.setName(dSZADName);

      // register the DSZAD
      m_idfObjects.push_back(dSZAD);

      // Only translate the non-defaulted fields
      if (!modelObject.isDesignZoneAirDistributionEffectivenessinCoolingModeDefaulted()) {
        dSZAD.setDouble(DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinCoolingMode,
                        modelObject.designZoneAirDistributionEffectivenessinCoolingMode());
      }

      if (!modelObject.isDesignZoneAirDistributionEffectivenessinHeatingModeDefaulted()) {
        dSZAD.setDouble(DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinHeatingMode,
                        modelObject.designZoneAirDistributionEffectivenessinHeatingMode());
      }

      if (!modelObject.isDesignZoneSecondaryRecirculationFractionDefaulted()) {
        dSZAD.setDouble(DesignSpecification_ZoneAirDistributionFields::ZoneSecondaryRecirculationFraction,
                        modelObject.designZoneSecondaryRecirculationFraction());
      }

      if (!modelObject.isDesignMinimumZoneVentilationEfficiencyDefaulted()) {
        dSZAD.setDouble(DesignSpecification_ZoneAirDistributionFields::MinimumZoneVentilationEfficiency,
                        modelObject.designMinimumZoneVentilationEfficiency());
      }

      idfObject.setString(Sizing_ZoneFields::DesignSpecificationZoneAirDistributionObjectName, dSZAD.name().get());
    }

    // Add ThermalZone and associated design objects to ControllerMechanicalVentilation.
    // This would be done in forwardTranslateControllerMechanicalVentilation except doing it here maintains proper order of the idf file.

    // Now that Multiple AirLoopHVACs serving the same zone are possible, need to loop on all
    for (const auto& airLoopHVAC : thermalZone.airLoopHVACs()) {
      if (boost::optional<model::AirLoopHVACOutdoorAirSystem> oaSystem = airLoopHVAC.airLoopHVACOutdoorAirSystem()) {
        model::ControllerOutdoorAir controllerOutdoorAir = oaSystem->getControllerOutdoorAir();
        model::ControllerMechanicalVentilation controllerMechanicalVentilation = controllerOutdoorAir.controllerMechanicalVentilation();
        if (boost::optional<IdfObject> _controllerMechanicalVentilation = translateAndMapModelObject(controllerMechanicalVentilation)) {
          IdfExtensibleGroup eg = _controllerMechanicalVentilation->pushExtensibleGroup();

          // Thermal Zone Name
          eg.setString(Controller_MechanicalVentilationExtensibleFields::ZoneorZoneListName, name);

          // DesignSpecificationOutdoorAir
          std::vector<model::Space> spaces = thermalZone.spaces();

          if (spaces.size() > 0) {
            if (boost::optional<model::DesignSpecificationOutdoorAir> designOASpec = spaces.front().designSpecificationOutdoorAir()) {
              if (boost::optional<IdfObject> _designOASpec = translateAndMapModelObject(designOASpec.get())) {
                eg.setString(Controller_MechanicalVentilationExtensibleFields::DesignSpecificationOutdoorAirObjectName, _designOASpec->name().get());
              }
            }
          }

          // DesignSpecificationZoneAirDistributionObjectName
          if (isDSZADTranslated) {
            eg.setString(Controller_MechanicalVentilationExtensibleFields::DesignSpecificationZoneAirDistributionObjectName, dSZADName);
          }
        }
      }
    }

    if (modelObject.accountforDedicatedOutdoorAirSystem()) {
      idfObject.setString(Sizing_ZoneFields::AccountforDedicatedOutdoorAirSystem, "Yes");

      {
        auto value = modelObject.dedicatedOutdoorAirSystemControlStrategy();
        idfObject.setString(Sizing_ZoneFields::DedicatedOutdoorAirSystemControlStrategy, value);
      }

      {
        if (modelObject.isDedicatedOutdoorAirLowSetpointTemperatureforDesignAutosized()) {
          idfObject.setString(Sizing_ZoneFields::DedicatedOutdoorAirLowSetpointTemperatureforDesign, "Autosize");
        } else if (auto value = modelObject.dedicatedOutdoorAirLowSetpointTemperatureforDesign()) {
          idfObject.setDouble(Sizing_ZoneFields::DedicatedOutdoorAirLowSetpointTemperatureforDesign, value.get());
        }
      }

      {
        if (modelObject.isDedicatedOutdoorAirHighSetpointTemperatureforDesignAutosized()) {
          idfObject.setString(Sizing_ZoneFields::DedicatedOutdoorAirHighSetpointTemperatureforDesign, "Autosize");
        } else if (auto value = modelObject.dedicatedOutdoorAirHighSetpointTemperatureforDesign()) {
          idfObject.setDouble(Sizing_ZoneFields::DedicatedOutdoorAirHighSetpointTemperatureforDesign, value.get());
        }
      }
    } else {
      idfObject.setString(Sizing_ZoneFields::AccountforDedicatedOutdoorAirSystem, "No");
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
