/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/OutputDiagnostics.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Output_Diagnostics_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateOutputDiagnostics(model::OutputDiagnostics& modelObject) {
    // If nothing to write, don't
    auto keys = modelObject.keys();
    if (keys.empty()) {
      return boost::none;
    }

    IdfObject idfObject = createAndRegisterIdfObject(openstudio::IddObjectType::Output_Diagnostics, modelObject);

    for (const std::string& key : modelObject.keys()) {
      IdfExtensibleGroup eg = idfObject.pushExtensibleGroup();
      eg.setString(Output_DiagnosticsExtensibleFields::Key, key);
    }

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
