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

boost::optional<IdfObject> ForwardTranslator::translateShadingControl( model::ShadingControl & modelObject )
{
  // after pre-processing in ForwardTranslator, all ShadingControls should only reference subsurfaces in a single zone
  // additionally, the additional property "Shading Control Sequence Number" is set as an integer
  
  std::vector<SubSurface> subSurfaces = modelObject.subSurfaces();

  if (subSurfaces.empty()) {
    LOG(Warn, modelObject.briefDescription() << " does not control any SubSurfaces, will not be translated");
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

  if (zone) {
    boost::optional<DaylightingControl> daylightingControl = zone->primaryDaylightingControl();
    if (daylightingControl) {
      idfObject.setString(WindowShadingControlFields::DaylightingControlObjectName, daylightingControl->nameString());
    } else {
      LOG(Warn, "Cannot find DaylightingControl for " << modelObject.briefDescription());
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

  idfObject.setString(WindowShadingControlFields::MultipleSurfaceControlType, "Group");

  std::string shadingType = modelObject.shadingType();
  if (istringEqual("InteriorDaylightRedirectionDevice", shadingType)){
    idfObject.setString(WindowShadingControlFields::ShadingType, "InteriorBlind");
  } else{
    idfObject.setString(WindowShadingControlFields::ShadingType, shadingType);
  }

  boost::optional<Construction> construction = modelObject.construction();
  boost::optional<ShadingMaterial> shadingMaterial = modelObject.shadingMaterial();
  if (construction){
    idfObject.setString(WindowShadingControlFields::ConstructionwithShadingName, construction->name().get());
  }else if (shadingMaterial){
    idfObject.setString(WindowShadingControlFields::ShadingDeviceMaterialName, shadingMaterial->name().get());
  }

  std::string shadingControlType = modelObject.shadingControlType();
  idfObject.setString(WindowShadingControlFields::ShadingControlType, shadingControlType);

  boost::optional<Schedule> schedule = modelObject.schedule();
  if (schedule){
    idfObject.setString(WindowShadingControlFields::ScheduleName, schedule->name().get());
    idfObject.setString(WindowShadingControlFields::ShadingControlIsScheduled, "Yes");
  }else{
    idfObject.setString(WindowShadingControlFields::ShadingControlIsScheduled, "No");
  }

  boost::optional<double> setpoint = modelObject.setpoint();
  if (istringEqual("OnIfHighSolarOnWindow", shadingControlType)){
    if (!setpoint){
      setpoint = 100; // W/m2
    }
    OS_ASSERT(setpoint);
    idfObject.setDouble(WindowShadingControlFields::Setpoint, *setpoint);
  }

  idfObject.setString(WindowShadingControlFields::GlareControlIsActive, "No");

  //idfObject.setString(WindowShadingControlFields::TypeofSlatAngleControlforBlinds, "FixedSlatAngle");

  //idfObject.setString(WindowShadingControlFields::SlatAngleScheduleName, "");

  //idfObject.setDouble(WindowShadingControlFields::Setpoint2, 0.0);

  idfObject.clearExtensibleGroups();
  for (const SubSurface& subSurface : subSurfaces) {
    IdfExtensibleGroup group = idfObject.pushExtensibleGroup();
    OS_ASSERT(group.numFields() == 1);
    group.setString(0, subSurface.nameString());
  }

  return idfObject;
}

} // energyplus

} // openstudio

