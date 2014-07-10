/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "SqliteMerge.hpp"
#include "sqlite3.h"
#include <vector>
#include "boost/filesystem/operations.hpp"
#include "boost/filesystem/path.hpp"
#include "boost/filesystem.hpp"
#include <iostream>
#include <sstream>


static int callback(void *r, int argc, char **argv, char **azColName) {

  //std::cout << "callback" << std::endl;
  //for(int i=0; i<argc; i++){
    //printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  //}
  //printf("\n");
  return 0;
}

static int rowCount(void *r, int argc, char **argv, char **azColName) {

  if( argc > 0 ){
    std::cout << argv[0];
  } else {
    std::cout << 0;
  }

  return 0;
}


SqliteMerge::SqliteMerge()
  : m_final("final")  //name of final database
{
}

SqliteMerge::~SqliteMerge()
{

}


void SqliteMerge::mergeFiles()
{

  // If there is only one file, we are done.
  if (m_files.size() == 1)
  {
    sqlite3 *main_db = openDatabase(m_files[0]);
    createABUPS(main_db);
    closeDatabase(main_db);
    renameFinalDatabase( m_files[0]);
  } else {
    // Otherwise, there are more files..
    sqlite3 *main_db = openDatabase(m_files[0]);

    //summary(main_db);
    //m_primaryKeyBase = sqlite3_last_insert_rowid(main_db);  
    //std::sort(m_files.begin(),m_files.end()); 

    for (size_t i = 1; i < m_files.size(); ++i)
    {
      //m_primaryKeyBase = i*1000;
      //m_primaryKeyBase = sqlite3_last_insert_rowid(main_db);  
      mergeDatabases(main_db,m_files[i]);
    }

    //summary(main_db);

    //printMeterData(main_db);
    //CreateView 
    //CreateView(main_db);  //for testing purposes
    // create ABUPS table in main database 
    //
    begin(main_db); 

    //meaningless is the tabular data now
    dropTabularData(main_db);
    createABUPS(main_db); 
    commit(main_db); 
    closeDatabase(main_db);
    renameFinalDatabase( m_files[0]);
  }
}

void SqliteMerge::dropTabularData(sqlite3 *db)
{
  std::stringstream cmd;
  cmd << "delete from tabulardata";
  executeCommand(db, cmd.str().c_str());
}

void SqliteMerge::createABUPS(sqlite3 *db)
{
  std::stringstream cmd;
  cmd << "CREATE TABLE ABUPS (Rowindex INTEGER PRIMARY KEY, EndUseName TEXT, Electricity REAL, ";
  cmd << "NaturalGas REAL, OtherFuel REAL, DistrictCooling REAL, DistrictHeating REAL, Water REAL)";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  cmd << "INSERT INTO ABUPS (EndUseName) VALUES ('Heating'); ";
  cmd << "INSERT INTO ABUPS (EndUseName) VALUES ('Cooling'); ";
  cmd << "INSERT INTO ABUPS (EndUseName) VALUES ('Interior Lighting'); ";
  cmd << "INSERT INTO ABUPS (EndUseName) VALUES ('Exterior Lighting'); ";
  cmd << "INSERT INTO ABUPS (EndUseName) VALUES ('Interior Equipment'); ";
  cmd << "INSERT INTO ABUPS (EndUseName) VALUES ('Exterior Equipment'); ";
  cmd << "INSERT INTO ABUPS (EndUseName) VALUES ('Fans'); ";
  cmd << "INSERT INTO ABUPS (EndUseName) VALUES ('Pumps'); ";
  cmd << "INSERT INTO ABUPS (EndUseName) VALUES ('Heat Rejection'); ";
  cmd << "INSERT INTO ABUPS (EndUseName) VALUES ('Humidification'); ";
  cmd << "INSERT INTO ABUPS (EndUseName) VALUES ('Heat Recovery'); ";
  cmd << "INSERT INTO ABUPS (EndUseName) VALUES ('Water Systems'); ";
  cmd << "INSERT INTO ABUPS (EndUseName) VALUES ('Refrigeration'); ";
  cmd << "INSERT INTO ABUPS (EndUseName) VALUES ('Generators'); ";
  cmd << "INSERT INTO ABUPS (EndUseName) VALUES ('TotalEndUses'); ";
  cmd << "UPDATE ABUPS SET Electricity =  0; ";
  cmd << "UPDATE ABUPS SET NaturalGas =  0; ";
  cmd << "UPDATE ABUPS SET OtherFuel =  0; ";
  cmd << "UPDATE ABUPS SET DistrictCooling =  0; ";
  cmd << "UPDATE ABUPS SET DistrictHeating =  0; ";
  cmd << "UPDATE ABUPS SET Water =  0; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string
  //heating
  cmd << "UPDATE ABUPS SET Electricity = Electricity + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'Heating:Electricity' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Heating'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  cmd << "UPDATE ABUPS SET NaturalGas = NaturalGas + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'Heating:Gas' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Heating'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  cmd << "UPDATE ABUPS SET DistrictHeating = DistrictHeating + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'Heating:DistrictHeating' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Heating'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string
  //cooling

  cmd << "UPDATE ABUPS SET Electricity = Electricity + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'Cooling:Electricity' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Cooling'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  cmd << "UPDATE ABUPS SET NaturalGas = NaturalGas + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'Cooling:Gas' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Cooling'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  cmd << "UPDATE ABUPS SET DistrictCooling = DistrictCooling + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'Cooling:DistrictCooling' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Cooling'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  //Interior Lighting
  cmd << "UPDATE ABUPS SET Electricity = Electricity + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'InteriorLights:Electricity' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Interior Lighting'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  //Exterior Lighting
  cmd << "UPDATE ABUPS SET Electricity = Electricity + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'ExteriorLights:Electricity' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Exterior Lighting'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string  

  //Interior Equipment
  cmd << "UPDATE ABUPS SET Electricity = Electricity + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'InteriorEquipment:Electricity' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Interior Equipment'; ";

  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string     
  cmd << "UPDATE ABUPS SET NaturalGas = NaturalGas + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'InteriorEquipment:Gas' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Interior Equipment'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  // Exterior Equipment
  cmd << "UPDATE ABUPS SET Electricity = Electricity + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'ExteriorEquipment:Electricity' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Exterior Equipment'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  cmd << "UPDATE ABUPS SET NaturalGas = NaturalGas + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'ExteriorEquipment:Gas' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Exterior Equipment'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  // Fans
  cmd << "UPDATE ABUPS SET Electricity = Electricity + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'Fans:Electricity' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Fans'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  // Pumps
  cmd << "UPDATE ABUPS SET Electricity = Electricity + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'Pumps:Electricity' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Pumps'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  //Heat Rejection
  cmd << "UPDATE ABUPS SET Electricity = Electricity + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'HeatRejection:Electricity' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Heat Rejection'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  cmd << "UPDATE ABUPS SET OtherFuel = OtherFuel + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'HeatRejection:EnergyTransfer' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Heat Rejection'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  cmd << "UPDATE ABUPS SET Water = Water + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'HeatRejection:Water' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Heat Rejection'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  //  Humidification
  cmd << "UPDATE ABUPS SET Electricity = Electricity + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'Humidifier:Electricity' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Humidification'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  cmd << "UPDATE ABUPS SET Water = Water + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'Humidifier:Water' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Humidification'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string 

  //  Heat Recovery 
  cmd << "UPDATE ABUPS SET Electricity = Electricity + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'HeatRecovery:Electricity' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Heat Recovery'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  // Water Systems
  cmd << "UPDATE ABUPS SET Electricity = Electricity + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'WaterSystems:Electricity' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Water Systems'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  cmd << "UPDATE ABUPS SET NaturalGas = NaturalGas + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'WaterSystems:Gas' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Water Systems'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  cmd << "UPDATE ABUPS SET Water = Water + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'WaterSystems:Water' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Water Systems'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  //  Refrigeration 
  cmd << "UPDATE ABUPS SET Electricity = Electricity + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'Refrigeration:Electricity' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Refrigeration'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  //  Generators
  cmd << "UPDATE ABUPS SET Electricity = Electricity + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'CoGeneration:Electricity' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Generators'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  cmd << "UPDATE ABUPS SET NaturalGas = NaturalGas + (SELECT CASE WHEN sum(reportmeterdata.variablevalue) is not NULL THEN  sum(reportmeterdata.variablevalue) ELSE 0 END FROM ";
  cmd << "time,reportmeterdata,reportmeterdatadictionary where ";
  cmd << "time.daytype != 'WinterDesignDay' and time.daytype != 'SummerDesignDay' and time.daytype != 'customday1' and time.daytype != 'customday2' and time.timeindex = reportmeterdata.timeindex and ";
  cmd << "reportmeterdata.reportmeterdatadictionaryindex = reportmeterdatadictionary.reportmeterdatadictionaryindex and reportmeterdatadictionary.variablename = 'CoGeneration:Gas' and reportmeterdatadictionary.reportingfrequency = 'Hourly') where EndUseName = 'Generators'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  //TotalEndUses
  cmd << "UPDATE ABUPS SET Electricity = (select Sum(Electricity) from ABUPS) where EndUseName = 'TotalEndUses'; ";
  cmd << "UPDATE ABUPS SET NaturalGas = (select Sum(NaturalGas) from ABUPS) where EndUseName = 'TotalEndUses'; ";
  cmd << "UPDATE ABUPS SET OtherFuel = (select Sum(OtherFuel) from ABUPS) where EndUseName = 'TotalEndUses'; ";
  cmd << "UPDATE ABUPS SET DistrictCooling = (select Sum(DistrictCooling) from ABUPS) where EndUseName = 'TotalEndUses'; ";
  cmd << "UPDATE ABUPS SET DistrictHeating = (select Sum(DistrictHeating) from ABUPS) where EndUseName = 'TotalEndUses'; ";
  cmd << "UPDATE ABUPS SET Water = (select Sum(Water) from ABUPS) where EndUseName = 'TotalEndUses'; ";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string
}


void SqliteMerge::createView(sqlite3 *db)
{
  std::stringstream cmd;

  cmd << "CREATE VIEW 'Electricity:Facility:Monthly' AS ";
  cmd << "SELECT Time.Month, Time.Day, Time.hour, Time.TimeIndex, ReportMeterData.VariableValue from Time, ReportMeterData, ";
  cmd << "ReportMeterDataDictionary where (time.TimeIndex = ReportMeterData.TimeIndex) and (ReportMeterData.ReportMeterDataDictionaryIndex = ";
  cmd << "ReportMeterDataDictionary.ReportMeterDataDictionaryIndex) and (ReportMeterDataDictionary.VariableName = 'Electricity:Facility' and ";
  cmd << "ReportMeterDataDictionary.ReportingFrequency = 'Monthly')";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  cmd << "CREATE VIEW 'Electricity:Facility:Hourly' AS ";
  cmd << "SELECT Time.Month, Time.Day, Time.hour, Time.TimeIndex, ReportMeterData.VariableValue from Time, ReportMeterData, ";
  cmd << "ReportMeterDataDictionary where (time.TimeIndex = ReportMeterData.TimeIndex) and (ReportMeterData.ReportMeterDataDictionaryIndex = ";
  cmd << "ReportMeterDataDictionary.ReportMeterDataDictionaryIndex) and (ReportMeterDataDictionary.VariableName = 'Electricity:Facility' and ";
  cmd << "ReportMeterDataDictionary.ReportingFrequency = 'Hourly')";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  cmd << "CREATE VIEW 'Gas:Facility:Monthly' AS ";
  cmd << "SELECT Time.Month, Time.Day, Time.hour, Time.TimeIndex, ReportMeterData.VariableValue from Time, ReportMeterData, ";
  cmd << "ReportMeterDataDictionary where (time.TimeIndex = ReportMeterData.TimeIndex) and (ReportMeterData.ReportMeterDataDictionaryIndex = ";
  cmd << "ReportMeterDataDictionary.ReportMeterDataDictionaryIndex) and (ReportMeterDataDictionary.VariableName = 'Gas:Facility' and ";
  cmd << "ReportMeterDataDictionary.ReportingFrequency = 'Monthly')";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  cmd << "CREATE VIEW 'Gas:Facility:Hourly' AS ";
  cmd << "SELECT Time.Month, Time.Day, Time.hour, Time.TimeIndex, ReportMeterData.VariableValue from Time, ReportMeterData, ";
  cmd << "ReportMeterDataDictionary where (time.TimeIndex = ReportMeterData.TimeIndex) and (ReportMeterData.ReportMeterDataDictionaryIndex = ";
  cmd << "ReportMeterDataDictionary.ReportMeterDataDictionaryIndex) and (ReportMeterDataDictionary.VariableName = 'Gas:Facility' and ";
  cmd << "ReportMeterDataDictionary.ReportingFrequency = 'Hourly')";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  cmd << "CREATE VIEW 'Fans:Electricity:Hourly' AS ";
  cmd << "SELECT Time.Month, Time.Day, Time.hour, Time.TimeIndex, ReportMeterData.VariableValue from Time, ReportMeterData, ";
  cmd << "ReportMeterDataDictionary where (time.TimeIndex = ReportMeterData.TimeIndex) and (ReportMeterData.ReportMeterDataDictionaryIndex = ";
  cmd << "ReportMeterDataDictionary.ReportMeterDataDictionaryIndex) and (ReportMeterDataDictionary.VariableName = 'Fans:Electricity' and ";
  cmd << "ReportMeterDataDictionary.ReportingFrequency = 'Hourly')";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  cmd << "CREATE VIEW 'Cooling:Electricity:Hourly' AS ";
  cmd << "SELECT Time.Month, Time.Day, Time.hour, Time.TimeIndex, ReportMeterData.VariableValue from Time, ReportMeterData, ";
  cmd << "ReportMeterDataDictionary where (time.TimeIndex = ReportMeterData.TimeIndex) and (ReportMeterData.ReportMeterDataDictionaryIndex = ";
  cmd << "ReportMeterDataDictionary.ReportMeterDataDictionaryIndex) and (ReportMeterDataDictionary.VariableName = 'Cooling:Electricity' and ";
  cmd << "ReportMeterDataDictionary.ReportingFrequency = 'Hourly')";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  cmd << "CREATE VIEW 'Heating:Gas:Hourly' AS ";
  cmd << "SELECT Time.Month, Time.Day, Time.hour, Time.TimeIndex, ReportMeterData.VariableValue from Time, ReportMeterData, ";
  cmd << "ReportMeterDataDictionary where (time.TimeIndex = ReportMeterData.TimeIndex) and (ReportMeterData.ReportMeterDataDictionaryIndex = ";
  cmd << "ReportMeterDataDictionary.ReportMeterDataDictionaryIndex) and (ReportMeterDataDictionary.VariableName = 'Heating:Gas' and ";
  cmd << "ReportMeterDataDictionary.ReportingFrequency = 'Hourly')";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string

  cmd << "CREATE VIEW 'InteriorLights:Electricity:Hourly' AS ";
  cmd << "SELECT Time.Month, Time.Day, Time.hour, Time.TimeIndex, ReportMeterData.VariableValue from Time, ReportMeterData, ";
  cmd << "ReportMeterDataDictionary where (time.TimeIndex = ReportMeterData.TimeIndex) and (ReportMeterData.ReportMeterDataDictionaryIndex = ";
  cmd << "ReportMeterDataDictionary.ReportMeterDataDictionaryIndex) and (ReportMeterDataDictionary.VariableName = 'InteriorLights:Electricity' and ";
  cmd << "ReportMeterDataDictionary.ReportingFrequency = 'Hourly')";
  executeCommand(db, cmd.str().c_str());
  cmd.str(std::string());  //clear out the string
}


void SqliteMerge::renameFinalDatabase(const openstudio::path &file)
{
  std::string tmp = m_final;
  openstudio::path final_path =  m_working / openstudio::toPath(tmp.append(".sql"));
  boost::filesystem::remove_all( final_path);
  boost::filesystem::copy_file( file, final_path);
  //std::cout << "creating " << final_path << std::endl;
}


void SqliteMerge::loadFile(const openstudio::path &file)
{
  if (openstudio::toString(boost::filesystem::extension(file)) == ".sql")
  {
    m_files.push_back(file);
  }
}


// Database helper functions...

void SqliteMerge::summary(sqlite3 *db)
{
  //printNumberRows(db, "Time");
  //printNumberRows(db, "ReportMeterData");
  printNumberRows(db, "Time");  
  //ReturnNumberRows(db, "Time");  
  //printNumberRows(db, "ReportMeterDataDictionary");
  //printNumberRows(db, "ReportMeterExtendedData");
}

void SqliteMerge::printNumberRows(sqlite3 *db, const std::string &table)
{
  std::stringstream cmd;
  cmd << "select count(*) from " << table;
  std::cout << "\t" << table << " ";
  char *zErrMsg = nullptr;
  int rc = sqlite3_exec(db, cmd.str().c_str(),
      rowCount, nullptr, &zErrMsg);
  if (rc != SQLITE_OK)
  {
    //fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
  std::cout << std::endl;
}

void SqliteMerge::returnNumberRows(sqlite3 *db, const std::string &table) 
{
  //results->clear();
  std::stringstream cmd;
  cmd << "select count(*) from " << table;
  std::cout << "\t" << table << " ";
  char *zErrMsg = nullptr;
  int rc = sqlite3_exec(db, cmd.str().c_str(),
      rowCount, nullptr, &zErrMsg);
  if (rc != SQLITE_OK)
  {
    //fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
  std::cout << std::endl;
}

sqlite3 * SqliteMerge::openDatabase(const openstudio::path &file)
{
  sqlite3 *db = nullptr;
  int rc = sqlite3_open(openstudio::toString(file).c_str(), &db);
  if (rc)
  {
    throw std::runtime_error("Unable to open database " + openstudio::toString(file));
  }
  return db;
}

void SqliteMerge::closeDatabase(sqlite3 *db)
{
  //std::cout << "closing database" << std::endl;
  sqlite3_close(db);
}

void SqliteMerge::mergeDatabases(sqlite3 *dest, const openstudio::path &source)
{
  // Attach source to destination...
  // Insert table information...

  attachDatabases(dest, source);
  begin(dest);

  //------------------------------------------------------------
  /*  Meter Data
   *
   *  Uses 4 tables
   *  1. Time: append
   *  2. Report Meter Data Dictionary: no change
   *  3. Report Meter Data: append
   *  4. Report Meter Extended Data: modify, has summary stats
   */

  tableReportMeterData(dest);
  tableReportMeterExtendedData(dest);
  tableReportVariableData(dest);
  tableReportVariableExtendedData(dest);
  tableTime(dest);
  //------------------------------------------------------------

  commit(dest);
  detachDatabases(dest);
}

void SqliteMerge::tableReportVariableData(sqlite3 *db)
{
  std::stringstream cmd;

  cmd << "insert into reportVariableData ";
  //cmd << "select TimeIndex+(select Max(TimeIndex)from time) as TimeIndex, ";  
  cmd << "select TimeIndex+(select Max(TimeIndex)from time)-(select Min(TimeIndex) from merger.Time)+1 as TimeIndex, ";
  //cmd << "ReportVariableDataDictionaryIndex, VariableValue, ReportVariableExtendedDataIndex+(select Max(TimeIndex)from time) ";
  cmd << "ReportVariableDataDictionaryIndex, VariableValue, ReportVariableExtendedDataIndex+(select Max(ReportVariableExtendedDataIndex)from reportVariableData)-(select Min(ReportVariableExtendedDataIndex) from merger.reportVariableData)+1 ";
  cmd << "from merger.reportVariableData ";

  executeCommand(db, cmd.str().c_str());
}
//insert into reportVariableData select TimeIndex+(select Max(TimeIndex)from time) as TimeIndex, ReportVariableDataDictionaryIndex, VariableValue, ReportVariableExtendedDataIndex+(select Max(TimeIndex)from time) from reportMeterData2

void SqliteMerge::tableReportVariableExtendedData(sqlite3 *db) 
{
  std::stringstream cmd;

  cmd << "insert into reportVariableExtendedData ";
  //cmd << "select ReportVariableExtendedDataIndex+(select Max(TimeIndex)from time) as ReportVariableExtendedDataIndex, "; 
  cmd << "select ReportVariableExtendedDataIndex+(select Max(ReportVariableExtendedDataIndex)from reportVariableExtendedData)-(select Min(ReportVariableExtendedDataIndex) from merger.reportVariableExtendedData)+1 as ReportVariableExtendedDataIndex, ";
  cmd << "MaxValue, MaxMonth, MaxDay, MaxHour, MaxStartMinute, MaxMinute, MinValue, MinMonth, MinDay, MinHour, ";
  cmd << "MinStartMinute, MinMinute ";
  cmd << "from merger.ReportVariableExtendedData ";

  executeCommand(db, cmd.str().c_str());
  //insert into reportVariableExtendedData select ReportVariableExtendedDataIndex+(select Max(TimeIndex)from time) as ReportVariableExtendedDataIndex, MaxValue, MaxMonth, MaxDay, MaxHour, MaxStartMinute, MaxMinute, MinValue, MinMonth, MinDay, MinHour, MinStartMinute, MinMinute from ReportVariableExtendedData2
}
void SqliteMerge::tableReportMeterData(sqlite3 *db)
{
  std::stringstream cmd;

  cmd << "insert into reportMeterData ";
  //cmd << "select TimeIndex+(select Max(TimeIndex)from time) as TimeIndex, ";  
  cmd << "select TimeIndex+(select Max(TimeIndex)from time)-(select Min(TimeIndex) from merger.Time)+1 as TimeIndex, ";
  //cmd << "ReportMeterDataDictionaryIndex, VariableValue, ReportVariableExtendedDataIndex+(select Max(TimeIndex)from time) ";
  cmd << "ReportMeterDataDictionaryIndex, VariableValue, ReportVariableExtendedDataIndex+(select Max(ReportVariableExtendedDataIndex)from reportMeterData)-(select Min(ReportVariableExtendedDataIndex) from merger.reportMeterData)+1 ";
  cmd << "from merger.reportMeterData ";

  executeCommand(db, cmd.str().c_str());
}
//insert into reportMeterData select TimeIndex+(select Max(TimeIndex)from time) as TimeIndex, ReportMeterDataDictionaryIndex, VariableValue, ReportVariableExtendedDataIndex+(select Max(TimeIndex)from time) from reportMeterData2

void SqliteMerge::tableTime(sqlite3 *db)
{
  std::stringstream cmd;

  cmd << "insert into time ";
  //cmd << "select NULL, ";
  //cmd << "select TimeIndex+" << m_primaryKeyBase << ", ";
  //cmd << "select TimeIndex+(select Max(TimeIndex)from time) as TimeIndex, ";
  cmd << "select TimeIndex+(select Max(TimeIndex)from time)-(select Min(TimeIndex) from merger.Time)+1 as TimeIndex, ";
  cmd << "Month, Day, Hour, Minute, Dst, Interval, IntervalType, ";
  cmd << "SimulationDays+(select Max(SimulationDays)from time)-(select Min(SimulationDays) from merger.Time)+1 as SimulationDays, DayType, EnvironmentPeriodIndex, WarmupFlag ";
  //cmd << "SimulationDays, DayType, EnvironmentPeriodIndex, WarmupFlag ";
  cmd << "from merger.time ";
  //cmd << "where merger.time.TimeIndex > 6";  6 = 3*2, 3 days of offset in test

  executeCommand(db, cmd.str().c_str());
  //insert into time select TimeIndex+(select Max(TimeIndex)from time) as TimeIndex, Month, Day, Hour, Minute, Dst, Interval, IntervalType, SimulationDays, DayType, EnvironmentPeriodIndex, WarmupFlag from Time2
  //insert into Time select TimeIndex+((select Max(TimeIndex)from time)-(select Min(TimeIndex) from Time2)-1) as TimeIndex, Month, Day, Hour, Minute, Dst, Interval, IntervalType, SimulationDays, DayType, EnvironmentPeriodIndex, WarmupFlag from Time2
}

void SqliteMerge::tableReportMeterExtendedData(sqlite3 *db) 
{
  std::stringstream cmd;

  cmd << "insert into reportMeterExtendedData ";
  //cmd << "select ReportMeterExtendedDataIndex+(select Max(TimeIndex)from time) as ReportMeterExtendedDataIndex, "; 
  cmd << "select ReportMeterExtendedDataIndex+(select Max(ReportMeterExtendedDataIndex)from reportMeterExtendedData)-(select Min(ReportMeterExtendedDataIndex) from merger.reportMeterExtendedData)+1 as ReportMeterExtendedDataIndex, ";
  cmd << "MaxValue, MaxMonth, MaxDay, MaxHour, MaxStartMinute, MaxMinute, MinValue, MinMonth, MinDay, MinHour, ";
  cmd << "MinStartMinute, MinMinute ";
  cmd << "from merger.ReportMeterExtendedData ";

  executeCommand(db, cmd.str().c_str());
  //insert into reportMeterExtendedData select ReportMeterExtendedDataIndex+(select Max(TimeIndex)from time) as ReportMeterExtendedDataIndex, MaxValue, MaxMonth, MaxDay, MaxHour, MaxStartMinute, MaxMinute, MinValue, MinMonth, MinDay, MinHour, MinStartMinute, MinMinute from ReportMeterExtendedData2
}

void SqliteMerge::printMeterData(sqlite3 * dest)
{

  std::stringstream cmd;

  cmd << "select month, day, variableName, variableValue  ";
  cmd << "from ReportMeterData a, ReportMeterDataDictionary b, time t ";
  cmd << "where a.ReportMeterDataDictionaryIndex = b.ReportMeterDataDictionaryIndex ";
  cmd << "and a.timeIndex = t.timeIndex";

  char *zErrMsg = nullptr;
  int rc = sqlite3_exec(dest, cmd.str().c_str(),
      callback, nullptr, &zErrMsg);
  if (rc != SQLITE_OK)
  {
    //fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

}


bool SqliteMerge::commit(sqlite3 *dest)
{
  std::string tmp = "commit";
  executeCommand(dest, tmp);
  return true;
}


bool SqliteMerge::begin(sqlite3 *dest)
{
  std::string tmp = "begin";
  executeCommand(dest, tmp);
  return true;
}

void SqliteMerge::attachDatabases(sqlite3 *destination, const openstudio::path &source){

  //std::cout << "Attaching database" << std::endl;
  std::stringstream cmd;
  cmd << "attach '" << openstudio::toString(source) << "' as merger";
  std::string tmpCmd = cmd.str();
  executeCommand(destination, tmpCmd);

}

void SqliteMerge::detachDatabases(sqlite3 *destination)
{
  //std::cout << "Detaching database" << std::endl;
  std::string tmpCmd = "detach database merger";
  executeCommand(destination, tmpCmd);
}

bool SqliteMerge::executeCommand(sqlite3 *destination, const std::string &cmd)
{
  char *zErrMsg = nullptr;
  int rc = sqlite3_exec(destination, cmd.c_str(), callback, nullptr, &zErrMsg);
  if (rc != SQLITE_OK)
  {
    //fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return false;
  }
  return true;

}





