/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "FuelFactors.hpp"
#include "FuelFactors_Impl.hpp"

#include "Schedule.hpp"
#include "Schedule_Impl.hpp"
#include "ScheduleTypeLimits.hpp"
#include "ScheduleTypeRegistry.hpp"

#include <utilities/idd/IddFactory.hxx>
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_FuelFactors_FieldEnums.hxx>

#include "../utilities/units/Unit.hpp"

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    FuelFactors_Impl::FuelFactors_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == FuelFactors::iddObjectType());
    }

    FuelFactors_Impl::FuelFactors_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == FuelFactors::iddObjectType());
    }

    FuelFactors_Impl::FuelFactors_Impl(const FuelFactors_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& FuelFactors_Impl::outputVariableNames() const {
      static std::vector<std::string> result;
      if (result.empty()) {
      }
      return result;
    }

    IddObjectType FuelFactors_Impl::iddObjectType() const {
      return FuelFactors::iddObjectType();
    }

    std::vector<ScheduleTypeKey> FuelFactors_Impl::getScheduleTypeKeys(const Schedule& schedule) const {
      std::vector<ScheduleTypeKey> result;
      UnsignedVector fieldIndices = getSourceIndices(schedule.handle());
      UnsignedVector::const_iterator b(fieldIndices.begin());
      UnsignedVector::const_iterator e(fieldIndices.end());
      if (std::find(b, e, OS_FuelFactorsFields::SourceEnergyScheduleName) != e) {
        result.push_back(ScheduleTypeKey("FuelFactors", "Source Energy"));
      }
      if (std::find(b, e, OS_FuelFactorsFields::CO2EmissionFactorScheduleName) != e) {
        result.push_back(ScheduleTypeKey("FuelFactors", "CO2 Emission Factor"));
      }
      if (std::find(b, e, OS_FuelFactorsFields::COEmissionFactorScheduleName) != e) {
        result.push_back(ScheduleTypeKey("FuelFactors", "CO Emission Factor"));
      }
      if (std::find(b, e, OS_FuelFactorsFields::CH4EmissionFactorScheduleName) != e) {
        result.push_back(ScheduleTypeKey("FuelFactors", "CH4 Emission Factor"));
      }
      if (std::find(b, e, OS_FuelFactorsFields::NOxEmissionFactorScheduleName) != e) {
        result.push_back(ScheduleTypeKey("FuelFactors", "NOx Emission Factor"));
      }
      if (std::find(b, e, OS_FuelFactorsFields::N2OEmissionFactorScheduleName) != e) {
        result.push_back(ScheduleTypeKey("FuelFactors", "N2O Emission Factor"));
      }
      if (std::find(b, e, OS_FuelFactorsFields::SO2EmissionFactorScheduleName) != e) {
        result.push_back(ScheduleTypeKey("FuelFactors", "SO2 Emission Factor"));
      }
      if (std::find(b, e, OS_FuelFactorsFields::PMEmissionFactorScheduleName) != e) {
        result.push_back(ScheduleTypeKey("FuelFactors", "PM Emission Factor"));
      }
      if (std::find(b, e, OS_FuelFactorsFields::PM10EmissionFactorScheduleName) != e) {
        result.push_back(ScheduleTypeKey("FuelFactors", "PM10 Emission Factor"));
      }
      if (std::find(b, e, OS_FuelFactorsFields::PM25EmissionFactorScheduleName) != e) {
        result.push_back(ScheduleTypeKey("FuelFactors", "PM2.5 Emission Factor"));
      }
      if (std::find(b, e, OS_FuelFactorsFields::NH3EmissionFactorScheduleName) != e) {
        result.push_back(ScheduleTypeKey("FuelFactors", "NH3 Emission Factor"));
      }
      if (std::find(b, e, OS_FuelFactorsFields::NMVOCEmissionFactorScheduleName) != e) {
        result.push_back(ScheduleTypeKey("FuelFactors", "NMVOC Emission Factor"));
      }
      if (std::find(b, e, OS_FuelFactorsFields::HgEmissionFactorScheduleName) != e) {
        result.push_back(ScheduleTypeKey("FuelFactors", "Hg Emission Factor"));
      }
      if (std::find(b, e, OS_FuelFactorsFields::PbEmissionFactorScheduleName) != e) {
        result.push_back(ScheduleTypeKey("FuelFactors", "Pb Emission Factor"));
      }
      if (std::find(b, e, OS_FuelFactorsFields::WaterEmissionFactorScheduleName) != e) {
        result.push_back(ScheduleTypeKey("FuelFactors", "Water Emission Factor"));
      }
      if (std::find(b, e, OS_FuelFactorsFields::NuclearHighLevelEmissionFactorScheduleName) != e) {
        result.push_back(ScheduleTypeKey("FuelFactors", "Nuclear High Level Emission Factor"));
      }
      if (std::find(b, e, OS_FuelFactorsFields::NuclearLowLevelEmissionFactorScheduleName) != e) {
        result.push_back(ScheduleTypeKey("FuelFactors", "Nuclear Low Level Emission Factor"));
      }
      return result;
    }

    std::string FuelFactors_Impl::existingFuelResourceName() const {
      boost::optional<std::string> value = getString(OS_FuelFactorsFields::ExistingFuelResourceName, true);
      OS_ASSERT(value);
      return value.get();
    }

    double FuelFactors_Impl::sourceEnergyFactor() const {
      boost::optional<double> value = getDouble(OS_FuelFactorsFields::SourceEnergyFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> FuelFactors_Impl::sourceEnergySchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_FuelFactorsFields::SourceEnergyScheduleName);
    }

    double FuelFactors_Impl::cO2EmissionFactor() const {
      boost::optional<double> value = getDouble(OS_FuelFactorsFields::CO2EmissionFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> FuelFactors_Impl::cO2EmissionFactorSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_FuelFactorsFields::CO2EmissionFactorScheduleName);
    }

    double FuelFactors_Impl::cOEmissionFactor() const {
      boost::optional<double> value = getDouble(OS_FuelFactorsFields::COEmissionFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> FuelFactors_Impl::cOEmissionFactorSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_FuelFactorsFields::COEmissionFactorScheduleName);
    }

    double FuelFactors_Impl::cH4EmissionFactor() const {
      boost::optional<double> value = getDouble(OS_FuelFactorsFields::CH4EmissionFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> FuelFactors_Impl::cH4EmissionFactorSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_FuelFactorsFields::CH4EmissionFactorScheduleName);
    }

    double FuelFactors_Impl::nOxEmissionFactor() const {
      boost::optional<double> value = getDouble(OS_FuelFactorsFields::NOxEmissionFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> FuelFactors_Impl::nOxEmissionFactorSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_FuelFactorsFields::NOxEmissionFactorScheduleName);
    }

    double FuelFactors_Impl::n2OEmissionFactor() const {
      boost::optional<double> value = getDouble(OS_FuelFactorsFields::N2OEmissionFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> FuelFactors_Impl::n2OEmissionFactorSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_FuelFactorsFields::N2OEmissionFactorScheduleName);
    }

    double FuelFactors_Impl::sO2EmissionFactor() const {
      boost::optional<double> value = getDouble(OS_FuelFactorsFields::SO2EmissionFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> FuelFactors_Impl::sO2EmissionFactorSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_FuelFactorsFields::SO2EmissionFactorScheduleName);
    }

    double FuelFactors_Impl::pMEmissionFactor() const {
      boost::optional<double> value = getDouble(OS_FuelFactorsFields::PMEmissionFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> FuelFactors_Impl::pMEmissionFactorSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_FuelFactorsFields::PMEmissionFactorScheduleName);
    }

    double FuelFactors_Impl::pM10EmissionFactor() const {
      boost::optional<double> value = getDouble(OS_FuelFactorsFields::PM10EmissionFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> FuelFactors_Impl::pM10EmissionFactorSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_FuelFactorsFields::PM10EmissionFactorScheduleName);
    }

    double FuelFactors_Impl::pM25EmissionFactor() const {
      boost::optional<double> value = getDouble(OS_FuelFactorsFields::PM25EmissionFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> FuelFactors_Impl::pM25EmissionFactorSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_FuelFactorsFields::PM25EmissionFactorScheduleName);
    }

    double FuelFactors_Impl::nH3EmissionFactor() const {
      boost::optional<double> value = getDouble(OS_FuelFactorsFields::NH3EmissionFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> FuelFactors_Impl::nH3EmissionFactorSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_FuelFactorsFields::NH3EmissionFactorScheduleName);
    }

    double FuelFactors_Impl::nMVOCEmissionFactor() const {
      boost::optional<double> value = getDouble(OS_FuelFactorsFields::NMVOCEmissionFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> FuelFactors_Impl::nMVOCEmissionFactorSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_FuelFactorsFields::NMVOCEmissionFactorScheduleName);
    }

    double FuelFactors_Impl::hgEmissionFactor() const {
      boost::optional<double> value = getDouble(OS_FuelFactorsFields::HgEmissionFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> FuelFactors_Impl::hgEmissionFactorSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_FuelFactorsFields::HgEmissionFactorScheduleName);
    }

    double FuelFactors_Impl::pbEmissionFactor() const {
      boost::optional<double> value = getDouble(OS_FuelFactorsFields::PbEmissionFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> FuelFactors_Impl::pbEmissionFactorSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_FuelFactorsFields::PbEmissionFactorScheduleName);
    }

    double FuelFactors_Impl::waterEmissionFactor() const {
      boost::optional<double> value = getDouble(OS_FuelFactorsFields::WaterEmissionFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> FuelFactors_Impl::waterEmissionFactorSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_FuelFactorsFields::WaterEmissionFactorScheduleName);
    }

    double FuelFactors_Impl::nuclearHighLevelEmissionFactor() const {
      boost::optional<double> value = getDouble(OS_FuelFactorsFields::NuclearHighLevelEmissionFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> FuelFactors_Impl::nuclearHighLevelEmissionFactorSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_FuelFactorsFields::NuclearHighLevelEmissionFactorScheduleName);
    }

    double FuelFactors_Impl::nuclearLowLevelEmissionFactor() const {
      boost::optional<double> value = getDouble(OS_FuelFactorsFields::NuclearLowLevelEmissionFactor, true);
      OS_ASSERT(value);
      return value.get();
    }

    boost::optional<Schedule> FuelFactors_Impl::nuclearLowLevelEmissionFactorSchedule() const {
      return getObject<ModelObject>().getModelObjectTarget<Schedule>(OS_FuelFactorsFields::NuclearLowLevelEmissionFactorScheduleName);
    }

    bool FuelFactors_Impl::setExistingFuelResourceName(const std::string& existingFuelResourceName) {
      bool result = setString(OS_FuelFactorsFields::ExistingFuelResourceName, existingFuelResourceName);
      return result;
    }

    bool FuelFactors_Impl::setSourceEnergyFactor(double sourceEnergyFactor) {
      bool result = setDouble(OS_FuelFactorsFields::SourceEnergyFactor, sourceEnergyFactor);
      return result;
    }

    bool FuelFactors_Impl::setSourceEnergySchedule(Schedule& schedule) {
      bool result = setSchedule(OS_FuelFactorsFields::SourceEnergyScheduleName, "FuelFactors", "Source Energy", schedule);
      return result;
    }

    void FuelFactors_Impl::resetSourceEnergySchedule() {
      bool result = setString(OS_FuelFactorsFields::SourceEnergyScheduleName, "");
      OS_ASSERT(result);
    }

    bool FuelFactors_Impl::setCO2EmissionFactor(double cO2EmissionFactor) {
      bool result = setDouble(OS_FuelFactorsFields::CO2EmissionFactor, cO2EmissionFactor);
      return result;
    }

    bool FuelFactors_Impl::setCO2EmissionFactorSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_FuelFactorsFields::CO2EmissionFactorScheduleName, "FuelFactors", "CO2 Emission Factor", schedule);
      return result;
    }

    void FuelFactors_Impl::resetCO2EmissionFactorSchedule() {
      bool result = setString(OS_FuelFactorsFields::CO2EmissionFactorScheduleName, "");
      OS_ASSERT(result);
    }

    bool FuelFactors_Impl::setCOEmissionFactor(double cOEmissionFactor) {
      bool result = setDouble(OS_FuelFactorsFields::COEmissionFactor, cOEmissionFactor);
      return result;
    }

    bool FuelFactors_Impl::setCOEmissionFactorSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_FuelFactorsFields::COEmissionFactorScheduleName, "FuelFactors", "CO Emission Factor", schedule);
      return result;
    }

    void FuelFactors_Impl::resetCOEmissionFactorSchedule() {
      bool result = setString(OS_FuelFactorsFields::COEmissionFactorScheduleName, "");
      OS_ASSERT(result);
    }

    bool FuelFactors_Impl::setCH4EmissionFactor(double cH4EmissionFactor) {
      bool result = setDouble(OS_FuelFactorsFields::CH4EmissionFactor, cH4EmissionFactor);
      return result;
    }

    bool FuelFactors_Impl::setCH4EmissionFactorSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_FuelFactorsFields::CH4EmissionFactorScheduleName, "FuelFactors", "CH4 Emission Factor", schedule);
      return result;
    }

    void FuelFactors_Impl::resetCH4EmissionFactorSchedule() {
      bool result = setString(OS_FuelFactorsFields::CH4EmissionFactorScheduleName, "");
      OS_ASSERT(result);
    }

    bool FuelFactors_Impl::setNOxEmissionFactor(double nOxEmissionFactor) {
      bool result = setDouble(OS_FuelFactorsFields::NOxEmissionFactor, nOxEmissionFactor);
      return result;
    }

    bool FuelFactors_Impl::setNOxEmissionFactorSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_FuelFactorsFields::NOxEmissionFactorScheduleName, "FuelFactors", "NOx Emission Factor", schedule);
      return result;
    }

    void FuelFactors_Impl::resetNOxEmissionFactorSchedule() {
      bool result = setString(OS_FuelFactorsFields::NOxEmissionFactorScheduleName, "");
      OS_ASSERT(result);
    }

    bool FuelFactors_Impl::setN2OEmissionFactor(double n2OEmissionFactor) {
      bool result = setDouble(OS_FuelFactorsFields::N2OEmissionFactor, n2OEmissionFactor);
      return result;
    }

    bool FuelFactors_Impl::setN2OEmissionFactorSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_FuelFactorsFields::N2OEmissionFactorScheduleName, "FuelFactors", "N2O Emission Factor", schedule);
      return result;
    }

    void FuelFactors_Impl::resetN2OEmissionFactorSchedule() {
      bool result = setString(OS_FuelFactorsFields::N2OEmissionFactorScheduleName, "");
      OS_ASSERT(result);
    }

    bool FuelFactors_Impl::setSO2EmissionFactor(double sO2EmissionFactor) {
      bool result = setDouble(OS_FuelFactorsFields::SO2EmissionFactor, sO2EmissionFactor);
      return result;
    }

    bool FuelFactors_Impl::setSO2EmissionFactorSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_FuelFactorsFields::SO2EmissionFactorScheduleName, "FuelFactors", "SO2 Emission Factor", schedule);
      return result;
    }

    void FuelFactors_Impl::resetSO2EmissionFactorSchedule() {
      bool result = setString(OS_FuelFactorsFields::SO2EmissionFactorScheduleName, "");
      OS_ASSERT(result);
    }

    bool FuelFactors_Impl::setPMEmissionFactor(double pMEmissionFactor) {
      bool result = setDouble(OS_FuelFactorsFields::PMEmissionFactor, pMEmissionFactor);
      return result;
    }

    bool FuelFactors_Impl::setPMEmissionFactorSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_FuelFactorsFields::PMEmissionFactorScheduleName, "FuelFactors", "PM Emission Factor", schedule);
      return result;
    }

    void FuelFactors_Impl::resetPMEmissionFactorSchedule() {
      bool result = setString(OS_FuelFactorsFields::PMEmissionFactorScheduleName, "");
      OS_ASSERT(result);
    }

    bool FuelFactors_Impl::setPM10EmissionFactor(double pM10EmissionFactor) {
      bool result = setDouble(OS_FuelFactorsFields::PM10EmissionFactor, pM10EmissionFactor);
      return result;
    }

    bool FuelFactors_Impl::setPM10EmissionFactorSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_FuelFactorsFields::PM10EmissionFactorScheduleName, "FuelFactors", "PM10 Emission Factor", schedule);
      return result;
    }

    void FuelFactors_Impl::resetPM10EmissionFactorSchedule() {
      bool result = setString(OS_FuelFactorsFields::PM10EmissionFactorScheduleName, "");
      OS_ASSERT(result);
    }

    bool FuelFactors_Impl::setPM25EmissionFactor(double pM25EmissionFactor) {
      bool result = setDouble(OS_FuelFactorsFields::PM25EmissionFactor, pM25EmissionFactor);
      return result;
    }

    bool FuelFactors_Impl::setPM25EmissionFactorSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_FuelFactorsFields::PM25EmissionFactorScheduleName, "FuelFactors", "PM2.5 Emission Factor", schedule);
      return result;
    }

    void FuelFactors_Impl::resetPM25EmissionFactorSchedule() {
      bool result = setString(OS_FuelFactorsFields::PM25EmissionFactorScheduleName, "");
      OS_ASSERT(result);
    }

    bool FuelFactors_Impl::setNH3EmissionFactor(double nH3EmissionFactor) {
      bool result = setDouble(OS_FuelFactorsFields::NH3EmissionFactor, nH3EmissionFactor);
      return result;
    }

    bool FuelFactors_Impl::setNH3EmissionFactorSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_FuelFactorsFields::NH3EmissionFactorScheduleName, "FuelFactors", "NH3 Emission Factor", schedule);
      return result;
    }

    void FuelFactors_Impl::resetNH3EmissionFactorSchedule() {
      bool result = setString(OS_FuelFactorsFields::NH3EmissionFactorScheduleName, "");
      OS_ASSERT(result);
    }

    bool FuelFactors_Impl::setNMVOCEmissionFactor(double nMVOCEmissionFactor) {
      bool result = setDouble(OS_FuelFactorsFields::NMVOCEmissionFactor, nMVOCEmissionFactor);
      return result;
    }

    bool FuelFactors_Impl::setNMVOCEmissionFactorSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_FuelFactorsFields::NMVOCEmissionFactorScheduleName, "FuelFactors", "NMVOC Emission Factor", schedule);
      return result;
    }

    void FuelFactors_Impl::resetNMVOCEmissionFactorSchedule() {
      bool result = setString(OS_FuelFactorsFields::NMVOCEmissionFactorScheduleName, "");
      OS_ASSERT(result);
    }

    bool FuelFactors_Impl::setHgEmissionFactor(double hgEmissionFactor) {
      bool result = setDouble(OS_FuelFactorsFields::HgEmissionFactor, hgEmissionFactor);
      return result;
    }

    bool FuelFactors_Impl::setHgEmissionFactorSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_FuelFactorsFields::HgEmissionFactorScheduleName, "FuelFactors", "Hg Emission Factor", schedule);
      return result;
    }

    void FuelFactors_Impl::resetHgEmissionFactorSchedule() {
      bool result = setString(OS_FuelFactorsFields::HgEmissionFactorScheduleName, "");
      OS_ASSERT(result);
    }

    bool FuelFactors_Impl::setPbEmissionFactor(double pbEmissionFactor) {
      bool result = setDouble(OS_FuelFactorsFields::PbEmissionFactor, pbEmissionFactor);
      return result;
    }

    bool FuelFactors_Impl::setPbEmissionFactorSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_FuelFactorsFields::PbEmissionFactorScheduleName, "FuelFactors", "Pb Emission Factor", schedule);
      return result;
    }

    void FuelFactors_Impl::resetPbEmissionFactorSchedule() {
      bool result = setString(OS_FuelFactorsFields::PbEmissionFactorScheduleName, "");
      OS_ASSERT(result);
    }

    bool FuelFactors_Impl::setWaterEmissionFactor(double waterEmissionFactor) {
      bool result = setDouble(OS_FuelFactorsFields::WaterEmissionFactor, waterEmissionFactor);
      return result;
    }

    bool FuelFactors_Impl::setWaterEmissionFactorSchedule(Schedule& schedule) {
      bool result = setSchedule(OS_FuelFactorsFields::WaterEmissionFactorScheduleName, "FuelFactors", "Water Emission Factor", schedule);
      return result;
    }

    void FuelFactors_Impl::resetWaterEmissionFactorSchedule() {
      bool result = setString(OS_FuelFactorsFields::WaterEmissionFactorScheduleName, "");
      OS_ASSERT(result);
    }

    bool FuelFactors_Impl::setNuclearHighLevelEmissionFactor(double nuclearHighLevelEmissionFactor) {
      bool result = setDouble(OS_FuelFactorsFields::NuclearHighLevelEmissionFactor, nuclearHighLevelEmissionFactor);
      return result;
    }

    bool FuelFactors_Impl::setNuclearHighLevelEmissionFactorSchedule(Schedule& schedule) {
      bool result =
        setSchedule(OS_FuelFactorsFields::NuclearHighLevelEmissionFactorScheduleName, "FuelFactors", "Nuclear High Level Emission Factor", schedule);
      return result;
    }

    void FuelFactors_Impl::resetNuclearHighLevelEmissionFactorSchedule() {
      bool result = setString(OS_FuelFactorsFields::NuclearHighLevelEmissionFactorScheduleName, "");
      OS_ASSERT(result);
    }

    bool FuelFactors_Impl::setNuclearLowLevelEmissionFactor(double nuclearLowLevelEmissionFactor) {
      bool result = setDouble(OS_FuelFactorsFields::NuclearLowLevelEmissionFactor, nuclearLowLevelEmissionFactor);
      return result;
    }

    bool FuelFactors_Impl::setNuclearLowLevelEmissionFactorSchedule(Schedule& schedule) {
      bool result =
        setSchedule(OS_FuelFactorsFields::NuclearLowLevelEmissionFactorScheduleName, "FuelFactors", "Nuclear Low Level Emission Factor", schedule);
      return result;
    }

    void FuelFactors_Impl::resetNuclearLowLevelEmissionFactorSchedule() {
      bool result = setString(OS_FuelFactorsFields::NuclearLowLevelEmissionFactorScheduleName, "");
      OS_ASSERT(result);
    }

  }  // namespace detail

  FuelFactors::FuelFactors(const Model& model) : ModelObject(FuelFactors::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::FuelFactors_Impl>());

    // From ElectricityUSAEnvironmentalImpactFactors.idf,
    // United States 1999 national average electricity emissions factors based on eGRID, 1605, AirData
    setExistingFuelResourceName("Electricity");
    setSourceEnergyFactor(2.253);
    setCO2EmissionFactor(168.33317);
    setCOEmissionFactor(4.20616E-02);
    setCH4EmissionFactor(1.39858E-03);
    setNOxEmissionFactor(4.10753E-01);
    setN2OEmissionFactor(2.41916E-03);
    setSO2EmissionFactor(8.65731E-01);
    setPMEmissionFactor(2.95827E-02);
    setPM10EmissionFactor(1.80450E-02);
    setPM25EmissionFactor(1.15377E-02);
    setNH3EmissionFactor(1.10837E-03);
    setNMVOCEmissionFactor(3.72332E-03);
    setHgEmissionFactor(3.36414E-06);
    setPbEmissionFactor(0.0);
    setWaterEmissionFactor(2.10074);
    setNuclearHighLevelEmissionFactor(0.0);
    setNuclearLowLevelEmissionFactor(0.0);
  }

  IddObjectType FuelFactors::iddObjectType() {
    return {IddObjectType::OS_FuelFactors};
  }

  std::vector<std::string> FuelFactors::existingFuelResourceNameValues() {
    return getIddKeyNames(IddFactory::instance().getObject(iddObjectType()).get(), OS_FuelFactorsFields::ExistingFuelResourceName);
  }

  std::string FuelFactors::existingFuelResourceName() const {
    return getImpl<detail::FuelFactors_Impl>()->existingFuelResourceName();
  }

  double FuelFactors::sourceEnergyFactor() const {
    return getImpl<detail::FuelFactors_Impl>()->sourceEnergyFactor();
  }

  boost::optional<Schedule> FuelFactors::sourceEnergySchedule() const {
    return getImpl<detail::FuelFactors_Impl>()->sourceEnergySchedule();
  }

  double FuelFactors::cO2EmissionFactor() const {
    return getImpl<detail::FuelFactors_Impl>()->cO2EmissionFactor();
  }

  boost::optional<Schedule> FuelFactors::cO2EmissionFactorSchedule() const {
    return getImpl<detail::FuelFactors_Impl>()->cO2EmissionFactorSchedule();
  }

  double FuelFactors::cOEmissionFactor() const {
    return getImpl<detail::FuelFactors_Impl>()->cOEmissionFactor();
  }

  boost::optional<Schedule> FuelFactors::cOEmissionFactorSchedule() const {
    return getImpl<detail::FuelFactors_Impl>()->cOEmissionFactorSchedule();
  }

  double FuelFactors::cH4EmissionFactor() const {
    return getImpl<detail::FuelFactors_Impl>()->cH4EmissionFactor();
  }

  boost::optional<Schedule> FuelFactors::cH4EmissionFactorSchedule() const {
    return getImpl<detail::FuelFactors_Impl>()->cH4EmissionFactorSchedule();
  }

  double FuelFactors::nOxEmissionFactor() const {
    return getImpl<detail::FuelFactors_Impl>()->nOxEmissionFactor();
  }

  boost::optional<Schedule> FuelFactors::nOxEmissionFactorSchedule() const {
    return getImpl<detail::FuelFactors_Impl>()->nOxEmissionFactorSchedule();
  }

  double FuelFactors::n2OEmissionFactor() const {
    return getImpl<detail::FuelFactors_Impl>()->n2OEmissionFactor();
  }

  boost::optional<Schedule> FuelFactors::n2OEmissionFactorSchedule() const {
    return getImpl<detail::FuelFactors_Impl>()->n2OEmissionFactorSchedule();
  }

  double FuelFactors::sO2EmissionFactor() const {
    return getImpl<detail::FuelFactors_Impl>()->sO2EmissionFactor();
  }

  boost::optional<Schedule> FuelFactors::sO2EmissionFactorSchedule() const {
    return getImpl<detail::FuelFactors_Impl>()->sO2EmissionFactorSchedule();
  }

  double FuelFactors::pMEmissionFactor() const {
    return getImpl<detail::FuelFactors_Impl>()->pMEmissionFactor();
  }

  boost::optional<Schedule> FuelFactors::pMEmissionFactorSchedule() const {
    return getImpl<detail::FuelFactors_Impl>()->pMEmissionFactorSchedule();
  }

  double FuelFactors::pM10EmissionFactor() const {
    return getImpl<detail::FuelFactors_Impl>()->pM10EmissionFactor();
  }

  boost::optional<Schedule> FuelFactors::pM10EmissionFactorSchedule() const {
    return getImpl<detail::FuelFactors_Impl>()->pM10EmissionFactorSchedule();
  }

  double FuelFactors::pM25EmissionFactor() const {
    return getImpl<detail::FuelFactors_Impl>()->pM25EmissionFactor();
  }

  boost::optional<Schedule> FuelFactors::pM25EmissionFactorSchedule() const {
    return getImpl<detail::FuelFactors_Impl>()->pM25EmissionFactorSchedule();
  }

  double FuelFactors::nH3EmissionFactor() const {
    return getImpl<detail::FuelFactors_Impl>()->nH3EmissionFactor();
  }

  boost::optional<Schedule> FuelFactors::nH3EmissionFactorSchedule() const {
    return getImpl<detail::FuelFactors_Impl>()->nH3EmissionFactorSchedule();
  }

  double FuelFactors::nMVOCEmissionFactor() const {
    return getImpl<detail::FuelFactors_Impl>()->nMVOCEmissionFactor();
  }

  boost::optional<Schedule> FuelFactors::nMVOCEmissionFactorSchedule() const {
    return getImpl<detail::FuelFactors_Impl>()->nMVOCEmissionFactorSchedule();
  }

  double FuelFactors::hgEmissionFactor() const {
    return getImpl<detail::FuelFactors_Impl>()->hgEmissionFactor();
  }

  boost::optional<Schedule> FuelFactors::hgEmissionFactorSchedule() const {
    return getImpl<detail::FuelFactors_Impl>()->hgEmissionFactorSchedule();
  }

  double FuelFactors::pbEmissionFactor() const {
    return getImpl<detail::FuelFactors_Impl>()->pbEmissionFactor();
  }

  boost::optional<Schedule> FuelFactors::pbEmissionFactorSchedule() const {
    return getImpl<detail::FuelFactors_Impl>()->pbEmissionFactorSchedule();
  }

  double FuelFactors::waterEmissionFactor() const {
    return getImpl<detail::FuelFactors_Impl>()->waterEmissionFactor();
  }

  boost::optional<Schedule> FuelFactors::waterEmissionFactorSchedule() const {
    return getImpl<detail::FuelFactors_Impl>()->waterEmissionFactorSchedule();
  }

  double FuelFactors::nuclearHighLevelEmissionFactor() const {
    return getImpl<detail::FuelFactors_Impl>()->nuclearHighLevelEmissionFactor();
  }

  boost::optional<Schedule> FuelFactors::nuclearHighLevelEmissionFactorSchedule() const {
    return getImpl<detail::FuelFactors_Impl>()->nuclearHighLevelEmissionFactorSchedule();
  }

  double FuelFactors::nuclearLowLevelEmissionFactor() const {
    return getImpl<detail::FuelFactors_Impl>()->nuclearLowLevelEmissionFactor();
  }

  boost::optional<Schedule> FuelFactors::nuclearLowLevelEmissionFactorSchedule() const {
    return getImpl<detail::FuelFactors_Impl>()->nuclearLowLevelEmissionFactorSchedule();
  }

  bool FuelFactors::setExistingFuelResourceName(const std::string& existingFuelResourceName) {
    return getImpl<detail::FuelFactors_Impl>()->setExistingFuelResourceName(existingFuelResourceName);
  }

  bool FuelFactors::setSourceEnergyFactor(double sourceEnergyFactor) {
    return getImpl<detail::FuelFactors_Impl>()->setSourceEnergyFactor(sourceEnergyFactor);
  }

  bool FuelFactors::setSourceEnergySchedule(Schedule& schedule) {
    return getImpl<detail::FuelFactors_Impl>()->setSourceEnergySchedule(schedule);
  }

  void FuelFactors::resetSourceEnergySchedule() {
    getImpl<detail::FuelFactors_Impl>()->resetSourceEnergySchedule();
  }

  bool FuelFactors::setCO2EmissionFactor(double cO2EmissionFactor) {
    return getImpl<detail::FuelFactors_Impl>()->setCO2EmissionFactor(cO2EmissionFactor);
  }

  bool FuelFactors::setCO2EmissionFactorSchedule(Schedule& schedule) {
    return getImpl<detail::FuelFactors_Impl>()->setCO2EmissionFactorSchedule(schedule);
  }

  void FuelFactors::resetCO2EmissionFactorSchedule() {
    getImpl<detail::FuelFactors_Impl>()->resetCO2EmissionFactorSchedule();
  }

  bool FuelFactors::setCOEmissionFactor(double cOEmissionFactor) {
    return getImpl<detail::FuelFactors_Impl>()->setCOEmissionFactor(cOEmissionFactor);
  }

  bool FuelFactors::setCOEmissionFactorSchedule(Schedule& schedule) {
    return getImpl<detail::FuelFactors_Impl>()->setCOEmissionFactorSchedule(schedule);
  }

  void FuelFactors::resetCOEmissionFactorSchedule() {
    getImpl<detail::FuelFactors_Impl>()->resetCOEmissionFactorSchedule();
  }

  bool FuelFactors::setCH4EmissionFactor(double cH4EmissionFactor) {
    return getImpl<detail::FuelFactors_Impl>()->setCH4EmissionFactor(cH4EmissionFactor);
  }

  bool FuelFactors::setCH4EmissionFactorSchedule(Schedule& schedule) {
    return getImpl<detail::FuelFactors_Impl>()->setCH4EmissionFactorSchedule(schedule);
  }

  void FuelFactors::resetCH4EmissionFactorSchedule() {
    getImpl<detail::FuelFactors_Impl>()->resetCH4EmissionFactorSchedule();
  }

  bool FuelFactors::setNOxEmissionFactor(double nOxEmissionFactor) {
    return getImpl<detail::FuelFactors_Impl>()->setNOxEmissionFactor(nOxEmissionFactor);
  }

  bool FuelFactors::setNOxEmissionFactorSchedule(Schedule& schedule) {
    return getImpl<detail::FuelFactors_Impl>()->setNOxEmissionFactorSchedule(schedule);
  }

  void FuelFactors::resetNOxEmissionFactorSchedule() {
    getImpl<detail::FuelFactors_Impl>()->resetNOxEmissionFactorSchedule();
  }

  bool FuelFactors::setN2OEmissionFactor(double n2OEmissionFactor) {
    return getImpl<detail::FuelFactors_Impl>()->setN2OEmissionFactor(n2OEmissionFactor);
  }

  bool FuelFactors::setN2OEmissionFactorSchedule(Schedule& schedule) {
    return getImpl<detail::FuelFactors_Impl>()->setN2OEmissionFactorSchedule(schedule);
  }

  void FuelFactors::resetN2OEmissionFactorSchedule() {
    getImpl<detail::FuelFactors_Impl>()->resetN2OEmissionFactorSchedule();
  }

  bool FuelFactors::setSO2EmissionFactor(double sO2EmissionFactor) {
    return getImpl<detail::FuelFactors_Impl>()->setSO2EmissionFactor(sO2EmissionFactor);
  }

  bool FuelFactors::setSO2EmissionFactorSchedule(Schedule& schedule) {
    return getImpl<detail::FuelFactors_Impl>()->setSO2EmissionFactorSchedule(schedule);
  }

  void FuelFactors::resetSO2EmissionFactorSchedule() {
    getImpl<detail::FuelFactors_Impl>()->resetSO2EmissionFactorSchedule();
  }

  bool FuelFactors::setPMEmissionFactor(double pMEmissionFactor) {
    return getImpl<detail::FuelFactors_Impl>()->setPMEmissionFactor(pMEmissionFactor);
  }

  bool FuelFactors::setPMEmissionFactorSchedule(Schedule& schedule) {
    return getImpl<detail::FuelFactors_Impl>()->setPMEmissionFactorSchedule(schedule);
  }

  void FuelFactors::resetPMEmissionFactorSchedule() {
    getImpl<detail::FuelFactors_Impl>()->resetPMEmissionFactorSchedule();
  }

  bool FuelFactors::setPM10EmissionFactor(double pM10EmissionFactor) {
    return getImpl<detail::FuelFactors_Impl>()->setPM10EmissionFactor(pM10EmissionFactor);
  }

  bool FuelFactors::setPM10EmissionFactorSchedule(Schedule& schedule) {
    return getImpl<detail::FuelFactors_Impl>()->setPM10EmissionFactorSchedule(schedule);
  }

  void FuelFactors::resetPM10EmissionFactorSchedule() {
    getImpl<detail::FuelFactors_Impl>()->resetPM10EmissionFactorSchedule();
  }

  bool FuelFactors::setPM25EmissionFactor(double pM25EmissionFactor) {
    return getImpl<detail::FuelFactors_Impl>()->setPM25EmissionFactor(pM25EmissionFactor);
  }

  bool FuelFactors::setPM25EmissionFactorSchedule(Schedule& schedule) {
    return getImpl<detail::FuelFactors_Impl>()->setPM25EmissionFactorSchedule(schedule);
  }

  void FuelFactors::resetPM25EmissionFactorSchedule() {
    getImpl<detail::FuelFactors_Impl>()->resetPM25EmissionFactorSchedule();
  }

  bool FuelFactors::setNH3EmissionFactor(double nH3EmissionFactor) {
    return getImpl<detail::FuelFactors_Impl>()->setNH3EmissionFactor(nH3EmissionFactor);
  }

  bool FuelFactors::setNH3EmissionFactorSchedule(Schedule& schedule) {
    return getImpl<detail::FuelFactors_Impl>()->setNH3EmissionFactorSchedule(schedule);
  }

  void FuelFactors::resetNH3EmissionFactorSchedule() {
    getImpl<detail::FuelFactors_Impl>()->resetNH3EmissionFactorSchedule();
  }

  bool FuelFactors::setNMVOCEmissionFactor(double nMVOCEmissionFactor) {
    return getImpl<detail::FuelFactors_Impl>()->setNMVOCEmissionFactor(nMVOCEmissionFactor);
  }

  bool FuelFactors::setNMVOCEmissionFactorSchedule(Schedule& schedule) {
    return getImpl<detail::FuelFactors_Impl>()->setNMVOCEmissionFactorSchedule(schedule);
  }

  void FuelFactors::resetNMVOCEmissionFactorSchedule() {
    getImpl<detail::FuelFactors_Impl>()->resetNMVOCEmissionFactorSchedule();
  }

  bool FuelFactors::setHgEmissionFactor(double hgEmissionFactor) {
    return getImpl<detail::FuelFactors_Impl>()->setHgEmissionFactor(hgEmissionFactor);
  }

  bool FuelFactors::setHgEmissionFactorSchedule(Schedule& schedule) {
    return getImpl<detail::FuelFactors_Impl>()->setHgEmissionFactorSchedule(schedule);
  }

  void FuelFactors::resetHgEmissionFactorSchedule() {
    getImpl<detail::FuelFactors_Impl>()->resetHgEmissionFactorSchedule();
  }

  bool FuelFactors::setPbEmissionFactor(double pbEmissionFactor) {
    return getImpl<detail::FuelFactors_Impl>()->setPbEmissionFactor(pbEmissionFactor);
  }

  bool FuelFactors::setPbEmissionFactorSchedule(Schedule& schedule) {
    return getImpl<detail::FuelFactors_Impl>()->setPbEmissionFactorSchedule(schedule);
  }

  void FuelFactors::resetPbEmissionFactorSchedule() {
    getImpl<detail::FuelFactors_Impl>()->resetPbEmissionFactorSchedule();
  }

  bool FuelFactors::setWaterEmissionFactor(double waterEmissionFactor) {
    return getImpl<detail::FuelFactors_Impl>()->setWaterEmissionFactor(waterEmissionFactor);
  }

  bool FuelFactors::setWaterEmissionFactorSchedule(Schedule& schedule) {
    return getImpl<detail::FuelFactors_Impl>()->setWaterEmissionFactorSchedule(schedule);
  }

  void FuelFactors::resetWaterEmissionFactorSchedule() {
    getImpl<detail::FuelFactors_Impl>()->resetWaterEmissionFactorSchedule();
  }

  bool FuelFactors::setNuclearHighLevelEmissionFactor(double nuclearHighLevelEmissionFactor) {
    return getImpl<detail::FuelFactors_Impl>()->setNuclearHighLevelEmissionFactor(nuclearHighLevelEmissionFactor);
  }

  bool FuelFactors::setNuclearHighLevelEmissionFactorSchedule(Schedule& schedule) {
    return getImpl<detail::FuelFactors_Impl>()->setNuclearHighLevelEmissionFactorSchedule(schedule);
  }

  void FuelFactors::resetNuclearHighLevelEmissionFactorSchedule() {
    getImpl<detail::FuelFactors_Impl>()->resetNuclearHighLevelEmissionFactorSchedule();
  }

  bool FuelFactors::setNuclearLowLevelEmissionFactor(double nuclearLowLevelEmissionFactor) {
    return getImpl<detail::FuelFactors_Impl>()->setNuclearLowLevelEmissionFactor(nuclearLowLevelEmissionFactor);
  }

  bool FuelFactors::setNuclearLowLevelEmissionFactorSchedule(Schedule& schedule) {
    return getImpl<detail::FuelFactors_Impl>()->setNuclearLowLevelEmissionFactorSchedule(schedule);
  }

  void FuelFactors::resetNuclearLowLevelEmissionFactorSchedule() {
    getImpl<detail::FuelFactors_Impl>()->resetNuclearLowLevelEmissionFactorSchedule();
  }

  /// @cond
  FuelFactors::FuelFactors(std::shared_ptr<detail::FuelFactors_Impl> impl) : ModelObject(impl) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
