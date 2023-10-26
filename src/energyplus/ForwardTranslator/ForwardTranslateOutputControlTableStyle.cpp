/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"
#include "../../model/OutputControlTableStyle.hpp"
#include <utilities/idd/OutputControl_Table_Style_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateOutputControlTableStyle(OutputControlTableStyle& modelObject) {

    IdfObject idfObject = createAndRegisterIdfObject(openstudio::IddObjectType::OutputControl_Table_Style, modelObject);

    idfObject.setString(OutputControl_Table_StyleFields::ColumnSeparator, modelObject.columnSeparator());

    idfObject.setString(OutputControl_Table_StyleFields::UnitConversion, modelObject.unitConversion());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
