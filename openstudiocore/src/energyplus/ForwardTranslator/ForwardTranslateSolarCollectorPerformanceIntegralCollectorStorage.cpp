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

#include "../../model/SolarCollectorPerformanceIntegralCollectorStorage.hpp"

#include <utilities/idd/SolarCollectorPerformance_IntegralCollectorStorage_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateSolarCollectorPerformanceIntegralCollectorStorage(model::SolarCollectorPerformanceIntegralCollectorStorage & modelObject)
{
  IdfObject idfObject(openstudio::IddObjectType::SolarCollectorPerformance_IntegralCollectorStorage);

  m_idfObjects.push_back(idfObject);

  boost::optional<double> d;
  
  std::string name = modelObject.name().get();
  idfObject.setName(name);

  idfObject.setString(SolarCollectorPerformance_IntegralCollectorStorageFields::ICSCollectorType, modelObject.iCSCollectorType());

  d = modelObject.grossArea();
  if (d){
    idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::GrossArea, *d);
  }

  d = modelObject.collectorWaterVolume();
  if (d){
    idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::CollectorWaterVolume, *d);
  }

  idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::BottomHeatLossConductance, modelObject.bottomHeatLossConductance());

  idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::SideHeatLossConductance, modelObject.sideHeatLossConductance());

  idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::AspectRatio, modelObject.aspectRatio());

  idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::CollectorSideHeight, modelObject.collectorSideHeight());

  idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::ThermalMassofAbsorberPlate, modelObject.thermalMassOfAbsorberPlate());

  idfObject.setInt(SolarCollectorPerformance_IntegralCollectorStorageFields::NumberofCovers, modelObject.numberOfCovers());

  idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::CoverSpacing, modelObject.coverSpacing());

  idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::RefractiveIndexofOuterCover, modelObject.refractiveIndexOfOuterCover());

  idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::ExtinctionCoefficientTimesThicknessofOuterCover, modelObject.extinctionCoefficientTimesThicknessOfOuterCover());

  idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::EmissivityofOuterCover, modelObject.emissivityOfOuterCover());

  idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::RefractiveIndexofInnerCover, modelObject.refractiveIndexOfInnerCover());

  idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::ExtinctionCoefficientTimesThicknessoftheinnerCover, modelObject.extinctionCoefficientTimesThicknessOfTheInnerCover());

  idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::EmissivityofInnerCover, modelObject.emissivityOfInnerCover());

  idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::AbsorptanceofAbsorberPlate, modelObject.absorptanceOfAbsorberPlate());

  idfObject.setDouble(SolarCollectorPerformance_IntegralCollectorStorageFields::EmissivityofAbsorberPlate, modelObject.emissivityOfAbsorberPlate());

  return idfObject;
}

} // energyplus

} // openstudio

