/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Energy Innovation, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/People.hpp"
#include "../../model/People_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"
#include "../../model/ZoneMRTCalculation.hpp"
#include "../../model/ZoneMRTCalculation_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/ZoneMRTCalculation_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateZoneMRTCalculation(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::ZoneMRTCalculation) {
      LOG(Error, "WorkspaceObject is not IddObjectType: ZoneMRTCalculation");
      return boost::none;
    }

    boost::optional<WorkspaceObject> zoneTarget = workspaceObject.getTarget(ZoneMRTCalculationFields::ZoneName);
    if (!zoneTarget) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " Can't find associated Zone.");
      return boost::none;
    }

    OptionalModelObject zoneModelObject = translateAndMapWorkspaceObject(*zoneTarget);
    if (!zoneModelObject) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " Can't translate associated Zone.");
      return boost::none;
    }

    boost::optional<ThermalZone> thermalZone;
    if (boost::optional<Space> space = zoneModelObject->optionalCast<Space>()) {
      thermalZone = space->thermalZone();
    }

    if (!thermalZone) {
      LOG(Error, "Error importing object: " << workspaceObject.briefDescription() << " Can't find associated ThermalZone.");
      return boost::none;
    }

    openstudio::model::ZoneMRTCalculation zoneMRTCalculation = thermalZone->getZoneMRTCalculation();

    for (const IdfExtensibleGroup& idfGroup : workspaceObject.extensibleGroups()) {
      auto workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();

      boost::optional<WorkspaceObject> peopleTarget = workspaceGroup.getTarget(ZoneMRTCalculationExtensibleFields::PeopleName);
      OptionalDouble mrtWeightingFactor = workspaceGroup.getDouble(ZoneMRTCalculationExtensibleFields::MRTWeightingFactor);

      if (!peopleTarget || !mrtWeightingFactor) {
        LOG(Error, "Could not retrieve an MRTWeightingFactor group for " << workspaceObject.briefDescription() << ". Continuing with the rest.");
        continue;
      }

      OptionalModelObject peopleModelObject = translateAndMapWorkspaceObject(*peopleTarget);
      if (peopleModelObject) {
        if (boost::optional<People> people = peopleModelObject->optionalCast<People>()) {
          try {
            MRTWeightingFactor weightingFactor(people.get(), mrtWeightingFactor.get());
            if (!zoneMRTCalculation.addMRTWeightingFactor(weightingFactor)) {
              LOG(Warn, "Adding MRTWeightingFactor in ThermalZone " << thermalZone->nameString()
                                                                    << " failed for mrtWeightingFactor=" << weightingFactor << ".");
            }
          } catch (...) {
            LOG(Error, "Could not create MRTWeightingFactor in ThermalZone " << thermalZone->nameString() << " for people=("
                                                                             << people->briefDescription()
                                                                             << ") and mrtWeightingFactor=" << mrtWeightingFactor.get() << ".");
          }
        } else {
          LOG(Error, "Could not translate People for an MRTWeightingFactor group in " << workspaceObject.briefDescription() << ".");
        }
      } else {
        LOG(Error, "Could not translate People for an MRTWeightingFactor group in " << workspaceObject.briefDescription() << ".");
      }
    }

    return zoneMRTCalculation;
  }

}  // namespace energyplus

}  // namespace openstudio