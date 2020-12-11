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

#include "../ReverseTranslator.hpp"

#include "../../model/SwimmingPoolIndoor.hpp"

#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include <utilities/idd/SwimmingPool_Indoor_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateSwimmingPoolIndoor(const WorkspaceObject& workspaceObject) {
    boost::optional<WorkspaceObject> _wo;
    boost::optional<ModelObject> _mo;

    // Surface Name: Required Object, and as Ctor arg. And will throw if not the right SurfaceType...
    boost::optional<Surface> _surface;
    if ((_wo = workspaceObject.getTarget(SwimmingPool_IndoorFields::SurfaceName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if ((_surface = _mo->optionalCast<Surface>())) {
          if (_surface->surfaceType() != "Floor") {
            LOG(Warn, workspaceObject.briefDescription() << " has a surface assigned, but it is NOT of type 'Floor' like it should");
            return ::boost::none;
          }
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Surface Name'");
          return boost::none;
        }
      } else {
        LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Surface Name'");
        return boost::none;
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Surface Name'");
      return boost::none;
    }

    // Instantiate an object of the class to store the values,
    // but we don't return it until we know it's ok
    openstudio::model::SwimmingPoolIndoor modelObject(m_model, _surface.get());

    // Name
    if (boost::optional<std::string> _name = workspaceObject.name()) {
      modelObject.setName(_name.get());
    }

    // Average Depth: Required Double
    if (boost::optional<double> _averageDepth = workspaceObject.getDouble(SwimmingPool_IndoorFields::AverageDepth)) {
      modelObject.setAverageDepth(_averageDepth.get());
    } else {
      LOG(Warn, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Average Depth', using default");
      return boost::none;
    }

    // Activity Factor Schedule Name: Required Object
    if ((_wo = workspaceObject.getTarget(SwimmingPool_IndoorFields::ActivityFactorScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _activityFactorSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setActivityFactorSchedule(_activityFactorSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Activity Factor Schedule Name'");
        }
      } else {
        LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Activity Factor Schedule Name'");
        return boost::none;
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Activity Factor Schedule Name'");
      return boost::none;
    }
    // Make-up Water Supply Schedule Name: Required Object
    if ((_wo = workspaceObject.getTarget(SwimmingPool_IndoorFields::MakeupWaterSupplyScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _makeupWaterSupplySchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setMakeupWaterSupplySchedule(_makeupWaterSupplySchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Make-up Water Supply Schedule Name'");
        }
      } else {
        LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Make-up Water Supply Schedule Name'");
        return boost::none;
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Make-up Water Supply Schedule Name'");
      return boost::none;
    }
    // Cover Schedule Name: Required Object
    if ((_wo = workspaceObject.getTarget(SwimmingPool_IndoorFields::CoverScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _coverSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setCoverSchedule(_coverSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Cover Schedule Name'");
        }
      } else {
        LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Cover Schedule Name'");
        return boost::none;
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Cover Schedule Name'");
      return boost::none;
    }
    // Cover Evaporation Factor: Optional Double
    if (boost::optional<double> _coverEvaporationFactor = workspaceObject.getDouble(SwimmingPool_IndoorFields::CoverEvaporationFactor)) {
      modelObject.setCoverEvaporationFactor(_coverEvaporationFactor.get());
    }

    // Cover Convection Factor: Optional Double
    if (boost::optional<double> _coverConvectionFactor = workspaceObject.getDouble(SwimmingPool_IndoorFields::CoverConvectionFactor)) {
      modelObject.setCoverConvectionFactor(_coverConvectionFactor.get());
    }

    // Cover Short-Wavelength Radiation Factor: Optional Double
    if (boost::optional<double> _coverShortWavelengthRadiationFactor =
          workspaceObject.getDouble(SwimmingPool_IndoorFields::CoverShortWavelengthRadiationFactor)) {
      modelObject.setCoverShortWavelengthRadiationFactor(_coverShortWavelengthRadiationFactor.get());
    }

    // Cover Long-Wavelength Radiation Factor: Optional Double
    if (boost::optional<double> _coverLongWavelengthRadiationFactor =
          workspaceObject.getDouble(SwimmingPool_IndoorFields::CoverLongWavelengthRadiationFactor)) {
      modelObject.setCoverLongWavelengthRadiationFactor(_coverLongWavelengthRadiationFactor.get());
    }

    // Pool Water Inlet Node: Required Node

    // Pool Water Outlet Node: Required Node

    // Pool Heating System Maximum Water Flow Rate: Optional Double
    if (boost::optional<double> _poolHeatingSystemMaximumWaterFlowRate =
          workspaceObject.getDouble(SwimmingPool_IndoorFields::PoolHeatingSystemMaximumWaterFlowRate)) {
      modelObject.setPoolHeatingSystemMaximumWaterFlowRate(_poolHeatingSystemMaximumWaterFlowRate.get());
    }

    // Pool Miscellaneous Equipment Power: Optional Double
    if (boost::optional<double> _poolMiscellaneousEquipmentPower =
          workspaceObject.getDouble(SwimmingPool_IndoorFields::PoolMiscellaneousEquipmentPower)) {
      modelObject.setPoolMiscellaneousEquipmentPower(_poolMiscellaneousEquipmentPower.get());
    }

    // Setpoint Temperature Schedule: Required Object
    if ((_wo = workspaceObject.getTarget(SwimmingPool_IndoorFields::SetpointTemperatureSchedule))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _setpointTemperatureSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setSetpointTemperatureSchedule(_setpointTemperatureSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Setpoint Temperature Schedule'");
        }
      } else {
        LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Setpoint Temperature Schedule'");
        return boost::none;
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Setpoint Temperature Schedule'");
      return boost::none;
    }
    // Maximum Number of People: Required Double
    if (boost::optional<double> _maximumNumberofPeople = workspaceObject.getDouble(SwimmingPool_IndoorFields::MaximumNumberofPeople)) {
      modelObject.setMaximumNumberofPeople(_maximumNumberofPeople.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Maximum Number of People'");
      return boost::none;
    }

    // People Schedule: Optional Object
    if ((_wo = workspaceObject.getTarget(SwimmingPool_IndoorFields::PeopleSchedule))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _peopleSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setPeopleSchedule(_peopleSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'People Schedule'");
        }
      }
    }
    // People Heat Gain Schedule: Optional Object
    if ((_wo = workspaceObject.getTarget(SwimmingPool_IndoorFields::PeopleHeatGainSchedule))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        if (boost::optional<Schedule> _peopleHeatGainSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setPeopleHeatGainSchedule(_peopleHeatGainSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'People Heat Gain Schedule'");
        }
      }
    }

    return modelObject;

  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
