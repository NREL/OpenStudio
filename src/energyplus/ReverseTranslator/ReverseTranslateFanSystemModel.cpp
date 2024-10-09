/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/FanSystemModel.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Curve.hpp"
#include "../../model/Curve_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Fan_SystemModel_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateFanSystemModel(const WorkspaceObject& workspaceObject) {
    boost::optional<ModelObject> result;
    boost::optional<WorkspaceObject> _wo;
    boost::optional<ModelObject> _mo;

    // Instantiate an object of the class to store the values,
    // but we don't return it until we know it's ok
    openstudio::model::FanSystemModel modelObject(m_model);

    // Name
    if (boost::optional<std::string> _name = workspaceObject.name()) {
      modelObject.setName(_name.get());
    }

    // Availability Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(Fan_SystemModelFields::AvailabilityScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _availabilitySchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setAvailabilitySchedule(_availabilitySchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Availability Schedule Name'");
        }
      }
    }

    // Air Inlet Node Name: Required Node

    // Air Outlet Node Name: Required Node

    // Design Maximum Air Flow Rate: Required Double
    if (boost::optional<double> _designMaximumAirFlowRate = workspaceObject.getDouble(Fan_SystemModelFields::DesignMaximumAirFlowRate)) {
      modelObject.setDesignMaximumAirFlowRate(_designMaximumAirFlowRate.get());
    } else {
      modelObject.autosizeDesignMaximumAirFlowRate();
    }

    // Speed Control Method: Optional String
    if (boost::optional<std::string> _speedControlMethod = workspaceObject.getString(Fan_SystemModelFields::SpeedControlMethod)) {
      modelObject.setSpeedControlMethod(_speedControlMethod.get());
    }

    // Electric Power Minimum Flow Rate Fraction: Optional Double
    if (boost::optional<double> _electricPowerMinimumFlowRateFraction =
          workspaceObject.getDouble(Fan_SystemModelFields::ElectricPowerMinimumFlowRateFraction)) {
      modelObject.setElectricPowerMinimumFlowRateFraction(_electricPowerMinimumFlowRateFraction.get());
    }

    // Design Pressure Rise: Required Double
    if (boost::optional<double> _designPressureRise = workspaceObject.getDouble(Fan_SystemModelFields::DesignPressureRise)) {
      modelObject.setDesignPressureRise(_designPressureRise.get());
    } else {
      // required, without a sensible default... so throw
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Design Pressure Rise'");
      return result;
    }

    // Motor Efficiency: Optional Double
    if (boost::optional<double> _motorEfficiency = workspaceObject.getDouble(Fan_SystemModelFields::MotorEfficiency)) {
      modelObject.setMotorEfficiency(_motorEfficiency.get());
    }

    // Motor In Air Stream Fraction: Optional Double
    if (boost::optional<double> _motorInAirStreamFraction = workspaceObject.getDouble(Fan_SystemModelFields::MotorInAirStreamFraction)) {
      modelObject.setMotorInAirStreamFraction(_motorInAirStreamFraction.get());
    }

    // Design Electric Power Consumption: Optional Double
    if (boost::optional<double> _designElectricPowerConsumption = workspaceObject.getDouble(Fan_SystemModelFields::DesignElectricPowerConsumption)) {
      modelObject.setDesignElectricPowerConsumption(_designElectricPowerConsumption.get());
    } else {
      modelObject.autosizeDesignElectricPowerConsumption();
    }

    // Design Power Sizing Method: Optional String
    if (boost::optional<std::string> _designPowerSizingMethod = workspaceObject.getString(Fan_SystemModelFields::DesignPowerSizingMethod)) {
      modelObject.setDesignPowerSizingMethod(_designPowerSizingMethod.get());
    }

    // Electric Power Per Unit Flow Rate: Optional Double
    if (boost::optional<double> _electricPowerPerUnitFlowRate = workspaceObject.getDouble(Fan_SystemModelFields::ElectricPowerPerUnitFlowRate)) {
      modelObject.setElectricPowerPerUnitFlowRate(_electricPowerPerUnitFlowRate.get());
    }

    // Electric Power Per Unit Flow Rate Per Unit Pressure: Optional Double
    if (boost::optional<double> _electricPowerPerUnitFlowRatePerUnitPressure =
          workspaceObject.getDouble(Fan_SystemModelFields::ElectricPowerPerUnitFlowRatePerUnitPressure)) {
      modelObject.setElectricPowerPerUnitFlowRatePerUnitPressure(_electricPowerPerUnitFlowRatePerUnitPressure.get());
    }

    // Fan Total Efficiency: Optional Double
    if (boost::optional<double> _fanTotalEfficiency = workspaceObject.getDouble(Fan_SystemModelFields::FanTotalEfficiency)) {
      modelObject.setFanTotalEfficiency(_fanTotalEfficiency.get());
    }

    // Electric Power Function of Flow Fraction Curve Name: Optional Object
    if ((_wo = workspaceObject.getTarget(Fan_SystemModelFields::ElectricPowerFunctionofFlowFractionCurveName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Curve> _electricPowerFunctionofFlowFractionCurve = _mo->optionalCast<Curve>()) {
          modelObject.setElectricPowerFunctionofFlowFractionCurve(_electricPowerFunctionofFlowFractionCurve.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Electric Power Function of Flow Fraction Curve Name'");
        }
      }
    }

    // Night Ventilation Mode Pressure Rise: Optional Double
    if (boost::optional<double> _nightVentilationModePressureRise =
          workspaceObject.getDouble(Fan_SystemModelFields::NightVentilationModePressureRise)) {
      modelObject.setNightVentilationModePressureRise(_nightVentilationModePressureRise.get());
    }

    // Night Ventilation Mode Flow Fraction: Optional Double
    if (boost::optional<double> _nightVentilationModeFlowFraction =
          workspaceObject.getDouble(Fan_SystemModelFields::NightVentilationModeFlowFraction)) {
      modelObject.setNightVentilationModeFlowFraction(_nightVentilationModeFlowFraction.get());
    }

    // Motor Loss Zone Name: Optional Object
    if ((_wo = workspaceObject.getTarget(Fan_SystemModelFields::MotorLossZoneName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // Zone is translated, and a Space is returned instead
        if (boost::optional<Space> space_ = _mo->optionalCast<Space>()) {
          if (auto z_ = space_->thermalZone()) {
            modelObject.setMotorLossZone(z_.get());
          }
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Motor Loss Zone Name'");
        }
      }
    }

    // Motor Loss Radiative Fraction: Optional Double
    if (boost::optional<double> _motorLossRadiativeFraction = workspaceObject.getDouble(Fan_SystemModelFields::MotorLossRadiativeFraction)) {
      modelObject.setMotorLossRadiativeFraction(_motorLossRadiativeFraction.get());
    }

    // End-Use Subcategory: Optional String
    if (boost::optional<std::string> _endUseSubcategory = workspaceObject.getString(Fan_SystemModelFields::EndUseSubcategory)) {
      modelObject.setEndUseSubcategory(_endUseSubcategory.get());
    }

    // Number of Speeds: Optional Integer
    std::vector<IdfExtensibleGroup> extensibleGroups = workspaceObject.extensibleGroups();

    boost::optional<int> _numberofSpeeds = workspaceObject.getInt(Fan_SystemModelFields::NumberofSpeeds);
    int nExts = extensibleGroups.size();
    int nspeeds = 0;
    if (_numberofSpeeds) {
      nspeeds = _numberofSpeeds.get();
      if (nExts < nspeeds) {
        LOG(Warn, workspaceObject.briefDescription() << " lists " << nspeeds << " Speeds, but has only " << nExts << " extensibleGroups.");
        nspeeds = nExts;
      } else if (nExts > nspeeds) {
        LOG(Warn, workspaceObject.briefDescription() << " lists " << nspeeds << " Speeds, but has more with " << nExts
                                                     << " extensibleGroups. Only the " << nspeeds << " first ones will be used.");
      }
    }

    std::vector<FanSystemModelSpeed> speeds;
    for (auto i = 0; i < nspeeds; ++i) {
      boost::optional<double> _flowFraction = extensibleGroups[i].getDouble(Fan_SystemModelExtensibleFields::SpeedFlowFraction);
      boost::optional<double> _electricPowerFraction = extensibleGroups[i].getDouble(Fan_SystemModelExtensibleFields::SpeedElectricPowerFraction);
      if (_flowFraction.has_value() && _electricPowerFraction.has_value()) {

        // try to create a Speed, which will throw in case wrong type, or wrong double value. Here we let it slide though
        try {
          speeds.push_back(FanSystemModelSpeed(_flowFraction.get(), _electricPowerFraction.get()));
        } catch (...) {
          // The Speed Ctor threw, so there's a wrong type, or a wrong double
          LOG(Error, "Could not create Speed " << i << " in FanSystemModel " << modelObject.nameString());
        }

      } else {
        LOG(Error, "Speed " << i << " is invalid, both flow Fraction and Electric Power Fraction are required. Ignoring.");
      }
    }
    modelObject.setSpeeds(speeds);

    result = modelObject;
    return result;
  }  // End of translate function

}  // end namespace energyplus

}  // end namespace openstudio
