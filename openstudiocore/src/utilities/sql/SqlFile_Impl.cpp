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

#include <utilities/sql/SqlFile_Impl.hpp>
#include <utilities/sql/SqlFileTimeSeriesQuery.hpp>

#include <utilities/core/String.hpp>
#include <utilities/time/Calendar.hpp>
#include <utilities/filetypes/EpwFile.hpp>
#include <utilities/core/Containers.hpp>
#include <utilities/core/Optional.hpp>
#include <utilities/time/DateTime.hpp>
#include <utilities/core/Assert.hpp>

#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/regex.hpp>
#include <boost/foreach.hpp>

using boost::multi_index_container;
using boost::multi_index::indexed_by;
using boost::multi_index::ordered_unique;
using boost::multi_index::ordered_non_unique;
using boost::multi_index::tag;
using boost::multi_index::composite_key;
using boost::multi_index::member;

namespace openstudio{

  namespace detail{

    std::string columnText(const unsigned char* column)
    {
      return std::string(reinterpret_cast<const char*>(column));
    }

    SqlFile_Impl::SqlFile_Impl(const openstudio::path& path)
      : m_path(path), m_connectionOpen(false), m_supportedVersion(false)
    {
      reopen();
    }

    SqlFile_Impl::SqlFile_Impl(const openstudio::path &t_path, const openstudio::EpwFile &t_epwFile, const openstudio::DateTime &t_simulationTime,
        const openstudio::Calendar &t_calendar)
      : m_path(t_path), m_sqliteFilename(toString(m_path))
    {
      m_sqliteFilename = toString(m_path);
      std::string fileName = m_sqliteFilename;

      bool initschema = false;

      if (!boost::filesystem::exists(m_path))
      {
        initschema = true;
      }

      sqlite3_open_v2(fileName.c_str(), &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_EXCLUSIVE, NULL);

      if (initschema)
      {
        execAndThrowOnError(
            /* extracted from real eplusout.sql */
            "CREATE TABLE ComponentSizes (CompType TEXT, CompName TEXT, Description TEXT, Value REAL, Units TEXT);"
            "CREATE TABLE ConstructionLayers (ConstructionIndex INTEGER, LayerIndex INTEGER, MaterialIndex INTEGER);"
            "CREATE TABLE Constructions (ConstructionIndex INTEGER PRIMARY KEY, Name TEXT, TotalLayers INTEGER, TotalSolidLayers INTEGER, TotalGlassLayers INTEGER, InsideAbsorpVis REAL, OutsideAbsorpVis REAL, InsideAbsorpSolar REAL, OutsideAbsorpSolar REAL, InsideAbsorpThermal REAL, OutsideAbsorpThermal REAL, OutsideRoughness INTEGER, TypeIsWindow INTEGER, Uvalue REAL);"
            "CREATE TABLE DaylightMapHourlyData (HourlyReportIndex INTEGER, X REAL, Y REAL, Illuminance REAL);"
            "CREATE TABLE DaylightMapHourlyReports (HourlyReportIndex INTEGER PRIMARY KEY, MapNumber INTEGER, Month INTEGER, DayOfMonth INTEGER, Hour INTEGER);"
            "CREATE TABLE DaylightMaps (MapNumber INTEGER PRIMARY KEY, MapName TEXT, Environment TEXT, Zone INTEGER, ReferencePt1 TEXT, ReferencePt2 TEXT, Z REAL);"
            "CREATE TABLE EnvironmentPeriods (EnvironmentPeriodIndex INTEGER PRIMARY KEY, SimulationIndex INTEGER, EnvironmentName TEXT, EnvironmentType INTEGER);"
            "CREATE TABLE Errors (ErrorIndex INTEGER PRIMARY KEY, SimulationIndex INTEGER, ErrorType INTEGER, ErrorMessage TEXT, Count INTEGER);"
            "CREATE TABLE Materials (MaterialIndex INTEGER PRIMARY KEY, Name TEXT, MaterialType INTEGER, Roughness INTEGER, Conductivity REAL, Density REAL, IsoMoistCap REAL, Porosity REAL, Resistance REAL, ROnly INTEGER, SpecHeat REAL, ThermGradCoef REAL, Thickness REAL, VaporDiffus);"
            "CREATE TABLE NominalBaseboardHeaters (NominalBaseboardHeaterIndex INTEGER PRIMARY KEY, ObjectName TEXT, ZoneIndex INTEGER, ScheduleIndex INTEGER, CapatLowTemperature REAL, LowTemperature REAL, CapatHighTemperature REAL, HighTemperature REAL, FractionRadiant REAL, FractionConvected REAL, EndUseSubcategory TEXT);"
            "CREATE TABLE NominalElectricEquipment (NominalElectricEquipmentIndex INTEGER PRIMARY KEY, ObjectName TEXT, ZoneIndex INTEGER, ScheduleIndex INTEGER, DesignLevel REAL, FractionLatent REAL, FractionRadiant REAL, FractionLost REAL, FractionConvected REAL, EndUseSubcategory TEXT);"
            "CREATE TABLE NominalGasEquipment(NominalGasEquipmentIndex INTEGER PRIMARY KEY, ObjectName TEXT, ZoneIndex INTEGER, ScheduleIndex INTEGER, DesignLevel REAL, FractionLatent REAL, FractionRadiant REAL, FractionLost REAL, FractionConvected REAL, EndUseSubcategory TEXT);"
            "CREATE TABLE NominalHotWaterEquipment(NominalHotWaterEquipmentIndex INTEGER PRIMARY KEY, ObjectName TEXT, ZoneIndex INTEGER, SchedNo INTEGER, DesignLevel REAL, FractionLatent REAL, FractionRadiant REAL, FractionLost REAL, FractionConvected REAL, EndUseSubcategory TEXT);"
            "CREATE TABLE NominalInfiltration (NominalInfiltrationIndex INTEGER PRIMARY KEY, ObjectName TEXT, ZoneIndex INTEGER, ScheduleIndex INTEGER, DesignLevel REAL);"
            "CREATE TABLE NominalLighting (NominalLightingIndex INTEGER PRIMARY KEY, ObjectName TEXT, ZoneIndex INTEGER, ScheduleIndex INTEGER, DesignLevel REAL, FractionReturnAir REAL, FractionRadiant REAL, FractionShortWave REAL, FractionReplaceable REAL, FractionConvected REAL, EndUseSubcategory TEXT);"
            "CREATE TABLE NominalOtherEquipment(NominalOtherEquipmentIndex INTEGER PRIMARY KEY, ObjectName TEXT, ZoneIndex INTEGER, ScheduleIndex INTEGER, DesignLevel REAL, FractionLatent REAL, FractionRadiant REAL, FractionLost REAL, FractionConvected REAL, EndUseSubcategory TEXT);"
            "CREATE TABLE NominalPeople (NominalPeopleIndex INTEGER PRIMARY KEY, ObjectName TEXT, ZoneIndex INTEGER,NumberOfPeople INTEGER, NumberOfPeopleScheduleIndex INTEGER, ActivityScheduleIndex INTEGER, FractionRadiant REAL, FractionConvected REAL, WorkEfficiencyScheduleIndex INTEGER, ClothingEfficiencyScheduleIndex INTEGER, AirVelocityScheduleIndex INTEGER, Fanger INTEGER, Pierce INTEGER, KSU INTEGER, MRTCalcType INTEGER, SurfaceIndex INTEGER, AngleFactorListName TEXT, AngleFactorList INTEGER, UserSpecifeidSensibleFraction REAL, Show55Warning INTEGER);"
            "CREATE TABLE NominalSteamEquipment(NominalSteamEquipmentIndex INTEGER PRIMARY KEY, ObjectName TEXT, ZoneIndex INTEGER, ScheduleIndex INTEGER, DesignLevel REAL, FractionLatent REAL, FractionRadiant REAL, FractionLost REAL, FractionConvected REAL, EndUseSubcategory TEXT);"
            "CREATE TABLE NominalVentilation (NominalVentilationIndex INTEGER PRIMARY KEY, ObjectName TEXT, ZoneIndex INTEGER, ScheduleIndex INTEGER, DesignLevel REAL);"
            "CREATE TABLE ReportMeterData(TimeIndex INTEGER, ReportMeterDataDictionaryIndex INTEGER, VariableValue REAL, ReportVariableExtendedDataIndex INTEGER);"
            "CREATE TABLE ReportMeterDataDictionary (ReportMeterDataDictionaryIndex INTEGER PRIMARY KEY, VariableType TEXT, IndexGroup TEXT, TimestepType TEXT, KeyValue TEXT, VariableName TEXT, ReportingFrequency TEXT, ScheduleName TEXT, VariableUnits TEXT);"
            "CREATE TABLE ReportMeterExtendedData (ReportMeterExtendedDataIndex INTEGER PRIMARY KEY, MaxValue REAL, MaxMonth INTEGER, MaxDay INTEGER, MaxHour INTEGER, MaxStartMinute INTEGER, MaxMinute INTEGER, MinValue REAL, MinMonth INTEGER, MinDay INTEGER, MinHour INTEGER, MinStartMinute INTEGER, MinMinute INTEGER);"
            "CREATE TABLE ReportVariableData (TimeIndex INTEGER, ReportVariableDataDictionaryIndex INTEGER, VariableValue REAL, ReportVariableExtendedDataIndex INTEGER);"
            "CREATE TABLE ReportVariableDataDictionary(ReportVariableDataDictionaryIndex INTEGER PRIMARY KEY, VariableType TEXT, IndexGroup TEXT, TimestepType TEXT, KeyValue TEXT, VariableName TEXT, ReportingFrequency TEXT, ScheduleName TEXT, VariableUnits TEXT);"
            "CREATE TABLE ReportVariableExtendedData (ReportVariableExtendedDataIndex INTEGER PRIMARY KEY, MaxValue REAL, MaxMonth INTEGER, MaxDay INTEGER, MaxHour INTEGER, MaxStartMinute INTEGER, MaxMinute INTEGER, MinValue REAL, MinMonth INTEGER, MinDay INTEGER, MinHour INTEGER, MinStartMinute INTEGER, MinMinute INTEGER);"
            "CREATE TABLE RoomAirModels (ZoneIndex INTEGER PRIMARY KEY, AirModelName TEXT, AirModelType INTEGER, TempCoupleScheme INTEGER, SimAirModel INTEGER);"
            "CREATE TABLE Schedules (ScheduleIndex INTEGER PRIMARY KEY, ScheduleName TEXT, ScheduleType TEXT, ScheduleMinimum REAL, ScheduleMaximum REAL);"
            "CREATE TABLE Simulations (SimulationIndex INTEGER PRIMARY KEY, EnergyPlusVersion TEXT, TimeStamp TEXT, NumTimestepsPerHour INTEGER, Completed BOOL, CompletedSuccessfully BOOL);"
            "CREATE TABLE StringTypes (StringTypeIndex INTEGER PRIMARY KEY, Value TEXT);"
            "CREATE TABLE Strings (StringIndex INTEGER PRIMARY KEY, StringTypeIndex  INTEGER, Value TEXT);"
            "CREATE TABLE Surfaces (SurfaceIndex INTEGER PRIMARY KEY, SurfaceName, ConstructionIndex INTEGER, ClassName TEXT, Area REAL, GrossArea REAL, Perimeter REAL, Azimuth REAL, Height REAL, Reveal REAL, Shape INTEGER, Sides INTEGER, Tilt REAL, Width REAL, HeatTransferSurf INTEGER, BaseSurfaceIndex INTEGER, ZoneIndex INTEGER, ExtBoundCond INTEGER,  ExtSolar INTEGER, ExtWind INTEGER);"
            "CREATE TABLE SystemSizes (SystemName TEXT, Description TEXT, Value REAL, Units TEXT);"
            "CREATE TABLE TabularData (ReportNameIndex INTEGER, ReportForStringIndex INTEGER, TableNameIndex INTEGER, SimulationIndex INTEGER, RowNameIndex INTEGER, ColumnNameIndex INTEGER, RowId INTEGER, ColumnId INTEGER, Value TEXT, UnitsIndex INTEGER);"
            "CREATE TABLE Time (TimeIndex INTEGER PRIMARY KEY, Month INTEGER, Day INTEGER, Hour INTEGER, Minute INTEGER, Dst INTEGER, Interval INTEGER, IntervalType INTEGER, SimulationDays INTEGER, DayType TEXT, EnvironmentPeriodIndex INTEGER, WarmupFlag INTEGER);"
            "CREATE TABLE ZoneGroups (ZoneGroupIndex INTEGER PRIMARY KEY, ZoneListName TEXT, ZoneListMultiplier INTEGER);"
            "CREATE TABLE ZoneLists (ZoneListIndex INTEGER PRIMARY KEY, Name TEXT, ZoneIndex INTEGER);"
            "CREATE TABLE ZoneSizes (ZoneName TEXT, LoadType TEXT, DesLoad REAL, CalcDesFlow REAL, UserDesFlow REAL, DesDayName TEXT, PeakHrMin TEXT, PeakTemp REAL, PeakHumRat REAL, CalcOutsideAirFlow REAL);"
            "CREATE TABLE Zones (ZoneIndex INTEGER PRIMARY KEY, ZoneName TEXT, RelNorth REAL, OriginX REAL, OriginY REAL, OriginZ REAL, CentroidX REAL, CentroidY REAL, CentroidZ REAL, OfType INTEGER, Multiplier REAL, ListMultiplier REAL, MinimumX REAL, MaximumX REAL, MinimumY REAL, MaximumY REAL, MinimumZ REAL, MaximumZ REAL, CeilingHeight REAL, Volume REAL, InsideConvectionAlgo INTEGER, OutsideConvectionAlgo INTEGER, FloorArea REAL, ExtGrossWallArea REAL, ExtNetWallArea REAL, ExtWindowArea REAL, IsPartOfTotalArea INTEGER);"
            "CREATE VIEW ReportVariableWithTime AS SELECT ReportVariableData.*, Time.*, ReportVariableDataDictionary.*, ReportVariableExtendedData.* FROM ReportVariableData LEFT OUTER JOIN ReportVariableExtendedData INNER JOIN Time INNER JOIN ReportVariableDataDictionary ON (ReportVariableData.ReportVariableExtendedDataIndex = ReportVariableExtendedData.ReportVariableExtendedDataIndex) AND (ReportVariableData.TimeIndex = Time.TimeIndex) AND (ReportVariableDataDictionary.ReportVariableDataDictionaryIndex = ReportVariableData.ReportVariableDataDictionaryIndex);"
            "CREATE VIEW TabularDataWithStrings AS SELECT td.Value Value, reportn.Value ReportName, fs.Value ReportForString, tn.Value TableName, rn.Value RowName, cn.Value ColumnName, u.Value Units, RowId FROM TabularData td INNER JOIN Strings reportn ON reportn.StringIndex=td.ReportNameIndex INNER JOIN Strings fs ON fs.StringIndex=td.ReportForStringIndex INNER JOIN Strings tn ON tn.StringIndex=td.TableNameIndex INNER JOIN Strings rn ON rn.StringIndex=td.RowNameIndex INNER JOIN Strings cn ON cn.StringIndex=td.ColumnNameIndex INNER JOIN Strings u ON u.StringIndex=td.UnitsIndex WHERE reportn.StringTypeIndex=1 AND fs.StringTypeIndex=2 AND tn.StringTypeIndex=3 AND rn.StringTypeIndex=4 AND cn.StringTypeIndex=5 AND u.StringTypeIndex=6;"
          );

      }

      addSimulation(t_epwFile, t_simulationTime, t_calendar);

      reopen();
      createIndexes();
    }


    void SqlFile_Impl::removeIndexes()
    {
      if (m_connectionOpen)
      {
        try {
          execAndThrowOnError("DROP INDEX rmdTI;");
        } catch (const std::runtime_error &e) {
          LOG(Trace, "Error dropping index: " + std::string(e.what()));
        }

        try {
          execAndThrowOnError("DROP INDEX rmdDI;");
        } catch (const std::runtime_error &e) {
          LOG(Trace, "Error dropping index: " + std::string(e.what()));
        }

        try {
          execAndThrowOnError("DROP INDEX rvdTI;");
        } catch (const std::runtime_error &e) {
          LOG(Trace, "Error dropping index: " + std::string(e.what()));
        }

        try {
          execAndThrowOnError("DROP INDEX rvdDI;");
        } catch (const std::runtime_error &e) {
          LOG(Trace, "Error dropping index: " + std::string(e.what()));
        }

        try {
          execAndThrowOnError("DROP INDEX dmhdHRI;");
        } catch (const std::runtime_error &e) {
          LOG(Trace, "Error dropping index: " + std::string(e.what()));
        }
      }
    }

    void SqlFile_Impl::createIndexes()
    {
      if (m_connectionOpen)
      {
        try {
          execAndThrowOnError("CREATE INDEX rmdTI ON ReportMeterData (TimeIndex ASC);");
        } catch (const std::runtime_error &e) {
          LOG(Trace, "Error adding index: " + std::string(e.what()));
        }

        try {
          execAndThrowOnError("CREATE INDEX rmdDI ON ReportMeterData (ReportMeterDataDictionaryIndex ASC);");
        } catch (const std::runtime_error &e) {
          LOG(Trace, "Error adding index: " + std::string(e.what()));
        }

        try {
          execAndThrowOnError("CREATE INDEX rvdTI ON ReportVariableData (TimeIndex ASC);");
        } catch (const std::runtime_error &e) {
          LOG(Trace, "Error adding index: " + std::string(e.what()));
        }

        try {
          execAndThrowOnError("CREATE INDEX rvdDI ON ReportVariableData (ReportVariableDataDictionaryIndex ASC);");
        } catch (const std::runtime_error &e) {
          LOG(Trace, "Error adding index: " + std::string(e.what()));
        }

        try {
          execAndThrowOnError("CREATE INDEX dmhdHRI ON DaylightMapHourlyData (HourlyReportIndex ASC);");
        } catch (const std::runtime_error &e) {
          LOG(Trace, "Error adding index: " + std::string(e.what()));
        }
      }
    }

    SqlFile_Impl::~SqlFile_Impl ()
    {
      close();
    }

    struct PreparedStatement
    {
      sqlite3 *m_db;
      sqlite3_stmt *m_statement;
      bool m_transaction;

      PreparedStatement(const std::string &t_stmt, sqlite3 *t_db, bool t_transaction = false)
        : m_db(t_db), m_statement(0), m_transaction(t_transaction)
      {
        if (m_transaction)
        {
          sqlite3_exec(m_db, "BEGIN", 0, 0, 0);
        }

        sqlite3_prepare_v2(m_db, t_stmt.c_str(), t_stmt.size(), &m_statement, 0);

        if (!m_statement)
        {
          throw std::runtime_error("Error creating prepared statement: " + t_stmt);
        }

      }

      ~PreparedStatement()
      {
        if (m_statement)
        {
          sqlite3_finalize(m_statement);
        }

        if (m_transaction)
        {
          sqlite3_exec(m_db, "COMMIT", 0, 0, 0);
        }
      }

      void bind(int position, const std::string &t_str)
      {
        sqlite3_bind_text(m_statement, position, t_str.c_str(), t_str.size(), SQLITE_TRANSIENT);
      }

      void bind(int position, int val)
      {
        sqlite3_bind_int(m_statement, position, val);
      }

      void bind(int position, unsigned int val)
      {
        sqlite3_bind_int(m_statement, position, val);
      }


      void bind(int position, double val)
      {
        sqlite3_bind_double(m_statement, position, val);
      }

      void execute()
      {
        if (sqlite3_step(m_statement) != SQLITE_DONE)
        {
          sqlite3_reset(m_statement);
          throw std::runtime_error("Error executing SQL statement step");
        } else {
          sqlite3_reset(m_statement);
        }
      }

    };


    void SqlFile_Impl::addSimulation(const openstudio::EpwFile &t_epwFile, const openstudio::DateTime &t_simulationTime,
        const openstudio::Calendar &t_calendar)
    {
      int nextSimulationIndex = getNextIndex("simulations", "SimulationIndex");

      std::stringstream timeStamp;
      timeStamp << t_simulationTime.date().year() << "." << t_simulationTime.date().monthOfYear().value() << "." <<  t_simulationTime.date().dayOfMonth() << " " << t_simulationTime.time().toString();

      std::stringstream insertSimulation;


      insertSimulation << "insert into simulations (SimulationIndex, EnergyPlusVersion, TimeStamp, NumTimestepsPerHour, Completed, CompletedSuccessfully) values ("
        << nextSimulationIndex << ", 'EnergyPlus, VERSION 8.1, (OpenStudio) YMD=" << timeStamp.str() << "', '" << timeStamp.str() << "', 6, 1, 1);";

      execAndThrowOnError(insertSimulation.str());


      int nextEnvironmentPeriodIndex = getNextIndex("environmentperiods", "EnvironmentPeriodIndex");

      std::stringstream insertEnvironment;
      insertEnvironment << "insert into environmentperiods (EnvironmentPeriodIndex, SimulationIndex, EnvironmentName, EnvironmentType) values ("
        << nextEnvironmentPeriodIndex << ", " << nextSimulationIndex << ", '" << t_epwFile.stateProvinceRegion() << " WMO#=" << t_epwFile.wmoNumber() << "', 3);";

      execAndThrowOnError(insertEnvironment.str());

      int nextTimeIndex = getNextIndex("time", "TimeIndex");



      PreparedStatement stmt("insert into time (TimeIndex, Month, Day, Hour, Minute, Dst, Interval, IntervalType, SimulationDays, DayType, EnvironmentPeriodIndex, WarmupFlag) values (?, ?, ?, ?, 0, 0, 60, 1, ?, ?, ?, null)", m_db, true);

      int simulationDay = 1;
      for (openstudio::Date d = t_calendar.startDate();
           d <= t_calendar.endDate();
           d += openstudio::Time(1,0))
      {
        for (int i = 1; i <= 24; ++i)
        {
          stmt.bind(1, nextTimeIndex);
          stmt.bind(2, d.monthOfYear().value());
          stmt.bind(3, d.dayOfMonth());
          stmt.bind(4, i);
          stmt.bind(5, simulationDay);

          if (t_calendar.isHoliday(d))
          {
            stmt.bind(6, "Holiday");
          } else {
            stmt.bind(6, d.dayOfWeek().valueName());
          }

          stmt.bind(7, nextEnvironmentPeriodIndex);
          
          stmt.execute();

          ++nextTimeIndex;
        }

        ++simulationDay;
      }
        
    }

    void SqlFile_Impl::execAndThrowOnError(const std::string &t_stmt)
    {
      char *err = 0;
      if (sqlite3_exec(m_db, t_stmt.c_str(), 0, 0, &err) != SQLITE_OK)
      {
        std::string errstr;
       
        if (err)
        {
          errstr = err;
          sqlite3_free(err);
        }

        throw std::runtime_error("Error executing SQL statement: " + t_stmt + " " + errstr);
      }
    }

    bool SqlFile_Impl::connectionOpen() const
    {
      return m_connectionOpen;
    }

    int SqlFile_Impl::getNextIndex(const std::string &t_tableName, const std::string &t_columnName)
    {
      boost::optional<int> maxindex = execAndReturnFirstInt("select max(" + t_columnName + ") from " + t_tableName);

      if (maxindex)
      {
        return *maxindex + 1;
      } else {
        return 1;
      }
    }

    openstudio::path SqlFile_Impl::path() const
    {
      return m_path;
    }

    bool SqlFile_Impl::close()
    {
      if (m_connectionOpen)
      {
        sqlite3_close(m_db);
        m_connectionOpen = false;
      }
      return true;
    }

    bool SqlFile_Impl::reopen()
    {
      bool result = true;
      try{
        close();
        init(m_path);
      }catch(const std::exception&e){
        LOG(Error, "Exception while opening database at '" << toString(m_path) 
            << "': " << e.what());
        result = false;
      }
      return result;
    }

    void SqlFile_Impl::init(const openstudio::path& path)
    {
      m_sqliteFilename = toString(m_path);
      std::string fileName = m_sqliteFilename;

      int code = sqlite3_open_v2(fileName.c_str(), &m_db, SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_EXCLUSIVE, NULL);

      m_connectionOpen = (code == 0);
      if (m_connectionOpen) {// create index on dictionaryIndex for large table reportvariabledata
        if (!isValidConnection()) {
          sqlite3_close(m_db);
          m_connectionOpen = false;
          throw openstudio::Exception("ResultsViewer is not compatable with this file.");
        }
        // set a 1 second timeout
        code = sqlite3_busy_timeout(m_db, 1000);

        // set locking mode to exlusive
        //code = sqlite3_exec(m_db, "PRAGMA locking_mode=EXCLUSIVE", NULL, NULL, NULL);

        // retrieve DataDictionaryTable
        retrieveDataDictionary();
      } else {
        throw openstudio::Exception("File not successfully opened.");
      }
    }

    bool SqlFile_Impl::isSupportedVersion() const
    {
      return m_supportedVersion;
    }

    bool SqlFile_Impl::isValidConnection()
    {
      int code = -1;
      if (m_db) {
        sqlite3_stmt* sqlStmtPtr;
        sqlite3_prepare_v2(m_db,"SELECT * FROM Simulations WHERE EnergyPlusVersion LIKE '%7.0%' OR EnergyPlusVersion LIKE '%7.1%' OR EnergyPlusVersion LIKE '%7.2%' OR EnergyPlusVersion LIKE '%8.0%' OR EnergyPlusVersion LIKE '%8.1%'",-1,&sqlStmtPtr,NULL);
        code = sqlite3_step(sqlStmtPtr);
        sqlite3_finalize(sqlStmtPtr);
        m_supportedVersion = true;

        // use this code block to try to support EnergyPlus versions before they are released
        if (code != SQLITE_ROW){
          LOG(Warn, "Trying unsupported EnergyPlus version 8.2");
          sqlite3_prepare_v2(m_db,"SELECT * FROM Simulations WHERE EnergyPlusVersion LIKE '%8.2%'",-1,&sqlStmtPtr,NULL);
          code = sqlite3_step(sqlStmtPtr);
          sqlite3_finalize(sqlStmtPtr);
          m_supportedVersion = false;
        }

        // use this code block to try to support EnergyPlus versions before they are released
        if (code != SQLITE_ROW){
          LOG(Warn, "Trying unsupported EnergyPlus version 6.0");
          sqlite3_prepare_v2(m_db,"SELECT * FROM Simulations WHERE EnergyPlusVersion LIKE '%6.0%'",-1,&sqlStmtPtr,NULL);
          code = sqlite3_step(sqlStmtPtr);
          sqlite3_finalize(sqlStmtPtr);
          m_supportedVersion = false;
        }
      }
      return (code == SQLITE_ROW);
    }


    int SqlFile_Impl::insertZone(const std::string &t_name,
        double t_relNorth,
        double t_originX, double t_originY, double t_originZ,
        double t_centroidX, double t_centroidY, double t_centroidZ,
        int t_ofType,
        double t_multiplier,
        double t_listMultiplier,
        double t_minimumX, double t_maximumX,
        double t_minimumY, double t_maximumY,
        double t_minimumZ, double t_maximumZ,
        double t_ceilingHeight,
        double t_volume,
        int t_insideConvectionAlgo,
        int t_outsideConvectionAlgo,
        double t_floorArea,
        double t_extGrossWallArea,
        double t_extNetWallArea,
        double t_extWindowArea,
        bool t_isPartOfTotalArea)
    {
      int zoneIndex = getNextIndex("zones", "ZoneIndex");

      std::stringstream insertZone;
      insertZone << "insert into zones (ZoneIndex, ZoneName, RelNorth, OriginX, OriginY, OriginZ, CentroidX, CentroidY, CentroidZ, OfType, Multiplier, ListMultiplier, MinimumX, MaximumX, MinimumY, MaximumY, MinimumZ, MaximumZ, CeilingHeight, Volume, InsideConvectionAlgo, OutsideConvectionAlgo, FloorArea, ExtGrossWallArea, ExtNetWallArea, ExtWindowArea, IsPartOfTotalArea) values ("
        << zoneIndex << ", "
        << "'" << t_name << "', "
        << t_relNorth << ", "
        << t_originX << ", "
        << t_originY << ", "
        << t_originZ << ", "
        << t_centroidX << ", "
        << t_centroidY << ", "
        << t_centroidZ << ", "
        << t_ofType << ", "
        << t_multiplier << ", "
        << t_listMultiplier << ", "
        << t_minimumX << ", "
        << t_maximumX << ", "
        << t_minimumY << ", "
        << t_maximumY << ", "
        << t_minimumZ << ", "
        << t_maximumZ << ", "
        << t_ceilingHeight << ", "
        << t_volume << ", "
        << t_insideConvectionAlgo << ", "
        << t_outsideConvectionAlgo << ", "
        << t_floorArea << ", "
        << t_extGrossWallArea << ", "
        << t_extNetWallArea << ", "
        << t_extWindowArea << ", "
        << t_isPartOfTotalArea << ");";

      execAndThrowOnError(insertZone.str());


      return zoneIndex;
    }


    void SqlFile_Impl::insertIlluminanceMap(const std::string &t_zoneName, const std::string &t_name, 
        const std::string &t_environmentName, const std::vector<DateTime> &t_times,
        const std::vector<double> &t_xs, const std::vector<double> &t_ys, double t_z, const std::vector<Matrix> &t_maps)
    {
      boost::optional<int> zoneIndex = execAndReturnFirstInt("select ZoneIndex from zones where ZoneName='" + t_zoneName + "';");

      if (!zoneIndex)
      {
        throw std::runtime_error("Unknown zone name: " + t_zoneName);
      }

      if (t_times.size() != t_maps.size())
      {
        throw std::runtime_error("Number of times does not match number of maps");
      }

      int mapIndex = getNextIndex("daylightmaps", "MapNumber");

      std::string referencePt1 = "RefPt1=(" + boost::lexical_cast<std::string>(t_xs.front()) + ":" + boost::lexical_cast<std::string>(t_ys.front()) + ":" + boost::lexical_cast<std::string>(t_z) + ")";

      std::string referencePt2 = "RefPt1=(" + boost::lexical_cast<std::string>(t_xs.back()) + ":" + boost::lexical_cast<std::string>(t_ys.back()) + ":" + boost::lexical_cast<std::string>(t_z) + ")";

      std::stringstream mapInsert;
      mapInsert << "insert into daylightmaps (MapNumber, MapName, Environment, Zone, ReferencePt1, ReferencePt2, Z) values ("
        << mapIndex << ", "
        << "'" << t_name << "', "
        << "'" << t_environmentName << "', "
        << *zoneIndex << ", "
        << "'" << referencePt1 << "', "
        << "'" << referencePt2 << "', "
        << t_z << ");";

      execAndThrowOnError(mapInsert.str());


      int hourlyReportIndex = getNextIndex("daylightmaphourlyreports", "HourlyReportIndex");

      // we'll let stmt1 have the transaction
      PreparedStatement stmt1("insert into daylightmaphourlyreports (HourlyReportIndex, MapNumber, Month, DayOfMonth, Hour) values (?, ?, ?, ?, ?)", m_db, true);

      for (size_t dateidx = 0; dateidx < t_times.size(); ++dateidx)
      {
        stmt1.bind(1, hourlyReportIndex);
        stmt1.bind(2, mapIndex);

        DateTime dt = t_times[dateidx];

        int monthOfYear = dt.date().monthOfYear().value();
        int dayOfMonth = dt.date().dayOfMonth();
        int hours = dt.time().hours();
        // EnergyPlus deals in a 00:00:01 -> 24:00:00 world instead of
        // 00:00:00 -> 23:59:59 hrs that the real world uses, so
        // we are going to adjust for that, so we subtract an hour,
        // to get it back to the previous day, then we will replace
        // the hours with 24
        // \sa illuminanceMapHourlyReportIndex
        if (hours == 0)
        {
          dt -= Time(0, 1);
          monthOfYear = dt.date().monthOfYear().value();
          dayOfMonth = dt.date().dayOfMonth();
          hours = 24;
        }


        stmt1.bind(3, monthOfYear);
        stmt1.bind(4, dayOfMonth);
        stmt1.bind(5, hours);
        stmt1.execute();

        if (t_xs.size() != t_maps[dateidx].size1()
            || t_ys.size() != t_maps[dateidx].size2())
        {
          throw std::runtime_error("map size does not match given x's and y's");
        }

        for (size_t xidx = 0; xidx < t_xs.size(); ++xidx)
        {
          for (size_t yidx = 0; yidx < t_ys.size(); ++yidx)
          {
            // we are already inside of a transaction from stmt1, so not creating a new one here
            PreparedStatement stmt2("insert into daylightmaphourlydata (HourlyReportIndex, X, Y, Illuminance) values (?, ?, ?, ?)", m_db, false);

            stmt2.bind(1, hourlyReportIndex);
            stmt2.bind(2, t_xs[xidx]);
            stmt2.bind(3, t_ys[yidx]);
            stmt2.bind(4, t_maps[dateidx](xidx, yidx));

            stmt2.execute();
          }
        }

        ++hourlyReportIndex;
      }
    }


    void SqlFile_Impl::insertTimeSeriesData(const std::string &t_variableType, const std::string &t_indexGroup, 
        const std::string &t_timestepType, const std::string &t_keyValue, const std::string &t_variableName,
        const openstudio::ReportingFrequency &t_reportingFrequency, const boost::optional<std::string> &t_scheduleName,
        const std::string &t_variableUnits, const openstudio::TimeSeries &t_timeSeries)
    {
      int datadicindex = getNextIndex("reportvariabledatadictionary", "ReportVariableDataDictionaryIndex");


      std::stringstream insertReportVariableDataDictionary;
      insertReportVariableDataDictionary
        << "insert into reportvariabledatadictionary (ReportVariableDataDictionaryIndex, VariableType, IndexGroup, TimestepType, KeyValue, VariableName, ReportingFrequency, ScheduleName, VariableUnits) values ("
        << datadicindex << ", "
        << "'" << t_variableType << "', "
        << "'" << t_indexGroup << "', "
        << "'" << t_timestepType << "', "
        << "'" << t_keyValue << "', "
        << "'" << t_variableName << "', "
        << "'" << t_reportingFrequency.valueName() << "', ";

      if (t_scheduleName)
      {
        insertReportVariableDataDictionary
          << "'" << *t_scheduleName << "', ";
      } else {
        insertReportVariableDataDictionary
          << "null, ";
      }

      insertReportVariableDataDictionary
        << "'" << t_variableUnits << "');";


      execAndThrowOnError(insertReportVariableDataDictionary.str());


      std::vector<double> values = toStandardVector(t_timeSeries.values());
      std::vector<double> days = toStandardVector(t_timeSeries.daysFromFirstReport());

      openstudio::DateTime firstdate = t_timeSeries.firstReportDateTime();

      // we'll let stmt1 have the transaction
      PreparedStatement stmt("insert into reportvariabledata (TimeIndex, ReportVariableDataDictionaryIndex, VariableValue, ReportVariableExtendedDataIndex) values ( (select TimeIndex from time where Month=? and Day=? and Hour=? and Minute=? limit 1), ?, ?, null);", m_db, true);

      for (size_t i = 0; i < values.size(); ++i)
      {
        openstudio::DateTime dt = firstdate + openstudio::Time(days[i]);
        double value = values[i];

        if (dt.time().seconds() == 59) 
        {
          // rounding error, let's help
          dt += openstudio::Time(0,0,0,1);
        }

        if (dt.time().seconds() == 1) 
        {
          // rounding error, let's help
          dt -= openstudio::Time(0,0,0,1);
        }

        int month = dt.date().monthOfYear().value();
        int day = dt.date().dayOfMonth();
        int hour = dt.time().hours();
        int minute = dt.time().minutes();

        ++hour; // energyplus says time goes from 1-24 not from 0-23

        stmt.bind(1, month);
        stmt.bind(2, day);
        stmt.bind(3, hour);
        stmt.bind(4, minute);
        stmt.bind(5, datadicindex);
        stmt.bind(6, value);

        stmt.execute();
      }
    }

    std::vector<SummaryData> SqlFile_Impl::getSummaryData() const
    {
      std::vector<SummaryData> retval;

      if (m_db)
      {
        sqlite3_stmt* sqlStmtPtr;

        std::string stmt = 
          "select sum(VariableValue), VariableName, ReportingFrequency, VariableUnits "
          "from ReportMeterData, ReportMeterDataDictionary "
          "where (ReportMeterData.ReportMeterDataDictionaryIndex = ReportMeterDataDictionary.ReportMeterDataDictionaryIndex) "
          "  and VariableType='Sum' "
          "  group by VariableName, ReportingFrequency, VariableUnits";

        sqlite3_prepare_v2(m_db,stmt.c_str(),-1,&sqlStmtPtr,NULL);
        while(sqlite3_step(sqlStmtPtr) == SQLITE_ROW)
        {
          double value = sqlite3_column_double(sqlStmtPtr, 0);
          std::string variablename = columnText(sqlite3_column_text(sqlStmtPtr, 1));
          std::string reportingfrequency = columnText(sqlite3_column_text(sqlStmtPtr, 2));
          std::string units = columnText(sqlite3_column_text(sqlStmtPtr, 3));

          size_t colon = variablename.find(':');

          if (colon == std::string::npos)
          {
            LOG(Error, "Unable to parse variable name, no ':' found " + variablename);
          } else {
            std::string fueltype = variablename.substr(0, colon);
            std::string installlocation = variablename.substr(colon+1);

            try {
              retval.push_back(
                  SummaryData(
                    value,
                    openstudio::parseUnitString(units),
                    openstudio::ReportingFrequency(reportingfrequency),
                    openstudio::FuelType(fueltype),
                    openstudio::InstallLocationType(installlocation)
                    )

                  );
            } catch (const std::exception &e) {
              LOG(Error, "Error adding / parsing summary data: " << e.what());
            }
          }
        }

        sqlite3_finalize(sqlStmtPtr);
      }

      return retval;
    }


    void SqlFile_Impl::retrieveDataDictionary()
    {
      std::string table, name, keyValue, units, rf;

      if (m_db)
      {
        int dictionaryIndex, code;

        std::stringstream s;
        sqlite3_stmt* sqlStmtPtr;
        std::map<int,std::string> envPeriods;
        std::map<int,std::string>::iterator envPeriodsItr;

        s << "SELECT EnvironmentPeriodIndex, EnvironmentName FROM EnvironmentPeriods";
        sqlite3_prepare_v2(m_db,s.str().c_str(),-1,&sqlStmtPtr,NULL);
        code = sqlite3_step(sqlStmtPtr);
        while(code == SQLITE_ROW)
        {
          std::string queryEnvPeriod = boost::to_upper_copy(columnText(sqlite3_column_text(sqlStmtPtr,1)));
          envPeriods.insert(std::pair<int,std::string>( sqlite3_column_int(sqlStmtPtr,0), queryEnvPeriod ) );
          code = sqlite3_step(sqlStmtPtr);
        }
        sqlite3_finalize(sqlStmtPtr);

        s.str("");
        s << "SELECT ReportMeterDataDictionaryIndex, VariableName, KeyValue, ReportingFrequency, VariableUnits";
        s << " FROM ReportMeterDataDictionary";
        code = sqlite3_prepare_v2(m_db,s.str().c_str(),-1,&sqlStmtPtr,NULL);

        table= "ReportMeterData";

        code = sqlite3_step(sqlStmtPtr);
        while (code == SQLITE_ROW)
        {
          dictionaryIndex = sqlite3_column_int(sqlStmtPtr,0);
          name = columnText(sqlite3_column_text(sqlStmtPtr,1));
          keyValue = columnText(sqlite3_column_text(sqlStmtPtr,2));
          rf = columnText(sqlite3_column_text(sqlStmtPtr,3));
          units = columnText(sqlite3_column_text(sqlStmtPtr,4));

          for(envPeriodsItr = envPeriods.begin();
              envPeriodsItr != envPeriods.end();
              ++envPeriodsItr)
          {
            std::string queryEnvPeriod = boost::to_upper_copy(envPeriodsItr->second);
            m_dataDictionary.insert(DataDictionaryItem(dictionaryIndex,envPeriodsItr->first,name,keyValue,queryEnvPeriod,rf,units,table));
            LOG(Trace,"Creating data dictionary item " << dictionaryIndex << ", " << (*envPeriodsItr).first 
                << ", " << name << ", " << keyValue << ", " << queryEnvPeriod << ", " << rf << ", " 
                << units << ", " << table << ".");
          }

          code = sqlite3_step(sqlStmtPtr);
        }
        sqlite3_finalize(sqlStmtPtr);

        s.str("");
        s << "SELECT ReportVariableDatadictionaryIndex, VariableName, KeyValue, ReportingFrequency, VariableUnits";
        s << " FROM ReportVariableDatadictionary";
        code = sqlite3_prepare_v2(m_db,s.str().c_str(),-1,&sqlStmtPtr,NULL);

        table= "ReportVariableData";

        code = sqlite3_step(sqlStmtPtr);
        while (code == SQLITE_ROW)
        {
          dictionaryIndex = sqlite3_column_int(sqlStmtPtr,0);
          name = columnText(sqlite3_column_text(sqlStmtPtr,1));
          keyValue = columnText(sqlite3_column_text(sqlStmtPtr,2));
          rf = columnText(sqlite3_column_text(sqlStmtPtr,3));
          units = columnText(sqlite3_column_text(sqlStmtPtr,4));

          for(envPeriodsItr = envPeriods.begin();
              envPeriodsItr != envPeriods.end();
              ++envPeriodsItr)
          {
            std::string queryEnvPeriod = boost::to_upper_copy(envPeriodsItr->second);
            m_dataDictionary.insert(DataDictionaryItem(dictionaryIndex,envPeriodsItr->first,name,keyValue,queryEnvPeriod,rf,units,table));
          }

          // step to next row
          code = sqlite3_step(sqlStmtPtr);
        }
        sqlite3_finalize(sqlStmtPtr);
      }
      LOG(Debug,"Dictionary Built");
    }

    boost::optional<double> SqlFile_Impl::energyConsumptionByMonth(
        const openstudio::EndUseFuelType &t_fuelType,
        const openstudio::EndUseCategoryType &t_categoryType,
        const openstudio::MonthOfYear &t_monthOfYear) const
    {
      const std::string reportname = "BUILDING ENERGY PERFORMANCE - " + boost::algorithm::to_upper_copy(t_fuelType.valueDescription());
      const std::string columnname = boost::algorithm::to_upper_copy(t_categoryType.valueName()) + ":" + 
        boost::algorithm::to_upper_copy(t_fuelType.valueName());
      const std::string rowname = t_monthOfYear.valueDescription();

      const std::string& s = "SELECT Value FROM tabulardatawithstrings WHERE \
                              ReportName='" + reportname + "' and \
                              ReportForString='Meter' AND \
                              RowName='" + rowname + "' AND \
                              ColumnName='" + columnname + "' AND \
                              Units='J'";

      return execAndReturnFirstDouble(s);
    }
    
    //TODO
    boost::optional<double> SqlFile_Impl::peakEnergyDemandByMonth(
        const openstudio::EndUseFuelType &t_fuelType,
        const openstudio::EndUseCategoryType &t_categoryType,
        const openstudio::MonthOfYear &t_monthOfYear) const
    {
      const std::string reportname = "BUILDING ENERGY PERFORMANCE - " + boost::algorithm::to_upper_copy(t_fuelType.valueDescription()) + " PEAK DEMAND";
      const std::string columnname = boost::algorithm::to_upper_copy(t_categoryType.valueName()) + ":" + 
        boost::algorithm::to_upper_copy(t_fuelType.valueName()) + 
        " {AT MAX/MIN}";
      const std::string rowname = t_monthOfYear.valueDescription();

      const std::string& s = "SELECT Value FROM tabulardatawithstrings WHERE \
                              ReportName='" + reportname + "' and \
                              ReportForString='Meter' AND \
                              RowName='" + rowname + "' AND \
                              ColumnName='" + columnname + "' AND \
                              Units='W'";

      return execAndReturnFirstDouble(s);
    }

    /// hours simulated
    boost::optional<double> SqlFile_Impl::hoursSimulated() const
    {
      const std::string& s = "SELECT Value FROM tabulardatawithstrings WHERE \
                              ReportName='InputVerificationandResultsSummary' AND \
                              ReportForString='Entire Facility' AND \
                              TableName='General' AND \
                              RowName='Hours Simulated' AND \
                              Units='hrs'";
      boost::optional<double> ret = execAndReturnFirstDouble(s);

      if (ret) return ret;

      // Otherwise, let's try to calculate it:
      return execAndReturnFirstDouble(
        "select "
        "    (select max(t.hour + ((t.simulationdays-1) * 24)) as mintime from time t join reportmeterdata r on (t.timeindex=r.timeindex))"
        "  - (select min(t.hour + ((t.simulationdays-1) * 24)) as mintime from time t join reportmeterdata r on (t.timeindex=r.timeindex))"
        "  + 1;"
       );

    }

    boost::optional<double> SqlFile_Impl::netSiteEnergy() const
    {
      boost::optional<double> hours = hoursSimulated();
      if (!hours){
        LOG(Warn, "Reporting Net Site Energy with unknown number of simulation hours");
      }else if(*hours != 8760){
        LOG(Warn, "Reporting Net Site Energy with " << *hours << " hrs");
      }

      std::string s = "SELECT Value FROM tabulardatawithstrings WHERE \
                              ReportName='AnnualBuildingUtilityPerformanceSummary' AND \
                              ReportForString='Entire Facility' AND \
                              TableName='Site and Source Energy' AND \
                              RowName='Net Site Energy' AND \
                              ColumnName='Total Energy' AND \
                              Units='GJ'";
      boost::optional<double> d = execAndReturnFirstDouble(s);

      if (!d) {
        LOG(Warn, "Tabular results were not found, trying to calculate it ourselves");
        std::string s = " \
          select sum(VariableValue)/1000000000 from ReportMeterData, ReportMeterDataDictionary \
          where (ReportMeterData.ReportMeterDataDictionaryIndex = ReportMeterDataDictionary.ReportMeterDataDictionaryIndex and variablename not like '%EnergyTransfer%')\
          group by ReportingFrequency;\
          ";
        d = execAndReturnFirstDouble(s);
      }

      return d;
    }

    boost::optional<double> SqlFile_Impl::netSourceEnergy() const
    {
      boost::optional<double> hours = hoursSimulated();
      if (!hours){
        LOG(Warn, "Reporting Net Source Energy with unknown number of simulation hours");
      }else if(*hours != 8760){
        LOG(Warn, "Reporting Net Source Energy with " << *hours << " hrs");
      }

      const std::string& s = "SELECT Value FROM tabulardatawithstrings WHERE \
                              ReportName='AnnualBuildingUtilityPerformanceSummary' AND \
                              ReportForString='Entire Facility' AND \
                              TableName='Site and Source Energy' AND \
                              RowName='Net Source Energy' AND \
                              ColumnName='Total Energy' AND \
                              Units='GJ'";
      return execAndReturnFirstDouble(s);
    }


    boost::optional<double> SqlFile_Impl::totalSiteEnergy() const
    {
      boost::optional<double> hours = hoursSimulated();
      if (!hours){
        LOG(Warn, "Reporting Total Site Energy with unknown number of simulation hours");
      }else if(*hours != 8760){
        LOG(Warn, "Reporting Total Site Energy with " << *hours << " hrs");
      }

      const std::string& s = "SELECT Value FROM tabulardatawithstrings WHERE \
                              ReportName='AnnualBuildingUtilityPerformanceSummary' AND \
                              ReportForString='Entire Facility' AND \
                              TableName='Site and Source Energy' AND \
                              RowName='Total Site Energy' AND \
                              ColumnName='Total Energy' AND \
                              Units='GJ'";
      return execAndReturnFirstDouble(s);
    }


    boost::optional<double> SqlFile_Impl::totalSourceEnergy() const
    {
      boost::optional<double> hours = hoursSimulated();
      if (!hours){
        LOG(Warn, "Reporting Total Source Energy with unknown number of simulation hours");
      }else if(*hours != 8760){
        LOG(Warn, "Reporting Total Source Energy with " << *hours << " hrs");
      }

      const std::string& s = "SELECT Value FROM tabulardatawithstrings WHERE \
                              ReportName='AnnualBuildingUtilityPerformanceSummary' AND \
                              ReportForString='Entire Facility' AND \
                              TableName='Site and Source Energy' AND \
                              RowName='Total Source Energy' AND \
                              ColumnName='Total Energy' AND \
                              Units='GJ'";
      return execAndReturnFirstDouble(s);
    }


    OptionalDouble SqlFile_Impl::annualTotalCost(const FuelType& fuel) const
    {
      if (fuel == FuelType::Electricity){
        return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'Economics Results Summary Report') and (ReportForString = 'Entire Facility') and (TableName = 'Annual Cost') and (ColumnName ='Electric') and (((RowName = 'Cost') and (Units = '~~$~~')) or (RowName = 'Cost (~~$~~)'))");
      } else if (fuel == FuelType::Gas){
        return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'Economics Results Summary Report') and (ReportForString = 'Entire Facility') and (TableName = 'Annual Cost') and (ColumnName ='Gas') and (((RowName = 'Cost') and (Units = '~~$~~')) or (RowName = 'Cost (~~$~~)'))");
      } else if (fuel == FuelType::DistrictCooling){
        return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'Economics Results Summary Report') and (ReportForString = 'Entire Facility') and (TableName = 'Annual Cost') and (ColumnName ='District Cooling') and (((RowName = 'Cost') and (Units = '~~$~~')) or (RowName = 'Cost (~~$~~)'))");
      } else if (fuel == FuelType::DistrictHeating){
        return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'Economics Results Summary Report') and (ReportForString = 'Entire Facility') and (TableName = 'Annual Cost') and (ColumnName ='District Heating') and (((RowName = 'Cost') and (Units = '~~$~~')) or (RowName = 'Cost (~~$~~)'))");
      } else if (fuel == FuelType::Water){
        return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'Economics Results Summary Report') and (ReportForString = 'Entire Facility') and (TableName = 'Annual Cost') and (ColumnName ='Water') and (((RowName = 'Cost') and (Units = '~~$~~')) or (RowName = 'Cost (~~$~~)'))");
      } else{
        return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'Economics Results Summary Report') and (ReportForString = 'Entire Facility') and (TableName = 'Annual Cost') and (ColumnName ='Other') and (((RowName = 'Cost') and (Units = '~~$~~')) or (RowName = 'Cost (~~$~~)'))");
      }
    }


    OptionalDouble SqlFile_Impl::annualTotalCostPerBldgArea(const FuelType& fuel) const
    {
      if (fuel == FuelType::Electricity){
        return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'Economics Results Summary Report') and (ReportForString = 'Entire Facility') and (TableName = 'Annual Cost') and (ColumnName ='Electric') and (((RowName = 'Cost per Total Building Area') and (Units = '~~$~~/m2')) or (RowName = 'Cost per Total Building Area (~~$~~/m2)'))");
      } else if (fuel == FuelType::Gas){
        return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'Economics Results Summary Report') and (ReportForString = 'Entire Facility') and (TableName = 'Annual Cost') and (ColumnName ='Gas') and (((RowName = 'Cost per Total Building Area') and (Units = '~~$~~/m2')) or (RowName = 'Cost per Total Building Area (~~$~~/m2)'))");
      } else if (fuel == FuelType::DistrictCooling){
        return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'Economics Results Summary Report') and (ReportForString = 'Entire Facility') and (TableName = 'Annual Cost') and (ColumnName ='District Cooling') and (((RowName = 'Cost per Total Building Area') and (Units = '~~$~~/m2')) or (RowName = 'Cost per Total Building Area (~~$~~/m2)'))");
      } else if (fuel == FuelType::DistrictHeating){
        return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'Economics Results Summary Report') and (ReportForString = 'Entire Facility') and (TableName = 'Annual Cost') and (ColumnName ='District Heating') and (((RowName = 'Cost per Total Building Area') and (Units = '~~$~~/m2')) or (RowName = 'Cost per Total Building Area (~~$~~/m2)'))");
      } else if (fuel == FuelType::Water){
        return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'Economics Results Summary Report') and (ReportForString = 'Entire Facility') and (TableName = 'Annual Cost') and (ColumnName ='Water') and (((RowName = 'Cost per Total Building Area') and (Units = '~~$~~/m2')) or (RowName = 'Cost per Total Building Area (~~$~~/m2)'))");
      } else {
        return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'Economics Results Summary Report') and (ReportForString = 'Entire Facility') and (TableName = 'Annual Cost') and (ColumnName ='Other') and (((RowName = 'Cost per Total Building Area') and (Units = '~~$~~/m2')) or (RowName = 'Cost per Total Building Area (~~$~~/m2)'))");
      }
    }

    OptionalDouble SqlFile_Impl::annualTotalCostPerNetConditionedBldgArea(const FuelType& fuel) const
    {
      if (fuel == FuelType::Electricity){
        return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'Economics Results Summary Report') and (ReportForString = 'Entire Facility') and (TableName = 'Annual Cost') and (ColumnName ='Electric') and (((RowName = 'Cost per Net Conditioned Building Area') and (Units = '~~$~~/m2')) or (RowName = 'Cost per Total Building Area (~~$~~/m2)'))");
      } else if (fuel == FuelType::Gas){
        return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'Economics Results Summary Report') and (ReportForString = 'Entire Facility') and (TableName = 'Annual Cost') and (ColumnName ='Gas') and (((RowName = 'Cost per Net Conditioned Building Area') and (Units = '~~$~~/m2')) or (RowName = 'Cost per Total Building Area (~~$~~/m2)'))");
      } else if (fuel == FuelType::DistrictCooling){
        return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'Economics Results Summary Report') and (ReportForString = 'Entire Facility') and (TableName = 'Annual Cost') and (ColumnName ='District Cooling') and (((RowName = 'Cost per Net Conditioned Building Area') and (Units = '~~$~~/m2')) or (RowName = 'Cost per Total Building Area (~~$~~/m2)'))");
      } else if (fuel == FuelType::DistrictHeating){
        return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'Economics Results Summary Report') and (ReportForString = 'Entire Facility') and (TableName = 'Annual Cost') and (ColumnName ='District Heating') and (((RowName = 'Cost per Net Conditioned Building Area') and (Units = '~~$~~/m2')) or (RowName = 'Cost per Total Building Area (~~$~~/m2)'))");
      } else if (fuel == FuelType::Water){
        return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'Economics Results Summary Report') and (ReportForString = 'Entire Facility') and (TableName = 'Annual Cost') and (ColumnName ='Water') and (((RowName = 'Cost per Net Conditioned Building Area') and (Units = '~~$~~/m2')) or (RowName = 'Cost per Total Building Area (~~$~~/m2)'))");
      } else {
        return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'Economics Results Summary Report') and (ReportForString = 'Entire Facility') and (TableName = 'Annual Cost') and (ColumnName ='Other') and (((RowName = 'Cost per Net Conditioned Building Area') and (Units = '~~$~~/m2')) or (RowName = 'Cost per Total Building Area (~~$~~/m2)'))");
      }
    }

    boost::optional<double> SqlFile_Impl::annualTotalUtilityCost() const
    {
      boost::optional<double> annualElectricTotalCost = this->annualTotalCost(FuelType::Electricity);
      boost::optional<double> annualGasTotalCost = this->annualTotalCost(FuelType::Gas);
      boost::optional<double> annualDistrictCoolingTotalCost = this->annualTotalCost(FuelType::DistrictCooling);
      boost::optional<double> annualDistrictHeatingTotalCost = this->annualTotalCost(FuelType::DistrictHeating);
      boost::optional<double> annualWaterTotalCost = this->annualTotalCost(FuelType::Water);

      if (annualElectricTotalCost || annualGasTotalCost || annualDistrictCoolingTotalCost || annualDistrictHeatingTotalCost || annualWaterTotalCost){
        double result = 0.0;
        if(annualElectricTotalCost){
          result += *annualElectricTotalCost;
        }
        if(annualGasTotalCost){
          result += *annualGasTotalCost;
        }
        if(annualDistrictCoolingTotalCost){
          result += *annualDistrictCoolingTotalCost;
        }
        if(annualDistrictHeatingTotalCost){
          result += *annualDistrictHeatingTotalCost;
        }
        if(annualWaterTotalCost){
          result += *annualWaterTotalCost;
        }
        return result;
      } else {
        return boost::optional<double>();
      }
    }


    std::vector<std::string> SqlFile_Impl::availableTimeSeries()
    {
      std::vector<std::string> vec;
      std::string timeSeriesName;
      DataDictionaryTable::index<name>::type::iterator iname;
      for (iname=m_dataDictionary.get<name>().begin(); iname!=m_dataDictionary.get<name>().end(); ++iname)
      {
        timeSeriesName = (*iname).name;
        if (std::find(vec.begin(), vec.end(), timeSeriesName) == vec.end())
        {
          vec.push_back(timeSeriesName);
        }
      }
      return vec;
    };

    std::vector<std::string> SqlFile_Impl::availableVariableNames(const std::string& envPeriod, const std::string& reportingFrequency) const
    {
      std::string queryEnvPeriod = boost::to_upper_copy(envPeriod);

      std::vector<std::string> vec;
      std::string variableName;
      DataDictionaryTable::index<name>::type::iterator iname;
      for (iname=m_dataDictionary.get<name>().begin(); iname!=m_dataDictionary.get<name>().end(); ++iname)
      {
        if ((iname->envPeriod == queryEnvPeriod) && (iname->reportingFrequency == reportingFrequency))
        {
          variableName = iname->name;
          if (std::find(vec.begin(), vec.end(), variableName) == vec.end())
          {
            vec.push_back(variableName);
          }
        }
      }
      return vec;
    }

    std::vector<std::string> SqlFile_Impl::availableReportingFrequencies(const std::string& envPeriod)
    {
      std::string queryEnvPeriod = boost::to_upper_copy(envPeriod);

      std::vector<std::string> vec;
      std::string reportingFrequencyName;
      DataDictionaryTable::index<reportingFrequency>::type::iterator ireportingFrequency;
      for (ireportingFrequency=m_dataDictionary.get<reportingFrequency>().begin(); ireportingFrequency!=m_dataDictionary.get<reportingFrequency>().end(); ++ireportingFrequency)
      {
        if (ireportingFrequency->envPeriod == queryEnvPeriod)
        {
          reportingFrequencyName = ireportingFrequency->reportingFrequency;
          if (std::find(vec.begin(), vec.end(), reportingFrequencyName) == vec.end())
          {
            vec.push_back(reportingFrequencyName);
          }
        }
      }
      return vec;
    };

    ReportingFrequency SqlFile_Impl::reportingFrequencyFromDB(const std::string &dbReportingFrequency)
    {
      // EP+ version specific translation
      // use OPENSTUDIO_ENUM string handling
      try {
        ReportingFrequency result(dbReportingFrequency);
        return result;
      }
      catch (...) {
        // default value -- may want to return optional
        return openstudio::ReportingFrequency::RunPeriod;
      }
    };


    boost::optional<EnvironmentType> SqlFile_Impl::environmentType(const std::string& envPeriod) const
    {
      boost::optional<EnvironmentType> result;
      std::string query = "SELECT EnvironmentType FROM environmentperiods WHERE EnvironmentName='" + envPeriod + "' COLLATE NOCASE";
      boost::optional<int> temp = execAndReturnFirstInt(query);
      if (temp){
        try{
          result = EnvironmentType(*temp);
        }catch(...){
          LOG(Error, "Could not convert integer value " << *temp << " to EnvironmentType");
        }
      }
      return result;
    }

    OptionalDouble SqlFile_Impl::economicsEnergyCost() const
    {
      return execAndReturnFirstDouble("SELECT * from tabulardatawithstrings where (reportname = 'Economics Results Summary Report') and (ReportForString = 'Entire Facility') and (TableName = 'Annual Cost') and (ColumnName ='Total') and (((RowName = 'Cost') and (Units = '~~$~~')) or (RowName = 'Cost (~~$~~)'))");
    }

    OptionalDouble SqlFile_Impl::getElecOrGasUse(bool bGetGas) const
    {
      boost::optional<std::vector<std::string> > selectedRowNames;

      boost::optional<std::vector<std::string> > qualifiedRowNames;
      boost::optional<std::vector<std::string> > fuelTypeRowNames;
      OptionalDouble result;

      std::string fuelType;
      if(bGetGas){
        fuelType = "'COMM GAS'";
      }
      else{
        fuelType = "'COMM ELECT'";
      }

      std::string query("select rowname from tabulardatawithstrings where TableName = 'Tariff Summary' and ColumnName = 'Group' and Value = ");
      query += fuelType;

      selectedRowNames = execAndReturnVectorOfString("select rowname from tabulardatawithstrings where TableName = 'Tariff Summary' and (ColumnName = 'Selected' and Value = 'Yes')");
      qualifiedRowNames = execAndReturnVectorOfString("select rowname from tabulardatawithstrings where TableName = 'Tariff Summary' and (ColumnName = 'Qualified' and Value = 'Yes')");
      fuelTypeRowNames = execAndReturnVectorOfString(query);

      if(!selectedRowNames || !qualifiedRowNames || !fuelTypeRowNames) return result;

      std::vector<std::string> names;
      for(unsigned i=0; i<selectedRowNames->size(); i++){
        for(unsigned j=0; j<qualifiedRowNames->size(); j++){
          if(selectedRowNames->at(i) == qualifiedRowNames->at(j)){
            names.push_back(selectedRowNames->at(i));
          }
        }
      }

      std::string name;
      for(unsigned i=0; i<names.size(); i++){
        for(unsigned j=0; j<fuelTypeRowNames->size(); j++){
          if(names.at(i) == fuelTypeRowNames->at(j)){
            name = names.at(i);
            break;
          }
        }
      }
      if(name.size() == 0) return result;

      query = "SELECT value from tabulardatawithstrings where ReportName = 'Tariff Report' and ReportForString = '";
      query += name;
      query +=  "' and TableName = 'Native Variables' and ColumnName = 'Sum' and RowName = 'TotalEnergy'";
      result = execAndReturnFirstDouble(query);

      return result;
    }


    OptionalDouble SqlFile_Impl::getElecOrGasCost(bool bGetGas) const
    {
      std::string fuelType;
      if(bGetGas){
        fuelType = "'Gas'";
      }
      else{
        fuelType = "'Electric'";
      }

      std::string query("select value from tabulardatawithstrings where TableName = 'Annual Cost' and (((RowName = 'Cost') and (Units = '~~$~~')) or (RowName = 'Cost (~~$~~)')) and ColumnName = ");
      query += fuelType;

      return execAndReturnFirstDouble(query);
    }

    boost::optional<EndUses> SqlFile_Impl::endUses() const
    {
      EndUses result;

      BOOST_FOREACH(EndUseFuelType fuelType, result.fuelTypes()){
        std::string units = result.getUnitsForFuelType(fuelType);
        BOOST_FOREACH(EndUseCategoryType category, result.categories()){

          std::string query = "SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='" + \
                               fuelType.valueDescription() + "') and (RowName ='" + category.valueDescription() + "') and (Units = '" + units + "')";

          boost::optional<double> value = execAndReturnFirstDouble(query);
          OS_ASSERT(value);

          if (*value != 0.0){
            result.addEndUse(*value, fuelType, category);
          }
        }
      }

      return result;
    }



    OptionalDouble SqlFile_Impl::electricityHeating() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Electricity') and (RowName = 'Heating') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::electricityCooling() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Electricity') and (RowName = 'Cooling') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::electricityInteriorLighting() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Electricity') and (RowName = 'Interior Lighting') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::electricityExteriorLighting() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Electricity') and (RowName = 'Exterior Lighting') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::electricityInteriorEquipment() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Electricity') and (RowName = 'Interior Equipment') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::electricityExteriorEquipment() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Electricity') and (RowName = 'Exterior Equipment') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::electricityFans() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Electricity') and (RowName = 'Fans') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::electricityPumps() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Electricity') and (RowName = 'Pumps') and (Units = 'GJ')");
    }


    OptionalDouble SqlFile_Impl::electricityHeatRejection() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Electricity') and (RowName ='Heat Rejection') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::electricityHumidification() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Electricity') and (RowName = 'Humidification') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::electricityHeatRecovery() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Electricity') and (RowName ='Heat Recovery') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::electricityWaterSystems() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Electricity') and (RowName ='Water Systems') and (Units = 'GJ')");
    }


    OptionalDouble SqlFile_Impl::electricityRefrigeration() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Electricity') and (RowName ='Refrigeration') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::electricityGenerators() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Electricity') and (RowName ='Generators') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::electricityTotalEndUses() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Electricity') and (RowName ='Total End Uses') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::naturalGasHeating() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Natural Gas') and (RowName ='Heating') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::naturalGasCooling() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Natural Gas') and (RowName ='Cooling') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::naturalGasInteriorLighting() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Natural Gas') and (RowName ='Interior Lighting') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::naturalGasExteriorLighting() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Natural Gas') and (RowName ='Exterior Lighting') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::naturalGasInteriorEquipment() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Natural Gas') and (RowName ='Interior Equipment') and (Units = 'GJ')");
    }
    OptionalDouble SqlFile_Impl::naturalGasExteriorEquipment() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Natural Gas') and (RowName ='Exterior Equipment') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::naturalGasFans() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Natural Gas') and (RowName ='Fans') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::naturalGasPumps() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Natural Gas') and (RowName ='Pumps') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::naturalGasHeatRejection() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Natural Gas') and (RowName ='Heat Rejection') and (Units = 'GJ')");
    }


    OptionalDouble SqlFile_Impl::naturalGasHumidification() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Natural Gas') and (RowName ='Humidification') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::naturalGasHeatRecovery() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Natural Gas') and (RowName ='Heat Recovery') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::naturalGasWaterSystems() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Natural Gas') and (RowName ='Water Systems') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::naturalGasRefrigeration() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Natural Gas') and (RowName ='Refrigeration') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::naturalGasGenerators() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Natural Gas') and (RowName ='Generators') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::naturalGasTotalEndUses() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Natural Gas') and (RowName ='Total End Uses') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::otherFuelHeating() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Additional Fuel') and (RowName ='Heating') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::otherFuelCooling() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Additional Fuel') and (RowName ='Cooling') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::otherFuelInteriorLighting() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Additional Fuel') and (RowName ='Interior Lighting') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::otherFuelExteriorLighting() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Additional Fuel') and (RowName ='Exterior Lighting') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::otherFuelInteriorEquipment() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Additional Fuel') and (RowName ='Interior Equipment') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::otherFuelExteriorEquipment() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Additional Fuel') and (RowName ='Exterior Equipment') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::otherFuelFans() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Additional Fuel') and (RowName ='Fans') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::otherFuelPumps() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Additional Fuel') and (RowName ='Pumps') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::otherFuelHeatRejection() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Additional Fuel') and (RowName ='Heat Rejection') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::otherFuelHumidification() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Additional Fuel') and (RowName ='Humidification') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::otherFuelHeatRecovery() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Additional Fuel') and (RowName ='Heat Recovery') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::otherFuelWaterSystems() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Additional Fuel') and (RowName ='Water Systems') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::otherFuelRefrigeration() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Additional Fuel') and (RowName ='Refrigeration') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::otherFuelGenerators() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Additional Fuel') and (RowName ='Generators') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::otherFuelTotalEndUses() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Additional Fuel') and (RowName ='Total End Uses') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtCoolingHeating() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Cooling') and (RowName ='Heating') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtCoolingCooling() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Cooling') and (RowName ='Cooling') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtCoolingInteriorLighting() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Cooling') and (RowName ='Interior Lighting') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtCoolingExteriorLighting() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Cooling') and (RowName ='Exterior Lighting') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtCoolingInteriorEquipment() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Cooling') and (RowName ='Interior Equipment') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtCoolingExteriorEquipment() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Cooling') and (RowName ='Exterior Equipment') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtCoolingFans() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Cooling') and (RowName ='Fans') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtCoolingPumps() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Cooling') and (RowName ='Pumps') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtCoolingHeatRejection() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Cooling') and (RowName ='Heat Rejection') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtCoolingHumidification() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Cooling') and (RowName ='Humidification') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtCoolingHeatRecovery() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Cooling') and (RowName ='Heat Recovery') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtCoolingWaterSystems() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Cooling') and (RowName ='Water Systems') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtCoolingRefrigeration() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Cooling') and (RowName ='Refrigeration') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtCoolingGenerators() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Cooling') and (RowName ='Generators') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtCoolingTotalEndUses() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Cooling') and (RowName ='Total End Uses') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtHeatingHeating() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Heating') and (RowName ='Heating') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtHeatingCooling() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Heating') and (RowName ='Cooling') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtHeatingInteriorLighting() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Heating') and (RowName ='Interior Lights') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtHeatingExteriorLighting() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Heating') and (RowName ='Exterior Lights') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtHeatingInteriorEquipment() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Heating') and (RowName ='Interior Equipment') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtHeatingExteriorEquipment() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Heating') and (RowName ='Exterior Equipment') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtHeatingFans() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Heating') and (RowName ='Fans') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtHeatingPumps() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Heating') and (RowName ='Pumps') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtHeatingHeatRejection() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Heating') and (RowName ='Heat Rejection') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtHeatingHumidification() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Heating') and (RowName ='Humidification') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtHeatingHeatRecovery() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Heating') and (RowName ='Heat Recovery') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtHeatingWaterSystems() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Heating') and (RowName ='Water Systems') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtHeatingRefrigeration() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Heating') and (RowName ='Refrigeration') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtHeatingGenerators() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Heating') and (RowName ='Generators') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::districtHeatingTotalEndUses() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='District Heating') and (RowName ='Total End Uses') and (Units = 'GJ')");
    }

    OptionalDouble SqlFile_Impl::waterHeating() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Water') and (RowName ='Heating') and (Units = 'm3')");
    }

    OptionalDouble SqlFile_Impl::waterCooling() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Water') and (RowName ='Cooling') and (Units = 'm3')");
    }

    OptionalDouble SqlFile_Impl::waterInteriorLighting() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Water') and (RowName ='Interior Lighting') and (Units = 'm3')");
    }

    OptionalDouble SqlFile_Impl::waterExteriorLighting() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Water') and (RowName ='Exterior Lighting') and (Units = 'm3')");
    }

    OptionalDouble SqlFile_Impl::waterInteriorEquipment() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Water') and (RowName ='Interior Equipment') and (Units = 'm3')");
    }

    OptionalDouble SqlFile_Impl::waterExteriorEquipment() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Water') and (RowName ='Exterior Equipment') and (Units = 'm3')");
    }

    OptionalDouble SqlFile_Impl::waterFans() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Water') and (RowName ='Fans') and (Units = 'm3')");
    }

    OptionalDouble SqlFile_Impl::waterPumps() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Water') and (RowName ='Pumps') and (Units = 'm3')");
    }

    OptionalDouble SqlFile_Impl::waterHeatRejection() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Water') and (RowName ='Heat Rejection') and (Units = 'm3')");
    }

    OptionalDouble SqlFile_Impl::waterHumidification() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Water') and (RowName ='Humidification') and (Units = 'm3')");
    }

    OptionalDouble SqlFile_Impl::waterHeatRecovery() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Water') and (RowName ='Heat Recovery') and (Units = 'm3')");
    }

    OptionalDouble SqlFile_Impl::waterWaterSystems() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Water') and (RowName ='Water Systems') and (Units = 'm3')");
    }

    OptionalDouble SqlFile_Impl::waterRefrigeration() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Water') and (RowName ='Refrigeration') and (Units = 'm3')");
    }

    OptionalDouble SqlFile_Impl::waterGenerators() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Water') and (RowName ='Generators') and (Units = 'm3')");
    }

    OptionalDouble SqlFile_Impl::waterTotalEndUses() const
    {
      return execAndReturnFirstDouble("SELECT Value from tabulardatawithstrings where (reportname = 'AnnualBuildingUtilityPerformanceSummary') and (ReportForString = 'Entire Facility') and (TableName = 'End Uses'  ) and (ColumnName ='Water') and (RowName ='Total End Uses') and (Units = 'm3')");
    }

    OptionalDouble SqlFile_Impl::hoursHeatingSetpointNotMet() const
    {
      return execAndReturnFirstDouble("SELECT Value FROM TabularDataWithStrings WHERE (ReportName='SystemSummary') AND (ReportForString='Entire Facility') AND (TableName='Time Setpoint Not Met') AND (RowName = 'Facility') AND (ColumnName='During Heating') AND (Units = 'hr')");
    }

    OptionalDouble SqlFile_Impl::hoursCoolingSetpointNotMet() const
    {
      return execAndReturnFirstDouble("SELECT Value FROM TabularDataWithStrings WHERE (ReportName='SystemSummary') AND (ReportForString='Entire Facility') AND (TableName='Time Setpoint Not Met') AND (RowName = 'Facility') AND (ColumnName='During Cooling') AND (Units = 'hr')");
    }


    std::vector<std::string> SqlFile_Impl::availableEnvPeriods() const
    {
      std::vector<std::string> vec;
      std::string envPeriodName;
      DataDictionaryTable::index<envPeriod>::type::iterator ienvPeriod;
      for (ienvPeriod=m_dataDictionary.get<envPeriod>().begin(); ienvPeriod!=m_dataDictionary.get<envPeriod>().end(); ++ienvPeriod)
      {
        envPeriodName = ienvPeriod->envPeriod;
        if (std::find(vec.begin(), vec.end(), envPeriodName) == vec.end())
        {
          vec.push_back(envPeriodName);
        }
      }
      return vec;
    }

    std::vector<std::string> SqlFile_Impl::availableKeyValues(const std::string& envPeriod, const std::string& reportingFrequency, const std::string& timeSeriesName)
    {
      std::string queryEnvPeriod = boost::to_upper_copy(envPeriod);

      std::vector<std::string> vec;
      std::string keyValueName;
      DataDictionaryTable::index<keyValue>::type::iterator ikeyValue;
      for (ikeyValue=m_dataDictionary.get<keyValue>().begin(); ikeyValue!=m_dataDictionary.get<keyValue>().end(); ++ikeyValue)
      {
        if ((ikeyValue->envPeriod == queryEnvPeriod) &&
            (ikeyValue->reportingFrequency == reportingFrequency) &&
            (ikeyValue->name == timeSeriesName) )
        {
          keyValueName = ikeyValue->keyValue;
          if (std::find(vec.begin(), vec.end(), keyValueName) == vec.end())
          {
            vec.push_back(keyValueName);
          }
        }
      }
      return vec;
    }



    openstudio::TimeSeriesVector SqlFile_Impl::timeSeries(const std::string &envPeriod, const std::string& reportingFrequency, const std::string &timeSeriesName)
    {

      openstudio::TimeSeriesVector vec;
      openstudio::OptionalTimeSeries ts;

      std::vector<std::string> vecKeyValues = availableKeyValues(envPeriod, reportingFrequency, timeSeriesName);
      std::vector<std::string>::iterator iter;
      for (iter=vecKeyValues.begin();iter!=vecKeyValues.end();++iter)
      {
        ts = timeSeries(envPeriod, reportingFrequency, timeSeriesName, *iter);
        if (ts){
          vec.push_back(*ts);
        }
      }
      return vec;
    }

    boost::optional<double> SqlFile_Impl::runPeriodValue(const std::string& envPeriod, const std::string& timeSeriesName, const std::string& keyValue)
    {
      std::string queryEnvPeriod = boost::to_upper_copy(envPeriod);

      DataDictionaryTable::index<envPeriodReportingFrequencyNameKeyValue>::type::iterator iEpRfNKv = m_dataDictionary.get<envPeriodReportingFrequencyNameKeyValue>().find(boost::make_tuple(queryEnvPeriod, ReportingFrequency(ReportingFrequency::RunPeriod).valueName(), timeSeriesName, keyValue));

      if (iEpRfNKv == m_dataDictionary.get<envPeriodReportingFrequencyNameKeyValue>().end() ){
        return boost::optional<double>();
      }

      std::stringstream s;
      s << "SELECT VariableValue FROM ";
      s << iEpRfNKv->table;
      s << " INNER JOIN Time t ON .TimeIndex = t.TimeIndex";
      //    s << " INNER JOIN EnvironmentPeriods ep ON ep.EnvironmentPeriodIndex = t.EnvironmentPeriodIndex";
      if (iEpRfNKv->table == "ReportMeterData")
      {
        s << " WHERE ReportMeterDataDictionaryIndex=";
      }
      else if (iEpRfNKv->table == "ReportVariableData")
      {
        s << " WHERE ReportVariableDataDictionaryIndex=";
      }
      s << boost::lexical_cast<std::string>(iEpRfNKv->recordIndex);
      //    s << " AND ep.EnvironmentName=";
      //    s << "'" << envPeriod << "'";
      s << " AND t.EnvironmentPeriodIndex=";
      s << boost::lexical_cast<std::string>(iEpRfNKv->envPeriodIndex);

      return execAndReturnFirstDouble(s.str());
    }

    boost::optional<double> SqlFile_Impl::execAndReturnFirstDouble(const std::string& statement) const
    {
      boost::optional<double> value;
      if (m_db)
      {
        sqlite3_stmt* sqlStmtPtr;

        int code = sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, NULL);

        code = sqlite3_step(sqlStmtPtr);
        if (code == SQLITE_ROW)
        {
          value = sqlite3_column_double(sqlStmtPtr, 0);
        }

        // must finalize to prevent memory leaks
        sqlite3_finalize(sqlStmtPtr);
      }
      return value;
    }

    boost::optional<int> SqlFile_Impl::execAndReturnFirstInt(const std::string& statement) const
    {
      boost::optional<int> value;
      if (m_db)
      {
        sqlite3_stmt* sqlStmtPtr;

        int code = sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, NULL);

        code = sqlite3_step(sqlStmtPtr);
        if (code == SQLITE_ROW)
        {
          value = sqlite3_column_int(sqlStmtPtr, 0);
        }

        // must finalize to prevent memory leaks
        sqlite3_finalize(sqlStmtPtr);
      }
      return value;
    }

    boost::optional<std::string> SqlFile_Impl::execAndReturnFirstString(const std::string& statement) const
    {
      boost::optional<std::string> value;
      if (m_db)
      {
        sqlite3_stmt* sqlStmtPtr;

        int code = sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, NULL);

        code = sqlite3_step(sqlStmtPtr);
        if (code == SQLITE_ROW)
        {
          value = columnText(sqlite3_column_text(sqlStmtPtr, 0));
        }

        // must finalize to prevent memory leaks
        sqlite3_finalize(sqlStmtPtr);
      }
      return value;
    }

    boost::optional<std::vector<double> > SqlFile_Impl::execAndReturnVectorOfDouble(const std::string& statement) const
    {
      boost::optional<double> value;
      boost::optional<std::vector<double> > valueVector;
      if (m_db)
      {
        sqlite3_stmt* sqlStmtPtr;

        int code = sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, NULL);
        while ((code!= SQLITE_DONE) && (code != SQLITE_BUSY)&& (code != SQLITE_ERROR) && (code != SQLITE_MISUSE)  )//loop until SQLITE_DONE
        {
          if (!valueVector){
            valueVector = std::vector<double>();
          }

          code = sqlite3_step(sqlStmtPtr);
          if (code == SQLITE_ROW)
          {
            value = sqlite3_column_double(sqlStmtPtr, 0);
            valueVector->push_back(*value);
          }
          else  // i didn't get a row.  something is wrong so set the exit condition.
          {     // should never get here since i test for all documented return states above
            code = SQLITE_DONE;
          }

        }// end loop

        // must finalize to prevent memory leaks
        sqlite3_finalize(sqlStmtPtr);
      }

      return valueVector;
    }

    boost::optional<std::vector<int> > SqlFile_Impl::execAndReturnVectorOfInt(const std::string& statement) const
    {
      boost::optional<int> value;
      boost::optional<std::vector<int> > valueVector;
      if (m_db)
      {
        sqlite3_stmt* sqlStmtPtr;

        int code = sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, NULL);
        while ((code!= SQLITE_DONE) && (code != SQLITE_BUSY)&& (code != SQLITE_ERROR) && (code != SQLITE_MISUSE)  )//loop until SQLITE_DONE
        {
          if (!valueVector){
            valueVector = std::vector<int>();
          }

          code = sqlite3_step(sqlStmtPtr);
          if (code == SQLITE_ROW)
          {
            value = sqlite3_column_int(sqlStmtPtr, 0);
            valueVector->push_back(*value);
          }
          else  // i didn't get a row.  something is wrong so set the exit condition.
          {     // should never get here since i test for all documented return states above
            code = SQLITE_DONE;
          }

        }// end loop

        // must finalize to prevent memory leaks
        sqlite3_finalize(sqlStmtPtr);
      }

      return valueVector;
    }

    boost::optional<std::vector<std::string> > SqlFile_Impl::execAndReturnVectorOfString(const std::string& statement) const
    {
      boost::optional<std::string> value;
      boost::optional<std::vector<std::string> > valueVector;
      if (m_db)
      {
        sqlite3_stmt* sqlStmtPtr;

        int code = sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, NULL);
        while ((code!= SQLITE_DONE) && (code != SQLITE_BUSY)&& (code != SQLITE_ERROR) && (code != SQLITE_MISUSE)  )//loop until SQLITE_DONE
        {
          if (!valueVector){
            valueVector = std::vector<std::string>();
          }

          code = sqlite3_step(sqlStmtPtr);
          if (code == SQLITE_ROW)
          {
            value = columnText(sqlite3_column_text(sqlStmtPtr, 0));
            valueVector->push_back(*value);
          }
          else  // i didn't get a row.  something is wrong so set the exit condition. 
          {     // should never get here since i test for all documented return states above
            code = SQLITE_DONE;  
          }

        }// end loop
        // must finalize to prevent memory leaks
        sqlite3_finalize(sqlStmtPtr);
      }
      return valueVector;
    }


    // execute a statement and return the error code, used for create/drop tables
    int SqlFile_Impl::execute(const std::string& statement)
    {
      int code = SQLITE_ERROR;
      if (m_db)
      {
        sqlite3_stmt* sqlStmtPtr;

        sqlite3_prepare_v2(m_db, statement.c_str(), -1, &sqlStmtPtr, NULL);

        code = sqlite3_step(sqlStmtPtr);

        // must finalize to prevent memory leaks
        sqlite3_finalize(sqlStmtPtr);
      }
      return code;
    }

    std::vector<double> SqlFile_Impl::timeSeriesValues(const DataDictionaryItem& dataDictionary)
    {
      std::vector<double> stdValues;

      if (m_db)
      {
        std::stringstream s;
        s << "SELECT VariableValue FROM ";
        s << dataDictionary.table;
        // ensure that there are time indice values for variablevalues (slows from 0.094s to 0.125s)
        s << " rvd INNER JOIN Time ti ON ti.TimeIndex = rvd.TimeIndex";
        //    s << " INNER JOIN EnvironmentPeriods ep ON ti.EnvironmentPeriodIndex = ep.EnvironmentPeriodIndex";
        if (dataDictionary.table == "ReportMeterData")
        {
          s << " WHERE rvd.ReportMeterDataDictionaryIndex=";
        }
        else if (dataDictionary.table == "ReportVariableData")
        {
          s << " WHERE rvd.ReportVariableDataDictionaryIndex=";
        }
        s << boost::lexical_cast<std::string>(dataDictionary.recordIndex);
        //      s << " AND ep.EnvironmentName = ";
        //      s << "'" << dataDictionary.envPeriod << "'";
        s << " AND ti.EnvironmentPeriodIndex = ";
        s << boost::lexical_cast<std::string>(dataDictionary.envPeriodIndex);
        // assume that timeindices.timeIndex are ordered from start to end
        //      s << " ORDER BY ti.TimeIndex";

        sqlite3_stmt* sqlStmtPtr;

        int code = sqlite3_prepare_v2(m_db, s.str().c_str(), -1, &sqlStmtPtr, NULL);

        code = sqlite3_step(sqlStmtPtr);
        std::stringstream s2;
        s2 << "SQL Query:" << std::endl;
        s2 << s.str();
        s2 << "Return Code:" << std::endl;
        s2 << code;
        LOG(Debug, s2.str());
        while (code == SQLITE_ROW)
        {
          stdValues.push_back( sqlite3_column_double(sqlStmtPtr, 0) ); // values

          code = sqlite3_step(sqlStmtPtr);
        }
        sqlite3_finalize(sqlStmtPtr);
      }

      LOG(Debug, "Created Timeseries with " << stdValues.size() << " values");

      return stdValues;
    }


    openstudio::OptionalDate SqlFile_Impl::timeSeriesStartDate(const DataDictionaryItem& dataDictionary)
    {
      unsigned int day=1;
      unsigned int month=1;
      if (m_db)
      {
        std::stringstream s;
        s << "SELECT ti.Month, ti.Day, ti.Hour from ";
        s << dataDictionary.table;
        s << " rvd INNER JOIN Time ti on ti.TimeIndex = rvd.TimeIndex";
        if (dataDictionary.table == "ReportMeterData")
        {
          s << " WHERE rvd.ReportMeterDataDictionaryIndex=";
        }
        else if (dataDictionary.table == "ReportVariableData")
        {
          s << " WHERE rvd.ReportVariableDataDictionaryIndex=";
        }
        s << boost::lexical_cast<std::string>(dataDictionary.recordIndex);
        s << " AND ti.EnvironmentPeriodIndex=";
        s << boost::lexical_cast<std::string>(dataDictionary.envPeriodIndex);

        sqlite3_stmt* sqlStmtPtr;
        int code = sqlite3_prepare_v2(m_db, s.str().c_str(), -1, &sqlStmtPtr, NULL);

        code = sqlite3_step(sqlStmtPtr);
        if (code == SQLITE_ROW)
        {
          month = sqlite3_column_int(sqlStmtPtr, 0);
          day = sqlite3_column_int(sqlStmtPtr, 1);
        }
        sqlite3_finalize(sqlStmtPtr);
      }
      try {
        // DLM@20100707: RunPeriod timeseries return month=0, day=0.
        // what is a sensible value to put here? is this a bug in E+?
        // DLM: potential leap year problem
        return openstudio::Date(openstudio::monthOfYear(month), day);
      } catch (...){
        //return min date in time table.
        std::stringstream s;
        s << "SELECT Month, Day, Hour from Time where TimeIndex in (";
        s << "SELECT min(timeIndex) FROM time )";
        sqlite3_stmt* sqlStmtPtr;
        int code = sqlite3_prepare_v2(m_db, s.str().c_str(), -1, &sqlStmtPtr, NULL);

        code = sqlite3_step(sqlStmtPtr);
        if (code == SQLITE_ROW)
        {
          month = sqlite3_column_int(sqlStmtPtr, 0);
          day = sqlite3_column_int(sqlStmtPtr, 1);
        }
        sqlite3_finalize(sqlStmtPtr);

        // DLM: potential leap year problem
        return openstudio::Date(openstudio::monthOfYear(month), day);
      }
    }

    openstudio::OptionalTime SqlFile_Impl::timeSeriesInterval(const DataDictionaryItem& dataDictionary)
    {
      double minutes=0;
      switch (reportingFrequencyFromDB(dataDictionary.reportingFrequency).value())
      {
        case ReportingFrequency::Detailed:
          // unsupported
          return boost::optional<openstudio::Time>();
          break;
        case ReportingFrequency::Timestep:
          // unsupported
          return boost::optional<openstudio::Time>();
          break;
        case ReportingFrequency::Hourly:
          // unsupported - warmup days - see trac #236
          return boost::optional<openstudio::Time>();
          //        return boost::optional<openstudio::Time>(openstudio::Time(0,1,0,0));
          break;
        case ReportingFrequency::Daily:
          return boost::optional<openstudio::Time>(openstudio::Time(1,0,0,0));
          break;
        case ReportingFrequency::Monthly:
          // unsupported
          return boost::optional<openstudio::Time>();
          break;
        case ReportingFrequency::RunPeriod:
          //          return boost::optional<openstudio::Time>();
          if (m_db)
          {
            std::stringstream s;
            s << "SELECT Interval from Time where TimeIndex in (";
            s << "SELECT min(ti.timeIndex) FROM ";
            s << dataDictionary.table;
            s << " rvd INNER JOIN Time ti on ti.TimeIndex = rvd.TimeIndex";
            if (dataDictionary.table == "ReportMeterData")
            {
              s << " WHERE rvd.ReportMeterDataDictionaryIndex=";
            }
            else if (dataDictionary.table == "ReportVariableData")
            {
              s << " WHERE rvd.ReportVariableDataDictionaryIndex=";
            }
            s << boost::lexical_cast<std::string>(dataDictionary.recordIndex);
            s << " AND ti.EnvironmentPeriodIndex=";
            s << boost::lexical_cast<std::string>(dataDictionary.envPeriodIndex);
            s << ")";

            sqlite3_stmt* sqlStmtPtr;
            int code = sqlite3_prepare_v2(m_db, s.str().c_str(), -1, &sqlStmtPtr, NULL);

            code = sqlite3_step(sqlStmtPtr);
            if (code == SQLITE_ROW)
            {
              minutes = sqlite3_column_double(sqlStmtPtr, 0);
            }
            sqlite3_finalize(sqlStmtPtr);
          }
          // minutes - 1 to remove starting minute
          return boost::optional<openstudio::Time>(openstudio::Time(0,0,int(std::ceil(minutes-1.0)),0));
          break;
        default:
          // unsupported
          return boost::optional<openstudio::Time>();
          break;
      }
    }

    openstudio::DateTime SqlFile_Impl::firstDateTime(bool includeHourAndMinute)
    {
      // default until added to eplusout.sql from energy plus
      unsigned month=1, day=1, hour=1, minute=0;

      openstudio::YearDescription yd;
      yd.isLeapYear = false;

      if (m_db)
      {
        std::stringstream s;
        s << "SELECT Month, Day, Hour, Minute from Time where Month is not NULL and Day is not null LIMIT 1";

        sqlite3_stmt* sqlStmtPtr;
        int code = sqlite3_prepare_v2(m_db, s.str().c_str(), -1, &sqlStmtPtr, NULL);

        code = sqlite3_step(sqlStmtPtr);
        if (code == SQLITE_ROW)
        {
          month = sqlite3_column_int(sqlStmtPtr, 0);
          day = sqlite3_column_int(sqlStmtPtr, 1);
          if (includeHourAndMinute){
            hour = sqlite3_column_int(sqlStmtPtr, 2);
            minute = sqlite3_column_int(sqlStmtPtr, 3);
          }
        }
        sqlite3_finalize(sqlStmtPtr);

        // DLM: could also try to check DayType to find yearStartsOnDayOfWeek
        if ((month == 2) && (day == 29)){
          yd.isLeapYear = true;
        }
      }
      // DLM: potential leap year problem
      openstudio::Date date(monthOfYear(month), day, yd);
      openstudio::Time time(0, hour, minute, 0);
      return openstudio::DateTime(date, time);
    }

    openstudio::OptionalTimeSeries SqlFile_Impl::timeSeries(const DataDictionaryItem& dataDictionary)
    {
      openstudio::OptionalTimeSeries ts;
      std::string units = dataDictionary.units;

      boost::optional<openstudio::DateTime> startDateTime; 
      std::vector<long> stdSecondsFromFirstReport;
      stdSecondsFromFirstReport.reserve(8760);

      std::vector<double> stdValues;
      stdValues.reserve(8760);
      boost::optional<unsigned> reportingIntervalMinutes;

      bool isIntervalTimeSeries = false;
      try {
        ReportingFrequency reportingFrequency(dataDictionary.reportingFrequency);
        isIntervalTimeSeries = (reportingFrequency != ReportingFrequency::Detailed);
      }catch(const std::exception&){
      }

      if (m_db) 
      {
        std::stringstream s;
        s << "SELECT dt.VariableValue, Time.Month, Time.Day, Time.Hour, Time.Minute, Time.Interval FROM ";
        s << dataDictionary.table;
        s << " dt INNER JOIN Time ON Time.timeIndex = dt.TimeIndex";
        s << " WHERE ";
        if (dataDictionary.table == "ReportMeterData")
        {
          s << " dt.ReportMeterDataDictionaryIndex=";
        }
        else if (dataDictionary.table == "ReportVariableData")
        {
          s << " dt.ReportVariableDataDictionaryIndex=";
        }
        s << boost::lexical_cast<std::string>(dataDictionary.recordIndex);
        s << " AND Time.EnvironmentPeriodIndex = ";
        s << boost::lexical_cast<std::string>(dataDictionary.envPeriodIndex);

        sqlite3_stmt* sqlStmtPtr;

        int code = sqlite3_prepare_v2(m_db, s.str().c_str(), -1, &sqlStmtPtr, NULL);

        code = sqlite3_step(sqlStmtPtr);
        std::stringstream s2;
        s2 << "SQL Query:" << std::endl;
        s2 << s.str();
        s2 << "Return Code:" << std::endl;
        s2 << code;
        LOG(Debug, s2.str());

        long cumulativeSeconds = 0;

        while (code == SQLITE_ROW) 
        {
          double value = sqlite3_column_double(sqlStmtPtr, 0);
          stdValues.push_back(value);

          unsigned month = sqlite3_column_int(sqlStmtPtr, 1);
          unsigned day = sqlite3_column_int(sqlStmtPtr, 2);
          unsigned hour = sqlite3_column_int(sqlStmtPtr, 3);
          unsigned minute = sqlite3_column_int(sqlStmtPtr, 4);
          unsigned intervalMinutes = sqlite3_column_int(sqlStmtPtr, 5); // used for run periods

          if (!startDateTime){
            if ((month==0) || (day==0)){
              // gets called for RunPeriod reports, just returns the first date in the time table, not sure if this is right
              startDateTime = firstDateTime(false);
            }else{
              // DLM: potential leap year problem
              startDateTime = openstudio::DateTime(openstudio::Date(month, day), openstudio::Time(0,0,intervalMinutes,0));
            }
          }

          stdSecondsFromFirstReport.push_back(cumulativeSeconds);

          cumulativeSeconds += 60*intervalMinutes;

          // check if this interval is same as the others
          if (isIntervalTimeSeries && !reportingIntervalMinutes){
            reportingIntervalMinutes = intervalMinutes;
          }else if (reportingIntervalMinutes && (reportingIntervalMinutes.get() != intervalMinutes)){
            isIntervalTimeSeries = false;
            reportingIntervalMinutes.reset();
          }

          // step to next row
          code = sqlite3_step(sqlStmtPtr);
        }

        // must finalize to prevent memory leaks
        sqlite3_finalize(sqlStmtPtr);

        if (startDateTime && !stdSecondsFromFirstReport.empty()){
          if (isIntervalTimeSeries){
            openstudio::Time intervalTime(0,0,*reportingIntervalMinutes,0);
            openstudio::Vector values = createVector(stdValues);
            ts = openstudio::TimeSeries(*startDateTime, intervalTime, values, units);
          }else{
            openstudio::Vector values = createVector(stdValues);
            ts = openstudio::TimeSeries(*startDateTime, stdSecondsFromFirstReport, values, units);
          }
        }
      }

      return ts;
    }

    openstudio::DateTimeVector SqlFile_Impl::dateTimeVec(const DataDictionaryItem& dataDictionary)
    {
      openstudio::DateTimeVector dateTimes;

      if (m_db) {
        std::stringstream s;
        s << "SELECT Time.month, Time.day, Time.hour, Time.minute, Time.dst FROM ";
        s << dataDictionary.table;
        s << " dt INNER JOIN Time ON Time.timeIndex = dt.TimeIndex";
        s << " WHERE ";
        if (dataDictionary.table == "ReportMeterData")
        {
          s << " dt.ReportMeterDataDictionaryIndex=";
        }
        else if (dataDictionary.table == "ReportVariableData")
        {
          s << " dt.ReportVariableDataDictionaryIndex=";
        }
        s << boost::lexical_cast<std::string>(dataDictionary.recordIndex);
        s << " AND Time.EnvironmentPeriodIndex = ";
        s << boost::lexical_cast<std::string>(dataDictionary.envPeriodIndex);

        sqlite3_stmt* sqlStmtPtr;

        int code = sqlite3_prepare_v2(m_db, s.str().c_str(), -1, &sqlStmtPtr, NULL);

        code = sqlite3_step(sqlStmtPtr);
        std::stringstream s2;
        s2 << "SQL Query:" << std::endl;
        s2 << s.str();
        s2 << "Return Code:" << std::endl;
        s2 << code;
        LOG(Debug, s2.str());
        while (code == SQLITE_ROW) {
          unsigned month, day, hour, minute;//, simulationDay;
          month = sqlite3_column_int(sqlStmtPtr, 0);
          day = sqlite3_column_int(sqlStmtPtr, 1);
          hour = sqlite3_column_int(sqlStmtPtr, 2);
          minute = sqlite3_column_int(sqlStmtPtr, 3);

          // DLM: potential leap year problem
          openstudio::DateTime dateTime(openstudio::Date(monthOfYear(month),day), openstudio::Time(0,hour, minute, 0));
          dateTimes.push_back(dateTime);

          // step to next row
          code = sqlite3_step(sqlStmtPtr);
        }
        // must finalize to prevent memory leaks
        sqlite3_finalize(sqlStmtPtr);
      }

      return dateTimes;
    }

    openstudio::OptionalTimeSeries SqlFile_Impl::timeSeries(const std::string& envPeriod, const std::string& reportingFrequency, const std::string& timeSeriesName, const std::string& keyValue)
    {
      //std::string queryEnvPeriod = envPeriod;
      std::string queryEnvPeriod = boost::to_upper_copy(envPeriod);

      LOG(Debug, "Making time series for envPeriod = '" << queryEnvPeriod <<
          "', reportingFrequency = '" << reportingFrequency <<
          "', timeSeriesName = '" << timeSeriesName <<
          "', keyValue = '" << keyValue << "'");

      openstudio::OptionalTimeSeries ts;
      DataDictionaryTable::index<envPeriodReportingFrequencyNameKeyValue>::type::iterator iEpRfNKv = m_dataDictionary.get<envPeriodReportingFrequencyNameKeyValue>().find(boost::make_tuple(queryEnvPeriod, reportingFrequency, timeSeriesName, keyValue));

      if (iEpRfNKv == m_dataDictionary.get<envPeriodReportingFrequencyNameKeyValue>().end()) {
        // not found
        LOG(Debug,"Tuple: " << queryEnvPeriod << ", " << reportingFrequency << ", " << timeSeriesName << ", " << keyValue << " not found in data dictionary.");
      } else if (!iEpRfNKv->timeSeries.values().empty()) {
        ts = iEpRfNKv->timeSeries;
      } else {// lazy caching
        DataDictionaryItem ddi = *iEpRfNKv;
        LOG(Debug, ddi.envPeriod);
        LOG(Debug, ddi.name);
        ts =  timeSeries(ddi);
        if (ts) {
          ddi.timeSeries = *ts;
          m_dataDictionary.get<envPeriodReportingFrequencyNameKeyValue>().replace(iEpRfNKv, ddi);
        }
      }
      if (ts) {
        LOG(Debug, "Time series found");
      }else{
        LOG(Debug, "No time series found");
      }
      return ts;
    }

    SqlFileTimeSeriesQueryVector SqlFile_Impl::expandQuery(const SqlFileTimeSeriesQuery& query) {

      SqlFileTimeSeriesQueryVector result, temp1, temp2;

      // no work needed
      if (query.m_vetted) { 
        result.push_back(query);
        return result; 
      } 

      temp1 = expandEnvironment(query);

      // build up temp2
      BOOST_FOREACH(const SqlFileTimeSeriesQuery& q,temp1) {
        SqlFileTimeSeriesQueryVector temp3 = expandReportingFrequency(q);
        temp2.insert(temp2.end(),temp3.begin(),temp3.end());
      }

      // build up temp1
      temp1.clear();
      BOOST_FOREACH(const SqlFileTimeSeriesQuery& q,temp2) {
        SqlFileTimeSeriesQueryVector temp3 = expandTimeSeries(q);
        temp1.insert(temp1.end(),temp3.begin(),temp3.end());
      }

      // build up result
      BOOST_FOREACH(const SqlFileTimeSeriesQuery& q,temp1) {
        SqlFileTimeSeriesQueryVector temp3 = expandKeyValues(q);
        result.insert(result.end(),temp3.begin(),temp3.end());
      }

      // mark each query as vetted
      BOOST_FOREACH(SqlFileTimeSeriesQuery& resultQuery,result) {
        resultQuery.m_vetted = true;
      }

      return result;
    }

    SqlFileTimeSeriesQueryVector SqlFile_Impl::expandEnvironment(const SqlFileTimeSeriesQuery& query) {
      SqlFileTimeSeriesQueryVector result;

      EnvironmentIdentifier envId;
      StringVector envNames = availableEnvPeriods();
      StringVector keepers;
      if (query.environment()) {
        envId = query.environment().get();
        if (envId.type()) {
          EnvironmentType envType = envId.type().get();
          // specified by type--get all matching environment names
          BOOST_FOREACH(const std::string& envName,envNames) {
            OptionalEnvironmentType oEnvType = environmentType(envName);
            if (oEnvType && (*oEnvType == envType)) { keepers.push_back(envName); }
          }
        }
        else {
          std::string envName = envId.name().get();
          // check validity of name
          StringVector::const_iterator it = std::find_if(envNames.begin(),envNames.end(),
              boost::bind(istringEqual,envName,_1));
          if (it != envNames.end()) { keepers.push_back(*it); }
        }
      }
      else {
        // keep all envNames
        keepers = envNames;
      }

      // initialize result queries
      SqlFileTimeSeriesQuery wQuery(query);
      BOOST_FOREACH(const std::string& envName,keepers) {
        envId = EnvironmentIdentifier(envName);
        wQuery.setEnvironment(envId);
        result.push_back(wQuery);
      }

      // check result queries for consistency
      mf_makeConsistent(result);

      return result;
    }

    SqlFileTimeSeriesQueryVector SqlFile_Impl::expandReportingFrequency(const SqlFileTimeSeriesQuery& query) {
      SqlFileTimeSeriesQueryVector result;

      if (query.reportingFrequency()) {
        result.push_back(query);
      }
      else {
        ReportingFrequencySet rfSet;
        StringVector envNames;
        // get environment names
        if (query.environment()) {
          EnvironmentIdentifier envId = query.environment().get();
          if (envId.name()) {
            envNames.push_back(envId.name().get());
          }
          else {
            OS_ASSERT(envId.type());
            EnvironmentType envType = envId.type().get();
            BOOST_FOREACH(const std::string& envName,availableEnvPeriods()) {
              OptionalEnvironmentType oEnvType = environmentType(envName);
              if (oEnvType && (*oEnvType == envType)) { envNames.push_back(envName); }
            }
          } // if-else
        }
        else {
          envNames = availableEnvPeriods();
        }
        // get reporting frequencies
        BOOST_FOREACH(const std::string& envName,envNames) {
          ReportingFrequencySet tempSet = availableReportingFrequencySet(*this,envName);
          rfSet.insert(tempSet.begin(),tempSet.end());
        }

        // make one query per rf
        BOOST_FOREACH(const ReportingFrequency& rf,rfSet) {
          SqlFileTimeSeriesQuery wQuery(query);
          wQuery.setReportingFrequency(rf);
          result.push_back(wQuery);
        }
      }

      // check result queries for consistency
      mf_makeConsistent(result);

      return result;
    }

    SqlFileTimeSeriesQueryVector SqlFile_Impl::expandTimeSeries(const SqlFileTimeSeriesQuery& query) {
      SqlFileTimeSeriesQueryVector result;

      StringVector tsNames;
      if (query.timeSeries() && query.timeSeries().get().name()) {
        if (query.timeSeries().get().name()) {
          tsNames.push_back(*(query.timeSeries().get().name()));
        }
      }
      else {
        // get candidate time series names
        if (query.environment() && query.environment().get().name() && query.reportingFrequency()) {
          tsNames = availableVariableNames(query.environment().get().name().get(),
              query.reportingFrequency().get().valueDescription());
        }
        else {
          // get all and let other function filter out
          tsNames = availableTimeSeries();
        }
        // filter by regex if applicable
        if (query.timeSeries()) {
          OS_ASSERT(query.timeSeries().get().regex());
          boost::regex re = query.timeSeries().get().regex().get();
          for (StringVector::iterator it = tsNames.begin(); it != tsNames.end(); ) {
            if (!boost::regex_match(*it,re)) {
              it = tsNames.erase(it);
              continue;
            }
            ++it;
          }      
        }
      }

      // make one query per time series name
      BOOST_FOREACH(const std::string& tsName,tsNames) {
        SqlFileTimeSeriesQuery wQuery(query);
        TimeSeriesIdentifier tsId(tsName);
        wQuery.setTimeSeries(tsId);
        result.push_back(wQuery);
      }

      // check result queries for consistency
      mf_makeConsistent(result);

      return result;
    }

    SqlFileTimeSeriesQueryVector SqlFile_Impl::expandKeyValues(const SqlFileTimeSeriesQuery& query) {
      SqlFileTimeSeriesQueryVector result;

      // only "expand" regex
      result.push_back(query);
      if (query.keyValues() && query.keyValues().get().regex()) {
        // get available key value names
        StringVector kvNames;
        if (query.environment() && query.environment().get().name() && query.reportingFrequency()
            && query.timeSeries() && query.timeSeries().get().name()) {
          kvNames = availableKeyValues(query.environment().get().name().get(),
              query.reportingFrequency().get().valueDescription(),
              query.timeSeries().get().name().get());
        }
        else {
          LOG_AND_THROW("Not yet implemented.");
        }
        // check against regex
        boost::regex re = *(query.keyValues().get().regex());
        for (StringVector::iterator it = kvNames.begin(); it != kvNames.end(); ) {
          if (!boost::regex_match(*it,re)) {
            it = kvNames.erase(it);
            continue;
          }
          ++it;
        }
        if (kvNames.empty()) { result.clear(); }
        else {
          // set names
          KeyValueIdentifier kvId(kvNames);
          result[0].setKeyValues(kvId);
        }
      }

      // check result queries for consistency
      mf_makeConsistent(result);

      return result;
    }


    TimeSeriesVector SqlFile_Impl::timeSeries(const SqlFileTimeSeriesQuery& query) {
      TimeSeriesVector result;
      SqlFileTimeSeriesQuery wquery(query);
      if (!wquery.m_vetted) {
        SqlFileTimeSeriesQueryVector expanded = expandQuery(query);
        if (expanded.size() == 1) {
          wquery = expanded[0];
        }
        else {
          if (expanded.size() == 0) {
            LOG(Info,"Unable to return timeSeries based on query: " << std::endl << query
                << ", because there are no matching timeSeries in SqlFile " << toString(path()) 
                << ".");
          }
          else {
            OS_ASSERT(expanded.size() > 1);
            LOG(Info,"Unable to return timeSeries based on query: " << std::endl << query
                << ", because it expands to more than one (" << expanded.size() << ") query.");
          }
          return result;
        }
      }

      OS_ASSERT(wquery.m_vetted);
      OS_ASSERT(wquery.environment()); 
      OS_ASSERT(!wquery.environment().get().type());
      OS_ASSERT(wquery.reportingFrequency());
      OS_ASSERT(wquery.timeSeries());
      OS_ASSERT(!wquery.timeSeries().get().regex());
      if (wquery.keyValues()) { OS_ASSERT(!wquery.keyValues().get().regex()); }

      // environment, reportingPeriod, and timeSeries will all be unique and explicit.
      // keyValues may or may not be explicit.
      // get all matching timeSeries and append to result.
      std::string envPeriod = *(wquery.environment().get().name());
      ReportingFrequency rf = *(wquery.reportingFrequency());
      std::string tsName = *(wquery.timeSeries().get().name());
      if (wquery.keyValues()) {
        BOOST_FOREACH(const std::string kvName,wquery.keyValues().get().names()) {
          OptionalTimeSeries ots = timeSeries(envPeriod,rf.valueDescription(),tsName,kvName);
          if (ots) { result.push_back(*ots); }
        }
      }
      else {
        result = timeSeries(envPeriod,rf.valueDescription(),tsName);
      }

      return result;
    }

    boost::optional<std::pair<DateTime, DateTime> > SqlFile_Impl::daylightSavingsPeriod() const
    {
      // first and last date for dst=1
      // sqlite3 does not have interface for first and last record in recordset
      // minimize recordset size and use group by and order by
      // TODO - unit test

      boost::optional<std::pair<DateTime, DateTime> > dstPeriod;
      int startMonth=0, startDay=0, startHour=0, startMinute=0;
      int endMonth=0, endDay=0, endHour=0, endMinute=0;



      if (m_db)
      {

        sqlite3_stmt* sqlStmtPtr;

        // first date time of dst
        std::string s = "select month, day, hour, minute from Time where dst=1 group by month order by month, day, hour, minute";

        int code = sqlite3_prepare_v2(m_db, s.c_str(),-1,&sqlStmtPtr,NULL);

        code = sqlite3_step(sqlStmtPtr);
        if (code == SQLITE_ROW)
        {
          startMonth = sqlite3_column_int(sqlStmtPtr,0);
          startDay = sqlite3_column_int(sqlStmtPtr,1);
          startHour = sqlite3_column_int(sqlStmtPtr,2);
          startMinute = sqlite3_column_int(sqlStmtPtr,3);
        }

        /// must finalize to prevent memory leaks
        sqlite3_finalize(sqlStmtPtr);

        // last date time of dst
        s = "select month, day, hour, minute from Time where dst=1 group by month order by month desc, day desc, hour desc, minute desc";

        code = sqlite3_prepare_v2(m_db, s.c_str(),-1,&sqlStmtPtr,NULL);

        code = sqlite3_step(sqlStmtPtr);
        if (code == SQLITE_ROW)
        {
          endMonth = sqlite3_column_int(sqlStmtPtr,0);
          endDay = sqlite3_column_int(sqlStmtPtr,1);
          endHour = sqlite3_column_int(sqlStmtPtr,2);
          endMinute = sqlite3_column_int(sqlStmtPtr,3);
        }

        /// must finalize to prevent memory leaks
        sqlite3_finalize(sqlStmtPtr);
      }

      // DLM: potential leap year problem
      DateTime dstStart(Date(openstudio::monthOfYear(startMonth),startDay), Time(startDay, startHour, startMinute,0));
      DateTime dstEnd(Date(openstudio::monthOfYear(endMonth),endDay), Time(endDay, endHour, endMinute,0));

      dstPeriod->first = dstStart;
      dstPeriod->first = dstEnd;
      return dstPeriod ;
    }

    /// returns datadictionary of available timeseries
    DataDictionaryTable SqlFile_Impl::dataDictionary() const
    {
      return m_dataDictionary;
    }

    /// Energy plus version number
    // DLM@20100511: can we query this?
    std::string SqlFile_Impl::energyPlusVersion() const
    {
      return "5-0-0";
    }

    /// Energy Plus eplusout.sql file name
    std::string SqlFile_Impl::energyPlusSqliteFile() const
    {
      return m_sqliteFilename;
    }

    /// get names of all available illuminance maps
    std::vector<std::string> SqlFile_Impl::illuminanceMapNames() const
    {
      std::vector<std::string> names;

      const std::string& s = "select MapName from daylightmaps";

      sqlite3_stmt* sqlStmtPtr;

      int code = sqlite3_prepare_v2(m_db, s.c_str(),-1,&sqlStmtPtr,NULL);
      code = sqlite3_step(sqlStmtPtr);

      while (code == SQLITE_ROW)
      {
        names.push_back(columnText(sqlite3_column_text(sqlStmtPtr,0)));

        // step to next row
        code = sqlite3_step(sqlStmtPtr);
      }

      /// must finalize to prevent memory leaks
      sqlite3_finalize(sqlStmtPtr);

      return names;
    }

    /// get names of all available illuminance maps for the given environment period
    std::vector<std::string> SqlFile_Impl::illuminanceMapNames(const std::string& envPeriod) const
    {
      std::vector<std::string> names;

      std::stringstream s;
      s << "select MapName from daylightmaps where Environment = '" << envPeriod << "' COLLATE NOCASE";

      sqlite3_stmt* sqlStmtPtr;

      int code = sqlite3_prepare_v2(m_db, s.str().c_str(),-1,&sqlStmtPtr,NULL);
      code = sqlite3_step(sqlStmtPtr);

      while (code == SQLITE_ROW)
      {
        names.push_back(columnText(sqlite3_column_text(sqlStmtPtr,0)));

        // step to next row
        code = sqlite3_step(sqlStmtPtr);
      }

      /// must finalize to prevent memory leaks
      sqlite3_finalize(sqlStmtPtr);

      return names;
    }

    /// reference point for map - form RefPtn=(x:y:illuminance)
    boost::optional<std::string> SqlFile_Impl::illuminanceMapRefPt(const std::string& name, const int& ptNum) const
    {
      boost::optional<std::string> refPt;
      boost::optional<int> mapIndex = illuminanceMapIndex(name);

      if (mapIndex)
        refPt = illuminanceMapRefPt(*mapIndex, ptNum);
      else
        LOG(Error, "Unknown illuminance map '" << name << "'");

      return refPt;
    }

    boost::optional<std::string> SqlFile_Impl::illuminanceMapRefPt(const int& mapIndex, const int& ptNum) const
    {
      boost::optional<std::string> refPt;
      std::stringstream s;
      s << "select ReferencePt" << ptNum << " from daylightmaps where MapNumber=" << mapIndex;

      sqlite3_stmt* sqlStmtPtr;

      int code = sqlite3_prepare_v2(m_db, s.str().c_str(),-1,&sqlStmtPtr,NULL);
      code = sqlite3_step(sqlStmtPtr);

      if (code == SQLITE_ROW)
        refPt = columnText(sqlite3_column_text(sqlStmtPtr,0));

      /// must finalize to prevent memory leaks
      sqlite3_finalize(sqlStmtPtr);

      return refPt;
    }

    /// minimum value of map
    boost::optional<double> SqlFile_Impl::illuminanceMapMinValue(const std::string& name) const
    {
      boost::optional<double> minValue;
      boost::optional<int> mapIndex = illuminanceMapIndex(name);

      if (mapIndex)
        minValue = illuminanceMapMinValue(*mapIndex);
      else
        LOG(Error, "Unknown illuminance map '" << name << "'");

      return minValue;
    }

    /// minimum value of map
    boost::optional<double> SqlFile_Impl::illuminanceMapMinValue(const int& mapIndex) const
    {
      boost::optional<double> minValue;
      std::stringstream s;
      s << "select min(d.Illuminance) from daylightmaphourlydata d inner join daylightmaphourlyreports r on d.HourlyReportIndex = r.HourlyReportIndex where r.MapNumber=" << mapIndex;

      sqlite3_stmt* sqlStmtPtr;

      int code = sqlite3_prepare_v2(m_db, s.str().c_str(),-1,&sqlStmtPtr,NULL);
      code = sqlite3_step(sqlStmtPtr);

      if (code == SQLITE_ROW)
        minValue = sqlite3_column_double(sqlStmtPtr,0);

      /// must finalize to prevent memory leaks
      sqlite3_finalize(sqlStmtPtr);

      return minValue;
    }

    /// maximum value of map
    boost::optional<double> SqlFile_Impl::illuminanceMapMaxValue(const std::string& name) const
    {
      boost::optional<double> maxValue;
      boost::optional<int> mapIndex = illuminanceMapIndex(name);

      if (mapIndex)
        maxValue = illuminanceMapMaxValue(*mapIndex);
      else
        LOG(Error, "Unknown illuminance map '" << name << "'");

      return maxValue;
    }

    /// maximum value of map
    boost::optional<double> SqlFile_Impl::illuminanceMapMaxValue(const int& mapIndex) const
    {
      boost::optional<double> maxValue;
      std::stringstream s;
      s << "select max(d.Illuminance) from daylightmaphourlydata d inner join daylightmaphourlyreports r on d.HourlyReportIndex = r.HourlyReportIndex where r.MapNumber=" << mapIndex;

      sqlite3_stmt* sqlStmtPtr;

      int code = sqlite3_prepare_v2(m_db, s.str().c_str(),-1,&sqlStmtPtr,NULL);
      code = sqlite3_step(sqlStmtPtr);

      if (code == SQLITE_ROW)
        maxValue = sqlite3_column_double(sqlStmtPtr,0);

      /// must finalize to prevent memory leaks
      sqlite3_finalize(sqlStmtPtr);

      return maxValue;
    }

    void SqlFile_Impl::illuminanceMapMaxValue(const std::string& name, double& minValue, double& maxValue) const
    {
      boost::optional<int> mapIndex = illuminanceMapIndex(name);

      if (mapIndex)
        illuminanceMapMaxValue(*mapIndex,minValue,maxValue);
      else
        LOG(Error, "Unknown illuminance map '" << name << "'");
    }

    /// minimum and maximim of map
    void SqlFile_Impl::illuminanceMapMaxValue(const int& mapIndex, double& minValue, double& maxValue) const
    {
      std::stringstream s;
      s << "select min(d.Illuminance), max(d.Illuminance) from daylightmaphourlydata d inner join daylightmaphourlyreports r on d.HourlyReportIndex = r.HourlyReportIndex where r.MapNumber=" << mapIndex;

      sqlite3_stmt* sqlStmtPtr;

      int code = sqlite3_prepare_v2(m_db, s.str().c_str(),-1,&sqlStmtPtr,NULL);
      code = sqlite3_step(sqlStmtPtr);

      if (code == SQLITE_ROW)
      {
        minValue = sqlite3_column_double(sqlStmtPtr,0);
        maxValue = sqlite3_column_double(sqlStmtPtr,1);
      }
      /// must finalize to prevent memory leaks
      sqlite3_finalize(sqlStmtPtr);
    }


    /// get all zone names for specified illuminance map
    std::vector<std::string> SqlFile_Impl::illuminanceMapZoneNames(const std::string& name) const
    {
      std::vector<std::string> names;
      boost::optional<int> mapIndex = illuminanceMapIndex(name);

      if (mapIndex)
        names = illuminanceMapZoneNames(*mapIndex);
      else
        LOG(Error, "Unknown illuminance map '" << name << "'");

      return names;
    }

    /// get all zone names for specified illuminance map
    std::vector<std::string> SqlFile_Impl::illuminanceMapZoneNames(const int& mapIndex) const
    {
      std::vector<std::string> names;

      std::stringstream s;
      s << "select ZoneName from zones where ZoneIndex in (select Zone from daylightmaps where MapNumber = " << mapIndex << ")";

      sqlite3_stmt* sqlStmtPtr;

      int code = sqlite3_prepare_v2(m_db, s.str().c_str(),-1,&sqlStmtPtr,NULL);
      code = sqlite3_step(sqlStmtPtr);

      while (code == SQLITE_ROW)
      {
        names.push_back(columnText(sqlite3_column_text(sqlStmtPtr,0)));

        // step to next row
        code = sqlite3_step(sqlStmtPtr);
      }

      /// must finalize to prevent memory leaks
      sqlite3_finalize(sqlStmtPtr);

      return names;
    }

    /// x position (m) of the illuminance map
    Vector SqlFile_Impl::illuminanceMapX(const int& hourlyReportIndex) const
    {
      std::vector<double> xv;
      std::stringstream s;
      s << "select X from daylightmaphourlydata where HourlyReportIndex=" << hourlyReportIndex << " group by X";

      sqlite3_stmt* sqlStmtPtr;

      int code = sqlite3_prepare_v2(m_db, s.str().c_str(),-1,&sqlStmtPtr,NULL);
      code = sqlite3_step(sqlStmtPtr);

      while (code == SQLITE_ROW)
      {
        xv.push_back(sqlite3_column_double(sqlStmtPtr,0));

        // step to next row
        code = sqlite3_step(sqlStmtPtr);
      }

      /// must finalize to prevent memory leaks
      sqlite3_finalize(sqlStmtPtr);

      // copy std::vector to Vector, will be sorted in ascending order
      Vector x(xv.size());
      std::copy(xv.begin(), xv.end(), x.begin());

      return x;
    }

    /// x position (m) of the illuminance map
    Vector SqlFile_Impl::illuminanceMapX(const std::string& name, const DateTime& dateTime) const
    {
      // figure out map index
      boost::optional<int> mapIndex = illuminanceMapIndex(name);

      if (!mapIndex){
        LOG(Error, "Unknown illuminance map '" << name << "'");
        return Vector();
      }
      boost::optional<int> timeIndex = illuminanceMapHourlyReportIndex(*mapIndex, dateTime);
      if (!timeIndex){
        LOG(Error, "Unknown date and time '" << dateTime << "'");
        return Vector();
      }

      Vector x(illuminanceMapX(*timeIndex));

      return x;
    }

    /// y position (m) of the illuminance map
    Vector SqlFile_Impl::illuminanceMapY(const int& hourlyReportIndex) const
    {
      std::vector<double> yv;
      std::stringstream s;
      s << "select Y from daylightmaphourlydata where HourlyReportIndex=" << hourlyReportIndex << " group by Y";

      sqlite3_stmt* sqlStmtPtr;

      int code = sqlite3_prepare_v2(m_db, s.str().c_str(),-1,&sqlStmtPtr,NULL);
      code = sqlite3_step(sqlStmtPtr);

      while (code == SQLITE_ROW)
      {
        yv.push_back(sqlite3_column_double(sqlStmtPtr,0));

        // step to next row
        code = sqlite3_step(sqlStmtPtr);
      }

      /// must finalize to prevent memory leaks
      sqlite3_finalize(sqlStmtPtr);

      // copy std::vector to Vector, will be sorted in ascending order
      Vector y(yv.size());
      std::copy(yv.begin(), yv.end(), y.begin());

      return y;
    }

    /// y position (m) of the illuminance map
    Vector SqlFile_Impl::illuminanceMapY(const std::string& name, const DateTime& dateTime) const
    {
      // figure out map index
      boost::optional<int> mapIndex = illuminanceMapIndex(name);

      if (!mapIndex){
        LOG(Error, "Unknown illuminance map '" << name << "'");
        return Vector();
      }
      boost::optional<int> timeIndex = illuminanceMapHourlyReportIndex(*mapIndex, dateTime);
      if (!timeIndex){
        LOG(Error, "Unknown date and time '" << dateTime << "'");
        return Vector();
      }

      Vector y(illuminanceMapY(*timeIndex));

      return y;
    }

    std::vector<int> SqlFile_Impl::illuminanceMapHourlyReportIndices(const std::string& name) const
    {
      std::vector<int> reportIndices;
      // figure out map index
      boost::optional<int> mapIndex = illuminanceMapIndex(name);
      if (!mapIndex)
      {
        LOG(Error, "Unknown illuminance map '" << name << "'");
        return reportIndices;
      }
      reportIndices = illuminanceMapHourlyReportIndices(*mapIndex);
      return reportIndices;
    }

    std::vector<int> SqlFile_Impl::illuminanceMapHourlyReportIndices(const int& mapIndex) const
    {
      std::vector<int> reportIndices;
      std::stringstream s;
      s << "select HourlyReportIndex from daylightmaphourlyreports where MapNumber=" << mapIndex;

      sqlite3_stmt* sqlStmtPtr;

      int code = sqlite3_prepare_v2(m_db, s.str().c_str(),-1,&sqlStmtPtr,NULL);
      code = sqlite3_step(sqlStmtPtr);
      while (code == SQLITE_ROW)
      {
        reportIndices.push_back( sqlite3_column_int(sqlStmtPtr,0) );
        // step to next row
        code = sqlite3_step(sqlStmtPtr);
      }

      /// must finalize to prevent memory leaks
      sqlite3_finalize(sqlStmtPtr);

      return reportIndices;
    }


    std::vector< std::pair<int, DateTime> > SqlFile_Impl::illuminanceMapHourlyReportIndicesDates(const std::string& name) const
    {
      std::vector< std::pair<int, DateTime> > reportIndicesDates;
      // figure out map index
      boost::optional<int> mapIndex = illuminanceMapIndex(name);
      if (!mapIndex)
      {
        LOG(Error, "Unknown illuminance map '" << name << "'");
        return reportIndicesDates;
      }
      reportIndicesDates = illuminanceMapHourlyReportIndicesDates(*mapIndex);
      return reportIndicesDates;
    }



    std::vector< std::pair<int, DateTime> > SqlFile_Impl::illuminanceMapHourlyReportIndicesDates(const int& mapIndex) const
    {
      std::vector< std::pair<int, DateTime> > reportIndicesDates;
      std::stringstream s;
      s << "select HourlyReportIndex, Month, DayOfMonth, Hour from daylightmaphourlyreports where MapNumber=" << mapIndex;

      sqlite3_stmt* sqlStmtPtr;

      int code = sqlite3_prepare_v2(m_db, s.str().c_str(),-1,&sqlStmtPtr,NULL);
      code = sqlite3_step(sqlStmtPtr);
      while (code == SQLITE_ROW)
      {
        std::pair<int, DateTime> pair;
        pair.first = sqlite3_column_int(sqlStmtPtr,0);

        // DLM: potential leap year problem
        pair.second = DateTime( Date( monthOfYear( sqlite3_column_int(sqlStmtPtr,1) ), sqlite3_column_int(sqlStmtPtr,2) ), Time( 0, sqlite3_column_int(sqlStmtPtr, 3), 0, 0) );
        reportIndicesDates.push_back( pair );
        // step to next row
        code = sqlite3_step(sqlStmtPtr);
      }

      /// must finalize to prevent memory leaks
      sqlite3_finalize(sqlStmtPtr);

      return reportIndicesDates;
    }

    boost::optional<DateTime> SqlFile_Impl::illuminanceMapDate(const int& hourlyReportIndex) const
    {
      int month=0, dayOfMonth=0, hour=0;
      std::stringstream s;
      s << "select Month, DayOfMonth, Hour from daylightmaphourlyreports where HourlyReportIndex=" << hourlyReportIndex;

      sqlite3_stmt* sqlStmtPtr;

      int code = sqlite3_prepare_v2(m_db, s.str().c_str(),-1,&sqlStmtPtr,NULL);
      code = sqlite3_step(sqlStmtPtr);
      if (code == SQLITE_ROW)
      {
        month = sqlite3_column_int(sqlStmtPtr,0);
        dayOfMonth = sqlite3_column_int(sqlStmtPtr,1);
        hour = sqlite3_column_int(sqlStmtPtr,2);
      }
      else
      {
        LOG(Error, "Unknown hourly report index " << hourlyReportIndex);
        return DateTime();
      }

      /// must finalize to prevent memory leaks
      sqlite3_finalize(sqlStmtPtr);

      // DLM: potential leap year problem
      return DateTime(Date(monthOfYear(month),dayOfMonth), Time(0,hour, 0, 0));
    }

    boost::optional<int> SqlFile_Impl::illuminanceMapHourlyReportIndex(const int& mapIndex, const DateTime& dateTime) const
    {
      int monthOfYear = dateTime.date().monthOfYear().value();
      int dayOfMonth = dateTime.date().dayOfMonth();
      int hours = dateTime.time().hours();
      // EnergyPlus deals in a 00:00:01 -> 24:00:00 instead of
      // 00:00:00 -> 23:59:59 hrs that the real world uses, so
      // we are going to adjust for that, so we subtract an hour,
      // to get it back to the previous day, then we will replace
      // the hours with 24
      if (hours == 0)
      {
        DateTime dt = dateTime - Time(0, 1);
        monthOfYear = dt.date().monthOfYear().value();
        dayOfMonth = dt.date().dayOfMonth();
        hours = 24;
      }

      std::stringstream s;
      s << "select HourlyReportIndex from daylightmaphourlyreports where MapNumber='" << mapIndex <<
        "' AND Month=" << monthOfYear <<
        " AND DayOfMonth=" << dayOfMonth <<
        " AND Hour=" << hours ;

      sqlite3_stmt* sqlStmtPtr;

      boost::optional<int> timeIndex;
      int code = sqlite3_prepare_v2(m_db, s.str().c_str(),-1,&sqlStmtPtr,NULL);
      code = sqlite3_step(sqlStmtPtr);
      if (code == SQLITE_ROW)
      {
        timeIndex = sqlite3_column_int(sqlStmtPtr,0);
      }

      /// must finalize to prevent memory leaks
      sqlite3_finalize(sqlStmtPtr);

      if (!timeIndex)
      {
        LOG(Error, "Unknown date and time '" << dateTime << "'");
      }
      return timeIndex;

    }


    /// value (lux) of the illuminance map at date and time
    /// value(i,j) is the illuminance at x(i), y(j)
    Matrix SqlFile_Impl::illuminanceMap(const std::string& name, const DateTime& dateTime) const
    {
      // figure out map index
      boost::optional<int> mapIndex = illuminanceMapIndex(name);

      if (!mapIndex){
        LOG(Error, "Unknown illuminance map '" << name << "'");
        return Matrix();
      }

      // find the HourlyReportIndex
      boost::optional<int> timeIndex = illuminanceMapHourlyReportIndex(*mapIndex, dateTime);
      if (!timeIndex){
        LOG(Error, "Unknown date and time '" << dateTime << "'");
        return Matrix();
      }

      return illuminanceMap(*timeIndex);
    }

    void SqlFile_Impl::illuminanceMap(const int& hourlyReportIndex, 
                                      std::vector<double>& x, 
                                      std::vector<double>& y, 
                                      std::vector<double>& illuminance) const
    {
      double xVal(0.0), yVal(0.0), yValPrevious(0.0), illuminanceVal(0.0);
      bool yValChanged=false;

      std::stringstream statement;
      statement << "select X,Y,Illuminance from daylightmaphourlydata where HourlyReportIndex=" << hourlyReportIndex <<
        " order by Y asc, X asc";

      sqlite3_stmt* sqlStmtPtr;

      int code = sqlite3_prepare_v2(m_db, statement.str().c_str(),-1,&sqlStmtPtr,NULL);
      code = sqlite3_step(sqlStmtPtr);
      if (code == SQLITE_ROW)
      {
        xVal = sqlite3_column_double(sqlStmtPtr,0);
        yVal = sqlite3_column_double(sqlStmtPtr,1);
        yValPrevious = yVal;
        illuminanceVal = sqlite3_column_double(sqlStmtPtr,2);
        x.push_back(xVal);
        y.push_back(yVal);
        illuminance.push_back(illuminanceVal);
      }
      code = sqlite3_step(sqlStmtPtr);

      while ((code == SQLITE_ROW) && (!yValChanged))
      {
        xVal = sqlite3_column_double(sqlStmtPtr,0);
        yVal = sqlite3_column_double(sqlStmtPtr,1);
        illuminanceVal = sqlite3_column_double(sqlStmtPtr,2);

        if (yVal != yValPrevious)
        {
          y.push_back(yVal);
          yValPrevious = yVal;
          yValChanged = true;
        }
        else {
          x.push_back(xVal);
        }

        illuminance.push_back(illuminanceVal);

        // step to next row

        code = sqlite3_step(sqlStmtPtr);
      } // while

      while (code == SQLITE_ROW)
      {
        yVal = sqlite3_column_double(sqlStmtPtr,1);
        illuminanceVal = sqlite3_column_double(sqlStmtPtr,2);

        illuminance.push_back(illuminanceVal);
        // wrap columns at N
        if (yVal != yValPrevious)
        {
          y.push_back(yVal);
          yValPrevious = yVal;
        }

        // step to next row
        code = sqlite3_step(sqlStmtPtr);
      }

      /// must finalize to prevent memory leaks
      sqlite3_finalize(sqlStmtPtr);

    }


    /// value (lux) of the illuminance map at hourlyReportIndex
    /// value(i,j) is the illuminance at x(i), y(j)
    Matrix SqlFile_Impl::illuminanceMap(const int& hourlyReportIndex) const
    {

      /// loop over all points
      Vector x = SqlFile_Impl::illuminanceMapX(hourlyReportIndex);
      Vector y = SqlFile_Impl::illuminanceMapY(hourlyReportIndex);
      unsigned M = x.size();
      unsigned N = y.size();
      Matrix illuminance(M, N, 0);

      unsigned i = 0;
      unsigned j = 0;

      std::stringstream statement;
      statement << "select Illuminance from daylightmaphourlydata where HourlyReportIndex=" << hourlyReportIndex <<
        " order by X asc, Y asc";

      sqlite3_stmt* sqlStmtPtr;

      int code = sqlite3_prepare_v2(m_db, statement.str().c_str(),-1,&sqlStmtPtr,NULL);
      code = sqlite3_step(sqlStmtPtr);
      while (code == SQLITE_ROW)
      {
        if (i >= M){
          LOG(Error, "Too much illiminance map data retrieved at time index " << hourlyReportIndex <<
              " for map name = '" << hourlyReportIndex << "'.  Size is " << M << "x" << N <<
              ", current i is " << i << ", current j is " << j);
          break;
        }

        illuminance(i,j) = sqlite3_column_double(sqlStmtPtr,0);

        // step to next row
        code = sqlite3_step(sqlStmtPtr);

        // wrap columns at N
        ++j;
        if (j==N){
          j=0;
          ++i;
        }
      }

      /// must finalize to prevent memory leaks
      sqlite3_finalize(sqlStmtPtr);

      return illuminance;
    }

    // find the illuminance map index by name
    boost::optional<int> SqlFile_Impl::illuminanceMapIndex(const std::string& name) const
    {
      boost::optional<int> index;

      const std::string& s = "select MapNumber from daylightmaps where MapName like '%" + name + "%'";

      sqlite3_stmt* sqlStmtPtr;

      int code = sqlite3_prepare_v2(m_db, s.c_str(),-1,&sqlStmtPtr,NULL);
      code = sqlite3_step(sqlStmtPtr);

      if(code == SQLITE_ROW)
        index = sqlite3_column_int(sqlStmtPtr,0);

      /// must finalize to prevent memory leaks
      sqlite3_finalize(sqlStmtPtr);

      return index;
    }

    void SqlFile_Impl::mf_makeConsistent(std::vector<SqlFileTimeSeriesQuery>& queries) 
    {
      // make each query consistent, or delete it if there is no possibility of a matching TimeSeries
      // do not check name validity--only cross-validity
      for (SqlFileTimeSeriesQueryVector::iterator queryIt = queries.begin(); 
          queryIt < queries.end(); ) 
      {
        // environment
        OptionalString envName;
        if (queryIt->environment() && queryIt->environment()->name()) {
          envName = queryIt->environment()->name();
        }     

        // reporting frequency
        OptionalReportingFrequency rf;
        if (queryIt->reportingFrequency()) {
          rf = queryIt->reportingFrequency();
        }
        ReportingFrequencySet rfSet;
        if (envName && rf) {
          // check compatibilty
          rfSet = availableReportingFrequencySet(*this,*envName);
          if (rfSet.find(*rf) == rfSet.end()) {
            queryIt = queries.erase(queryIt);
            continue;
          }
        }

        // time series
        OptionalString tsName;
        if (queryIt->timeSeries() && queryIt->timeSeries()->name()) {
          tsName = queryIt->timeSeries()->name();
        }
        IStringSet tsSet;
        StringVector temp;
        StringVector envNames;
        if (envName && tsName) {
          if (rf) {
            temp = availableVariableNames(*envName,rf->valueDescription());
            tsSet.insert(temp.begin(),temp.end());
          }
          else {
            rfSet = availableReportingFrequencySet(*this,*envName);
            BOOST_FOREACH(const ReportingFrequency& rf,rfSet) {
              temp = availableVariableNames(*envName,rf.valueDescription());
              tsSet.insert(temp.begin(),temp.end());
            }
          }
        }
        else if (rf && tsName) {
          envNames = availableEnvPeriods();
          BOOST_FOREACH(const std::string& envName,envNames) {
            temp = availableVariableNames(envName,rf->valueDescription());
            tsSet.insert(temp.begin(),temp.end());
          }
        }
        else if (tsName) {
          temp = availableTimeSeries();
          tsSet.insert(temp.begin(),temp.end());
        }
        if (tsName) {
          if (tsSet.find(*tsName) == tsSet.end()) { 
            queryIt = queries.erase(queryIt);
            continue;
          }
        }

        // key values
        // for now, only check if tsName is specified
        StringVector keyValueNames;
        StringVector kvAvail;
        IStringSet keepers;
        if (tsName && queryIt->keyValues() && !(queryIt->keyValues().get().regex())) {
          keyValueNames = queryIt->keyValues()->names();
          if (envName && rf) {
            kvAvail = availableKeyValues(*envName,rf->valueDescription(),*tsName);
            BOOST_FOREACH(const std::string& kv,keyValueNames) {
              StringVector::const_iterator it = std::find_if(kvAvail.begin(),kvAvail.end(),
                  boost::bind(istringEqual,kv,_1));
              if (it != kvAvail.end()) { keepers.insert(*it); }
            }
          }
          else if (envName) {
            OS_ASSERT(!rfSet.empty());
            BOOST_FOREACH(const ReportingFrequency& rf,rfSet) {
              kvAvail = availableKeyValues(*envName,rf.valueDescription(),*tsName);
              BOOST_FOREACH(const std::string& kv,keyValueNames) {
                StringVector::const_iterator it = std::find_if(kvAvail.begin(),kvAvail.end(),
                    boost::bind(istringEqual,kv,_1));
                if (it != kvAvail.end()) { keepers.insert(*it); }
              }
            }
          }
          else if (rf) {
            OS_ASSERT(!envNames.empty());
            BOOST_FOREACH(const std::string& envName,envNames) {
              kvAvail = availableKeyValues(envName,rf->valueDescription(),*tsName);
              BOOST_FOREACH(const std::string& kv,keyValueNames) {
                StringVector::const_iterator it = std::find_if(kvAvail.begin(),kvAvail.end(),
                    boost::bind(istringEqual,kv,_1));
                if (it != kvAvail.end()) { keepers.insert(*it); }
              }
            }
          }
          else {
            envNames = availableEnvPeriods();
            BOOST_FOREACH(const std::string& envName,envNames) {
              rfSet = availableReportingFrequencySet(*this,envName);
              BOOST_FOREACH(const ReportingFrequency& rf,rfSet) {
                kvAvail = availableKeyValues(envName,rf.valueDescription(),*tsName);
                BOOST_FOREACH(const std::string& kv,keyValueNames) {
                  StringVector::const_iterator it = std::find_if(kvAvail.begin(),kvAvail.end(),
                      boost::bind(istringEqual,kv,_1));
                  if (it != kvAvail.end()) { 
                    keepers.insert(*it); 
                    if (keepers.size() == keyValueNames.size()) { break; }
                  }
                }
                if (keepers.size() == keyValueNames.size()) { break; }
              }
              if (keepers.size() == keyValueNames.size()) { break; }
            }           
          }
          // set key values to keepers
          if (keepers.empty()) {
            queryIt = queries.erase(queryIt);
            continue;
          }
          else {
            KeyValueIdentifier kvId(StringVector(keepers.begin(),keepers.end()));
            queryIt->setKeyValues(kvId);
          }
        }

        // check next query
        ++queryIt;
      } // for
    }

    // NON-MEMBER HELPER FUNCTIONS
    ReportingFrequencySet availableReportingFrequencySet(SqlFile_Impl& sqlFileImpl, const std::string& envPeriod) {
      ReportingFrequencySet rfSet;
      StringVector rfStrs = sqlFileImpl.availableReportingFrequencies(envPeriod);
      BOOST_FOREACH(const std::string& rfStr,rfStrs) {
        OptionalReportingFrequency orf = sqlFileImpl.reportingFrequencyFromDB(rfStr);
        if (orf) { rfSet.insert(*orf); }
      }
      OS_ASSERT(rfSet.size() == rfStrs.size());
      return rfSet;
    }
  } // detail

} // openstudio
