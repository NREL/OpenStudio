/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
#include "../../model/Schedule.hpp"

#include "../../utilities/core/Assert.hpp"
#include "../../utilities/core/Compare.hpp"
#include "../../utilities/core/Logger.hpp"
#include "../../utilities/idd/IddEnums.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Sizing_Zone_FieldEnums.hxx>
#include <utilities/idd/DesignSpecification_ZoneAirDistribution_FieldEnums.hxx>
#include <utilities/idd/Controller_MechanicalVentilation_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

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

          if (!spaces.empty()) {
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

    // Zone Load Sizing Method: Optional String
    std::string zoneLoadSizingMethod = modelObject.zoneLoadSizingMethod();
    idfObject.setString(Sizing_ZoneFields::ZoneLoadSizingMethod, zoneLoadSizingMethod);

    // Zone Latent Cooling Design Supply Air Humidity Ratio Input Method: Optional String
    std::string zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod = modelObject.zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod();
    idfObject.setString(Sizing_ZoneFields::ZoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod,
                        zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod);
    if (openstudio::istringEqual(zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod, "SupplyAirHumidityRatio")) {

      // Zone Dehumidification Design Supply Air Humidity Ratio: boost::optional<double>
      if (boost::optional<double> _zoneDehumidificationDesignSupplyAirHumidityRatio =
            modelObject.zoneDehumidificationDesignSupplyAirHumidityRatio()) {
        idfObject.setDouble(Sizing_ZoneFields::ZoneDehumidificationDesignSupplyAirHumidityRatio,
                            _zoneDehumidificationDesignSupplyAirHumidityRatio.get());
      } else {
        LOG(Error, "For " << modelObject.briefDescription()
                          << ", when 'Zone Latent Cooling Design Supply Air Humidity Ratio Input Method' is 'SupplyAirHumidityRatio', you must enter "
                             "a 'Zone Dehumidification Design Supply Air Humidity Ratio'.");
      }
    } else if (openstudio::istringEqual(zoneLatentCoolingDesignSupplyAirHumidityRatioInputMethod, "HumidityRatioDifference")) {
      // Zone Cooling Design Supply Air Humidity Ratio Difference: Optional Double
      double zoneCoolingDesignSupplyAirHumidityRatioDifference = modelObject.zoneCoolingDesignSupplyAirHumidityRatioDifference();
      idfObject.setDouble(Sizing_ZoneFields::ZoneCoolingDesignSupplyAirHumidityRatioDifference, zoneCoolingDesignSupplyAirHumidityRatioDifference);
    } else {
      OS_ASSERT(false);
    }

    // Zone Latent Heating Design Supply Air Humidity Ratio Input Method: Optional String
    std::string zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod = modelObject.zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod();
    idfObject.setString(Sizing_ZoneFields::ZoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod,
                        zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod);

    if (openstudio::istringEqual(zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod, "SupplyAirHumidityRatio")) {

      // Zone Humidification Design Supply Air Humidity Ratio: boost::optional<double>
      if (boost::optional<double> d_ = modelObject.zoneHumidificationDesignSupplyAirHumidityRatio()) {
        idfObject.setDouble(Sizing_ZoneFields::ZoneHumidificationDesignSupplyAirHumidityRatio, d_.get());
      } else {
        LOG(Error, "For " << modelObject.briefDescription()
                          << ", when 'Zone Latent Heating Design Supply Air Humidity Ratio Input Method' is 'SupplyAirHumidityRatio', you must enter "
                             "a 'Zone Dehumidification Design Supply Air Humidity Ratio'.");
      }
    } else if (openstudio::istringEqual(zoneLatentHeatingDesignSupplyAirHumidityRatioInputMethod, "HumidityRatioDifference")) {
      // Zone Humidification Design Supply Air Humidity Ratio Difference: Optional Double
      idfObject.setDouble(Sizing_ZoneFields::ZoneHumidificationDesignSupplyAirHumidityRatioDifference,
                          modelObject.zoneHumidificationDesignSupplyAirHumidityRatioDifference());
    } else {
      OS_ASSERT(false);
    }

    if (!openstudio::istringEqual(zoneLoadSizingMethod, "Sensible Load Only No Latent Load")) {
      // Zone Humidistat Dehumidification Set Point Schedule Name
      if (boost::optional<Schedule> sch_ = modelObject.zoneHumidistatDehumidificationSetPointSchedule()) {
        if (auto idf_sch_ = translateAndMapModelObject(sch_.get())) {
          idfObject.setString(Sizing_ZoneFields::ZoneHumidistatDehumidificationSetPointScheduleName, idf_sch_->nameString());
        }
      }

      // Zone Humidistat Humidification Set Point Schedule Name
      if (boost::optional<Schedule> sch_ = modelObject.zoneHumidistatHumidificationSetPointSchedule()) {
        if (auto idf_sch_ = translateAndMapModelObject(sch_.get())) {

          idfObject.setString(Sizing_ZoneFields::ZoneHumidistatHumidificationSetPointScheduleName, idf_sch_->nameString());
        }
      }
    }

    s = modelObject.sizingOption();
    if (s) {
      idfObject.setString(Sizing_ZoneFields::TypeofSpaceSumtoUse, s.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
