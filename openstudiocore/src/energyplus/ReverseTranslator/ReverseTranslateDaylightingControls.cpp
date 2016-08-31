/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/DaylightingControl.hpp"
#include "../../model/DaylightingControl_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/ThermalZone_Impl.hpp"

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

OptionalModelObject ReverseTranslator::translateDaylightingControls( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::Daylighting_Controls ){
    LOG(Error, "WorkspaceObject is not IddObjectType: Daylighting:Controls");
    return boost::none;
  }

  std::vector<IdfExtensibleGroup> groups = workspaceObject.extensibleGroups();
  if (groups.empty()){
    LOG(Warn, "Daylighting:Controls '" << workspaceObject.nameString() << "' has no reference points, will not be translated.");
    return boost::none;
  }

  OptionalDouble d;
  double sum = 0;
  std::vector<double> fractions;
  std::vector<std::size_t> indices;
  std::size_t idx = 0;
  for (const auto& group : groups){
    boost::optional<double> fraction = workspaceObject.getDouble(Daylighting_ControlsExtensibleFields::FractionofZoneControlledbyReferencePoint, true);
    OS_ASSERT(fraction);

    sum += *fraction;
    fractions.push_back(*fraction);
    indices.push_back(idx);
    ++idx;
  }
  // sort indices by fraction in descending order
  sort(indices.begin(), indices.end(), [&fractions](size_t i1, size_t i2) {return fractions[i1] > fractions[i2];});

  if (sum > 1.0){
    LOG(Warn, "Daylighting:Controls '" << workspaceObject.nameString() << "' specifies fraction controlled greater than one.")
  }

  DaylightingControl daylightingControl(m_model);

  OptionalThermalZone thermalZone;
  OptionalSpace space;
  OptionalWorkspaceObject target = workspaceObject.getTarget(Daylighting_ControlsFields::ZoneName);
  if (target){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
    if (modelObject){
      if (modelObject->optionalCast<Space>()){
        space = modelObject->cast<Space>();
        thermalZone = space->thermalZone();
      }
    }
  }

  if (space){
    daylightingControl.setSpace(*space);
  }

  if (thermalZone){
    thermalZone->setPrimaryDaylightingControl(daylightingControl);
  }

  // primary group
  boost::optional<WorkspaceObject> primaryReferencePoint = groups[indices[0]].cast<WorkspaceExtensibleGroup>().getTarget(Daylighting_ControlsExtensibleFields::DaylightingReferencePointName);
  if (primaryReferencePoint){
    d = primaryReferencePoint->getDouble(Daylighting_ReferencePointFields::XCoordinateofReferencePoint);
    if (d){
      daylightingControl.setPositionXCoordinate(*d);
    }

    d = primaryReferencePoint->getDouble(Daylighting_ReferencePointFields::YCoordinateofReferencePoint);
    if (d){
      daylightingControl.setPositionYCoordinate(*d);
    }

    d = primaryReferencePoint->getDouble(Daylighting_ReferencePointFields::ZCoordinateofReferencePoint);
    if (d){
      daylightingControl.setPositionZCoordinate(*d);
    }
  }

  d = groups[indices[0]].getDouble(Daylighting_ControlsExtensibleFields::FractionofZoneControlledbyReferencePoint);
  if (d && thermalZone){
    thermalZone->setFractionofZoneControlledbyPrimaryDaylightingControl(*d);
  }

  d = groups[indices[0]].getDouble(Daylighting_ControlsExtensibleFields::IlluminanceSetpointatReferencePoint);
  if (d){
    daylightingControl.setIlluminanceSetpoint(*d);
  }

  OptionalInt i = workspaceObject.getInt(Daylighting_ControlsFields::LightingControlType);
  if (i){
    switch (*i){
      case 1:
        daylightingControl.setLightingControlType("Continuous");
        break;
      case 2:
        daylightingControl.setLightingControlType("Stepped");
        break;
      case 3:
        daylightingControl.setLightingControlType("Continuous/Off");
        break;
      default:
        ;
    }
  }

  d = workspaceObject.getDouble(Daylighting_ControlsFields::GlareCalculationAzimuthAngleofViewDirectionClockwisefromZoneyAxis);
  if (d){
    daylightingControl.setThetaRotationAroundYAxis( -degToRad(*d) );
  }

  d = workspaceObject.getDouble(Daylighting_ControlsFields::MaximumAllowableDiscomfortGlareIndex);
  if (d){
    daylightingControl.setMaximumAllowableDiscomfortGlareIndex(*d);
  }

  d = workspaceObject.getDouble(Daylighting_ControlsFields::MinimumInputPowerFractionforContinuousorContinuousOffDimmingControl);
  if (d){
    daylightingControl.setMinimumInputPowerFractionforContinuousDimmingControl(*d);
  }

  d = workspaceObject.getDouble(Daylighting_ControlsFields::MinimumLightOutputFractionforContinuousorContinuousOffDimmingControl);
  if (d){
    daylightingControl.setMinimumLightOutputFractionforContinuousDimmingControl(*d);
  }

  i = workspaceObject.getInt(Daylighting_ControlsFields::NumberofSteppedControlSteps);
  if (i){
    daylightingControl.setNumberofSteppedControlSteps(*i);
  }

  d = workspaceObject.getDouble(Daylighting_ControlsFields::ProbabilityLightingwillbeResetWhenNeededinManualSteppedControl);
  if (d){
    daylightingControl.setProbabilityLightingwillbeResetWhenNeededinManualSteppedControl(*d);
  }

  if (groups.size() == 1){
    return daylightingControl;
  }

  // secondary group
  DaylightingControl daylightingControl2(m_model);

  if (space){
    daylightingControl2.setSpace(*space);
  }

  if (thermalZone){
    thermalZone->setSecondaryDaylightingControl(daylightingControl2);
  }

  boost::optional<WorkspaceObject> secondaryReferencePoint = groups[indices[1]].cast<WorkspaceExtensibleGroup>().getTarget(Daylighting_ControlsExtensibleFields::DaylightingReferencePointName);
  if (secondaryReferencePoint){
    d = secondaryReferencePoint->getDouble(Daylighting_ReferencePointFields::XCoordinateofReferencePoint);
    if (d){
      daylightingControl2.setPositionXCoordinate(*d);
    }

    d = secondaryReferencePoint->getDouble(Daylighting_ReferencePointFields::YCoordinateofReferencePoint);
    if (d){
      daylightingControl2.setPositionYCoordinate(*d);
    }

    d = secondaryReferencePoint->getDouble(Daylighting_ReferencePointFields::ZCoordinateofReferencePoint);
    if (d){
      daylightingControl2.setPositionZCoordinate(*d);
    }
  }

  d = groups[indices[1]].getDouble(Daylighting_ControlsExtensibleFields::FractionofZoneControlledbyReferencePoint);
  if (d && thermalZone){
    thermalZone->setFractionofZoneControlledbySecondaryDaylightingControl(*d);
  }

  d = groups[indices[1]].getDouble(Daylighting_ControlsExtensibleFields::IlluminanceSetpointatReferencePoint);
  if (d){
    daylightingControl2.setIlluminanceSetpoint(*d);
  }

  i = workspaceObject.getInt(Daylighting_ControlsFields::LightingControlType);
  if (i){
    switch (*i){
      case 1:
        daylightingControl2.setLightingControlType("Continuous");
        break;
      case 2:
        daylightingControl2.setLightingControlType("Stepped");
        break;
      case 3:
        daylightingControl2.setLightingControlType("Continuous/Off");
        break;
      default:
        ;
    }
  }

  d = workspaceObject.getDouble(Daylighting_ControlsFields::GlareCalculationAzimuthAngleofViewDirectionClockwisefromZoneyAxis);
  if (d){
    daylightingControl2.setThetaRotationAroundYAxis( -degToRad(*d) );
  }

  d = workspaceObject.getDouble(Daylighting_ControlsFields::MaximumAllowableDiscomfortGlareIndex);
  if (d){
    daylightingControl2.setMaximumAllowableDiscomfortGlareIndex(*d);
  }

  d = workspaceObject.getDouble(Daylighting_ControlsFields::MinimumInputPowerFractionforContinuousorContinuousOffDimmingControl);
  if (d){
    daylightingControl2.setMinimumInputPowerFractionforContinuousDimmingControl(*d);
  }

  d = workspaceObject.getDouble(Daylighting_ControlsFields::MinimumLightOutputFractionforContinuousorContinuousOffDimmingControl);
  if (d){
    daylightingControl2.setMinimumLightOutputFractionforContinuousDimmingControl(*d);
  }

  i = workspaceObject.getInt(Daylighting_ControlsFields::NumberofSteppedControlSteps);
  if (i){
    daylightingControl2.setNumberofSteppedControlSteps(*i);
  }

  d = workspaceObject.getDouble(Daylighting_ControlsFields::ProbabilityLightingwillbeResetWhenNeededinManualSteppedControl);
  if (d){
    daylightingControl2.setProbabilityLightingwillbeResetWhenNeededinManualSteppedControl(*d);
  }

  return daylightingControl;
}

} // energyplus

} // openstudio

