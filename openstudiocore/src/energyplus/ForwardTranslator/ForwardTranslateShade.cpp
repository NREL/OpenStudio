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
#include "../../model/Shade.hpp"
#include <utilities/idd/OS_WindowMaterial_Shade_FieldEnums.hxx>

#include <utilities/idd/WindowMaterial_Shade_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateShade( Shade & modelObject )
{
  IdfObject idfObject( openstudio::IddObjectType::WindowMaterial_Shade);

  m_idfObjects.push_back(idfObject);

  idfObject.setString(WindowMaterial_ShadeFields::Name, modelObject.name().get());
  
  OptionalDouble d = modelObject.getDouble(OS_WindowMaterial_ShadeFields::SolarTransmittance, true);
  if (d){
    idfObject.setDouble(WindowMaterial_ShadeFields::SolarTransmittance, *d);
  }else{
    LOG(Error, "Missing required input 'Solar Transmittance' for WindowMaterial:Shade named '" << modelObject.name().get() << "'");
  }
    
  d = modelObject.getDouble(OS_WindowMaterial_ShadeFields::SolarReflectance, true);
  if (d){
    idfObject.setDouble(WindowMaterial_ShadeFields::SolarReflectance, *d);
  }else{
    LOG(Error, "Missing required input 'Solar Reflectance' for WindowMaterial:Shade named '" << modelObject.name().get() << "'");
  }

  d = modelObject.getDouble(OS_WindowMaterial_ShadeFields::VisibleTransmittance, true);
  if (d){
    idfObject.setDouble(WindowMaterial_ShadeFields::VisibleTransmittance, *d);
  }else{
    LOG(Error, "Missing required input 'Visible Transmittance' for WindowMaterial:Shade named '" << modelObject.name().get() << "'");
  }

  d = modelObject.getDouble(OS_WindowMaterial_ShadeFields::VisibleReflectance, true);
  if (d){
    idfObject.setDouble(WindowMaterial_ShadeFields::VisibleReflectance, *d);
  }else{
    LOG(Error, "Missing required input 'Visible Reflectance' for WindowMaterial:Shade named '" << modelObject.name().get() << "'");
  }

  d = modelObject.getDouble(OS_WindowMaterial_ShadeFields::ThermalHemisphericalEmissivity, true);
  if (d){
    idfObject.setDouble(WindowMaterial_ShadeFields::InfraredHemisphericalEmissivity, *d);
  }else{
    LOG(Error, "Missing required input 'Thermal Hemispherical Emissivity' for WindowMaterial:Shade named '" << modelObject.name().get() << "'");
  }

  d = modelObject.getDouble(OS_WindowMaterial_ShadeFields::ThermalTransmittance, true);
  if (d){
    idfObject.setDouble(WindowMaterial_ShadeFields::InfraredTransmittance, *d);
  }else{
    LOG(Error, "Missing required input 'Thermal Transmittance' for WindowMaterial:Shade named '" << modelObject.name().get() << "'");
  }

  d = modelObject.getDouble(OS_WindowMaterial_ShadeFields::Thickness, true);
  if (d){
    idfObject.setDouble(WindowMaterial_ShadeFields::Thickness, *d);
  }else{
    LOG(Error, "Missing required input 'Thickness' for WindowMaterial:Shade named '" << modelObject.name().get() << "'");
  }

  d = modelObject.getDouble(OS_WindowMaterial_ShadeFields::Conductivity, true);
  if (d){
    idfObject.setDouble(WindowMaterial_ShadeFields::Conductivity, *d);
  }else{
    LOG(Error, "Missing required input 'Conductivity' for WindowMaterial:Shade named '" << modelObject.name().get() << "'");
  }

  d = modelObject.getDouble(OS_WindowMaterial_ShadeFields::ShadetoGlassDistance, true);
  if (d){
    idfObject.setDouble(WindowMaterial_ShadeFields::ShadetoGlassDistance, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_ShadeFields::TopOpeningMultiplier, true);
  if (d){
    idfObject.setDouble(WindowMaterial_ShadeFields::TopOpeningMultiplier, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_ShadeFields::BottomOpeningMultiplier, true);
  if (d){
    idfObject.setDouble(WindowMaterial_ShadeFields::BottomOpeningMultiplier, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_ShadeFields::LeftSideOpeningMultiplier, true);
  if (d){
    idfObject.setDouble(WindowMaterial_ShadeFields::LeftSideOpeningMultiplier, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_ShadeFields::RightSideOpeningMultiplier, true);
  if (d){
    idfObject.setDouble(WindowMaterial_ShadeFields::RightSideOpeningMultiplier, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_ShadeFields::AirflowPermeability, true);
  if (d){
    idfObject.setDouble(WindowMaterial_ShadeFields::AirflowPermeability, *d);
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

