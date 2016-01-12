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
#include "../../model/RoofVegetation.hpp"
#include <utilities/idd/OS_Material_RoofVegetation_FieldEnums.hxx>

#include <utilities/idd/Material_RoofVegetation_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateRoofVegetation( RoofVegetation & modelObject )
{
  IdfObject idfObject(openstudio::IddObjectType::Material_RoofVegetation);

  m_idfObjects.push_back(idfObject);

  idfObject.setString(Material_RoofVegetationFields::Name, modelObject.name().get());

  OptionalDouble d = modelObject.getDouble(OS_Material_RoofVegetationFields::HeightofPlants, false);
  if (d){
    idfObject.setDouble(Material_RoofVegetationFields::HeightofPlants, *d);
  }

  d = modelObject.getDouble(OS_Material_RoofVegetationFields::LeafAreaIndex, false);
  if (d){
    idfObject.setDouble(Material_RoofVegetationFields::LeafAreaIndex, *d);
  }

  d = modelObject.getDouble(OS_Material_RoofVegetationFields::LeafReflectivity, false);
  if (d){
    idfObject.setDouble(Material_RoofVegetationFields::LeafReflectivity, *d);
  }
    
  d = modelObject.getDouble(OS_Material_RoofVegetationFields::LeafEmissivity, false);
  if (d){
    idfObject.setDouble(Material_RoofVegetationFields::LeafEmissivity, *d);
  }

  d = modelObject.getDouble(OS_Material_RoofVegetationFields::MinimumStomatalResistance, false);
  if (d){
    idfObject.setDouble(Material_RoofVegetationFields::MinimumStomatalResistance, *d);
  }

  OptionalString s = modelObject.getString(OS_Material_RoofVegetationFields::SoilLayerName, false, true);
  if (s){
    idfObject.setString(Material_RoofVegetationFields::SoilLayerName, *s);
  }

  s = modelObject.getString(OS_Material_RoofVegetationFields::Roughness, false, true);
  if (s){
    idfObject.setString(Material_RoofVegetationFields::Roughness, *s);
  }

  d = modelObject.getDouble(OS_Material_RoofVegetationFields::Thickness, false);
  if (d){
    idfObject.setDouble(Material_RoofVegetationFields::Thickness, *d);
  }

  d = modelObject.getDouble(OS_Material_RoofVegetationFields::ConductivityofDrySoil, false);
  if (d){
    idfObject.setDouble(Material_RoofVegetationFields::ConductivityofDrySoil, *d);
  }

  d = modelObject.getDouble(OS_Material_RoofVegetationFields::DensityofDrySoil, false);
  if (d){
    idfObject.setDouble(Material_RoofVegetationFields::DensityofDrySoil, *d);
  }

  d = modelObject.getDouble(OS_Material_RoofVegetationFields::SpecificHeatofDrySoil, false);
  if (d){
    idfObject.setDouble(Material_RoofVegetationFields::SpecificHeatofDrySoil, *d);
  }

  d = modelObject.getDouble(OS_Material_RoofVegetationFields::ThermalAbsorptance, false);
  if (d){
    idfObject.setDouble(Material_RoofVegetationFields::ThermalAbsorptance, *d);
  }

  d = modelObject.getDouble(OS_Material_RoofVegetationFields::SolarAbsorptance, false);
  if (d){
    idfObject.setDouble(Material_RoofVegetationFields::SolarAbsorptance, *d);
  }

  d = modelObject.getDouble(OS_Material_RoofVegetationFields::VisibleAbsorptance, false);
  if (d){
    idfObject.setDouble(Material_RoofVegetationFields::VisibleAbsorptance, *d);
  }

  d = modelObject.getDouble(OS_Material_RoofVegetationFields::SaturationVolumetricMoistureContentoftheSoilLayer, false);
  if (d){
    idfObject.setDouble(Material_RoofVegetationFields::SaturationVolumetricMoistureContentoftheSoilLayer, *d);
  }

  d = modelObject.getDouble(OS_Material_RoofVegetationFields::ResidualVolumetricMoistureContentoftheSoilLayer, false);
  if (d){
    idfObject.setDouble(Material_RoofVegetationFields::ResidualVolumetricMoistureContentoftheSoilLayer, *d);
  }

  d = modelObject.getDouble(OS_Material_RoofVegetationFields::InitialVolumetricMoistureContentoftheSoilLayer, false);
  if (d){
    idfObject.setDouble(Material_RoofVegetationFields::InitialVolumetricMoistureContentoftheSoilLayer, *d);
  }

  s = modelObject.getString(OS_Material_RoofVegetationFields::MoistureDiffusionCalculationMethod, false, true);
  if (s){
    idfObject.setString(Material_RoofVegetationFields::MoistureDiffusionCalculationMethod, *s);
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

