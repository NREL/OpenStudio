/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    openstudio::model::OutputJSON modelObject = m_model.getUniqueModelObject<OutputJSON>();

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
