/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/OutputControlTimestamp.hpp"
#include "../../model/OutputControlTimestamp_Impl.hpp"

#include <utilities/idd/OutputControl_Timestamp_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateOutputControlTimestamp(const WorkspaceObject& workspaceObject) {
    boost::optional<ModelObject> result;

    // This is a Unique ModelObject
    openstudio::model::OutputControlTimestamp modelObject = m_model.getUniqueModelObject<OutputControlTimestamp>();

    if (boost::optional<std::string> _iso8601Format = workspaceObject.getString(OutputControl_TimestampFields::ISO8601Format, true)) {
      if (istringEqual("Yes", _iso8601Format.get())) {
        modelObject.setISO8601Format(true);
      } else {
        modelObject.setISO8601Format(false);
      }
    }

    if (boost::optional<std::string> _timestampAtBeginningofInterval =
          workspaceObject.getString(OutputControl_TimestampFields::TimestampatBeginningofInterval, true)) {
      if (istringEqual("Yes", _timestampAtBeginningofInterval.get())) {
        modelObject.setTimestampAtBeginningOfInterval(true);
      } else {
        modelObject.setTimestampAtBeginningOfInterval(false);
      }
    }

    result = modelObject;
    return result;

  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
