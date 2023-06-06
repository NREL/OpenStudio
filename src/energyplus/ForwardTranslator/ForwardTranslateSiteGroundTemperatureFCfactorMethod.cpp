/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/SiteGroundTemperatureFCfactorMethod.hpp"
#include "../../model/SiteGroundTemperatureFCfactorMethod_Impl.hpp"

#include <utilities/idd/Site_GroundTemperature_FCfactorMethod_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSiteGroundTemperatureFCfactorMethod(SiteGroundTemperatureFCfactorMethod& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Site_GroundTemperature_FCfactorMethod);

    m_idfObjects.push_back(idfObject);

    idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::JanuaryGroundTemperature, modelObject.januaryGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::FebruaryGroundTemperature, modelObject.februaryGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::MarchGroundTemperature, modelObject.marchGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::AprilGroundTemperature, modelObject.aprilGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::MayGroundTemperature, modelObject.mayGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::JuneGroundTemperature, modelObject.juneGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::JulyGroundTemperature, modelObject.julyGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::AugustGroundTemperature, modelObject.augustGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::SeptemberGroundTemperature, modelObject.septemberGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::OctoberGroundTemperature, modelObject.octoberGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::NovemberGroundTemperature, modelObject.novemberGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_FCfactorMethodFields::DecemberGroundTemperature, modelObject.decemberGroundTemperature());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
