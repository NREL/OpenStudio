/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"
#include "../../model/MaterialPropertyMoisturePenetrationDepthSettings.hpp"
#include "../../model/MaterialPropertyMoisturePenetrationDepthSettings_Impl.hpp"
#include "../../model/MaterialPropertyPhaseChange.hpp"
#include "../../model/MaterialPropertyPhaseChange_Impl.hpp"
#include "../../model/MaterialPropertyPhaseChangeHysteresis.hpp"
#include "../../model/MaterialPropertyPhaseChangeHysteresis_Impl.hpp"

#include <utilities/idd/Material_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateStandardOpaqueMaterial(StandardOpaqueMaterial& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Material);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(openstudio::MaterialFields::Name, modelObject.name().get());

    idfObject.setString(openstudio::MaterialFields::Roughness, modelObject.roughness());

    idfObject.setDouble(openstudio::MaterialFields::Thickness, modelObject.thickness());

    idfObject.setDouble(openstudio::MaterialFields::Conductivity, modelObject.thermalConductivity());

    idfObject.setDouble(openstudio::MaterialFields::Density, modelObject.density());

    idfObject.setDouble(openstudio::MaterialFields::SpecificHeat, modelObject.specificHeat());

    OptionalDouble d = modelObject.thermalAbsorptance();
    if (d) {
      idfObject.setDouble(openstudio::MaterialFields::ThermalAbsorptance, *d);
    }

    d = modelObject.solarAbsorptance();
    if (d) {
      idfObject.setDouble(openstudio::MaterialFields::SolarAbsorptance, *d);
    }

    d = modelObject.visibleAbsorptance();
    if (d) {
      idfObject.setDouble(openstudio::MaterialFields::VisibleAbsorptance, *d);
    }

    // Call the translation of these objects, which has two advantages:
    // * will not translate them if they are orphaned (=not referencing a material), and,
    // * makes the order of these objects in the IDF deterministic
    if (boost::optional<MaterialPropertyMoisturePenetrationDepthSettings> _empd = modelObject.materialPropertyMoisturePenetrationDepthSettings()) {
      translateAndMapModelObject(_empd.get());
    }

    if (boost::optional<MaterialPropertyPhaseChange> _phaseChange = modelObject.materialPropertyPhaseChange()) {
      translateAndMapModelObject(_phaseChange.get());
    }

    if (boost::optional<MaterialPropertyPhaseChangeHysteresis> _phaseChangeHysteresis = modelObject.materialPropertyPhaseChangeHysteresis()) {
      translateAndMapModelObject(_phaseChangeHysteresis.get());
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
