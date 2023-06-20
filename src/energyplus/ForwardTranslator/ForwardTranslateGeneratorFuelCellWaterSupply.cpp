/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/GeneratorFuelCellWaterSupply.hpp"
#include "../../model/GeneratorFuelCellWaterSupply_Impl.hpp"

#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveQuadratic_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Generator_FuelCell_WaterSupply_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateGeneratorFuelCellWaterSupply(GeneratorFuelCellWaterSupply& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> d;
    boost::optional<Node> node;
    boost::optional<CurveQuadratic> curvequad;
    boost::optional<CurveCubic> curvecubic;
    boost::optional<Schedule> sch;

    IdfObject pcm = createAndRegisterIdfObject(openstudio::IddObjectType::Generator_FuelCell_WaterSupply, modelObject);
    //Name
    s = modelObject.name();
    if (s) {
      pcm.setName(*s);
    }

    //ReformerWaterFlowRateFunctionofFuelRateCurveName
    curvequad = modelObject.reformerWaterFlowRateFunctionofFuelRateCurve();
    if (curvequad) {
      pcm.setString(Generator_FuelCell_WaterSupplyFields::ReformerWaterFlowRateFunctionofFuelRateCurveName, curvequad.get().nameString());
    }

    //ReformerWaterPumpPowerFunctionofFuelRateCurveName
    curvecubic = modelObject.reformerWaterPumpPowerFunctionofFuelRateCurve();
    if (curvecubic) {
      pcm.setString(Generator_FuelCell_WaterSupplyFields::ReformerWaterPumpPowerFunctionofFuelRateCurveName, curvecubic.get().nameString());
    }

    //PumpHeatLossFactor
    d = modelObject.pumpHeatLossFactor();
    if (d) {
      pcm.setDouble(Generator_FuelCell_WaterSupplyFields::PumpHeatLossFactor, d.get());
    }

    //WaterTemperatureModelingMode
    s = modelObject.waterTemperatureModelingMode();
    if (s) {
      pcm.setString(Generator_FuelCell_WaterSupplyFields::WaterTemperatureModelingMode, s.get());
    }

    //WaterTemperatureReferenceNodeName
    node = modelObject.waterTemperatureReferenceNode();
    if (node) {
      pcm.setString(Generator_FuelCell_WaterSupplyFields::WaterTemperatureReferenceNodeName, node.get().nameString());
    }

    //WaterTemperatureScheduleName
    sch = modelObject.waterTemperatureSchedule();
    if (sch) {
      pcm.setString(Generator_FuelCell_WaterSupplyFields::WaterTemperatureScheduleName, sch.get().nameString());
    }

    return pcm;
  }

}  // namespace energyplus

}  // namespace openstudio
