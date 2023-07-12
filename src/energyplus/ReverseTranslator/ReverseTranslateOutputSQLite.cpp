/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/OutputSQLite.hpp"
#include "../../model/OutputSQLite_Impl.hpp"

#include <utilities/idd/Output_SQLite_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateOutputSQLite(const WorkspaceObject& workspaceObject) {
    boost::optional<ModelObject> result;

    // This is a Unique ModelObject
    openstudio::model::OutputSQLite modelObject = m_model.getUniqueModelObject<OutputSQLite>();

    if (boost::optional<std::string> _optionType = workspaceObject.getString(Output_SQLiteFields::OptionType, true)) {
      modelObject.setOptionType(_optionType.get());
    }

    if (boost::optional<std::string> _unitConversionforTabularData =
          workspaceObject.getString(Output_SQLiteFields::UnitConversionforTabularData, true)) {
      modelObject.setUnitConversionforTabularData(_unitConversionforTabularData.get());
    }

    result = modelObject;
    return result;

  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
