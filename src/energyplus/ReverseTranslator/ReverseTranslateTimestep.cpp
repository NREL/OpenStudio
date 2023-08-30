/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/Timestep.hpp"
#include "../../model/Timestep_Impl.hpp"
#include <utilities/idd/Timestep_FieldEnums.hxx>
#include <utilities/idd/OS_Timestep_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateTimestep(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Timestep) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Timestep");
      return boost::none;
    }

    auto mo = m_model.getUniqueModelObject<Timestep>();

    boost::optional<int> i = workspaceObject.getInt(TimestepFields::NumberofTimestepsperHour);
    if (i) {
      mo.setInt(OS_TimestepFields::NumberofTimestepsperHour, i.get());
    }

    return mo;
  }

}  // namespace energyplus

}  // namespace openstudio
