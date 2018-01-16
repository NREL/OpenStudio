/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_ELECTRICLOADCENTERDISTRIBUTION_HPP
#define MODEL_ELECTRICLOADCENTERDISTRIBUTION_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

class Schedule;
class Generator;
class Inverter;
class ElectricalStorage;
//class Transformer;
class ElectricLoadCenterStorageConverter;

namespace detail {

  class ElectricLoadCenterDistribution_Impl;

} // detail

/** ElectricLoadCenterDistribution is a ParentObject that wraps the OpenStudio IDD object 'OS:ElectricLoadCenter:Distribution'.
 *  There is no ElectricLoadCenter:Generators object in OpenStudio.  Instead, the ElectricLoadCenterDistribution object references
 *  a model object list which references all of the associated generators.  The rated electric power, availability schedule, and thermal
 *  to electric power ratio fields are contained in the generator objects themselves rather than the ElectricLoadCenter:Generators
 *  as in EnergyPlus. Electric storage and transformers will be added at a later date.
*/
class MODEL_API ElectricLoadCenterDistribution : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit ElectricLoadCenterDistribution(const Model& model);

  virtual ~ElectricLoadCenterDistribution() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> generatorOperationSchemeTypeValues();

  static std::vector<std::string> electricalBussTypeValues();

  static std::vector<std::string> storageOperationSchemeValues();

  /** @name Getters */
  //@{

  /// Returns associated generators, note there is no ElectricLoadCenter:Generators object in OpenStudio.
  std::vector<Generator> generators() const;

  std::string generatorOperationSchemeType() const;

  bool isGeneratorOperationSchemeTypeDefaulted() const;

  boost::optional<double> demandLimitSchemePurchasedElectricDemandLimit() const;

  boost::optional<Schedule> trackScheduleSchemeSchedule() const;

  boost::optional<std::string> trackMeterSchemeMeterName() const;

  std::string electricalBussType() const;

  bool isElectricalBussTypeDefaulted() const;

  boost::optional<Inverter> inverter() const;

  boost::optional<ElectricalStorage> electricalStorage() const;

  //boost::optional<Transformer> transformer() const;


  // New

  // Storage Operation Scheme, defaults to TrackFacilityElectricDemandStoreExcessOnSite
  std::string storageOperationScheme() const;
  bool isStorageOperationSchemeDefaulted() const;

  // Storage Control Track Meter Name, required if operation = TrackMeterDemandStoreExcessOnSite
  boost::optional<std::string> storageControlTrackMeterName() const;

  // Storage Converter Object Name
  boost::optional<ElectricLoadCenterStorageConverter> storageConverter() const;

  // Maximum Storage State of Charge Fraction, required if storage, defaults
  double maximumStorageStateofChargeFraction() const;
  bool isMaximumStorageStateofChargeFractionDefaulted() const;

  // Minimum Storage State of Charge Fraction, required if storage, defaults
  double minimumStorageStateofChargeFraction() const;
  bool isMinimumStorageStateofChargeFractionDefaulted() const;

  // Design Storage Control Charge Power, required if FacilityDemandLeveling or TrackChargeDischargeSchedules
  boost::optional<double> designStorageControlChargePower() const;

  // Storage Charge Power Fraction Schedule Name, required if TrackChargeDischargeSchedules
  // TODO: do I want to default that to daytime?
  boost::optional<Schedule> storageChargePowerFractionSchedule() const;

  // Design Storage Control Discharge Power, required if FacilityDemandLeveling or TrackChargeDischargeSchedules
  boost::optional<double> designStorageControlDischargePower() const;

  // Storage Charge Power Fraction Schedule Name, required if TrackChargeDischargeSchedules
  // TODO: do I want to default that to daytime?
  boost::optional<Schedule> storageDischargePowerFractionSchedule() const;


  // Storage Control Utility Demand Target, required if FacilityDemandLeveling
  boost::optional<double> storageControlUtilityDemandTarget() const;


  // Storage Control Utility Demand Target Fraction Schedule Name, will be used only if FacilityDemandLeveling, defaults to 1.0
  Schedule storageControlUtilityDemandTargetFractionSchedule() const;
  bool isStorageControlUtilityDemandTargetFractionScheduleDefaulted() const;


  //@}
  /** @name Setters */
  //@{

  bool addGenerator(const Generator& generator);

  bool removeGenerator(const Generator& generator);

  void resetGenerators();

  bool setGeneratorOperationSchemeType(const std::string& generatorOperationSchemeType);

  void resetGeneratorOperationSchemeType();

  bool setDemandLimitSchemePurchasedElectricDemandLimit(double demandLimitSchemePurchasedElectricDemandLimit);

  void resetDemandLimitSchemePurchasedElectricDemandLimit();

  bool setTrackScheduleSchemeSchedule(Schedule& schedule);

  void resetTrackScheduleSchemeSchedule();

  bool setTrackMeterSchemeMeterName(const std::string& trackMeterSchemeMeterName);

  void resetTrackMeterSchemeMeterName();

  bool setElectricalBussType(const std::string& electricalBussType);

  void resetElectricalBussType();

  bool setInverter(const Inverter& inverter);

  void resetInverter();

  bool setElectricalStorage(const ElectricalStorage& electricalStorage);

  void resetElectricalStorage();

  //bool setTransformer(const Transformer& transformer);

  //void resetTransformer();

  // Storage Operation Scheme
  bool setStorageOperationScheme(const std::string& operationScheme);
  void resetStorageOperationScheme();

  // Storage Control Track Meter Name, required if operation = TrackMeterDemandStoreExcessOnSite
  bool setStorageControlTrackMeterName(const std::string& meterName);
  void resetStorageControlTrackMeterName();

  // Storage Converter Object Name
  bool setStorageConverter(const ElectricLoadCenterStorageConverter& converter);
  void resetStorageConverter();

  // Maximum Storage State of Charge Fraction, required if storage, defaults
  bool setMaximumStorageStateofChargeFraction(const double maxStateofCharge);
  void resetMaximumStorageStateofChargeFraction();

  // Minimum Storage State of Charge Fraction, required if storage, defaults
  bool setMinimumStorageStateofChargeFraction(const double minStateofCharge);
  void resetMinimumStorageStateofChargeFraction();

  // Design Storage Control Charge Power, required if FacilityDemandLeveling or TrackChargeDischargeSchedules
  bool setDesignStorageControlChargePower(const double designStorageControlChargePower);
  void resetDesignStorageControlChargePower();

  // Storage Charge Power Fraction Schedule Name, required if TrackChargeDischargeSchedules
  // TODO: do I want to default that to daytime?
  bool setStorageChargePowerFractionSchedule(Schedule& schedule);
  void resetStorageChargePowerFractionSchedule();

  // Design Storage Control Discharge Power, required if FacilityDemandLeveling or TrackChargeDischargeSchedules
  bool setDesignStorageControlDischargePower(const double designStorageControlDischargePower);
  void resetDesignStorageControlDischargePower();

  // Storage Charge Power Fraction Schedule Name, required if TrackChargeDischargeSchedules
  // TODO: do I want to default that to daytime?
  bool setStorageDischargePowerFractionSchedule(Schedule& schedule);
  void resetStorageDischargePowerFractionSchedule();

  // Storage Control Utility Demand Target, required if FacilityDemandLeveling
  bool setStorageControlUtilityDemandTarget(const double storageControlUtilityDemandTarget);
  void resetStorageControlUtilityDemandTarget();

  // Storage Control Utility Demand Target Fraction Schedule Name, will be used only if FacilityDemandLeveling, defaults to 1.0
  bool setStorageControlUtilityDemandTargetFractionSchedule(Schedule& schedule);
  void resetStorageControlUtilityDemandTargetFractionSchedule();

  //@}
  /** @name Other */
  //@{

  bool validityCheck() const;

  //@}
 protected:

  /// @cond
  typedef detail::ElectricLoadCenterDistribution_Impl ImplType;

  explicit ElectricLoadCenterDistribution(std::shared_ptr<detail::ElectricLoadCenterDistribution_Impl> impl);

  friend class detail::ElectricLoadCenterDistribution_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ElectricLoadCenterDistribution");
};

/** \relates ElectricLoadCenterDistribution*/
typedef boost::optional<ElectricLoadCenterDistribution> OptionalElectricLoadCenterDistribution;

/** \relates ElectricLoadCenterDistribution*/
typedef std::vector<ElectricLoadCenterDistribution> ElectricLoadCenterDistributionVector;

} // model
} // openstudio

#endif // MODEL_ELECTRICLOADCENTERDISTRIBUTION_HPP

