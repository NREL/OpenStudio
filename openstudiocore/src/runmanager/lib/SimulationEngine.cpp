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

#include "SimulationEngine.hpp"
#include "Job.hpp"
#include "Workflow.hpp"
#include "WorkItem.hpp"
#include <boost/filesystem.hpp>
#include <QCryptographicHash>
#include "../../utilities/core/ApplicationPathHelpers.hpp"

namespace openstudio {
  namespace runmanager {

    SimulationEngine::SimulationEngine(const openstudio::path &t_cacheFolder, size_t t_numVariables)
      : m_numVariables(t_numVariables), m_folder(validateFolder(t_cacheFolder)), 
        m_runManager(m_folder / openstudio::toPath("run.db"), false, true)
    {
      LOG(Info, "Starting SimulationEngine: " << openstudio::toString(t_cacheFolder));
      openstudio::runmanager::ConfigOptions co = m_runManager.getConfigOptions();
      co.findTools(true, true, true, true);

      m_runManager.setConfigOptions(co);

      std::vector<openstudio::runmanager::Job> jobs = m_runManager.getJobs();


      LOG(Info, "Scanning jobs from RunManager: " << jobs.size());
      for (const auto & job : jobs)
      {
        if (!job.parent())
        {
          LOG(Info, "Connecting top level job: " << job.description());
          connectSignals(job);

          if (job.treeStatus() == TreeStatusEnum::Finished)
          {
            LOG(Info, "Loading results from job tree: " << openstudio::toString(job.uuid()));
            loadResults(job);
          } else {
            LOG(Info, "Tree status was not finished, was: " << job.treeStatus().valueName());
          }

          // register that we are keeping track of this simulation
          std::vector<double> variables = getVariables(job);
          std::vector<int> discreteVariables = getDiscreteVariables(job);
          LOG(Info, "Registering loaded simulation " << toString(variables) << " " << toString(discreteVariables));
          m_simulations.insert(std::make_pair(variables, discreteVariables));
        }
      }
   

      LOG(Info, "Unpausing runmanager");
      m_runManager.setPaused(false);
    }

    openstudio::path SimulationEngine::validateFolder(const openstudio::path &t_folder)
    {
      if (boost::filesystem::exists(t_folder))
      {
        if (!boost::filesystem::is_directory(t_folder))
        {
          throw std::runtime_error("cache foler exists and is not a folder");
        } else {
          return t_folder;
        }
      } else {
        boost::filesystem::create_directories(t_folder);
        return t_folder;
      }
    }

    std::vector<double> SimulationEngine::getVariables(const openstudio::runmanager::Job &t_job) 
    {
      return fromString<double>(t_job.jobParams().get("simulationEngineVariables").children.at(0).value); 
    }

    std::vector<int> SimulationEngine::getDiscreteVariables(const openstudio::runmanager::Job &t_job) 
    {
      return fromString<int>(t_job.jobParams().get("simulationEngineDiscreteVariables").children.at(0).value); 
    }

    FuelUses SimulationEngine::fuelUses(const openstudio::model::Model &t_model, const std::vector<double> &t_variables,
        const std::vector<int> &t_discreteVariables,
        const std::string &t_simulationId,
        const openstudio::path &t_weatherFile)
    {
      validateNumVariables(t_variables);

      std::pair<std::vector<double>, std::vector<int> > key(t_variables, t_discreteVariables);

      if (m_simulations.count(key) == 0)
      {
        LOG(Info, "Simulation did not exist, adding " << toString(t_variables) << " " << toString(t_discreteVariables));
        enqueueSimulations(t_model, t_variables, t_discreteVariables, t_simulationId, t_weatherFile);
        m_simulations.insert(key);
      } else {
        LOG(Info, "Simulation already existed " << toString(t_variables) << " " << toString(t_discreteVariables));
      }

      return fuelUses(t_variables, t_discreteVariables);
    }

    void SimulationEngine::enqueueSimulations(const openstudio::model::Model &t_model, const std::vector<double> &t_variables,
        const std::vector<int> &t_discreteVariables,
        const std::string &t_simulationId,
        const openstudio::path &t_weatherFile)
    {
      validateNumVariables(t_variables);

      QCryptographicHash hash(QCryptographicHash::Sha1);

      hash.addData(reinterpret_cast<const char *>(&t_variables.front()), (t_variables.size() * sizeof(double)) / sizeof(char));
      hash.addData(reinterpret_cast<const char *>(&t_discreteVariables.front()), (t_discreteVariables.size() * sizeof(int)) / sizeof(char));

      // We are using a hash because this list of variables could get very long indeed
      const std::string hashstring(hash.result().toHex().data());

      openstudio::path basePath = m_folder / openstudio::toPath(hashstring);

      if (!t_simulationId.empty())
      {
        m_simulationIds[t_simulationId] = std::make_pair(t_variables, t_discreteVariables);
      }

      // estimation
      openstudio::runmanager::Job estimation = createEstimationJob(basePath, t_model, t_variables, t_discreteVariables, t_simulationId, t_weatherFile);
      connectSignals(estimation);

      // Full run
      openstudio::runmanager::Job fullrun = createFullJob(basePath, t_model, t_variables, t_discreteVariables, t_simulationId, t_weatherFile);
      connectSignals(fullrun);

      // Radiance run
      openstudio::runmanager::Job radianceRun = createRadianceJob(basePath, t_model, t_variables, t_discreteVariables, t_simulationId, t_weatherFile);
      connectSignals(radianceRun);

      m_runManager.enqueue(estimation, true);
      m_runManager.enqueue(fullrun, true);
      m_runManager.enqueue(radianceRun, true);
    }

    void SimulationEngine::connectSignals(const openstudio::runmanager::Job &t_job)
    {
      t_job.connect(SIGNAL(treeChanged(const openstudio::UUID &)), this, SLOT(jobTreeChanged(const openstudio::UUID &)));
    }

    void SimulationEngine::jobTreeChanged(const openstudio::UUID &t_uuid)
    {
      openstudio::runmanager::Job j = m_runManager.getJob(t_uuid);

      while (j.parent())
      {
        j = *j.parent();
      }

      LOG(Debug, "TreeStatus " << j.treeStatus() << " " << j.parent() << " " << openstudio::toString(j.uuid()));

      if (j.treeStatus() == TreeStatusEnum::Finished)
      {
        try {
          loadResults(j);
        } catch (const std::exception &e) {
          LOG(Error, "Error loading job results: " << e.what());
        }
      }
    }

    void SimulationEngine::loadResults(const openstudio::runmanager::Job &t_job)
    {
      std::string runtype = t_job.jobParams().get("simulationEngineRunType").children.at(0).value; 

      std::vector<double> variables = getVariables(t_job);
      std::vector<int> discreteVariables = getDiscreteVariables(t_job);

      try {
        JobParams params = t_job.jobParams();
        if (params.has("simulationEngineSimulationId"))
        {
          std::string simulationId = params.get("simulationEngineSimulationId").children.at(0).value;
          m_simulationIds[simulationId] = std::make_pair(variables, discreteVariables);
        }
      } catch (...) { /* no simulation id */ }

      openstudio::path sqlfilePath = getSqlFilePath(t_job);
      SqlFile sql(sqlfilePath); 


      getErrorEstimation(discreteVariables).add(sql, runtype, variables); 

      double confidence = getErrorEstimation(discreteVariables).getConfidence(runtype);


      if (m_details[std::make_pair(variables, discreteVariables)].confidence < confidence)
      {
        m_details[std::make_pair(variables, discreteVariables)] = SimulationDetails(confidence, sqlfilePath);
      }

    }

    openstudio::runmanager::Job SimulationEngine::createEstimationJob(const openstudio::path &t_path, const openstudio::model::Model &t_model,
        const std::vector<double> &t_variables, const std::vector<int> &t_discreteVariables, 
        const std::string &t_simulationId,
        const openstudio::path &t_weatherFile)
    {
      return createJob(t_path / openstudio::toPath(estimationJobString()), t_model,
          t_variables, t_discreteVariables, t_simulationId, estimationJobString(), true, m_runManager.getConfigOptions().getMaxLocalJobs(),
          false, t_weatherFile);
    }

    openstudio::runmanager::Job SimulationEngine::createFullJob(const openstudio::path &t_path, const openstudio::model::Model &t_model,
        const std::vector<double> &t_variables, const std::vector<int> &t_discreteVariables, 
        const std::string &t_simulationId,
        const openstudio::path &t_weatherFile)
    {
      return createJob(t_path / openstudio::toPath(fullJobString()), t_model,
          t_variables, t_discreteVariables, t_simulationId, fullJobString(), false, m_runManager.getConfigOptions().getMaxLocalJobs(),
          false, t_weatherFile);
    }

    openstudio::runmanager::Job SimulationEngine::createRadianceJob(const openstudio::path &t_path, const openstudio::model::Model &t_model,
        const std::vector<double> &t_variables, const std::vector<int> &t_discreteVariables,
        const std::string &t_simulationId,
        const openstudio::path &t_weatherFile)
    {
      return createJob(t_path / openstudio::toPath(radianceJobString()), t_model,
          t_variables, t_discreteVariables, t_simulationId, radianceJobString(), false, m_runManager.getConfigOptions().getMaxLocalJobs(),
          true, t_weatherFile);
    }

    std::string SimulationEngine::radianceJobString()
    {
      return "RadianceSimulation";
    }

    std::string SimulationEngine::fullJobString()
    {
      return "FullSimulation";
    }

    std::string SimulationEngine::estimationJobString()
    {
      return "EstimationSimulation";
    }

    openstudio::path SimulationEngine::getSqlFilePath(const openstudio::runmanager::Job &t_job)
    {
      if (t_job.treeStatus() != TreeStatusEnum::Finished)
      {
        throw std::runtime_error("cannot get file path, job has not finished");
      }

      return t_job.treeOutputFiles().getLastByFilename("eplusout.sql").fullPath;
    }

    template<typename T>
    std::string SimulationEngine::toString(const std::vector<T> &t_variables)
    {
      std::stringstream ss;
      for(const auto & elem : t_variables)
      {
        ss << elem << ",";
      }
      return ss.str();
    }

    template<typename T>
    std::vector<T> SimulationEngine::fromString(const std::string &t_variableString)
    {
      std::stringstream ss(t_variableString);

      std::vector<T> results;
      while(ss.good())
      {
        T val;
        ss >> val;
        char comma;
        ss >> comma;

        if (ss.good()) 
        {
          // data was read properly
          results.push_back(val);
        }
      }

      return results;
    }

    openstudio::runmanager::Job SimulationEngine::createJob(const openstudio::path &t_path, openstudio::model::Model t_model,
        const std::vector<double> &t_variables, const std::vector<int> &t_discreteVariables, const std::string &t_simulationId,
        const std::string &t_runType,
        bool t_estimation, int t_numParallel, bool t_radiance, const openstudio::path &t_weatherFile)
    {
      openstudio::runmanager::ConfigOptions co = m_runManager.getConfigOptions();

      openstudio::runmanager::Workflow workflow;

      workflow.addJob(openstudio::runmanager::JobType::ModelToIdf);
      workflow.addJob(openstudio::runmanager::JobType::ExpandObjects);
      if (t_radiance)
      {
        std::vector<openstudio::runmanager::ToolInfo> radiancetools = co.getTools().getAllByName("rad").tools();

        workflow.addJob(runmanager::Workflow::radianceDaylightCalculations(openstudio::getOpenStudioRubyIncludePath(), 
            radiancetools.at(radiancetools.size()-1).localBinPath.parent_path()));
      }

      //workflow.addJob(new openstudio.runmanager.JobType("EnergyPlusPreProcess"));
      workflow.addJob(openstudio::runmanager::JobType::EnergyPlus);

      JobParams params;
      if (!t_simulationId.empty())
      {
        params.append("simulationEngineSimulationId", t_simulationId);
      }
      params.append("simulationEngineRunType", t_runType);
      params.append("simulationEngineVariables", toString(t_variables));
      params.append("simulationEngineDiscreteVariables", toString(t_discreteVariables));

      workflow.add(params);

      workflow.add(co.getTools());

      LOG(Debug, "Running simulation in " << openstudio::toString(t_path));
      boost::filesystem::create_directories(t_path);

      openstudio::path osmpath = t_path / openstudio::toPath("model.osm");
      LOG(Debug, "Saving OSM to " << openstudio::toString(osmpath));

      if (t_estimation)
      {
        t_model = openstudio::model::Model(t_model.clone());
      }

      t_model.save(osmpath, true);

      if (t_numParallel > 1)
      {
        workflow.parallelizeEnergyPlus(t_numParallel, 7);
      }

      return workflow.create(t_path, osmpath, t_weatherFile);
    }
    
    FuelUses SimulationEngine::fuelUses(const std::vector<double> &t_variables, const std::vector<int> &t_discreteVariables) 
    {
      validateNumVariables(t_variables);

      return getErrorEstimation(t_discreteVariables).approximate(t_variables);
    }

    FuelUses SimulationEngine::fuelUses(const std::vector<double> &t_variables, const std::vector<int> &t_discreteVariables) const
    {
      validateNumVariables(t_variables);

      return getErrorEstimation(t_discreteVariables).approximate(t_variables);
    }

    std::pair<std::vector<double>, std::vector<int> > SimulationEngine::lookupSimulationId(const std::string &t_simulationId) const
    {
      auto itr = m_simulationIds.find(t_simulationId);

      if (itr != m_simulationIds.end())
      {
        return itr->second;
      } else {
        throw std::runtime_error("Unknown simulationId: '" + t_simulationId + "'");
      }
    }

    FuelUses SimulationEngine::fuelUses(const std::string &t_simulationId) const
    {
      std::pair<std::vector<double>, std::vector<int> > p = lookupSimulationId(t_simulationId);
      return fuelUses(p.first, p.second);
    }

    ErrorEstimation &SimulationEngine::getErrorEstimation(const std::vector<int> &t_discreteVariables)
    {
      auto itr = m_errorEstimations.find(t_discreteVariables);
      if (itr != m_errorEstimations.end())
      {
        return itr->second;
      } else {
        ErrorEstimation ee(m_numVariables);
        ee.setConfidence(estimationJobString(), .75);
        ee.setConfidence(fullJobString(), .90);
        ee.setConfidence(radianceJobString(), 1.0);
        return m_errorEstimations.insert(std::map<std::vector<int>, ErrorEstimation>::value_type(t_discreteVariables, ee)).first->second;
      }
    }

    const ErrorEstimation &SimulationEngine::getErrorEstimation(const std::vector<int> &t_discreteVariables) const
    {
      auto itr = m_errorEstimations.find(t_discreteVariables);
      if (itr != m_errorEstimations.end())
      {
        return itr->second;
      } else {
        throw std::range_error("Unknown set of discrete variables");
      }
    }

    SimulationDetails SimulationEngine::details(const std::vector<double> &t_variables, const std::vector<int> &t_discreteVariables) const
    {
      validateNumVariables(t_variables);

      auto itr = m_details.find(std::make_pair(t_variables, t_discreteVariables));

      if (itr != m_details.end())
      {
        return itr->second;
      }

      return SimulationDetails();
    }

    SimulationDetails SimulationEngine::details(const std::string &t_simulationId) const
    {
      std::pair<std::vector<double>, std::vector<int> > p = lookupSimulationId(t_simulationId);
      return details(p.first, p.second);
    }


    void SimulationEngine::validateNumVariables(const std::vector<double> &t_variables) const
    {
      if (t_variables.size() != m_numVariables)
      {
        throw std::runtime_error("Incorrect number of variables");
      }
    }


  }
}


