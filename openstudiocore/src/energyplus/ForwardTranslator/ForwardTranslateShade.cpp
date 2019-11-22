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

