/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include <gtest/gtest.h>

#include "ModelFixture.hpp"
#include "../Facility.hpp"
#include "../Facility_Impl.hpp"
#include "../OutputMeter.hpp"
#include "../OutputMeter_Impl.hpp"
#include "../Model_Impl.hpp"

#include "../../utilities/sql/SqlFileEnums.hpp"

#include "../../utilities/data/TimeSeries.hpp"
#include "../../utilities/core/Compare.hpp"
#include <utilities/idd/OS_Output_Meter_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include <future>
#include <vector>

using namespace openstudio::model;
using namespace openstudio;
using std::string;

TEST_F(ModelFixture, MeterRegex) {
  // regex to search meter name
  // matches[1], specific end use type
  // matches[2], end use type
  // matches[3], fuel type
  // matches[4], install location type
  // matches[5], install location name

  boost::smatch matches;
  string subject;

  subject = "Facility";
  ASSERT_TRUE(boost::regex_search(subject, matches, OutputMeter::meterRegex()));
  ASSERT_TRUE(matches[1].matched);
  EXPECT_FALSE(matches[2].matched);
  EXPECT_FALSE(matches[3].matched);
  ASSERT_TRUE(matches[4].matched);
  ASSERT_TRUE(matches[5].matched);
  EXPECT_EQ("", string(matches[1].first, matches[1].second));
  EXPECT_EQ("Facility", string(matches[4].first, matches[4].second));
  EXPECT_EQ("", string(matches[5].first, matches[5].second));

  subject = "Electricity";
  ASSERT_TRUE(boost::regex_search(subject, matches, OutputMeter::meterRegex()));
  ASSERT_TRUE(matches[1].matched);
  EXPECT_FALSE(matches[2].matched);
  ASSERT_TRUE(matches[3].matched);
  EXPECT_FALSE(matches[4].matched);
  ASSERT_TRUE(matches[5].matched);
  EXPECT_EQ("", string(matches[1].first, matches[1].second));
  EXPECT_EQ("Electricity", string(matches[3].first, matches[3].second));
  EXPECT_EQ("", string(matches[5].first, matches[5].second));

  subject = "Electricity:Facility";
  ASSERT_TRUE(boost::regex_search(subject, matches, OutputMeter::meterRegex()));
  ASSERT_TRUE(matches[1].matched);
  EXPECT_FALSE(matches[2].matched);
  ASSERT_TRUE(matches[3].matched);
  ASSERT_TRUE(matches[4].matched);
  ASSERT_TRUE(matches[5].matched);
  EXPECT_EQ("", string(matches[1].first, matches[1].second));
  EXPECT_EQ("Electricity", string(matches[3].first, matches[3].second));
  EXPECT_EQ("Facility", string(matches[4].first, matches[4].second));
  EXPECT_EQ("", string(matches[5].first, matches[5].second));

  subject = "NaturalGas:Facility";
  ASSERT_TRUE(boost::regex_search(subject, matches, OutputMeter::meterRegex()));
  ASSERT_TRUE(matches[1].matched);
  EXPECT_FALSE(matches[2].matched);
  ASSERT_TRUE(matches[3].matched);
  ASSERT_TRUE(matches[4].matched);
  ASSERT_TRUE(matches[5].matched);
  EXPECT_EQ("", string(matches[1].first, matches[1].second));
  EXPECT_EQ("NaturalGas", string(matches[3].first, matches[3].second));
  EXPECT_EQ("Facility", string(matches[4].first, matches[4].second));
  EXPECT_EQ("", string(matches[5].first, matches[5].second));

  subject = "EnergyTransfer:Plant";
  ASSERT_TRUE(boost::regex_search(subject, matches, OutputMeter::meterRegex()));
  ASSERT_TRUE(matches[1].matched);
  EXPECT_FALSE(matches[2].matched);
  ASSERT_TRUE(matches[3].matched);
  ASSERT_TRUE(matches[4].matched);
  ASSERT_TRUE(matches[5].matched);
  EXPECT_EQ("", string(matches[1].first, matches[1].second));
  EXPECT_EQ("EnergyTransfer", string(matches[3].first, matches[3].second));
  EXPECT_EQ("Plant", string(matches[4].first, matches[4].second));
  EXPECT_EQ("", string(matches[5].first, matches[5].second));

  subject = "InteriorLights:Electricity";
  ASSERT_TRUE(boost::regex_search(subject, matches, OutputMeter::meterRegex()));
  ASSERT_TRUE(matches[1].matched);
  ASSERT_TRUE(matches[2].matched);
  ASSERT_TRUE(matches[3].matched);
  EXPECT_FALSE(matches[4].matched);
  ASSERT_TRUE(matches[5].matched);
  EXPECT_EQ("", string(matches[1].first, matches[1].second));
  EXPECT_EQ("InteriorLights", string(matches[2].first, matches[2].second));
  EXPECT_EQ("Electricity", string(matches[3].first, matches[3].second));
  EXPECT_EQ("", string(matches[5].first, matches[5].second));

  subject = "InteriorLights:Electricity:Facility";
  ASSERT_TRUE(boost::regex_search(subject, matches, OutputMeter::meterRegex()));
  ASSERT_TRUE(matches[1].matched);
  ASSERT_TRUE(matches[2].matched);
  ASSERT_TRUE(matches[3].matched);
  ASSERT_TRUE(matches[4].matched);
  ASSERT_TRUE(matches[5].matched);
  EXPECT_EQ("", string(matches[1].first, matches[1].second));
  EXPECT_EQ("InteriorLights", string(matches[2].first, matches[2].second));
  EXPECT_EQ("Electricity", string(matches[3].first, matches[3].second));
  EXPECT_EQ("Facility", string(matches[4].first, matches[4].second));
  EXPECT_EQ("", string(matches[5].first, matches[5].second));

  subject = "General:InteriorLights:Electricity:Facility";
  ASSERT_TRUE(boost::regex_search(subject, matches, OutputMeter::meterRegex()));
  ASSERT_TRUE(matches[1].matched);
  ASSERT_TRUE(matches[2].matched);
  ASSERT_TRUE(matches[3].matched);
  ASSERT_TRUE(matches[4].matched);
  ASSERT_TRUE(matches[5].matched);
  EXPECT_EQ("General", string(matches[1].first, matches[1].second));
  EXPECT_EQ("InteriorLights", string(matches[2].first, matches[2].second));
  EXPECT_EQ("Electricity", string(matches[3].first, matches[3].second));
  EXPECT_EQ("Facility", string(matches[4].first, matches[4].second));
  EXPECT_EQ("", string(matches[5].first, matches[5].second));

  subject = "InteriorLights:Electricity:Zone:North Zone";
  ASSERT_TRUE(boost::regex_search(subject, matches, OutputMeter::meterRegex()));
  ASSERT_TRUE(matches[1].matched);
  ASSERT_TRUE(matches[2].matched);
  ASSERT_TRUE(matches[3].matched);
  ASSERT_TRUE(matches[4].matched);
  ASSERT_TRUE(matches[5].matched);
  EXPECT_EQ("", string(matches[1].first, matches[1].second));
  EXPECT_EQ("InteriorLights", string(matches[2].first, matches[2].second));
  EXPECT_EQ("Electricity", string(matches[3].first, matches[3].second));
  EXPECT_EQ("Zone", string(matches[4].first, matches[4].second));
  EXPECT_EQ("North Zone", string(matches[5].first, matches[5].second));

  subject = "General:InteriorLights:Electricity:Zone:North Zone";
  ASSERT_TRUE(boost::regex_search(subject, matches, OutputMeter::meterRegex()));
  ASSERT_TRUE(matches[1].matched);
  ASSERT_TRUE(matches[2].matched);
  ASSERT_TRUE(matches[3].matched);
  ASSERT_TRUE(matches[4].matched);
  ASSERT_TRUE(matches[5].matched);
  EXPECT_EQ("General", string(matches[1].first, matches[1].second));
  EXPECT_EQ("InteriorLights", string(matches[2].first, matches[2].second));
  EXPECT_EQ("Electricity", string(matches[3].first, matches[3].second));
  EXPECT_EQ("Zone", string(matches[4].first, matches[4].second));
  EXPECT_EQ("North Zone", string(matches[5].first, matches[5].second));
}

TEST_F(ModelFixture, MeterConstructor) {
  Model model;

  OutputMeter meter(model);

  EXPECT_FALSE(meter.specificEndUse());
  EXPECT_FALSE(meter.endUseType());
  EXPECT_FALSE(meter.fuelType());
  EXPECT_FALSE(meter.installLocationType());
  EXPECT_FALSE(meter.specificInstallLocation());
  EXPECT_EQ("Hourly", meter.reportingFrequency());
  EXPECT_TRUE(meter.isReportingFrequencyDefaulted());
  EXPECT_FALSE(meter.cumulative());
  EXPECT_TRUE(meter.isCumulativeDefaulted());

  // check order of operations
  // this is a corner case of EnergyPlus, there is no 'Heating:NaturalGas:Facility', it is just 'Heating:NaturalGas'
  meter = OutputMeter(model);
  EXPECT_FALSE(meter.specificEndUse());
  EXPECT_FALSE(meter.endUseType());
  EXPECT_FALSE(meter.fuelType());
  EXPECT_FALSE(meter.installLocationType());
  EXPECT_FALSE(meter.specificInstallLocation());
  EXPECT_TRUE(meter.setFuelType(FuelType::Gas));
  EXPECT_TRUE(meter.setInstallLocationType(InstallLocationType::Facility));
  EXPECT_TRUE(meter.setEndUseType(EndUseType::Heating));
  ASSERT_TRUE(meter.fuelType());
  EXPECT_EQ(FuelType::Gas, meter.fuelType().get().value());
  EXPECT_FALSE(meter.installLocationType());
  ASSERT_TRUE(meter.endUseType());
  EXPECT_EQ(EndUseType::Heating, meter.endUseType().get().value());

  meter = OutputMeter(model);
  EXPECT_FALSE(meter.specificEndUse());
  EXPECT_FALSE(meter.endUseType());
  EXPECT_FALSE(meter.fuelType());
  EXPECT_FALSE(meter.installLocationType());
  EXPECT_FALSE(meter.specificInstallLocation());
  EXPECT_TRUE(meter.setInstallLocationType(InstallLocationType::Facility));
  EXPECT_TRUE(meter.setFuelType(FuelType::Gas));
  EXPECT_TRUE(meter.setEndUseType(EndUseType::Heating));
  ASSERT_TRUE(meter.fuelType());
  EXPECT_EQ(FuelType::Gas, meter.fuelType().get().value());
  EXPECT_FALSE(meter.installLocationType());
  ASSERT_TRUE(meter.endUseType());
  EXPECT_EQ(EndUseType::Heating, meter.endUseType().get().value());

  meter = OutputMeter(model);
  EXPECT_FALSE(meter.specificEndUse());
  EXPECT_FALSE(meter.endUseType());
  EXPECT_FALSE(meter.fuelType());
  EXPECT_FALSE(meter.installLocationType());
  EXPECT_FALSE(meter.specificInstallLocation());
  EXPECT_TRUE(meter.setEndUseType(EndUseType::Heating));
  EXPECT_TRUE(meter.setInstallLocationType(InstallLocationType::Facility));
  EXPECT_TRUE(meter.setFuelType(FuelType::Gas));
  ASSERT_TRUE(meter.fuelType());
  EXPECT_EQ(FuelType::Gas, meter.fuelType().get().value());
  EXPECT_FALSE(meter.installLocationType());
  ASSERT_TRUE(meter.endUseType());
  EXPECT_EQ(EndUseType::Heating, meter.endUseType().get().value());

  // make sure we don't mix up gasoline with gas
  meter = OutputMeter(model);
  EXPECT_FALSE(meter.specificEndUse());
  EXPECT_FALSE(meter.endUseType());
  EXPECT_FALSE(meter.fuelType());
  EXPECT_FALSE(meter.installLocationType());
  EXPECT_FALSE(meter.specificInstallLocation());
  EXPECT_TRUE(meter.setFuelType(FuelType::Gasoline));
  EXPECT_TRUE(meter.setInstallLocationType(InstallLocationType::Facility));
  ASSERT_TRUE(meter.fuelType());
  EXPECT_EQ(FuelType::Gasoline, meter.fuelType().get().value());
  ASSERT_TRUE(meter.installLocationType());
  EXPECT_EQ(InstallLocationType::Facility, meter.installLocationType().get().value());

  // make sure we can get FuelOil1
  meter = OutputMeter(model);
  EXPECT_FALSE(meter.specificEndUse());
  EXPECT_FALSE(meter.endUseType());
  EXPECT_FALSE(meter.fuelType());
  EXPECT_FALSE(meter.installLocationType());
  EXPECT_FALSE(meter.specificInstallLocation());
  EXPECT_TRUE(meter.setFuelType(FuelType::FuelOil_1));
  EXPECT_TRUE(meter.setInstallLocationType(InstallLocationType::Facility));
  ASSERT_TRUE(meter.fuelType());
  EXPECT_EQ(FuelType::FuelOil_1, meter.fuelType().get().value());
  ASSERT_TRUE(meter.installLocationType());
  EXPECT_EQ(InstallLocationType::Facility, meter.installLocationType().get().value());
}

TEST_F(ModelFixture, MeterFromModel) {
  Model model;

  IdfObjectVector idfObjects;

  idfObjects.push_back(IdfObject(IddObjectType::OS_Output_Meter));
  idfObjects.back().setString(OS_Output_MeterFields::Name, "Electricity:Facility");
  idfObjects.back().setString(OS_Output_MeterFields::ReportingFrequency, "monthly");
  idfObjects.back().setString(OS_Output_MeterFields::MeterFileOnly, "false");

  LOG(Debug, "OutputMeter text: " << '\n' << idfObjects.back());

  idfObjects.push_back(IdfObject(IddObjectType::OS_Output_Meter));
  idfObjects.back().setString(OS_Output_MeterFields::Name, "NaturalGas:Building");
  idfObjects.back().setString(OS_Output_MeterFields::ReportingFrequency, "hourly");

  idfObjects.push_back(IdfObject(IddObjectType::OS_Output_Meter));
  idfObjects.back().setString(OS_Output_MeterFields::Name, "InteriorLights:Electricity");
  idfObjects.back().setString(OS_Output_MeterFields::ReportingFrequency, "runperiod");
  idfObjects.back().setString(OS_Output_MeterFields::MeterFileOnly, "false");
  idfObjects.back().setString(OS_Output_MeterFields::Cumulative, "true");

  idfObjects.push_back(IdfObject(IddObjectType::OS_Output_Meter));
  idfObjects.back().setString(OS_Output_MeterFields::Name, "General:InteriorLights:Electricity:Zone:North Zone");
  idfObjects.back().setString(OS_Output_MeterFields::ReportingFrequency, "daily");
  idfObjects.back().setString(OS_Output_MeterFields::Cumulative, "true");

  EXPECT_EQ(static_cast<unsigned>(4), idfObjects.size());

  HandleVector handles = getHandles(model.addObjects(idfObjects));
  ASSERT_EQ(static_cast<unsigned>(4), handles.size());
  EXPECT_EQ(static_cast<unsigned>(4), model.getConcreteModelObjects<OutputMeter>().size());

  //"Output:OutputMeter,Electricity:Facility,monthly;"
  OptionalOutputMeter meter = model.getModelObject<OutputMeter>(handles[0]);
  ASSERT_TRUE(meter);
  EXPECT_EQ("Electricity:Facility", meter->name());
  EXPECT_FALSE(meter->cumulative());
  EXPECT_FALSE(meter->specificEndUse());
  EXPECT_FALSE(meter->endUseType());
  ASSERT_TRUE(meter->fuelType());
  EXPECT_EQ(FuelType::Electricity, meter->fuelType().get().value());
  ASSERT_TRUE(meter->installLocationType());
  EXPECT_EQ(InstallLocationType::Facility, meter->installLocationType().get().value());
  EXPECT_FALSE(meter->specificInstallLocation());

  //"Output:OutputMeter:MeterFileOnly,NaturalGas:Building,hourly;"
  meter = model.getModelObject<OutputMeter>(handles[1]);
  ASSERT_TRUE(meter);
  EXPECT_EQ("NaturalGas:Building", meter->name());
  EXPECT_FALSE(meter->cumulative());
  EXPECT_FALSE(meter->specificEndUse());
  EXPECT_FALSE(meter->endUseType());
  ASSERT_TRUE(meter->fuelType());
  EXPECT_EQ(FuelType::Gas, meter->fuelType().get().value());
  ASSERT_TRUE(meter->installLocationType());
  EXPECT_EQ(InstallLocationType::Building, meter->installLocationType().get().value());
  EXPECT_FALSE(meter->specificInstallLocation());

  //"Output:OutputMeter:Cumulative,InteriorLights:Electricity,runperiod;"
  meter = model.getModelObject<OutputMeter>(handles[2]);
  ASSERT_TRUE(meter);
  EXPECT_EQ("InteriorLights:Electricity", meter->name());
  EXPECT_TRUE(meter->cumulative());
  EXPECT_FALSE(meter->specificEndUse());
  ASSERT_TRUE(meter->endUseType());
  EXPECT_EQ(EndUseType::InteriorLights, meter->endUseType().get().value());
  ASSERT_TRUE(meter->fuelType());
  EXPECT_EQ(FuelType::Electricity, meter->fuelType().get().value());
  EXPECT_FALSE(meter->installLocationType());
  EXPECT_FALSE(meter->specificInstallLocation());

  //"Output:OutputMeter:Cumulative:MeterFileOnly,General:InteriorLights:Electricity:Zone:North Zone,daily;"
  meter = model.getModelObject<OutputMeter>(handles[3]);
  ASSERT_TRUE(meter);
  EXPECT_EQ("General:InteriorLights:Electricity:Zone:North Zone", meter->name());
  EXPECT_TRUE(meter->cumulative());
  ASSERT_TRUE(meter->specificEndUse());
  EXPECT_EQ("General", meter->specificEndUse().get());
  ASSERT_TRUE(meter->endUseType());
  EXPECT_EQ(EndUseType::InteriorLights, meter->endUseType().get().value());
  ASSERT_TRUE(meter->fuelType());
  EXPECT_EQ(FuelType::Electricity, meter->fuelType().get().value());
  ASSERT_TRUE(meter->installLocationType());
  EXPECT_EQ(InstallLocationType::Zone, meter->installLocationType().get().value());
  ASSERT_TRUE(meter->specificInstallLocation());
  EXPECT_EQ("North Zone", meter->specificInstallLocation().get());
}

TEST_F(ModelFixture, MeterEnumValues) {
  Model model;

  const std::set<int> installLocationTypes = InstallLocationType::getValues();
  for (const InstallLocationType installLocationType : installLocationTypes) {
    OutputMeter meter(model);
    EXPECT_TRUE(meter.setInstallLocationType(installLocationType)) << installLocationType.valueName();
    ASSERT_TRUE(meter.installLocationType()) << installLocationType.valueName();
    EXPECT_EQ(installLocationType, meter.installLocationType().get().value())
      << installLocationType.valueName() << " != " << meter.installLocationType().get().valueName();
  }

  std::set<int> fuelTypes = FuelType::getValues();
  for (const FuelType fuelType : fuelTypes) {
    OutputMeter meter(model);
    if (fuelType == FuelType::Geothermal || fuelType == FuelType::Solar) {
      EXPECT_ANY_THROW(meter.setFuelType(fuelType));
      continue;
    }
    EXPECT_TRUE(meter.setFuelType(fuelType)) << fuelType.valueName();
    ASSERT_TRUE(meter.fuelType()) << fuelType.valueName();
    EXPECT_EQ(fuelType, meter.fuelType().get().value()) << fuelType.valueName() << " != " << meter.fuelType().get().valueName();
  }

  fuelTypes.erase(FuelType::Geothermal);
  fuelTypes.erase(FuelType::Solar);

  const std::set<int> endUseTypes = EndUseType::getValues();
  for (const EndUseType endUseType : endUseTypes) {
    OutputMeter meter(model);
    EXPECT_TRUE(meter.setEndUseType(endUseType)) << endUseType.valueName();
    ASSERT_TRUE(meter.endUseType()) << endUseType.valueName();
    EXPECT_EQ(endUseType, meter.endUseType().get().value()) << endUseType.valueName() << " != " << meter.endUseType().get().valueName();
  }

  for (const InstallLocationType installLocationType : installLocationTypes) {
    for (const FuelType fuelType : fuelTypes) {
      for (const EndUseType endUseType : endUseTypes) {
        OutputMeter meter(model);
        EXPECT_TRUE(meter.setInstallLocationType(installLocationType)) << installLocationType.valueName();
        EXPECT_TRUE(meter.setFuelType(fuelType)) << fuelType.valueName();
        EXPECT_TRUE(meter.setEndUseType(endUseType)) << endUseType.valueName();

        // this is a specific case handled by OutputMeter
        if (installLocationType != InstallLocationType::Facility) {
          ASSERT_TRUE(meter.installLocationType()) << installLocationType.valueName();
          EXPECT_EQ(installLocationType, meter.installLocationType().get().value())
            << installLocationType.valueName() << " != " << meter.installLocationType().get().valueName();
        }

        ASSERT_TRUE(meter.fuelType()) << fuelType.valueName();
        EXPECT_EQ(fuelType, meter.fuelType().get().value()) << fuelType.valueName() << " != " << meter.fuelType().get().valueName();

        ASSERT_TRUE(meter.endUseType()) << endUseType.valueName();
        EXPECT_EQ(endUseType, meter.endUseType().get().value()) << endUseType.valueName() << " != " << meter.endUseType().get().valueName();
      }
    }
  }
}

void get_meter_regex() {
  std::this_thread::yield();
  std::string subject = "Electricity:Facility";
  boost::smatch matches;
  boost::regex_search(subject, matches, OutputMeter::meterRegex());
}

TEST_F(ModelFixture, GetMeterRegex_ThreadSafe) {
  const unsigned N = 50;
  std::vector<std::future<void>> workers;
  for (unsigned i = 0; i < N; ++i) {
    workers.push_back(std::async(std::launch::async, get_meter_regex));
  }

  for (auto& f : workers) {
    EXPECT_TRUE(f.valid());
  }

  for (auto& f : workers) {
    f.get();
  }
}
