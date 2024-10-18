/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/Site.hpp"
#include "../../model/Site_Impl.hpp"

#include <utilities/idd/Site_Location_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateSiteLocation(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Site_Location) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Site:Location");
      return boost::none;
    }

    Site site = m_model.getUniqueModelObject<Site>();
    OptionalString optS = workspaceObject.name();
    if (optS) {
      site.setName(*optS);
    }

    OptionalDouble od = workspaceObject.getDouble(Site_LocationFields::Latitude);
    if (od) {
      site.setLatitude(*od);
    }

    od = workspaceObject.getDouble(Site_LocationFields::Longitude);
    if (od) {
      site.setLongitude(*od);
    }

    od = workspaceObject.getDouble(Site_LocationFields::TimeZone);
    if (od) {
      site.setTimeZone(*od);
    }

    od = workspaceObject.getDouble(Site_LocationFields::Elevation);
    if (od) {
      site.setElevation(*od);
    }

    optS = workspaceObject.getString(openstudio::Site_LocationFields::KeepSiteLocationInformation);
    if (optS) {
      if (istringEqual("Yes", *optS)) {
        site.setKeepSiteLocationInformation(true);
      } else {
        site.setKeepSiteLocationInformation(false);
      }
    }

    return site;
  }

}  // namespace energyplus

}  // namespace openstudio
