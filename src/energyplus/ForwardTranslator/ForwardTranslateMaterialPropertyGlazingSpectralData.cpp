/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/MaterialPropertyGlazingSpectralData.hpp"
#include "../../model/MaterialPropertyGlazingSpectralData_Impl.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/MaterialProperty_GlazingSpectralData_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateMaterialPropertyGlazingSpectralData(MaterialPropertyGlazingSpectralData& modelObject) {
    IdfObject idfObject(openstudio::IddObjectType::MaterialProperty_GlazingSpectralData);

    m_idfObjects.push_back(idfObject);

    idfObject.setString(openstudio::MaterialProperty_GlazingSpectralDataFields::Name, modelObject.name().get());

    idfObject.clearExtensibleGroups();
    for (const auto& spectralDataField : modelObject.spectralDataFields()) {
      IdfExtensibleGroup group = idfObject.pushExtensibleGroup();
      OS_ASSERT(group.numFields() == 4);
      group.setDouble(MaterialProperty_GlazingSpectralDataExtensibleFields::Wavelength, spectralDataField.wavelength());
      group.setDouble(MaterialProperty_GlazingSpectralDataExtensibleFields::Transmittance, spectralDataField.transmittance());
      group.setDouble(MaterialProperty_GlazingSpectralDataExtensibleFields::FrontReflectance, spectralDataField.frontReflectance());
      group.setDouble(MaterialProperty_GlazingSpectralDataExtensibleFields::BackReflectance, spectralDataField.backReflectance());
    }

    return boost::optional<IdfObject>(idfObject);
  }

}  // namespace energyplus

}  // namespace openstudio
