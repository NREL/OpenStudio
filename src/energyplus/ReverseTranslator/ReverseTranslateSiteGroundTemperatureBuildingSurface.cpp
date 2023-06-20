/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"
#include "../../model/SiteGroundTemperatureBuildingSurface.hpp"
#include "../../model/SiteGroundTemperatureBuildingSurface_Impl.hpp"
#include <utilities/idd/Site_GroundTemperature_BuildingSurface_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateSiteGroundTemperatureBuildingSurface(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Site_GroundTemperature_BuildingSurface) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Site_GroundTemperature_BuildingSurface");
      return boost::none;
    }

    auto mo = m_model.getUniqueModelObject<SiteGroundTemperatureBuildingSurface>();

    boost::optional<double> value = workspaceObject.getDouble(Site_GroundTemperature_BuildingSurfaceFields::JanuaryGroundTemperature);
    if (value) {
      mo.setJanuaryGroundTemperature(value.get());
    }

    value = workspaceObject.getDouble(Site_GroundTemperature_BuildingSurfaceFields::FebruaryGroundTemperature);
    if (value) {
      mo.setFebruaryGroundTemperature(value.get());
    }

    value = workspaceObject.getDouble(Site_GroundTemperature_BuildingSurfaceFields::MarchGroundTemperature);
    if (value) {
      mo.setMarchGroundTemperature(value.get());
    }

    value = workspaceObject.getDouble(Site_GroundTemperature_BuildingSurfaceFields::AprilGroundTemperature);
    if (value) {
      mo.setAprilGroundTemperature(value.get());
    }

    value = workspaceObject.getDouble(Site_GroundTemperature_BuildingSurfaceFields::MayGroundTemperature);
    if (value) {
      mo.setMayGroundTemperature(value.get());
    }

    value = workspaceObject.getDouble(Site_GroundTemperature_BuildingSurfaceFields::JuneGroundTemperature);
    if (value) {
      mo.setJuneGroundTemperature(value.get());
    }

    value = workspaceObject.getDouble(Site_GroundTemperature_BuildingSurfaceFields::JulyGroundTemperature);
    if (value) {
      mo.setJulyGroundTemperature(value.get());
    }

    value = workspaceObject.getDouble(Site_GroundTemperature_BuildingSurfaceFields::AugustGroundTemperature);
    if (value) {
      mo.setAugustGroundTemperature(value.get());
    }

    value = workspaceObject.getDouble(Site_GroundTemperature_BuildingSurfaceFields::SeptemberGroundTemperature);
    if (value) {
      mo.setSeptemberGroundTemperature(value.get());
    }

    value = workspaceObject.getDouble(Site_GroundTemperature_BuildingSurfaceFields::OctoberGroundTemperature);
    if (value) {
      mo.setOctoberGroundTemperature(value.get());
    }

    value = workspaceObject.getDouble(Site_GroundTemperature_BuildingSurfaceFields::NovemberGroundTemperature);
    if (value) {
      mo.setNovemberGroundTemperature(value.get());
    }

    value = workspaceObject.getDouble(Site_GroundTemperature_BuildingSurfaceFields::DecemberGroundTemperature);
    if (value) {
      mo.setDecemberGroundTemperature(value.get());
    }

    return mo;
  }

}  // namespace energyplus

}  // namespace openstudio
