/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/SiteGroundTemperatureDeep.hpp"
#include "../../model/SiteGroundTemperatureDeep_Impl.hpp"

#include <utilities/idd/Site_GroundTemperature_Deep_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSiteGroundTemperatureDeep(SiteGroundTemperatureDeep& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::Site_GroundTemperature_Deep);

    m_idfObjects.push_back(idfObject);

    idfObject.setDouble(Site_GroundTemperature_DeepFields::JanuaryDeepGroundTemperature, modelObject.januaryDeepGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_DeepFields::FebruaryDeepGroundTemperature, modelObject.februaryDeepGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_DeepFields::MarchDeepGroundTemperature, modelObject.marchDeepGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_DeepFields::AprilDeepGroundTemperature, modelObject.aprilDeepGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_DeepFields::MayDeepGroundTemperature, modelObject.mayDeepGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_DeepFields::JuneDeepGroundTemperature, modelObject.juneDeepGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_DeepFields::JulyDeepGroundTemperature, modelObject.julyDeepGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_DeepFields::AugustDeepGroundTemperature, modelObject.augustDeepGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_DeepFields::SeptemberDeepGroundTemperature, modelObject.septemberDeepGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_DeepFields::OctoberDeepGroundTemperature, modelObject.octoberDeepGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_DeepFields::NovemberDeepGroundTemperature, modelObject.novemberDeepGroundTemperature());

    idfObject.setDouble(Site_GroundTemperature_DeepFields::DecemberDeepGroundTemperature, modelObject.decemberDeepGroundTemperature());

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
