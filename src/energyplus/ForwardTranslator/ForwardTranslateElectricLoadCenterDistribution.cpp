/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "../ForwardTranslator.hpp"

#include "../../model/Model.hpp"
#include "../../model/ElectricLoadCenterDistribution.hpp"
#include "../../model/ElectricLoadCenterDistribution_Impl.hpp"
#include "../../model/Inverter.hpp"
#include "../../model/ElectricalStorage.hpp"
#include "../../model/ElectricLoadCenterStorageConverter.hpp"
#include "../../model/Generator.hpp"
#include "../../model/Schedule.hpp"
#include "../../model/ElectricLoadCenterTransformer.hpp"
#include "../../model/ElectricLoadCenterTransformer_Impl.hpp"

#include "../../utilities/core/Compare.hpp"
#include "../../utilities/idf/IdfExtensibleGroup.hpp"
#include "../../utilities/idd/IddEnums.hpp"

#include <utilities/idd/ElectricLoadCenter_Distribution_FieldEnums.hxx>
#include <utilities/idd/ElectricLoadCenter_Generators_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

using namespace openstudio::model;

using namespace std;

namespace openstudio {

namespace energyplus {

  boost::optional<IdfObject> ForwardTranslator::translateElectricLoadCenterDistribution(model::ElectricLoadCenterDistribution& modelObject) {
    boost::optional<double> optD;
    std::string s;
    boost::optional<std::string> optS;
    boost::optional<Schedule> schedule;

    // Remove Completely empty electric load center distribution objects (e.g. with no generators)
    std::vector<Generator> generators = modelObject.generators();
    boost::optional<Inverter> inverter = modelObject.inverter();
    boost::optional<ElectricLoadCenterTransformer> transformer = modelObject.transformer();
    boost::optional<ElectricalStorage> electricalStorage = modelObject.electricalStorage();

    // This is the rough, high-level check
    if ((generators.empty()) && (!inverter) && (!transformer) && !(electricalStorage)) {
      LOG(Warn, "ElectricLoadCenterDistribution "
                  << modelObject.name().get()
                  << " is not referencing any Generator, Inverter, Transformer, or Electrical Storage, it will not be translated.");
    }

    // I'm going to try to register as few objects as possible starting now, but I can't avoid some of them due to calls to translateAndMap...
    // It would require I change the translateAndMap functions for children to NOT register the objects, which would be
    // 1) An unusual deviation regarding common patterns, and,
    // 2) Subject to problems (some objects can stand on their own with an ELCD...like Transformer)

    IdfObject idfObject(openstudio::IddObjectType::ElectricLoadCenter_Distribution);
    idfObject.setName(modelObject.nameString());

    boost::optional<IdfObject> m_generatorsIdf;

    /// Generator Fields
    if (!generators.empty()) {
      // Don't push it just yet...
      IdfObject generatorsIdf(openstudio::IddObjectType::ElectricLoadCenter_Generators);
      m_generatorsIdf = generatorsIdf;

      generatorsIdf.setName(idfObject.name().get() + " Generators");

      idfObject.setString(ElectricLoadCenter_DistributionFields::GeneratorListName, generatorsIdf.name().get());

      idfObject.setString(ElectricLoadCenter_DistributionFields::GeneratorOperationSchemeType, modelObject.generatorOperationSchemeType());

      if ((optD = modelObject.demandLimitSchemePurchasedElectricDemandLimit())) {
        idfObject.setDouble(ElectricLoadCenter_DistributionFields::GeneratorDemandLimitSchemePurchasedElectricDemandLimit, optD.get());
      }

      if ((schedule = modelObject.trackScheduleSchemeSchedule())) {
        idfObject.setString(ElectricLoadCenter_DistributionFields::GeneratorTrackScheduleNameSchemeScheduleName, schedule->name().get());
      }

      if ((optS = modelObject.trackMeterSchemeMeterName())) {
        idfObject.setString(ElectricLoadCenter_DistributionFields::GeneratorTrackMeterSchemeMeterName, (*optS));
      }

      /// ElectricLoadCenter:Generators
      for (auto& generator : generators) {
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
          // Let it pass...
          LOG(Warn, "Could not translate generator '" << generator.name().get() << "' on ElectricLoadCenter:Distribution '" << idfObject.name().get()
                                                      << "'");
        }
      }
    }

    /// Transformer Object
    if (transformer) {
      boost::optional<IdfObject> transformerIdf = translateAndMapModelObject(transformer.get());
      if (transformerIdf) {
        idfObject.setString(ElectricLoadCenter_DistributionFields::TransformerObjectName, transformerIdf->name().get());
      }
    }

    // NEW: Note all of these checks should have been implemented in ElectricLoadCenter_Impl::validityCheck...

    // Logic based on Electrical Buss Type to translate or not translate inverters, storage
    std::string bussType = modelObject.electricalBussType();

    // Translate the buss Type already
    idfObject.setString(ElectricLoadCenter_DistributionFields::ElectricalBussType, bussType);

    /// Inverter and Buss Type
    bool bussWithInverter =
      (openstudio::istringEqual("DirectCurrentWithInverter", bussType) || openstudio::istringEqual("DirectCurrentWithInverterDCStorage", bussType)
       || openstudio::istringEqual("DirectCurrentWithInverterACStorage", bussType));

    // Case 3: if there is a buss that expects an inverter, but not inverter: this is bad, it'll throw a fatal in E+
    // I'm treating this case first, to avoid translating the inverter altogether...
    if (bussWithInverter && !inverter) {
      LOG(Error, modelObject.briefDescription() << ": Your Electric Buss Type '" << bussType << "' Requires an inverter but you didn't specify one");
      return boost::none;

      // Case 1: There is an inverter and a Buss with inverter: all good
    } else if (inverter && bussWithInverter) {
      boost::optional<IdfObject> inverterIdf = translateAndMapModelObject(*inverter);
      if (inverterIdf) {
        idfObject.setString(ElectricLoadCenter_DistributionFields::InverterName, inverterIdf->name().get());
      }

      // Case 2: if there's an inverter, but the buss is not compatible, we issue a Warning and don't translate the inverter
    } else if (inverter && !bussWithInverter) {
      LOG(Warn, modelObject.briefDescription() << ": Your Electric Buss Type '" << bussType
                                               << "' is not compatible with inverter objects. The inverter object '" << inverter->name().get()
                                               << " will not be translated'");
    }

    // Case 4: there's no inverter and a buss type without inverter: nothing needs to be done

    /// Storage and Buss Type
    bool bussWithStorage =
      (openstudio::istringEqual("AlternatingCurrentWithStorage", bussType) || openstudio::istringEqual("DirectCurrentWithInverterDCStorage", bussType)
       || openstudio::istringEqual("DirectCurrentWithInverterACStorage", bussType));

    // Case 3: if there is a buss that expects Storage, but no Storage: this is bad, it'll throw a fatal in E+
    // Treating it first to avoid translating more things if I can avoid it
    if (bussWithStorage && !electricalStorage) {
      LOG(Error, modelObject.briefDescription() << ": Your Electric Buss Type '" << bussType
                                                << "' Requires an electrical Storage object but you didn't specify one");
      return boost::none;

      // Case 1: There is a Storage object and a Buss with Storage: all good
    } else if (electricalStorage && bussWithStorage) {

      // Translate the storage object itself (I can't really avoid doing it until the end of the checks)
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
      idfObject.setDouble(ElectricLoadCenter_DistributionFields::MaximumStorageStateofChargeFraction,
                          modelObject.maximumStorageStateofChargeFraction());

      // Minimum Storage State of Charge Fraction, defaults
      idfObject.setDouble(ElectricLoadCenter_DistributionFields::MinimumStorageStateofChargeFraction,
                          modelObject.minimumStorageStateofChargeFraction());

      // Translate any storage converter if there is one, store it into a bool so we can check if present when mandatory
      bool has_storage_conv = false;
      // Storage Converter Object Name - This is actually a mandatory field
      boost::optional<ElectricLoadCenterStorageConverter> elcConv = modelObject.storageConverter();
      if (elcConv) {
        // If the buss is compatible, we translate the invert
        boost::optional<IdfObject> storageConverterIdf = translateAndMapModelObject(*elcConv);
        if (storageConverterIdf) {
          has_storage_conv = true;
          idfObject.setString(ElectricLoadCenter_DistributionFields::StorageConverterObjectName, storageConverterIdf->name().get());
        }
      }

      // Design Storage Control Charge Power
      if ((optD = modelObject.designStorageControlChargePower())) {
        idfObject.setDouble(ElectricLoadCenter_DistributionFields::DesignStorageControlChargePower, optD.get());
      } else {
        if (openstudio::istringEqual("FacilityDemandLeveling", storageOperationScheme)
            || openstudio::istringEqual("TrackChargeDischargeSchedules", storageOperationScheme)) {
          LOG(Error, modelObject.briefDescription() << ": You set the Storage Operation Scheme to " << storageOperationScheme
                                                    << " but you didn't specify the required 'Design Storage Control Charge Power'");
          return boost::none;
        }
      }

      // Design Storage Control Discharge Power
      if ((optD = modelObject.designStorageControlDischargePower())) {
        idfObject.setDouble(ElectricLoadCenter_DistributionFields::DesignStorageControlDischargePower, optD.get());
      } else {
        if (openstudio::istringEqual("FacilityDemandLeveling", storageOperationScheme)
            || openstudio::istringEqual("TrackChargeDischargeSchedules", storageOperationScheme)) {
          LOG(Error, modelObject.briefDescription() << ": You set the Storage Operation Scheme to " << storageOperationScheme
                                                    << " but you didn't specify the required 'Design Storage Control Discharge Power'");
          return boost::none;
        }
      }

      // Storage Charge Power Fraction Schedule Name
      if ((schedule = modelObject.storageChargePowerFractionSchedule())) {
        idfObject.setString(ElectricLoadCenter_DistributionFields::StorageChargePowerFractionScheduleName, schedule->name().get());
      } else {
        if (openstudio::istringEqual("TrackChargeDischargeSchedules", storageOperationScheme)) {
          LOG(Error, modelObject.briefDescription() << ": You set the Storage Operation Scheme to " << storageOperationScheme
                                                    << " but you didn't specify the required 'Storage Charge Power Fraction Schedule Name'");
          return boost::none;
        }
      }

      // Discharge Power Fraction Schedule Name
      if ((schedule = modelObject.storageDischargePowerFractionSchedule())) {
        idfObject.setString(ElectricLoadCenter_DistributionFields::StorageDischargePowerFractionScheduleName, schedule->name().get());
      } else {
        if (openstudio::istringEqual("TrackChargeDischargeSchedules", storageOperationScheme)) {
          LOG(Error, modelObject.briefDescription() << ": You set the Storage Operation Scheme to " << storageOperationScheme
                                                    << " but you didn't specify the required 'Storage Discharge Power Fraction Schedule Name'");
          return boost::none;
        }
      }

      /// Further testing based on storageOperationScheme
      if (openstudio::istringEqual("TrackMeterDemandStoreExcessOnSite", storageOperationScheme)) {
        // Storage Control Track Meter Name, required if operation = TrackMeterDemandStoreExcessOnSite or it'll produce a fatal
        if ((optS = modelObject.storageControlTrackMeterName())) {
          idfObject.setString(ElectricLoadCenter_DistributionFields::StorageControlTrackMeterName, (*optS));
        } else {
          LOG(Error, modelObject.briefDescription() << ": You set the Storage Operation Scheme to " << storageOperationScheme
                                                    << " but you didn't specify the required 'Storage Control Track Meter Name'");
          return boost::none;
        }

      } else if (openstudio::istringEqual("TrackChargeDischargeSchedules", storageOperationScheme)) {
        // Storage Converter Object Name - This is actually a mandatory field
        if (!has_storage_conv) {
          LOG(Error, modelObject.briefDescription() << ": You set the Storage Operation Scheme to " << storageOperationScheme
                                                    << " but you didn't specify the required 'Storage Converter Object Name'");
          return boost::none;
        }

      } else if (openstudio::istringEqual("FacilityDemandLeveling", storageOperationScheme)) {
        // Storage Converter Object Name - This is actually a mandatory field
        if (!has_storage_conv) {
          LOG(Error, modelObject.briefDescription() << ": You set the Storage Operation Scheme to " << storageOperationScheme
                                                    << " but you didn't specify the required 'Storage Converter Object Name'");
          return boost::none;
        }

        // Storage Control Utility Demand Target
        if ((optD = modelObject.storageControlUtilityDemandTarget())) {
          idfObject.setDouble(ElectricLoadCenter_DistributionFields::StorageControlUtilityDemandTarget, optD.get());
        } else {
          LOG(Error, modelObject.briefDescription() << ": You set the Storage Operation Scheme to " << storageOperationScheme
                                                    << " but you didn't specify the required 'Storage Control Utility Demand Target'");
          return boost::none;
        }

        // Storage Control Utility Demand Target Fraction Schedule Name
        // This defaults to always 1.0 if omitted
        idfObject.setString(ElectricLoadCenter_DistributionFields::StorageControlUtilityDemandTargetFractionScheduleName,
                            modelObject.storageControlUtilityDemandTargetFractionSchedule().name().get());

      }  // end if (storageOperationScheme)

      // Case 2: if there's a Storage object, but the buss is not compatible, we issue a Warning and don't translate Any of the storage objects
    } else if (electricalStorage && !bussWithStorage) {
      LOG(Warn, modelObject.briefDescription()
                  << ": Your Electric Buss Type '" << bussType
                  << "' is not compatible with storage objects. No storage objects will be translated including the Battery itself:'"
                  << electricalStorage->name().get() << "'");
    }

    // Case 4: there's no Storage object and the buss is not compatible: nothing needs to be done

    // We reach this point, congratulations, you can now register the object
    m_idfObjects.push_back(idfObject);
    // And the potential GeneratorList
    if (m_generatorsIdf) {
      m_idfObjects.push_back(m_generatorsIdf.get());
    }

    return idfObject;
  }

}  // namespace energyplus

}  // namespace openstudio
