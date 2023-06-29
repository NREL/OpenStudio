/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/GeneratorFuelCellAuxiliaryHeater.hpp"
#include "../../model/GeneratorFuelCellAuxiliaryHeater_Impl.hpp"

#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Generator_FuelCell_AuxiliaryHeater_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateGeneratorFuelCellAuxiliaryHeater(GeneratorFuelCellAuxiliaryHeater& modelObject) {
    boost::optional<std::string> s;
    boost::optional<double> d;
    boost::optional<ThermalZone> tz;

    IdfObject pcm = createAndRegisterIdfObject(openstudio::IddObjectType::Generator_FuelCell_AuxiliaryHeater, modelObject);
    //Name
    s = modelObject.name();
    if (s) {
      pcm.setName(*s);
    }

    //ExcessAirRatio
    d = modelObject.excessAirRatio();
    if (d) {
      pcm.setDouble(Generator_FuelCell_AuxiliaryHeaterFields::ExcessAirRatio, d.get());
    }

    //AncillaryPowerConstantTerm
    d = modelObject.ancillaryPowerConstantTerm();
    if (d) {
      pcm.setDouble(Generator_FuelCell_AuxiliaryHeaterFields::AncillaryPowerConstantTerm, d.get());
    }

    //AncillaryPowerLinearTerm
    d = modelObject.ancillaryPowerLinearTerm();
    if (d) {
      pcm.setDouble(Generator_FuelCell_AuxiliaryHeaterFields::AncillaryPowerLinearTerm, d.get());
    }

    //SkinLossUFactorTimesAreaValue
    d = modelObject.skinLossUFactorTimesAreaValue();
    if (d) {
      pcm.setDouble(Generator_FuelCell_AuxiliaryHeaterFields::SkinLossUFactorTimesAreaValue, d.get());
    }

    //SkinLossDestination
    s = modelObject.skinLossDestination();
    if (s) {
      pcm.setString(Generator_FuelCell_AuxiliaryHeaterFields::SkinLossDestination, s.get());
    }

    //ZoneNametoReceiveSkinLosses
    tz = modelObject.zonetoReceiveSkinLosses();
    if (tz) {
      pcm.setString(Generator_FuelCell_AuxiliaryHeaterFields::ZoneNametoReceiveSkinLosses, tz.get().nameString());
    }

    //HeatingCapacityUnits
    s = modelObject.heatingCapacityUnits();
    if (s) {
      pcm.setString(Generator_FuelCell_AuxiliaryHeaterFields::HeatingCapacityUnits, s.get());
    }

    //MaximumHeatingCapacityinWatts
    d = modelObject.maximumHeatingCapacityinWatts();
    if (d) {
      pcm.setDouble(Generator_FuelCell_AuxiliaryHeaterFields::MaximumHeatingCapacityinWatts, d.get());
    }

    //MinimumHeatingCapacityinWatts
    d = modelObject.minimumHeatingCapacityinWatts();
    if (d) {
      pcm.setDouble(Generator_FuelCell_AuxiliaryHeaterFields::MinimumHeatingCapacityinWatts, d.get());
    }

    //MaximumHeatingCapacityinKmolperSecond
    d = modelObject.maximumHeatingCapacityinKmolperSecond();
    if (d) {
      pcm.setDouble(Generator_FuelCell_AuxiliaryHeaterFields::MaximumHeatingCapacityinKmolperSecond, d.get());
    }

    //MinimumHeatingCapacityinKmolperSecond
    d = modelObject.minimumHeatingCapacityinKmolperSecond();
    if (d) {
      pcm.setDouble(Generator_FuelCell_AuxiliaryHeaterFields::MinimumHeatingCapacityinKmolperSecond, d.get());
    }

    return pcm;
  }

}  // namespace energyplus

}  // namespace openstudio
