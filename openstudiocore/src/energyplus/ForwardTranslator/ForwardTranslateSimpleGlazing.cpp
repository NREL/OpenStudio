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

#include "../../model/SimpleGlazing.hpp"
#include "../../model/SimpleGlazing_Impl.hpp"

#include <utilities/idd/WindowMaterial_SimpleGlazingSystem_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSimpleGlazing( SimpleGlazing & modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::WindowMaterial_SimpleGlazingSystem );

  m_idfObjects.push_back(idfObject);

  idfObject.setString(WindowMaterial_SimpleGlazingSystemFields::Name, modelObject.name().get());

  idfObject.setDouble(WindowMaterial_SimpleGlazingSystemFields::UFactor, modelObject.uFactor());

  idfObject.setDouble(WindowMaterial_SimpleGlazingSystemFields::SolarHeatGainCoefficient, modelObject.solarHeatGainCoefficient());

  OptionalDouble d = modelObject.visibleTransmittance();
  if(d) {
    idfObject.setDouble(WindowMaterial_SimpleGlazingSystemFields::VisibleTransmittance, *d);
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

