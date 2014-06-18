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

#include "JobFactory.hpp"
#include "EnergyPlusJob.hpp"
#include "ModelToIdfJob.hpp"
#include "IdfToModelJob.hpp"
#include "CalculateEconomicsJob.hpp"
#include "ExpandObjectsJob.hpp"
#include "PreviewIESJob.hpp"
#include "ReadVarsJob.hpp"
#include "XMLPreprocessorJob.hpp"
#include "RubyJob.hpp"
#include "RubyJobUtils.hpp"
#include "NullJob.hpp"
#include "NormalizeURLs.hpp"
#include "MergeJobError.hpp"
#include "EnergyPlusPostProcessJob.hpp"
#include "EnergyPlusPreProcessJob.hpp"
#include "OpenStudioPostProcessJob.hpp"
#include "ParallelEnergyPlusSplitJob.hpp"
#include "ParallelEnergyPlusJoinJob.hpp"
#include "BasementJob.hpp"
#include "SlabJob.hpp"
#include "ModelToRadJob.hpp"
#include "ModelToRadPreProcessJob.hpp"
#include "DakotaJob.hpp"
#include "UserScriptJob.hpp"
#include "Job_Impl.hpp"

namespace openstudio {
namespace runmanager {

  Job JobFactory::createJob(
      openstudio::runmanager::JobType t_type,
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    switch(t_type.value())
    {
      case JobType::EnergyPlus:
        return createEnergyPlusJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      case JobType::ExpandObjects:
        return createExpandObjectsJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      case JobType::Basement:
        return createBasementJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      case JobType::Slab:
        return createSlabJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      case JobType::XMLPreprocessor:
        return createXMLPreprocessorJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      case JobType::ModelToIdf:
        return createModelToIdfJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      case JobType::ModelToRad:
        return createModelToRadJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      case JobType::IdfToModel:
        return createIdfToModelJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      case JobType::CalculateEconomics:
        return createCalculateEconomicsJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      case JobType::ReadVars:
        return createReadVarsJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      case JobType::PreviewIES:
        return createPreviewIESJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      case JobType::Ruby:
        return createRubyJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      case JobType::UserScript:
        return createUserScriptJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      case JobType::Null:
        return createNullJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      case JobType::EnergyPlusPostProcess:
        return createEnergyPlusPostProcessJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      case JobType::OpenStudioPostProcess:
        return createOpenStudioPostProcessJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      case JobType::EnergyPlusPreProcess:
        return createEnergyPlusPreProcessJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      case JobType::ModelToRadPreProcess:
        return createModelToRadPreProcessJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      case JobType::ParallelEnergyPlusJoin:
        return createParallelEnergyPlusJoinJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      case JobType::ParallelEnergyPlusSplit:
        return createParallelEnergyPlusSplitJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      case JobType::Dakota:
        return createDakotaJob(t_tools, t_params, t_files, t_url_search_paths, t_loading, *t_uuid, t_state);
      default:
        throw std::domain_error("Unknown domain value to createJob");
    };
  }


  Job JobFactory::createModelToIdfJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::ModelToIdfJob(*t_uuid, t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  Job JobFactory::createModelToIdfJob(
      const openstudio::path &t_model,
      const openstudio::path &t_outdir,
      const boost::optional<openstudio::UUID> &t_uuid)
  {
    JobParams params;
    params.append("outdir", toString(t_outdir));

    Files files;
    files.append(FileInfo(t_model, "osm"));

    return createModelToIdfJob(Tools(), params, files, std::vector<openstudio::URLSearchPath>(), t_uuid);
  }

  Job JobFactory::createModelToRadJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::ModelToRadJob(*t_uuid, t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  Job JobFactory::createModelToRadJob(
      const openstudio::path &t_model,
      const openstudio::path &t_sql,
      const openstudio::path &t_outdir,
      const boost::optional<openstudio::UUID> &t_uuid)
  {
    JobParams params;
    params.append("outdir", toString(t_outdir));

    Files files;
    files.append(FileInfo(t_model, "osm"));
    files.append(FileInfo(t_sql, "sql"));

    return createModelToRadJob(Tools(), params, files, std::vector<openstudio::URLSearchPath>(), t_uuid);
  }

  Job JobFactory::createIdfToModelJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::IdfToModelJob(*t_uuid, t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  Job JobFactory::createIdfToModelJob(
      const openstudio::path &t_idf,
      const openstudio::path &t_outdir,
      const boost::optional<openstudio::UUID> &t_uuid)
  {
    JobParams params;
    params.append("outdir", toString(t_outdir));

    Files files;
    files.append(FileInfo(t_idf, "idf"));

    return createIdfToModelJob(Tools(), params, files, std::vector<openstudio::URLSearchPath>(), t_uuid);
  }


  Job JobFactory::createCalculateEconomicsJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::CalculateEconomicsJob(*t_uuid, t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  // Creates a job that handles processing of EnergyPlus job outputs
  Job JobFactory::createEnergyPlusPostProcessJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::EnergyPlusPostProcessJob(*t_uuid, t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  // Creates a job that handles processing of EnergyPlus job outputs from an OSM file
  Job JobFactory::createOpenStudioPostProcessJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::OpenStudioPostProcessJob(*t_uuid, t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  // Creates a job that handles prepping an input file by enabling sql output
  Job JobFactory::createEnergyPlusPreProcessJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::EnergyPlusPreProcessJob(*t_uuid, t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  Job JobFactory::createModelToRadPreProcessJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::ModelToRadPreProcessJob(*t_uuid, t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  Job JobFactory::createEnergyPlusJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::EnergyPlusJob(*t_uuid,
            t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  Job JobFactory::createEnergyPlusJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::path &t_idd,
      const openstudio::path &t_idf,
      const openstudio::path &t_epw,
      const openstudio::path &t_outdir,
      const boost::optional<openstudio::UUID> &t_uuid)
  {
    JobParams params;
    params.append("outdir", toString(t_outdir));

    Files files;
    FileInfo idf(t_idf, "idf");

    if (!t_idd.empty())
    {
      idf.addRequiredFile(t_idd,toPath("Energy+.idd"));
    }

    if (boost::filesystem::is_directory(t_epw))
    {
      params.append("epwdir", toString(t_epw));
    } else {
      idf.addRequiredFile(t_epw, toPath("in.epw"));
    }

    files.append(idf);

    return createEnergyPlusJob(t_tools, params, files, std::vector<openstudio::URLSearchPath>(), t_uuid);
  }

  Job JobFactory::createEnergyPlusJob(
      ToolInfo t_energyPlusTool,
      const openstudio::path &t_idd,
      const openstudio::path &t_idf,
      const openstudio::path &t_epw,
      const openstudio::path &t_outdir,
      const boost::optional<openstudio::UUID> &t_uuid)
  {
    JobParams params;
    params.append("outdir", toString(t_outdir));

    Tools tools;
    t_energyPlusTool.name = "energyplus";
    tools.append(t_energyPlusTool);

    Files files;
    FileInfo idf(t_idf, "idf");

    if (!t_idd.empty())
    {
      idf.addRequiredFile(t_idd,toPath("Energy+.idd"));
    }

    idf.addRequiredFile(t_epw, toPath("in.epw"));
    files.append(idf);

    return createEnergyPlusJob(tools, params, files, std::vector<openstudio::URLSearchPath>(), t_uuid);
  }

  Job JobFactory::createExpandObjectsJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::ExpandObjectsJob(*t_uuid,
            t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  Job JobFactory::createXMLPreprocessorJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::XMLPreprocessorJob(*t_uuid,
            t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  Job JobFactory::createPreviewIESJob(
      const openstudio::path &t_ies,
      const openstudio::path &t_outdir,
      const boost::optional<openstudio::UUID> &t_uuid)
  {
    JobParams params;
    params.append("outdir", toString(t_outdir));

    Files files;
    files.append(FileInfo(t_ies, "ies"));

    return createPreviewIESJob(Tools(), params, files, std::vector<openstudio::URLSearchPath>(), t_uuid);

  }

  Job JobFactory::createPreviewIESJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::PreviewIESJob(*t_uuid,
            t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  Job JobFactory::createRubyJob(
      const openstudio::path &t_rubyfile,
      const std::string &t_infileregex,
      const openstudio::path &t_outdir,
      const boost::optional<openstudio::UUID> &t_uuid)
  {
    JobParams params;
    params.append("outdir", toString(t_outdir));
    params.append("input_file_regex", t_infileregex);

    Files files;
    files.append(FileInfo(t_rubyfile, "rb"));


    return createRubyJob(Tools(), params, files, std::vector<openstudio::URLSearchPath>(), t_uuid);
  }

  Job JobFactory::createRubyJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::RubyJob(*t_uuid,
            t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  Job JobFactory::createUserScriptJob (
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::UserScriptJob(*t_uuid,
            t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  Job JobFactory::createUserScriptJob(
      const openstudio::path &t_rubyfile,
      const std::vector<ruleset::UserScriptArgument> &t_args,
      const boost::optional<openstudio::UUID> &t_uuid)
  {
    RubyJobBuilder rjb = runmanager::RubyJobBuilder::createUserScriptRubyJob(t_rubyfile, t_args);
    Workflow wf;
    rjb.addToWorkflow(wf);
    return wf.create();
  }

  Job JobFactory::createRubyJob(
      const openstudio::runmanager::RubyJobBuilder &t_builder,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid)
  {
    Files files;
    openstudio::path scriptPath = t_builder.script();
    if (!scriptPath.empty())
    {
      openstudio::runmanager::FileInfo fi(scriptPath,"rb");
      for (const PathPair& reqFile : t_builder.requiredFiles()) {
        fi.addRequiredFile(reqFile.first,reqFile.second);
      }
      files.append(fi);
    }
    return Job(std::shared_ptr<detail::Job_Impl>(
          new detail::RubyJob(*t_uuid,Tools(),t_builder.toParams(),
            normalizeURLs(files,t_url_search_paths,t_loading,*t_uuid), JobState())));
  }

  Job JobFactory::createNullJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::NullJob(*t_uuid,
            t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  Job JobFactory::createNullJob(
      const boost::optional<openstudio::UUID> &t_uuid)
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::NullJob(*t_uuid,
            Tools(), JobParams(), Files(), JobState())));
  }

  Job JobFactory::createReadVarsJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::ReadVarsJob(*t_uuid,
            t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  Job JobFactory::createParallelEnergyPlusSplitJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::ParallelEnergyPlusSplitJob(*t_uuid,
            t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  Job JobFactory::createParallelEnergyPlusJoinJob(int t_numSplits, int t_offset)
  {
    JobParams p;
    p.append("numsplits", boost::lexical_cast<std::string>(t_numSplits));
    p.append("offset", boost::lexical_cast<std::string>(t_offset));

    return Job(std::shared_ptr<detail::Job_Impl>(new detail::ParallelEnergyPlusJoinJob(createUUID(),
            Tools(), p, Files(), JobState())));
  }

  Job JobFactory::createParallelEnergyPlusSplitJob(int t_numSplits, int t_offset)
  {
    JobParams p;
    p.append("numsplits", boost::lexical_cast<std::string>(t_numSplits));
    p.append("offset", boost::lexical_cast<std::string>(t_offset));

    return Job(std::shared_ptr<detail::Job_Impl>(new detail::ParallelEnergyPlusSplitJob(createUUID(),
            Tools(), p, Files(), JobState())));
  }

  Job JobFactory::createEnergyPlusJob(const std::string &t_filename)
  {
    JobParams p;
    p.append("filename", t_filename);
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::EnergyPlusJob(createUUID(), Tools(), p, Files(), JobState())));
  }

  Job JobFactory::createParallelEnergyPlusJobTree(
      int t_numSplits, int t_offset)
  {
    Job j = createParallelEnergyPlusSplitJob(t_numSplits, t_offset);

    for (int i = 0; i < t_numSplits; ++i)
    {
      // Add a child EnergyPlus job for each split, telling it to look up the expected output file
      // that will be created by the split job
      j.addChild(createEnergyPlusJob("out" + boost::lexical_cast<std::string>(i) + ".idf"));
    }

    j.setFinishedJob(createParallelEnergyPlusJoinJob(t_numSplits, t_offset));

    return j;
  }

  Job JobFactory::createParallelEnergyPlusJoinJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::ParallelEnergyPlusJoinJob(*t_uuid,
            t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  Job JobFactory::createBasementJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::BasementJob(*t_uuid,
            t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  Job JobFactory::createSlabJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::SlabJob(*t_uuid,
            t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  Job JobFactory::createDakotaJob(
      const openstudio::runmanager::Tools &t_tools,
      const openstudio::runmanager::JobParams &t_params,
      const openstudio::runmanager::Files &t_files,
      const std::vector<openstudio::URLSearchPath> &t_url_search_paths,
      bool t_loading,
      const boost::optional<openstudio::UUID> &t_uuid,
      const JobState &t_state
      )
  {
    return Job(std::shared_ptr<detail::Job_Impl>(new detail::DakotaJob(*t_uuid,
            t_tools, t_params, normalizeURLs(t_files, t_url_search_paths, t_loading, *t_uuid), t_state)));
  }

  Files JobFactory::normalizeURLs(
      const openstudio::runmanager::Files &t_files, 
      const std::vector<openstudio::URLSearchPath> &t_search_paths, 
      bool t_loading, 
      const openstudio::UUID &t_uuid)
  {
    if (t_loading) { return t_files; } // nothing to do, we are loading from disk

    std::vector<FileInfo> files = t_files.files();

    Files retval;

    NormalizeURLs normalizer;

    for (std::vector<FileInfo>::const_iterator itr = files.begin(); itr != files.end(); ++itr)
    {
      retval.append(*itr); // Keep the original version no matter what
      FileInfo normalized = normalizer.normalize(*itr, t_search_paths, t_uuid);
      if (normalized != *itr)
      {
        retval.append(normalized); // Add the normalized version if it is different
      }
    }

    return retval;
  } 

  void JobFactory::optimizeJobTree(Job t_job)
  {
    while (optimizeJobTreeImpl(t_job))
    {
      // run as long as it takes to mean no more changes to the tree
    }
  }

  bool JobFactory::optimizeJobTreeImpl(Job t_job)
  {
    bool didsomething = false;

    std::vector<Job> children = t_job.children();
    boost::optional<Job> finished = t_job.finishedJob();

    // Optimize all children
    std::for_each(children.begin(), children.end(), std::function<void (Job)>(&JobFactory::optimizeJobTree));
    if (finished)
    {
      optimizeJobTree(*finished);
    }

    // Then try to merge them with the parent
    for (const auto & child : children)
    {
      try {
        t_job.mergeJob(child);
        didsomething = true;
      } catch (const openstudio::runmanager::MergeJobError &) {
        // continue anyhow
      }
    }

    if (finished)
    {
      try {
        t_job.mergeJob(*finished);
        didsomething = true;
      } catch (const openstudio::runmanager::MergeJobError &) {
        // continue anyhow
      }
    }

    return didsomething;
  }

}
}

