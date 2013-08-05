#include "SimulationEngine.hpp"
#include "Job.hpp"
#include "Workflow.hpp"
#include "WorkItem.hpp"
#include <boost/filesystem.hpp>
#include <QCryptographicHash>
#include <utilities/core/ApplicationPathHelpers.hpp>

namespace openstudio {
  namespace runmanager {

    SimulationEngine::SimulationEngine(const openstudio::path &t_cacheFolder, size_t t_numVariables)
      : m_numVariables(t_numVariables), m_folder(validateFolder(t_cacheFolder)), 
        m_errorEstimations(t_numVariables),
        m_runManager(m_folder / openstudio::toPath("run.db"), false, true)
    {
      LOG(Info, "Starting SimulationEngine: " << openstudio::toString(t_cacheFolder));
      openstudio::runmanager::ConfigOptions co = m_runManager.getConfigOptions();
      co.findTools(true, true, true, true);

      m_runManager.setConfigOptions(co);

      std::vector<openstudio::runmanager::Job> jobs = m_runManager.getJobs();

      m_errorEstimations.setConfidence(estimationJobString(), .75);
      m_errorEstimations.setConfidence(fullJobString(), .90);
      m_errorEstimations.setConfidence(radianceJobString(), 1.0);

      LOG(Info, "Scanning jobs from RunManager: " << jobs.size());
      for (std::vector<openstudio::runmanager::Job>::iterator itr = jobs.begin();
          itr != jobs.end();
          ++itr)
      {
        if (!itr->parent())
        {
          LOG(Info, "Connecting top level job: " << itr->description());
          connectSignals(*itr);

          if (itr->treeStatus() == TreeStatusEnum::Finished)
          {
            LOG(Info, "Loading results from job tree: " << openstudio::toString(itr->uuid()));
            loadResults(*itr);
          } else {
            LOG(Info, "Tree status was not finished, was: " << itr->treeStatus().valueName());
          }

          // register that we are keeping track of this simulation
          std::vector<double> variables = getVariables(*itr);
          LOG(Info, "Registering loaded simulation " << toString(variables));
          m_simulations.insert(variables);
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
      return fromString(t_job.jobParams().get("simulationEngineVariables").children.at(0).value); 
    }

    FuelUses SimulationEngine::fuelUses(const openstudio::model::Model &t_model, const std::vector<double> &t_variables,
        const openstudio::path &t_weatherFile)
    {
      validateNumVariables(t_variables);

      if (m_simulations.count(t_variables) == 0)
      {
        LOG(Info, "Simulation did not exist, adding " << toString(t_variables));
        enqueueSimulations(t_model, t_variables, t_weatherFile);
        m_simulations.insert(t_variables);
      } else {
        LOG(Info, "Simulation already existed " << toString(t_variables));
      }

      return fuelUses(t_variables);
    }

    void SimulationEngine::enqueueSimulations(const openstudio::model::Model &t_model, const std::vector<double> &t_variables,
        const openstudio::path &t_weatherFile)
    {
      validateNumVariables(t_variables);

      QCryptographicHash hash(QCryptographicHash::Sha1);

      hash.addData(reinterpret_cast<const char *>(&t_variables.front()), (t_variables.size() * sizeof(double)) / sizeof(char));

      // We are using a hash because this list of variables could get very long indeed
      const std::string hashstring(hash.result().toHex().data());

      openstudio::path basePath = m_folder / openstudio::toPath(hashstring);

      // estimation
      openstudio::runmanager::Job estimation = createEstimationJob(basePath, t_model, t_variables, t_weatherFile);
      connectSignals(estimation);

      // Full run
      openstudio::runmanager::Job fullrun = createFullJob(basePath, t_model, t_variables, t_weatherFile);
      connectSignals(fullrun);

      // Radiance run
      openstudio::runmanager::Job radianceRun = createRadianceJob(basePath, t_model, t_variables, t_weatherFile);
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

      openstudio::path sqlfilePath = getSqlFilePath(t_job);
      SqlFile sql(sqlfilePath); 

      m_errorEstimations.add(sql, runtype, variables); 

      double confidence = m_errorEstimations.getConfidence(runtype);

      if (m_details[variables].confidence < confidence)
      {
        m_details[variables] = SimulationDetails(confidence, sqlfilePath);
      }
    }

    openstudio::runmanager::Job SimulationEngine::createEstimationJob(const openstudio::path &t_path, const openstudio::model::Model &t_model,
        const std::vector<double> &t_variables, const openstudio::path &t_weatherFile)
    {
      return createJob(t_path / openstudio::toPath(estimationJobString()), t_model,
          t_variables, estimationJobString(), true, m_runManager.getConfigOptions().getMaxLocalJobs(),
          false, t_weatherFile);
    }

    openstudio::runmanager::Job SimulationEngine::createFullJob(const openstudio::path &t_path, const openstudio::model::Model &t_model,
        const std::vector<double> &t_variables, const openstudio::path &t_weatherFile)
    {
      return createJob(t_path / openstudio::toPath(fullJobString()), t_model,
          t_variables, fullJobString(), false, m_runManager.getConfigOptions().getMaxLocalJobs(),
          false, t_weatherFile);
    }

    openstudio::runmanager::Job SimulationEngine::createRadianceJob(const openstudio::path &t_path, const openstudio::model::Model &t_model,
        const std::vector<double> &t_variables, const openstudio::path &t_weatherFile)
    {
      return createJob(t_path / openstudio::toPath(radianceJobString()), t_model,
          t_variables, radianceJobString(), false, m_runManager.getConfigOptions().getMaxLocalJobs(),
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

    std::string SimulationEngine::toString(const std::vector<double> &t_variables)
    {
      std::stringstream ss;
      for(std::vector<double>::const_iterator itr = t_variables.begin();
          itr != t_variables.end();
          ++itr)
      {
        ss << *itr << ",";
      }
      return ss.str();
    }

    std::vector<double> SimulationEngine::fromString(const std::string &t_variableString)
    {
      std::stringstream ss(t_variableString);

      std::vector<double> results;
      while(ss.good())
      {
        double val;
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
        const std::vector<double> &t_variables, const std::string &t_runType,
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
      params.append("simulationEngineRunType", t_runType);
      params.append("simulationEngineVariables", toString(t_variables));
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

    FuelUses SimulationEngine::fuelUses(const std::vector<double> &t_variables) const
    {
      validateNumVariables(t_variables);

      return m_errorEstimations.approximate(t_variables);
    }

    SimulationDetails SimulationEngine::details(const std::vector<double> &t_variables) const
    {
      validateNumVariables(t_variables);

      std::map<std::vector<double>, SimulationDetails>::const_iterator itr = m_details.find(t_variables);

      if (itr != m_details.end())
      {
        return itr->second;
      }

      return SimulationDetails();
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


