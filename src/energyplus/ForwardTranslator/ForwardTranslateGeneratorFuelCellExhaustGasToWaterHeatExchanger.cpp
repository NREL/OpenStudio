/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/GeneratorFuelCellExhaustGasToWaterHeatExchanger.hpp"
#include "../../model/GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl.hpp"

#include "../../model/Connection.hpp"
#include "../../model/Connection_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Generator_FuelCell_ExhaustGasToWaterHeatExchanger_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateGeneratorFuelCellExhaustGasToWaterHeatExchanger(GeneratorFuelCellExhaustGasToWaterHeatExchanger& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> d;
    boost::optional<Connection> connection;
    boost::optional<Node> node;

    IdfObject pcm = createAndRegisterIdfObject(openstudio::IddObjectType::Generator_FuelCell_ExhaustGasToWaterHeatExchanger, modelObject);
    //Name
    s = modelObject.name();
    if (s) {
      pcm.setName(*s);
    }

    // HeatRecoveryWaterInletNodeName

    if (boost::optional<ModelObject> mo = modelObject.inletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        pcm.setString(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::HeatRecoveryWaterInletNodeName, node->name().get());
      }
    }

    // HeatRecoveryWaterOutletNodeName

    if (boost::optional<ModelObject> mo = modelObject.outletModelObject()) {
      if (boost::optional<Node> node = mo->optionalCast<Node>()) {
        pcm.setString(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::HeatRecoveryWaterOutletNodeName, node->name().get());
      }
    }

    //HeatRecoveryWaterMaximumFlowRate
    d = modelObject.heatRecoveryWaterMaximumFlowRate();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::HeatRecoveryWaterMaximumFlowRate, d.get());
    }

    //ExhaustOutletAirNodeName
    node = modelObject.exhaustOutletAirNode();
    if (node) {
      pcm.setString(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::ExhaustOutletAirNodeName, node.get().nameString());
    } else {
      pcm.setString(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::ExhaustOutletAirNodeName,
                    modelObject.nameString() + " Exhaust Outlet Air Node");
    }

    //HeatExchangerCalculationMethod
    s = modelObject.heatExchangerCalculationMethod();
    if (s) {
      pcm.setString(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::HeatExchangerCalculationMethod, s.get());
    }

    //Method1HeatExchangerEffectiveness
    d = modelObject.method1HeatExchangerEffectiveness();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method1HeatExchangerEffectiveness, d.get());
    }

    //Method2Parameterhxs0
    d = modelObject.method2Parameterhxs0();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method2Parameterhxs0, d.get());
    }

    //Method2Parameterhxs1
    d = modelObject.method2Parameterhxs1();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method2Parameterhxs1, d.get());
    }

    //Method2Parameterhxs2
    d = modelObject.method2Parameterhxs2();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method2Parameterhxs2, d.get());
    }

    //Method2Parameterhxs3
    d = modelObject.method2Parameterhxs3();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method2Parameterhxs3, d.get());
    }

    //Method2Parameterhxs4
    d = modelObject.method2Parameterhxs4();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method2Parameterhxs4, d.get());
    }

    //Method3h0GasCoefficient
    d = modelObject.method3h0GasCoefficient();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3h0GasCoefficient, d.get());
    }

    //Method3NdotGasRefCoefficient
    d = modelObject.method3NdotGasRefCoefficient();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3NdotGasRefCoefficient, d.get());
    }

    //Method3nCoefficient
    d = modelObject.method3nCoefficient();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3nCoefficient, d.get());
    }

    //Method3GasArea
    d = modelObject.method3GasArea();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3GasArea, d.get());
    }

    //Method3h0WaterCoefficient
    d = modelObject.method3h0WaterCoefficient();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3h0WaterCoefficient, d.get());
    }

    //Method3NdotWaterrefCoefficient
    d = modelObject.method3NdotWaterrefCoefficient();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3NdotWaterrefCoefficient, d.get());
    }

    //Method3mCoefficient
    d = modelObject.method3mCoefficient();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3mCoefficient, d.get());
    }

    //Method3WaterArea
    d = modelObject.method3WaterArea();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3WaterArea, d.get());
    }

    //Method3FAdjustmentFactor
    d = modelObject.method3FAdjustmentFactor();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method3FAdjustmentFactor, d.get());
    }

    //Method4hxl1Coefficient
    d = modelObject.method4hxl1Coefficient();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method4hxl1Coefficient, d.get());
    }

    //Method4hxl2Coefficient
    d = modelObject.method4hxl2Coefficient();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method4hxl2Coefficient, d.get());
    }

    //Method4CondensationThreshold
    d = modelObject.method4CondensationThreshold();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ExhaustGasToWaterHeatExchangerFields::Method4CondensationThreshold, d.get());
    }

    return pcm;
  }

}  // namespace energyplus

}  // namespace openstudio
