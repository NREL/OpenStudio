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

#include "../../model/Blind.hpp"
#include <utilities/idd/OS_WindowMaterial_Blind_FieldEnums.hxx>

#include <utilities/idd/WindowMaterial_Blind_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateBlind( model::Blind & modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::WindowMaterial_Blind);

  m_idfObjects.push_back(idfObject);

  idfObject.setString(WindowMaterial_BlindFields::Name, modelObject.name().get());

  OptionalString s = modelObject.getString(OS_WindowMaterial_BlindFields::SlatOrientation, true, true);
  if (s){
    idfObject.setString(WindowMaterial_BlindFields::SlatOrientation, *s);
  }

  OptionalDouble d = modelObject.getDouble(OS_WindowMaterial_BlindFields::SlatWidth, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::SlatWidth, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::SlatSeparation, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::SlatSeparation, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::SlatThickness, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::SlatThickness, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::SlatAngle, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::SlatAngle, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::SlatConductivity, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::SlatConductivity, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::SlatBeamSolarTransmittance, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::SlatBeamSolarTransmittance, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::FrontSideSlatBeamSolarReflectance, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::FrontSideSlatBeamSolarReflectance, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::BackSideSlatBeamSolarReflectance, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::BackSideSlatBeamSolarReflectance, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::SlatDiffuseSolarTransmittance, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::SlatDiffuseSolarTransmittance, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::FrontSideSlatDiffuseSolarReflectance, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::FrontSideSlatDiffuseSolarReflectance, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::BackSideSlatDiffuseSolarReflectance, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::BackSideSlatDiffuseSolarReflectance, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::SlatBeamVisibleTransmittance, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::SlatBeamVisibleTransmittance, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::FrontSideSlatBeamVisibleReflectance, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::FrontSideSlatBeamVisibleReflectance, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::BackSideSlatBeamVisibleReflectance, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::BackSideSlatBeamVisibleReflectance, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::SlatDiffuseVisibleTransmittance, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::SlatDiffuseVisibleTransmittance, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::FrontSideSlatDiffuseVisibleReflectance, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::FrontSideSlatDiffuseVisibleReflectance, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::BackSideSlatDiffuseVisibleReflectance, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::BackSideSlatDiffuseVisibleReflectance, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::SlatInfraredHemisphericalTransmittance, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::SlatInfraredHemisphericalTransmittance, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::FrontSideSlatInfraredHemisphericalEmissivity, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::FrontSideSlatInfraredHemisphericalEmissivity, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::BackSideSlatInfraredHemisphericalEmissivity, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::BackSideSlatInfraredHemisphericalEmissivity, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::BlindtoGlassDistance, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::BlindtoGlassDistance, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::BlindTopOpeningMultiplier, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::BlindTopOpeningMultiplier, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::BlindBottomOpeningMultiplier, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::BlindBottomOpeningMultiplier, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::BlindLeftSideOpeningMultiplier, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::BlindLeftSideOpeningMultiplier, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::BlindRightSideOpeningMultiplier, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::BlindRightSideOpeningMultiplier, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::MinimumSlatAngle, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::MinimumSlatAngle, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_BlindFields::MaximumSlatAngle, true);
  if (d){
    idfObject.setDouble(WindowMaterial_BlindFields::MaximumSlatAngle, *d);
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

