/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/InteriorPartitionSurfaceGroup.hpp"
#include "../../model/InteriorPartitionSurfaceGroup_Impl.hpp"
#include "../../model/InteriorPartitionSurface.hpp"

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateInteriorPartitionSurfaceGroup(model::InteriorPartitionSurfaceGroup& modelObject) {
    InteriorPartitionSurfaceVector interiorPartitionSurfaces = modelObject.interiorPartitionSurfaces();
    std::sort(interiorPartitionSurfaces.begin(), interiorPartitionSurfaces.end(), WorkspaceObjectNameLess());
    for (InteriorPartitionSurface& interiorPartitionSurface : interiorPartitionSurfaces) {
      translateAndMapModelObject(interiorPartitionSurface);
    }

    return boost::none;
  }

}  // namespace energyplus

}  // namespace openstudio
