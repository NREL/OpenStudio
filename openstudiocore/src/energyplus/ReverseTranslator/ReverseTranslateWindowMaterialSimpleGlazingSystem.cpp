/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "../../model/SimpleGlazing.hpp"
#include "../../model/SimpleGlazing_Impl.hpp"

#include <utilities/idd/WindowMaterial_SimpleGlazingSystem_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

OptionalModelObject ReverseTranslator::translateWindowMaterialSimpleGlazingSystem( const WorkspaceObject & workspaceObject )
{
  OptionalModelObject result;
  SimpleGlazing simpleGlazing( m_model );
  OptionalString optS = workspaceObject.name();
  if(optS)
  {
    simpleGlazing.setName(*optS);
  }

  OptionalDouble d = workspaceObject.getDouble(WindowMaterial_SimpleGlazingSystemFields::UFactor);
  if(d) {
    simpleGlazing.setUFactor(*d);
  }

  d = workspaceObject.getDouble(WindowMaterial_SimpleGlazingSystemFields::SolarHeatGainCoefficient);
  if(d) {
    simpleGlazing.setSolarHeatGainCoefficient(*d);
  }

  d = workspaceObject.getDouble(WindowMaterial_SimpleGlazingSystemFields::VisibleTransmittance);
  if(d) {
    simpleGlazing.setVisibleTransmittance(*d);
  }

  result = simpleGlazing;
  return result;
}

} // energyplus

} // openstudio

