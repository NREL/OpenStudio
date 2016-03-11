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

#include "../../model/GasMixture.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/WindowMaterial_GasMixture_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateGasMixture( GasMixture & modelObject )
{
  IdfObject idfObject( openstudio::IddObjectType::WindowMaterial_GasMixture);

  m_idfObjects.push_back(idfObject);

  idfObject.setString(WindowMaterial_GasMixtureFields::Name, modelObject.name().get());

  idfObject.setDouble(WindowMaterial_GasMixtureFields::Thickness, modelObject.thickness());

  idfObject.setInt(WindowMaterial_GasMixtureFields::NumberofGasesinMixture, modelObject.numGases());

  int fieldIndex = WindowMaterial_GasMixtureFields::Gas1Type;
  for (unsigned gasIndex = 0; gasIndex < modelObject.numGases(); ++gasIndex){

    idfObject.setString(fieldIndex++, modelObject.getGasType(gasIndex));

    idfObject.setDouble(fieldIndex++, modelObject.getGasFraction(gasIndex));
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

