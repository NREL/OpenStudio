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

#include "../../model/SwimmingPoolIndoor.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../../model/FloorSurface.hpp"
#include "../../model/FloorSurface_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include <utilities/idd/SwimmingPool_Indoor_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSwimmingPoolIndoor( model::SwimmingPoolIndoor& modelObject )
{
  boost::optional<WorkspaceObject> result;
  boost::optional<WorkspaceObject> _wo;
  boost::optional<ModelObject> _mo;

  // Instantiate an IdfObject of the class to store the values,
  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::SwimmingPool_Indoor, modelObject);
  // If it doesn't have a name, or if you aren't sure you are going to want to return it
  // IdfObject idfObject( openstudio::IddObjectType::SwimmingPool_Indoor );
  // m_idfObjects.push_back(idfObject);

  // TODO: Note JM 2018-10-17
  // You are responsible for implementing any additional logic based on choice fields, etc.
  // The ForwardTranslator generator script is meant to facilitate your work, not get you 100% of the way

  // TODO: If you keep createRegisterAndNameIdfObject above, you don't need this.
  // But in some cases, you'll want to handle failure without pushing to the map
  // Name
  if (boost::optional<std::string> moName = modelObject.name()) {
    idfObject.setName(*moName);
  }

  // Surface Name: Required Object
  FloorSurface surface = modelObject.surface();  if ( boost::optional<WorkspaceObject> _owo = translateAndMapModelObject(surface) )  {
    idfObject.setString(SwimmingPool_IndoorFields::SurfaceName, _owo->nameString());
  }
  if ( (_wo = workspaceObject.getTarget(SwimmingPool_IndoorFields::SurfaceName)) ) {
    if( (_mo = translateAndMapWorkspaceObject(_wo.get())) ) {
      // TODO: check return types
      if (boost::optional<FloorSurface> _surface = _mo->optionalCast<FloorSurface>()) {
        modelObject.setSurface(_surface.get());
      } else {
        LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Surface Name'");
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Surface Name'");
      return result;
    }
  } else {
    LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Surface Name'");
    return result;
  }

  // Average Depth: Required Double
  double averageDepth = modelObject.averageDepth();
  idfObject.setDouble(SwimmingPool_IndoorFields::AverageDepth, averageDepth)


  // Activity Factor Schedule Name: Required Object
  Schedule activityFactorSchedule = modelObject.activityFactorSchedule();  if ( boost::optional<WorkspaceObject> _owo = translateAndMapModelObject(activityFactorSchedule) )  {
    idfObject.setString(SwimmingPool_IndoorFields::ActivityFactorScheduleName, _owo->nameString());
  }
  if ( (_wo = workspaceObject.getTarget(SwimmingPool_IndoorFields::ActivityFactorScheduleName)) ) {
    if( (_mo = translateAndMapWorkspaceObject(_wo.get())) ) {
      // TODO: check return types
      if (boost::optional<Schedule> _activityFactorSchedule = _mo->optionalCast<Schedule>()) {
        modelObject.setActivityFactorSchedule(_activityFactorSchedule.get());
      } else {
        LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Activity Factor Schedule Name'");
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Activity Factor Schedule Name'");
      return result;
    }
  } else {
    LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Activity Factor Schedule Name'");
    return result;
  }

  // Make-up Water Supply Schedule Name: Required Object
  Schedule makeupWaterSupplySchedule = modelObject.makeupWaterSupplySchedule();  if ( boost::optional<WorkspaceObject> _owo = translateAndMapModelObject(makeupWaterSupplySchedule) )  {
    idfObject.setString(SwimmingPool_IndoorFields::MakeupWaterSupplyScheduleName, _owo->nameString());
  }
  if ( (_wo = workspaceObject.getTarget(SwimmingPool_IndoorFields::MakeupWaterSupplyScheduleName)) ) {
    if( (_mo = translateAndMapWorkspaceObject(_wo.get())) ) {
      // TODO: check return types
      if (boost::optional<Schedule> _makeupWaterSupplySchedule = _mo->optionalCast<Schedule>()) {
        modelObject.setMakeupWaterSupplySchedule(_makeupWaterSupplySchedule.get());
      } else {
        LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Make-up Water Supply Schedule Name'");
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Make-up Water Supply Schedule Name'");
      return result;
    }
  } else {
    LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Make-up Water Supply Schedule Name'");
    return result;
  }

  // Cover Schedule Name: Required Object
  Schedule coverSchedule = modelObject.coverSchedule();  if ( boost::optional<WorkspaceObject> _owo = translateAndMapModelObject(coverSchedule) )  {
    idfObject.setString(SwimmingPool_IndoorFields::CoverScheduleName, _owo->nameString());
  }
  if ( (_wo = workspaceObject.getTarget(SwimmingPool_IndoorFields::CoverScheduleName)) ) {
    if( (_mo = translateAndMapWorkspaceObject(_wo.get())) ) {
      // TODO: check return types
      if (boost::optional<Schedule> _coverSchedule = _mo->optionalCast<Schedule>()) {
        modelObject.setCoverSchedule(_coverSchedule.get());
      } else {
        LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Cover Schedule Name'");
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Cover Schedule Name'");
      return result;
    }
  } else {
    LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Cover Schedule Name'");
    return result;
  }

  // Cover Evaporation Factor: Optional Double
  double coverEvaporationFactor = modelObject.coverEvaporationFactor();
  idfObject.setDouble(SwimmingPool_IndoorFields::CoverEvaporationFactor, coverEvaporationFactor)


  // Cover Convection Factor: Optional Double
  double coverConvectionFactor = modelObject.coverConvectionFactor();
  idfObject.setDouble(SwimmingPool_IndoorFields::CoverConvectionFactor, coverConvectionFactor)


  // Cover Short-Wavelength Radiation Factor: Optional Double
  double coverShortWavelengthRadiationFactor = modelObject.coverShortWavelengthRadiationFactor();
  idfObject.setDouble(SwimmingPool_IndoorFields::CoverShortWavelengthRadiationFactor, coverShortWavelengthRadiationFactor)


  // Cover Long-Wavelength Radiation Factor: Optional Double
  double coverLongWavelengthRadiationFactor = modelObject.coverLongWavelengthRadiationFactor();
  idfObject.setDouble(SwimmingPool_IndoorFields::CoverLongWavelengthRadiationFactor, coverLongWavelengthRadiationFactor)


  // Pool Water Inlet Node: Required Node
  Node poolWaterInletNode = modelObject.poolWaterInletNode();  if ( boost::optional<WorkspaceObject> _owo = translateAndMapModelObject(poolWaterInletNode) )  {
    idfObject.setString(SwimmingPool_IndoorFields::PoolWaterInletNode, _owo->nameString());
  }
  if ( (_wo = workspaceObject.getTarget(SwimmingPool_IndoorFields::PoolWaterInletNode)) ) {
    if( (_mo = translateAndMapWorkspaceObject(_wo.get())) ) {
      // TODO: check return types
      if (boost::optional<Node> _poolWaterInletNode = _mo->optionalCast<Node>()) {
        modelObject.setPoolWaterInletNode(_poolWaterInletNode.get());
      } else {
        LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Pool Water Inlet Node'");
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Pool Water Inlet Node'");
      return result;
    }
  } else {
    LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Pool Water Inlet Node'");
    return result;
  }

  // Pool Water Outlet Node: Required Node
  Node poolWaterOutletNode = modelObject.poolWaterOutletNode();  if ( boost::optional<WorkspaceObject> _owo = translateAndMapModelObject(poolWaterOutletNode) )  {
    idfObject.setString(SwimmingPool_IndoorFields::PoolWaterOutletNode, _owo->nameString());
  }
  if ( (_wo = workspaceObject.getTarget(SwimmingPool_IndoorFields::PoolWaterOutletNode)) ) {
    if( (_mo = translateAndMapWorkspaceObject(_wo.get())) ) {
      // TODO: check return types
      if (boost::optional<Node> _poolWaterOutletNode = _mo->optionalCast<Node>()) {
        modelObject.setPoolWaterOutletNode(_poolWaterOutletNode.get());
      } else {
        LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Pool Water Outlet Node'");
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Pool Water Outlet Node'");
      return result;
    }
  } else {
    LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Pool Water Outlet Node'");
    return result;
  }

  // Pool Heating System Maximum Water Flow Rate: boost::optional<double>
  if (boost::optional<double> _poolHeatingSystemMaximumWaterFlowRate = modelObject.poolHeatingSystemMaximumWaterFlowRate()) {
    idfObject.setDouble(SwimmingPool_IndoorFields::PoolHeatingSystemMaximumWaterFlowRate, _poolHeatingSystemMaximumWaterFlowRate.get())
  }

  // Pool Miscellaneous Equipment Power: boost::optional<double>
  if (boost::optional<double> _poolMiscellaneousEquipmentPower = modelObject.poolMiscellaneousEquipmentPower()) {
    idfObject.setDouble(SwimmingPool_IndoorFields::PoolMiscellaneousEquipmentPower, _poolMiscellaneousEquipmentPower.get())
  }

  // Setpoint Temperature Schedule: Required Object
  Schedule setpointTemperatureSchedule = modelObject.setpointTemperatureSchedule();  if ( boost::optional<WorkspaceObject> _owo = translateAndMapModelObject(setpointTemperatureSchedule) )  {
    idfObject.setString(SwimmingPool_IndoorFields::SetpointTemperatureSchedule, _owo->nameString());
  }
  if ( (_wo = workspaceObject.getTarget(SwimmingPool_IndoorFields::SetpointTemperatureSchedule)) ) {
    if( (_mo = translateAndMapWorkspaceObject(_wo.get())) ) {
      // TODO: check return types
      if (boost::optional<Schedule> _setpointTemperatureSchedule = _mo->optionalCast<Schedule>()) {
        modelObject.setSetpointTemperatureSchedule(_setpointTemperatureSchedule.get());
      } else {
        LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Setpoint Temperature Schedule'");
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Setpoint Temperature Schedule'");
      return result;
    }
  } else {
    LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Setpoint Temperature Schedule'");
    return result;
  }

  // Maximum Number of People: Required Double
  double maximumNumberofPeople = modelObject.maximumNumberofPeople();
  idfObject.setDouble(SwimmingPool_IndoorFields::MaximumNumberofPeople, maximumNumberofPeople)


  // People Schedule: Optional Object
  if (boost::optional<Schedule> _peopleSchedule = peopleSchedule()) {
    if ( boost::optional<WorkspaceObject> _owo = translateAndMapModelObject(_peopleSchedule.get()) )  {
      idfObject.setString(SwimmingPool_IndoorFields::PeopleSchedule, _owo->nameString());
    }
  }
  if ( (_wo = workspaceObject.getTarget(SwimmingPool_IndoorFields::PeopleSchedule)) ) {
    if( (_mo = translateAndMapWorkspaceObject(_wo.get())) ) {
      // TODO: check return types
      if (boost::optional<Schedule> _peopleSchedule = _mo->optionalCast<Schedule>()) {
        modelObject.setPeopleSchedule(_peopleSchedule.get());
      } else {
        LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'People Schedule'");
      }
    }
  }

  // People Heat Gain Schedule: Optional Object
  if (boost::optional<Schedule> _peopleHeatGainSchedule = peopleHeatGainSchedule()) {
    if ( boost::optional<WorkspaceObject> _owo = translateAndMapModelObject(_peopleHeatGainSchedule.get()) )  {
      idfObject.setString(SwimmingPool_IndoorFields::PeopleHeatGainSchedule, _owo->nameString());
    }
  }
  if ( (_wo = workspaceObject.getTarget(SwimmingPool_IndoorFields::PeopleHeatGainSchedule)) ) {
    if( (_mo = translateAndMapWorkspaceObject(_wo.get())) ) {
      // TODO: check return types
      if (boost::optional<Schedule> _peopleHeatGainSchedule = _mo->optionalCast<Schedule>()) {
        modelObject.setPeopleHeatGainSchedule(_peopleHeatGainSchedule.get());
      } else {
        LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'People Heat Gain Schedule'");
      }
    }
  }

  result = modelObject;
  return result;
} // End of translate function

} // end namespace energyplus
} // end namespace openstudio