/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/GroundHeatExchangerHorizontalTrench.hpp"
#include "../../model/GroundHeatExchangerHorizontalTrench_Impl.hpp"
#include "../../model/Model.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../utilities/core/Assert.hpp"
#include <utilities/idd/GroundHeatExchanger_HorizontalTrench_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;
using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateGroundHeatExchangerHorizontalTrench(GroundHeatExchangerHorizontalTrench& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> value;

    // UndisturbedGroundTemperatureModelName, is required, so start by that
    ModelObject undisturbedGroundTemperatureModel = modelObject.undisturbedGroundTemperatureModel();
    boost::optional<IdfObject> _undisturbedGroundTemperatureModel = translateAndMapModelObject(undisturbedGroundTemperatureModel);
    if (_undisturbedGroundTemperatureModel) {
      s = _undisturbedGroundTemperatureModel->name().get();
    } else {
      LOG(Warn, modelObject.briefDescription() << " cannot be translated as its undisturbed ground temperature model object cannot be translated: "
                                               << undisturbedGroundTemperatureModel.briefDescription() << ".");
      return boost::none;
    }

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::GroundHeatExchanger_HorizontalTrench, modelObject);

    // UndisturbedGroundTemperatureModelName
    idfObject.setString(GroundHeatExchanger_HorizontalTrenchFields::UndisturbedGroundTemperatureModelName, s.get());

    // UndisturbedGroundTemperatureModelType
    idfObject.setString(GroundHeatExchanger_HorizontalTrenchFields::UndisturbedGroundTemperatureModelType,
                        _undisturbedGroundTemperatureModel->iddObject().name());

    // InletNodeName
    if (auto mo = modelObject.inletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(GroundHeatExchanger_HorizontalTrenchFields::InletNodeName, node->name().get());
      }
    }

    // OutletNodeName
    if (auto mo = modelObject.outletModelObject()) {
      if (auto node = mo->optionalCast<Node>()) {
        idfObject.setString(GroundHeatExchanger_HorizontalTrenchFields::OutletNodeName, node->name().get());
      }
    }

    // DesignFlowRate
    {
      auto value = modelObject.designFlowRate();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::DesignFlowRate, value);
    }

    // TrenchLengthinPipeAxialDirection
    {
      auto value = modelObject.trenchLengthinPipeAxialDirection();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::TrenchLengthinPipeAxialDirection, value);
    }

    // NumberofTrenches
    {
      auto value = modelObject.numberofTrenches();
      idfObject.setInt(GroundHeatExchanger_HorizontalTrenchFields::NumberofTrenches, value);
    }

    // HorizontalSpacingBetweenPipes
    {
      auto value = modelObject.horizontalSpacingBetweenPipes();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::HorizontalSpacingBetweenPipes, value);
    }

    // PipeInnerDiameter
    {
      auto value = modelObject.pipeInnerDiameter();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::PipeInnerDiameter, value);
    }

    // PipeOuterDiameter
    {
      auto value = modelObject.pipeOuterDiameter();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::PipeOuterDiameter, value);
    }

    // BurialDepth
    {
      auto value = modelObject.burialDepth();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::BurialDepth, value);
    }

    // SoilThermalConductivity
    {
      auto value = modelObject.soilThermalConductivity();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::SoilThermalConductivity, value);
    }

    // SoilDensity
    {
      auto value = modelObject.soilDensity();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::SoilDensity, value);
    }

    // SoilSpecificHeat
    {
      auto value = modelObject.soilSpecificHeat();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::SoilSpecificHeat, value);
    }

    // PipeThermalConductivity
    {
      auto value = modelObject.pipeThermalConductivity();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::PipeThermalConductivity, value);
    }

    // PipeDensity
    {
      auto value = modelObject.pipeDensity();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::PipeDensity, value);
    }

    // PipeSpecificHeat
    {
      auto value = modelObject.pipeSpecificHeat();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::PipeSpecificHeat, value);
    }

    // SoilMoistureContentPercent
    {
      auto value = modelObject.soilMoistureContentPercent();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::SoilMoistureContentPercent, value);
    }

    // SoilMoistureContentPercentatSaturation
    {
      auto value = modelObject.soilMoistureContentPercentatSaturation();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::SoilMoistureContentPercentatSaturation, value);
    }

    // EvapotranspirationGroundCoverParameter
    {
      auto value = modelObject.evapotranspirationGroundCoverParameter();
      idfObject.setDouble(GroundHeatExchanger_HorizontalTrenchFields::EvapotranspirationGroundCoverParameter, value);
    }

    return idfObject;
  }

}  // namespace energyplus
}  // namespace openstudio
