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

#include "../../model/StandardOpaqueMaterial.hpp"
#include "../../model/StandardOpaqueMaterial_Impl.hpp"

#include <utilities/idd/Material_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateStandardOpaqueMaterial( StandardOpaqueMaterial & modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::Material );

  m_idfObjects.push_back(idfObject);

  idfObject.setString(openstudio::MaterialFields::Name, modelObject.name().get());

  idfObject.setString(openstudio::MaterialFields::Roughness, modelObject.roughness());

  idfObject.setDouble(openstudio::MaterialFields::Thickness, modelObject.thickness());

  idfObject.setDouble(openstudio::MaterialFields::Conductivity, modelObject.thermalConductivity());

  idfObject.setDouble( openstudio::MaterialFields::Density, modelObject.density());

  idfObject.setDouble(openstudio::MaterialFields::SpecificHeat, modelObject.specificHeat());

  OptionalDouble d = modelObject.thermalAbsorptance();
  if(d) {
    idfObject.setDouble(openstudio::MaterialFields::ThermalAbsorptance, *d);
  }

  d = modelObject.solarAbsorptance();
  if(d) {
    idfObject.setDouble(openstudio::MaterialFields::SolarAbsorptance, *d);
  }

  d = modelObject.visibleAbsorptance();
  if(d) {
    idfObject.setDouble(openstudio::MaterialFields::VisibleAbsorptance, *d);
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

