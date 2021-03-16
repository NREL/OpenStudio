/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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
#include "../../model/Screen.hpp"
#include <utilities/idd/OS_WindowMaterial_Screen_FieldEnums.hxx>

#include <utilities/idd/WindowMaterial_Screen_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateScreen(Screen& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::WindowMaterial_Screen);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(WindowMaterial_ScreenFields::Name, modelObject.name().get());

    OptionalString s = modelObject.getString(OS_WindowMaterial_ScreenFields::ReflectedBeamTransmittanceAccountingMethod, true, true);
    if (s) {
      idfObject.setString(WindowMaterial_ScreenFields::ReflectedBeamTransmittanceAccountingMethod, *s);
    }

    OptionalDouble d = modelObject.getDouble(OS_WindowMaterial_ScreenFields::DiffuseSolarReflectance, true);
    if (d) {
      idfObject.setDouble(WindowMaterial_ScreenFields::DiffuseSolarReflectance, *d);
    } else {
      LOG(Error, "Missing required input 'Site Diffuse Solar Radiation Rate per Area Reflectance' for WindowMaterial:Screen named '"
                   << modelObject.name().get() << "'");
    }

    d = modelObject.getDouble(OS_WindowMaterial_ScreenFields::DiffuseVisibleReflectance, true);
    if (d) {
      idfObject.setDouble(WindowMaterial_ScreenFields::DiffuseVisibleReflectance, *d);
    } else {
      LOG(Error, "Missing required input 'Diffuse Visible Reflectance' for WindowMaterial:Screen named '" << modelObject.name().get() << "'");
    }

    d = modelObject.getDouble(OS_WindowMaterial_ScreenFields::ThermalHemisphericalEmissivity, true);
    if (d) {
      idfObject.setDouble(WindowMaterial_ScreenFields::ThermalHemisphericalEmissivity, *d);
    }

    d = modelObject.getDouble(OS_WindowMaterial_ScreenFields::Conductivity, true);
    if (d) {
      idfObject.setDouble(WindowMaterial_ScreenFields::Conductivity, *d);
    }

    d = modelObject.getDouble(OS_WindowMaterial_ScreenFields::ScreenMaterialSpacing, true);
    if (d) {
      idfObject.setDouble(WindowMaterial_ScreenFields::ScreenMaterialSpacing, *d);
    } else {
      LOG(Error, "Missing required input 'Screen Material Spacing' for WindowMaterial:Screen named '" << modelObject.name().get() << "'");
    }

    d = modelObject.getDouble(OS_WindowMaterial_ScreenFields::ScreenMaterialDiameter, true);
    if (d) {
      idfObject.setDouble(WindowMaterial_ScreenFields::ScreenMaterialDiameter, *d);
    } else {
      LOG(Error, "Missing required input 'Screen Material Diameter' for WindowMaterial:Screen named '" << modelObject.name().get() << "'");
    }

    d = modelObject.getDouble(OS_WindowMaterial_ScreenFields::ScreentoGlassDistance, true);
    if (d) {
      idfObject.setDouble(WindowMaterial_ScreenFields::ScreentoGlassDistance, *d);
    }

    d = modelObject.getDouble(OS_WindowMaterial_ScreenFields::TopOpeningMultiplier, true);
    if (d) {
      idfObject.setDouble(WindowMaterial_ScreenFields::TopOpeningMultiplier, *d);
    }

    d = modelObject.getDouble(OS_WindowMaterial_ScreenFields::BottomOpeningMultiplier, true);
    if (d) {
      idfObject.setDouble(WindowMaterial_ScreenFields::BottomOpeningMultiplier, *d);
    }

    d = modelObject.getDouble(OS_WindowMaterial_ScreenFields::LeftSideOpeningMultiplier, true);
    if (d) {
      idfObject.setDouble(WindowMaterial_ScreenFields::LeftSideOpeningMultiplier, *d);
    }

    d = modelObject.getDouble(OS_WindowMaterial_ScreenFields::RightSideOpeningMultiplier, true);
    if (d) {
      idfObject.setDouble(WindowMaterial_ScreenFields::RightSideOpeningMultiplier, *d);
    }

    s = modelObject.getString(OS_WindowMaterial_ScreenFields::AngleofResolutionforScreenTransmittanceOutputMap, true, true);
    if (s) {
      idfObject.setString(WindowMaterial_ScreenFields::AngleofResolutionforScreenTransmittanceOutputMap, *s);
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
