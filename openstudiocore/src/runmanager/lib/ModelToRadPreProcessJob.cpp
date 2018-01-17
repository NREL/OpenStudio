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
#include <iterator>
#include <algorithm>

#include "ModelToRadPreProcessJob.hpp"
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
#include "../../model/Timestep.hpp"
#include "../../model/Timestep_Impl.hpp"
#include "../../model/ThermalZone.hpp"
#include "../../model/WeatherFile.hpp"
#include "../../model/WeatherFile_Impl.hpp"
#include "../../model/Space.hpp"
#include "../../model/Space_Impl.hpp"
#include "../../model/Surface.hpp"
#include "../../model/Surface_Impl.hpp"
#include "../../model/SubSurface.hpp"
#include "../../model/SubSurface_Impl.hpp"
#include "../../model/SpaceItem.hpp"
#include "../../model/SpaceItem_Impl.hpp"
#include "../../model/ShadingSurfaceGroup.hpp"
#include "../../model/ShadingSurfaceGroup_Impl.hpp"
#include "../../model/SimulationControl.hpp"
#include "../../model/SimulationControl_Impl.hpp"
#include "../../model/RunPeriod.hpp"
#include "../../model/RunPeriod_Impl.hpp"
#include "../../model/People.hpp"
#include "../../model/People_Impl.hpp"
#include "../../model/Lights.hpp"
#include "../../model/Lights_Impl.hpp"
#include "../../model/Luminaire.hpp"
#include "../../model/Luminaire_Impl.hpp"
#include "../../model/OutputVariable.hpp"
#include "../../model/OutputVariable_Impl.hpp"

#include "../../energyplus/ReverseTranslator.hpp"

#include "../../utilities/data/Attribute.hpp"
#include "../../utilities/idf/IdfFile.hpp"
#include "../../utilities/idf/Workspace.hpp"
#include "../../utilities/sql/SqlFile.hpp"

#include "../../utilities/core/Assert.hpp"

#include <QDir>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {


  ModelToRadPreProcessJob::ModelToRadPreProcessJob(const UUID &t_uuid,
          const Tools &tools,
          const JobParams &params,
          const Files &files,
      const JobState &t_restoreData)
    : Job_Impl(t_uuid, JobType::ModelToRadPreProcess, tools, params, files, t_restoreData)
  {
    try {
      m_osm = files.getLastByExtension("osm");
      resetFiles(m_files, m_osm);
    } catch (const std::exception &) {
    }

    m_description = buildDescription("osm");
  }

  ModelToRadPreProcessJob::~ModelToRadPreProcessJob()
  {
    requestStop();
    wait();
    // disconnect any remaining slots
    disconnect(this);
  }

  void ModelToRadPreProcessJob::requestStop()
  {
  }

  bool ModelToRadPreProcessJob::outOfDateImpl(const boost::optional<QDateTime> &t_lastrun) const
  {
    if (!t_lastrun)
    {
      return true;
    } else {
      QReadLocker l(&m_mutex);
      if (m_osm){
        return filesChanged(m_files, *t_lastrun);
      } else {
        return true;
      }
    }
  }

  void ModelToRadPreProcessJob::cleanup()
  {
  }

  std::string ModelToRadPreProcessJob::description() const
  {
    QReadLocker l(&m_mutex);
    return m_description;
  }

  void ModelToRadPreProcessJob::startImpl(const std::shared_ptr<ProcessCreator> &)
  {
    openstudio::path outpath = outdir(true);

    QWriteLocker l(&m_mutex);

    JobErrors errors;
    errors.result = ruleset::OSResultValue::Success;
    
    try {
      m_osm = osmFile();
      resetFiles(m_files, m_osm);
    } catch (const std::exception &e) {
      errors.result = ruleset::OSResultValue::Fail;
      errors.addError(ErrorType::Error, e.what());
    }

    LOG(Info, "ModelToRadPreProcess starting, loading model file: " << toString(m_osm->fullPath));
    openstudio::model::OptionalModel model = openstudio::model::Model::load(m_osm->fullPath);

    if (!model)
    {
      errors.result = ruleset::OSResultValue::Fail;
      errors.addError(ErrorType::Error, "Unable to load model file");
    }
          
    LOG(Info, "ModelToRadPreProcess starting, outdir: " << toString(outpath));

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
      boost::filesystem::create_directories(outpath);

      openstudio::path path = outpath / openstudio::toPath("out.osm");
  
      openstudio::model::Model outmodel; 
      outmodel.purgeUnusedResourceObjects();
      outmodel.getUniqueModelObject<openstudio::model::Building>(); // implicitly create building object
      outmodel.getUniqueModelObject<openstudio::model::Timestep>(); // implicitly create timestep object
      outmodel.getUniqueModelObject<openstudio::model::RunPeriod>(); // implicitly create runperiod object
  
      if (model->getOptionalUniqueModelObject<openstudio::model::WeatherFile>())
      {
        outmodel.addObject(model->getUniqueModelObject<openstudio::model::WeatherFile>());
      }

      std::map<std::string, openstudio::model::ThermalZone> thermalZones;

      std::vector<openstudio::model::Space> spaces = model->getConcreteModelObjects<openstudio::model::Space>();
      for (auto & space : spaces)
      {
        space.hardApplyConstructions();
        space.hardApplySpaceType(true);
        space.hardApplySpaceLoadSchedules();

        // make all surfaces with surface boundary condition adiabatic
        std::vector<openstudio::model::Surface> surfaces = space.surfaces();
        for (auto & surf_it : surfaces){
          boost::optional<openstudio::model::Surface> adjacentSurface = surf_it.adjacentSurface();
          if (adjacentSurface){

            // make sure to hard apply constructions in other space before messing with surface in other space
            boost::optional<openstudio::model::Space> adjacentSpace = adjacentSurface->space();
            if (adjacentSpace){
              adjacentSpace->hardApplyConstructions();
            }

            // resets both surfaces
            surf_it.resetAdjacentSurface();

            // set both to adiabatic
            surf_it.setOutsideBoundaryCondition("Adiabatic");
            adjacentSurface->setOutsideBoundaryCondition("Adiabatic");

            // remove interior windows
            for (openstudio::model::SubSurface subSurface : surf_it.subSurfaces()){
              subSurface.remove();
            }
            for (openstudio::model::SubSurface subSurface : adjacentSurface->subSurfaces()){
              subSurface.remove();
            }
          }
        }

        openstudio::model::Space new_space = space.clone(outmodel).optionalCast<openstudio::model::Space>().get();

        boost::optional<openstudio::model::ThermalZone> thermalZone = space.thermalZone();

        if (thermalZone && thermalZone->name())
        {
          if (thermalZones.find(*thermalZone->name()) == thermalZones.end())
          {
            openstudio::model::ThermalZone newThermalZone(outmodel);
            newThermalZone.setName(*thermalZone->name());
            newThermalZone.setUseIdealAirLoads(true);
            thermalZones.insert(std::make_pair(*thermalZone->name(), newThermalZone));
          }

          auto itr = thermalZones.find(*thermalZone->name());
          OS_ASSERT(itr != thermalZones.end()); // We just added it above if we needed it
          new_space.setThermalZone(itr->second);
        } else if (thermalZone && !thermalZone->name()) {
          errors.addError(ErrorType::Warning, "Space discovered in un-named thermalZone, not translating");
        }
      }
 
      std::vector<openstudio::model::ShadingSurfaceGroup> shadingsurfacegroups = outmodel.getConcreteModelObjects<openstudio::model::ShadingSurfaceGroup>(); 
      for (auto & shadingSurfaceGroup : shadingsurfacegroups)
      {
        shadingSurfaceGroup.remove();
      }
  
      std::vector<openstudio::model::SpaceItem> spaceitems = outmodel.getModelObjects<openstudio::model::SpaceItem>(); 
      for (auto & spaceItem : spaceitems)
      {
        if (spaceItem.optionalCast<openstudio::model::People>()){
          // keep people
        }else if (spaceItem.optionalCast<openstudio::model::Lights>()){
          // keep lights
        }else if (spaceItem.optionalCast<openstudio::model::Luminaire>()){
          // keep luminaires
        }else{
          spaceItem.remove();
        }
      }

      std::vector<openstudio::model::OutputVariable> outputVariables = outmodel.getConcreteModelObjects<openstudio::model::OutputVariable>();
      for (auto & outputVariable : outputVariables)
      {
        outputVariable.remove();
      }

      openstudio::model::OutputVariable outputVariable("Site Exterior Horizontal Sky Illuminance", outmodel);
      outputVariable.setReportingFrequency("Hourly");

      outputVariable = openstudio::model::OutputVariable("Site Exterior Beam Normal Illuminance", outmodel);
      outputVariable.setReportingFrequency("Hourly");

      outputVariable = openstudio::model::OutputVariable("Site Solar Altitude Angle", outmodel);
      outputVariable.setReportingFrequency("Hourly");

      outputVariable = openstudio::model::OutputVariable("Site Solar Azimuth Angle", outmodel);
      outputVariable.setReportingFrequency("Hourly");

      outputVariable = openstudio::model::OutputVariable("Site Sky Diffuse Solar Radiation Luminous Efficacy", outmodel);
      outputVariable.setReportingFrequency("Hourly");

      outputVariable = openstudio::model::OutputVariable("Site Beam Solar Radiation Luminous Efficacy", outmodel);
      outputVariable.setReportingFrequency("Hourly");

      outputVariable = openstudio::model::OutputVariable("Zone People Occupant Count", outmodel);
      outputVariable.setReportingFrequency("Hourly");

      outputVariable = openstudio::model::OutputVariable("Zone Lights Electric Power", outmodel);
      outputVariable.setReportingFrequency("Hourly");

      // only report weather periods
      openstudio::model::SimulationControl simulation_control = outmodel.getUniqueModelObject<openstudio::model::SimulationControl>();
      simulation_control.setRunSimulationforSizingPeriods(false);
      simulation_control.setRunSimulationforWeatherFileRunPeriods(true);
      simulation_control.setSolarDistribution("MinimalShadowing");

      // purge unused 
      outmodel.purgeUnusedResourceObjects();

      outmodel.save(path, true);

    } catch (const std::exception &e) {
      LOG(Error, "Error with ModelToRadPreProcessJob: " + std::string(e.what()));
      errors.addError(ErrorType::Error, "Error with ModelToRadPreProcessJob: " + std::string(e.what()));
      errors.result = ruleset::OSResultValue::Fail;
    }

    // Change this to whatever output files you generate
    emitOutputFileChanged(RunManager_Util::dirFile(outpath / openstudio::toPath("out.osm")));
    setErrors(errors);
  }

  std::string ModelToRadPreProcessJob::getOutput() const
  {
    return "";
  }

  void ModelToRadPreProcessJob::basePathChanged()
  {
    m_osm.reset();
    resetFiles(m_files);
  }

  FileInfo ModelToRadPreProcessJob::osmFile() const
  {
    if (!m_osm)
    {
      return allInputFiles().getLastByExtension("osm");
    } else {
      return *m_osm;
    }
  }

  Files ModelToRadPreProcessJob::outputFilesImpl() const
  {
    openstudio::path outpath = outdir();
    if (!boost::filesystem::exists(outpath / toPath("out.osm")) || !m_osm)
    {
      // no output file has been generated yet
      return Files();
    }

    Files f;
    FileInfo fi(outpath / toPath("out.osm"), "osm");
    fi.requiredFiles = m_osm->requiredFiles;

    if (!fi.hasRequiredFile(openstudio::toPath("in.epw")))
    {
      /// \todo we need better handling of OSM files and their attachments
      // epw wasn't found, look for parent one
      openstudio::path possibleepw = m_osm->fullPath.parent_path() / openstudio::toPath("in.epw");

      if (boost::filesystem::exists(possibleepw))
      {
        LOG(Info, "Fixing up EPW file for incoming OSM attachment to " << openstudio::toString(possibleepw));
        fi.addRequiredFile(possibleepw, openstudio::toPath("in.epw"));
      }
    }

    f.append(fi);
    return f;
  }


} // detail
} // runmanager
} // openstudio



