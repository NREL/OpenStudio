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

#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"

#include <utilities/idd/Material_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateMaterial( const WorkspaceObject & workspaceObject )
{
  OptionalModelObject result;
  if( workspaceObject.iddObject().type() != IddObjectType::Material )
  {
    LOG(Error, "WorkspaceObject is not IddObjectType: Material");
    return result;
  }

  openstudio::model::StandardOpaqueMaterial mat( m_model );
  OptionalString optS = workspaceObject.name();
  if(optS) {
    mat.setName(*optS);
  }

  optS = workspaceObject.getString(MaterialFields::Roughness);
  if(optS) {
    mat.setRoughness(*optS);
  }

  OptionalDouble od = workspaceObject.getDouble( MaterialFields::Thickness );
  if(od) {
    mat.setThickness(*od);
  }

  od = workspaceObject.getDouble( MaterialFields::Conductivity );
  if(od) {
    mat.setThermalConductivity(*od);
  }

  od = workspaceObject.getDouble( MaterialFields::Density );
  if(od) {
    mat.setDensity(*od);
  }

  od = workspaceObject.getDouble( MaterialFields::SpecificHeat );
  if(od) {
    mat.setSpecificHeat(*od);
  }

  od = workspaceObject.getDouble( MaterialFields::ThermalAbsorptance );
  if(od) {
    mat.setThermalAbsorptance(od);
  }

  od = workspaceObject.getDouble( MaterialFields::SolarAbsorptance );
  if(od) {
    mat.setSolarAbsorptance(od);
  }

  od = workspaceObject.getDouble( MaterialFields::VisibleAbsorptance );
  if(od) {
    mat.setVisibleAbsorptance(od);
  }

  result = mat;
  return result;
}

} // energyplus

} // openstudio

