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

#ifndef RUNMANAGER_LIB_EXPANDOBJECTSJOB_HPP
#define RUNMANAGER_LIB_EXPANDOBJECTSJOB_HPP

#include <boost/filesystem.hpp>
#include <string>
#include "../../utilities/core/Logger.hpp"
#include "Job_Impl.hpp"
#include "ToolInfo.hpp"
#include "ToolBasedJob.hpp"
#include "WeatherFileFinder.hpp"

#include "../../energyplus/ErrorFile.hpp"

#include <QProcess>
#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDateTime>

namespace openstudio {
namespace runmanager {
namespace detail {

  /// Job that uses the ToolBasedJob base class to implement a call to the ExpandObject executabel
  class ExpandObjectsJob : public ToolBasedJob
  {
    Q_OBJECT;

    public:
      ExpandObjectsJob(const UUID &t_uuid,
          const Tools &t_tools,
          const JobParams &t_params,
          const Files &t_files,
      const JobState &t_restoreData);

      virtual ~ExpandObjectsJob();


    protected:
      virtual std::string descriptionImpl() const;
      virtual std::string detailedDescriptionImpl() const
      {
        return "";
      }
      virtual ToolVersion getToolVersionImpl(const std::string &t_toolName) const;
      virtual void startHandlerImpl();

      virtual void basePathChanged();

    private:
      REGISTER_LOGGER("openstudio.runmanager.ExpandObjectsJob");

      void getFiles(const Files &t_files) const;
       
      mutable boost::optional<openstudio::runmanager::FileInfo> m_idf;
      std::string m_description; //< Description of job

   }; 

}
}
}
#endif // RUNMANAGER_LIB_EXPANDOBJECTSJOB_HPP
