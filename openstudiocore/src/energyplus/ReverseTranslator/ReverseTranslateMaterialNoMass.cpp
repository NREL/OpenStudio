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

#include "../ReverseTranslator.hpp"

#include "../../model/MasslessOpaqueMaterial.hpp"
#include "../../model/MasslessOpaqueMaterial_Impl.hpp"

#include <utilities/idd/Material_NoMass_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateMaterialNoMass( const WorkspaceObject & workspaceObject )
{
  OptionalModelObject result;
  if( workspaceObject.iddObject().type() != IddObjectType::Material_NoMass )
  {
    LOG(Error, "WorkspaceObject is not IddObjectType: Material:NoMass");
    return result;
  }

  openstudio::model::MasslessOpaqueMaterial MOMat( m_model );
  OptionalString optS = workspaceObject.name();
  if(optS) {
    MOMat.setName(*optS);
  }

  optS = workspaceObject.getString(Material_NoMassFields::Roughness);
  if(optS) {
    MOMat.setRoughness(*optS);
  }

  OptionalDouble d = workspaceObject.getDouble( Material_NoMassFields::ThermalResistance );
  if(d) {
    MOMat.setThermalResistance(*d);
  }

  d = workspaceObject.getDouble( Material_NoMassFields::ThermalAbsorptance );
  if(d) {
    MOMat.setThermalAbsorptance(*d);
  }

  d = workspaceObject.getDouble( Material_NoMassFields::SolarAbsorptance );
  if(d) {
    MOMat.setSolarAbsorptance(*d);
  }

  d = workspaceObject.getDouble( Material_NoMassFields::VisibleAbsorptance );
  if(d) {
    MOMat.setVisibleAbsorptance(*d);
  }

  result = MOMat;
  return result;
}

} // energyplus

} // openstudio

