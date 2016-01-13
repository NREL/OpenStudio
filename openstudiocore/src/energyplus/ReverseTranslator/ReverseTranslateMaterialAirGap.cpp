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

#include "../../model/AirGap.hpp"
#include "../../model/AirGap_Impl.hpp"

#include <utilities/idd/Material_AirGap_FieldEnums.hxx>
#include <utilities/idd/OS_Material_AirGap_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateMaterialAirGap( const WorkspaceObject & workspaceObject )
{
  if( workspaceObject.iddObject().type() != IddObjectType::Material_AirGap )
  {
    LOG(Error, "WorkspaceObject is not IddObjectType: Material_AirGap");
    return boost::none;
  }

  openstudio::model::AirGap airGap(m_model);
  OptionalString s = workspaceObject.name();
  if (s) {
    airGap.setName(*s);
  }

  OptionalDouble d = workspaceObject.getDouble(Material_AirGapFields::ThermalResistance);
  if (d) {
    airGap.setDouble(OS_Material_AirGapFields::ThermalResistance, *d);
  }

  return airGap;
}

} // energyplus

} // openstudio

