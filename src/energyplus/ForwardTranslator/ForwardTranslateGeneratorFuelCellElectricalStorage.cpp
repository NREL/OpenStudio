/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/GeneratorFuelCellElectricalStorage.hpp"
#include "../../model/GeneratorFuelCellElectricalStorage_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Generator_FuelCell_ElectricalStorage_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateGeneratorFuelCellElectricalStorage(GeneratorFuelCellElectricalStorage& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> d;

    IdfObject pcm = createAndRegisterIdfObject(openstudio::IddObjectType::Generator_FuelCell_ElectricalStorage, modelObject);
    //Name
    s = modelObject.name();
    if (s) {
      pcm.setName(*s);
    }

    //ChoiceofModel
    s = modelObject.choiceofModel();
    if (s) {
      pcm.setString(Generator_FuelCell_ElectricalStorageFields::ChoiceofModel, s.get());
    }

    //NominalChargingEnergeticEfficiency
    d = modelObject.nominalChargingEnergeticEfficiency();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ElectricalStorageFields::NominalChargingEnergeticEfficiency, d.get());
    }

    //NominalDischargingEnergeticEfficiency
    d = modelObject.nominalDischargingEnergeticEfficiency();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ElectricalStorageFields::NominalDischargingEnergeticEfficiency, d.get());
    }

    //SimpleMaximumCapacity
    d = modelObject.simpleMaximumCapacity();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ElectricalStorageFields::SimpleMaximumCapacity, d.get());
    }

    //SimpleMaximumPowerDraw
    d = modelObject.simpleMaximumPowerDraw();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ElectricalStorageFields::SimpleMaximumPowerDraw, d.get());
    }

    //SimpleMaximumPowerStore
    d = modelObject.simpleMaximumPowerStore();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ElectricalStorageFields::SimpleMaximumPowerStore, d.get());
    }

    //InitialChargeState
    d = modelObject.initialChargeState();
    if (d) {
      pcm.setDouble(Generator_FuelCell_ElectricalStorageFields::InitialChargeState, d.get());
    }

    return pcm;
  }

}  // namespace energyplus

}  // namespace openstudio
