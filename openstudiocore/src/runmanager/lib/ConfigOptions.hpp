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

#ifndef RUNMANAGER_LIB_CONFIGOPTIONS_HPP
#define RUNMANAGER_LIB_CONFIGOPTIONS_HPP

#include "RunManagerAPI.hpp"
#include "ToolInfo.hpp"

#include "../../utilities/core/Path.hpp"
#include "../../utilities/core/Enum.hpp"

namespace openstudio {
namespace runmanager {

  /** \class ToolType
   *
   *  \relates ToolLocationInfo */
  OPENSTUDIO_ENUM(ToolType,
      ((EnergyPlus))
      ((Radiance))
      ((Ruby))
      ((XMLPreprocessor)(XML Pre-processor))
      ((Dakota))
    );

  /// Contains information about a tool installation
  class RUNMANAGER_API ToolLocationInfo
  {
    public:
      ToolLocationInfo() {} 

      /** 
       *  \param[in] t_type Enumeration value specifying a tool/application
       *  \param[in] t_bindir Path to the binary for local execution
       *  \param[in] t_linuxbinarchive Path to the .tar.gz file for remote distribution and 
       *             execution */
      ToolLocationInfo(const ToolType &t_type, 
                       const openstudio::path &t_bindir, 
                       const openstudio::path &t_linuxbinarchive);

      bool operator<(const ToolLocationInfo &rhs) const;
      bool operator==(const ToolLocationInfo &rhs) const;

      /// Type of tool at this location
      ToolType toolType;

      /// Location of binary for local execution
      openstudio::path binaryDir;

      /// Location of archive containing binary for remote execution
      openstudio::path linuxBinaryArchive;
  };

  /// Operator overload for streaming ToolLocationInfo to ostream
  RUNMANAGER_API std::ostream &operator<<(std::ostream &os, const ToolLocationInfo &epi);


  //! Stores configuration options for the runmanager project
  //! \sa openstudio::runmanager::Configuration
  class RUNMANAGER_API ConfigOptions
  {
    public:
      //! Constructor to set intelligent defaults for the current system
      //! Defaults to max parallel local jobs as number of hardware threads - 1
      explicit ConfigOptions(bool t_loadQSettings = false);

      //! Automatically find tools and add them to the ConfigOptions
      bool findTools(bool t_showProgressDialog, bool t_onlyIfZeroTools=false, bool t_promptUser=false, bool t_checkQSettings=false);

      //! Automatically find EnergyPlus if it is in a well defined, known place, then add to ConfigOptions
      void fastFindEnergyPlus();

      //! Automatically find Radiance if it is in a well defined, known place, then add to ConfigOptions
      void fastFindRadiance();

      //! Saves the tools to the appropriate QSettings location.
      void saveQSettings() const;

      //! Add a tool to the list of known versions
      void setToolLocation(const ToolVersion &t_epv, const ToolLocationInfo &t_info);

      //! get all known version / path pairs
      std::vector<std::pair<ToolVersion, ToolLocationInfo> > getToolLocations() const;

      //! get all known tools for passing to jobs
      openstudio::runmanager::Tools getTools() const;

      //! Convert a pair returned from getToolLocation to a ToolInfo object
      //! appropriate for creating an EnergyPlusJob
      //! \param[in] eplus results from getToolLocations create ToolInfo from
      static openstudio::runmanager::ToolInfo toEPlusToolInfo(const std::pair<ToolVersion, ToolLocationInfo> &eplus);

      //! Convert a pair returned from getToolLocation to a ToolInfo object
      //! appropriate for creating an ExpandObjects job
      //! \param[in] eplus results from getToolLocations create ToolInfo from
      static openstudio::runmanager::ToolInfo toExpandObjectsToolInfo(const std::pair<ToolVersion, ToolLocationInfo> &eplus);

      //! Convert a pair returned from getToolLocation to a ToolInfo object
      //! appropriate for creating a Basement job
      //! \param[in] eplus results from getToolLocations create ToolInfo from
      static openstudio::runmanager::ToolInfo toBasementToolInfo(const std::pair<ToolVersion, ToolLocationInfo> &eplus);
      
      //! Convert a pair returned from getToolLocation to a ToolInfo object
      //! appropriate for creating a Slab job
      //! \param[in] eplus results from getToolLocations create ToolInfo from
      static openstudio::runmanager::ToolInfo toSlabToolInfo(const std::pair<ToolVersion, ToolLocationInfo> &eplus);
      
      //! Convert a pair returned from getToolLocation to a ToolInfo object
      //! appropriate for creating an xmlpreprocessor job
      //! \param[in] preproc results from getToolLocations create ToolInfo from
      static openstudio::runmanager::ToolInfo 
        toXMLPreprocessorToolInfo(const std::pair<ToolVersion, ToolLocationInfo> &preproc);


      //! Convert a pair returned from getToolLocation to a ToolInfo object
      //! appropriate for creating an ies2rad job
      //! \param[in] rad results from getToolLocations create ToolInfo from
      static openstudio::runmanager::ToolInfo toIES2RadToolInfo(const std::pair<ToolVersion, ToolLocationInfo> &rad);

      //! Convert a pair returned from getToolLocation to a ToolInfo object
      //! appropriate for creating an ra_image job
      //! \param[in] rad results from getToolLocations create ToolInfo from
      static openstudio::runmanager::ToolInfo toRaImageToolInfo(const std::pair<ToolVersion, ToolLocationInfo> &rad);

      //! Convert a pair returned from getToolLocation to a ToolInfo object
      //! appropriate for creating a rad job
      //! \param[in] rad results from getToolLocations create ToolInfo from
      static openstudio::runmanager::ToolInfo toRadToolInfo(const std::pair<ToolVersion, ToolLocationInfo> &rad);

      //! Convert a pair returned from getToolLocation to a ToolInfo object
      //! appropriate for creating a rtrace job
      //! \param[in] rad results from getToolLocations create ToolInfo from
      static openstudio::runmanager::ToolInfo toRTraceToolInfo(const std::pair<ToolVersion, ToolLocationInfo> &rad);

      //! Convert a pair returned from getToolLocation to a ToolInfo object
      //! appropriate for creating a rtrace job
      //! \param[in] rad results from getToolLocations create ToolInfo from
      static openstudio::runmanager::ToolInfo toEpw2Wea(const std::pair<ToolVersion, ToolLocationInfo> &eplus);

      //! Convert a pair returned from getToolLocation to a ToolInfo object
      //! appropriate for creating a ruby RubyJob
      //! \param[in] ruby results from getToolLocations create ToolInfo from
      static openstudio::runmanager::ToolInfo toRubyToolInfo(const std::pair<ToolVersion, ToolLocationInfo> &ruby);

      //! Convert a pair returned from getToolLocation to a ToolInfo object
      //! appropriate for creating a DakotaJob
      //! \param[in] dakota result from getToolLocations create ToolInfo from
      static openstudio::runmanager::ToolInfo toDakotaToolInfo(const std::pair<ToolVersion, ToolLocationInfo>& dakota);

      //! Convert a pair returned from getToolLocation to a ToolInfo object
      //! \param[in] t_type Type of tool to create
      //! \param[in] t_tool results from getToolLocations create ToolInfo from
      static openstudio::runmanager::Tools makeTools(
          const ToolType &t_type,
          const std::pair<ToolVersion, ToolLocationInfo> &t_tool);

      //! Convert some paths into standard understood ToolInfo objects. The created
      //! ToolInfos have version numbers determined by ToolFinder::parseToolVersion. 
      //! They have no remote execution information associated with them.
      //! \param[in] t_energyplus Path to energy plus binaries
      //! \param[in] t_xmlpreproc Path to xmlpreproc binary
      //! \param[in] t_radiance Path to radiance binaries
      //! \param[in] t_ruby Path to ruby binary
      static openstudio::runmanager::Tools makeTools(const openstudio::path &t_energyplus, 
                                                     const openstudio::path &t_xmlpreproc, 
                                                     const openstudio::path &t_radiance,
                                                     const openstudio::path &t_ruby,
                                                     const openstudio::path &t_dakota);

      //! Convert some paths into standard understood ToolInfo objects. The created
      //! ToolInfos have no version number. They do have remote execution information 
      //! associated if provided.
      //! \param[in] t_energyplus Path to energy plus binaries
      //! \param[in] t_xmlpreproc Path to xmlpreproc binary
      //! \param[in] t_radiance Path to radiance binaries
      //! \param[in] t_ruby Path to ruby binary
      //! \param[in] t_remote_energyplus Path to energy plus binaries for remote execution
      //! \param[in] t_remote_xmlpreproc Path to xmlpreproc binary for remote execution
      //! \param[in] t_remote_radiance Path to radiance binaries for remote execution
      //! \param[in] t_remote_ruby Path to ruby binary for remote execution
      static openstudio::runmanager::Tools makeTools(const openstudio::path &t_energyplus, 
                                                     const openstudio::path &t_xmlpreproc, 
                                                     const openstudio::path &t_radiance,
                                                     const openstudio::path &t_ruby,
                                                     const openstudio::path &t_dakota,
                                                     const openstudio::path &t_remote_energyplus, 
                                                     const openstudio::path &t_remote_xmlpreproc,
                                                     const openstudio::path &t_remote_radiance,
                                                     const openstudio::path &t_remote_ruby,
                                                     const openstudio::path &t_remove_dakota);

      //! Construct a ToolInfo object from the set of information
      //! used as a convenience function
      //! \param[in] t_type Type of tool to create
      //! \param[in] t_local Path to local binaries for tool
      //! \param[in] t_remote Path to binaries for tool for remote execution
      //! \param[in] t_version Version info to attach to created Tools
      static openstudio::runmanager::Tools makeTools(
          const ToolType &t_type,
          const openstudio::path &t_local,
          const openstudio::path &t_remote,
          const ToolVersion &t_version);

      //! Remove all known versions of E+. All other configuration data stays the same.
      void reset();

      //! \returns default location to open UI dialogs to when opening an IDF
      openstudio::path getDefaultIDFLocation() const;

      //! Sets the default location to open UI dialogs to when opening an IDF
      //! \param[in] t_loc The new location
      void setDefaultIDFLocation(const openstudio::path &t_loc);

      //! \returns default location to open UI dialogs to when opening an EPW
      openstudio::path getDefaultEPWLocation() const;

      //! Sets the default location to open UI dialogs to when opening an EPW
      //! \param[in] t_loc The new location
      void setDefaultEPWLocation(const openstudio::path &t_loc);

      //! \returns configured output location to use for jobs
      openstudio::path getOutputLocation() const;

      //! Sets the configured output location
      //! \param[in] t_loc The new location
      void setOutputLocation(const openstudio::path &t_loc);

      //! \returns the maximum number of simultaneous jobs to run locally
      int getMaxLocalJobs() const;

      //! Set the maximum number of simultaneous jobs to run locally
      //! \param[in] t_numjobs the new max
      void setMaxLocalJobs(int t_numjobs);

      //! Return the user name to use when connecting to a remote SLURM based system over ssh
      std::string getSLURMUserName() const;

      //! Set the user name to use when connecting to a remote SLURM based system over ssh
      //! \param[in] t_username the new user name
      void setSLURMUserName(const std::string &t_username);

      //! \returns the host to use when connecting to a SLURM based system over ssh
      std::string getSLURMHost() const;

      //! Set the SLURM hostname to use when connecting via SSH
      //! \param[in] t_host the new hostname
      void setSLURMHost(const std::string &t_host);
  
      //! \returns the maximum number of jobs to queue up on a remote SLURM system    
      int getMaxSLURMJobs() const;

      //! Set the maximum number of jobs to run on the SLURM system
      //! \param[in] t_numjobs the new maximum
      void setMaxSLURMJobs(int t_numjobs);

      //! \returns True if jobs created by the RunManager UI should have simple folder names
      //!          these folder names are more likely to conflict with other jobs
      bool getSimpleName() const;

      //! Set the SimpleName state
      //! \param[in] t_simplename Whether or not the folder names used by the RunManager UI should be more simple
      //! \sa setSimpleName
      void setSimpleName(bool t_simplename);

      //! Return the max time in minutes that the job should be allowed to run by SLURM
      int getSLURMMaxTime() const;

      //! Set the max time in minutes that the job should be allowed to run by SLURM
      void setSLURMMaxTime(int t_maxTime);

      //! Get the SLURM partition string to set when queuing the job. Optional. An empty string means no partition is set
      std::string getSLURMPartition() const;

      //! Set the SLURM partition string to set when queuing the job. Optional. An empty string means no partition is set 
      void setSLURMPartition(const std::string &t_queueName);

      //! Get the SLURM account id to charge to when queuing the job. Optional. An empty string means no account is set
      std::string getSLURMAccount() const;

      //! Set the SLURM account id to charge to when queuing the job. Optional. An empty string means no account is set
      void setSLURMAccount(const std::string &t_account);
    
    private:
      REGISTER_LOGGER("RunManager.ConfigOptions");
      //! Returns a vector of paths that may contain EnergyPlus
      std::vector<openstudio::path> potentialEnergyPlusLocations() const;

      //! Returns a vector of paths that may contain Radiance
      std::vector<openstudio::path> potentialRadianceLocations() const;

      void saveQSettings(const std::vector<std::pair<openstudio::runmanager::ToolVersion, openstudio::runmanager::ToolLocationInfo> > &t_tools) const;
      std::vector<std::pair<openstudio::runmanager::ToolVersion, openstudio::runmanager::ToolLocationInfo> > loadQSettingsTools() const;
      void loadQSettingsData();

      void saveQSettingsData() const;

      //! Set up default paths for idf and epw dirs
      void setIDFEPWDefaults();

      //! Location for energyplus executables
      std::set<std::pair<ToolVersion, ToolLocationInfo> > m_toolLocations;

      //! Location the UI will default to for the IDF open dialog
      openstudio::path m_defaultIDFLocation;

      //! Location the UI will default to for the EPW open dialog
      openstudio::path m_defaultEPWLocation;

      //! Location the UI will use for the base dir for output files
      openstudio::path m_outputLocation;

      //! Number of jobs that can be run locally simultaneously
      int m_maxLocalJobs;

      //! SLURM host to connect to for remote jobs
      std::string m_slurmHost;

      //! Username to use when connecting to slurm job host
      std::string m_slurmUserName;

      //! Max time for a SLURM job to run
      int m_slurmMaxTime;

      //! Name of queue for SLURM job to run in
      std::string m_slurmPartition;

      //! Name of account to set for SLURM job runs
      std::string m_slurmAccount;

      //! number of jobs that can be run via slurm simultaneously
      int m_slurmMaxJobs;

      //! Whether the UI should use simplified folder names which are more likely to conflict with each other
      bool m_simpleName;

  };

}
}

#endif // RUNMANAGER_LIB_CONFIGOPTIONS_HPP
