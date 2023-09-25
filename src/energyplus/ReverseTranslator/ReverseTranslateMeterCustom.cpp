/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/MeterCustom.hpp"
#include "../../model/MeterCustom_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Meter_Custom_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateMeterCustom(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Meter_Custom) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Meter:Custom");
      return boost::none;
    }

    // Create an OS:Meter:Custom object
    MeterCustom meterCustom = MeterCustom(m_model);

    // Name
    boost::optional<std::string> s = workspaceObject.getString(Meter_CustomFields::Name);
    if (s) {
      meterCustom.setName(s.get());
    }

    // Fuel Type
    s = workspaceObject.getString(Meter_CustomFields::ResourceType);
    if (s) {
      // TODO: JM to DLM: should I also check it's part of the validFuelTypes?
      meterCustom.setFuelType(s.get());
    }

    // Get all the (key, var) extensible groups from IDF
    std::vector<IdfExtensibleGroup> keyVarGroups = workspaceObject.extensibleGroups();

    // Clean out the (key, var) groups (just in case: the constructor doesn't default any, for now at least!)
    meterCustom.removeAllKeyVarGroups();

    // Push them all to the object
    for (const auto& keyVarGroup : keyVarGroups) {
      meterCustom.pushExtensibleGroup(keyVarGroup.fields());
    }

    return meterCustom;
  }

}  // namespace energyplus

}  // namespace openstudio
