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

#include <cstring>
#include <sstream>
#include <iterator>
#include <algorithm>

#include "OpenStudioPostProcessJob.hpp"
#include "PostProcessReporting.hpp"
#include "FileInfo.hpp"
#include "JobOutputCleanup.hpp"
#include "RunManager_Util.hpp"

#include <utilities/time/DateTime.hpp>
#include <model/Model.hpp>
#include <model/Model_Impl.hpp>
#include <model/Building.hpp>
#include <model/Building_Impl.hpp>
#include <model/Facility.hpp>
#include <model/Facility_Impl.hpp>
#include <model/TimeDependentValuation.hpp>
#include <model/TimeDependentValuation_Impl.hpp>

#include <energyplus/ReverseTranslator.hpp>

#include <utilities/data/CalibrationResult.hpp>
#include <utilities/data/EndUses.hpp>
#include <utilities/data/Attribute.hpp>
#include <utilities/idf/IdfFile.hpp>
#include <utilities/idf/Workspace.hpp>
#include <utilities/sql/SqlFile.hpp>

#include <QDir>
#include <QDateTime>

#include <boost/bind.hpp>

namespace openstudio {
namespace runmanager {
namespace detail {


  OpenStudioPostProcessJob::OpenStudioPostProcessJob(const UUID &t_uuid,
          const Tools &tools,
          const JobParams &params,
          const Files &files,
      const JobState &t_restoreData)
    : Job_Impl(t_uuid, JobType::OpenStudioPostProcess, tools, params, files, t_restoreData)
  {
    try {
      m_sql = files.getLastByFilename("eplusout.sql");
      resetFiles(m_files, m_sql);
    } catch (const std::runtime_error &) {
    }

    // what should this be?
    m_description = buildDescription("sql");
  }

  OpenStudioPostProcessJob::~OpenStudioPostProcessJob()
  {
    requestStop();
    wait();
    // disconnect any remaining slots
    disconnect(this);
  }

  void OpenStudioPostProcessJob::requestStop()
  {
  }

  bool OpenStudioPostProcessJob::outOfDateImpl(const boost::optional<QDateTime> &t_lastrun) const
  {
    if (!t_lastrun) {
      return true;
    } else {
      QReadLocker l(&m_mutex);
      if (m_sql) {
        return filesChanged(m_files, *t_lastrun);
      } else {
        // must have a sql
        return true;
      }
    }

  }

  void OpenStudioPostProcessJob::cleanup()
  {
  }

  std::string OpenStudioPostProcessJob::description() const
  {
    QReadLocker l(&m_mutex);
    return m_description;
  }

  void OpenStudioPostProcessJob::startImpl(const boost::shared_ptr<ProcessCreator> &)
  {
    QWriteLocker l(&m_mutex);

    JobErrors errors;
    errors.result = ruleset::OSResultValue::Success;
    
    try {
      m_sql = sqlFile();
      m_osm = osmFile();
      resetFiles(m_files, m_sql, m_osm);
    } catch (const std::exception &e) {
      JobErrors error;
      error.result = ruleset::OSResultValue::Fail;
      error.addError(ErrorType::Error, e.what());
      errors = error;
    }


    if (m_sql)
    {
      LOG(Info, "OpenStudioPostProcess starting, filename: " << toString(m_sql->fullPath));
    }

    LOG(Info, "OpenStudioPostProcess starting, outdir: " << toString(outdir()));

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
      SqlFile sqlFile(m_sql->fullPath);

      boost::optional<openstudio::model::Model> model = model::Model::load(m_osm->fullPath);

      if (!model)
      {
        throw std::runtime_error("Unable to load model file from " + openstudio::toString(m_osm->fullPath));
      }

      model->setSqlFile(sqlFile);

      boost::filesystem::create_directories(outdir(true));

      std::vector<Attribute> attributes = PostProcessReporting::go(sqlFile);

      boost::optional<model::Building> building = model->getOptionalUniqueModelObject<model::Building>();
      if (building)
      {
        LOG(Debug,"Extracting attributes from model::Building.");
        boost::optional<Attribute> attribute;
        
        boost::optional<double> value = building->floorArea();
        if (value){
          attribute = Attribute("floorArea", *value, "m^2");
          attribute->setDisplayName("Floor Area");
          attributes.push_back(*attribute);
        }

        value = building->conditionedFloorArea();
        if (value){
          attribute = Attribute("conditionedFloorArea", *value, "m^2");
          attribute->setDisplayName("Conditioned Floor Area");
          attributes.push_back(*attribute);
        }
      }

      boost::optional<model::Facility> facility = model->getOptionalUniqueModelObject<model::Facility>();
      if (facility){
        LOG(Debug,"Extracting attributes from model::Facility.");     

        boost::optional<Attribute> attribute;

        boost::optional<double> value = facility->economicsCapitalCost();
        if (value){
          attribute = Attribute("economicsCapitalCost", *value, "$");
          attribute->setDisplayName("Capital Cost");
          attributes.push_back(*attribute);
        }

        value = facility->economicsTLCC();
        if (value){
          attribute = Attribute("economicsTLCC", *value, "$");
          attribute->setDisplayName("Total Life Cycle Cost");
          attributes.push_back(*attribute);
        }

        value = facility->annualWaterTotalCost();
        if (value){
          attribute = Attribute("annualWaterTotalCost", *value, "$");
          attribute->setDisplayName("Annual Water Total Cost");
          attributes.push_back(*attribute);
        }

        attribute = facility->endUsesAttribute();
        if (attribute){
          attributes.push_back(*attribute);
        }

        attribute = facility->calibrationResultAttribute();
        if (attribute){
          attributes.push_back(*attribute);
        }
      }

      boost::optional<model::TimeDependentValuation> timeDependentValuation = model->getOptionalUniqueModelObject<model::TimeDependentValuation>();
      if (timeDependentValuation){
        LOG(Debug,"Extracting attributes from model::TimeDependentValuation.");      
        boost::optional<Attribute> attribute;
        
        boost::optional<double> value = timeDependentValuation->energyTimeDependentValuation();
        if (value){
          attribute = Attribute("energyTimeDependentValuation", *value, "J");
          attribute->setDisplayName("Energy Time Dependent Valuation");
          attributes.push_back(*attribute);
        }

        value = timeDependentValuation->costTimeDependentValuation();
        if (value){
          attribute = Attribute("costTimeDependentValuation", *value, "$");
          attribute->setDisplayName("Cost Time Dependent Valuation");
          attributes.push_back(*attribute);
        }
      }

      if (attributes.empty())
      {
        LOG(Warn, "No attributes loaded for report");
      }

      Attribute report("Report", attributes);
      bool result = report.saveToXml(outdir() / openstudio::toPath("report.xml"));
      if (!result){
        LOG_AND_THROW("Failed to write report.xml");
      }

    } catch (const std::exception &e) {
      LOG(Error, "Error with OpenStudioPostProcessJob: " + std::string(e.what()));
      errors.addError(ErrorType::Error, "Error with OpenStudioPostProcessJob: " + std::string(e.what()));
      errors.result = ruleset::OSResultValue::Fail;
    }

    // Change this to whatever output files you generate
    emitOutputFileChanged(RunManager_Util::dirFile(outdir() / openstudio::toPath("report.xml")));
    setErrors(errors);
  }

  std::string OpenStudioPostProcessJob::getOutput() const
  {
    return "";
  }

  void OpenStudioPostProcessJob::basePathChanged() 
  {
    m_sql.reset();
    m_osm.reset();
    resetFiles(m_files);
  }

  FileInfo OpenStudioPostProcessJob::sqlFile() const
  {
    if (!m_sql)
    {
      return allInputFiles().getLastByFilename("eplusout.sql");
    } else {
      return *m_sql;
    }
  }

  FileInfo OpenStudioPostProcessJob::osmFile() const
  {
    if (!m_osm)
    {
      return allInputFiles().getLastByExtension("osm");
    } else {
      return *m_osm;
    }
  }

  Files OpenStudioPostProcessJob::outputFilesImpl() const
  {
    // Dan: what's the output files generated?
    if (!boost::filesystem::exists(outdir() / toPath("report.xml")))
    {
      // no output file has been generated yet
      return Files();
    }

    Files f;
    f.append(FileInfo(outdir() / toPath("report.xml"), "xml"));
    return f;
  }


} // detail
} // runmanager
} // openstudio



