/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/ShadingControl.hpp"
#include "../../model/Construction.hpp"
#include "../../model/ShadingMaterial.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/Space.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/DaylightingControl.hpp"
#include "../../model/AdditionalProperties.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/WindowShadingControl_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateShadingControl(model::ShadingControl& modelObject) {
    // after pre-processing in ForwardTranslator, all ShadingControls should only reference subsurfaces in a single zone
    // additionally, the additional property "Shading Control Sequence Number" is set as an integer

    std::vector<SubSurface> subSurfaces = modelObject.subSurfaces();

    if (subSurfaces.empty()) {
      LOG(Warn, modelObject.briefDescription() << " does not control any SubSurfaces, it will not be translated");
      return boost::none;
    }
    if (!modelObject.shadingMaterial() && !modelObject.construction()) {
      LOG(Error, modelObject.briefDescription() << " does not have either a Shading Material or a Construction assigned, it will not be translated");
      return boost::none;
    }

    // Read this now, since we'll use it to test if a Warn is needed
    std::string shadingControlType = modelObject.shadingControlType();

    if (modelObject.isControlTypeValueNeedingSetpoint1() && !modelObject.setpoint()) {
      LOG(Error, modelObject.briefDescription() << " is using Shading Control Type '" << shadingControlType
                                                << "' which requires a Setpoint which is missing, it will not be translated");
      return boost::none;
    }

    if (modelObject.isControlTypeValueNeedingSetpoint2() && !modelObject.setpoint2()) {
      LOG(Error, modelObject.briefDescription() << " is using Shading Control Type '" << shadingControlType
                                                << "' which requires a Setpoint2 which is missing, it will not be translated");
      return boost::none;
    }

    if (modelObject.isControlTypeValueRequiringSchedule() && !modelObject.schedule()) {
      LOG(Error, modelObject.briefDescription() << " is using Shading Control Type '" << shadingControlType
                                                << "' which requires a Schedule which is missing, it will not be translated");
      return boost::none;
    }

    IdfObject idfObject(openstudio::IddObjectType::WindowShadingControl);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(WindowShadingControlFields::Name, modelObject.name().get());

    std::string zoneName;
    boost::optional<ThermalZone> zone;
    for (const auto& subSurface : subSurfaces) {
      boost::optional<Space> space = subSurface.space();
      if (space) {
        boost::optional<ThermalZone> thermalZone = space->thermalZone();
        if (thermalZone) {
          if (zoneName.empty()) {
            zoneName = thermalZone->nameString();
            zone = thermalZone;
          } else if (zoneName != thermalZone->nameString()) {
            LOG(Warn, modelObject.briefDescription() << " controls SubSurfaces in multiple zones");
          }
        } else {
          LOG(Error, "Cannot find ThermalZone for " << subSurface.briefDescription() << " referencing " << modelObject.briefDescription());
        }
      } else {
        LOG(Error, "Cannot find Space for " << subSurface.briefDescription() << " referencing " << modelObject.briefDescription());
      }
    }
    idfObject.setString(WindowShadingControlFields::ZoneName, zoneName);

    idfObject.setString(WindowShadingControlFields::ShadingControlType, shadingControlType);

    if (zone) {
      boost::optional<DaylightingControl> daylightingControl = zone->primaryDaylightingControl();
      if (daylightingControl) {
        idfObject.setString(WindowShadingControlFields::DaylightingControlObjectName, daylightingControl->nameString());
      } else {
        // Warn only if a Shading Control Type that relies on DaylightingControl object is used
        // We would probably need to warn if "Glare Control Is Active" was "Yes",
        // but there's no setter in Model API and we default it to "No" later in this file
        if (istringEqual("MeetDaylightIlluminanceSetpoint", shadingControlType) || istringEqual("OnIfHighGlare", shadingControlType)) {
          LOG(Warn, "Cannot find DaylightingControl for " << modelObject.briefDescription() << ", while the Shading Control Type is '"
                                                          << shadingControlType << "'.");
        }
      }
    } else {
      LOG(Error, "Cannot find ThermalZone for " << modelObject.briefDescription());
    }

    boost::optional<int> sequenceNumber = modelObject.additionalProperties().getFeatureAsInteger("Shading Control Sequence Number");
    if (sequenceNumber) {
      idfObject.setInt(WindowShadingControlFields::ShadingControlSequenceNumber, *sequenceNumber);
    } else {
      LOG(Error, modelObject.briefDescription() << " has unknown Shading Control Sequence Number");
    }

    std::string shadingType = modelObject.shadingType();
    if (istringEqual("InteriorDaylightRedirectionDevice", shadingType)) {
      idfObject.setString(WindowShadingControlFields::ShadingType, "InteriorBlind");
    } else {
      idfObject.setString(WindowShadingControlFields::ShadingType, shadingType);
    }

    boost::optional<Construction> construction = modelObject.construction();
    boost::optional<ShadingMaterial> shadingMaterial = modelObject.shadingMaterial();
    if (construction) {
      idfObject.setString(WindowShadingControlFields::ConstructionwithShadingName, construction->name().get());
    } else if (shadingMaterial) {
      idfObject.setString(WindowShadingControlFields::ShadingDeviceMaterialName, shadingMaterial->name().get());
    }

    if (modelObject.isControlTypeValueAllowingSchedule()) {
      boost::optional<Schedule> schedule = modelObject.schedule();
      if (schedule) {
        idfObject.setString(WindowShadingControlFields::ScheduleName, schedule->name().get());
        idfObject.setString(WindowShadingControlFields::ShadingControlIsScheduled, "Yes");
      } else {
        idfObject.setString(WindowShadingControlFields::ShadingControlIsScheduled, "No");
      }
    } else {
      idfObject.setString(WindowShadingControlFields::ShadingControlIsScheduled, "No");
    }

    if (modelObject.isControlTypeValueNeedingSetpoint1()) {
      idfObject.setDouble(WindowShadingControlFields::Setpoint, modelObject.setpoint().get());
    }

    if (modelObject.glareControlIsActive() || istringEqual("OnIfHighGlare", shadingControlType)) {
      idfObject.setString(WindowShadingControlFields::GlareControlIsActive, "Yes");
    } else {
      idfObject.setString(WindowShadingControlFields::GlareControlIsActive, "No");
    }

    boost::optional<Schedule> slatAngleSchedule = modelObject.slatAngleSchedule();
    if (slatAngleSchedule) {
      idfObject.setString(WindowShadingControlFields::SlatAngleScheduleName, slatAngleSchedule->name().get());
      idfObject.setString(WindowShadingControlFields::TypeofSlatAngleControlforBlinds, "ScheduledSlatAngle");
    } else {
      if (!istringEqual("ScheduledSlatAngle", modelObject.typeofSlatAngleControlforBlinds())) {
        idfObject.setString(WindowShadingControlFields::TypeofSlatAngleControlforBlinds, modelObject.typeofSlatAngleControlforBlinds());
      }
    }

    if (modelObject.isControlTypeValueNeedingSetpoint2()) {
      idfObject.setDouble(WindowShadingControlFields::Setpoint2, modelObject.setpoint2().get());
    }

    idfObject.setString(WindowShadingControlFields::MultipleSurfaceControlType, modelObject.multipleSurfaceControlType());

    idfObject.clearExtensibleGroups();
    for (const SubSurface& subSurface : subSurfaces) {
      IdfExtensibleGroup group = idfObject.pushExtensibleGroup();
      OS_ASSERT(group.numFields() == 1);
      group.setString(0, subSurface.nameString());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
