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

#include "../ReverseTranslator.hpp"

#include "../../model/FanSystemModel.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include "../../model/UnivariateFunctions.hpp"
#include "../../model/UnivariateFunctions_Impl.hpp"

#include "../../model/Zone.hpp"
#include "../../model/Zone_Impl.hpp"

#include <utilities/idd/Fan_SystemModel_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<ModelObject> ReverseTranslator::translateFanSystemModel( const WorkspaceObject & workspaceObject )
{
  boost::optional<ModelObject> result;
  boost::optional<WorkspaceObject> _wo;
  boost::optional<ModelObject> _mo;

  // Instantiate an object of the class to store the values,
  // but we don't return it until we know it's ok
  // TODO: check constructor, it might need other objects
  openstudio::model::FanSystemModel modelObject( m_model );

  // TODO: Note JM 2018-10-17
  // You are responsible for implementing any additional logic based on choice fields, etc.
  // The ReverseTranslator generator script is meant to facilitate your work, not get you 100% of the way

  // Name
  if (boost::optional<std::string> _name = workspaceObject.name()) {
    modelObject.setName(_name.get());
  }

  // Availability Schedule Name: Optional Object
  if ( (_wo = workspaceObject.getTarget(Fan_SystemModelFields::AvailabilityScheduleName)) ) {
    if( (_mo = translateAndMapWorkspaceObject(_wo.get())) ) {
      // TODO: check return types
      if (boost::optional<Schedule> _availabilitySchedule = _mo->optionalCast<Schedule>()) {
        modelObject.setAvailabilitySchedule(_availabilitySchedule.get());
      } else {
        LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Availability Schedule Name'");
      }
    }
  }
  // Air Inlet Node Name: Required Node
  if ( (_wo = workspaceObject.getTarget(Fan_SystemModelFields::AirInletNodeName)) ) {
    if( (_mo = translateAndMapWorkspaceObject(_wo.get())) ) {
      // TODO: check return types
      if (boost::optional<Node> _airInletNodeName = _mo->optionalCast<Node>()) {
        modelObject.setAirInletNodeName(_airInletNodeName.get());
      } else {
        LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Air Inlet Node Name'");
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Air Inlet Node Name'");
      return result;
    }
  } else {
    LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Air Inlet Node Name'");
    return result;
  }
  // Air Outlet Node Name: Required Node
  if ( (_wo = workspaceObject.getTarget(Fan_SystemModelFields::AirOutletNodeName)) ) {
    if( (_mo = translateAndMapWorkspaceObject(_wo.get())) ) {
      // TODO: check return types
      if (boost::optional<Node> _airOutletNodeName = _mo->optionalCast<Node>()) {
        modelObject.setAirOutletNodeName(_airOutletNodeName.get());
      } else {
        LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Air Outlet Node Name'");
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Air Outlet Node Name'");
      return result;
    }
  } else {
    LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Air Outlet Node Name'");
    return result;
  }
  // Design Maximum Air Flow Rate: Required Double
  if (boost::optional<double> _designMaximumAirFlowRate = getDouble(Fan_SystemModelFields::DesignMaximumAirFlowRate)) {
    modelObject.setDesignMaximumAirFlowRate(_designMaximumAirFlowRate.get());
 } else {
   LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Design Maximum Air Flow Rate'");
    return result;
  }

  // Speed Control Method: Optional String
  if (boost::optional<std::string> _speedControlMethod = getString(Fan_SystemModelFields::SpeedControlMethod)) {
    modelObject.setSpeedControlMethod(_speedControlMethod.get());
  }

  // Electric Power Minimum Flow Rate Fraction: Optional Double
  if (boost::optional<double> _electricPowerMinimumFlowRateFraction = getDouble(Fan_SystemModelFields::ElectricPowerMinimumFlowRateFraction)) {
    modelObject.setElectricPowerMinimumFlowRateFraction(_electricPowerMinimumFlowRateFraction.get());
  }

  // Design Pressure Rise: Required Double
  if (boost::optional<double> _designPressureRise = getDouble(Fan_SystemModelFields::DesignPressureRise)) {
    modelObject.setDesignPressureRise(_designPressureRise.get());
 } else {
   LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Design Pressure Rise'");
    return result;
  }

  // Motor Efficiency: Optional Double
  if (boost::optional<double> _motorEfficiency = getDouble(Fan_SystemModelFields::MotorEfficiency)) {
    modelObject.setMotorEfficiency(_motorEfficiency.get());
  }

  // Motor In Air Stream Fraction: Optional Double
  if (boost::optional<double> _motorInAirStreamFraction = getDouble(Fan_SystemModelFields::MotorInAirStreamFraction)) {
    modelObject.setMotorInAirStreamFraction(_motorInAirStreamFraction.get());
  }

  // Design Electric Power Consumption: Optional Double
  if (boost::optional<double> _designElectricPowerConsumption = getDouble(Fan_SystemModelFields::DesignElectricPowerConsumption)) {
    modelObject.setDesignElectricPowerConsumption(_designElectricPowerConsumption.get());
  }

  // Design Power Sizing Method: Optional String
  if (boost::optional<std::string> _designPowerSizingMethod = getString(Fan_SystemModelFields::DesignPowerSizingMethod)) {
    modelObject.setDesignPowerSizingMethod(_designPowerSizingMethod.get());
  }

  // Electric Power Per Unit Flow Rate: Optional Double
  if (boost::optional<double> _electricPowerPerUnitFlowRate = getDouble(Fan_SystemModelFields::ElectricPowerPerUnitFlowRate)) {
    modelObject.setElectricPowerPerUnitFlowRate(_electricPowerPerUnitFlowRate.get());
  }

  // Electric Power Per Unit Flow Rate Per Unit Pressure: Optional Double
  if (boost::optional<double> _electricPowerPerUnitFlowRatePerUnitPressure = getDouble(Fan_SystemModelFields::ElectricPowerPerUnitFlowRatePerUnitPressure)) {
    modelObject.setElectricPowerPerUnitFlowRatePerUnitPressure(_electricPowerPerUnitFlowRatePerUnitPressure.get());
  }

  // Fan Total Efficiency: Optional Double
  if (boost::optional<double> _fanTotalEfficiency = getDouble(Fan_SystemModelFields::FanTotalEfficiency)) {
    modelObject.setFanTotalEfficiency(_fanTotalEfficiency.get());
  }

  // Electric Power Function of Flow Fraction Curve Name: Optional Object
  if ( (_wo = workspaceObject.getTarget(Fan_SystemModelFields::ElectricPowerFunctionofFlowFractionCurveName)) ) {
    if( (_mo = translateAndMapWorkspaceObject(_wo.get())) ) {
      // TODO: check return types
      if (boost::optional<UnivariateFunctions> _electricPowerFunctionofFlowFractionCurve = _mo->optionalCast<UnivariateFunctions>()) {
        modelObject.setElectricPowerFunctionofFlowFractionCurve(_electricPowerFunctionofFlowFractionCurve.get());
      } else {
        LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Electric Power Function of Flow Fraction Curve Name'");
      }
    }
  }
  // Night Ventilation Mode Pressure Rise: Optional Double
  if (boost::optional<double> _nightVentilationModePressureRise = getDouble(Fan_SystemModelFields::NightVentilationModePressureRise)) {
    modelObject.setNightVentilationModePressureRise(_nightVentilationModePressureRise.get());
  }

  // Night Ventilation Mode Flow Fraction: Optional Double
  if (boost::optional<double> _nightVentilationModeFlowFraction = getDouble(Fan_SystemModelFields::NightVentilationModeFlowFraction)) {
    modelObject.setNightVentilationModeFlowFraction(_nightVentilationModeFlowFraction.get());
  }

  // Motor Loss Zone Name: Optional Object
  if ( (_wo = workspaceObject.getTarget(Fan_SystemModelFields::MotorLossZoneName)) ) {
    if( (_mo = translateAndMapWorkspaceObject(_wo.get())) ) {
      // TODO: check return types
      if (boost::optional<Zone> _motorLossZone = _mo->optionalCast<Zone>()) {
        modelObject.setMotorLossZone(_motorLossZone.get());
      } else {
        LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Motor Loss Zone Name'");
      }
    }
  }
  // Motor Loss Radiative Fraction: Optional Double
  if (boost::optional<double> _motorLossRadiativeFraction = getDouble(Fan_SystemModelFields::MotorLossRadiativeFraction)) {
    modelObject.setMotorLossRadiativeFraction(_motorLossRadiativeFraction.get());
  }

  // End-Use Subcategory: Optional String
  if (boost::optional<std::string> _endUseSubcategory = getString(Fan_SystemModelFields::EndUseSubcategory)) {
    modelObject.setEndUseSubcategory(_endUseSubcategory.get());
  }

  // Number of Speeds: Optional Integer
  if (boost::optional<int> _numberofSpeeds = getInt(Fan_SystemModelFields::NumberofSpeeds)) {
    modelObject.setNumberofSpeeds(_numberofSpeeds.get());
  }

  result = modelObject;
  return result;
}
} // End of translate function

} // end namespace energyplus

} // end namespace openstudio