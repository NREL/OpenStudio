/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/MaterialPropertyPhaseChange.hpp"
#include "../../model/MaterialPropertyPhaseChange_Impl.hpp"
#include "../../model/Material.hpp"
#include "../../model/Material_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/MaterialProperty_PhaseChange_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateMaterialPropertyPhaseChange(MaterialPropertyPhaseChange& modelObject) {
    if (modelObject.temperatureEnthalpys().size() < 4) {
      LOG(Warn, modelObject.briefDescription() << " cannot be translated as it has less than 4 temperature-enthalpy pairs.");
      return boost::none;
    }

    IdfObject idfObject = createAndRegisterIdfObject(openstudio::IddObjectType::MaterialProperty_PhaseChange, modelObject);

    idfObject.setString(MaterialProperty_PhaseChangeFields::Name, modelObject.material().nameString());

    idfObject.setDouble(MaterialProperty_PhaseChangeFields::TemperatureCoefficientforThermalConductivity,
                        modelObject.temperatureCoefficientforThermalConductivity());

    std::vector<TemperatureEnthalpy> temperatureEnthalpys = modelObject.temperatureEnthalpys();
    if (!temperatureEnthalpys.empty()) {
      for (const TemperatureEnthalpy& temperatureEnthalpy : temperatureEnthalpys) {
        auto eg = idfObject.pushExtensibleGroup();
        eg.setDouble(MaterialProperty_PhaseChangeExtensibleFields::Temperature, temperatureEnthalpy.temperature());
        eg.setDouble(MaterialProperty_PhaseChangeExtensibleFields::Enthalpy, temperatureEnthalpy.enthalpy());
      }
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
