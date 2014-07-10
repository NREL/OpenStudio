/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
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

#ifndef RUNMANAGER_LIB_JOBFACTORY_HPP
#define RUNMANAGER_LIB_JOBFACTORY_HPP

#include "Job.hpp"
#include "JobParam.hpp"
#include "RunManagerAPI.hpp"
#include "ToolInfo.hpp"
#include "JobState.hpp"
#include "../../utilities/idf/URLSearchPath.hpp"

#include "../../utilities/core/Path.hpp"

namespace openstudio {
namespace ruleset {
  class ModelRuleset;
  class OSArgument;
}
namespace runmanager {

  class RubyJobBuilder;

  /// Class with static methods for creating various types of Job objects. A Job can be created either
  /// with a specific method, or with the generically defined "createJob."
  ///
  /// \sa \ref RunManagerJobTypes
  class RUNMANAGER_API JobFactory
  {
    public:

      /** Create a generically defined job using the parameters passed in
       *  Can be useful by things like WorkflowJob which can create a list of user specified jobs.
       *  Also used during loading of the RunManager database file
       *  \param[in] t_type The job type to create. See JobType enumeration
       *  \param[in] t_tools A vector of ToolInfo objects for all known/used tools. The job specific
       *                     code finds and uses the ToolInfo objects that are needed for the specific job.
       *  \param[in] t_params Named heirarchical parameter list that Job parameters are extracted from
       *  \param[in] t_files Vector of FileInfo objects the specific job can choose from.
       *  \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
       *  \param[in] t_loading If loading from a db and you do not want to perform any fixing up, set to true
       *  \param[in] t_uuid Option to specify the UUID of the job.
       *
       *  \sa \ref RunManagerJobTypes
       **/
      static Job createJob(openstudio::runmanager::JobType t_type,
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      /// Create an EnergyPlusJob, generic version
      ///
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref EnergyPlusJobType
      static Job createEnergyPlusJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      /// Create an EnergyPlusJob
      ///
      /// \param[in] t_energyPlusTool ToolInfo representing where to find energyplus
      /// \param[in] t_idd Location to idd file to use
      /// \param[in] t_idf Location to idf to simulate
      /// \param[in] t_epw Location to epw file to use during simulation
      /// \param[in] t_outdir Location to use during simulation
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref EnergyPlusJobType
      static Job createEnergyPlusJob(
          openstudio::runmanager::ToolInfo t_energyPlusTool,
          const openstudio::path &t_idd,
          const openstudio::path &t_idf,
          const openstudio::path &t_epw,
          const openstudio::path &t_outdir,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID());

      /// Create an EnergyPlusJob
      ///
      /// \param[in] t_tools Set of tools to use during simulation. Should contain at least one EnegyPlus tool.
      /// \param[in] t_idd Location to idd file to use
      /// \param[in] t_idf Location to idf to simulate
      /// \param[in] t_epw Location to epw file to use during simulation
      /// \param[in] t_outdir Location to use during simulation
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref EnergyPlusJobType
      static Job createEnergyPlusJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::path &t_idd,
          const openstudio::path &t_idf,
          const openstudio::path &t_epw,
          const openstudio::path &t_outdir,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID());

      /// Create an ExpandObjectsJob, generic version
      ///
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref ExpandObjectsJobType
      static Job createExpandObjectsJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      /// Create an XMLPreprocessorJob, generic version
      ///
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref XMLPreprocessorJobType
      static Job createXMLPreprocessorJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      /// Create a ReadVarsJob, generic version
      ///
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref ReadVarsJobType
      static Job createReadVarsJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      /// Create a ModelToIdfJob, generic version
      ///
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref ModelToIdfJobType
      static Job createModelToIdfJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      /// Create a ModelToIdfJob
      ///
      /// \param[in] t_model Path to OSM file
      /// \param[in] t_outdir Path to place generated out.idf file in
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref ModelToIdfJobType
      static Job createModelToIdfJob(
          const openstudio::path &t_model,
          const openstudio::path &t_outdir,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID());

      /// Create a ModelToRadJob, generic version
      ///
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref ModelToRadJobType
      static Job createModelToRadJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      /// Create a ModelToIdfJob
      ///
      /// \param[in] t_model Path to OSM file
      /// \param[in] t_sql Path to SQL simulation file from previous EnergyPlus run
      /// \param[in] t_outdir Path to place generated out.idf file in
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref ModelToRadJobType
      static Job createModelToRadJob(
          const openstudio::path &t_model,
          const openstudio::path &t_sql,
          const openstudio::path &t_outdir,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID());

      /// Create a IdfToModelJob, generic version
      ///
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref IdfToModelJobType
      static Job createIdfToModelJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      /// Create a IdfToModelJob
      ///
      /// \param[in] t_idf Path to IDF
      /// \param[in] t_outdir Path to place generated out.osm file in
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref ModelToIdfJobType
      static Job createIdfToModelJob(
          const openstudio::path &t_idf,
          const openstudio::path &t_outdir,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID());

      /// Create a CalculateEconomicsJob, generic version
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref CalculateEconomicsJobType
      static Job createCalculateEconomicsJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      /// Creates a PreviewIESJob
      ///
      /// \param[in] t_ies Path to input IES file
      /// \param[in] t_outdir Directory to create TIFF / BMP in
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref PreviewIESJobType
      static Job createPreviewIESJob(
          const openstudio::path &t_ies,
          const openstudio::path &t_outdir,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID());

      /// Create a PreviewIESJob, generic version
      ///
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref PreviewIESJobType
      static Job createPreviewIESJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      /// Create a RubyJob. this version assumes that a parent job has already provided the ToolInfo for the ruby executable
      ///
      /// \param[in] t_rubyfile Ruby file to execute
      /// \param[in] t_infileregex Regular expression indicating which input files are requirements for the job
      /// \param[in] t_outdir Output directory to use during simulation.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref RubyJobType
      static Job createRubyJob(
          const openstudio::path &t_rubyfile,
          const std::string &t_infileregex,
          const openstudio::path &t_outdir,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID());

      /// Create a RubyJob, generic version
      ///
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref RubyJobType
      static Job createRubyJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      static Job createRubyJob(
          const openstudio::runmanager::RubyJobBuilder &t_builder,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID());

      /// Create a UserScriptJob, generic version
      ///
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref RubyJobType
      static Job createUserScriptJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      /// Create a UserScriptJob, generic version
      ///
      /// \param[in] t_rubyfile UserScript ruby file to execute
      /// \param[in] t_args Arguments to pass to the UserScript job
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref UserScriptJob
      static Job createUserScriptJob(
          const openstudio::path &t_rubyfile,
          const std::vector<ruleset::OSArgument> &t_args,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID());

      /// Create a NullJob, generic version
      ///
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref NullJobType
      static Job createNullJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      /// Create a EnergyPlusPostProcessJob, generic version
      ///
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref EnergyPlusPostProcessJobType
      static Job createEnergyPlusPostProcessJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      /// Create a OpenStudioPostProcessJob, generic version
      ///
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref OpenStudioPostProcessJobType
      static Job createOpenStudioPostProcessJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      /// Create a EnergyPlusPreProcessJob, generic version
      ///
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref EnergyPlusPreProcessJobType
      static Job createEnergyPlusPreProcessJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      /// Create a ModelToRadPreProcessJob, generic version
      ///
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref ModelToRadPreProcessJobType
      static Job createModelToRadPreProcessJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );


      /// Create a ParallelEnergyPlusSplitJob, generic version
      ///
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref ParallelEnergyPlusSplitJobType
      static Job createParallelEnergyPlusSplitJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      /// Create a ParallelEnergyPlusJoinJob, generic version
      ///
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref ParallelEnergyPlusJoinJobType
      static Job createParallelEnergyPlusJoinJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      /// Create a BasementJob, generic version
      ///
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref BasementJobType
      static Job createBasementJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      /// Create a SlabJob, generic version
      ///
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref SlabJobType
      static Job createSlabJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      /// Create a DakotaJob, generic version
      ///
      /// \param[in] t_tools Tools to pass in
      /// \param[in] t_params JobParams to pass in
      /// \param[in] t_files Files to pass in
      /// \param[in] t_url_search_paths Vector of paths to search when fixing up URLs in an IDF or OSM
      /// \param[in] t_loading If true, skip fixing up of URLs, used when loading RunManager database file.
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref DakotaJobType
      static Job createDakotaJob(
          const openstudio::runmanager::Tools &t_tools,
          const openstudio::runmanager::JobParams &t_params,
          const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>(),
          bool t_loading = false,
          const boost::optional<openstudio::UUID> &t_uuid = createUUID(),
          const openstudio::runmanager::JobState &t_state = JobState()
          );

      /// Create a ParallelEnergyPlusJoinJob, with the expected number of jobs to join
      ///
      /// \param[in] t_numSplits number of splits to join
      /// \param[in] t_offset number of days to precalculate
      ///
      /// \sa \ref ParallelEnergyPlusJoinJobType
      static Job createParallelEnergyPlusJoinJob(int t_numSplits, int t_offset);

      /// Create a ParallelEnergyPlusSplitJob, with the number of jobs to split
      ///
      /// \param[in] t_numSplits number of splits to create
      /// \param[in] t_offset number of days to precalculate
      ///
      /// \sa \ref ParallelEnergyPlusSplitJobType
      static Job createParallelEnergyPlusSplitJob(int t_numSplits, int t_offset);

      /// Creates a Parallel EnergyPlus job tree consisting of the split job, N child jobs and
      /// a join finishedJob
      ///
      /// \param[in] t_numSplits number of splits to create
      /// \param[in] t_offset number of days to precalculate
      ///
      /// \sa \ref ParallelEnergyPlusSplitJobType
      /// \sa \ref ParallelEnergyPlusJoinJobType
      static Job createParallelEnergyPlusJobTree(
          int t_numSplits, int t_offset);

      /// Creates an EnergyPlusJob with the filename to pull from the list
      /// of provided inherited FileInfo objects specified
      ///
      /// \param[in] t_filename Filename for the EnergyPlus job to look for at job execution time
      static Job createEnergyPlusJob(const std::string &t_filename);


      /// Creates a NullJob
      ///
      /// \param[in] t_uuid Option to specify the UUID of the job.
      ///
      /// \sa \ref NullJobType
      static Job createNullJob(
          const boost::optional<openstudio::UUID> &t_uuid = createUUID());

      /// Merges all jobs which may be merged together. This includes eliminating Null jobs where possible.
      /// Additional merge behavior is job type specific.
      static void optimizeJobTree(openstudio::runmanager::Job t_job);

    private:
      static Files normalizeURLs(const openstudio::runmanager::Files &t_files,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths, bool t_loading,
          const openstudio::UUID &t_uuid);

      static bool optimizeJobTreeImpl(openstudio::runmanager::Job t_job);

  };
}
}

#endif // RUNMANAGER_LIB_JOBFACTORY_HPP

