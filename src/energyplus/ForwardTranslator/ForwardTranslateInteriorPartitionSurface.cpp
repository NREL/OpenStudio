/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/InteriorPartitionSurface.hpp"
#include "../../model/InteriorPartitionSurface_Impl.hpp"
#include "../../model/InteriorPartitionSurfaceGroup.hpp"
#include "../../model/Space.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ConstructionBase.hpp"
#include "../../model/ConstructionBase_Impl.hpp"
#include "../../model/DaylightingDeviceShelf.hpp"

#include <utilities/idd/InternalMass_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateInteriorPartitionSurface(model::InteriorPartitionSurface& modelObject) {
    if (!modelObject.converttoInternalMass()) {
      return boost::none;
    }

    // will get translated with light shelf
    if (modelObject.daylightingDeviceShelf()) {
      return boost::none;
    }

    if (modelObject.isAirWall()) {
      return boost::none;
    }

    boost::optional<ConstructionBase> construction = modelObject.construction();
    if (!construction) {
      return boost::none;
    }

    IdfObject idfObject(openstudio::IddObjectType::InternalMass);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(InternalMassFields::Name, modelObject.nameString());

    idfObject.setString(InternalMassFields::ConstructionName, construction->nameString());

    // TODO: when m_forwardTranslatorOptions.excludeSpaceTranslation() is false, should we write the `Space or SpaceList Name` field? (cf InternalMass too)
    double multiplier = 1.0;
    boost::optional<InteriorPartitionSurfaceGroup> group = modelObject.interiorPartitionSurfaceGroup();
    if (group) {
      multiplier = group->multiplier();
      boost::optional<Space> space = group->space();
      if (space) {

        // Even if we want to bind to a Space Name, we need to write the Zone Name as it's a required field
        // cf https://github.com/NREL/EnergyPlus/issues/9141
        boost::optional<ThermalZone> thermalZone = space->thermalZone();
        if (thermalZone) {
          idfObject.setString(InternalMassFields::ZoneorZoneListName, thermalZone->nameString());
        }

        if (!m_forwardTranslatorOptions.excludeSpaceTranslation()) {
          idfObject.setString(InternalMassFields::SpaceorSpaceListName, space->name().get());
        }
      }
    }

    boost::optional<double> surfaceArea = modelObject.surfaceArea();
    if (!surfaceArea) {
      surfaceArea = modelObject.grossArea();
    }

    idfObject.setDouble(InternalMassFields::SurfaceArea, multiplier * (*surfaceArea));

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
