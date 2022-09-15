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

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/ChillerElectricASHRAE205.hpp"

// TODO: Check the following class names against object getters and setters.
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"

#include "../../model/Zone.hpp"
#include "../../model/Zone_Impl.hpp"

#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include <utilities/idd/Chiller_Electric_ASHRAE205_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateChillerElectricASHRAE205(model::ChillerElectricASHRAE205& modelObject) {
    boost::optional<IdfObject> result;
    boost::optional<WorkspaceObject> _wo;
    boost::optional<ModelObject> _mo;

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Chiller_Electric_ASHRAE205, modelObject);
    // If it doesn't have a name, or if you aren't sure you are going to want to return it
    // IdfObject idfObject( openstudio::IddObjectType::Chiller_Electric_ASHRAE205 );
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

    // Representation File Name: Required String
    std::string representationFileName = modelObject.representationFileName();
    idfObject.setString(Chiller_Electric_ASHRAE205Fields::RepresentationFileName, representationFileName);

    // Performance Interpolation Method: Optional String
    std::string performanceInterpolationMethod = modelObject.performanceInterpolationMethod();
    idfObject.setString(Chiller_Electric_ASHRAE205Fields::PerformanceInterpolationMethod, performanceInterpolationMethod);

    if (modelObject.isRatedCapacityAutosized()) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::RatedCapacity, "Autosize");
    } else {
      // Rated Capacity: boost::optional<double>
      if (boost::optional<double> _ratedCapacity = modelObject.ratedCapacity()) {
        idfObject.setDouble(Chiller_Electric_ASHRAE205Fields::RatedCapacity, _ratedCapacity.get());
      }
    }

    // Sizing Factor: Optional Double
    double sizingFactor = modelObject.sizingFactor();
    idfObject.setDouble(Chiller_Electric_ASHRAE205Fields::SizingFactor, sizingFactor);

    // Ambient Temperature Indicator: Required String
    std::string ambientTemperatureIndicator = modelObject.ambientTemperatureIndicator();
    idfObject.setString(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureIndicator, ambientTemperatureIndicator);

    // Ambient Temperature Schedule Name: Optional Object
    if (boost::optional<Schedule> _ambientTemperatureSchedule = modelObject.ambientTemperatureSchedule()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_ambientTemperatureSchedule.get())) {
        idfObject.setString(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureScheduleName, _owo->nameString());
      }
    }

    // Ambient Temperature Zone Name: Optional Object
    if (boost::optional<Zone> _ambientTemperatureZone = modelObject.ambientTemperatureZone()) {
      if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_ambientTemperatureZone.get())) {
        idfObject.setString(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureZoneName, _owo->nameString());
      }
    }

    // Ambient Temperature Outdoor Air Node Name: Optional Node
    Node ambientTemperatureOutdoorAirNodeName = modelObject.ambientTemperatureOutdoorAirNodeName();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(ambientTemperatureOutdoorAirNodeName)) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureOutdoorAirNodeName, _owo->nameString());
    }

    // Chilled Water Inlet Node Name: Required Node
    Node chilledWaterInletNodeName = modelObject.chilledWaterInletNodeName();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(chilledWaterInletNodeName)) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::ChilledWaterInletNodeName, _owo->nameString());
    }

    // Chilled Water Outlet Node Name: Required Node
    Node chilledWaterOutletNodeName = modelObject.chilledWaterOutletNodeName();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(chilledWaterOutletNodeName)) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::ChilledWaterOutletNodeName, _owo->nameString());
    }

    if (modelObject.isChilledWaterMaximumRequestedFlowRateAutosized()) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::ChilledWaterMaximumRequestedFlowRate, "Autosize");
    } else {
      // Chilled Water Maximum Requested Flow Rate: boost::optional<double>
      if (boost::optional<double> _chilledWaterMaximumRequestedFlowRate = modelObject.chilledWaterMaximumRequestedFlowRate()) {
        idfObject.setDouble(Chiller_Electric_ASHRAE205Fields::ChilledWaterMaximumRequestedFlowRate, _chilledWaterMaximumRequestedFlowRate.get());
      }
    }

    // Condenser Inlet Node Name: Optional Node
    Node condenserInletNodeName = modelObject.condenserInletNodeName();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(condenserInletNodeName)) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::CondenserInletNodeName, _owo->nameString());
    }

    // Condenser Outlet Node Name: Optional Node
    Node condenserOutletNodeName = modelObject.condenserOutletNodeName();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(condenserOutletNodeName)) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::CondenserOutletNodeName, _owo->nameString());
    }

    if (modelObject.isCondenserMaximumRequestedFlowRateAutosized()) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::CondenserMaximumRequestedFlowRate, "Autosize");
    } else {
      // Condenser Maximum Requested Flow Rate: boost::optional<double>
      if (boost::optional<double> _condenserMaximumRequestedFlowRate = modelObject.condenserMaximumRequestedFlowRate()) {
        idfObject.setDouble(Chiller_Electric_ASHRAE205Fields::CondenserMaximumRequestedFlowRate, _condenserMaximumRequestedFlowRate.get());
      }
    }

    // Chiller Flow Mode: Optional String
    std::string chillerFlowMode = modelObject.chillerFlowMode();
    idfObject.setString(Chiller_Electric_ASHRAE205Fields::ChillerFlowMode, chillerFlowMode);

    // Oil Cooler Inlet Node Name: Optional Node
    Node oilCoolerInletNodeName = modelObject.oilCoolerInletNodeName();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(oilCoolerInletNodeName)) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::OilCoolerInletNodeName, _owo->nameString());
    }

    // Oil Cooler Outlet Node Name: Optional Node
    Node oilCoolerOutletNodeName = modelObject.oilCoolerOutletNodeName();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(oilCoolerOutletNodeName)) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::OilCoolerOutletNodeName, _owo->nameString());
    }

    // Oil Cooler Design Flow Rate: boost::optional<double>
    if (boost::optional<double> _oilCoolerDesignFlowRate = modelObject.oilCoolerDesignFlowRate()) {
      idfObject.setDouble(Chiller_Electric_ASHRAE205Fields::OilCoolerDesignFlowRate, _oilCoolerDesignFlowRate.get());
    }

    // Auxiliary Inlet Node Name: Optional Node
    Node auxiliaryInletNodeName = modelObject.auxiliaryInletNodeName();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(auxiliaryInletNodeName)) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::AuxiliaryInletNodeName, _owo->nameString());
    }

    // Auxiliary Outlet Node Name: Optional Node
    Node auxiliaryOutletNodeName = modelObject.auxiliaryOutletNodeName();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(auxiliaryOutletNodeName)) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::AuxiliaryOutletNodeName, _owo->nameString());
    }

    // Auxiliary Cooling Design Flow Rate: boost::optional<double>
    if (boost::optional<double> _auxiliaryCoolingDesignFlowRate = modelObject.auxiliaryCoolingDesignFlowRate()) {
      idfObject.setDouble(Chiller_Electric_ASHRAE205Fields::AuxiliaryCoolingDesignFlowRate, _auxiliaryCoolingDesignFlowRate.get());
    }

    // Heat Recovery Inlet Node Name: Optional Node
    Node heatRecoveryInletNodeName = modelObject.heatRecoveryInletNodeName();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(heatRecoveryInletNodeName)) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::HeatRecoveryInletNodeName, _owo->nameString());
    }

    // Heat Recovery Outlet Node Name: Optional Node
    Node heatRecoveryOutletNodeName = modelObject.heatRecoveryOutletNodeName();
    if (boost::optional<IdfObject> _owo = translateAndMapModelObject(heatRecoveryOutletNodeName)) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::HeatRecoveryOutletNodeName, _owo->nameString());
    }

    // End-Use Subcategory: Optional String
    std::string endUseSubcategory = modelObject.endUseSubcategory();
    idfObject.setString(Chiller_Electric_ASHRAE205Fields::EndUseSubcategory, endUseSubcategory);

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio