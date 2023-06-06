/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ZonePropertyUserViewFactorsBySurfaceName.hpp"
#include "../../model/ZonePropertyUserViewFactorsBySurfaceName_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/ZoneProperty_UserViewFactors_BySurfaceName_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject>
    ForwardTranslator::translateZonePropertyUserViewFactorsBySurfaceName(ZonePropertyUserViewFactorsBySurfaceName& modelObject) {
    ThermalZone zone = modelObject.thermalZone();
    boost::optional<IdfObject> _zone = translateAndMapModelObject(zone);
    if (!_zone) {
      LOG(Error, "ZonePropertyUserViewFactorsBySurfaceName references a zone '" << zone.nameString()
                                                                                << "' that was not translated, so it will not be translated either");
    }

    // No need to translate it if it doesn't have at least one view factor
    std::vector<ViewFactor> viewFactors = modelObject.viewFactors();
    if (viewFactors.empty()) {
      LOG(Error, "ZonePropertyUserViewFactorsBySurfaceName for zone '" << zone.nameString()
                                                                       << "' doesn't have at least one view factor, it will not be translated.");
      return boost::none;
    }

    IdfObject idfObject(openstudio::IddObjectType::ZoneProperty_UserViewFactors_BySurfaceName);
    m_idfObjects.push_back(idfObject);

    idfObject.setString(ZoneProperty_UserViewFactors_BySurfaceNameFields::ZoneorZoneListorSpaceorSpaceListName, _zone->name().get());

    for (const ViewFactor& viewFactor : viewFactors) {

      ModelObject fromSurface = viewFactor.fromSurface();
      boost::optional<IdfObject> _fromSurface = translateAndMapModelObject(fromSurface);
      ModelObject toSurface = viewFactor.toSurface();
      boost::optional<IdfObject> _toSurface = translateAndMapModelObject(toSurface);

      if (_fromSurface && _toSurface) {
        // Push group only if ok
        auto eg = idfObject.pushExtensibleGroup();
        eg.setString(ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::FromSurface, _fromSurface->name().get());
        eg.setString(ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ToSurface, _toSurface->name().get());
        eg.setDouble(ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ViewFactor, viewFactor.viewFactor());
      } else {
        LOG(Error, "Could not translate a viewFactor group for " << modelObject.briefDescription() << ". Continuing with the rest.");
      }
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
