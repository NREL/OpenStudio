/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef RUNMANAGER_LIB_PROCESSCREATOR_HPP
#define RUNMANAGER_LIB_PROCESSCREATOR_HPP

#include "Process.hpp"
#include "ToolInfo.hpp"
#include "../../utilities/core/UUID.hpp"

namespace openstudio {
namespace runmanager {

  /// Interface for Creating processes. Originally abstracted such that the Job did not need to
  /// care if the processes were running locally or remotely
  class ProcessCreator : public QObject
  {
    Q_OBJECT;

    public:
      /** \param[in] t_tooltoexecute The tool to execute locally
       *  \param[in] t_requiredFiles The vector of files needed to execute the tool
       *  \param[in] t_parameters The vector of parameters to pass to tool during execution
       *  \param[in] t_outdir Directory to watch for output files to be created 
       *  \param[in] t_expectedOutputFiles Output files in various directories to explicitly watch
       *  \param[in] t_stdin Input to send to the process over stdin after it has started
       *  \param[in] t_basePath Base path from which required files should be evaluated if the required file
       *                        is a relative path and does not reside in the tool path
       *  \returns the created Process */
      virtual std::shared_ptr<Process> createProcess(
          const ToolInfo &t_tooltoexecute,
          const std::vector<std::pair<openstudio::path, openstudio::path> > &t_requiredFiles,
          const std::vector<std::string> &t_parameters,
          const openstudio::path &t_outdir,
          const std::vector<openstudio::path> &t_expectedOutputFiles,
          const std::string &t_stdin,
          const openstudio::path &t_basePath) = 0;



  };

}
}

#endif // RUNMANAGER_LIB_PROCESSCREATOR_HPP
