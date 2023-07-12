/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/ChillerElectricASHRAE205.hpp"

#include "../../model/Schedule.hpp"
#include "../../model/ExternalFile.hpp"
#include "../../model/ThermalZone.hpp"

#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include <utilities/idd/Chiller_Electric_ASHRAE205_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateChillerElectricASHRAE205(model::ChillerElectricASHRAE205& modelObject) {

    path filePath = modelObject.representationFile().filePath();
    if (!openstudio::filesystem::exists(filePath)) {
      LOG(Warn, modelObject.briefDescription() << "will not be translated, cannot find the representation file '" << filePath << "'");
      return boost::none;
    }

    // make the path correct for this system
    filePath = openstudio::filesystem::system_complete(filePath);

    if (!modelObject.chilledWaterLoop()) {
      LOG(Warn, modelObject.briefDescription() << "will not be translated, it not on a Chilled Water Loop.");
      return boost::none;
    }

    // Instantiate an IdfObject of the class to store the values
    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::Chiller_Electric_ASHRAE205, modelObject);

    // Representation File Name: Required String
    idfObject.setString(Chiller_Electric_ASHRAE205Fields::RepresentationFileName, openstudio::toString(filePath));

    // Performance Interpolation Method
    std::string performanceInterpolationMethod = modelObject.performanceInterpolationMethod();
    idfObject.setString(Chiller_Electric_ASHRAE205Fields::PerformanceInterpolationMethod, performanceInterpolationMethod);

    if (modelObject.isRatedCapacityAutosized()) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::RatedCapacity, "Autosize");
    } else {
      idfObject.setDouble(Chiller_Electric_ASHRAE205Fields::RatedCapacity, modelObject.ratedCapacity().get());
    }

    // Sizing Factor: Optional Double
    idfObject.setDouble(Chiller_Electric_ASHRAE205Fields::SizingFactor, modelObject.sizingFactor());

    // Ambient Temperature Indicator: Required String
    std::string ambientTemperatureIndicator = modelObject.ambientTemperatureIndicator();
    bool ambientTempOk = false;

    if (openstudio::istringEqual("Schedule", ambientTemperatureIndicator)) {
      // Ambient Temperature Schedule Name: Optional Object
      if (boost::optional<Schedule> _ambientTemperatureSchedule = modelObject.ambientTemperatureSchedule()) {
        if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_ambientTemperatureSchedule.get())) {
          idfObject.setString(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureScheduleName, _owo->nameString());
          ambientTempOk = true;
        }
      }
    } else if (openstudio::istringEqual("Zone", ambientTemperatureIndicator)) {
      // Ambient Temperature Zone Name: Optional Object
      if (boost::optional<ThermalZone> _ambientTemperatureZone = modelObject.ambientTemperatureZone()) {
        if (boost::optional<IdfObject> _owo = translateAndMapModelObject(_ambientTemperatureZone.get())) {
          idfObject.setString(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureZoneName, _owo->nameString());
          ambientTempOk = true;
        }
      }
    } else if (openstudio::istringEqual("Outdoors", ambientTemperatureIndicator)) {
      ambientTempOk = true;
    }
    if (!ambientTempOk) {
      LOG(Warn, "For " << modelObject.briefDescription() << " the Ambient Temperature Indicator is " << ambientTemperatureIndicator
                       << " but the required objects don't match. Falling back to 'Outdoors'");
      ambientTemperatureIndicator = "Outdoors";
    }

    idfObject.setString(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureIndicator, ambientTemperatureIndicator);

    if (openstudio::istringEqual("Outdoors", ambientTemperatureIndicator)) {
      std::string oaNodeName = modelObject.nameString() + " OA Node";
      if (boost::optional<std::string> s_ = modelObject.ambientTemperatureOutdoorAirNodeName()) {
        oaNodeName = s_.get();
      }

      idfObject.setString(Chiller_Electric_ASHRAE205Fields::AmbientTemperatureOutdoorAirNodeName, oaNodeName);

      IdfObject _oaNodeList(openstudio::IddObjectType::OutdoorAir_NodeList);
      _oaNodeList.setString(0, oaNodeName);
      m_idfObjects.push_back(_oaNodeList);
    }

    // Chilled Water Node Names: Required Nodes
    if (auto node_ = modelObject.chilledWaterInletNode()) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::ChilledWaterInletNodeName, node_->nameString());
    }

    if (auto node_ = modelObject.chilledWaterOutletNode()) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::ChilledWaterOutletNodeName, node_->nameString());
    }

    // Optional Nodes

    // Condenser
    if (auto node_ = modelObject.condenserInletNode()) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::CondenserInletNodeName, node_->nameString());
    }

    if (auto node_ = modelObject.condenserOutletNode()) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::CondenserOutletNodeName, node_->nameString());
    }

    // Heat Recovery
    if (auto node_ = modelObject.heatRecoveryInletNode()) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::HeatRecoveryInletNodeName, node_->nameString());
    }

    if (auto node_ = modelObject.heatRecoveryOutletNode()) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::HeatRecoveryOutletNodeName, node_->nameString());
    }

    // Oil Cooler
    if (auto node_ = modelObject.oilCoolerInletNode()) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::OilCoolerInletNodeName, node_->nameString());
    }

    if (auto node_ = modelObject.oilCoolerOutletNode()) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::OilCoolerOutletNodeName, node_->nameString());
    }

    // Auxiliary
    if (auto node_ = modelObject.auxiliaryInletNode()) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::AuxiliaryInletNodeName, node_->nameString());
    }

    if (auto node_ = modelObject.auxiliaryOutletNode()) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::AuxiliaryOutletNodeName, node_->nameString());
    }

    if (modelObject.isChilledWaterMaximumRequestedFlowRateAutosized()) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::ChilledWaterMaximumRequestedFlowRate, "Autosize");
    } else {
      // ChilledWater Maximum Requested Flow Rate: boost::optional<double>
      if (boost::optional<double> _chilledWaterMaximumRequestedFlowRate = modelObject.chilledWaterMaximumRequestedFlowRate()) {
        idfObject.setDouble(Chiller_Electric_ASHRAE205Fields::ChilledWaterMaximumRequestedFlowRate, _chilledWaterMaximumRequestedFlowRate.get());
      }
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
    idfObject.setString(Chiller_Electric_ASHRAE205Fields::ChillerFlowMode, modelObject.chillerFlowMode());

    // Oil Cooler Design Flow Rate: boost::optional<double>
    if (boost::optional<double> _oilCoolerDesignFlowRate = modelObject.oilCoolerDesignFlowRate()) {
      idfObject.setDouble(Chiller_Electric_ASHRAE205Fields::OilCoolerDesignFlowRate, _oilCoolerDesignFlowRate.get());
    }

    // Auxiliary Cooling Design Flow Rate: boost::optional<double>
    if (boost::optional<double> _auxiliaryCoolingDesignFlowRate = modelObject.auxiliaryCoolingDesignFlowRate()) {
      idfObject.setDouble(Chiller_Electric_ASHRAE205Fields::AuxiliaryCoolingDesignFlowRate, _auxiliaryCoolingDesignFlowRate.get());
    }

    // End-Use Subcategory: Optional String
    if (!modelObject.isEndUseSubcategoryDefaulted()) {
      idfObject.setString(Chiller_Electric_ASHRAE205Fields::EndUseSubcategory, modelObject.endUseSubcategory());
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
