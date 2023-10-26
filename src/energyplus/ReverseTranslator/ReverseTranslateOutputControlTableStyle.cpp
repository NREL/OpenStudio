/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/OutputControlTableStyle.hpp"
#include "../../model/OutputControlTableStyle_Impl.hpp"

#include <utilities/idd/OutputControl_Table_Style_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateOutputControlTableStyle(const WorkspaceObject& workspaceObject) {
    boost::optional<ModelObject> result;

    // This is a Unique ModelObject
    openstudio::model::OutputControlTableStyle modelObject = m_model.getUniqueModelObject<OutputControlTableStyle>();

    if (boost::optional<std::string> _columnSeparator = workspaceObject.getString(OutputControl_Table_StyleFields::ColumnSeparator, true)) {
      modelObject.setColumnSeparator(_columnSeparator.get());
    }

    if (boost::optional<std::string> _unitConversion = workspaceObject.getString(OutputControl_Table_StyleFields::UnitConversion, true)) {
      modelObject.setUnitConversion(_unitConversion.get());
    }

    result = modelObject;
    return result;

  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
