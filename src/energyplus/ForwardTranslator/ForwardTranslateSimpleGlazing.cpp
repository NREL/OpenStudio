/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/SimpleGlazing.hpp"
#include "../../model/SimpleGlazing_Impl.hpp"

#include <utilities/idd/WindowMaterial_SimpleGlazingSystem_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSimpleGlazing(SimpleGlazing& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::WindowMaterial_SimpleGlazingSystem);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(WindowMaterial_SimpleGlazingSystemFields::Name, modelObject.name().get());

    idfObject.setDouble(WindowMaterial_SimpleGlazingSystemFields::UFactor, modelObject.uFactor());

    idfObject.setDouble(WindowMaterial_SimpleGlazingSystemFields::SolarHeatGainCoefficient, modelObject.solarHeatGainCoefficient());

    OptionalDouble d = modelObject.visibleTransmittance();
    if (d) {
      idfObject.setDouble(WindowMaterial_SimpleGlazingSystemFields::VisibleTransmittance, *d);
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
