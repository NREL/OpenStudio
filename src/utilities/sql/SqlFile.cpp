/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SqlFile.hpp"
#include "SqlFile_Impl.hpp"
#include "SqlFileTimeSeriesQuery.hpp"

#include <sqlite3.h>

namespace openstudio {

SqlFile::SqlFile() = default;

SqlFile::SqlFile(const openstudio::path& path, const bool createIndexes) {
  try {
    m_impl = std::shared_ptr<detail::SqlFile_Impl>(new detail::SqlFile_Impl(path, createIndexes));
  } catch (const std::exception& e) {
    LOG(Error, "Could not create SqlFile for path '" << openstudio::toString(path) << "' error:" << e.what());
  }
}

SqlFile::SqlFile(const openstudio::path& t_path, const openstudio::EpwFile& t_epwFile, const openstudio::DateTime& t_simulationTime,
                 const openstudio::Calendar& t_calendar, const bool createIndexes) {
  try {
    m_impl = std::shared_ptr<detail::SqlFile_Impl>(new detail::SqlFile_Impl(t_path, t_epwFile, t_simulationTime, t_calendar, createIndexes));
  } catch (const std::exception& e) {
    LOG(Error, "Could not create SqlFile for path '" << openstudio::toString(t_path) << "' error:" << e.what());
  }
}

void SqlFile::createIndexes() {
  if (m_impl) {
    m_impl->createIndexes();
  }
}

void SqlFile::removeIndexes() {
  if (m_impl) {
    m_impl->removeIndexes();
  }
}

bool SqlFile::connectionOpen() const {
  bool result = false;
  if (m_impl) {
    result = m_impl->connectionOpen();
  }
  return result;
}

openstudio::path SqlFile::path() const {
  openstudio::path result;
  if (m_impl) {
    result = m_impl->path();
  }
  return result;
}

bool SqlFile::close() {
  bool result = false;
  if (m_impl) {
    result = m_impl->close();
  }
  return result;
}

bool SqlFile::reopen() {
  bool result = false;
  if (m_impl) {
    result = m_impl->reopen();
  }
  return result;
}

boost::optional<double> SqlFile::hoursSimulated() const {
  boost::optional<double> result;
  if (m_impl) {
    result = m_impl->hoursSimulated();
  }
  return result;
}

boost::optional<double> SqlFile::netSiteEnergy() const {
  boost::optional<double> result;
  if (m_impl) {
    result = m_impl->netSiteEnergy();
  }
  return result;
}

boost::optional<double> SqlFile::netSourceEnergy() const {
  boost::optional<double> result;
  if (m_impl) {
    result = m_impl->netSourceEnergy();
  }
  return result;
}

boost::optional<double> SqlFile::totalSiteEnergy() const {
  boost::optional<double> result;
  if (m_impl) {
    result = m_impl->totalSiteEnergy();
  }
  return result;
}

boost::optional<double> SqlFile::totalSourceEnergy() const {
  boost::optional<double> result;
  if (m_impl) {
    result = m_impl->totalSourceEnergy();
  }
  return result;
}

boost::optional<double> SqlFile::annualTotalCost(const FuelType& fuel) const {
  boost::optional<double> result;
  if (m_impl) {
    result = m_impl->annualTotalCost(fuel);
  }
  return result;
}

boost::optional<double> SqlFile::annualTotalCostPerBldgArea(const FuelType& fuel) const {
  boost::optional<double> result;
  if (m_impl) {
    result = m_impl->annualTotalCostPerBldgArea(fuel);
  }
  return result;
}

boost::optional<double> SqlFile::annualTotalCostPerNetConditionedBldgArea(const FuelType& fuel) const {
  boost::optional<double> result;
  if (m_impl) {
    result = m_impl->annualTotalCostPerNetConditionedBldgArea(fuel);
  }
  return result;
}

boost::optional<double> SqlFile::annualTotalUtilityCost() const {
  boost::optional<double> result;
  if (m_impl) {
    result = m_impl->annualTotalUtilityCost();
  }
  return result;
}

boost::optional<double> SqlFile::economicsEnergyCost() const {
  if (m_impl) {
    return m_impl->economicsEnergyCost();
  } else {
    return {};
  }
}

boost::optional<EndUses> SqlFile::endUses() const {
  if (m_impl) {
    return m_impl->endUses();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::getElecOrGasCost(bool t_getGas) const {
  if (m_impl) {
    return m_impl->getElecOrGasCost(t_getGas);
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::getElecOrGasUse(bool t_getGas) const {
  if (m_impl) {
    return m_impl->getElecOrGasUse(t_getGas);
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::energyConsumptionByMonth(const openstudio::EndUseFuelType& t_fuelType,
                                                          const openstudio::EndUseCategoryType& t_categoryType,
                                                          const openstudio::MonthOfYear& t_monthOfYear) const {
  if (m_impl) {
    return m_impl->energyConsumptionByMonth(t_fuelType, t_categoryType, t_monthOfYear);
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::peakEnergyDemandByMonth(const openstudio::EndUseFuelType& t_fuelType,
                                                         const openstudio::EndUseCategoryType& t_categoryType,
                                                         const openstudio::MonthOfYear& t_monthOfYear) const {
  if (m_impl) {
    return m_impl->peakEnergyDemandByMonth(t_fuelType, t_categoryType, t_monthOfYear);
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::electricityHeating() const {
  if (m_impl) {
    return m_impl->electricityHeating();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::electricityCooling() const {
  if (m_impl) {
    return m_impl->electricityCooling();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::electricityInteriorLighting() const {
  if (m_impl) {
    return m_impl->electricityInteriorLighting();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::electricityExteriorLighting() const {
  if (m_impl) {
    return m_impl->electricityExteriorLighting();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::electricityInteriorEquipment() const {
  if (m_impl) {
    return m_impl->electricityInteriorEquipment();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::electricityExteriorEquipment() const {
  if (m_impl) {
    return m_impl->electricityExteriorEquipment();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::electricityFans() const {
  if (m_impl) {
    return m_impl->electricityFans();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::electricityPumps() const {
  if (m_impl) {
    return m_impl->electricityPumps();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::electricityHeatRejection() const {
  if (m_impl) {
    return m_impl->electricityHeatRejection();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::electricityHumidification() const {
  if (m_impl) {
    return m_impl->electricityHumidification();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::electricityHeatRecovery() const {
  if (m_impl) {
    return m_impl->electricityHeatRecovery();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::electricityWaterSystems() const {
  if (m_impl) {
    return m_impl->electricityWaterSystems();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::electricityRefrigeration() const {
  if (m_impl) {
    return m_impl->electricityRefrigeration();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::electricityGenerators() const {
  if (m_impl) {
    return m_impl->electricityGenerators();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::electricityTotalEndUses() const {
  if (m_impl) {
    return m_impl->electricityTotalEndUses();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::naturalGasHeating() const {
  if (m_impl) {
    return m_impl->naturalGasHeating();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::naturalGasCooling() const {
  if (m_impl) {
    return m_impl->naturalGasCooling();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::naturalGasInteriorLighting() const {
  if (m_impl) {
    return m_impl->naturalGasInteriorLighting();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::naturalGasExteriorLighting() const {
  if (m_impl) {
    return m_impl->naturalGasExteriorLighting();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::naturalGasInteriorEquipment() const {
  if (m_impl) {
    return m_impl->naturalGasInteriorEquipment();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::naturalGasExteriorEquipment() const {
  if (m_impl) {
    return m_impl->naturalGasExteriorEquipment();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::naturalGasFans() const {
  if (m_impl) {
    return m_impl->naturalGasFans();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::naturalGasPumps() const {
  if (m_impl) {
    return m_impl->naturalGasPumps();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::naturalGasHeatRejection() const {
  if (m_impl) {
    return m_impl->naturalGasHeatRejection();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::naturalGasHumidification() const {
  if (m_impl) {
    return m_impl->naturalGasHumidification();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::naturalGasHeatRecovery() const {
  if (m_impl) {
    return m_impl->naturalGasHeatRecovery();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::naturalGasWaterSystems() const {
  if (m_impl) {
    return m_impl->naturalGasWaterSystems();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::naturalGasRefrigeration() const {
  if (m_impl) {
    return m_impl->naturalGasRefrigeration();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::naturalGasGenerators() const {
  if (m_impl) {
    return m_impl->naturalGasGenerators();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::naturalGasTotalEndUses() const {
  if (m_impl) {
    return m_impl->naturalGasTotalEndUses();
  } else {
    return {};
  }
}

// Gasoline
boost::optional<double> SqlFile::gasolineHeating() const {
  if (m_impl) {
    return m_impl->gasolineHeating();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::gasolineCooling() const {
  if (m_impl) {
    return m_impl->gasolineCooling();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::gasolineInteriorLighting() const {
  if (m_impl) {
    return m_impl->gasolineInteriorLighting();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::gasolineExteriorLighting() const {
  if (m_impl) {
    return m_impl->gasolineExteriorLighting();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::gasolineInteriorEquipment() const {
  if (m_impl) {
    return m_impl->gasolineInteriorEquipment();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::gasolineExteriorEquipment() const {
  if (m_impl) {
    return m_impl->gasolineExteriorEquipment();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::gasolineFans() const {
  if (m_impl) {
    return m_impl->gasolineFans();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::gasolinePumps() const {
  if (m_impl) {
    return m_impl->gasolinePumps();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::gasolineHeatRejection() const {
  if (m_impl) {
    return m_impl->gasolineHeatRejection();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::gasolineHumidification() const {
  if (m_impl) {
    return m_impl->gasolineHumidification();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::gasolineHeatRecovery() const {
  if (m_impl) {
    return m_impl->gasolineHeatRecovery();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::gasolineWaterSystems() const {
  if (m_impl) {
    return m_impl->gasolineWaterSystems();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::gasolineRefrigeration() const {
  if (m_impl) {
    return m_impl->gasolineRefrigeration();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::gasolineGenerators() const {
  if (m_impl) {
    return m_impl->gasolineGenerators();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::gasolineTotalEndUses() const {
  if (m_impl) {
    return m_impl->gasolineTotalEndUses();
  } else {
    return boost::optional<double>();
  }
}

// Diesel
boost::optional<double> SqlFile::dieselHeating() const {
  if (m_impl) {
    return m_impl->dieselHeating();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::dieselCooling() const {
  if (m_impl) {
    return m_impl->dieselCooling();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::dieselInteriorLighting() const {
  if (m_impl) {
    return m_impl->dieselInteriorLighting();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::dieselExteriorLighting() const {
  if (m_impl) {
    return m_impl->dieselExteriorLighting();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::dieselInteriorEquipment() const {
  if (m_impl) {
    return m_impl->dieselInteriorEquipment();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::dieselExteriorEquipment() const {
  if (m_impl) {
    return m_impl->dieselExteriorEquipment();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::dieselFans() const {
  if (m_impl) {
    return m_impl->dieselFans();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::dieselPumps() const {
  if (m_impl) {
    return m_impl->dieselPumps();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::dieselHeatRejection() const {
  if (m_impl) {
    return m_impl->dieselHeatRejection();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::dieselHumidification() const {
  if (m_impl) {
    return m_impl->dieselHumidification();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::dieselHeatRecovery() const {
  if (m_impl) {
    return m_impl->dieselHeatRecovery();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::dieselWaterSystems() const {
  if (m_impl) {
    return m_impl->dieselWaterSystems();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::dieselRefrigeration() const {
  if (m_impl) {
    return m_impl->dieselRefrigeration();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::dieselGenerators() const {
  if (m_impl) {
    return m_impl->dieselGenerators();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::dieselTotalEndUses() const {
  if (m_impl) {
    return m_impl->dieselTotalEndUses();
  } else {
    return boost::optional<double>();
  }
}

// Coal
boost::optional<double> SqlFile::coalHeating() const {
  if (m_impl) {
    return m_impl->coalHeating();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::coalCooling() const {
  if (m_impl) {
    return m_impl->coalCooling();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::coalInteriorLighting() const {
  if (m_impl) {
    return m_impl->coalInteriorLighting();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::coalExteriorLighting() const {
  if (m_impl) {
    return m_impl->coalExteriorLighting();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::coalInteriorEquipment() const {
  if (m_impl) {
    return m_impl->coalInteriorEquipment();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::coalExteriorEquipment() const {
  if (m_impl) {
    return m_impl->coalExteriorEquipment();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::coalFans() const {
  if (m_impl) {
    return m_impl->coalFans();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::coalPumps() const {
  if (m_impl) {
    return m_impl->coalPumps();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::coalHeatRejection() const {
  if (m_impl) {
    return m_impl->coalHeatRejection();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::coalHumidification() const {
  if (m_impl) {
    return m_impl->coalHumidification();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::coalHeatRecovery() const {
  if (m_impl) {
    return m_impl->coalHeatRecovery();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::coalWaterSystems() const {
  if (m_impl) {
    return m_impl->coalWaterSystems();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::coalRefrigeration() const {
  if (m_impl) {
    return m_impl->coalRefrigeration();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::coalGenerators() const {
  if (m_impl) {
    return m_impl->coalGenerators();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::coalTotalEndUses() const {
  if (m_impl) {
    return m_impl->coalTotalEndUses();
  } else {
    return boost::optional<double>();
  }
}

// Fuel Oil No 1
boost::optional<double> SqlFile::fuelOilNo1Heating() const {
  if (m_impl) {
    return m_impl->fuelOilNo1Heating();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo1Cooling() const {
  if (m_impl) {
    return m_impl->fuelOilNo1Cooling();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo1InteriorLighting() const {
  if (m_impl) {
    return m_impl->fuelOilNo1InteriorLighting();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo1ExteriorLighting() const {
  if (m_impl) {
    return m_impl->fuelOilNo1ExteriorLighting();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo1InteriorEquipment() const {
  if (m_impl) {
    return m_impl->fuelOilNo1InteriorEquipment();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo1ExteriorEquipment() const {
  if (m_impl) {
    return m_impl->fuelOilNo1ExteriorEquipment();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo1Fans() const {
  if (m_impl) {
    return m_impl->fuelOilNo1Fans();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo1Pumps() const {
  if (m_impl) {
    return m_impl->fuelOilNo1Pumps();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo1HeatRejection() const {
  if (m_impl) {
    return m_impl->fuelOilNo1HeatRejection();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo1Humidification() const {
  if (m_impl) {
    return m_impl->fuelOilNo1Humidification();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo1HeatRecovery() const {
  if (m_impl) {
    return m_impl->fuelOilNo1HeatRecovery();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo1WaterSystems() const {
  if (m_impl) {
    return m_impl->fuelOilNo1WaterSystems();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo1Refrigeration() const {
  if (m_impl) {
    return m_impl->fuelOilNo1Refrigeration();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo1Generators() const {
  if (m_impl) {
    return m_impl->fuelOilNo1Generators();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo1TotalEndUses() const {
  if (m_impl) {
    return m_impl->fuelOilNo1TotalEndUses();
  } else {
    return boost::optional<double>();
  }
}

// Fuel Oil No 2
boost::optional<double> SqlFile::fuelOilNo2Heating() const {
  if (m_impl) {
    return m_impl->fuelOilNo2Heating();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo2Cooling() const {
  if (m_impl) {
    return m_impl->fuelOilNo2Cooling();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo2InteriorLighting() const {
  if (m_impl) {
    return m_impl->fuelOilNo2InteriorLighting();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo2ExteriorLighting() const {
  if (m_impl) {
    return m_impl->fuelOilNo2ExteriorLighting();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo2InteriorEquipment() const {
  if (m_impl) {
    return m_impl->fuelOilNo2InteriorEquipment();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo2ExteriorEquipment() const {
  if (m_impl) {
    return m_impl->fuelOilNo2ExteriorEquipment();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo2Fans() const {
  if (m_impl) {
    return m_impl->fuelOilNo2Fans();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo2Pumps() const {
  if (m_impl) {
    return m_impl->fuelOilNo2Pumps();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo2HeatRejection() const {
  if (m_impl) {
    return m_impl->fuelOilNo2HeatRejection();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo2Humidification() const {
  if (m_impl) {
    return m_impl->fuelOilNo2Humidification();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo2HeatRecovery() const {
  if (m_impl) {
    return m_impl->fuelOilNo2HeatRecovery();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo2WaterSystems() const {
  if (m_impl) {
    return m_impl->fuelOilNo2WaterSystems();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo2Refrigeration() const {
  if (m_impl) {
    return m_impl->fuelOilNo2Refrigeration();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo2Generators() const {
  if (m_impl) {
    return m_impl->fuelOilNo2Generators();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::fuelOilNo2TotalEndUses() const {
  if (m_impl) {
    return m_impl->fuelOilNo2TotalEndUses();
  } else {
    return boost::optional<double>();
  }
}

// Propane
boost::optional<double> SqlFile::propaneHeating() const {
  if (m_impl) {
    return m_impl->propaneHeating();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::propaneCooling() const {
  if (m_impl) {
    return m_impl->propaneCooling();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::propaneInteriorLighting() const {
  if (m_impl) {
    return m_impl->propaneInteriorLighting();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::propaneExteriorLighting() const {
  if (m_impl) {
    return m_impl->propaneExteriorLighting();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::propaneInteriorEquipment() const {
  if (m_impl) {
    return m_impl->propaneInteriorEquipment();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::propaneExteriorEquipment() const {
  if (m_impl) {
    return m_impl->propaneExteriorEquipment();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::propaneFans() const {
  if (m_impl) {
    return m_impl->propaneFans();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::propanePumps() const {
  if (m_impl) {
    return m_impl->propanePumps();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::propaneHeatRejection() const {
  if (m_impl) {
    return m_impl->propaneHeatRejection();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::propaneHumidification() const {
  if (m_impl) {
    return m_impl->propaneHumidification();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::propaneHeatRecovery() const {
  if (m_impl) {
    return m_impl->propaneHeatRecovery();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::propaneWaterSystems() const {
  if (m_impl) {
    return m_impl->propaneWaterSystems();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::propaneRefrigeration() const {
  if (m_impl) {
    return m_impl->propaneRefrigeration();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::propaneGenerators() const {
  if (m_impl) {
    return m_impl->propaneGenerators();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::propaneTotalEndUses() const {
  if (m_impl) {
    return m_impl->propaneTotalEndUses();
  } else {
    return boost::optional<double>();
  }
}

// Other Fuel 1
boost::optional<double> SqlFile::otherFuel1Heating() const {
  if (m_impl) {
    return m_impl->otherFuel1Heating();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel1Cooling() const {
  if (m_impl) {
    return m_impl->otherFuel1Cooling();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel1InteriorLighting() const {
  if (m_impl) {
    return m_impl->otherFuel1InteriorLighting();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel1ExteriorLighting() const {
  if (m_impl) {
    return m_impl->otherFuel1ExteriorLighting();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel1InteriorEquipment() const {
  if (m_impl) {
    return m_impl->otherFuel1InteriorEquipment();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel1ExteriorEquipment() const {
  if (m_impl) {
    return m_impl->otherFuel1ExteriorEquipment();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel1Fans() const {
  if (m_impl) {
    return m_impl->otherFuel1Fans();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel1Pumps() const {
  if (m_impl) {
    return m_impl->otherFuel1Pumps();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel1HeatRejection() const {
  if (m_impl) {
    return m_impl->otherFuel1HeatRejection();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel1Humidification() const {
  if (m_impl) {
    return m_impl->otherFuel1Humidification();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel1HeatRecovery() const {
  if (m_impl) {
    return m_impl->otherFuel1HeatRecovery();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel1WaterSystems() const {
  if (m_impl) {
    return m_impl->otherFuel1WaterSystems();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel1Refrigeration() const {
  if (m_impl) {
    return m_impl->otherFuel1Refrigeration();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel1Generators() const {
  if (m_impl) {
    return m_impl->otherFuel1Generators();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel1TotalEndUses() const {
  if (m_impl) {
    return m_impl->otherFuel1TotalEndUses();
  } else {
    return boost::optional<double>();
  }
}

// Other Fuel 2
boost::optional<double> SqlFile::otherFuel2Heating() const {
  if (m_impl) {
    return m_impl->otherFuel2Heating();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel2Cooling() const {
  if (m_impl) {
    return m_impl->otherFuel2Cooling();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel2InteriorLighting() const {
  if (m_impl) {
    return m_impl->otherFuel2InteriorLighting();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel2ExteriorLighting() const {
  if (m_impl) {
    return m_impl->otherFuel2ExteriorLighting();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel2InteriorEquipment() const {
  if (m_impl) {
    return m_impl->otherFuel2InteriorEquipment();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel2ExteriorEquipment() const {
  if (m_impl) {
    return m_impl->otherFuel2ExteriorEquipment();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel2Fans() const {
  if (m_impl) {
    return m_impl->otherFuel2Fans();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel2Pumps() const {
  if (m_impl) {
    return m_impl->otherFuel2Pumps();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel2HeatRejection() const {
  if (m_impl) {
    return m_impl->otherFuel2HeatRejection();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel2Humidification() const {
  if (m_impl) {
    return m_impl->otherFuel2Humidification();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel2HeatRecovery() const {
  if (m_impl) {
    return m_impl->otherFuel2HeatRecovery();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel2WaterSystems() const {
  if (m_impl) {
    return m_impl->otherFuel2WaterSystems();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel2Refrigeration() const {
  if (m_impl) {
    return m_impl->otherFuel2Refrigeration();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel2Generators() const {
  if (m_impl) {
    return m_impl->otherFuel2Generators();
  } else {
    return boost::optional<double>();
  }
}

boost::optional<double> SqlFile::otherFuel2TotalEndUses() const {
  if (m_impl) {
    return m_impl->otherFuel2TotalEndUses();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtCoolingHeating() const {
  if (m_impl) {
    return m_impl->districtCoolingHeating();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtCoolingCooling() const {
  if (m_impl) {
    return m_impl->districtCoolingCooling();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtCoolingInteriorLighting() const {
  if (m_impl) {
    return m_impl->districtCoolingInteriorLighting();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtCoolingExteriorLighting() const {
  if (m_impl) {
    return m_impl->districtCoolingExteriorLighting();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtCoolingInteriorEquipment() const {
  if (m_impl) {
    return m_impl->districtCoolingInteriorEquipment();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtCoolingExteriorEquipment() const {
  if (m_impl) {
    return m_impl->districtCoolingExteriorEquipment();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtCoolingFans() const {
  if (m_impl) {
    return m_impl->districtCoolingFans();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtCoolingPumps() const {
  if (m_impl) {
    return m_impl->districtCoolingPumps();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtCoolingHeatRejection() const {
  if (m_impl) {
    return m_impl->districtCoolingHeatRejection();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtCoolingHumidification() const {
  if (m_impl) {
    return m_impl->districtCoolingHumidification();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtCoolingHeatRecovery() const {
  if (m_impl) {
    return m_impl->districtCoolingHeatRecovery();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtCoolingWaterSystems() const {
  if (m_impl) {
    return m_impl->districtCoolingWaterSystems();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtCoolingRefrigeration() const {
  if (m_impl) {
    return m_impl->districtCoolingRefrigeration();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtCoolingGenerators() const {
  if (m_impl) {
    return m_impl->districtCoolingGenerators();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtCoolingTotalEndUses() const {
  if (m_impl) {
    return m_impl->districtCoolingTotalEndUses();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtHeatingHeating() const {
  if (m_impl) {
    return m_impl->districtHeatingHeating();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtHeatingCooling() const {
  if (m_impl) {
    return m_impl->districtHeatingCooling();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtHeatingInteriorLighting() const {
  if (m_impl) {
    return m_impl->districtHeatingInteriorLighting();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtHeatingExteriorLighting() const {
  if (m_impl) {
    return m_impl->districtHeatingExteriorLighting();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtHeatingInteriorEquipment() const {
  if (m_impl) {
    return m_impl->districtHeatingInteriorEquipment();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtHeatingExteriorEquipment() const {
  if (m_impl) {
    return m_impl->districtHeatingExteriorEquipment();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtHeatingFans() const {
  if (m_impl) {
    return m_impl->districtHeatingFans();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtHeatingPumps() const {
  if (m_impl) {
    return m_impl->districtHeatingPumps();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtHeatingHeatRejection() const {
  if (m_impl) {
    return m_impl->districtHeatingHeatRejection();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtHeatingHumidification() const {
  if (m_impl) {
    return m_impl->districtHeatingHumidification();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtHeatingHeatRecovery() const {
  if (m_impl) {
    return m_impl->districtHeatingHeatRecovery();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtHeatingWaterSystems() const {
  if (m_impl) {
    return m_impl->districtHeatingWaterSystems();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtHeatingRefrigeration() const {
  if (m_impl) {
    return m_impl->districtHeatingRefrigeration();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtHeatingGenerators() const {
  if (m_impl) {
    return m_impl->districtHeatingGenerators();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::districtHeatingTotalEndUses() const {
  if (m_impl) {
    return m_impl->districtHeatingTotalEndUses();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::waterHeating() const {
  if (m_impl) {
    return m_impl->waterHeating();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::waterCooling() const {
  if (m_impl) {
    return m_impl->waterCooling();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::waterInteriorLighting() const {
  if (m_impl) {
    return m_impl->waterInteriorLighting();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::waterExteriorLighting() const {
  if (m_impl) {
    return m_impl->waterExteriorLighting();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::waterInteriorEquipment() const {
  if (m_impl) {
    return m_impl->waterInteriorEquipment();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::waterExteriorEquipment() const {
  if (m_impl) {
    return m_impl->waterExteriorEquipment();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::waterFans() const {
  if (m_impl) {
    return m_impl->waterFans();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::waterPumps() const {
  if (m_impl) {
    return m_impl->waterPumps();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::waterHeatRejection() const {
  if (m_impl) {
    return m_impl->waterHeatRejection();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::waterHumidification() const {
  if (m_impl) {
    return m_impl->waterHumidification();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::waterHeatRecovery() const {
  if (m_impl) {
    return m_impl->waterHeatRecovery();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::waterWaterSystems() const {
  if (m_impl) {
    return m_impl->waterWaterSystems();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::waterRefrigeration() const {
  if (m_impl) {
    return m_impl->waterRefrigeration();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::waterGenerators() const {
  if (m_impl) {
    return m_impl->waterGenerators();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::waterTotalEndUses() const {
  if (m_impl) {
    return m_impl->waterTotalEndUses();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::hoursHeatingSetpointNotMet() const {
  if (m_impl) {
    return m_impl->hoursHeatingSetpointNotMet();
  } else {
    return {};
  }
}

boost::optional<double> SqlFile::hoursCoolingSetpointNotMet() const {
  if (m_impl) {
    return m_impl->hoursCoolingSetpointNotMet();
  } else {
    return {};
  }
}

std::vector<std::string> SqlFile::availableTimeSeries() {
  std::vector<std::string> result;
  if (m_impl) {
    result = m_impl->availableTimeSeries();
  }
  return result;
}

std::vector<std::string> SqlFile::availableVariableNames(const std::string& envPeriod, const std::string& reportingFrequency) const {
  std::vector<std::string> result;
  if (m_impl) {
    result = m_impl->availableVariableNames(envPeriod, reportingFrequency);
  }
  return result;
}

int SqlFile::insertZone(const std::string& t_name, double t_relNorth, double t_originX, double t_originY, double t_originZ, double t_centroidX,
                        double t_centroidY, double t_centroidZ, int t_ofType, double t_multiplier, double t_listMultiplier, double t_minimumX,
                        double t_maximumX, double t_minimumY, double t_maximumY, double t_minimumZ, double t_maximumZ, double t_ceilingHeight,
                        double t_volume, int t_insideConvectionAlgo, int t_outsideConvectionAlgo, double t_floorArea, double t_extGrossWallArea,
                        double t_extNetWallArea, double t_extWindowArea, bool t_isPartOfTotalArea) {
  return m_impl->insertZone(t_name, t_relNorth, t_originX, t_originY, t_originZ, t_centroidX, t_centroidY, t_centroidZ, t_ofType, t_multiplier,
                            t_listMultiplier, t_minimumX, t_maximumX, t_minimumY, t_maximumY, t_minimumZ, t_maximumZ, t_ceilingHeight, t_volume,
                            t_insideConvectionAlgo, t_outsideConvectionAlgo, t_floorArea, t_extGrossWallArea, t_extNetWallArea, t_extWindowArea,
                            t_isPartOfTotalArea);
}

void SqlFile::insertIlluminanceMap(const std::string& t_zoneName, const std::string& t_name, const std::string& t_environmentName,
                                   const std::vector<DateTime>& t_times, const std::vector<double>& t_xs, const std::vector<double>& t_ys, double t_z,
                                   const std::vector<Matrix>& t_maps) {
  m_impl->insertIlluminanceMap(t_zoneName, t_name, t_environmentName, t_times, t_xs, t_ys, t_z, t_maps);
}

void SqlFile::insertTimeSeriesData(const std::string& t_variableType, const std::string& t_indexGroup, const std::string& t_timestepType,
                                   const std::string& t_keyValue, const std::string& t_variableName,
                                   const openstudio::ReportingFrequency& t_reportingFrequency, const boost::optional<std::string>& t_scheduleName,
                                   const std::string& t_variableUnits, const openstudio::TimeSeries& t_timeSeries) {
  m_impl->insertTimeSeriesData(t_variableType, t_indexGroup, t_timestepType, t_keyValue, t_variableName, t_reportingFrequency, t_scheduleName,
                               t_variableUnits, t_timeSeries);
}

std::vector<std::string> SqlFile::availableReportingFrequencies(const std::string& envPeriod) {
  std::vector<std::string> result;
  if (m_impl) {
    result = m_impl->availableReportingFrequencies(envPeriod);
  }
  return result;
}

boost::optional<openstudio::ReportingFrequency> SqlFile::reportingFrequencyFromDB(const std::string& dbReportingFrequency) {
  boost::optional<openstudio::ReportingFrequency> result;
  if (m_impl) {
    result = m_impl->reportingFrequencyFromDB(dbReportingFrequency);
  }
  return result;
}

boost::optional<EnvironmentType> SqlFile::environmentType(const std::string& envPeriod) const {
  boost::optional<EnvironmentType> result;
  if (m_impl) {
    result = m_impl->environmentType(envPeriod);
  }
  return result;
}

std::vector<std::string> SqlFile::availableEnvPeriods() const {
  std::vector<std::string> result;
  if (m_impl) {
    result = m_impl->availableEnvPeriods();
  }
  return result;
}

std::vector<std::string> SqlFile::availableKeyValues(const std::string& envPeriod, const std::string& reportingFrequency,
                                                     const std::string& timeSeriesName) {
  std::vector<std::string> result;
  if (m_impl) {
    result = m_impl->availableKeyValues(envPeriod, reportingFrequency, timeSeriesName);
  }
  return result;
}

openstudio::TimeSeriesVector SqlFile::timeSeries(const std::string& envPeriod, const std::string& reportingFrequency,
                                                 const std::string& timeSeriesName) {

  openstudio::TimeSeriesVector result;
  if (m_impl) {
    result = m_impl->timeSeries(envPeriod, reportingFrequency, timeSeriesName);
  }
  return result;
}

boost::optional<double> SqlFile::runPeriodValue(const std::string& envPeriod, const std::string& timeSeriesName, const std::string& keyValue) {
  boost::optional<double> result;
  if (m_impl) {
    result = m_impl->runPeriodValue(envPeriod, timeSeriesName, keyValue);
  }
  return result;
}

openstudio::OptionalTimeSeries SqlFile::timeSeries(const std::string& envPeriod, const std::string& reportingFrequency,
                                                   const std::string& timeSeriesName, const std::string& keyValue) {
  openstudio::OptionalTimeSeries result;
  if (m_impl) {
    result = m_impl->timeSeries(envPeriod, reportingFrequency, timeSeriesName, keyValue);
  }
  return result;
}

SqlFileTimeSeriesQueryVector SqlFile::expandQuery(const SqlFileTimeSeriesQuery& query) {
  SqlFileTimeSeriesQueryVector result;
  if (m_impl) {
    result = m_impl->expandQuery(query);
  }
  return result;
}

TimeSeriesVector SqlFile::timeSeries(const SqlFileTimeSeriesQuery& query) {
  TimeSeriesVector result;
  if (m_impl) {
    result = m_impl->timeSeries(query);
  }
  return result;
}

boost::optional<std::pair<DateTime, DateTime>> SqlFile::daylightSavingsPeriod() const {
  boost::optional<std::pair<DateTime, DateTime>> result;
  if (m_impl) {
    result = m_impl->daylightSavingsPeriod();
  }
  return result;
}

/// returns datadictionary of available timeseries
/*
   DataDictionaryTable SqlFile::dataDictionary() const
   {
   DataDictionaryTable result;
   if (m_impl){
   result = m_impl->dataDictionary();
   }
   return result;
   }
   */

/// Energy plus version number
std::string SqlFile::energyPlusVersion() const {
  std::string result;
  if (m_impl) {
    result = m_impl->energyPlusVersion();
  }
  return result;
}

/// Energy Plus eplusout.sql file name
std::string SqlFile::energyPlusSqliteFile() const {
  std::string result;
  if (m_impl) {
    result = m_impl->energyPlusSqliteFile();
  }
  return result;
}

boost::optional<int> SqlFile::illuminanceMapHourlyReportIndex(int mapIndex, const DateTime& dateTime) const {
  if (m_impl) {
    return m_impl->illuminanceMapHourlyReportIndex(mapIndex, dateTime);
  } else {
    return {};
  }
}

/// get zone names of for specified illuminance map
std::vector<std::string> SqlFile::illuminanceMapZoneNames(const std::string& name) const {
  std::vector<std::string> result;
  if (m_impl) {
    result = m_impl->illuminanceMapZoneNames(name);
  }
  return result;
}

std::vector<std::string> SqlFile::illuminanceMapZoneNames(int mapIndex) const {
  std::vector<std::string> result;
  if (m_impl) {
    result = m_impl->illuminanceMapZoneNames(mapIndex);
  }
  return result;
}

/// get names of all available illuminance maps
std::vector<std::string> SqlFile::illuminanceMapNames() const {
  std::vector<std::string> result;
  if (m_impl) {
    result = m_impl->illuminanceMapNames();
  }
  return result;
}

/// get names of all available illuminance maps
std::vector<std::string> SqlFile::illuminanceMapNames(const std::string& envPeriod) const {
  std::vector<std::string> result;
  if (m_impl) {
    result = m_impl->illuminanceMapNames(envPeriod);
  }
  return result;
}

/// x position (m) of the illuminance map
Vector SqlFile::illuminanceMapX(const std::string& name, const DateTime& dateTime) const {
  Vector result;
  if (m_impl) {
    result = m_impl->illuminanceMapX(name, dateTime);
  }
  return result;
}

/// y position (m) of the illuminance map
Vector SqlFile::illuminanceMapY(const std::string& name, const DateTime& dateTime) const {
  Vector result;
  if (m_impl) {
    result = m_impl->illuminanceMapY(name, dateTime);
  }
  return result;
}

/// x position (m) of the illuminance map
Vector SqlFile::illuminanceMapX(int hourlyReportIndex) const {
  Vector result;
  if (m_impl) {
    result = m_impl->illuminanceMapX(hourlyReportIndex);
  }
  return result;
}

/// y position (m) of the illuminance map
Vector SqlFile::illuminanceMapY(int hourlyReportIndex) const {
  Vector result;
  if (m_impl) {
    result = m_impl->illuminanceMapY(hourlyReportIndex);
  }
  return result;
}

/// hourly report indices of the illuminance map
std::vector<int> SqlFile::illuminanceMapHourlyReportIndices(const std::string& name) const {
  std::vector<int> result;
  if (m_impl) {
    result = m_impl->illuminanceMapHourlyReportIndices(name);
  }
  return result;
}

// find the illuminance map index by name
boost::optional<int> SqlFile::illuminanceMapIndex(const std::string& name) const {
  boost::optional<int> result;
  if (m_impl) {
    result = m_impl->illuminanceMapIndex(name);
  }
  return result;
}

/// reference point for map - form RefPtn=(x:y:illuminance)
boost::optional<std::string> SqlFile::illuminanceMapRefPt(const std::string& name, int ptNum) const {
  boost::optional<std::string> result;
  if (m_impl) {
    result = m_impl->illuminanceMapRefPt(name, ptNum);
  }
  return result;
}

boost::optional<std::string> SqlFile::illuminanceMapRefPt(int mapIndex, int ptNum) const {
  boost::optional<std::string> result;
  if (m_impl) {
    result = m_impl->illuminanceMapRefPt(mapIndex, ptNum);
  }
  return result;
}

/// min value for map
boost::optional<double> SqlFile::illuminanceMapMinValue(const std::string& name) const {
  boost::optional<double> result;
  if (m_impl) {
    result = m_impl->illuminanceMapMinValue(name);
  }
  return result;
}

/// min value for map
boost::optional<double> SqlFile::illuminanceMapMinValue(int mapIndex) const {
  boost::optional<double> result;
  if (m_impl) {
    result = m_impl->illuminanceMapMinValue(mapIndex);
  }
  return result;
}

/// max value for map
boost::optional<double> SqlFile::illuminanceMapMaxValue(const std::string& name) const {
  boost::optional<double> result;
  if (m_impl) {
    result = m_impl->illuminanceMapMaxValue(name);
  }
  return result;
}

/// max value for map
boost::optional<double> SqlFile::illuminanceMapMaxValue(int mapIndex) const {
  boost::optional<double> result;
  if (m_impl) {
    result = m_impl->illuminanceMapMaxValue(mapIndex);
  }
  return result;
}

/// min and max values for map
void SqlFile::illuminanceMapMaxValue(const std::string& name, double& minValue, double& maxValue) const {
  if (m_impl) {
    m_impl->illuminanceMapMaxValue(name, minValue, maxValue);
  }
}

/// min and max values for map
void SqlFile::illuminanceMapMaxValue(int mapIndex, double& minValue, double& maxValue) const {
  if (m_impl) {
    m_impl->illuminanceMapMaxValue(mapIndex, minValue, maxValue);
  }
}

/// hourly report indices of the illuminance map
std::vector<int> SqlFile::illuminanceMapHourlyReportIndices(int mapIndex) const {
  std::vector<int> result;
  if (m_impl) {
    result = m_impl->illuminanceMapHourlyReportIndices(mapIndex);
  }
  return result;
}

/// hourly report indices and dates of the illuminance map
std::vector<std::pair<int, DateTime>> SqlFile::illuminanceMapHourlyReportIndicesDates(const std::string& name) const {
  std::vector<std::pair<int, DateTime>> result;
  if (m_impl) {
    result = m_impl->illuminanceMapHourlyReportIndicesDates(name);
  }
  return result;
}

/// hourly report indices and dates of the illuminance map
std::vector<std::pair<int, DateTime>> SqlFile::illuminanceMapHourlyReportIndicesDates(int mapIndex) const {
  std::vector<std::pair<int, DateTime>> result;
  if (m_impl) {
    result = m_impl->illuminanceMapHourlyReportIndicesDates(mapIndex);
  }
  return result;
}

/// date and time for hourly report index
boost::optional<DateTime> SqlFile::illuminanceMapDate(int hourlyReportIndex) const {
  boost::optional<DateTime> result;
  if (m_impl) {
    result = m_impl->illuminanceMapDate(hourlyReportIndex);
  }
  return result;
}

/// value (lux) of the illuminance map at date and time
/// value(i,j) is the illuminance at x(i), y(j)
Matrix SqlFile::illuminanceMap(const std::string& name, const DateTime& dateTime) const {
  Matrix result;
  if (m_impl) {
    result = m_impl->illuminanceMap(name, dateTime);
  }
  return result;
}

/// value (lux) of the illuminance map at hourly report index
/// value(i,j) is the illuminance at x(i), y(j)
Matrix SqlFile::illuminanceMap(int hourlyReportIndex) const {
  Matrix result;
  if (m_impl) {
    result = m_impl->illuminanceMap(hourlyReportIndex);
  }
  return result;
}

std::vector<SummaryData> SqlFile::getSummaryData() const {
  if (m_impl) {
    return m_impl->getSummaryData();
  }
  return {};
}

/** value (lux) of the illuminance map at hourlyReportIndex
 *  value(i,j) is the illuminance at x(i), y(j) fills in x,y, illuminance*/
void SqlFile::illuminanceMap(int hourlyReportIndex, std::vector<double>& x, std::vector<double>& y, std::vector<double>& illuminance) const {
  if (m_impl) {
    m_impl->illuminanceMap(hourlyReportIndex, x, y, illuminance);
  }
}

// equality test
bool SqlFile::operator==(const SqlFile& other) const {
  return (m_impl == other.m_impl);
}

// inequality test
bool SqlFile::operator<(const SqlFile& other) const {
  return (m_impl < other.m_impl);
}

/// returns datadictionary of available timeseries
detail::DataDictionaryTable SqlFile::dataDictionary() const {
  detail::DataDictionaryTable result;
  if (m_impl) {
    result = m_impl->dataDictionary();
  }
  return result;
}

bool SqlFile::supportedVersion() const {
  if (m_impl) {
    return m_impl->isSupportedVersion();
  }

  return false;
}

bool SqlFile::hasYear() const {
  if (m_impl) {
    return m_impl->hasYear();
  }

  return true;
}

bool SqlFile::hasIlluminanceMapYear() const {
  if (m_impl) {
    return m_impl->hasIlluminanceMapYear();
  }

  return true;
}

boost::optional<double> SqlFile::getExteriorFenestrationValue(const std::string& subSurfaceName, const std::string& columnName) const {
  boost::optional<double> result;
  if (m_impl) {
    result = m_impl->getExteriorFenestrationValue(subSurfaceName, columnName);
  }
  return result;
}

boost::optional<double> SqlFile::assemblyUFactor(const std::string& subSurfaceName) const {
  boost::optional<double> result;
  if (m_impl) {
    result = m_impl->assemblyUFactor(subSurfaceName);
  }
  return result;
}

boost::optional<double> SqlFile::assemblySHGC(const std::string& subSurfaceName) const {
  boost::optional<double> result;
  if (m_impl) {
    result = m_impl->assemblySHGC(subSurfaceName);
  }
  return result;
}

boost::optional<double> SqlFile::assemblyVisibleTransmittance(const std::string& subSurfaceName) const {
  boost::optional<double> result;
  if (m_impl) {
    result = m_impl->assemblyVisibleTransmittance(subSurfaceName);
  }
  return result;
}

}  // namespace openstudio
