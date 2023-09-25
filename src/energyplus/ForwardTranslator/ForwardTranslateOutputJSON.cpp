/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/OutputJSON.hpp"

#include <utilities/idd/Output_JSON_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateOutputJSON(model::OutputJSON& modelObject) {

    IdfObject idfObject = createAndRegisterIdfObject(openstudio::IddObjectType::Output_JSON, modelObject);

    // Option Type: Required String
    std::string optionType = modelObject.optionType();
    idfObject.setString(Output_JSONFields::OptionType, optionType);

    // Output JSON: Optional Boolean
    if (modelObject.outputJSON()) {
      idfObject.setString(Output_JSONFields::OutputJSON, "Yes");
    } else {
      idfObject.setString(Output_JSONFields::OutputJSON, "No");
    }

    // Output CBOR: Optional Boolean
    if (modelObject.outputCBOR()) {
      idfObject.setString(Output_JSONFields::OutputCBOR, "Yes");
    } else {
      idfObject.setString(Output_JSONFields::OutputCBOR, "No");
    }

    // Output MessagePack: Optional Boolean
    if (modelObject.outputMessagePack()) {
      idfObject.setString(Output_JSONFields::OutputMessagePack, "Yes");
    } else {
      idfObject.setString(Output_JSONFields::OutputMessagePack, "No");
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
