/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/OutputDiagnostics.hpp"
#include "../../model/OutputDiagnostics_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/Output_Diagnostics_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateOutputDiagnostics(const WorkspaceObject& workspaceObject) {
    boost::optional<ModelObject> result;

    std::vector<std::string> keys;

    for (const IdfExtensibleGroup& eg : workspaceObject.extensibleGroups()) {
      if (auto _s = eg.getString(Output_DiagnosticsExtensibleFields::Key)) {
        keys.push_back(_s.get());
      }
    }

    if (!keys.empty()) {
      // Instantiate an object of the class to store the values,
      auto modelObject = m_model.getUniqueModelObject<OutputDiagnostics>();
      modelObject.setKeys(keys);
      result = modelObject;
    }

    return result;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
