/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/SiteGroundTemperatureBuildingSurface.hpp"
#include "../../model/SiteGroundTemperatureBuildingSurface_Impl.hpp"

#include <utilities/idd/Site_GroundTemperature_BuildingSurface_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSiteGroundTemperatureBuildingSurface(SiteGroundTemperatureBuildingSurface& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Site_GroundTemperature_BuildingSurface);

    m_idfObjects.push_back(idfObject);

    if (!modelObject.isJanuaryGroundTemperatureDefaulted()) {
      idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::JanuaryGroundTemperature, modelObject.januaryGroundTemperature());
    }

    if (!modelObject.isFebruaryGroundTemperatureDefaulted()) {
      idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::FebruaryGroundTemperature, modelObject.februaryGroundTemperature());
    }

    if (!modelObject.isMarchGroundTemperatureDefaulted()) {
      idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::MarchGroundTemperature, modelObject.marchGroundTemperature());
    }

    if (!modelObject.isAprilGroundTemperatureDefaulted()) {
      idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::AprilGroundTemperature, modelObject.aprilGroundTemperature());
    }

    if (!modelObject.isMayGroundTemperatureDefaulted()) {
      idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::MayGroundTemperature, modelObject.mayGroundTemperature());
    }

    if (!modelObject.isJuneGroundTemperatureDefaulted()) {
      idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::JuneGroundTemperature, modelObject.juneGroundTemperature());
    }

    if (!modelObject.isJulyGroundTemperatureDefaulted()) {
      idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::JulyGroundTemperature, modelObject.julyGroundTemperature());
    }

    if (!modelObject.isAugustGroundTemperatureDefaulted()) {
      idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::AugustGroundTemperature, modelObject.augustGroundTemperature());
    }

    if (!modelObject.isSeptemberGroundTemperatureDefaulted()) {
      idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::SeptemberGroundTemperature, modelObject.septemberGroundTemperature());
    }

    if (!modelObject.isOctoberGroundTemperatureDefaulted()) {
      idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::OctoberGroundTemperature, modelObject.octoberGroundTemperature());
    }

    if (!modelObject.isNovemberGroundTemperatureDefaulted()) {
      idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::NovemberGroundTemperature, modelObject.novemberGroundTemperature());
    }

    if (!modelObject.isDecemberGroundTemperatureDefaulted()) {
      idfObject.setDouble(Site_GroundTemperature_BuildingSurfaceFields::DecemberGroundTemperature, modelObject.decemberGroundTemperature());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
