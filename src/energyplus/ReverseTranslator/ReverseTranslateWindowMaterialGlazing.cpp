/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
