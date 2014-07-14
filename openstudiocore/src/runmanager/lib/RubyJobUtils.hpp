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

#ifndef RUNMANAGER_LIB_RUBYJOBUTILS_HPP
#define RUNMANAGER_LIB_RUBYJOBUTILS_HPP

#include "RunManagerAPI.hpp"
#include "Workflow.hpp"
#include "../../ruleset/OSArgument.hpp"

namespace openstudio {
  class BCLMeasure;

namespace runmanager {
  class RunManager;

  struct ScriptInfo
  {
    ScriptInfo(const openstudio::path &t_scriptPath, bool t_isUserScript, bool t_argsChanged)
      : isUserScript(t_isUserScript), argsChanged(t_argsChanged), detectionSucceeded(false),
        scriptPath(t_scriptPath)
    {
      dbPath = scriptPath;
      dbPath.replace_extension(toPath(".db").extension());
    }

    bool isUserScript;
    bool argsChanged;
    bool detectionSucceeded;
    openstudio::path scriptPath;
    openstudio::path dbPath;

    std::map<std::string, ruleset::OSArgument> arguments;
  };

  struct ScriptDetectionError : std::runtime_error
  {
    ScriptDetectionError(const std::string &t_what, const ScriptInfo &t_info) throw()
      : std::runtime_error(t_what), scriptInfo(t_info)
    {
    }

    virtual ~ScriptDetectionError() throw()
    {
    }

    ScriptInfo scriptInfo;
  };

  /** \class FileSelection */
  OPENSTUDIO_ENUM(FileSelection,
      ((Last))
      ((All))
    );

  /** \class FileSource */
  OPENSTUDIO_ENUM(FileSource,
      ((Parent))
      ((All))
      ((Self))
    );


  /// Utilities class for encoding and decoding the JobParams object needed for executing a RubyJob.
  ///
  /// Example:
  ///
  /// \code
  /// openstudio::runmanager::Workflow wf;
  ///
  /// // make a new ruby job
  /// openstudio::runmanager::RubyJobBuilder rubyJobBuilder;
  /// rubyJobBuilder.setScriptFile(perturbScript);
  /// rubyJobBuilder.addScriptParameter("inputPath", "5ZoneWarmest.idf");
  /// rubyJobBuilder.addScriptParameter("outputPath", "out.idf");
  /// rubyJobBuilder.addScriptParameter("objectType", "Building");
  /// rubyJobBuilder.addScriptParameter("nameRegex", ".*");
  /// rubyJobBuilder.addScriptParameter("field", "0");
  /// rubyJobBuilder.addScriptParameter("value", "My Building");
  /// rubyJobBuilder.addInputFile(openstudio::runmanager::FileSelection("last"),
  ///                             openstudio::runmanager::FileSource("All"),
  ///                             ".*\\.idf",
  ///                             "5ZoneWarmest.idf");
  /// rubyJobBuilder.addToolArgument("-I" + rubyOpenStudioDir()) ;
  ///
  /// // create a dummy workflow and add ruby job
  /// openstudio::runmanager::Workflow rubyJob;
  /// rubyJobBuilder.addToWorkflow(wf);
  /// \endcode
  class RUNMANAGER_API RubyJobBuilder {
    public:
      RubyJobBuilder(bool t_userScriptJob = false);

      /// Construct a RubyJobBuilder from a json file
      RubyJobBuilder(const openstudio::path &t_json);

      /// Construct a RubyJobBuilder from an existing set of JobParams (useful when loading from
      /// the database).
      RubyJobBuilder(const JobParams &t_params);

      /// Create from the workflow WorkItem
      RubyJobBuilder(const WorkItem &t_workItem);

      /// Create from the workflow WorkItem
      RubyJobBuilder(const WorkItem &t_workItem,
                     const openstudio::path& t_originalBasePath,
                     const openstudio::path& t_newBasePath);

      /// Create from a BCLMeasure by:
      ///   Setting scriptFile to t_measure.primaryRubyScriptPath()
      ///   Calling addRequiredFile for each t_measure.files()
      ///   Calling addInputFile for appropriate t_measure.inputFileType()
      ///   Calling copyRequiredFiles for t_measure.inputFileType to t_measure.outputFileType()
      RubyJobBuilder(const openstudio::BCLMeasure &t_measure,
                     const std::vector<ruleset::OSArgument>& t_args = std::vector<ruleset::OSArgument>(),
                     const openstudio::path &t_relativeTo = openstudio::path(),
                     bool t_copyFileTrue = false);

      /** \overload */
      RubyJobBuilder(const openstudio::BCLMeasure &t_measure,
                     const std::map<std::string,ruleset::OSArgument>& t_args,
                     const openstudio::path &t_relativeTo = openstudio::path(),
                     bool t_copyFileTrue = false);

      openstudio::path script() const;

      bool userScriptJob() const;

      /// Sets the name of the ruby script file to execute
      void setScriptFile(const openstudio::path &t_script);

      std::vector< std::pair<openstudio::path, openstudio::path> > requiredFiles() const;

      bool addRequiredFile(const openstudio::path& currentPath,
                           const openstudio::path& copyPath,
                           const openstudio::path& relativeTo = openstudio::path(),
                           bool verifyExistence = true);

      /// Specifies that the "requiredFiles" from an input file with given extension (t_infileextension)
      /// to all generated output files with the matching extension
      /// (t_outextension) if the requiredFile matches the given filename (t_requiredFilename)
      void copyRequiredFiles(const std::string &t_infileextension,
                             const std::string &t_outextension,
                             const std::string &t_requiredFilename);

      /// Adds a parameter to be passed to the ruby script that is executed
      void addScriptParameter(const std::string &name, const std::string &value);

      /// Adds a valueless parameter to be passed to the ruby script that is executed
      void addScriptParameter(const std::string &name);

      /// Adds an argument with no "--" prefix to the ruby script that is executed
      void addScriptArgument(const std::string &name);

      /// Clears any -I parameters currently set to be sent to the ruby script interpreter
      void clearIncludeDir();

      /// Sets the -I include dir that is passed to the ruby script interpreter during execution
      void setIncludeDir(const openstudio::path &value);

      /// Sets a parameter to pass to the ruby interpreter itself
      void addToolArgument(const std::string &name);

      /// Adds an input file from the Job defined list of input FileInfo objects
      /// that should be added as a requirement to this
      void addInputFile(const FileSelection &t_fileselection,
                        const FileSource &t_filesource,
                        const std::string &t_inputregex,
                        const std::string &t_outputname);

      /// Adds the Ruby job as it is currently defined to the given workflow.
      /// \param[in] t_requiredFiles Set of files to add as required files to the script file
      /// \deprecated
      /// \sa toWorkItem
      void addToWorkflow(Workflow &t_wf,
                         const std::vector<openstudio::path> &t_requiredFiles = std::vector<openstudio::path>()) const;

      /// Create a WorkItem representing this RubyScriptJob
      WorkItem toWorkItem(const std::vector<openstudio::path> &t_requiredFiles = std::vector<openstudio::path>()) const;

      /// Returns the set of JobParams defined by this object currently.
      JobParams toParams() const;

      /// Returns the script parameters defined
      ///
      /// \sa addScriptParameter
      /// \sa addscriptArgument
      std::vector<std::string> getScriptParameters() const;

      /// Returns the tool parameters defined.
      ///
      /// \sa addToolArguement
      std::vector<std::string> getToolParameters() const;

      /// Returns the collection of input files specified for this job
      std::vector<std::tuple<FileSelection, FileSource, std::string, std::string> > inputFiles() const;

      /// Returns the collection of required files to copy for this job
      std::vector<std::tuple<std::string, std::string, std::string> > copyRequiredFiles() const;


      const std::vector<RubyJobBuilder> &mergedJobs() const;

      boost::optional<openstudio::UUID> originalUUID() const;

      boost::optional<openstudio::UUID> bclMeasureUUID() const;

      static JobParams toJobParams(const std::vector<ruleset::OSArgument> &t_args,
          const openstudio::path &t_basePath=openstudio::path());

      static std::vector<ruleset::OSArgument> toOSArguments(const JobParams &t_params,
          const openstudio::path &t_basePath=openstudio::path());

      static void addJobsToWorkflow(
          Workflow &t_workflow,
          const std::vector<RubyJobBuilder> &t_jobs,
          const openstudio::path &t_includePath,
          const std::vector<openstudio::path> &t_requiredFiles);

      static std::vector<RubyJobBuilder> createRubyJobsFromFolder(
          const openstudio::path &t_path,
          const std::vector<std::pair<openstudio::path,std::vector<ruleset::OSArgument> > > &t_userScripts,
          const std::string &t_inputExtension,
          const openstudio::path &t_relativeTo = openstudio::path(),
          bool t_copyFileTrue = false);

      static RubyJobBuilder createUserScriptRubyJob(const openstudio::path& t_userScriptPath,
                                                    const std::vector<ruleset::OSArgument>& t_args,
                                                    const openstudio::path &t_relativeTo = openstudio::path(),
                                                    bool t_copyFileTrue = false);

      static ScriptInfo refreshArgumentsFromScript(runmanager::RunManager &t_rm, runmanager::ScriptInfo t_scriptInfo);
      static ScriptInfo updateArgumentsFromDb(runmanager::ScriptInfo t_scriptInfo);
      static ScriptInfo saveArgumentsToDb(runmanager::ScriptInfo t_scriptInfo);


    private:
      REGISTER_LOGGER("openstudio.runmanager.RubyJobUtils");
      std::vector<std::tuple<FileSelection, FileSource, std::string, std::string> > m_inputfiles;
      std::vector<std::tuple<std::string, std::string, std::string> > m_copyRequiredFiles;
      // required files to add to m_script. .first is current path, .second is path for copy
      std::vector< std::pair<openstudio::path, openstudio::path> > m_requiredFiles;
      std::vector<std::string> m_params;
      std::vector<std::string> m_toolparams;
      openstudio::path m_script;
      std::vector<RubyJobBuilder> m_mergedJobs;
      boost::optional<openstudio::UUID> m_originalUUID;

      bool m_userScriptJob;
      std::string m_jobkeyname;

      boost::optional<openstudio::UUID> m_bclMeasureUUID;

      static bool stringToBool(const std::string &t_val);
      static std::string boolToString(bool t_val);
      void initializeFromParams(const JobParams &t_params,
                                const openstudio::path& t_originalBasePath = openstudio::path(),
                                const openstudio::path& t_newBasePath = openstudio::path());

      // used internally to initialize userScriptRuby jobs and RubyJobs created from BCLMeasures
      void setAsUserScriptRubyJob(const openstudio::path& t_userScriptPath,
          const std::vector<ruleset::OSArgument>& t_args,
          const openstudio::path &t_relativeTo,
          bool t_copyFileArguments);

      void constructFromBCLMeasure(const openstudio::BCLMeasure &t_measure,
                                   const std::vector<ruleset::OSArgument>& t_args,
                                   const openstudio::path &t_relativeTo,
                                   bool t_copyFileTrue);
  };

} // runmanager
} // openstudio

#endif // RUNMANAGER_LIB_RUBYJOBUTILS_HPP


