/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/InsideSurfaceConvectionAlgorithm.hpp"
#include "../../model/InsideSurfaceConvectionAlgorithm_Impl.hpp"
#include <utilities/idd/SurfaceConvectionAlgorithm_Inside_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateSurfaceConvectionAlgorithmInside(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::SurfaceConvectionAlgorithm_Inside) {
      LOG(Error, "WorkspaceObject is not IddObjectType: SurfaceConvectionAlgorithm_Inside");
      return boost::none;
    }

    auto mo = m_model.getUniqueModelObject<InsideSurfaceConvectionAlgorithm>();

    if (OptionalString s = workspaceObject.getString(SurfaceConvectionAlgorithm_InsideFields::Algorithm)) {
      mo.setAlgorithm(s.get());
    }

    return mo;
  }

}  // namespace energyplus
}  // namespace openstudio
