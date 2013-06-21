/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <energyplus/ForwardTranslator.hpp>

#include <model/Model.hpp>
#include <model/ConstructionWithInternalSource.hpp>
#include <model/ConstructionWithInternalSource_Impl.hpp>

#include <model/Material.hpp>
#include <model/Material_Impl.hpp>

#include <utilities/idd/Construction_InternalSource_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/IddFactory.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateConstructionWithInternalSource( model::ConstructionWithInternalSource & modelObject )
{
  boost::optional<double> value;
  IdfObject idfObject( openstudio::IddObjectType::Construction_InternalSource );

  m_idfObjects.push_back(idfObject);

  //Name
  idfObject.setString(Construction_InternalSourceFields::Name, modelObject.name().get());
  
  //Source Present After Layer Number
  if (value = modelObject.sourcePresentAfterLayerNumber()){
      idfObject.setDouble(Construction_InternalSourceFields::SourcePresentAfterLayerNumber,value.get());
  }

  //Temperature Calculation Requested After Layer Number
  if (value = modelObject.temperatureCalculationRequestedAfterLayerNumber()){
    idfObject.setDouble(Construction_InternalSourceFields::TemperatureCalculationRequestedAfterLayerNumber,value.get());
  }

  // Dimensions for the CTF Calculation
  if (value = modelObject.dimensionsForTheCTFCalculation()){
    idfObject.setDouble(Construction_InternalSourceFields::DimensionsfortheCTFCalculation,value.get());
  }
  // Tube Spacing
  if (value = modelObject.tubeSpacing()){
  idfObject.setDouble(Construction_InternalSourceFields::TubeSpacing,modelObject.tubeSpacing());
  }else{
    LOG(Error, "Missing required tube Spacing for Construction:InternalSource named '"<<modelObject.name().get()<<"'")
  }
  //Layers
  MaterialVector layers = modelObject.layers();

  unsigned fieldIndex = 1;
  for(unsigned layerIndex = 0; layerIndex < layers.size(); ++layerIndex ) {
    Material material = layers[layerIndex];
    translateAndMapModelObject(material);
    idfObject.setString(fieldIndex++, material.name().get());
  }

  return boost::optional<IdfObject>(idfObject);
}

} // energyplus

} // openstudio

