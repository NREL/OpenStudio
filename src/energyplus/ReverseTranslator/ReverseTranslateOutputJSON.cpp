/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/OutputJSON.hpp"
#include "../../model/OutputJSON_Impl.hpp"

#include <utilities/idd/Output_JSON_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateOutputJSON(const WorkspaceObject& workspaceObject) {
    boost::optional<ModelObject> result;

    // This is a Unique ModelObject
    auto modelObject = m_model.getUniqueModelObject<OutputJSON>();

    // Option Type: Required String
    if (boost::optional<std::string> _optionType = workspaceObject.getString(Output_JSONFields::OptionType)) {
      modelObject.setOptionType(_optionType.get());
    } else {
      LOG(Error, "For " << workspaceObject.briefDescription() << ", cannot find required property 'Option Type'");
      return result;
    }

    // Output JSON: Optional Boolean
    if (boost::optional<std::string> _outputJSON = workspaceObject.getString(Output_JSONFields::OutputJSON, true)) {
      if (istringEqual("Yes", _outputJSON.get())) {
        modelObject.setOutputJSON(true);
      } else {
        modelObject.setOutputJSON(false);
      }
    }

    // Output CBOR: Optional Boolean
    if (boost::optional<std::string> _outputCBOR = workspaceObject.getString(Output_JSONFields::OutputCBOR, true)) {
      if (istringEqual("Yes", _outputCBOR.get())) {
        modelObject.setOutputCBOR(true);
      } else {
        modelObject.setOutputCBOR(false);
      }
    }

    // Output MessagePack: Optional Boolean
    if (boost::optional<std::string> _outputMessagePack = workspaceObject.getString(Output_JSONFields::OutputMessagePack, true)) {
      if (istringEqual("Yes", _outputMessagePack.get())) {
        modelObject.setOutputMessagePack(true);
      } else {
        modelObject.setOutputMessagePack(false);
      }
    }

    result = modelObject;
    return result;

  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
