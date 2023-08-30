/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/OutsideSurfaceConvectionAlgorithm.hpp"
#include "../../model/OutsideSurfaceConvectionAlgorithm_Impl.hpp"
#include <utilities/idd/SurfaceConvectionAlgorithm_Outside_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateSurfaceConvectionAlgorithmOutside(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::SurfaceConvectionAlgorithm_Outside) {
      LOG(Error, "WorkspaceObject is not IddObjectType: SurfaceConvectionAlgorithm_Outside");
      return boost::none;
    }

    auto mo = m_model.getUniqueModelObject<OutsideSurfaceConvectionAlgorithm>();

    boost::optional<std::string> s = workspaceObject.getString(SurfaceConvectionAlgorithm_OutsideFields::Algorithm);
    if (s) {
      mo.setAlgorithm(s.get());
    }

    return mo;
  }

}  // namespace energyplus

}  // namespace openstudio
