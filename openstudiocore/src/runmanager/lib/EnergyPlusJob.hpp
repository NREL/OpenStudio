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

#ifndef RUNMANAGER_LIB_ENERGYPLUSJOB_HPP
#define RUNMANAGER_LIB_ENERGYPLUSJOB_HPP

#include <boost/filesystem.hpp>
#include <string>
#include "../../utilities/core/Logger.hpp"
#include "Job_Impl.hpp"
#include "ToolInfo.hpp"
#include "ToolBasedJob.hpp"

#include <QProcess>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {

  /// Implementation of ToolBasedJob for executing EnergyPlus jobs
  ///
  /// \sa \ref EnergyPlusJobType
  class EnergyPlusJob : public ToolBasedJob
  {
    Q_OBJECT;

    public:
      EnergyPlusJob(const UUID &t_uuid,
          const Tools &tools,
          const JobParams &params,
          const Files &files,
      const JobState &t_restoreData);

      virtual ~EnergyPlusJob();

    protected:
      virtual std::string descriptionImpl() const;
      virtual std::string detailedDescriptionImpl() const;

      /// Returns version of EnergyPlus required to run this simulation
      virtual ToolVersion getToolVersionImpl(const std::string &t_toolName) const;

      /// Actual execution implementation
      virtual void startHandlerImpl();

      virtual void basePathChanged();

    private:
      void getFiles(const Files &t_files, const JobParams &t_params) const;


      REGISTER_LOGGER("openstudio.runmanager.EnergyPlusJob");

      /// May be updated and cached at any point
      mutable boost::optional<openstudio::runmanager::FileInfo> m_idf;

      mutable boost::optional<std::string> m_filelocationname;
      mutable boost::optional<std::string> m_weatherfilename;

      std::string m_description; //< Description of job

  }; 

}
}
}
#endif // RUNMANAGER_LIB_ENERGYPLUSJOB_HPP
