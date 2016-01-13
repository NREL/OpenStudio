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
#include "../../model/StandardGlazing.hpp"
#include <utilities/idd/OS_WindowMaterial_Glazing_FieldEnums.hxx>

#include <utilities/idd/WindowMaterial_Glazing_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateStandardGlazing( StandardGlazing & modelObject )
{
  IdfObject idfObject( openstudio::IddObjectType::WindowMaterial_Glazing);

  m_idfObjects.push_back(idfObject);

  idfObject.setString(WindowMaterial_GlazingFields::Name, modelObject.name().get());

  std::string opticalDataType;
  OptionalString s = modelObject.getString(OS_WindowMaterial_GlazingFields::OpticalDataType, false, true);
  if (s){
    opticalDataType = *s;
    idfObject.setString(WindowMaterial_GlazingFields::OpticalDataType, *s);
  }else{
    LOG(Error, "Missing required input 'Optical Data Type' for WindowMaterial:Glazing named '" << modelObject.name().get() << "'");
  }

  if (istringEqual("Spectral", opticalDataType)){
    s = modelObject.getString(OS_WindowMaterial_GlazingFields::WindowGlassSpectralDataSetName, false, true);
    if (s){
      idfObject.setString(WindowMaterial_GlazingFields::WindowGlassSpectralDataSetName, *s);
    }
  }

  OptionalDouble d = modelObject.getDouble(OS_WindowMaterial_GlazingFields::Thickness, false);
  if (d){
    idfObject.setDouble(WindowMaterial_GlazingFields::Thickness, *d);
  }else{
    LOG(Error, "Missing required input 'Thickness' for WindowMaterial:Glazing named '" << modelObject.name().get() << "'");
  }

  if (istringEqual("SpectralAverage", opticalDataType)){
    d = modelObject.getDouble(OS_WindowMaterial_GlazingFields::SolarTransmittanceatNormalIncidence, false);
    if (d){
      idfObject.setDouble(WindowMaterial_GlazingFields::SolarTransmittanceatNormalIncidence, *d);
    }

    d = modelObject.getDouble(OS_WindowMaterial_GlazingFields::FrontSideSolarReflectanceatNormalIncidence, false);
    if (d){
      idfObject.setDouble(WindowMaterial_GlazingFields::FrontSideSolarReflectanceatNormalIncidence, *d);
    }

    d = modelObject.getDouble(OS_WindowMaterial_GlazingFields::BackSideSolarReflectanceatNormalIncidence, false);
    if (d){
      idfObject.setDouble(WindowMaterial_GlazingFields::BackSideSolarReflectanceatNormalIncidence, *d);
    }

    d = modelObject.getDouble(OS_WindowMaterial_GlazingFields::VisibleTransmittanceatNormalIncidence, false);
    if (d){
      idfObject.setDouble(WindowMaterial_GlazingFields::VisibleTransmittanceatNormalIncidence, *d);
    }

    d = modelObject.getDouble(OS_WindowMaterial_GlazingFields::FrontSideVisibleReflectanceatNormalIncidence, false);
    if (d){
      idfObject.setDouble(WindowMaterial_GlazingFields::FrontSideVisibleReflectanceatNormalIncidence, *d);
    }

    d = modelObject.getDouble(OS_WindowMaterial_GlazingFields::BackSideVisibleReflectanceatNormalIncidence, false);
    if (d){
      idfObject.setDouble(WindowMaterial_GlazingFields::BackSideVisibleReflectanceatNormalIncidence, *d);
    }

  }

  d = modelObject.getDouble(OS_WindowMaterial_GlazingFields::InfraredTransmittanceatNormalIncidence, false);
  if (d){
    idfObject.setDouble(WindowMaterial_GlazingFields::InfraredTransmittanceatNormalIncidence, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_GlazingFields::FrontSideInfraredHemisphericalEmissivity, false);
  if (d){
    idfObject.setDouble(WindowMaterial_GlazingFields::FrontSideInfraredHemisphericalEmissivity, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_GlazingFields::BackSideInfraredHemisphericalEmissivity, false);
  if (d){
    idfObject.setDouble(WindowMaterial_GlazingFields::BackSideInfraredHemisphericalEmissivity, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_GlazingFields::Conductivity, false);
  if (d){
    idfObject.setDouble(WindowMaterial_GlazingFields::Conductivity, *d);
  }

  d = modelObject.getDouble(OS_WindowMaterial_GlazingFields::DirtCorrectionFactorforSolarandVisibleTransmittance, false);
  if (d){
    idfObject.setDouble(WindowMaterial_GlazingFields::DirtCorrectionFactorforSolarandVisibleTransmittance, *d);
  }

  s = modelObject.getString(OS_WindowMaterial_GlazingFields::SolarDiffusing, false, true);
  if (s){
    idfObject.setString(WindowMaterial_GlazingFields::SolarDiffusing, *s);
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

