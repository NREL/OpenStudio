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
#include "../../model/AirWallMaterial.hpp"
#include "../../model/AirWallMaterial_Impl.hpp"

#include <utilities/idd/Material_FieldEnums.hxx>

#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateAirWallMaterial( AirWallMaterial& modelObject )
{
  IdfObject idfObject(IddObjectType::Material);
  m_idfObjects.push_back(idfObject);

  // convert to a single layer of gypsum
  idfObject.setString(MaterialFields::Name, modelObject.name().get());
  idfObject.setString(MaterialFields::Roughness, "MediumSmooth");
  idfObject.setDouble(MaterialFields::Thickness, 0.01);
  idfObject.setDouble(MaterialFields::Conductivity, 0.6);
  idfObject.setDouble(MaterialFields::Density, 800);
  idfObject.setDouble(MaterialFields::SpecificHeat, 1000);
  idfObject.setDouble(MaterialFields::ThermalAbsorptance, 0.95);
  idfObject.setDouble(MaterialFields::SolarAbsorptance, 0.7);
  idfObject.setDouble(MaterialFields::VisibleAbsorptance, 0.7);

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

