/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include <cstring>
#include <sstream>
#include <fstream>
#include <iterator>
#include <algorithm>

#include "EnergyPlusPreProcessJob.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"
#include "RunManager_Util.hpp"

#include "../../utilities/time/DateTime.hpp"
#include "../../model/Model.hpp"
#include "../../model/Model_Impl.hpp"
#include "../../model/Building.hpp"
#include "../../model/Building_Impl.hpp"
#include "../../model/Facility.hpp"
#include "../../model/Facility_Impl.hpp"

#include "../../energyplus/ReverseTranslator.hpp"

#include "../../utilities/data/Attribute.hpp"
#include "../../utilities/core/Compare.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/sql/SqlFile.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/Output_Meter_FieldEnums.hxx>
#include <utilities/idd/Output_Variable_FieldEnums.hxx>

#include <QDir>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {


  EnergyPlusPreProcessJob::EnergyPlusPreProcessJob(const UUID &t_uuid,
      const Tools &tools,
      const JobParams &params,
      const Files &files,
      const JobState &t_restoreData)
    : Job_Impl(t_uuid, JobType::EnergyPlusPreProcess, tools, params, files, t_restoreData)
  {
    try {
      m_idf = files.getLastByExtension("idf");
      resetFiles(m_files, m_idf);
    } catch (const std::runtime_error &) {
    }

    m_description = buildDescription("idf");
  }

  EnergyPlusPreProcessJob::~EnergyPlusPreProcessJob()
  {
    requestStop();
    wait();
    // disconnect any remaining slots
    disconnect(this);
  }

  void EnergyPlusPreProcessJob::requestStop()
  {
  }


  bool EnergyPlusPreProcessJob::outOfDateImpl(const boost::optional<QDateTime> &t_lastrun) const
  {
    if (!t_lastrun) {
      return true;
    } else {
      QReadLocker l(&m_mutex);
      if (m_idf) {
        return filesChanged(m_files, *t_lastrun);
      } else {
        return true;
      }
    }
  }

  void EnergyPlusPreProcessJob::cleanup()
  {
  }

  std::string EnergyPlusPreProcessJob::description() const
  {
    QReadLocker l(&m_mutex);
    return m_description;
  }

  void EnergyPlusPreProcessJob::startImpl(const std::shared_ptr<ProcessCreator> &)
  {
    openstudio::path outpath = outdir();

    QWriteLocker l(&m_mutex);

    JobErrors errors;
    errors.result = ruleset::OSResultValue::Success;

    try {
      m_idf = idfFile();
      resetFiles(m_files, m_idf); 
    } catch (const std::runtime_error &e) {
      errors.result = ruleset::OSResultValue::Fail;
      errors.addError(ErrorType::Error, e.what());
    }

    LOG(Info, "EnergyPlusPreProcess starting, outdir: " << toString(outpath));

    l.unlock();

    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Starting));

    emitStarted();
    emitStatusChanged(AdvancedStatus(AdvancedStatusEnum::Processing));

    if (errors.result == ruleset::OSResultValue::Fail)
    {
      setErrors(errors);
      return;
    }

    try {
      boost::filesystem::create_directories(outdir(true));

      bool needssqlobj = false;
      bool needsmonthlyoutput = false;

      // this is a workaround for issue #1699
      bool needsdetailedvariable = false;
      bool needstimestepvariable = true; // we add these at the bottom of the file
      bool needshourlyvariable = false;
      bool needsdailyvariable = false;
      bool needsmonthlyvariable = false;
      bool needsrunperiodvariable = false;

      {
        boost::optional<openstudio::Workspace> ws = openstudio::Workspace::load(m_idf->fullPath);

        if (!ws){
          LOG_AND_THROW("Unable to load idf into workspace");
        }

        std::vector<openstudio::WorkspaceObject> sqliteobjs = ws->getObjectsByType(IddObjectType::Output_SQLite);

        if (sqliteobjs.empty())
        {
          //          ws->addObject(IdfObject(IddObjectType::Output_SQLite));
          needssqlobj = true;
          //          sqliteobjs = ws->getObjectsByType(IddObjectType::Output_SQLite);
        }

        if (ws->getObjectsByName("Building Energy Performance - Natural Gas").empty()
            || ws->getObjectsByName("Building Energy Performance - Electricity").empty()
            || ws->getObjectsByName("Building Energy Performance - District Heating").empty()
            || ws->getObjectsByName("Building Energy Performance - District Cooling").empty()
           )
        {
          needsmonthlyoutput = true;
        }

        // always request Output:Constructions
        // DLM: these tables are not useful for getting visible transmittance in the SQL output https://github.com/NREL/EnergyPlus/issues/5272
        //std::vector<openstudio::WorkspaceObject> outputConstructions = ws->getObjectsByType(IddObjectType::Output_Constructions);
        //for (auto outputConstruction : outputConstructions){
        //  outputConstruction.remove();
        //}

        // this is a workaround for issue #1699
        // get all the meter requests
        std::vector<openstudio::WorkspaceObject> meterobjs;
        std::vector<openstudio::WorkspaceObject> tempobjs = ws->getObjectsByType(IddObjectType::Output_Meter);
        meterobjs.insert(meterobjs.end(), tempobjs.begin(), tempobjs.end());
        tempobjs = ws->getObjectsByType(IddObjectType::Output_Meter_MeterFileOnly);
        meterobjs.insert(meterobjs.end(), tempobjs.begin(), tempobjs.end());
        tempobjs = ws->getObjectsByType(IddObjectType::Output_Meter_Cumulative);
        meterobjs.insert(meterobjs.end(), tempobjs.begin(), tempobjs.end());
        tempobjs = ws->getObjectsByType(IddObjectType::Output_Meter_Cumulative_MeterFileOnly);
        meterobjs.insert(meterobjs.end(), tempobjs.begin(), tempobjs.end());

        for (const WorkspaceObject& meterobj : meterobjs){
          // DLM: same index for all types of meters
          boost::optional<std::string> reportingFrequency = meterobj.getString(Output_MeterFields::ReportingFrequency, true);
          OS_ASSERT(reportingFrequency);
          if (istringEqual(*reportingFrequency, "Detailed")){
            needsdetailedvariable = true;
          } else if (istringEqual(*reportingFrequency, "Timestep")){
            needstimestepvariable = true;
          } else if (istringEqual(*reportingFrequency, "Hourly")){
            needshourlyvariable = true;
          } else if (istringEqual(*reportingFrequency, "Daily")){
            needsdailyvariable = true;
          } else if (istringEqual(*reportingFrequency, "Monthly")){
            needsmonthlyvariable = true;
          } else if (istringEqual(*reportingFrequency, "RunPeriod") || istringEqual(*reportingFrequency, "Environment") || istringEqual(*reportingFrequency, "Annual")){
            needsrunperiodvariable = true;
          }
        }
        
        // get all the variable requests
        std::vector<openstudio::WorkspaceObject> variableobjs = ws->getObjectsByType(IddObjectType::Output_Variable);

        for (const WorkspaceObject& variableobj : variableobjs){
          boost::optional<std::string> reportingFrequency = variableobj.getString(Output_VariableFields::ReportingFrequency, true);
          OS_ASSERT(reportingFrequency);
          if (istringEqual(*reportingFrequency, "Detailed")){
            needsdetailedvariable = false;
          } else if (istringEqual(*reportingFrequency, "Timestep")){
            needstimestepvariable = false;
          } else if (istringEqual(*reportingFrequency, "Hourly")){
            needshourlyvariable = false;
          } else if (istringEqual(*reportingFrequency, "Daily")){
            needsdailyvariable = false;
          } else if (istringEqual(*reportingFrequency, "Monthly")){
            needsmonthlyvariable = false;
          } else if (istringEqual(*reportingFrequency, "RunPeriod") || istringEqual(*reportingFrequency, "Environment") || istringEqual(*reportingFrequency, "Annual")){
            needsrunperiodvariable = false;
          }
        }

        // warn if using OutputControl:Table:Style to change reporting units
        for (auto& outputControlStyle : ws->getObjectsByType(IddObjectType::OutputControl_Table_Style))
        {
          boost::optional<std::string> unitConversion = outputControlStyle.getString(1);
          if (unitConversion && !istringEqual(unitConversion.get(), "None")){
            errors.addError(ErrorType::Warning, "OutputControl:Table:Style object with unsupported value '" + unitConversion.get() + "'.  OpenStudio results processing may not work correctly.");
          }
        }
      }
 
      openstudio::path outfile = outdir(true)/openstudio::toPath("out.idf");

      if (boost::filesystem::exists(outfile))
      {
        try {
          boost::filesystem::remove(outfile);
        } catch (const boost::filesystem::filesystem_error &e) {
          LOG(Error, "Error removing existing out.idf file: " + std::string(e.what()) + " continuing with run, if copy_file errors, the process will fail");
        }
      }

      boost::filesystem::copy_file(m_idf->fullPath, outfile, boost::filesystem::copy_option::overwrite_if_exists);
      std::ofstream ofs(openstudio::toString(outfile).c_str(), std::ios::app);

      if (needssqlobj)
      {
        ofs << "Output:SQLite," << std::endl;
        ofs << "  SimpleAndTabular;         ! Option Type" << std::endl;
      }

      if (needsmonthlyoutput)
      {
        //energy consumption

        ofs << "Output:Table:Monthly," << std::endl;
        ofs << "  Building Energy Performance - Electricity,  !- Name" << std::endl;
        ofs << "    2,                       !- Digits After Decimal" << std::endl;
        ofs << "    InteriorLights:Electricity,  !- Variable or Meter 1 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 1" << std::endl;
        ofs << "    ExteriorLights:Electricity,  !- Variable or Meter 2 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 2" << std::endl;
        ofs << "    InteriorEquipment:Electricity,  !- Variable or Meter 3 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 3" << std::endl;
        ofs << "    ExteriorEquipment:Electricity,  !- Variable or Meter 4 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 4" << std::endl;
        ofs << "    Fans:Electricity,        !- Variable or Meter 5 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 5" << std::endl;
        ofs << "    Pumps:Electricity,       !- Variable or Meter 6 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 6" << std::endl;
        ofs << "    Heating:Electricity,     !- Variable or Meter 7 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 7" << std::endl;
        ofs << "    Cooling:Electricity,     !- Variable or Meter 8 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 8" << std::endl;
        ofs << "    HeatRejection:Electricity,  !- Variable or Meter 9 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 9" << std::endl;
        ofs << "    Humidifier:Electricity,  !- Variable or Meter 10 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 10" << std::endl;
        ofs << "    HeatRecovery:Electricity,!- Variable or Meter 11 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 11" << std::endl;
        ofs << "    WaterSystems:Electricity,!- Variable or Meter 12 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 12" << std::endl;
        ofs << "    Cogeneration:Electricity,!- Variable or Meter 13 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 13" << std::endl;
        ofs << "    Refrigeration:Electricity,!- Variable or Meter 14 Name" << std::endl;
        ofs << "    SumOrAverage;            !- Aggregation Type for Variable or Meter 14" << std::endl;

        ofs << "Output:Table:Monthly," << std::endl;
        ofs << "  Building Energy Performance - Natural Gas,  !- Name" << std::endl;
        ofs << "    2,                       !- Digits After Decimal" << std::endl;
        ofs << "    InteriorEquipment:Gas,   !- Variable or Meter 1 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 1" << std::endl;
        ofs << "    ExteriorEquipment:Gas,   !- Variable or Meter 2 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 2" << std::endl;
        ofs << "    Heating:Gas,             !- Variable or Meter 3 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 3" << std::endl;
        ofs << "    Cooling:Gas,             !- Variable or Meter 4 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 4" << std::endl;
        ofs << "    WaterSystems:Gas,        !- Variable or Meter 5 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 5" << std::endl;
        ofs << "    Cogeneration:Gas,        !- Variable or Meter 6 Name" << std::endl;
        ofs << "    SumOrAverage;            !- Aggregation Type for Variable or Meter 6" << std::endl;

        ofs << "Output:Table:Monthly," << std::endl;
        ofs << "  Building Energy Performance - District Heating,  !- Name" << std::endl;
        ofs << "    2,                       !- Digits After Decimal" << std::endl;
        ofs << "    InteriorLights:DistrictHeating,  !- Variable or Meter 1 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 1" << std::endl;
        ofs << "    ExteriorLights:DistrictHeating,  !- Variable or Meter 2 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 2" << std::endl;
        ofs << "    InteriorEquipment:DistrictHeating,  !- Variable or Meter 3 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 3" << std::endl;
        ofs << "    ExteriorEquipment:DistrictHeating,  !- Variable or Meter 4 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 4" << std::endl;
        ofs << "    Fans:DistrictHeating,        !- Variable or Meter 5 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 5" << std::endl;
        ofs << "    Pumps:DistrictHeating,       !- Variable or Meter 6 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 6" << std::endl;
        ofs << "    Heating:DistrictHeating,     !- Variable or Meter 7 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 7" << std::endl;
        ofs << "    Cooling:DistrictHeating,     !- Variable or Meter 8 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 8" << std::endl;
        ofs << "    HeatRejection:DistrictHeating,  !- Variable or Meter 9 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 9" << std::endl;
        ofs << "    Humidifier:DistrictHeating,  !- Variable or Meter 10 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 10" << std::endl;
        ofs << "    HeatRecovery:DistrictHeating,!- Variable or Meter 11 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 11" << std::endl;
        ofs << "    WaterSystems:DistrictHeating,!- Variable or Meter 12 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 12" << std::endl;
        ofs << "    Cogeneration:DistrictHeating,!- Variable or Meter 13 Name" << std::endl;
        ofs << "    SumOrAverage;            !- Aggregation Type for Variable or Meter 13" << std::endl;

        ofs << "Output:Table:Monthly," << std::endl;
        ofs << "  Building Energy Performance - District Cooling,  !- Name" << std::endl;
        ofs << "    2,                       !- Digits After Decimal" << std::endl;
        ofs << "    InteriorLights:DistrictCooling,  !- Variable or Meter 1 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 1" << std::endl;
        ofs << "    ExteriorLights:DistrictCooling,  !- Variable or Meter 2 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 2" << std::endl;
        ofs << "    InteriorEquipment:DistrictCooling,  !- Variable or Meter 3 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 3" << std::endl;
        ofs << "    ExteriorEquipment:DistrictCooling,  !- Variable or Meter 4 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 4" << std::endl;
        ofs << "    Fans:DistrictCooling,        !- Variable or Meter 5 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 5" << std::endl;
        ofs << "    Pumps:DistrictCooling,       !- Variable or Meter 6 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 6" << std::endl;
        ofs << "    Heating:DistrictCooling,     !- Variable or Meter 7 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 7" << std::endl;
        ofs << "    Cooling:DistrictCooling,     !- Variable or Meter 8 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 8" << std::endl;
        ofs << "    HeatRejection:DistrictCooling,  !- Variable or Meter 9 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 9" << std::endl;
        ofs << "    Humidifier:DistrictCooling,  !- Variable or Meter 10 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 10" << std::endl;
        ofs << "    HeatRecovery:DistrictCooling,!- Variable or Meter 11 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 11" << std::endl;
        ofs << "    WaterSystems:DistrictCooling,!- Variable or Meter 12 Name" << std::endl;
        ofs << "    SumOrAverage,            !- Aggregation Type for Variable or Meter 12" << std::endl;
        ofs << "    Cogeneration:DistrictCooling,!- Variable or Meter 13 Name" << std::endl;
        ofs << "    SumOrAverage;            !- Aggregation Type for Variable or Meter 13" << std::endl;

        //energy demand

        ofs << "Output:Table:Monthly," << std::endl;
        ofs << "  Building Energy Performance - Electricity Peak Demand,  !- Name" << std::endl;
        ofs << "    2,                       !- Digits After Decimal" << std::endl;
        ofs << "    Electricity:Facility,  !- Variable or Meter 1 Name" << std::endl;
        ofs << "    Maximum,            !- Aggregation Type for Variable or Meter 1" << std::endl;
        ofs << "    InteriorLights:Electricity,  !- Variable or Meter 1 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 1" << std::endl;
        ofs << "    ExteriorLights:Electricity,  !- Variable or Meter 2 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 2" << std::endl;
        ofs << "    InteriorEquipment:Electricity,  !- Variable or Meter 3 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 3" << std::endl;
        ofs << "    ExteriorEquipment:Electricity,  !- Variable or Meter 4 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 4" << std::endl;
        ofs << "    Fans:Electricity,        !- Variable or Meter 5 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 5" << std::endl;
        ofs << "    Pumps:Electricity,       !- Variable or Meter 6 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 6" << std::endl;
        ofs << "    Heating:Electricity,     !- Variable or Meter 7 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 7" << std::endl;
        ofs << "    Cooling:Electricity,     !- Variable or Meter 8 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 8" << std::endl;
        ofs << "    HeatRejection:Electricity,  !- Variable or Meter 9 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 9" << std::endl;
        ofs << "    Humidifier:Electricity,  !- Variable or Meter 10 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 10" << std::endl;
        ofs << "    HeatRecovery:Electricity,!- Variable or Meter 11 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 11" << std::endl;
        ofs << "    WaterSystems:Electricity,!- Variable or Meter 12 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 12" << std::endl;
        ofs << "    Cogeneration:Electricity,!- Variable or Meter 13 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 13" << std::endl;
        ofs << "    Refrigeration:Electricity,!- Variable or Meter 14 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum;            !- Aggregation Type for Variable or Meter 14" << std::endl;

        ofs << "Output:Table:Monthly," << std::endl;
        ofs << "  Building Energy Performance - Natural Gas Peak Demand,  !- Name" << std::endl;
        ofs << "    2,                       !- Digits After Decimal" << std::endl;
        ofs << "    Gas:Facility,  !- Variable or Meter 1 Name" << std::endl;
        ofs << "    Maximum,            !- Aggregation Type for Variable or Meter 1" << std::endl;
        ofs << "    InteriorEquipment:Gas,   !- Variable or Meter 1 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 1" << std::endl;
        ofs << "    ExteriorEquipment:Gas,   !- Variable or Meter 2 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 2" << std::endl;
        ofs << "    Heating:Gas,             !- Variable or Meter 3 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 3" << std::endl;
        ofs << "    Cooling:Gas,             !- Variable or Meter 4 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 4" << std::endl;
        ofs << "    WaterSystems:Gas,        !- Variable or Meter 5 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 5" << std::endl;
        ofs << "    Cogeneration:Gas,        !- Variable or Meter 6 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum;            !- Aggregation Type for Variable or Meter 6" << std::endl;

        ofs << "Output:Table:Monthly," << std::endl;
        ofs << "  Building Energy Performance - District Heating Peak Demand,  !- Name" << std::endl;
        ofs << "    2,                       !- Digits After Decimal" << std::endl;
        ofs << "    DistrictHeating:Facility,  !- Variable or Meter 1 Name" << std::endl;
        ofs << "    Maximum,            !- Aggregation Type for Variable or Meter 1" << std::endl;
        ofs << "    InteriorLights:DistrictHeating,  !- Variable or Meter 1 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 1" << std::endl;
        ofs << "    ExteriorLights:DistrictHeating,  !- Variable or Meter 2 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 2" << std::endl;
        ofs << "    InteriorEquipment:DistrictHeating,  !- Variable or Meter 3 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 3" << std::endl;
        ofs << "    ExteriorEquipment:DistrictHeating,  !- Variable or Meter 4 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 4" << std::endl;
        ofs << "    Fans:DistrictHeating,        !- Variable or Meter 5 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 5" << std::endl;
        ofs << "    Pumps:DistrictHeating,       !- Variable or Meter 6 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 6" << std::endl;
        ofs << "    Heating:DistrictHeating,     !- Variable or Meter 7 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 7" << std::endl;
        ofs << "    Cooling:DistrictHeating,     !- Variable or Meter 8 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 8" << std::endl;
        ofs << "    HeatRejection:DistrictHeating,  !- Variable or Meter 9 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 9" << std::endl;
        ofs << "    Humidifier:DistrictHeating,  !- Variable or Meter 10 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 10" << std::endl;
        ofs << "    HeatRecovery:DistrictHeating,!- Variable or Meter 11 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 11" << std::endl;
        ofs << "    WaterSystems:DistrictHeating,!- Variable or Meter 12 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 12" << std::endl;
        ofs << "    Cogeneration:DistrictHeating,!- Variable or Meter 13 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum;            !- Aggregation Type for Variable or Meter 13" << std::endl;

        ofs << "Output:Table:Monthly," << std::endl;
        ofs << "  Building Energy Performance - District Cooling Peak Demand,  !- Name" << std::endl;
        ofs << "    2,                       !- Digits After Decimal" << std::endl;
        ofs << "    DistrictCooling:Facility,  !- Variable or Meter 1 Name" << std::endl;
        ofs << "    Maximum,            !- Aggregation Type for Variable or Meter 1" << std::endl;
        ofs << "    InteriorLights:DistrictCooling,  !- Variable or Meter 1 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 1" << std::endl;
        ofs << "    ExteriorLights:DistrictCooling,  !- Variable or Meter 2 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 2" << std::endl;
        ofs << "    InteriorEquipment:DistrictCooling,  !- Variable or Meter 3 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 3" << std::endl;
        ofs << "    ExteriorEquipment:DistrictCooling,  !- Variable or Meter 4 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 4" << std::endl;
        ofs << "    Fans:DistrictCooling,        !- Variable or Meter 5 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 5" << std::endl;
        ofs << "    Pumps:DistrictCooling,       !- Variable or Meter 6 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 6" << std::endl;
        ofs << "    Heating:DistrictCooling,     !- Variable or Meter 7 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 7" << std::endl;
        ofs << "    Cooling:DistrictCooling,     !- Variable or Meter 8 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 8" << std::endl;
        ofs << "    HeatRejection:DistrictCooling,  !- Variable or Meter 9 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 9" << std::endl;
        ofs << "    Humidifier:DistrictCooling,  !- Variable or Meter 10 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 10" << std::endl;
        ofs << "    HeatRecovery:DistrictCooling,!- Variable or Meter 11 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 11" << std::endl;
        ofs << "    WaterSystems:DistrictCooling,!- Variable or Meter 12 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum,            !- Aggregation Type for Variable or Meter 12" << std::endl;
        ofs << "    Cogeneration:DistrictCooling,!- Variable or Meter 13 Name" << std::endl;
        ofs << "    ValueWhenMaximumOrMinimum;            !- Aggregation Type for Variable or Meter 13" << std::endl;

      }

      if (needsdetailedvariable){
        ofs << "    Output:Variable,*,Site Outdoor Air Drybulb Temperature,Detailed;" << std::endl;
      }
      if (needstimestepvariable){
        ofs << "    Output:Variable,*,Site Outdoor Air Drybulb Temperature,Timestep;" << std::endl;
      }
      if (needshourlyvariable){
        ofs << "    Output:Variable,*,Site Outdoor Air Drybulb Temperature,Hourly;" << std::endl;
      }
      if (needsdailyvariable){
        ofs << "    Output:Variable,*,Site Outdoor Air Drybulb Temperature,Daily;" << std::endl;
      }
      if (needsmonthlyvariable){
        ofs << "    Output:Variable,*,Site Outdoor Air Drybulb Temperature,Monthly;" << std::endl;
      }
      if (needsrunperiodvariable){
        ofs << "    Output:Variable,*,Site Outdoor Air Drybulb Temperature,RunPeriod;" << std::endl;
      }

      //timestep-level utility demand by fuel type to calculate demand
      ofs << "    Output:Meter,Electricity:Facility,Timestep; !- [J]" << std::endl;
      ofs << "    Output:Meter,Gas:Facility,Timestep; !- [J]" << std::endl;
      ofs << "    Output:Meter,DistrictCooling:Facility,Timestep; !- [J]" << std::endl;
      ofs << "    Output:Meter,DistrictHeating:Facility,Timestep; !- [J]" << std::endl;

      //// always report Output:Constructions
      // DLM: these tables are not useful for getting visible transmittance in the SQL output https://github.com/NREL/EnergyPlus/issues/5272
      //ofs << "    Output:Constructions,Constructions,Materials;";

      ofs.flush();
      ofs.close();

    } catch (const std::exception &e) {
      LOG(Error, "Error with EnergyPlusPreProcessJob: " + std::string(e.what()));
      errors.addError(ErrorType::Error, "Error with EnergyPlusPreProcessJob: " + std::string(e.what()));
      errors.result = ruleset::OSResultValue::Fail;
    }

    emitOutputFileChanged(RunManager_Util::dirFile(outpath / openstudio::toPath("out.idf")));
    setErrors(errors);
  }

  std::string EnergyPlusPreProcessJob::getOutput() const
  {
    return "";
  }

  void EnergyPlusPreProcessJob::basePathChanged()
  {
    m_idf.reset();
    resetFiles(m_files);
  }

  FileInfo EnergyPlusPreProcessJob::idfFile() const
  {
    if (!m_idf)
    {
      return allInputFiles().getLastByExtension("idf");
    } else {
      return *m_idf;
    }
  }

  Files EnergyPlusPreProcessJob::outputFilesImpl() const
  {
    openstudio::path outpath = outdir();
    if (!boost::filesystem::exists(outpath / toPath("out.idf")))
    {
      // no output file has been generated yet
      return Files();
    }

    Files f;
    FileInfo fi(outpath / toPath("out.idf"), "idf");
    if (m_idf)
    {
      fi.requiredFiles = m_idf->requiredFiles;
    }
    f.append(fi);
    return f;
  }


} // detail
} // runmanager
} // openstudio



