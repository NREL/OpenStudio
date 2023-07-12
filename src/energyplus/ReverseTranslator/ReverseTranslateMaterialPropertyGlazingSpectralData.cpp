/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ReverseTranslator.hpp"

#include "../../model/MaterialPropertyGlazingSpectralData.hpp"
#include "../../model/MaterialPropertyGlazingSpectralData_Impl.hpp"

#include "../../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/MaterialProperty_GlazingSpectralData_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

#include "../../utilities/core/Assert.hpp"

using namespace openstudio::model;

namespace openstudio {
namespace energyplus {

  boost::optional<ModelObject> ReverseTranslator::translateMaterialPropertyGlazingSpectralData(const WorkspaceObject& workspaceObject) {

    openstudio::model::MaterialPropertyGlazingSpectralData glazingSpectralData(m_model);

    // get extensible groups for spectral data fields
    for (const IdfExtensibleGroup& idfGroup : workspaceObject.extensibleGroups()) {
      auto workspaceGroup = idfGroup.cast<WorkspaceExtensibleGroup>();
      OptionalDouble wavelength = workspaceGroup.getDouble(MaterialProperty_GlazingSpectralDataExtensibleFields::Wavelength);
      OptionalDouble transmittance = workspaceGroup.getDouble(MaterialProperty_GlazingSpectralDataExtensibleFields::Transmittance);
      OptionalDouble frontReflectance = workspaceGroup.getDouble(MaterialProperty_GlazingSpectralDataExtensibleFields::FrontReflectance);
      OptionalDouble backReflectance = workspaceGroup.getDouble(MaterialProperty_GlazingSpectralDataExtensibleFields::BackReflectance);

      // add the spectral data field
      glazingSpectralData.addSpectralDataField(*wavelength, *transmittance, *frontReflectance, *backReflectance);
    }

    return glazingSpectralData;
  }

}  // namespace energyplus

}  // namespace openstudio
