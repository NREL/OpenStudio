/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/SiteGroundTemperatureShallow.hpp"
#include "../../model/SiteGroundTemperatureShallow_Impl.hpp"

#include <utilities/idd/Site_GroundTemperature_Shallow_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSiteGroundTemperatureShallow(SiteGroundTemperatureShallow& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Site_GroundTemperature_Shallow);

    m_idfObjects.push_back(idfObject);

    idfObject.setDouble(Site_GroundTemperature_ShallowFields::JanuarySurfaceGroundTemperature, modelObject.januarySurfaceGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_ShallowFields::FebruarySurfaceGroundTemperature, modelObject.februarySurfaceGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_ShallowFields::MarchSurfaceGroundTemperature, modelObject.marchSurfaceGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_ShallowFields::AprilSurfaceGroundTemperature, modelObject.aprilSurfaceGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_ShallowFields::MaySurfaceGroundTemperature, modelObject.maySurfaceGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_ShallowFields::JuneSurfaceGroundTemperature, modelObject.juneSurfaceGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_ShallowFields::JulySurfaceGroundTemperature, modelObject.julySurfaceGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_ShallowFields::AugustSurfaceGroundTemperature, modelObject.augustSurfaceGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_ShallowFields::SeptemberSurfaceGroundTemperature, modelObject.septemberSurfaceGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_ShallowFields::OctoberSurfaceGroundTemperature, modelObject.octoberSurfaceGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_ShallowFields::NovemberSurfaceGroundTemperature, modelObject.novemberSurfaceGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_ShallowFields::DecemberSurfaceGroundTemperature, modelObject.decemberSurfaceGroundTemperature());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
