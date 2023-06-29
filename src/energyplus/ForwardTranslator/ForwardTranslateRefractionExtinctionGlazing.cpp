/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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
