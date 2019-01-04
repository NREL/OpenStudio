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

#include "../../model/DaylightRedirectionDevice.hpp"
#include <utilities/idd/OS_WindowMaterial_DaylightRedirectionDevice_FieldEnums.hxx>

#include <utilities/idd/WindowMaterial_Blind_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateDaylightRedirectionDevice(model::DaylightRedirectionDevice & modelObject)
{
  boost::optional<IdfObject> result;

  if (modelObject.daylightRedirectionDeviceType() == "Film"){
    result = IdfObject(openstudio::IddObjectType::WindowMaterial_Blind);

    m_idfObjects.push_back(*result);

    result->setString(WindowMaterial_BlindFields::Name, modelObject.name().get());
    result->setString(WindowMaterial_BlindFields::SlatOrientation, "Horizontal");
    result->setDouble(WindowMaterial_BlindFields::SlatWidth, 0.0508); // 2 in
    result->setDouble(WindowMaterial_BlindFields::SlatSeparation, 0.0254); // 1 in
    result->setDouble(WindowMaterial_BlindFields::SlatThickness, 0.005);
    result->setDouble(WindowMaterial_BlindFields::SlatAngle, 60.0);
    result->setDouble(WindowMaterial_BlindFields::SlatConductivity, 221.0);
    result->setDouble(WindowMaterial_BlindFields::SlatBeamSolarTransmittance, 0.0);
    result->setDouble(WindowMaterial_BlindFields::FrontSideSlatBeamSolarReflectance, 0.9);
    result->setDouble(WindowMaterial_BlindFields::BackSideSlatBeamSolarReflectance, 0.9);
    result->setDouble(WindowMaterial_BlindFields::SlatDiffuseSolarTransmittance, 0.0);
    result->setDouble(WindowMaterial_BlindFields::FrontSideSlatDiffuseSolarReflectance, 0.9);
    result->setDouble(WindowMaterial_BlindFields::BackSideSlatDiffuseSolarReflectance, 0.9);
    result->setDouble(WindowMaterial_BlindFields::SlatBeamVisibleTransmittance, 0.0);
    result->setDouble(WindowMaterial_BlindFields::FrontSideSlatBeamVisibleReflectance, 0.9);
    result->setDouble(WindowMaterial_BlindFields::BackSideSlatBeamVisibleReflectance, 0.9);
    result->setDouble(WindowMaterial_BlindFields::SlatDiffuseVisibleTransmittance, 0.0);
    result->setDouble(WindowMaterial_BlindFields::FrontSideSlatDiffuseVisibleReflectance, 0.9);
    result->setDouble(WindowMaterial_BlindFields::BackSideSlatDiffuseVisibleReflectance, 0.9);
    result->setDouble(WindowMaterial_BlindFields::SlatInfraredHemisphericalTransmittance, 0.0);
    result->setDouble(WindowMaterial_BlindFields::FrontSideSlatInfraredHemisphericalEmissivity, 0.9);
    result->setDouble(WindowMaterial_BlindFields::BackSideSlatInfraredHemisphericalEmissivity, 0.9);
    result->setDouble(WindowMaterial_BlindFields::BlindtoGlassDistance, 0.0254);
    result->setDouble(WindowMaterial_BlindFields::BlindTopOpeningMultiplier, 0.5);
    result->setDouble(WindowMaterial_BlindFields::BlindBottomOpeningMultiplier, 0.0);
    result->setDouble(WindowMaterial_BlindFields::BlindLeftSideOpeningMultiplier, 0.5);
    result->setDouble(WindowMaterial_BlindFields::BlindRightSideOpeningMultiplier, 0.5);
    result->setDouble(WindowMaterial_BlindFields::MinimumSlatAngle, 60);
    result->setDouble(WindowMaterial_BlindFields::MaximumSlatAngle, 60);

  } else if (modelObject.daylightRedirectionDeviceType() == "Louver"){
    result = IdfObject(openstudio::IddObjectType::WindowMaterial_Blind);

    m_idfObjects.push_back(*result);

    result->setString(WindowMaterial_BlindFields::Name, modelObject.name().get());
    result->setString(WindowMaterial_BlindFields::SlatOrientation, "Horizontal");
    result->setDouble(WindowMaterial_BlindFields::SlatWidth, 0.0508); // 2 in
    result->setDouble(WindowMaterial_BlindFields::SlatSeparation, 0.0254); // 1 in
    result->setDouble(WindowMaterial_BlindFields::SlatThickness, 0.005);
    result->setDouble(WindowMaterial_BlindFields::SlatAngle, 60.0);
    result->setDouble(WindowMaterial_BlindFields::SlatConductivity, 221.0);
    result->setDouble(WindowMaterial_BlindFields::SlatBeamSolarTransmittance, 0.0);
    result->setDouble(WindowMaterial_BlindFields::FrontSideSlatBeamSolarReflectance, 0.9);
    result->setDouble(WindowMaterial_BlindFields::BackSideSlatBeamSolarReflectance, 0.9);
    result->setDouble(WindowMaterial_BlindFields::SlatDiffuseSolarTransmittance, 0.0);
    result->setDouble(WindowMaterial_BlindFields::FrontSideSlatDiffuseSolarReflectance, 0.9);
    result->setDouble(WindowMaterial_BlindFields::BackSideSlatDiffuseSolarReflectance, 0.9);
    result->setDouble(WindowMaterial_BlindFields::SlatBeamVisibleTransmittance, 0.0);
    result->setDouble(WindowMaterial_BlindFields::FrontSideSlatBeamVisibleReflectance, 0.9);
    result->setDouble(WindowMaterial_BlindFields::BackSideSlatBeamVisibleReflectance, 0.9);
    result->setDouble(WindowMaterial_BlindFields::SlatDiffuseVisibleTransmittance, 0.0);
    result->setDouble(WindowMaterial_BlindFields::FrontSideSlatDiffuseVisibleReflectance, 0.9);
    result->setDouble(WindowMaterial_BlindFields::BackSideSlatDiffuseVisibleReflectance, 0.9);
    result->setDouble(WindowMaterial_BlindFields::SlatInfraredHemisphericalTransmittance, 0.0);
    result->setDouble(WindowMaterial_BlindFields::FrontSideSlatInfraredHemisphericalEmissivity, 0.9);
    result->setDouble(WindowMaterial_BlindFields::BackSideSlatInfraredHemisphericalEmissivity, 0.9);
    result->setDouble(WindowMaterial_BlindFields::BlindtoGlassDistance, 0.0254);
    result->setDouble(WindowMaterial_BlindFields::BlindTopOpeningMultiplier, 0.5);
    result->setDouble(WindowMaterial_BlindFields::BlindBottomOpeningMultiplier, 0.0);
    result->setDouble(WindowMaterial_BlindFields::BlindLeftSideOpeningMultiplier, 0.5);
    result->setDouble(WindowMaterial_BlindFields::BlindRightSideOpeningMultiplier, 0.5);
    result->setDouble(WindowMaterial_BlindFields::MinimumSlatAngle, 60);
    result->setDouble(WindowMaterial_BlindFields::MaximumSlatAngle, 60);
  }

  return result;
}

} // energyplus

} // openstudio

