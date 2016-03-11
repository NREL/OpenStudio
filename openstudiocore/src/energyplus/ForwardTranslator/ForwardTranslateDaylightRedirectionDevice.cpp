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

