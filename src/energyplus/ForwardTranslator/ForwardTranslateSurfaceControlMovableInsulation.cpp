/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/SurfaceControlMovableInsulation.hpp"
#include "../../model/SurfaceControlMovableInsulation_Impl.hpp"
#include "../../model/Material.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Surface.hpp"

#include <utilities/idd/SurfaceControl_MovableInsulation_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSurfaceControlMovableInsulation(SurfaceControlMovableInsulation& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::SurfaceControl_MovableInsulation);

    m_idfObjects.push_back(idfObject);

    std::string insulationType = modelObject.insulationType();

    idfObject.setString(SurfaceControl_MovableInsulationFields::InsulationType, insulationType);

    Surface surface = modelObject.surface();
    if (boost::optional<IdfObject> surf = translateAndMapModelObject(surface)) {
      idfObject.setString(SurfaceControl_MovableInsulationFields::SurfaceName, surf->name().get());
    }

    Material material = modelObject.material();
    if (boost::optional<IdfObject> mat = translateAndMapModelObject(material)) {
      idfObject.setString(SurfaceControl_MovableInsulationFields::MaterialName, mat->name().get());
    }

    Schedule schedule = modelObject.schedule();
    if (boost::optional<IdfObject> sch = translateAndMapModelObject(schedule)) {
      idfObject.setString(SurfaceControl_MovableInsulationFields::ScheduleName, sch->name().get());
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
