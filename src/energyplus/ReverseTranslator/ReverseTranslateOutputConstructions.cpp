/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/OutputConstructions.hpp"
#include "../../model/OutputConstructions_Impl.hpp"

#include <utilities/idd/Output_Constructions_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateOutputConstructions(const WorkspaceObject& workspaceObject) {

    bool reportConstructions = false;
    bool reportMaterials = false;

    for (unsigned index : {Output_ConstructionsFields::DetailsType1, Output_ConstructionsFields::DetailsType2}) {
      if (boost::optional<std::string> detailsType_ = workspaceObject.getString(index, true, false)) {
        if (istringEqual("Constructions", detailsType_.get())) {
          reportConstructions = true;
        } else if (istringEqual("Materials", detailsType_.get())) {
          reportMaterials = true;
        }
      }
    }

    if (!reportConstructions && !reportMaterials) {
      return boost::none;
    }

    // This is a Unique ModelObject
    openstudio::model::OutputConstructions modelObject = m_model.getUniqueModelObject<OutputConstructions>();

    modelObject.setReportConstructions(reportConstructions);
    modelObject.setReportMaterials(reportMaterials);

    return modelObject;

  }  // End of translate function

}  // end namespace energyplus
}  // end namespace openstudio
