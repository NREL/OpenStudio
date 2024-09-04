/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/Site.hpp"
#include "../../model/Site_Impl.hpp"
#include "../../model/ShadingSurfaceGroup.hpp"

#include <utilities/idd/Site_Location_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateSite(Site& modelObject) {
    IdfObject site = createAndRegisterIdfObject(openstudio::IddObjectType::Site_Location, modelObject);

    OptionalString optS = modelObject.name();
    if (optS) {
      site.setName(*optS);
    }

    OptionalDouble od = modelObject.latitude();
    if (od) {
      site.setDouble(Site_LocationFields::Latitude, *od);
    }

    od = modelObject.longitude();
    if (od) {
      site.setDouble(Site_LocationFields::Longitude, *od);
    }

    od = modelObject.timeZone();
    if (od) {
      site.setDouble(Site_LocationFields::TimeZone, *od);
    }

    od = modelObject.elevation();
    if (od) {
      site.setDouble(Site_LocationFields::Elevation, *od);
    }

    if (!modelObject.isKeepSiteLocationInformationDefaulted()) {
      if (modelObject.keepSiteLocationInformation()) {
        site.setString(Site_LocationFields::KeepSiteLocationInformation, "Yes");
      } else {
        site.setString(Site_LocationFields::KeepSiteLocationInformation, "No");
      }
    }

    // translate shading groups
    ShadingSurfaceGroupVector shadingSurfaceGroups = modelObject.shadingSurfaceGroups();
    std::sort(shadingSurfaceGroups.begin(), shadingSurfaceGroups.end(), WorkspaceObjectNameLess());
    for (ShadingSurfaceGroup& shadingSurfaceGroup : shadingSurfaceGroups) {
      translateAndMapModelObject(shadingSurfaceGroup);
    }

    return boost::optional<IdfObject>(site);
  }

}  // namespace energyplus

}  // namespace openstudio
