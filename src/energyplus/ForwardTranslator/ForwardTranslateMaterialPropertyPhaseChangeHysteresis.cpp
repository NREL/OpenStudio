/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/MaterialPropertyPhaseChangeHysteresis.hpp"
#include "../../model/MaterialPropertyPhaseChangeHysteresis_Impl.hpp"
#include "../../model/Material.hpp"
#include "../../model/Material_Impl.hpp"

#include <utilities/idd/MaterialProperty_PhaseChangeHysteresis_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateMaterialPropertyPhaseChangeHysteresis(MaterialPropertyPhaseChangeHysteresis& modelObject) {
    IdfObject idfObject = createAndRegisterIdfObject(openstudio::IddObjectType::MaterialProperty_PhaseChangeHysteresis, modelObject);

    idfObject.setString(MaterialProperty_PhaseChangeHysteresisFields::Name, modelObject.material().nameString());

    idfObject.setDouble(MaterialProperty_PhaseChangeHysteresisFields::LatentHeatduringtheEntirePhaseChangeProcess,
                        modelObject.latentHeatduringtheEntirePhaseChangeProcess());

    idfObject.setDouble(MaterialProperty_PhaseChangeHysteresisFields::LiquidStateThermalConductivity, modelObject.liquidStateThermalConductivity());

    idfObject.setDouble(MaterialProperty_PhaseChangeHysteresisFields::LiquidStateDensity, modelObject.liquidStateDensity());

    idfObject.setDouble(MaterialProperty_PhaseChangeHysteresisFields::LiquidStateSpecificHeat, modelObject.liquidStateSpecificHeat());

    idfObject.setDouble(MaterialProperty_PhaseChangeHysteresisFields::HighTemperatureDifferenceofMeltingCurve,
                        modelObject.highTemperatureDifferenceofMeltingCurve());

    idfObject.setDouble(MaterialProperty_PhaseChangeHysteresisFields::PeakMeltingTemperature, modelObject.peakMeltingTemperature());

    idfObject.setDouble(MaterialProperty_PhaseChangeHysteresisFields::LowTemperatureDifferenceofMeltingCurve,
                        modelObject.lowTemperatureDifferenceofMeltingCurve());

    idfObject.setDouble(MaterialProperty_PhaseChangeHysteresisFields::SolidStateThermalConductivity, modelObject.solidStateThermalConductivity());

    idfObject.setDouble(MaterialProperty_PhaseChangeHysteresisFields::SolidStateDensity, modelObject.solidStateDensity());

    idfObject.setDouble(MaterialProperty_PhaseChangeHysteresisFields::SolidStateSpecificHeat, modelObject.solidStateSpecificHeat());

    idfObject.setDouble(MaterialProperty_PhaseChangeHysteresisFields::HighTemperatureDifferenceofFreezingCurve,
                        modelObject.highTemperatureDifferenceofFreezingCurve());

    idfObject.setDouble(MaterialProperty_PhaseChangeHysteresisFields::PeakFreezingTemperature, modelObject.peakFreezingTemperature());

    idfObject.setDouble(MaterialProperty_PhaseChangeHysteresisFields::LowTemperatureDifferenceofFreezingCurve,
                        modelObject.lowTemperatureDifferenceofFreezingCurve());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
