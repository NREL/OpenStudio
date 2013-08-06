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

#include <gtest/gtest.h>

#include <model/test/ModelFixture.hpp>
#include <model/Facility.hpp>
#include <model/Facility_Impl.hpp>
#include <model/Meter.hpp>
#include <model/Meter_Impl.hpp>
#include <model/Model_Impl.hpp>

#include <utilities/sql/SqlFileEnums.hpp>

#include <utilities/data/TimeSeries.hpp>
#include <utilities/core/Compare.hpp>
#include <utilities/idd/OS_Meter_FieldEnums.hxx>

#include <boost/foreach.hpp>

using namespace openstudio::model;
using namespace openstudio;
using std::string;

TEST_F(ModelFixture, MeterRegex)
{
  // regex to search meter name
  // matches[1], specific end use type
  // matches[2], end use type
  // matches[3], fuel type
  // matches[4], install location type
  // matches[5], install location name

  boost::smatch matches;
  string subject;

  subject = "Facility";
  ASSERT_TRUE(boost::regex_search(subject, matches, Meter::meterRegex()));
  ASSERT_TRUE(matches[1].matched);
  EXPECT_FALSE(matches[2].matched);
  EXPECT_FALSE(matches[3].matched);
  ASSERT_TRUE(matches[4].matched);
  ASSERT_TRUE(matches[5].matched);
  EXPECT_EQ("", string(matches[1].first, matches[1].second));
  EXPECT_EQ("Facility", string(matches[4].first, matches[4].second));
  EXPECT_EQ("", string(matches[5].first, matches[5].second));

  subject = "Electricity";
  ASSERT_TRUE(boost::regex_search(subject, matches, Meter::meterRegex()));
  ASSERT_TRUE(matches[1].matched);
  EXPECT_FALSE(matches[2].matched);
  ASSERT_TRUE(matches[3].matched);
  EXPECT_FALSE(matches[4].matched);
  ASSERT_TRUE(matches[5].matched);
  EXPECT_EQ("", string(matches[1].first, matches[1].second));
  EXPECT_EQ("Electricity", string(matches[3].first, matches[3].second));
  EXPECT_EQ("", string(matches[5].first, matches[5].second));

  subject = "Electricity:Facility";
  ASSERT_TRUE(boost::regex_search(subject, matches, Meter::meterRegex()));
  ASSERT_TRUE(matches[1].matched);
  EXPECT_FALSE(matches[2].matched);
  ASSERT_TRUE(matches[3].matched);
  ASSERT_TRUE(matches[4].matched);
  ASSERT_TRUE(matches[5].matched);
  EXPECT_EQ("", string(matches[1].first, matches[1].second));
  EXPECT_EQ("Electricity", string(matches[3].first, matches[3].second));
  EXPECT_EQ("Facility", string(matches[4].first, matches[4].second));
  EXPECT_EQ("", string(matches[5].first, matches[5].second));

  subject = "Gas:Facility";
  ASSERT_TRUE(boost::regex_search(subject, matches, Meter::meterRegex()));
  ASSERT_TRUE(matches[1].matched);
  EXPECT_FALSE(matches[2].matched);
  ASSERT_TRUE(matches[3].matched);
  ASSERT_TRUE(matches[4].matched);
  ASSERT_TRUE(matches[5].matched);
  EXPECT_EQ("", string(matches[1].first, matches[1].second));
  EXPECT_EQ("Gas", string(matches[3].first, matches[3].second));
  EXPECT_EQ("Facility", string(matches[4].first, matches[4].second));
  EXPECT_EQ("", string(matches[5].first, matches[5].second));

  subject = "EnergyTransfer:Plant";
  ASSERT_TRUE(boost::regex_search(subject, matches, Meter::meterRegex()));
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
  ASSERT_TRUE(boost::regex_search(subject, matches, Meter::meterRegex()));
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
  ASSERT_TRUE(boost::regex_search(subject, matches, Meter::meterRegex()));
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
  ASSERT_TRUE(boost::regex_search(subject, matches, Meter::meterRegex()));
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
  ASSERT_TRUE(boost::regex_search(subject, matches, Meter::meterRegex()));
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
  ASSERT_TRUE(boost::regex_search(subject, matches, Meter::meterRegex()));
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

TEST_F(ModelFixture, MeterConstructor)
{
  Model model;

  Meter meter(model);

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
  // this is a corner case of EnergyPlus, there is no 'Heating:Gas:Facility', it is just 'Heating:Gas'
  meter = Meter(model);
  EXPECT_TRUE(meter.setFuelType(FuelType::Gas));
  EXPECT_TRUE(meter.setInstallLocationType(InstallLocationType::Facility));
  EXPECT_TRUE(meter.setEndUseType(EndUseType::Heating));
  ASSERT_TRUE(meter.fuelType());
  EXPECT_EQ(FuelType::Gas, meter.fuelType().get().value());
  EXPECT_FALSE(meter.installLocationType());
  ASSERT_TRUE(meter.endUseType());
  EXPECT_EQ(EndUseType::Heating, meter.endUseType().get().value());

  meter = Meter(model);
  EXPECT_TRUE(meter.setInstallLocationType(InstallLocationType::Facility));
  EXPECT_TRUE(meter.setFuelType(FuelType::Gas));
  EXPECT_TRUE(meter.setEndUseType(EndUseType::Heating));
  ASSERT_TRUE(meter.fuelType());
  EXPECT_EQ(FuelType::Gas, meter.fuelType().get().value());
  EXPECT_FALSE(meter.installLocationType());
  ASSERT_TRUE(meter.endUseType());
  EXPECT_EQ(EndUseType::Heating, meter.endUseType().get().value());

  meter = Meter(model);
  EXPECT_TRUE(meter.setEndUseType(EndUseType::Heating));
  EXPECT_TRUE(meter.setInstallLocationType(InstallLocationType::Facility));
  EXPECT_TRUE(meter.setFuelType(FuelType::Gas));
  ASSERT_TRUE(meter.fuelType());
  EXPECT_EQ(FuelType::Gas, meter.fuelType().get().value());
  EXPECT_FALSE(meter.installLocationType());
  ASSERT_TRUE(meter.endUseType());
  EXPECT_EQ(EndUseType::Heating, meter.endUseType().get().value());

  // make sure we don't mix up gasoline with gas
  meter = Meter(model);
  EXPECT_TRUE(meter.setFuelType(FuelType::Gasoline));
  EXPECT_TRUE(meter.setInstallLocationType(InstallLocationType::Facility));
  ASSERT_TRUE(meter.fuelType());
  EXPECT_EQ(FuelType::Gasoline, meter.fuelType().get().value());
  ASSERT_TRUE(meter.installLocationType());
  EXPECT_EQ(InstallLocationType::Facility, meter.installLocationType().get().value());

  // make sure we can get FuelOil1
  meter = Meter(model);
  EXPECT_TRUE(meter.setFuelType(FuelType::FuelOil_1));
  EXPECT_TRUE(meter.setInstallLocationType(InstallLocationType::Facility));
  ASSERT_TRUE(meter.fuelType());
  EXPECT_EQ(FuelType::FuelOil_1, meter.fuelType().get().value());
  ASSERT_TRUE(meter.installLocationType());
  EXPECT_EQ(InstallLocationType::Facility, meter.installLocationType().get().value());
}


TEST_F(ModelFixture, MeterFromModel)
{
  Model model;

  IdfObjectVector idfObjects;

  idfObjects.push_back(IdfObject(IddObjectType::OS_Meter));
  idfObjects.back().setString(OS_MeterFields::Name,"Electricity:Facility");
  idfObjects.back().setString(OS_MeterFields::ReportingFrequency,"monthly");
  idfObjects.back().setString(OS_MeterFields::MeterFileOnly,"false");

  LOG(Debug,"Meter text: " << std::endl << idfObjects.back());

  idfObjects.push_back(IdfObject(IddObjectType::OS_Meter));
  idfObjects.back().setString(OS_MeterFields::Name,"Gas:Building");
  idfObjects.back().setString(OS_MeterFields::ReportingFrequency,"hourly");

  idfObjects.push_back(IdfObject(IddObjectType::OS_Meter));
  idfObjects.back().setString(OS_MeterFields::Name,"InteriorLights:Electricity");
  idfObjects.back().setString(OS_MeterFields::ReportingFrequency,"runperiod");
  idfObjects.back().setString(OS_MeterFields::MeterFileOnly,"false");
  idfObjects.back().setString(OS_MeterFields::Cumulative,"true");

  idfObjects.push_back(IdfObject(IddObjectType::OS_Meter));
  idfObjects.back().setString(OS_MeterFields::Name,"General:InteriorLights:Electricity:Zone:North Zone");
  idfObjects.back().setString(OS_MeterFields::ReportingFrequency,"daily");
  idfObjects.back().setString(OS_MeterFields::Cumulative,"true");

  EXPECT_EQ(static_cast<unsigned>(4), idfObjects.size());

  HandleVector handles = getHandles(model.addObjects(idfObjects));
  ASSERT_EQ(static_cast<unsigned>(4), handles.size());
  EXPECT_EQ(static_cast<unsigned>(4), model.getModelObjects<Meter>().size());

  //"Output:Meter,Electricity:Facility,monthly;"
  OptionalMeter meter = model.getModelObject<Meter>(handles[0]);
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

  //"Output:Meter:MeterFileOnly,Gas:Building,hourly;"
  meter = model.getModelObject<Meter>(handles[1]);
  ASSERT_TRUE(meter);
  EXPECT_EQ("Gas:Building", meter->name());
  EXPECT_FALSE(meter->cumulative());
  EXPECT_FALSE(meter->specificEndUse());
  EXPECT_FALSE(meter->endUseType());
  ASSERT_TRUE(meter->fuelType());
  EXPECT_EQ(FuelType::Gas, meter->fuelType().get().value());
  ASSERT_TRUE(meter->installLocationType());
  EXPECT_EQ(InstallLocationType::Building, meter->installLocationType().get().value());
  EXPECT_FALSE(meter->specificInstallLocation());

  //"Output:Meter:Cumulative,InteriorLights:Electricity,runperiod;"
  meter = model.getModelObject<Meter>(handles[2]);
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

  //"Output:Meter:Cumulative:MeterFileOnly,General:InteriorLights:Electricity:Zone:North Zone,daily;"
  meter = model.getModelObject<Meter>(handles[3]);
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

TEST_F(ModelFixture, MeterEnumValues)
{
  Model model;
    
  std::set<int> installLocationTypes = InstallLocationType::getValues();
  BOOST_FOREACH(int installLocationType, installLocationTypes){
    Meter meter(model);
    EXPECT_TRUE(meter.setInstallLocationType(InstallLocationType(installLocationType))) << InstallLocationType(installLocationType).valueName();
    ASSERT_TRUE(meter.installLocationType()) << InstallLocationType(installLocationType).valueName();
    EXPECT_EQ(installLocationType, meter.installLocationType().get().value()) << InstallLocationType(installLocationType).valueName() << " != " << meter.installLocationType().get().valueName();
  }

  std::set<int> fuelTypes = FuelType::getValues();
  BOOST_FOREACH(int fuelType, fuelTypes){
    Meter meter(model);
    EXPECT_TRUE(meter.setFuelType(FuelType(fuelType))) << FuelType(fuelType).valueName();
    ASSERT_TRUE(meter.fuelType()) << FuelType(fuelType).valueName();
    EXPECT_EQ(fuelType, meter.fuelType().get().value()) << FuelType(fuelType).valueName() << " != " << meter.fuelType().get().valueName();
  }

  std::set<int> endUseTypes = EndUseType::getValues();
  BOOST_FOREACH(int endUseType, endUseTypes){
    Meter meter(model);
    EXPECT_TRUE(meter.setEndUseType(EndUseType(endUseType))) << EndUseType(endUseType).valueName();
    ASSERT_TRUE(meter.endUseType()) << EndUseType(endUseType).valueName();
    EXPECT_EQ(endUseType, meter.endUseType().get().value()) << EndUseType(endUseType).valueName() << " != " << meter.endUseType().get().valueName();
  }

  BOOST_FOREACH(int installLocationType, installLocationTypes){
    BOOST_FOREACH(int fuelType, fuelTypes){
      BOOST_FOREACH(int endUseType, endUseTypes){
        Meter meter(model);
        EXPECT_TRUE(meter.setInstallLocationType(InstallLocationType(installLocationType))) << InstallLocationType(installLocationType).valueName();
        EXPECT_TRUE(meter.setFuelType(FuelType(fuelType))) << FuelType(fuelType).valueName();
        EXPECT_TRUE(meter.setEndUseType(EndUseType(endUseType))) << EndUseType(endUseType).valueName();

        // this is a specific case handled by Meter
        if (installLocationType != InstallLocationType::Facility){
          ASSERT_TRUE(meter.installLocationType()) << InstallLocationType(installLocationType).valueName();
          EXPECT_EQ(installLocationType, meter.installLocationType().get().value()) << InstallLocationType(installLocationType).valueName() << " != " << meter.installLocationType().get().valueName();
        }

        ASSERT_TRUE(meter.fuelType()) << FuelType(fuelType).valueName();
        EXPECT_EQ(fuelType, meter.fuelType().get().value()) << FuelType(fuelType).valueName() << " != " << meter.fuelType().get().valueName();

        ASSERT_TRUE(meter.endUseType()) << EndUseType(endUseType).valueName();
        EXPECT_EQ(endUseType, meter.endUseType().get().value()) << EndUseType(endUseType).valueName() << " != " << meter.endUseType().get().valueName();
      }
    }
  }
}