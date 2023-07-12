/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ShadingSurfaceGroup.hpp"
#include "../../model/ShadingSurfaceGroup_Impl.hpp"
#include "../../model/ShadingSurface.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateShadingSurfaceGroup(model::ShadingSurfaceGroup& modelObject) {

    ShadingSurfaceVector shadingSurfaces = modelObject.shadingSurfaces();
    std::sort(shadingSurfaces.begin(), shadingSurfaces.end(), WorkspaceObjectNameLess());
    for (ShadingSurface& shadingSurface : shadingSurfaces) {
      translateAndMapModelObject(shadingSurface);
    }

    return boost::none;
  }

}  // namespace energyplus

}  // namespace openstudio
