/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/GeneratorFuelSupply.hpp"
#include "../../model/GeneratorFuelSupply_Impl.hpp"

#include "../../model/Node.hpp"
#include "../../model/Node_Impl.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Schedule_Impl.hpp"
#include "../../model/ScheduleConstant.hpp"
#include "../../model/ScheduleConstant_Impl.hpp"
#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveQuadratic_Impl.hpp"
#include "../../model/ScheduleTypeLimits.hpp"
#include "../../model/ScheduleTypeRegistry.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Generator_FuelSupply_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateGeneratorFuelSupply(GeneratorFuelSupply& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> d;
    boost::optional<int> i;
    boost::optional<Node> node;
    boost::optional<Schedule> schedule;
    boost::optional<CurveCubic> curve;

    IdfObject idf_fS = createAndRegisterIdfObject(openstudio::IddObjectType::Generator_FuelSupply, modelObject);
    //Name
    s = modelObject.name();
    if (s) {
      idf_fS.setName(*s);
    }

    //fuelTemperatureModelingMode
    s = modelObject.fuelTemperatureModelingMode();
    if (s) {
      idf_fS.setString(Generator_FuelSupplyFields::FuelTemperatureModelingMode, s.get());
    }

    //FuelTemperatureReferenceNodeName
    node = modelObject.fuelTemperatureReferenceNode();
    if (node) {
      idf_fS.setString(Generator_FuelSupplyFields::FuelTemperatureReferenceNodeName, node.get().nameString());
    }

    //FuelTemperatureScheduleName
    schedule = modelObject.fuelTemperatureSchedule();
    if (schedule) {
      idf_fS.setString(Generator_FuelSupplyFields::FuelTemperatureScheduleName, schedule.get().nameString());
    }

    //CompressorPowerMultiplierFunctionofFuelRateCurveName
    curve = modelObject.compressorPowerMultiplierFunctionofFuelRateCurve();
    if (curve) {
      idf_fS.setString(Generator_FuelSupplyFields::CompressorPowerMultiplierFunctionofFuelRateCurveName, curve.get().nameString());
    }

    //CompressorHeatLossFactor
    d = modelObject.compressorHeatLossFactor();
    if (d) {
      idf_fS.setDouble(Generator_FuelSupplyFields::CompressorHeatLossFactor, d.get());
    }

    //FuelType
    s = modelObject.fuelType();
    if (s) {
      idf_fS.setString(Generator_FuelSupplyFields::FuelType, s.get());
    }

    //LiquidGenericFuelLowerHeatingValue
    d = modelObject.liquidGenericFuelLowerHeatingValue();
    if (d) {
      idf_fS.setDouble(Generator_FuelSupplyFields::LiquidGenericFuelLowerHeatingValue, d.get());
    }

    //LiquidGenericFuelHigherHeatingValue
    d = modelObject.liquidGenericFuelHigherHeatingValue();
    if (d) {
      idf_fS.setDouble(Generator_FuelSupplyFields::LiquidGenericFuelHigherHeatingValue, d.get());
    }

    //LiquidGenericFuelMolecularWeight
    d = modelObject.liquidGenericFuelMolecularWeight();
    if (d) {
      idf_fS.setDouble(Generator_FuelSupplyFields::LiquidGenericFuelMolecularWeight, d.get());
    }

    //LiquidGenericFuelCO2EmissionFactor
    d = modelObject.liquidGenericFuelCO2EmissionFactor();
    if (d) {
      idf_fS.setDouble(Generator_FuelSupplyFields::LiquidGenericFuelCO2EmissionFactor, d.get());
    }

    //UserDefinedConstituents
    std::vector<FuelSupplyConstituent> constituents = modelObject.constituents();
    if (!constituents.empty()) {
      // E+ expects the sum of the molar fractions to be 1
      double sumFractions = modelObject.sumofConstituentsMolarFractions();
      if ((sumFractions < 0.98) || (sumFractions > 1.02)) {
        LOG(Warn, "For " << modelObject.briefDescription() << ", the sum of molar fractions of the constituent isn't equal to 1, but " << sumFractions
                         << ".");
      }
      for (const FuelSupplyConstituent& constituent : constituents) {
        auto eg = idf_fS.pushExtensibleGroup();
        eg.setString(Generator_FuelSupplyExtensibleFields::ConstituentName, constituent.constituentName());
        eg.setDouble(Generator_FuelSupplyExtensibleFields::ConstituentMolarFraction, constituent.molarFraction());
      }
    }

    //NumberofUserDefinedConstituents
    idf_fS.setInt(Generator_FuelSupplyFields::NumberofConstituentsinGaseousConstituentFuelSupply, constituents.size());

    return idf_fS;
  }

}  // namespace energyplus

}  // namespace openstudio
