/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/GeneratorFuelCellInverter.hpp"
#include "../../model/GeneratorFuelCellInverter_Impl.hpp"

#include "../../model/CurveCubic.hpp"
#include "../../model/CurveCubic_Impl.hpp"
#include "../../model/CurveQuadratic.hpp"
#include "../../model/CurveQuadratic_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Generator_FuelCell_Inverter_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateGeneratorFuelCellInverter(GeneratorFuelCellInverter& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> d;
    boost::optional<CurveQuadratic> curvequad;

    IdfObject pcm = createAndRegisterIdfObject(openstudio::IddObjectType::Generator_FuelCell_Inverter, modelObject);
    //Name
    s = modelObject.name();
    if (s) {
      pcm.setName(*s);
    }

    //InverterEfficiencyCalculationMode
    s = modelObject.inverterEfficiencyCalculationMode();
    if (s) {
      pcm.setString(Generator_FuelCell_InverterFields::InverterEfficiencyCalculationMode, s.get());
    }

    //InverterEfficiency
    d = modelObject.inverterEfficiency();
    if (d) {
      pcm.setDouble(Generator_FuelCell_InverterFields::InverterEfficiency, d.get());
    }

    //EfficiencyFunctionofDCPowerCurveName
    curvequad = modelObject.efficiencyFunctionofDCPowerCurve();
    if (curvequad) {
      pcm.setString(Generator_FuelCell_InverterFields::EfficiencyFunctionofDCPowerCurveName, curvequad.get().nameString());
    }

    return pcm;
  }

}  // namespace energyplus

}  // namespace openstudio
