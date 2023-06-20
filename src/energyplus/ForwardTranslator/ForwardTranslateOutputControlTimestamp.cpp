/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/OutputControlTimestamp.hpp"

#include <utilities/idd/OutputControl_Timestamp_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateOutputControlTimestamp(model::OutputControlTimestamp& modelObject) {

    IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::OutputControl_Timestamp, modelObject);

    // ISO 8601 Format: Optional Boolean
    if (modelObject.iso8601Format()) {
      idfObject.setString(OutputControl_TimestampFields::ISO8601Format, "Yes");
    } else {
      idfObject.setString(OutputControl_TimestampFields::ISO8601Format, "No");
    }

    // Timestamp at Beginning of Interval: Optional Boolean
    if (modelObject.timestampAtBeginningOfInterval()) {
      idfObject.setString(OutputControl_TimestampFields::TimestampatBeginningofInterval, "Yes");
    } else {
      idfObject.setString(OutputControl_TimestampFields::TimestampatBeginningofInterval, "No");
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio