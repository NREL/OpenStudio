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

