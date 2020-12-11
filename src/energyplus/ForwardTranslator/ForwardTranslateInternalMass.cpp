/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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
    // EnergyPlus does not support internal mass objects referencing zone lists

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
        idfObject.setString(InternalMassFields::ConstructionName, construction->name().get());
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
      for (Space space : spaces) {

        IdfObject idfObject(openstudio::IddObjectType::InternalMass);

        m_idfObjects.push_back(idfObject);

        if (prefixSpaceName) {
          idfObject.setString(InternalMassFields::Name, space.name().get() + " " + modelObject.name().get());
        } else {
          idfObject.setString(InternalMassFields::Name, modelObject.name().get());
        }

        boost::optional<ThermalZone> thermalZone = space.thermalZone();
        if (thermalZone) {
          idfObject.setString(InternalMassFields::ZoneorZoneListName, thermalZone->name().get());
        }

        if (construction) {
          idfObject.setString(InternalMassFields::ConstructionName, construction->name().get());
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
