/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/SizingZone.hpp"
#include "../../model/SizingZone_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/DesignSpecificationOutdoorAir.hpp"
#include "../../model/DesignSpecificationOutdoorAir_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"
#include <utilities/idd/Sizing_Zone_FieldEnums.hxx>
#include <utilities/idd/DesignSpecification_ZoneAirDistribution_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateSizingZone(const WorkspaceObject& workspaceObject) {
    boost::optional<WorkspaceObject> target = workspaceObject.getTarget(Sizing_ZoneFields::ZoneorZoneListName);

    std::vector<ThermalZone> thermalZones;

    if (target) {

      // just one thermal zone
      boost::optional<ModelObject> mo;
      if (target->iddObject().type() == IddObjectType::Zone) {
        mo = translateAndMapWorkspaceObject(target.get());
        if (mo) {
          if (boost::optional<Space> space = mo->optionalCast<Space>()) {
            boost::optional<ThermalZone> thermalZone = space->thermalZone();
            if (thermalZone) {
              thermalZones.push_back(*thermalZone);
            }
          }
        }
      } else if (target->iddObject().type() == IddObjectType::ZoneList) {

        // get all thermal zones in zone list
        for (const IdfExtensibleGroup& idfGroup : target->extensibleGroups()) {
          auto workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();
          OptionalWorkspaceObject owo = workspaceGroup.getTarget(0);
          if (owo) {
            mo = translateAndMapWorkspaceObject(owo.get());
            if (mo) {
              if (boost::optional<Space> space = mo->optionalCast<Space>()) {
                boost::optional<ThermalZone> thermalZone = space->thermalZone();
                if (thermalZone) {
                  thermalZones.push_back(*thermalZone);
                }
              }
            }
          }
        }
      }
    }

    if (thermalZones.empty()) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " Can't find associated ThermalZone(s).");

      return boost::none;
    }

    boost::optional<ModelObject> result;
    for (const ThermalZone& thermalZone : thermalZones) {

      // sizing zone is constructed in thermal zone ctor
      openstudio::model::SizingZone sizingZone = thermalZone.sizingZone();

      // return first sizing zone
      if (!result) {
        result = sizingZone;
      }

      boost::optional<std::string> s;
      boost::optional<double> value;

      // ZoneCoolingDesignSupplyAirTemperatureInputMethod
      s = workspaceObject.getString(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureInputMethod);
      if (s) {
        sizingZone.setZoneCoolingDesignSupplyAirTemperatureInputMethod(s.get());
      }

      // ZoneCoolingDesignSupplyAirTemperature

      value = workspaceObject.getDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperature);
      if (value) {
        sizingZone.setZoneCoolingDesignSupplyAirTemperature(value.get());
      }

      // ZoneCoolingDesignSupplyAirTemperatureDifference
      value = workspaceObject.getDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirTemperatureDifference);
      if (value) {
        sizingZone.setZoneCoolingDesignSupplyAirTemperatureDifference(value.get());
      }

      // ZoneHeatingDesignSupplyAirTemperatureInputMethod
      s = workspaceObject.getString(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperatureInputMethod);
      if (s) {
        sizingZone.setZoneHeatingDesignSupplyAirTemperatureInputMethod(s.get());
      }

      // ZoneHeatingDesignSupplyAirTemperature

      value = workspaceObject.getDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperature);
      if (value) {
        sizingZone.setZoneHeatingDesignSupplyAirTemperature(value.get());
      }

      // ZoneHeatingDesignSupplyAirTemperatureDifference
      value = workspaceObject.getDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirTemperatureDifference);
      if (value) {
        sizingZone.setZoneHeatingDesignSupplyAirTemperatureDifference(value.get());
      }

      // ZoneCoolingDesignSupplyAirHumidityRatio

      value = workspaceObject.getDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirHumidityRatio);
      if (value) {
        sizingZone.setZoneHeatingDesignSupplyAirHumidityRatio(value.get());
      }

      // ZoneHeatingDesignSupplyAirHumidityRatio

      value = workspaceObject.getDouble(Sizing_ZoneFields::ZoneHeatingDesignSupplyAirHumidityRatio);
      if (value) {
        sizingZone.setZoneHeatingDesignSupplyAirHumidityRatio(value.get());
      }

      // DesignSpecificationOutdoorAirObjectName

      target = workspaceObject.getTarget(Sizing_ZoneFields::DesignSpecificationOutdoorAirObjectName);
      if (target) {
        OptionalModelObject mo = translateDesignSpecificationOutdoorAir(*target);
        if (mo) {
          if (mo->optionalCast<DesignSpecificationOutdoorAir>()) {
            std::vector<Space> spaces = thermalZone.spaces();
            OS_ASSERT(spaces.size() == 1);
            spaces[0].setDesignSpecificationOutdoorAir(mo->cast<DesignSpecificationOutdoorAir>());
          }
        }
      }

      // ZoneHeatingSizingFactor

      value = workspaceObject.getDouble(Sizing_ZoneFields::ZoneHeatingSizingFactor);
      if (value) {
        sizingZone.setZoneHeatingSizingFactor(value.get());
      }

      // ZoneCoolingSizingFactor

      value = workspaceObject.getDouble(Sizing_ZoneFields::ZoneCoolingSizingFactor);
      if (value) {
        sizingZone.setZoneCoolingSizingFactor(value.get());
      }

      // CoolingDesignAirFlowMethod

      s = workspaceObject.getString(Sizing_ZoneFields::CoolingDesignAirFlowMethod);
      if (s) {
        sizingZone.setCoolingDesignAirFlowMethod(s.get());
      }

      // CoolingDesignAirFlowRate

      value = workspaceObject.getDouble(Sizing_ZoneFields::CoolingDesignAirFlowRate);
      if (value) {
        sizingZone.setCoolingDesignAirFlowRate(value.get());
      }

      // CoolingMinimumAirFlowperZoneFloorArea

      value = workspaceObject.getDouble(Sizing_ZoneFields::CoolingMinimumAirFlowperZoneFloorArea);
      if (value) {
        sizingZone.setCoolingMinimumAirFlowperZoneFloorArea(value.get());
      }

      // CoolingMinimumAirFlow

      value = workspaceObject.getDouble(Sizing_ZoneFields::CoolingMinimumAirFlow);
      if (value) {
        sizingZone.setCoolingMinimumAirFlow(value.get());
      }

      // CoolingMinimumAirFlowFraction

      value = workspaceObject.getDouble(Sizing_ZoneFields::CoolingMinimumAirFlowFraction);
      if (value) {
        sizingZone.setCoolingMinimumAirFlowFraction(value.get());
      }

      // HeatingDesignAirFlowMethod

      s = workspaceObject.getString(Sizing_ZoneFields::HeatingDesignAirFlowMethod);
      if (s) {
        sizingZone.setHeatingDesignAirFlowMethod(s.get());
      }

      // HeatingDesignAirFlowRate

      value = workspaceObject.getDouble(Sizing_ZoneFields::HeatingDesignAirFlowRate);
      if (value) {
        sizingZone.setHeatingDesignAirFlowRate(value.get());
      }

      // HeatingMaximumAirFlowperZoneFloorArea

      value = workspaceObject.getDouble(Sizing_ZoneFields::HeatingMaximumAirFlowperZoneFloorArea);
      if (value) {
        sizingZone.setHeatingMaximumAirFlowperZoneFloorArea(value.get());
      }

      // HeatingMaximumAirFlow

      value = workspaceObject.getDouble(Sizing_ZoneFields::HeatingMaximumAirFlow);
      if (value) {
        sizingZone.setHeatingMaximumAirFlow(value.get());
      }

      // HeatingMaximumAirFlowFraction

      value = workspaceObject.getDouble(Sizing_ZoneFields::HeatingMaximumAirFlowFraction);
      if (value) {
        sizingZone.setHeatingMaximumAirFlowFraction(value.get());
      }

      // DesignSpecification_ZoneAirDistribution

      boost::optional<WorkspaceObject> _designSpecification =
        workspaceObject.getTarget(Sizing_ZoneFields::DesignSpecificationZoneAirDistributionObjectName);

      if (_designSpecification) {
        // ZoneAirDistributionEffectivenessinCoolingMode

        value = _designSpecification->getDouble(DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinCoolingMode);
        if (value) {
          sizingZone.setDesignZoneAirDistributionEffectivenessinCoolingMode(value.get());
        }

        // ZoneAirDistributionEffectivenessinHeatingMode

        value = _designSpecification->getDouble(DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessinHeatingMode);
        if (value) {
          sizingZone.setDesignZoneAirDistributionEffectivenessinHeatingMode(value.get());
        }

        value = _designSpecification->getDouble(DesignSpecification_ZoneAirDistributionFields::ZoneSecondaryRecirculationFraction);
        if (value) {
          sizingZone.setDesignZoneSecondaryRecirculationFraction(value.get());
        }

        // MinimumZoneVentilationEfficiency

        value = _designSpecification->getDouble(DesignSpecification_ZoneAirDistributionFields::MinimumZoneVentilationEfficiency);
        if (value) {
          sizingZone.setDesignMinimumZoneVentilationEfficiency(value.get());
        }

        // Zone Air Distribution Effectiveness Schedule Name: not translated
        s = workspaceObject.getString(DesignSpecification_ZoneAirDistributionFields::ZoneAirDistributionEffectivenessScheduleName);

        if (s && !s->empty()) {
          LOG(Warn, _designSpecification->nameString()
                      << ", field Zone Air Distribution Effectiveness Schedule Name (='" << s.get() << "') isn't translated back to OS:Sizing:Zone");
        }
      }

      // Account for Dedicated Outdoor Air System: Optional Boolean
      if (boost::optional<std::string> _accountforDedicatedOutdoorAirSystem =
            workspaceObject.getString(Sizing_ZoneFields::AccountforDedicatedOutdoorAirSystem, true)) {
        if (istringEqual("Yes", _accountforDedicatedOutdoorAirSystem.get())) {
          sizingZone.setAccountforDedicatedOutdoorAirSystem(true);
        } else {
          sizingZone.setAccountforDedicatedOutdoorAirSystem(false);
        }
      }

      // Dedicated Outdoor Air System Control Strategy: Optional String
      if (boost::optional<std::string> _dedicatedOutdoorAirSystemControlStrategy =
            workspaceObject.getString(Sizing_ZoneFields::DedicatedOutdoorAirSystemControlStrategy)) {
        sizingZone.setDedicatedOutdoorAirSystemControlStrategy(_dedicatedOutdoorAirSystemControlStrategy.get());
      }

      // Dedicated Outdoor Air Low Setpoint Temperature for Design: Optional Double
      if (boost::optional<double> _dedicatedOutdoorAirLowSetpointTemperatureforDesign =
            workspaceObject.getDouble(Sizing_ZoneFields::DedicatedOutdoorAirLowSetpointTemperatureforDesign)) {
        sizingZone.setDedicatedOutdoorAirLowSetpointTemperatureforDesign(_dedicatedOutdoorAirLowSetpointTemperatureforDesign.get());
      }

      // Dedicated Outdoor Air High Setpoint Temperature for Design: Optional Double
      if (boost::optional<double> _dedicatedOutdoorAirHighSetpointTemperatureforDesign =
            workspaceObject.getDouble(Sizing_ZoneFields::DedicatedOutdoorAirHighSetpointTemperatureforDesign)) {
        sizingZone.setDedicatedOutdoorAirHighSetpointTemperatureforDesign(_dedicatedOutdoorAirHighSetpointTemperatureforDesign.get());
      }

      // Zone Load Sizing Method: Optional String
      if (boost::optional<std::string> _zoneLoadSizingMethod = workspaceObject.getString(Sizing_ZoneFields::ZoneLoadSizingMethod)) {
        sizingZone.setZoneLoadSizingMethod(_zoneLoadSizingMethod.get());
      }

      // Zone Latent Cooling Design Supply Air Humidity Ratio Input Method: Optional String
      if (boost::optional<std::string> _zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod =
            workspaceObject.getString(Sizing_ZoneFields::ZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod)) {
        sizingZone.setZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod(_zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod.get());
      }

      // Zone Dehumidification Design Supply Air Humidity Ratio: Optional Double
      if (boost::optional<double> _zoneDehumidificationDesignSupplyAirHumidityRatio =
            workspaceObject.getDouble(Sizing_ZoneFields::ZoneDehumidificationDesignSupplyAirHumidityRatio)) {
        sizingZone.setZoneDehumidificationDesignSupplyAirHumidityRatio(_zoneDehumidificationDesignSupplyAirHumidityRatio.get());
      }

      // Zone Cooling Design Supply Air Humidity Ratio Difference: Optional Double
      if (boost::optional<double> _zoneCoolingDesignSupplyAirHumidityRatioDifference =
            workspaceObject.getDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirHumidityRatioDifference)) {
        sizingZone.setZoneCoolingDesignSupplyAirHumidityRatioDifference(_zoneCoolingDesignSupplyAirHumidityRatioDifference.get());
      }

      // Zone Latent Heating Design Supply Air Humidity Ratio Input Method: Optional String
      if (boost::optional<std::string> _zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod =
            workspaceObject.getString(Sizing_ZoneFields::ZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod)) {
        sizingZone.setZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod(_zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod.get());
      }

      // Zone Humidification Design Supply Air Humidity Ratio: Optional Double
      if (boost::optional<double> _zoneHumidificationDesignSupplyAirHumidityRatio =
            workspaceObject.getDouble(Sizing_ZoneFields::ZoneHumidificationDesignSupplyAirHumidityRatio)) {
        sizingZone.setZoneHumidificationDesignSupplyAirHumidityRatio(_zoneHumidificationDesignSupplyAirHumidityRatio.get());
      }

      // Zone Humidification Design Supply Air Humidity Ratio Difference: Optional Double
      if (boost::optional<double> _zoneHumidificationDesignSupplyAirHumidityRatioDifference =
            workspaceObject.getDouble(Sizing_ZoneFields::ZoneHumidificationDesignSupplyAirHumidityRatioDifference)) {
        sizingZone.setZoneHumidificationDesignSupplyAirHumidityRatioDifference(_zoneHumidificationDesignSupplyAirHumidityRatioDifference.get());
      }

      // Zone Humidistat Dehumidification Set Point Schedule Name: Optional String
      if (auto _wo = workspaceObject.getTarget(Sizing_ZoneFields::ZoneHumidistatDehumidificationSetPointScheduleName)) {
        if (auto _mo = translateAndMapWorkspaceObject(_wo.get())) {
          if (auto sch_ = _mo->optionalCast<Schedule>()) {
            sizingZone.setZoneHumidistatDehumidificationSetPointSchedule(sch_.get());
          } else {
            LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Zone Humidistat Dehumidification Set Point Schedule Name'");
          }
        }
      }

      // Zone Humidistat Humidification Set Point Schedule Name: Optional String
      if (auto _wo = workspaceObject.getTarget(Sizing_ZoneFields::ZoneHumidistatHumidificationSetPointScheduleName)) {
        if (auto _mo = translateAndMapWorkspaceObject(_wo.get())) {
          if (auto sch_ = _mo->optionalCast<Schedule>()) {
            sizingZone.setZoneHumidistatHumidificationSetPointSchedule(sch_.get());
          } else {
            LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Zone Humidistat Huumidification Set Point Schedule Name'");
          }
        }
      }

      // TypeofSpaceSumtoUse
      s = workspaceObject.getString(Sizing_ZoneFields::TypeofSpaceSumtoUse);
      if (s) {
        sizingZone.setSizingOption(s.get());
      }
    }

    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
