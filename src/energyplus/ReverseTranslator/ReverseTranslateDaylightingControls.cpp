/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/DaylightingControl.hpp"
#include "../../model/DaylightingControl_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

#include "../../utilities/core/Compare.hpp"
#include "../../utilities/geometry/Geometry.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/Daylighting_Controls_FieldEnums.hxx>
#include <utilities/idd/Daylighting_ReferencePoint_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  OptionalModelObject ReverseTranslator::translateDaylightingControls(const WorkspaceObject& workspaceObject) {
    if (workspaceObject.iddObject().type() != IddObjectType::Daylighting_Controls) {
      LOG(Error, "WorkspaceObject is not IddObjectType: Daylighting:Controls");
      return boost::none;
    }

    std::vector<IdfExtensibleGroup> groups = workspaceObject.extensibleGroups();
    if (groups.empty()) {
      LOG(Warn, "Daylighting:Controls '" << workspaceObject.nameString() << "' has no reference points, will not be translated.");
      return boost::none;
    }

    OptionalModelObject result;
    OptionalThermalZone controlThermalZone;
    OptionalSpace controlSpace;
    OptionalWorkspaceObject controlZone = workspaceObject.getTarget(Daylighting_ControlsFields::ZoneorSpaceName);
    if (controlZone) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*controlZone);
      if (modelObject) {
        if (modelObject->optionalCast<Space>()) {
          controlSpace = modelObject->cast<Space>();
          controlThermalZone = controlSpace->thermalZone();
        }
      }
    }

    OptionalDouble d;
    double sum = 0;
    std::vector<double> fractions;
    std::vector<std::size_t> indices;
    std::size_t idx = 0;
    for (const auto& group : groups) {
      boost::optional<double> fraction = group.getDouble(Daylighting_ControlsExtensibleFields::FractionofLightsControlledbyReferencePoint, true);
      OS_ASSERT(fraction);

      boost::optional<WorkspaceObject> referencePoint =
        group.cast<WorkspaceExtensibleGroup>().getTarget(Daylighting_ControlsExtensibleFields::DaylightingReferencePointName);
      if (!referencePoint) {
        if (*fraction > 0.0) {
          LOG(Warn, "Daylighting:Controls '" << workspaceObject.nameString() << "' missing reference point " << idx
                                             << ", setting fraction of zone controlled to 0.");
          fraction = 0;
        }
      }

      sum += *fraction;
      fractions.push_back(*fraction);
      indices.push_back(idx);
      idx += 1;
    }
    // sort indices by fraction in descending order
    sort(indices.begin(), indices.end(), [&fractions](const size_t& i1, const size_t& i2) { return fractions[i1] > fractions[i2]; });

    if (sum > 1.0) {
      LOG(Warn, "Daylighting:Controls '" << workspaceObject.nameString() << "' specifies fraction controlled greater than one.")
    }

    // loop over the groups in order by indices
    for (const auto& index : indices) {
      const auto& group = groups[index];

      bool isPrimary = false;
      bool isSecondary = false;

      if (controlThermalZone) {
        if (!controlThermalZone->primaryDaylightingControl()) {
          isPrimary = true;
        } else if (!controlThermalZone->secondaryDaylightingControl()) {
          isSecondary = true;
        }
      }

      boost::optional<WorkspaceObject> referencePoint =
        group.cast<WorkspaceExtensibleGroup>().getTarget(Daylighting_ControlsExtensibleFields::DaylightingReferencePointName);
      if (!referencePoint) {
        LOG(Warn, "Daylighting:Controls '" << workspaceObject.nameString() << "' missing reference point " << idx << ".");
        continue;
      }

      OptionalThermalZone refThermalZone;
      OptionalSpace refSpace;
      OptionalWorkspaceObject refZone = referencePoint->getTarget(Daylighting_ReferencePointFields::ZoneorSpaceName);
      if (refZone) {
        OptionalModelObject modelObject = translateAndMapWorkspaceObject(*refZone);
        if (modelObject) {
          if (modelObject->optionalCast<Space>()) {
            refSpace = modelObject->cast<Space>();
            refThermalZone = refSpace->thermalZone();
          }
        }
      }

      if (!refSpace) {
        LOG(Warn, "Daylighting:ReferencePoint '" << referencePoint->nameString() << "' missing zone, will not be translated.");
        continue;
      }

      DaylightingControl daylightingControl(m_model);
      if (!result) {
        result = daylightingControl;
      }

      daylightingControl.setName(referencePoint->nameString());

      daylightingControl.setSpace(*refSpace);

      d = referencePoint->getDouble(Daylighting_ReferencePointFields::XCoordinateofReferencePoint);
      if (d) {
        daylightingControl.setPositionXCoordinate(*d);
      }

      d = referencePoint->getDouble(Daylighting_ReferencePointFields::YCoordinateofReferencePoint);
      if (d) {
        daylightingControl.setPositionYCoordinate(*d);
      }

      d = referencePoint->getDouble(Daylighting_ReferencePointFields::ZCoordinateofReferencePoint);
      if (d) {
        daylightingControl.setPositionZCoordinate(*d);
      }

      d = group.getDouble(Daylighting_ControlsExtensibleFields::IlluminanceSetpointatReferencePoint);
      if (d) {
        daylightingControl.setIlluminanceSetpoint(*d);
      }

      OptionalString s = workspaceObject.getString(Daylighting_ControlsFields::LightingControlType, true);
      if (s) {
        if (istringEqual(*s, "Continuous")) {
          daylightingControl.setLightingControlType("Continuous");
        } else if (istringEqual(*s, "Stepped")) {
          daylightingControl.setLightingControlType("Stepped");
        } else if (istringEqual(*s, "ContinuousOff")) {
          daylightingControl.setLightingControlType("Continuous/Off");
        }
      }

      d = workspaceObject.getDouble(Daylighting_ControlsFields::GlareCalculationAzimuthAngleofViewDirectionClockwisefromZoneyAxis);
      if (d) {
        daylightingControl.setPhiRotationAroundZAxis(*d);
      }

      d = workspaceObject.getDouble(Daylighting_ControlsFields::MaximumAllowableDiscomfortGlareIndex);
      if (d) {
        daylightingControl.setMaximumAllowableDiscomfortGlareIndex(*d);
      }

      d = workspaceObject.getDouble(Daylighting_ControlsFields::MinimumInputPowerFractionforContinuousorContinuousOffDimmingControl);
      if (d) {
        daylightingControl.setMinimumInputPowerFractionforContinuousDimmingControl(*d);
      }

      d = workspaceObject.getDouble(Daylighting_ControlsFields::MinimumLightOutputFractionforContinuousorContinuousOffDimmingControl);
      if (d) {
        daylightingControl.setMinimumLightOutputFractionforContinuousDimmingControl(*d);
      }

      OptionalInt i = workspaceObject.getInt(Daylighting_ControlsFields::NumberofSteppedControlSteps);
      if (i) {
        daylightingControl.setNumberofSteppedControlSteps(*i);
      }

      d = workspaceObject.getDouble(Daylighting_ControlsFields::ProbabilityLightingwillbeResetWhenNeededinManualSteppedControl);
      if (d) {
        daylightingControl.setProbabilityLightingwillbeResetWhenNeededinManualSteppedControl(*d);
      }

      if (isPrimary) {
        d = group.getDouble(Daylighting_ControlsExtensibleFields::FractionofLightsControlledbyReferencePoint, true);
        if (d && controlThermalZone) {
          controlThermalZone->setPrimaryDaylightingControl(daylightingControl);
          controlThermalZone->setFractionofZoneControlledbyPrimaryDaylightingControl(*d);
        }
      } else if (isSecondary) {
        d = group.getDouble(Daylighting_ControlsExtensibleFields::FractionofLightsControlledbyReferencePoint, true);
        if (d && controlThermalZone) {
          controlThermalZone->setSecondaryDaylightingControl(daylightingControl);
          controlThermalZone->setFractionofZoneControlledbyPrimaryDaylightingControl(*d);
        }
      } else {
        d = group.getDouble(Daylighting_ControlsExtensibleFields::FractionofLightsControlledbyReferencePoint, true);
        if (d && d.get() > 0) {
          LOG(Warn, "Daylighting:Controls '" << workspaceObject.nameString() << "' reference point " << index << ", fraction of zone controlled "
                                             << *d << " not translated.");
        }
      }
    }

    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
