/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/ShadingControl.hpp"
#include "../../model/Construction.hpp"
#include "../../model/ShadingMaterial.hpp"
#include "../../model/Schedule.hpp"

#include <utilities/idd/WindowProperty_ShadingControl_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateShadingControl( model::ShadingControl & modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::WindowProperty_ShadingControl);

  m_idfObjects.push_back(idfObject);

  idfObject.setString(WindowProperty_ShadingControlFields::Name, modelObject.name().get());
  
  std::string shadingType = modelObject.shadingType();
  if (istringEqual("InteriorDaylightRedirectionDevice", shadingType)){
    idfObject.setString(WindowProperty_ShadingControlFields::ShadingType, "InteriorBlind");
  } else{
    idfObject.setString(WindowProperty_ShadingControlFields::ShadingType, shadingType);
  }

  boost::optional<Construction> construction = modelObject.construction();
  boost::optional<ShadingMaterial> shadingMaterial = modelObject.shadingMaterial();
  if (construction){
    idfObject.setString(WindowProperty_ShadingControlFields::ConstructionwithShadingName, construction->name().get());
  }else if (shadingMaterial){
    idfObject.setString(WindowProperty_ShadingControlFields::ShadingDeviceMaterialName, shadingMaterial->name().get());
  }
  
  std::string shadingControlType = modelObject.shadingControlType();
  idfObject.setString(WindowProperty_ShadingControlFields::ShadingControlType, shadingControlType);
  
  boost::optional<Schedule> schedule = modelObject.schedule();
  if (schedule){
    idfObject.setString(WindowProperty_ShadingControlFields::ScheduleName, schedule->name().get());
    idfObject.setString(WindowProperty_ShadingControlFields::ShadingControlIsScheduled, "Yes");
  }else{
    idfObject.setString(WindowProperty_ShadingControlFields::ShadingControlIsScheduled, "No");
  }

  boost::optional<double> setpoint = modelObject.setpoint();
  if (istringEqual("OnIfHighSolarOnWindow", shadingControlType)){
    if (!setpoint){
      setpoint = 100; // W/m2
    }
    OS_ASSERT(setpoint);
    idfObject.setDouble(WindowProperty_ShadingControlFields::Setpoint, *setpoint);
  }

  idfObject.setString(WindowProperty_ShadingControlFields::GlareControlIsActive, "No");

  //idfObject.setString(WindowProperty_ShadingControlFields::TypeofSlatAngleControlforBlinds, "FixedSlatAngle");

  //idfObject.setString(WindowProperty_ShadingControlFields::SlatAngleScheduleName, "");

  //idfObject.setDouble(WindowProperty_ShadingControlFields::Setpoint2, 0.0);

  return idfObject;
}

} // energyplus

} // openstudio

