/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/SurfacePropertyOtherSideConditionsModel.hpp"

#include <utilities/idd/SurfaceProperty_OtherSideConditionsModel_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateSurfacePropertyOtherSideConditionsModel(const WorkspaceObject& workspaceObject) {

    openstudio::model::SurfacePropertyOtherSideConditionsModel modelObject(m_model);

    // Name
    if (boost::optional<std::string> name_ = workspaceObject.name()) {
      modelObject.setName(name_.get());
    }

    // Type of Modeling: Optional String
    if (boost::optional<std::string> typeofModeling_ = workspaceObject.getString(SurfaceProperty_OtherSideConditionsModelFields::TypeofModeling)) {
      modelObject.setTypeOfModeling(typeofModeling_.get());
    }

    return modelObject;
  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
