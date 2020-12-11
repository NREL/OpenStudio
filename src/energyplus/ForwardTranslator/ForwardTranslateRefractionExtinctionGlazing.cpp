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
#include "../../model/RefractionExtinctionGlazing.hpp"
#include <utilities/idd/OS_WindowMaterial_Glazing_RefractionExtinctionMethod_FieldEnums.hxx>

#include <utilities/idd/WindowMaterial_Glazing_RefractionExtinctionMethod_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateRefractionExtinctionGlazing(RefractionExtinctionGlazing& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::WindowMaterial_Glazing_RefractionExtinctionMethod);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(WindowMaterial_Glazing_RefractionExtinctionMethodFields::Name, modelObject.name().get());

    idfObject.setDouble(WindowMaterial_Glazing_RefractionExtinctionMethodFields::Thickness, modelObject.thickness());

    OptionalDouble d = modelObject.getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarIndexofRefraction, false);
    if (d) {
      idfObject.setDouble(WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarIndexofRefraction, *d);
    } else {
      LOG(Error, "Missing required input 'Solar Index of Refraction' for WindowMaterial:Glazing:RefractionExtinctionMethod named '"
                   << modelObject.name().get() << "'");
    }

    d = modelObject.getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarExtinctionCoefficient, false);
    if (d) {
      idfObject.setDouble(WindowMaterial_Glazing_RefractionExtinctionMethodFields::SolarExtinctionCoefficient, *d);
    } else {
      LOG(Error, "Missing required input 'Solar Extinction Coefficient' for WindowMaterial:Glazing:RefractionExtinctionMethod named '"
                   << modelObject.name().get() << "'");
    }

    d = modelObject.getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::VisibleIndexofRefraction, false);
    if (d) {
      idfObject.setDouble(WindowMaterial_Glazing_RefractionExtinctionMethodFields::VisibleIndexofRefraction, *d);
    } else {
      LOG(Error, "Missing required input 'Visible Index of Refraction' for WindowMaterial:Glazing:RefractionExtinctionMethod named '"
                   << modelObject.name().get() << "'");
    }

    d = modelObject.getDouble(OS_WindowMaterial_Glazing_RefractionExtinctionMethodFields::VisibleExtinctionCoefficient, false);
    if (d) {
      idfObject.setDouble(WindowMaterial_Glazing_RefractionExtinctionMethodFields::VisibleExtinctionCoefficient, *d);
    } else {
      LOG(Error, "Missing required input 'Visible Extinction Coefficient' for WindowMaterial:Glazing:RefractionExtinctionMethod named '"
                   << modelObject.name().get() << "'");
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

}  // namespace energyplus

}  // namespace openstudio
