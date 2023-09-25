/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/OutputSchedules.hpp"
#include "../../model/OutputSchedules_Impl.hpp"

#include <utilities/idd/Output_Schedules_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateOutputSchedules(const WorkspaceObject& workspaceObject) {
    boost::optional<ModelObject> result;

    if (boost::optional<std::string> keyField_ = workspaceObject.getString(Output_SchedulesFields::KeyField, false, true)) {
      // This is a Unique ModelObject
      auto modelObject = m_model.getUniqueModelObject<OutputSchedules>();
      modelObject.setKeyField(keyField_.get());
      result = modelObject;
    }

    return result;

  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
