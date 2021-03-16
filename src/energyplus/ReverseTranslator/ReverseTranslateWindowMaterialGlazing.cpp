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

#include "../ReverseTranslator.hpp"

#include "../../model/StandardGlazing.hpp"
#include "../../model/StandardGlazing_Impl.hpp"
#include "../../model/MaterialPropertyGlazingSpectralData.hpp"
#include "../../model/MaterialPropertyGlazingSpectralData_Impl.hpp"

#include <utilities/idd/WindowMaterial_Glazing_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateWindowMaterialGlazing(const WorkspaceObject& workspaceObject) {
    OptionalModelObject result;
    StandardGlazing standardGlazing(m_model);
    OptionalString optS = workspaceObject.name();
    if (optS) {
      standardGlazing.setName(*optS);
    }

    optS = workspaceObject.getString(WindowMaterial_GlazingFields::OpticalDataType);
    if (optS) {
      standardGlazing.setOpticalDataType(*optS);
    }

    OptionalWorkspaceObject target = workspaceObject.getTarget(openstudio::WindowMaterial_GlazingFields::WindowGlassSpectralDataSetName);
    if (target) {
      OptionalModelObject modelObject = translateAndMapWorkspaceObject(*target);
      if (modelObject) {
        if (modelObject->optionalCast<MaterialPropertyGlazingSpectralData>()) {
          standardGlazing.setWindowGlassSpectralDataSet(modelObject->cast<MaterialPropertyGlazingSpectralData>());
        }
      }
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
      std::string temp = *optS;
      boost::to_lower(temp);
      if (temp == "no") {
        standardGlazing.setSolarDiffusing(false);
      } else {
        standardGlazing.setSolarDiffusing(true);
      }
    }

    result = standardGlazing;
    return result;
  }

}  // namespace energyplus

}  // namespace openstudio
