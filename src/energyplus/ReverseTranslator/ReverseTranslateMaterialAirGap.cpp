/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/AirGap.hpp"
#include "../../model/AirGap_Impl.hpp"

#include <utilities/idd/Material_AirGap_FieldEnums.hxx>
#include <utilities/idd/OS_Material_AirGap_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateMaterialAirGap(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Material_AirGap) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Material_AirGap");
      return boost::none;
    }

    openstudio::model::AirGap airGap(m_model);
    OptionalString s = workspaceObject.name();
    if (s) {
      airGap.setName(*s);
    }

    OptionalDouble d = workspaceObject.getDouble(Material_AirGapFields::ThermalResistance);
    if (d) {
      airGap.setDouble(OS_Material_AirGapFields::ThermalResistance, *d);
    }

    return airGap;
  }

}  // namespace energyplus

}  // namespace openstudio
