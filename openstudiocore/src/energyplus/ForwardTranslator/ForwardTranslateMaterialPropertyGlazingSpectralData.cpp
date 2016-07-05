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

boost::optional<IdfObject> ForwardTranslator::translateMaterialPropertyGlazingSpectralData( MaterialPropertyGlazingSpectralData & modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::MaterialProperty_GlazingSpectralData);

  m_idfObjects.push_back(idfObject);

  idfObject.setString(openstudio::MaterialProperty_GlazingSpectralDataFields::Name, modelObject.name().get());

  idfObject.clearExtensibleGroups();
  for (const auto& spectralDataField : modelObject.spectralDataFields()){
    IdfExtensibleGroup group = idfObject.pushExtensibleGroup();
    OS_ASSERT(group.numFields() == 4);
    group.setDouble(MaterialProperty_GlazingSpectralDataExtensibleFields::Wavelength, spectralDataField.wavelength());
    group.setDouble(MaterialProperty_GlazingSpectralDataExtensibleFields::transmittance, spectralDataField.transmittance());
    group.setDouble(MaterialProperty_GlazingSpectralDataExtensibleFields::frontreflectance, spectralDataField.frontReflectance());
    group.setDouble(MaterialProperty_GlazingSpectralDataExtensibleFields::backreflectance, spectralDataField.backReflectance());
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio