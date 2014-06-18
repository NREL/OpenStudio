/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "../ReverseTranslator.hpp"

#include "../../model/StandardGlazing.hpp"
#include "../../model/StandardGlazing_Impl.hpp"

#include <utilities/idd/WindowMaterial_Glazing_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

boost::optional<ModelObject> ReverseTranslator::translateWindowMaterialGlazing( 
    const WorkspaceObject& workspaceObject)
{
  OptionalModelObject result;
  StandardGlazing standardGlazing(m_model);
  OptionalString optS = workspaceObject.name();
  if(optS) {
    standardGlazing.setName(*optS);
  }

  optS = workspaceObject.getString(WindowMaterial_GlazingFields::OpticalDataType);
  if (optS) {
    standardGlazing.setOpticalDataType(*optS);
  }

  optS = workspaceObject.getString(WindowMaterial_GlazingFields::WindowGlassSpectralDataSetName);
  if (optS) {
    standardGlazing.setWindowGlassSpectralDataSetName(*optS);
  }

  OptionalDouble d = workspaceObject.getDouble(WindowMaterial_GlazingFields::Thickness);
  if (d) {
    standardGlazing.setThickness(*d);
  }

  d = workspaceObject.getDouble(WindowMaterial_GlazingFields::SolarTransmittanceatNormalIncidence);
  if (d) {
    standardGlazing.setSolarTransmittance(*d);
  }

  d = workspaceObject.getDouble(WindowMaterial_GlazingFields::FrontSideSolarReflectanceatNormalIncidence);
  if (d) {
    standardGlazing.setFrontSideSolarReflectanceatNormalIncidence(*d);
  }

  d = workspaceObject.getDouble(WindowMaterial_GlazingFields::BackSideSolarReflectanceatNormalIncidence);
  if (d) {
    standardGlazing.setBackSideSolarReflectanceatNormalIncidence(*d);
  }

  d = workspaceObject.getDouble(WindowMaterial_GlazingFields::VisibleTransmittanceatNormalIncidence);
  if (d) {
    standardGlazing.setVisibleTransmittance(*d);
  }

  d = workspaceObject.getDouble(WindowMaterial_GlazingFields::FrontSideVisibleReflectanceatNormalIncidence);
  if (d) {
    standardGlazing.setFrontSideVisibleReflectanceatNormalIncidence(*d);
  }

  d = workspaceObject.getDouble(WindowMaterial_GlazingFields::BackSideVisibleReflectanceatNormalIncidence);
  if (d) {
    standardGlazing.setBackSideVisibleReflectanceatNormalIncidence(*d);
  }

  d = workspaceObject.getDouble(WindowMaterial_GlazingFields::InfraredTransmittanceatNormalIncidence);
  if (d) {
    standardGlazing.setInfraredTransmittance(*d);
  }

  d = workspaceObject.getDouble(WindowMaterial_GlazingFields::FrontSideInfraredHemisphericalEmissivity);
  if (d) {
    standardGlazing.setFrontSideInfraredHemisphericalEmissivity(*d);
  }

  d = workspaceObject.getDouble(WindowMaterial_GlazingFields::BackSideInfraredHemisphericalEmissivity);
  if (d) {
    standardGlazing.setBackSideInfraredHemisphericalEmissivity(*d);
  }

  d = workspaceObject.getDouble(WindowMaterial_GlazingFields::Conductivity);
  if (d) {
    standardGlazing.setThermalConductivity(*d);
  }

  d = workspaceObject.getDouble(WindowMaterial_GlazingFields::DirtCorrectionFactorforSolarandVisibleTransmittance);
  if (d) {
    standardGlazing.setDirtCorrectionFactorforSolarandVisibleTransmittance(*d);
  }  

  optS = workspaceObject.getString(WindowMaterial_GlazingFields::SolarDiffusing);
  if (optS) {
    std::string temp=*optS;
    boost::to_lower(temp);
    if( temp == "no") {
      standardGlazing.setSolarDiffusing(false);
    }
    else {
      standardGlazing.setSolarDiffusing(true);
    }
  }

  result = standardGlazing;
  return result;
}

} // energyplus

} // openstudio

