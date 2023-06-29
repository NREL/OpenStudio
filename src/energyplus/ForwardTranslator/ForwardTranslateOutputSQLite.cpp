/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/OutputSQLite.hpp"

#include <utilities/idd/Output_SQLite_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateOutputSQLite(model::OutputSQLite& modelObject) {

    IdfObject idfObject = createAndRegisterIdfObject(openstudio::IddObjectType::Output_SQLite, modelObject);

    idfObject.setString(Output_SQLiteFields::OptionType, modelObject.optionType());

    idfObject.setString(Output_SQLiteFields::UnitConversionforTabularData, modelObject.unitConversionforTabularData());

    return idfObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
