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
#include "../../model/RefractionExtinctionGlazing.hpp"
#include <utilities/idd/OS_WindowMaterial_Glazing_RefractionExtinctionMethod_FieldEnums.hxx>

#include <utilities/idd/WindowMaterial_Glazing_RefractionExtinctionMethod_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateRefractionExtinctionGlazing( RefractionExtinctionGlazing & modelObject )
{
  IdfObject idfObject( openstudio::IddObjectType::WindowMaterial_Glazing_RefractionExtinctionMethod);

  m_idfObjects.push_back(idfObject);

  idfObject.setString(WindowMaterial_Glazing_RefractionExtinctionMethodFields::Name, modelObject.name().get());

  idfObject.setDouble(WindowMaterial_Glazing_RefractionExtinctionMethodFields::Thickness, modelObject.thickness());

  OptionalDouble d = modelObject.getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarIndexofRefraction, false);
  if (d) {
    idfObject.setDouble(WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarIndexofRefraction, *d);
  }else{
    LOG(Error, "Missing required input 'Solar Index of Refraction' for WindowMaterial:Glazing:RefractionExtinctionMethod named '" << modelObject.name().get() << "'");
  }

  d = modelObject.getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarExtinctionCoefficient, false);
  if (d) {
    idfObject.setDouble(WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarExtinctionCoefficient, *d);
  }else{
    LOG(Error, "Missing required input 'Solar Extinction Coefficient' for WindowMaterial:Glazing:RefractionExtinctionMethod named '" << modelObject.name().get() << "'");
  }

  d = modelObject.getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::VisibleIndexofRefraction, false);
  if (d) {
    idfObject.setDouble(WindowMaterial_Glazing_RefractionExtinctionMethodFields::VisibleIndexofRefraction, *d);
  }else{
    LOG(Error, "Missing required input 'Visible Index of Refraction' for WindowMaterial:Glazing:RefractionExtinctionMethod named '" << modelObject.name().get() << "'");
  }

  d = modelObject.getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::VisibleExtinctionCoefficient, false);
  if (d) {
    idfObject.setDouble(WindowMaterial_Glazing_RefractionExtinctionMethodFields::VisibleExtinctionCoefficient, *d);
  }else{
    LOG(Error, "Missing required input 'Visible Extinction Coefficient' for WindowMaterial:Glazing:RefractionExtinctionMethod named '" << modelObject.name().get() << "'");
  }

  d = modelObject.getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredTransmittanceatNormalIncidence, false);
  if (d) {
    idfObject.setDouble(WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredTransmittanceatNormalIncidence, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredHemisphericalEmissivity, false);
  if (d) {
    idfObject.setDouble(WindowMaterial_Glazing_RefractionExtinctionMethodFields::InfraredHemisphericalEmissivity, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::Conductivity, false);
  if (d) {
    idfObject.setDouble(WindowMaterial_Glazing_RefractionExtinctionMethodFields::Conductivity, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::DirtCorrectionFactorforSolarandVisibleTransmittance, false);
  if (d) {
    idfObject.setDouble(WindowMaterial_Glazing_RefractionExtinctionMethodFields::DirtCorrectionFactorforSolarandVisibleTransmittance, *d);
  }

  OptionalString s = modelObject.getString(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarDiffusing, false, true);
  if (s) {
    idfObject.setString(WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarDiffusing, *s);
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

