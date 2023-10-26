/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/GeneratorFuelCellPowerModule.hpp"
#include "../../model/GeneratorFuelCellPowerModule_Impl.hpp"
#include "../../model/GeneratorFuelCellAirSupply.hpp"
#include "../../model/GeneratorFuelCellAirSupply_Impl.hpp"
#include "../../model/GeneratorFuelCellWaterSupply.hpp"
#include "../../model/GeneratorFuelCellWaterSupply_Impl.hpp"
#include "../../model/GeneratorFuelCellAuxiliaryHeater.hpp"
#include "../../model/GeneratorFuelCellAuxiliaryHeater_Impl.hpp"
#include "../../model/GeneratorFuelCellExhaustGasToWaterHeatExchanger.hpp"
#include "../../model/GeneratorFuelCellExhaustGasToWaterHeatExchanger_Impl.hpp"
#include "../../model/GeneratorFuelCellElectricalStorage.hpp"
#include "../../model/GeneratorFuelCellElectricalStorage_Impl.hpp"
#include "../../model/GeneratorFuelCellInverter.hpp"
#include "../../model/GeneratorFuelCellInverter_Impl.hpp"
#include "../../model/GeneratorFuelCellStackCooler.hpp"
#include "../../model/GeneratorFuelCellStackCooler_Impl.hpp"
#include "../../model/GeneratorFuelSupply.hpp"
#include "../../model/GeneratorFuelSupply_Impl.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveQuadratic_Impl.hpp"
#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"
#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Generator_FuelCell_AirSupply_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateGeneratorFuelCellAirSupply(GeneratorFuelCellAirSupply& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> d;
    boost::optional<int> i;
    boost::optional<Node> node;
    boost::optional<CurveCubic> curve;
    boost::optional<CurveQuadratic> curvequad;

    IdfObject idf_aS = createAndRegisterIdfObject(openstudio::IddObjectType::Generator_FuelCell_AirSupply, modelObject);
    //Name
    s = modelObject.name();
    if (s) {
      idf_aS.setName(*s);
    }

    //AirInletNodeName
    node = modelObject.airInletNode();
    if (node) {
      idf_aS.setString(Generator_FuelCell_AirSupplyFields::AirInletNodeName, node.get().nameString());
    } else {
      auto name = modelObject.nameString() + " OA Node";
      IdfObject oaNodeListIdf(openstudio::IddObjectType::OutdoorAir_NodeList);
      oaNodeListIdf.setString(0, name);
      m_idfObjects.push_back(oaNodeListIdf);

      idf_aS.setString(openstudio::Generator_FuelCell_AirSupplyFields::AirInletNodeName, name);
    }

    //blowerPowerCurve
    curve = modelObject.blowerPowerCurve();
    if (curve) {
      idf_aS.setString(Generator_FuelCell_AirSupplyFields::BlowerPowerCurveName, curve.get().nameString());
    }

    //BlowerHeatLossFactor
    d = modelObject.blowerHeatLossFactor();
    if (d) {
      idf_aS.setDouble(Generator_FuelCell_AirSupplyFields::BlowerHeatLossFactor, d.get());
    }

    //AirSupplyRateCalculationMode
    s = modelObject.airSupplyRateCalculationMode();
    if (s) {
      idf_aS.setString(Generator_FuelCell_AirSupplyFields::AirSupplyRateCalculationMode, s.get());
    }

    //StoichiometricRatio
    d = modelObject.stoichiometricRatio();
    if (d) {
      idf_aS.setDouble(Generator_FuelCell_AirSupplyFields::StoichiometricRatio, d.get());
    }

    //AirRateFunctionofElectricPowerCurveName
    curvequad = modelObject.airRateFunctionofElectricPowerCurve();
    if (curvequad) {
      idf_aS.setString(Generator_FuelCell_AirSupplyFields::AirRateFunctionofElectricPowerCurveName, curvequad.get().nameString());
    }

    //AirRateAirTemperatureCoefficient
    d = modelObject.airRateAirTemperatureCoefficient();
    if (d) {
      idf_aS.setDouble(Generator_FuelCell_AirSupplyFields::AirRateAirTemperatureCoefficient, d.get());
    }

    //AirRateFunctionofElectricPowerCurveName
    curvequad = modelObject.airRateFunctionofElectricPowerCurve();
    if (curvequad) {
      idf_aS.setString(Generator_FuelCell_AirSupplyFields::AirRateFunctionofElectricPowerCurveName, curvequad.get().nameString());
    }

    //AirIntakeHeatRecoveryMode
    s = modelObject.airIntakeHeatRecoveryMode();
    if (s) {
      idf_aS.setString(Generator_FuelCell_AirSupplyFields::AirIntakeHeatRecoveryMode, s.get());
    }

    //AirSupplyConstituentMode
    s = modelObject.airSupplyConstituentMode();
    if (s) {
      idf_aS.setString(Generator_FuelCell_AirSupplyFields::AirSupplyConstituentMode, s.get());
    }

    //UserDefinedConstituents
    std::vector<AirSupplyConstituent> constituents = modelObject.constituents();
    if (!constituents.empty()) {
      // E+ expects the sum of the molar fractions to be 1
      double sumFractions = modelObject.sumofConstituentsMolarFractions();
      if ((sumFractions < 0.98) || (sumFractions > 1.02)) {
        LOG(Warn, "For " << modelObject.briefDescription() << ", the sum of molar fractions of the constituent isn't equal to 1, but " << sumFractions
                         << ".");
      }
      for (const AirSupplyConstituent& constituent : constituents) {
        auto eg = idf_aS.pushExtensibleGroup();
        eg.setString(Generator_FuelCell_AirSupplyExtensibleFields::ConstituentName, constituent.constituentName());
        eg.setDouble(Generator_FuelCell_AirSupplyExtensibleFields::MolarFraction, constituent.molarFraction());
      }
    }

    //NumberofUserDefinedConstituents
    idf_aS.setInt(Generator_FuelCell_AirSupplyFields::NumberofUserDefinedConstituents, constituents.size());

    return idf_aS;
  }

}  // namespace energyplus

}  // namespace openstudio
