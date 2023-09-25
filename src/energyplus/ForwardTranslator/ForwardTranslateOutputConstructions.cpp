/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"
#include "../../model/Model.hpp"

#include "../../model/OutputConstructions.hpp"

#include <utilities/idd/Output_Constructions_FieldEnums.hxx>
// #include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateOutputConstructions(OutputConstructions& modelObject) {
    // If nothing to write, don't
    bool reportConstructions = modelObject.reportConstructions();
    bool reportMaterials = modelObject.reportMaterials();
    if (!reportConstructions && !reportMaterials) {
      return boost::none;
    }

    IdfObject idfObject = createAndRegisterIdfObject(openstudio::IddObjectType::Output_Constructions, modelObject);

    if (reportConstructions) {
      idfObject.setString(Output_ConstructionsFields::DetailsType1, "Constructions");
      if (reportMaterials) {
        idfObject.setString(Output_ConstructionsFields::DetailsType2, "Materials");
      }
    } else if (reportMaterials) {
      idfObject.setString(Output_ConstructionsFields::DetailsType1, "Materials");
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
