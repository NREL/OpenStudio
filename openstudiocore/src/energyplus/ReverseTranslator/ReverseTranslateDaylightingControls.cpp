/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

  OptionalModelObject result;
  OptionalThermalZone controlThermalZone;
  OptionalSpace controlSpace;
  OptionalWorkspaceObject controlZone = workspaceObject.getTarget(Daylighting_ControlsFields::ZoneName);
  if (controlZone){
    OptionalModelObject modelObject = translateAndMapWorkspaceObject(*controlZone);
    if (modelObject){
      if (modelObject->optionalCast<Space>()){
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
  for (const auto& group : groups){
    boost::optional<double> fraction = group.getDouble(Daylighting_ControlsExtensibleFields::FractionofZoneControlledbyReferencePoint, true);
    OS_ASSERT(fraction);

    boost::optional<WorkspaceObject> referencePoint = group.cast<WorkspaceExtensibleGroup>().getTarget(Daylighting_ControlsExtensibleFields::DaylightingReferencePointName);
    if (!referencePoint){
      if (*fraction > 0.0){
        LOG(Warn, "Daylighting:Controls '" << workspaceObject.nameString() << "' missing reference point " << idx << ", setting fraction of zone controlled to 0.");
        fraction = 0;
      }
    }

    sum += *fraction;
    fractions.push_back(*fraction);
    indices.push_back(idx);
    idx += 1;
  }
  // sort indices by fraction in descending order
  sort(indices.begin(), indices.end(), [&fractions](const size_t& i1, const size_t& i2) {return fractions[i1] > fractions[i2];});

  if (sum > 1.0){
    LOG(Warn, "Daylighting:Controls '" << workspaceObject.nameString() << "' specifies fraction controlled greater than one.")
  }

  // loop over the groups in order by indices
  for (const auto& index : indices){
    const auto& group = groups[index];

    bool isPrimary = false;
    bool isSecondary = false;

    if (controlThermalZone){
      if (!controlThermalZone->primaryDaylightingControl()){
        isPrimary = true;
      } else if (!controlThermalZone->secondaryDaylightingControl()){
        isSecondary = true;
      }
    }

    boost::optional<WorkspaceObject> referencePoint = group.cast<WorkspaceExtensibleGroup>().getTarget(Daylighting_ControlsExtensibleFields::DaylightingReferencePointName);
    if (!referencePoint){
      LOG(Warn, "Daylighting:Controls '" << workspaceObject.nameString() << "' missing reference point " << idx << ".");
      continue;
    }

    OptionalThermalZone refThermalZone;
    OptionalSpace refSpace;
    OptionalWorkspaceObject refZone = referencePoint->getTarget(Daylighting_ReferencePointFields::ZoneName);
    if (refZone){
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*refZone);
      if (modelObject){
        if (modelObject->optionalCast<Space>()){
          refSpace = modelObject->cast<Space>();
          refThermalZone = refSpace->thermalZone();
        }
      }
    }

    if (!refSpace){
      LOG(Warn, "Daylighting:ReferencePoint '" << referencePoint->nameString() << "' missing zone, will not be translated.");
      continue;
    }

    DaylightingControl daylightingControl(m_model);
    if (!result){
      result = daylightingControl;
    }

    daylightingControl.setName(referencePoint->nameString());

    daylightingControl.setSpace(*refSpace);

    d = referencePoint->getDouble(Daylighting_ReferencePointFields::XCoordinateofReferencePoint);
    if (d){
      daylightingControl.setPositionXCoordinate(*d);
    }

    d = referencePoint->getDouble(Daylighting_ReferencePointFields::YCoordinateofReferencePoint);
    if (d){
      daylightingControl.setPositionYCoordinate(*d);
    }

    d = referencePoint->getDouble(Daylighting_ReferencePointFields::ZCoordinateofReferencePoint);
    if (d){
      daylightingControl.setPositionZCoordinate(*d);
    }

    d = group.getDouble(Daylighting_ControlsExtensibleFields::IlluminanceSetpointatReferencePoint);
    if (d){
      daylightingControl.setIlluminanceSetpoint(*d);
    }

    OptionalString s = workspaceObject.getString(Daylighting_ControlsFields::LightingControlType, true);
    if (s) {
      if (istringEqual(*s, "Continuous")){
        daylightingControl.setLightingControlType("Continuous");
      } else if (istringEqual(*s, "Stepped")){
        daylightingControl.setLightingControlType("Stepped");
      } else if (istringEqual(*s, "ContinuousOff")){
        daylightingControl.setLightingControlType("Continuous/Off");
      }
    }

    d = workspaceObject.getDouble(Daylighting_ControlsFields::GlareCalculationAzimuthAngleofViewDirectionClockwisefromZoneyAxis);
    if (d){
      daylightingControl.setThetaRotationAroundYAxis(-degToRad(*d));
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

    OptionalInt i = workspaceObject.getInt(Daylighting_ControlsFields::NumberofSteppedControlSteps);
    if (i){
      daylightingControl.setNumberofSteppedControlSteps(*i);
    }

    d = workspaceObject.getDouble(Daylighting_ControlsFields::ProbabilityLightingwillbeResetWhenNeededinManualSteppedControl);
    if (d){
      daylightingControl.setProbabilityLightingwillbeResetWhenNeededinManualSteppedControl(*d);
    }

    if (isPrimary){
      d = group.getDouble(Daylighting_ControlsExtensibleFields::FractionofZoneControlledbyReferencePoint, true);
      if (d && controlThermalZone){
        controlThermalZone->setPrimaryDaylightingControl(daylightingControl);
        controlThermalZone->setFractionofZoneControlledbyPrimaryDaylightingControl(*d);
      }
    } else if (isSecondary){
      d = group.getDouble(Daylighting_ControlsExtensibleFields::FractionofZoneControlledbyReferencePoint, true);
      if (d && controlThermalZone){
        controlThermalZone->setSecondaryDaylightingControl(daylightingControl);
        controlThermalZone->setFractionofZoneControlledbyPrimaryDaylightingControl(*d);
      }
    } else{
      d = group.getDouble(Daylighting_ControlsExtensibleFields::FractionofZoneControlledbyReferencePoint, true);
      if (d && d.get() > 0){
        LOG(Warn, "Daylighting:Controls '" << workspaceObject.nameString() << "' reference point " << index << ", fraction of zone controlled " << *d << " not translated.");
      }
    }
  }

  return result;
}

} // energyplus

} // openstudio

