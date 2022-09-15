/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "../../model/ChillerElectricASHRAE205.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../model/Zone.hpp"
#include "../../model/Zone_Impl.hpp"

#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include <utilities/idd/Chiller_Electric_ASHRAE205_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateChillerElectricASHRAE205(const WorkspaceObject& workspaceObject) {
    boost::optional<ModelObject> result;
    boost::optional<WorkspaceObject> _wo;
    boost::optional<ModelObject> _mo;

    // Instantiate an object of the class to store the values,
    // but we don't return it until we know it's ok
    // TODO: check constructor, it might need other objects
    openstudio::model::ChillerElectricASHRAE205 modelObject(m_model);

    // TODO: Note JM 2018-10-17
    // You are responsible for implementing any additional logic based on choice fields, etc.
    // The ReverseTranslator generator script is meant to facilitate your work, not get you 100% of the way

    // Name
    if (boost::optional<std::string> _name = workspaceObject.name()) {
      modelObject.setName(_name.get());
    }

    // Representation File Name: Required String
    if (boost::optional<std::string> _representationFileName = workspaceObject.getString(Chiller_Electric_ASHRAE205Fields::RepresentationFileName)) {
      modelObject.setRepresentationFileName(_representationFileName.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Representation File Name'");
      return result;
    }

    // Performance Interpolation Method: Optional String
    if (boost::optional<std::string> _performanceInterpolationMethod =
          workspaceObject.getString(Chiller_Electric_ASHRAE205Fields::PerformanceInterpolationMethod)) {
      modelObject.setPerformanceInterpolationMethod(_performanceInterpolationMethod.get());
    }

    // Rated Capacity: Optional Double
    if (boost::optional<double> _ratedCapacity = workspaceObject.getDouble(Chiller_Electric_ASHRAE205Fields::RatedCapacity)) {
      modelObject.setRatedCapacity(_ratedCapacity.get());
    }

    // Sizing Factor: Optional Double
    if (boost::optional<double> _sizingFactor = workspaceObject.getDouble(Chiller_Electric_ASHRAE205Fields::SizingFactor)) {
      modelObject.setSizingFactor(_sizingFactor.get());
    }

    // Ambient Temperature Indicator: Required String
    if (boost::optional<std::string> _ambientTemperatureIndicator =
          workspaceObject.getString(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureIndicator)) {
      modelObject.setAmbientTemperatureIndicator(_ambientTemperatureIndicator.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Ambient Temperature Indicator'");
      return result;
    }

    // Ambient Temperature Schedule Name: Optional Object
    if ((_wo = workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureScheduleName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Schedule> _ambientTemperatureSchedule = _mo->optionalCast<Schedule>()) {
          modelObject.setAmbientTemperatureSchedule(_ambientTemperatureSchedule.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Ambient Temperature Schedule Name'");
        }
      }
    }
    // Ambient Temperature Zone Name: Optional Object
    if ((_wo = workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureZoneName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Zone> _ambientTemperatureZone = _mo->optionalCast<Zone>()) {
          modelObject.setAmbientTemperatureZone(_ambientTemperatureZone.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Ambient Temperature Zone Name'");
        }
      }
    }
    // Ambient Temperature Outdoor Air Node Name: Optional Node
    if ((_wo = workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureOutdoorAirNodeName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Node> _ambientTemperatureOutdoorAirNodeName = _mo->optionalCast<Node>()) {
          modelObject.setAmbientTemperatureOutdoorAirNodeName(_ambientTemperatureOutdoorAirNodeName.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Ambient Temperature Outdoor Air Node Name'");
        }
      }
    }
    // Chilled Water Inlet Node Name: Required Node
    if ((_wo = workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::ChilledWaterInletNodeName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Node> _chilledWaterInletNodeName = _mo->optionalCast<Node>()) {
          modelObject.setChilledWaterInletNodeName(_chilledWaterInletNodeName.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Chilled Water Inlet Node Name'");
        }
      } else {
        LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Chilled Water Inlet Node Name'");
        return result;
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Chilled Water Inlet Node Name'");
      return result;
    }
    // Chilled Water Outlet Node Name: Required Node
    if ((_wo = workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::ChilledWaterOutletNodeName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Node> _chilledWaterOutletNodeName = _mo->optionalCast<Node>()) {
          modelObject.setChilledWaterOutletNodeName(_chilledWaterOutletNodeName.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Chilled Water Outlet Node Name'");
        }
      } else {
        LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot reverse translate required object 'Chilled Water Outlet Node Name'");
        return result;
      }
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required object 'Chilled Water Outlet Node Name'");
      return result;
    }
    // Chilled Water Maximum Requested Flow Rate: Optional Double
    if (boost::optional<double> _chilledWaterMaximumRequestedFlowRate =
          workspaceObject.getDouble(Chiller_Electric_ASHRAE205Fields::ChilledWaterMaximumRequestedFlowRate)) {
      modelObject.setChilledWaterMaximumRequestedFlowRate(_chilledWaterMaximumRequestedFlowRate.get());
    }

    // Condenser Inlet Node Name: Optional Node
    if ((_wo = workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::CondenserInletNodeName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Node> _condenserInletNodeName = _mo->optionalCast<Node>()) {
          modelObject.setCondenserInletNodeName(_condenserInletNodeName.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Condenser Inlet Node Name'");
        }
      }
    }
    // Condenser Outlet Node Name: Optional Node
    if ((_wo = workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::CondenserOutletNodeName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Node> _condenserOutletNodeName = _mo->optionalCast<Node>()) {
          modelObject.setCondenserOutletNodeName(_condenserOutletNodeName.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Condenser Outlet Node Name'");
        }
      }
    }
    // Condenser Maximum Requested Flow Rate: Optional Double
    if (boost::optional<double> _condenserMaximumRequestedFlowRate =
          workspaceObject.getDouble(Chiller_Electric_ASHRAE205Fields::CondenserMaximumRequestedFlowRate)) {
      modelObject.setCondenserMaximumRequestedFlowRate(_condenserMaximumRequestedFlowRate.get());
    }

    // Chiller Flow Mode: Optional String
    if (boost::optional<std::string> _chillerFlowMode = workspaceObject.getString(Chiller_Electric_ASHRAE205Fields::ChillerFlowMode)) {
      modelObject.setChillerFlowMode(_chillerFlowMode.get());
    }

    // Oil Cooler Inlet Node Name: Optional Node
    if ((_wo = workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::OilCoolerInletNodeName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Node> _oilCoolerInletNodeName = _mo->optionalCast<Node>()) {
          modelObject.setOilCoolerInletNodeName(_oilCoolerInletNodeName.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Oil Cooler Inlet Node Name'");
        }
      }
    }
    // Oil Cooler Outlet Node Name: Optional Node
    if ((_wo = workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::OilCoolerOutletNodeName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Node> _oilCoolerOutletNodeName = _mo->optionalCast<Node>()) {
          modelObject.setOilCoolerOutletNodeName(_oilCoolerOutletNodeName.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Oil Cooler Outlet Node Name'");
        }
      }
    }
    // Oil Cooler Design Flow Rate: Optional Double
    if (boost::optional<double> _oilCoolerDesignFlowRate = workspaceObject.getDouble(Chiller_Electric_ASHRAE205Fields::OilCoolerDesignFlowRate)) {
      modelObject.setOilCoolerDesignFlowRate(_oilCoolerDesignFlowRate.get());
    }

    // Auxiliary Inlet Node Name: Optional Node
    if ((_wo = workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::AuxiliaryInletNodeName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Node> _auxiliaryInletNodeName = _mo->optionalCast<Node>()) {
          modelObject.setAuxiliaryInletNodeName(_auxiliaryInletNodeName.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Auxiliary Inlet Node Name'");
        }
      }
    }
    // Auxiliary Outlet Node Name: Optional Node
    if ((_wo = workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::AuxiliaryOutletNodeName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Node> _auxiliaryOutletNodeName = _mo->optionalCast<Node>()) {
          modelObject.setAuxiliaryOutletNodeName(_auxiliaryOutletNodeName.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Auxiliary Outlet Node Name'");
        }
      }
    }
    // Auxiliary Cooling Design Flow Rate: Optional Double
    if (boost::optional<double> _auxiliaryCoolingDesignFlowRate =
          workspaceObject.getDouble(Chiller_Electric_ASHRAE205Fields::AuxiliaryCoolingDesignFlowRate)) {
      modelObject.setAuxiliaryCoolingDesignFlowRate(_auxiliaryCoolingDesignFlowRate.get());
    }

    // Heat Recovery Inlet Node Name: Optional Node
    if ((_wo = workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::HeatRecoveryInletNodeName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Node> _heatRecoveryInletNodeName = _mo->optionalCast<Node>()) {
          modelObject.setHeatRecoveryInletNodeName(_heatRecoveryInletNodeName.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Heat Recovery Inlet Node Name'");
        }
      }
    }
    // Heat Recovery Outlet Node Name: Optional Node
    if ((_wo = workspaceObject.getTarget(Chiller_Electric_ASHRAE205Fields::HeatRecoveryOutletNodeName))) {
      if ((_mo = translateAndMapWorkspaceObject(_wo.get()))) {
        // TODO: check return types
        if (boost::optional<Node> _heatRecoveryOutletNodeName = _mo->optionalCast<Node>()) {
          modelObject.setHeatRecoveryOutletNodeName(_heatRecoveryOutletNodeName.get());
        } else {
          LOG(Warn, workspaceObject.briefDescription() << " has a wrong type for 'Heat Recovery Outlet Node Name'");
        }
      }
    }
    // End-Use Subcategory: Optional String
    if (boost::optional<std::string> _endUseSubcategory = workspaceObject.getString(Chiller_Electric_ASHRAE205Fields::EndUseSubcategory)) {
      modelObject.setEndUseSubcategory(_endUseSubcategory.get());
    }

    result = modelObject;
    return result;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio