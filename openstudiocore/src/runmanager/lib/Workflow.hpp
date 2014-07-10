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

#ifndef RUNMANAGER_LIB_WORKFLOW_HPP
#define RUNMANAGER_LIB_WORKFLOW_HPP

#include "RunManagerAPI.hpp"
#include "JobFactory.hpp"

#include "../../utilities/core/Logger.hpp"
#include "../../utilities/core/FileReference.hpp"
#include "../../utilities/idf/URLSearchPath.hpp"

class QCryptographicHash;

namespace openstudio {
  namespace ruleset {
    class OSArgument;
  }

namespace runmanager {
  class WorkItem;


  /// Utility for constructing a reproducible Job tree.
  class RUNMANAGER_API Workflow
  {
    private:

      struct WorkflowJob
      {
        WorkflowJob(const openstudio::runmanager::JobType &t_type,
            const openstudio::runmanager::Tools &t_tools,
            const openstudio::runmanager::JobParams &t_params,
            const openstudio::runmanager::Files &t_files)
          : type(t_type), tools(t_tools), params(t_params), files(t_files)
        {
        }

        WorkflowJob(openstudio::runmanager::Job t_job)
          : type(t_job.jobType()), tools(t_job.tools()), params(t_job.params()), files(t_job.inputFiles())
        {
          params.remove("workflowname");
          boost::optional<Job> fj = t_job.finishedJob();

          if (fj)
          {
            finishedJob = std::shared_ptr<WorkflowJob>(new WorkflowJob(*fj));
          }

          std::vector<Job> c = t_job.children();

          for (std::vector<Job>::const_iterator itr = c.begin();
               itr != c.end();
               ++itr)
          {
            children.push_back(std::shared_ptr<WorkflowJob>(new WorkflowJob(*itr)));
          }
        }

        WorkflowJob(const WorkflowJob &t_wfjob)
          : type(t_wfjob.type), tools(t_wfjob.tools), params(t_wfjob.params), files(t_wfjob.files)
        {
          if (t_wfjob.finishedJob)
          {
            finishedJob = std::shared_ptr<WorkflowJob>(new WorkflowJob(*t_wfjob.finishedJob));
          }

          for (const auto & workflowJob : t_wfjob.children)
          {
            children.push_back(std::shared_ptr<WorkflowJob>(new WorkflowJob(*workflowJob.get())));
          }
        }

        bool operator==(const WorkflowJob &t_rhs) const
        {
          bool dataequal = type == t_rhs.type
                           && tools == t_rhs.tools
                           && params == t_rhs.params
                           && files == t_rhs.files;

          bool childrenequal = true;

          if (t_rhs.children.size() == children.size())
          {
            for (auto itr1 = children.begin(), itr2 = t_rhs.children.begin();
                itr1 != children.end() &&  itr2 != t_rhs.children.end();
                ++itr1, ++itr2)
            {
              assert(*itr1);
              assert(*itr2);
              if (!(*(*(itr1)) == *(*(itr2))))
              {
                childrenequal = false;
                break;
              }
            }
          } else {
            childrenequal = false;
          }

          bool finishedjobsequal =
            (!finishedJob && !t_rhs.finishedJob)
            || (finishedJob && t_rhs.finishedJob && *finishedJob == *t_rhs.finishedJob);

          return dataequal && childrenequal && finishedjobsequal;


        }

        openstudio::runmanager::JobType type;
        openstudio::runmanager::Tools tools;
        openstudio::runmanager::JobParams params;
        openstudio::runmanager::Files files;

        std::vector<std::shared_ptr<WorkflowJob> > children;
        std::shared_ptr<WorkflowJob> finishedJob;
      };

    public:

      /// Default constructor
      Workflow();

      /// Copy constructor
      Workflow(const Workflow &t_wf);


      /// Reconstruct a workflow from a Job object. The entire job tree
      /// is deconstructed into the Workflow object
      explicit Workflow(openstudio::runmanager::Job t_job, const std::string &t_workflowName="");

      /// Construct a workflow from the given string. The string is parsed as a simple
      /// parent->child relationship.
      ///
      /// Example:
      /// \code
      /// Workflow wf("NullJob->EnergyPlusJob");
      /// \endcode
      ///
      /// Creates a Workflow with a NullJob at the head with a single child job
      /// that is an EnergyPlusJob
      explicit Workflow(const std::string &t_workflowstring, const std::string &t_workflowName="");

      /// Construct a workflow tree of the given JobTypes, organized as:
      /// t_types[0]->t_types[1]->...->t_types[n]
      explicit Workflow(const std::vector<openstudio::runmanager::JobType> &t_types, const std::string &t_workflowName="");

      /// Construct a workflow tree from the linear vector of WorkItem objects
      explicit Workflow(const std::vector<openstudio::runmanager::WorkItem> &t_items, const std::string &t_workflowName="");

      Workflow &operator=(const openstudio::runmanager::Workflow &t_rhs);

      void swap(Workflow &t_other);

      bool operator==(const openstudio::runmanager::Workflow &t_rhs) const
      {
        if (!m_job && !t_rhs.m_job)
        {
          return true;
        } else if (!(m_job && t_rhs.m_job)) {
          return false;
        }

        WorkflowJob lhs(*(this->m_job));
        WorkflowJob rhs(*(t_rhs.m_job));

        lhs.params.remove("workflowkey");
        rhs.params.remove("workflowkey");

        return lhs == rhs;
      }

      /// Adds the tree of Job types to the current workflow
      ///
      /// \sa Workflow::Workflow(const std::vector<openstudio::runmanager::JobType> &)
      void addJobs(const std::vector<openstudio::runmanager::JobType> &t_types);

      /// Adds a Job of the given type as the child of the last most recently added job
      ///
      /// \param[in] type Type of the job to add
      void addJob(openstudio::runmanager::JobType type);

      /// Adds a Job of the given type as the child of the last most recently added job
      ///
      /// \param[in] type Type of the job to add
      /// \param[in] jobkeyname Key name to give the job.
      ///
      /// \sa replaceJobs
      void addJob(openstudio::runmanager::JobType type, const std::string &jobkeyname);

      /// Adds a Job as represented by the WorkItem
      ///
      /// \param[in] t_wi WorkItem of the job to add
      /// \param[in] jobkeyname Key name to give the job.
      ///
      /// \sa replaceJobs
      void addJob(const openstudio::runmanager::WorkItem &t_wi, const std::string &jobkeyname);

      /// Adds a Job as represented by the WorkItem
      ///
      /// \param[in] t_wi WorkItem of the job to add
      ///
      /// \sa replaceJobs
      void addJob(const openstudio::runmanager::WorkItem &t_wi);

      /// Adds a Job of the given type as the child of the last most recently added job
      ///
      /// \param[in] type Type of the job to add
      /// \param[in] tools Tools to attach to the Job
      /// \param[in] params JobParams to attach to the Job
      /// \param[in] files Files to attach to the Job
      void addJob(openstudio::runmanager::JobType type,
          const openstudio::runmanager::Tools &tools,
          const openstudio::runmanager::JobParams &params,
          const openstudio::runmanager::Files &files);

      /// Adds a Job of the given type as the child of the last most recently added job
      ///
      /// \param[in] type Type of the job to add
      /// \param[in] tools Tools to attach to the Job
      /// \param[in] params JobParams to attach to the Job
      /// \param[in] files Files to attach to the Job
      /// \param[in] jobkeyname Key name to give the job.
      ///
      /// \sa replaceJobs
      void addJob(openstudio::runmanager::JobType type,
          const openstudio::runmanager::Tools &tools,
          const openstudio::runmanager::JobParams &params,
          const openstudio::runmanager::Files &files,
          const std::string &jobkeyname);

      /// Replaces all jobs with the given keyname with the given type
      ///
      /// \param[in] jobkeyname Key name to replace
      /// \param[in] type New job type to add in place of old job
      void replaceJobs(const std::string &jobkeyname,
          openstudio::runmanager::JobType type);

      /// Replaces all jobs with the given keyname with the given type
      ///
      /// \param[in] jobkeyname Key name to replace
      /// \param[in] type New job type to add in place of old job
      /// \param[in] tools Tools to attach to the Job
      /// \param[in] params JobParams to attach to the Job
      /// \param[in] files Files to attach to the Job
      void replaceJobs(const std::string &jobkeyname,
          openstudio::runmanager::JobType type,
          const openstudio::runmanager::Tools &tools,
          const openstudio::runmanager::JobParams &params,
          const openstudio::runmanager::Files &files);

      /// Replaces all jobs with the given keyname with the given new Workflow
      ///
      /// \param[in] jobkeyname Key name to replace
      /// \param[in] t_wf New Workflow to add in place of old job
      void replaceJobs(const std::string &jobkeyname,
          const openstudio::runmanager::Workflow &t_wf);

      /// Replaces all jobs with the given keyname with the given new Workflow
      ///
      /// \param[in] jobkeyname Key name to replace
      /// \param[in] t_job New Job tree to add in place of old job
      void replaceJobs(const std::string &jobkeyname,
          const openstudio::runmanager::Job &t_job);

       /// Append workflow to last job in this workflow
      void addWorkflow(const openstudio::runmanager::Workflow &t_wf);


      /// Adds a FileInfo object to the first job in the Workflow
      void addFile(const std::string &key, const openstudio::path &filepath);

      /// Adds a ToolInfo object to the first job in the workflow
      void addTool(const openstudio::runmanager::ToolInfo &tool);

      /// Adds a JobParam object to the first job in the workflow
      void addParam(const openstudio::runmanager::JobParam &param);

      /// Adds a Tools object to the Tools object of the first job in the workflow
      void add(const openstudio::runmanager::Tools &t_tools);

      /// Adds a Files object to the Tools object of the first job in the workflow
      void add(const openstudio::runmanager::Files &t_files);

      /// Adds a JobParams object to the Tools object of the first job in the workflow
      void add(const openstudio::runmanager::JobParams &t_params);

      openstudio::UUID uuid() const;

      /** To be called by ProjectDatabase for deserialization. */
      void setUUID(const openstudio::UUID& uuid);

      /// Returns a generated hash key that identifies this exact workflow configuration
      std::string key() const;

      /// Returns the tools attached to the first job.
      openstudio::runmanager::Tools tools() const;

      /// \returns the params attached to the first job
      openstudio::runmanager::JobParams params() const;

      /// \returns the FileInfos attacked to the first job.
      openstudio::runmanager::Files files() const;

      /// \returns the type of the first job
      openstudio::runmanager::JobType type() const;

      /** Returns the file type required for this Workflow's first Job. */
      boost::optional<FileReferenceType> requiredSeedFileType() const ;

      void setInputFiles(const openstudio::path &infile,
                         const openstudio::path &epwdirorfile);

      void setInputFiles(const std::vector<openstudio::path> &infiles);

      /// Creates a Job tree from the current workflow
      ///
      /// \param[in] t_url_search_paths paths to search while normalizing any URLs in IDFs or OSMs that exist in the workflow
      openstudio::runmanager::Job create(const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>());

      /// Creates a Job tree from the current workflow
      ///
      /// \param[in] outdir Output directory to be applied to the first job in the Workflow (child jobs inherit the outdir)
      /// \param[in] t_url_search_paths paths to search while normalizing any URLs in IDFs or OSMs that exist in the workflow
      openstudio::runmanager::Job create(const openstudio::path &outdir,
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>());

      /// Creates a Job tree from the current workflow
      ///
      /// \param[in] outdir Output directory to be applied to the first job in the Workflow (child jobs inherit the outdir)
      /// \param[in] infile Input file to be attached to the first job in the workflow
      /// \param[in] epwdirorfile EPW file or directory containing EPW files to be search when looking for the appropriate
      ///                         EPW for energyplus simulation of infile
      /// \param[in] t_url_search_paths paths to search while normalizing any URLs in IDFs or OSMs that exist in the workflow
      openstudio::runmanager::Job create(
          const openstudio::path &outdir,
          const openstudio::path &infile,
          const openstudio::path &epwdirorfile = openstudio::path(),
          const std::vector<openstudio::URLSearchPath> &t_url_search_paths = std::vector<openstudio::URLSearchPath>());

      /** Creates a Job tree from the current workflow
       *
       *  \param[in] outdir Output directory to be applied to the first job in the Workflow (child jobs inherit the outdir)
       *  \param[in] infiles First path in vector will be attached to the first job in the workflow.
       *                     Subsequent paths will be designated requiredFiles of the first file.
       *  \param[in] t_url_search_paths paths to search while normalizing any URLs in IDFs or OSMs that exist in the workflow
       */
      openstudio::runmanager::Job create(
          const openstudio::path& outdir,
          const std::vector<openstudio::path>& infiles,
          const std::vector<openstudio::URLSearchPath>& t_url_search_paths = std::vector<openstudio::URLSearchPath>());

      /// Adds a "standard workflow" consisting of:
      /// [model_scripts] -> ModelToIdf -> [idf_scripts] -> EnergyPlus -> [post_energyplus]
      /// If ruby is not indicated and ruby scripts are found in the scripts folders, then true is returned,
      /// indicating that ruby scripts have been skipped
      ///
      /// \param[in] t_scriptsPath location to search for scripts to add between jobs
      /// \param[in] t_rubyInstalled indicates if ruby exists as a tool and if ruby scripts should be added
      ///            to the workflow
      /// \param[in] t_rubyIncludePath include path to pass to the ruby interpreter
      /// \param[in] t_useRadianceForDaylightCalculations If true, use radiance. Requires radiance and ruby installed
      bool addStandardWorkflow(const openstudio::path &t_scriptsPath,
                               bool t_rubyInstalled,
                               const openstudio::path &t_rubyIncludePath,
                               const std::map<openstudio::path, std::vector< std::pair<openstudio::path, std::vector<ruleset::OSArgument> > > >& t_userScriptsByFolder,
                               bool t_useRadianceForDaylightCalculations,
                               const openstudio::path &t_radiancePath = openstudio::path(),
                               const openstudio::path &t_relativeTo = openstudio::path(),
                               bool t_copyFileTrue = false);

      bool addStandardWorkflow(const openstudio::path &t_scriptsPath,
                               bool t_rubyInstalled,
                               const openstudio::path &t_rubyIncludePath,
                               bool t_useRadianceForDaylightCalculations,
                               const openstudio::path &t_radiancePath = openstudio::path());

      /// Sets the name of the workflow
      void setName(const std::string &t_workflowName);

      /// Returns the name of the workflow
      std::string getName() const;

      /// \returns a vector of WorkItem objects if the workflow is linear. Throws an exception if not.
      std::vector<WorkItem> toWorkItems() const;

      /// \returns a WorkItem that performs daylight calculations
      static WorkItem radianceDaylightCalculations(const openstudio::path &t_rubyIncludePath,
          const openstudio::path &t_radiancePath);

      /// Swaps out any EnergyPlusJob with an equivalent ParallelEnergyPlus job
      /// 
      /// \param[in] t_numSplits number of runperiods to split the job into
      /// \param[in] t_offset 
      void parallelizeEnergyPlus(int t_numSplits, int t_offset);

    private:
      REGISTER_LOGGER("openstudio.runmanager.Workflow");

      std::shared_ptr<WorkflowJob> getLastJob(const std::shared_ptr<Workflow::WorkflowJob> &t_job);
      std::shared_ptr<WorkflowJob> getLastJob();
      std::shared_ptr<WorkflowJob> getFirstJob() const;

      Job createJob(const std::shared_ptr<Workflow::WorkflowJob> &t_job, const std::vector<openstudio::URLSearchPath> &t_url_search_paths);

      void replaceJobs(const std::string &jobkeyname,
          const std::shared_ptr<Workflow::WorkflowJob> &t_job);

      void replaceJobs(const std::string &jobkeyname,
          const std::shared_ptr<Workflow::WorkflowJob> &t_newjob,
          std::shared_ptr<Workflow::WorkflowJob> &t_oldjob);

      bool addScripts(const openstudio::path &t_scriptsPath,
                      const std::vector<std::pair<openstudio::path,std::vector<ruleset::OSArgument> > > &t_userScripts,
                      const openstudio::path &t_resourcesPath,
                      const std::string &t_inputExtension,
                      bool t_rubyInstalled,
                      const openstudio::path &t_rubyIncludePath,
                      const openstudio::path &t_relativeTo = openstudio::path(),
                      bool t_copyFileTrue = false);

      static std::vector<openstudio::path> buildPathVector(const openstudio::path &t_path,
        const openstudio::path &t_relativeTo);


      void hash(const openstudio::runmanager::JobType &t_type, QCryptographicHash &t_hash) const;
      void hash(const openstudio::runmanager::ToolInfo &t_tool, QCryptographicHash &t_hash) const;
      void hash(const openstudio::runmanager::Tools &t_tools, QCryptographicHash &t_hash) const;
      void hash(const openstudio::runmanager::FileInfo &t_file, QCryptographicHash &t_hash) const;
      void hash(const openstudio::runmanager::Files &t_files, QCryptographicHash &t_hash) const;
      void hash(const openstudio::runmanager::JobParam &t_param, QCryptographicHash &t_hash) const;
      void hash(const openstudio::runmanager::JobParams &t_params, QCryptographicHash &t_hash) const;
      void hash(const std::shared_ptr<Workflow::WorkflowJob> &t_job, QCryptographicHash &t_hash) const;

      openstudio::UUID m_uuid; // for ProjectDatabase persistence
      std::string m_workflowName;
      std::shared_ptr<WorkflowJob> m_job;
      std::shared_ptr<WorkflowJob> m_last_job;
  };

  /** \relates Workflow */
  typedef std::vector<Workflow> WorkflowVector;

}
}

namespace std {
  template<>
  inline void swap(openstudio::runmanager::Workflow &t_lhs, openstudio::runmanager::Workflow &t_rhs)
  {
    t_lhs.swap(t_rhs);
  }
}

#endif // RUNMANAGER_LIB_WORKFLOW_HPP
