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
#include "../../model/ElectricLoadCenterDistribution.hpp"
#include "../../model/ElectricLoadCenterDistribution_Impl.hpp"
#include "../../model/Inverter.hpp"
#include "../../model/Generator.hpp"
#include "../../model/Schedule.hpp"

#include "../../utilities/idf/IdfExtensibleGroup.hpp"

#include <utilities/idd/ElectricLoadCenter_Distribution_FieldEnums.hxx>
#include <utilities/idd/ElectricLoadCenter_Generators_FieldEnums.hxx>
#include "../../utilities/idd/IddEnums.hpp"
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

boost::optional<IdfObject> ForwardTranslator::translateElectricLoadCenterDistribution(model::ElectricLoadCenterDistribution & modelObject)
{
  boost::optional<double> d;
  boost::optional<Schedule> schedule;

  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ElectricLoadCenter_Distribution, modelObject);

  IdfObject generatorsIdf(openstudio::IddObjectType::ElectricLoadCenter_Generators);
  generatorsIdf.setName(idfObject.name().get() + " Generators");
  m_idfObjects.push_back(generatorsIdf);

  idfObject.setString(ElectricLoadCenter_DistributionFields::GeneratorListName, generatorsIdf.name().get());

  idfObject.setString(ElectricLoadCenter_DistributionFields::GeneratorOperationSchemeType, modelObject.generatorOperationSchemeType());

  //idfObject.setDouble(ElectricLoadCenter_DistributionFields::DemandLimitSchemePurchasedElectricDemandLimit, modelObject.demandLimitSchemePurchasedElectricDemandLimit());

  //idfObject.setString(ElectricLoadCenter_DistributionFields::TrackScheduleNameSchemeScheduleName, modelObject.trackScheduleNameSchemeSchedule());

  //idfObject.setString(ElectricLoadCenter_DistributionFields::TrackMeterSchemeMeterName, modelObject.trackMeterSchemeMeter());

  idfObject.setString(ElectricLoadCenter_DistributionFields::ElectricalBussType, modelObject.electricalBussType());

  boost::optional<Inverter> inverter = modelObject.inverter();
  if (inverter){
    boost::optional<IdfObject> inverterIdf = translateAndMapModelObject(*inverter);
    if (inverterIdf){
      idfObject.setString(ElectricLoadCenter_DistributionFields::InverterName, inverterIdf->name().get());
    }
  }

  //boost::optional<Storage> storage = modelObject.storage();
  //if (storage){
  //  boost::optional<IdfObject> storageIdf = translateAndMapModelObject(storage);
  //  if (storageIdf){
  //    idfObject.setString(ElectricLoadCenter_DistributionFields::ElectricalStorageObjectName, storageIdf->name().get());
  //  }
  //}

  //boost::optional<Transformer> transformer = modelObject.transformer();
  //if (transformer){
  //  boost::optional<IdfObject> transformerIdf = translateAndMapModelObject(transformer);
  //  if (transformerIdf){
  //   idfObject.setString(ElectricLoadCenter_DistributionFields::TransformerObjectName, transformerIdf->name().get());
  //  }
  //}

  for (auto& generator : modelObject.generators()){
    boost::optional<IdfObject> generatorIdf = translateAndMapModelObject(generator);

    if (generatorIdf){
      IdfExtensibleGroup generatorGroup = generatorsIdf.pushExtensibleGroup();

      generatorGroup.setString(ElectricLoadCenter_GeneratorsExtensibleFields::GeneratorName, generatorIdf->name().get());
      
      generatorGroup.setString(ElectricLoadCenter_GeneratorsExtensibleFields::GeneratorObjectType, generator.generatorObjectType());
      
      d = generator.ratedElectricPowerOutput();
      if (d){
        generatorGroup.setDouble(ElectricLoadCenter_GeneratorsExtensibleFields::GeneratorRatedElectricPowerOutput, *d);
      }
      
      schedule = generator.availabilitySchedule();
      if (schedule){
        boost::optional<IdfObject> scheduleIdf = translateAndMapModelObject(*schedule);
        if (scheduleIdf){
          generatorGroup.setString(ElectricLoadCenter_GeneratorsExtensibleFields::GeneratorAvailabilityScheduleName, scheduleIdf->name().get());
        }
      }

      d = generator.ratedThermalToElectricalPowerRatio();
      if (d){
        generatorGroup.setDouble(ElectricLoadCenter_GeneratorsExtensibleFields::GeneratorRatedThermaltoElectricalPowerRatio, *d);
      }
    } else{
      LOG(Warn, "Could not translate generator '" << generator.name().get() << "' on ElectricLoadCenter:Distribution '" << idfObject.name().get() << "'")
    }
  }

  return idfObject;
}

} // energyplus

} // openstudio

