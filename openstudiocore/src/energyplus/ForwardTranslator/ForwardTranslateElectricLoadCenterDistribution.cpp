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
#include "../../model/ElectricalStorage.hpp"
//#include "../../model/ElectricLoadCenterStorageConverter.hpp"
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
  boost::optional<double> optD;
  std::string s;
  boost::optional<std::string> optS;
  boost::optional<Schedule> schedule;

  IdfObject idfObject = createRegisterAndNameIdfObject(openstudio::IddObjectType::ElectricLoadCenter_Distribution, modelObject);

  IdfObject generatorsIdf(openstudio::IddObjectType::ElectricLoadCenter_Generators);
  generatorsIdf.setName(idfObject.name().get() + " Generators");
  m_idfObjects.push_back(generatorsIdf);

  idfObject.setString(ElectricLoadCenter_DistributionFields::GeneratorListName, generatorsIdf.name().get());

  idfObject.setString(ElectricLoadCenter_DistributionFields::GeneratorOperationSchemeType, modelObject.generatorOperationSchemeType());

  if (optD = modelObject.demandLimitSchemePurchasedElectricDemandLimit()) {
    idfObject.setDouble(ElectricLoadCenter_DistributionFields::GeneratorDemandLimitSchemePurchasedElectricDemandLimit, optD.get());
  }

  if (schedule = modelObject.trackScheduleSchemeSchedule()) {
    idfObject.setString(ElectricLoadCenter_DistributionFields::GeneratorTrackScheduleNameSchemeScheduleName, schedule->name().get());
  }

  if ( optS = modelObject.trackMeterSchemeMeterName() ) {
    idfObject.setString(ElectricLoadCenter_DistributionFields::GeneratorTrackMeterSchemeMeterName, (*optS) );
  }

  //boost::optional<Transformer> transformer = modelObject.transformer();
  //if (transformer){
  //  boost::optional<IdfObject> transformerIdf = translateAndMapModelObject(transformer);
  //  if (transformerIdf){
  //   idfObject.setString(ElectricLoadCenter_DistributionFields::TransformerObjectName, transformerIdf->name().get());
  //  }
  //}

  for (auto& generator : modelObject.generators()) {
    boost::optional<IdfObject> generatorIdf = translateAndMapModelObject(generator);

    if (generatorIdf) {
      IdfExtensibleGroup generatorGroup = generatorsIdf.pushExtensibleGroup();

      generatorGroup.setString(ElectricLoadCenter_GeneratorsExtensibleFields::GeneratorName, generatorIdf->name().get());

      generatorGroup.setString(ElectricLoadCenter_GeneratorsExtensibleFields::GeneratorObjectType, generator.generatorObjectType());

      optD = generator.ratedElectricPowerOutput();
      if (optD) {
        generatorGroup.setDouble(ElectricLoadCenter_GeneratorsExtensibleFields::GeneratorRatedElectricPowerOutput, *optD);
      }

      schedule = generator.availabilitySchedule();
      if (schedule) {
        boost::optional<IdfObject> scheduleIdf = translateAndMapModelObject(*schedule);
        if (scheduleIdf) {
          generatorGroup.setString(ElectricLoadCenter_GeneratorsExtensibleFields::GeneratorAvailabilityScheduleName, scheduleIdf->name().get());
        }
      }

      optD = generator.ratedThermaltoElectricalPowerRatio();
      if (optD) {
        generatorGroup.setDouble(ElectricLoadCenter_GeneratorsExtensibleFields::GeneratorRatedThermaltoElectricalPowerRatio, *optD);
      }
    } else {
      LOG(Warn, "Could not translate generator '" << generator.name().get() << "' on ElectricLoadCenter:Distribution '" << idfObject.name().get() << "'")
    }
  }

  // NEW

  // Logic based on Electrical Buss Type to translate or not translate inverters, storage
  std::string bussType = modelObject.electricalBussType();
 
  // Translate the buss Type already
  idfObject.setString(ElectricLoadCenter_DistributionFields::ElectricalBussType, bussType);
  
  /// Inverter and Buss Type
  boost::optional<Inverter> inverter = modelObject.inverter();
  bool bussWithInverter = (bussType == "DirectCurrentWithInverter" ||
    bussType == "DirectCurrentWithInverterDCStorage" ||
    bussType == "DirectCurrentWithInverterACStorage"); 

  // Case 1: There is an inverter and a Buss with inverter: all good
  if (inverter && bussWithInverter) {
    boost::optional<IdfObject> inverterIdf = translateAndMapModelObject(*inverter);
    if (inverterIdf) {
      idfObject.setString(ElectricLoadCenter_DistributionFields::InverterName, inverterIdf->name().get());
    }
      
    // Case 2: if there's an inverter, but the buss is not compatible, we issue a Warning and don't translate the inverter
  } else if (inverter && !bussWithInverter) {
      LOG(Warn, modelObject.briefDescription() << ": Your Electric Buss Type '" << bussType
        << "' is not compatible with inverter objects. The inverter object '"
        << inverter->name().get() << " will not be translated'");

    // Case 3: if there is a buss that expects an inverter, but not inverter: this is bad, it'll throw a fatal in E+
  } else if (bussWithInverter && !inverter) {
      LOG(Error, modelObject.briefDescription() << ": Your Electric Buss Type '" << bussType
        << "' Requires an inverter but you didn't specify one");
  }
  // Case 4: there's no inverter and a buss type without inverter: nothing needs to be done
  
  
  /// Storage & Buss Type
  boost::optional<ElectricalStorage> electricalStorage = modelObject.electricalStorage();
  bool bussWithStorage = (bussType == "AlternatingCurrentWithStorage" ||
                          bussType == "DirectCurrentWithInverterDCStorage" ||
                          bussType == "DirectCurrentWithInverterACStorage");

  // Case 1: There is a Storage object and a Buss with Storage: all good
  if (electricalStorage && bussWithStorage) {
    
    // Translate the storage object itself
    boost::optional<IdfObject> storageIdf = translateAndMapModelObject(*electricalStorage);
    if (storageIdf) {
      idfObject.setString(ElectricLoadCenter_DistributionFields::ElectricalStorageObjectName, storageIdf->name().get());
    }

    // Storage Operation Scheme, defaults to TrackFacilityElectricDemandStoreExcessOnSite
    std::string storageOperationScheme = modelObject.storageOperationScheme();

    if (!modelObject.isStorageOperationSchemeDefaulted()) {
      idfObject.setString(ElectricLoadCenter_DistributionFields::StorageOperationScheme, storageOperationScheme);
    }

    // For all storageOperationScheme, we need to translate the Min/Max Storage SOC
    // Maximum Storage State of Charge Fraction, defaults
    idfObject.setDouble(ElectricLoadCenter_DistributionFields::MaximumStorageStateofChargeFraction, modelObject.maximumStorageStateofChargeFraction());

    // Minimum Storage State of Charge Fraction, defaults
    idfObject.setDouble(ElectricLoadCenter_DistributionFields::MinimumStorageStateofChargeFraction, modelObject.minimumStorageStateofChargeFraction());

    /// Further testing based on storageOperationScheme
    if (storageOperationScheme == "TrackMeterDemandStoreExcessOnSite") {
      // Storage Control Track Meter Name, required if operation = TrackMeterDemandStoreExcessOnSite or it'll produce a fatal
      if (optS = modelObject.storageControlTrackMeterName()) {
        idfObject.setString(ElectricLoadCenter_DistributionFields::StorageControlTrackMeterName, (*optS));
      } else {
        LOG(Error, modelObject.briefDescription() << ": You set the Storage Operation Scheme to " << storageOperationScheme
          << " but you didn't specify the required 'Storage Control Track Meter Name'");
      }

    } else if (storageOperationScheme == "TrackChargeDischargeSchedules") {
      // Storage Converter Object Name
      //boost::optional<ElectricLoadCenterStorageConverter> storageConverter = modelObject.storageConverter();
      //if (storageConverter) {
      //  // If the buss is compatible, we translate the invert
      //  boost::optional<IdfObject> storageConverterIdf = translateAndMapModelObject(*storageConverter);
      //  if (storageConverterIdf) {
      //    idfObject.setString(ElectricLoadCenter_DistributionFields
      //      ::StorageConverterObjectName, storageConverterIdf->name().get());
      //  }
      //} else {
      //  LOG(Error, modelObject.briefDescription() << ": You set the Storage Operation Scheme to " << storageOperationScheme
      //    << " but you didn't specify the required 'Storage Converter Object Name'");
      //}

      // Design Storage Control Charge Power
      if (optD = modelObject.designStorageControlChargePower()) {
        idfObject.setDouble(ElectricLoadCenter_DistributionFields::DesignStorageControlChargePower, optD.get());
      } else {
        LOG(Error, modelObject.briefDescription() << ": You set the Storage Operation Scheme to " << storageOperationScheme
          << " but you didn't specify the required 'Design Storage Control Charge Power'");
      }

      // Design Storage Control Discharge Power
      if (optD = modelObject.designStorageControlDischargePower()) {
        idfObject.setDouble(ElectricLoadCenter_DistributionFields::DesignStorageControlDischargePower, optD.get());
      } else {
        LOG(Error, modelObject.briefDescription() << ": You set the Storage Operation Scheme to " << storageOperationScheme
          << " but you didn't specify the required 'Design Storage Control Discharge Power'");
      }

      // Storage Charge Power Fraction Schedule Name
      if (schedule = modelObject.storageChargePowerFractionSchedule()) {
        idfObject.setString(ElectricLoadCenter_DistributionFields::StorageChargePowerFractionScheduleName, schedule->name().get());
      } else {
        LOG(Error, modelObject.briefDescription() << ": You set the Storage Operation Scheme to " << storageOperationScheme
          << " but you didn't specify the required 'Storage Charge Power Fraction Schedule Name'");
      }

      // Discharge Power Fraction Schedule Name
      if (schedule = modelObject.storageDischargePowerFractionSchedule()) {
        idfObject.setString(ElectricLoadCenter_DistributionFields::StorageDischargePowerFractionScheduleName, schedule->name().get());
      } else {
        LOG(Error, modelObject.briefDescription() << ": You set the Storage Operation Scheme to " << storageOperationScheme
          << " but you didn't specify the required 'Storage Discharge Power Fraction Schedule Name'");
      }

    } else if (storageOperationScheme == "FacilityDemandLeveling") {
      // Storage Converter Object Name
      //boost::optional<ElectricLoadCenterStorageConverter> storageConverter = modelObject.storageConverter();
      //if (storageConverter) {
      //  // If the buss is compatible, we translate the invert
      //  boost::optional<IdfObject> storageConverterIdf = translateAndMapModelObject(*storageConverter);
      //  if (storageConverterIdf) {
      //    idfObject.setString(ElectricLoadCenter_DistributionFields
      //      ::StorageConverterObjectName, storageConverterIdf->name().get());
      //  }
      //} else {
      //  LOG(Error, modelObject.briefDescription() << ": You set the Storage Operation Scheme to " << storageOperationScheme
      //    << " but you didn't specify the required 'Storage Converter Object Name'");
      //}

      // Design Storage Control Charge Power
      if (optD = modelObject.designStorageControlChargePower()) {
        idfObject.setDouble(ElectricLoadCenter_DistributionFields::DesignStorageControlChargePower, optD.get());
      } else {
        LOG(Error, modelObject.briefDescription() << ": You set the Storage Operation Scheme to " << storageOperationScheme
          << " but you didn't specify the required 'Design Storage Control Charge Power'");
      }

      // Design Storage Control Discharge Power
      if (optD = modelObject.designStorageControlDischargePower()) {
        idfObject.setDouble(ElectricLoadCenter_DistributionFields::DesignStorageControlDischargePower, optD.get());
      } else {
        LOG(Error, modelObject.briefDescription() << ": You set the Storage Operation Scheme to " << storageOperationScheme
          << " but you didn't specify the required 'Design Storage Control Discharge Power'");
      }

      // Storage Control Utility Demand Target
      if (optD = modelObject.storageControlUtilityDemandTarget()) {
        idfObject.setDouble(ElectricLoadCenter_DistributionFields::StorageControlUtilityDemandTarget, optD.get());
      } else {
        LOG(Error, modelObject.briefDescription() << ": You set the Storage Operation Scheme to " << storageOperationScheme
          << " but you didn't specify the required 'Storage Control Utility Demand Target'");
      }

      // Storage Control Utility Demand Target Fraction Schedule Name
      // This defaults to always 1.0 if omitted
      idfObject.setString(ElectricLoadCenter_DistributionFields::StorageControlUtilityDemandTargetFractionScheduleName,
        modelObject.storageControlUtilityDemandTargetFractionSchedule().name().get());


    } // end if (storageOperationScheme)

    // Case 2: if there's a Storage object, but the buss is not compatible, we issue a Warning and don't translate Any of the storage objects
  } else if (electricalStorage && !bussWithStorage) {
    LOG(Warn, modelObject.briefDescription() << ": Your Electric Buss Type '" << bussType
      << "' is not compatible with storage objects. No storage objects will be translated including the Battery itself:'"
      << electricalStorage->name().get() << "'");

    // Case 3: if there is a buss that expects Storage, but no Storage: this is bad, it'll throw a fatal in E+
  } else if (bussWithStorage && !electricalStorage) {
    LOG(Error, modelObject.briefDescription() << ": Your Electric Buss Type '" << bussType
      << "' Requires an electrical Storage object but you didn't specify one");
  }
  // Case 4: there's no inverter and a buss type without inverter: nothing needs to be done




  return idfObject;
}

} // energyplus

} // openstudio

