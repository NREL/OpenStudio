/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/InternalMass.hpp"
#include "../../model/InternalMass_Impl.hpp"
#include "../../model/InternalMassDefinition.hpp"
#include "../../model/InternalMassDefinition_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/SpaceType.hpp"
#include "../../model/SpaceType_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ConstructionBase.hpp"
#include "../../model/ConstructionBase_Impl.hpp"
#include "../../model/SurfacePropertyConvectionCoefficients.hpp"

#include <utilities/idd/InternalMass_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateInternalMass(model::InternalMass& modelObject) {

    // TODO: handle (m_forwardTranslatorOptions.excludeSpaceTranslation() = false)
    // Note: E+kept a ‘Zone or ZoneList name’ field and added a ‘Space or SpaceList’ field.
    // It should have been a single ‘Zone or ZoneList or Space or SpaceList Name’ like it's done on many other objects
    // If former is specified as a **ZONELIST** name, later is omitted.
    // Currently you do need to write BOTH the Zone Name (as its a required field) and Space Name when you want to bind it to a Space

    // EnergyPlus does not support internal mass objects referencing zone lists (TODO: <-- this is outdated)

    InternalMassDefinition definition = modelObject.internalMassDefinition();

    boost::optional<ConstructionBase> construction = definition.construction();

    bool prefixSpaceName = false;
    std::vector<Space> spaces;
    boost::optional<Space> space = modelObject.space();
    boost::optional<SpaceType> spaceType = modelObject.spaceType();
    if (space) {
      spaces.push_back(*space);
    } else if (spaceType) {
      prefixSpaceName = true;
      spaces = spaceType->spaces();
    }

    // Call the translation of the SurfacePropertyConvectionCoefficients, which has two advantages:
    // * will not translate them if they are orphaned (=not referencing a surface or subsurface), and,
    // * makes the order of these objects in the IDF deterministic
    if (boost::optional<SurfacePropertyConvectionCoefficients> _sCoefs = modelObject.surfacePropertyConvectionCoefficients()) {
      translateAndMapModelObject(_sCoefs.get());
    }

    boost::optional<IdfObject> result;
    if (spaces.empty()) {

      // translate detached internal mass object
      IdfObject idfObject(openstudio::IddObjectType::InternalMass);

      m_idfObjects.push_back(idfObject);

      if (construction) {
        idfObject.setString(InternalMassFields::ConstructionName, construction->nameString());
      }

      double surfaceArea = 0;

      OptionalDouble d = definition.surfaceArea();
      if (d) {
        surfaceArea += *d;
      }

      d = definition.surfaceAreaperSpaceFloorArea();
      if (d) {
        LOG(Error, "Cannot compute space area because InternalMass object does not reference a Space or SpaceType");
      }

      d = definition.surfaceAreaperPerson();
      if (d) {
        LOG(Error, "Cannot compute number of people because InternalMass object does not reference a Space or SpaceType");
      }

      double multiplier = modelObject.multiplier();

      idfObject.setDouble(InternalMassFields::SurfaceArea, multiplier * surfaceArea);

      result = idfObject;
    } else {

      // create InternalMass object for each zone
      for (const Space& space : spaces) {

        IdfObject idfObject(openstudio::IddObjectType::InternalMass);

        m_idfObjects.push_back(idfObject);

        if (prefixSpaceName) {
          idfObject.setString(InternalMassFields::Name, space.nameString() + " " + modelObject.nameString());
        } else {
          idfObject.setString(InternalMassFields::Name, modelObject.nameString());
        }

        // Even if we want to bind to a Space Name, we need to write the Zone Name as it's a required field
        // cf https://github.com/NREL/EnergyPlus/issues/9141
        // As of E+ v24.2, Zone Name is ignored when Space Name is specified
        boost::optional<ThermalZone> thermalZone = space.thermalZone();
        if (thermalZone) {
          idfObject.setString(InternalMassFields::ZoneorZoneListName, thermalZone->nameString());
        }
        if (!m_forwardTranslatorOptions.excludeSpaceTranslation()) {
          idfObject.setString(InternalMassFields::SpaceorSpaceListName, space.nameString());
        }

        if (construction) {
          idfObject.setString(InternalMassFields::ConstructionName, construction->nameString());
        }

        double surfaceArea = 0;

        OptionalDouble d = definition.surfaceArea();
        if (d) {
          surfaceArea += *d;
        }

        d = definition.surfaceAreaperSpaceFloorArea();
        if (d) {
          surfaceArea += (*d) * space.floorArea();
        }

        d = definition.surfaceAreaperPerson();
        if (d) {
          surfaceArea += (*d) * space.numberOfPeople();
        }

        double multiplier = modelObject.multiplier();

        idfObject.setDouble(InternalMassFields::SurfaceArea, multiplier * surfaceArea);

        if (!result) {
          result = idfObject;
        }
      }
    }

    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
