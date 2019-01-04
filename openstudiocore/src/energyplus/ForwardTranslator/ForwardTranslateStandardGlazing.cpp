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

