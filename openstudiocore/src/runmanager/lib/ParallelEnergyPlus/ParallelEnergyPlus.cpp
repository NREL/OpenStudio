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

#include "ParallelEnergyPlus.hpp"
//#include "Building.hpp"

#include <fstream>

#include <iostream>
#include <sstream>
#include <iomanip>

#include <string>
#include <vector>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/iter_find.hpp>

#include "../../../utilities/idf/IdfFile.hpp"
#include "../../../utilities/idf/IdfObject.hpp"
#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/RunPeriod_FieldEnums.hxx>

#include "../../../energyplus/ReverseTranslator.hpp"

ParallelEnergyPlus::ParallelEnergyPlus(const openstudio::path &t_idf, int t_numPartitions, int t_offsetInDays)
  : m_idfPath(t_idf), m_numPartitions(t_numPartitions), m_offset(t_offsetInDays), m_runPeriod(getRunPeriod(t_idf)), 
    m_partitions(createPartitions(m_runPeriod.second, m_offset, m_numPartitions))
{
}


ParallelEnergyPlus::~ParallelEnergyPlus()
{
}

std::pair<openstudio::Workspace, openstudio::WorkspaceObject> ParallelEnergyPlus::getRunPeriod(const openstudio::path &t_idfPath)
{

  boost::optional<openstudio::IdfFile> idfFile = openstudio::IdfFile::load(t_idfPath, openstudio::IddFileType::EnergyPlus);

  if (idfFile)
  {
    openstudio::Workspace workspace(*idfFile);
    std::vector<openstudio::WorkspaceObject> runPeriods = workspace.getObjectsByType(openstudio::IddObjectType::RunPeriod);

    if (runPeriods.size() != 1){
      throw std::runtime_error("Exactly one runperiod is required, " + boost::lexical_cast<std::string>(runPeriods.size()) + " found");
    } else {
      return std::make_pair(workspace, runPeriods[0]);
    }

  } else {
    throw std::runtime_error("Could not load IdfFile: " + openstudio::toString(t_idfPath));
  }


}

void ParallelEnergyPlus::getRunPeriod(const openstudio::WorkspaceObject &t_runPeriod, boost::gregorian::date &startdate, boost::gregorian::date &enddate)
{

  boost::optional<int> bm = t_runPeriod.getInt(openstudio::RunPeriodFields::BeginMonth);
  boost::optional<int> bd = t_runPeriod.getInt(openstudio::RunPeriodFields::BeginDayofMonth);
  boost::optional<int> em = t_runPeriod.getInt(openstudio::RunPeriodFields::EndMonth);
  boost::optional<int> ed = t_runPeriod.getInt(openstudio::RunPeriodFields::EndDayofMonth);

  if (!bm || !bd || !em || !ed)
  {
    throw std::runtime_error("Error reading dates from runperiod");
  }

  LOG(Debug, "Beginning Month: " << *bm << " beginDay: " << *bd);
  startdate = boost::gregorian::date(2010, *bm, *bd);

  LOG(Debug, "Ending Month: " << *em << " endDay: " << *ed);
  enddate = boost::gregorian::date(2010, *em, *ed);
}


void ParallelEnergyPlus::computeLinearCombination(int &A, int &B, int &X,
    int &Y, double &t_averageDays, const double daysPerPeriod, const double totalDays, const int t_numPartitions)
{
  if (totalDays < t_numPartitions)
  {
    throw std::runtime_error("cannot have more partitions than days");
  }

  B = ceil(daysPerPeriod);
  A = floor(daysPerPeriod);

  if (B > A)
  {
    Y = (totalDays - A * t_numPartitions) / (B - A);
  }
  else
  {
    Y = t_numPartitions / 2;
  }

  X = t_numPartitions - Y;

  t_averageDays = (X * A + Y * B) / static_cast<double> (X + Y);
}

void ParallelEnergyPlus::createPartitions(boost::gregorian::date &d1, const boost::gregorian::date &ed,
    const int A, const int startVal, const int endVal, const int t_offset, const boost::gregorian::date &t_startDate, 
    std::vector<std::pair<boost::gregorian::date, boost::gregorian::date> > &t_partitions)
{
  boost::gregorian::date_duration increment(A - 1);
  boost::gregorian::date_duration offset(t_offset);

  for (int i = startVal; i < endVal; ++i)
  {
    boost::gregorian::date d2(d1 + increment);
    boost::gregorian::date ds(d1);
    ds = ds - offset;
    if (ds < t_startDate)
    {
      ds = t_startDate;
    }

    if (d2 > ed)
    {
      d2 = ed;
    }

    //boost::gregorian::date_duration dur = d2 - ds;
    t_partitions[i] = std::make_pair(ds, d2);
    d1 = d2 + boost::gregorian::date_duration(1);
  }
}

// Break this up into smaller bits
std::vector<std::pair<boost::gregorian::date, boost::gregorian::date> > ParallelEnergyPlus::createPartitions(const openstudio::WorkspaceObject &t_runPeriod, int t_offset, int t_numPartitions)
{

  boost::gregorian::date sd, ed;
  getRunPeriod(t_runPeriod, sd, ed);

  boost::gregorian::date_duration totalduration = ed - sd;
  double totalDays = totalduration.days() + 1;

  LOG(Debug, "t_offset " << t_offset << " t_numPartitions " << t_numPartitions << " totalDays " << totalDays);

  std::vector<std::pair<boost::gregorian::date, boost::gregorian::date> > partitions(t_numPartitions);

  // Compute partitions
  double daysPerPeriod = totalDays / t_numPartitions;

  if (daysPerPeriod-1 < t_offset)
  {
    throw std::runtime_error("The days per period is too small compared to the offset, they fully overlap");
  }

  int X, Y, A, B;
  double averageDays;
  computeLinearCombination(A, B, X, Y, averageDays, daysPerPeriod, totalDays, t_numPartitions);

  // So... x processors get A days and y processors get B days
  // Start with ranks 0... x
  boost::gregorian::date d1(sd);

  //std::cout << boost::gregorian::to_simple_string(d1) << std::endl;
  createPartitions(d1, ed, A, 0, X, t_offset, sd, partitions);

  //std::cout << boost::gregorian::to_simple_string(d1) << std::endl;
  createPartitions(d1, ed, B, X, t_numPartitions, t_offset, sd, partitions);

  return partitions;
}


void ParallelEnergyPlus::writePartition(int t_partition, const openstudio::path &t_path) const
{
  openstudio::WorkspaceObject wo = m_runPeriod.second;
  openstudio::Workspace ws = m_runPeriod.first;

  wo.setInt(openstudio::RunPeriodFields::BeginMonth, m_partitions[t_partition].first.month().as_number());
  wo.setInt(openstudio::RunPeriodFields::BeginDayofMonth, m_partitions[t_partition].first.day().as_number());
  wo.setInt(openstudio::RunPeriodFields::EndMonth, m_partitions[t_partition].second.month().as_number());
  wo.setInt(openstudio::RunPeriodFields::EndDayofMonth, m_partitions[t_partition].second.day().as_number());

  boost::gregorian::greg_weekday wd = m_partitions[t_partition].first.day_of_week();

  wo.setString(openstudio::RunPeriodFields::DayofWeekforStartDay, wd.as_long_string());

  // make sure at least some meters exist
  if (ws.getObjectsByType(openstudio::IddObjectType::Output_Meter).size() == 0)
  {
    ws.insertObject(*openstudio::IdfObject::load("Output:Meter,Electricity:Facility,HOURLY"));
    ws.insertObject(*openstudio::IdfObject::load("Output:Meter,Gas:Facility,HOURLY"));
    ws.insertObject(*openstudio::IdfObject::load("Output:Meter,Gasoline:Facility,HOURLY"));
    ws.insertObject(*openstudio::IdfObject::load("Output:Meter,Diesel:Facility,HOURLY"));
    ws.insertObject(*openstudio::IdfObject::load("Output:Meter,Propane:Facility,HOURLY"));
    ws.insertObject(*openstudio::IdfObject::load("Output:Meter,FuelOil_1:Facility,HOURLY"));
    ws.insertObject(*openstudio::IdfObject::load("Output:Meter,FuelOil_2:Facility,HOURLY"));
    ws.insertObject(*openstudio::IdfObject::load("Output:Meter,DistrictHeating:Facility,HOURLY"));
    ws.insertObject(*openstudio::IdfObject::load("Output:Meter,DistrictCooling:Facility,HOURLY"));
//    ws.insertObject(*openstudio::IdfObject::load("Output:Meter,EnergyTransfer:Facility,HOURLY"));
    ws.insertObject(*openstudio::IdfObject::load("Output:Meter,Steam:Facility,HOURLY"));
    ws.insertObject(*openstudio::IdfObject::load("Output:Meter,Water:Facility,HOURLY"));
  } 

  // and that sqlite output is enabled
  if (ws.getObjectsByType(openstudio::IddObjectType::Output_SQLite).size() == 0)
  {
    ws.insertObject(*openstudio::IdfObject::load("Output:SQLite, Simple;"));
  }

  // And that SQL output is enabled

  ws.toIdfFile().save(t_path, true);

  //building.remove("output");
  //building.update("simulationcontrol,yes,yes,yes, no, yes;");
  //building.update("simulationcontrol, yes, yes, no, no, yes;");
  //building.update(createRunPeriod(m_runPeriod.second, m_partitions[t_partition].first, m_partitions[t_partition].second));



/*
  // remove summary reports since they dont make sense anymore //-BLB
  building.remove("Output:Table:");
  //building.remove("Output:meter:cumulative"); //-BLB  doesnt work, removes all output:meter
  building.remove("Output:environmentalimpactfactors");
  building.remove2("Output:VariableDictionary,regular");
  building.remove2("Output:VariableDictionary,IDF");
  building.remove("Output:Surfaces");
  building.remove("Output:Constructions");
  building.remove("Output:DaylightingFactors");
  building.remove("Output:Schedules");
  building.remove("Output:EnergyManagementSystem");


  // make sure SQLite is setup properly
  building.remove("Output:SQLite");
  // use add2 to insert sorted by first two names.
  building.add2("Output:SQLite, Simple;");

  // Add ABUPS points to idf  //-BLB
  // use add2 to insert by first two names, ex Output:Meter,Heating:Electricity and not Output:Meter,
  building.remove2("Output:Meter,Heating:Electricity,HOURLY;");
  building.add2("Output:Meter,Heating:Electricity,HOURLY;");
  building.remove2("Output:Meter,Heating:Gas,HOURLY;");
  building.add2("Output:Meter,Heating:Gas,HOURLY;");
  building.remove2("Output:Meter,Heating:DistrictHeating,HOURLY;");
  building.add2("Output:Meter,Heating:DistrictHeating,HOURLY;");

  building.remove2("Output:Meter,Cooling:Electricity,HOURLY;");
  building.add2("Output:Meter,Cooling:Electricity,HOURLY;");
  building.remove2("Output:Meter,Cooling:Gas,HOURLY;");
  building.add2("Output:Meter,Cooling:Gas,HOURLY;");
  building.remove2("Output:Meter,Cooling:DistrictCooling,HOURLY;");
  building.add2("Output:Meter,Cooling:DistrictCooling,HOURLY;");

  building.remove2("Output:Meter,InteriorLights:Electricity,HOURLY;");
  building.add2("Output:Meter,InteriorLights:Electricity,HOURLY;");

  building.remove2("Output:Meter,ExteriorLights:Electricity,hourly;");
  building.add2("Output:Meter,ExteriorLights:Electricity,hourly;");

  building.remove2("Output:Meter,InteriorEquipment:Electricity,HOURLY;");
  building.add2("Output:Meter,InteriorEquipment:Electricity,HOURLY;");
  building.remove2("Output:Meter,InteriorEquipment:Gas,HOURLY;");
  building.add2("Output:Meter,InteriorEquipment:Gas,HOURLY;");

  building.remove2("Output:Meter,ExteriorEquipment:Electricity,HOURLY;");
  building.add2("Output:Meter,ExteriorEquipment:Electricity,HOURLY;");
  building.remove2("Output:Meter,ExteriorEquipment:Gas,HOURLY;");
  building.add2("Output:Meter,ExteriorEquipment:Gas,HOURLY;");

  building.remove2("Output:Meter,Fans:Electricity,HOURLY;");
  building.add2("Output:Meter,Fans:Electricity,HOURLY;");

  building.remove2("Output:Meter,Pumps:Electricity,hourly;");
  building.add2("Output:Meter,Pumps:Electricity,hourly;");

  building.remove2("Output:Meter,HeatRejection:Electricity,hourly;");
  building.add2("Output:Meter,HeatRejection:Electricity,hourly;");
  building.remove2("Output:Meter,HeatRejection:EnergyTransfer,hourly;");
  building.add2("Output:Meter,HeatRejection:EnergyTransfer,hourly;");
  building.remove2("Output:Meter,HeatRejection:Water,hourly;");
  building.add2("Output:Meter,HeatRejection:Water,hourly;");

  building.remove2("Output:Meter,Humidifier:Electricity,hourly;");
  building.add2("Output:Meter,Humidifier:Electricity,hourly;");
  building.remove2("Output:Meter,Humidifier:Water,hourly;");
  building.add2("Output:Meter,Humidifier:Water,hourly;");

  building.remove2("Output:Meter,HeatRecovery:Electricity,hourly;");
  building.add2("Output:Meter,HeatRecovery:Electricity,hourly;");

  building.remove2("Output:Meter,WaterSystems:Electricity,hourly;");
  building.add2("Output:Meter,WaterSystems:Electricity,hourly;");
  building.remove2("Output:Meter,WaterSystems:Gas,hourly;");
  building.add2("Output:Meter,WaterSystems:Gas,hourly;");
  building.remove2("Output:Meter,WaterSystems:Water,hourly;");
  building.add2("Output:Meter,WaterSystems:Water,hourly;");

  building.remove2("Output:Meter,Refrigeration:Electricity,hourly;");
  building.add2("Output:Meter,Refrigeration:Electricity,hourly;");

  building.remove2("Output:Meter,Cogeneration:Electricity,hourly;");
  building.add2("Output:Meter,Cogeneration:Electricity,hourly;");
  building.remove2("Output:Meter,Cogeneration:Gas,hourly;");
  building.add2("Output:Meter,Cogeneration:Gas,hourly;");


  //building.add("Output:SQLite, Simple;");
  //building.add("Output:Meter, Electricity:Facility,  Daily;");
  //building.add("Output:Meter, Gas:Facility,  Daily;");

  //building.add("Output:Meter, Electricity:Facility,  hourly;");
  //building.add("Output:Meter, Gas:Facility,  hourly;");
  //building.add("Output:Meter, Electricity:Building,  hourly;");
  //building.add("Output:Meter, Electricity:HVAC,  hourly;");
  //building.add("Output:Variable,*,Zone Mean Air Temperature,hourly;");
  //building.add("Output:Variable,*,Cooling Coil Electric Power,hourly;");
  //building.add("Output:Variable,*,Site Outdoor Air Drybulb Temperature,HOURLY;");
  //building.add("Output:Variable,*,Site Outdoor Air Relative Humidity,HOURLY;");
  //building.add("OutputControl:Table:Style,HTML;");
  //building.add("Output:Table:SummaryReports,AllSummary;");
*/

  /*
Output:Meter,Electricity:Facility,HOURLY;

Output:Meter,Electricity:Building,HOURLY;

Output:Meter,Electricity:HVAC,HOURLY;

Output:Meter,Gas:Facility,HOURLY;

Output:Variable,*,Site Outdoor Air Drybulb Temperature,HOURLY;

Output:Variable,*,Site Outdoor Air Humidity Ratio,HOURLY;

Output:Variable,*,Site Outdoor Air Relative Humidity,HOURLY;

Output:Variable,*,Zone Mean Air Temperature,hourly;

Output:Variable,*,Air System Outdoor Air Flow Fraction,HOURLY;

Output:Variable,*,Cooling Coil Electric Power,hourly;

Output:Variable,*,Heating Coil Gas Energy Rate,hourly;
*/


  //building.write(t_path);


}





