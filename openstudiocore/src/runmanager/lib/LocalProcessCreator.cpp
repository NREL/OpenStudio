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

#include "LocalProcessCreator.hpp"
#include "LocalProcess.hpp"

namespace openstudio {
namespace runmanager {

  LocalProcessCreator::LocalProcessCreator()
  {
  }

  std::shared_ptr<Process> LocalProcessCreator::createProcess(
      const openstudio::runmanager::ToolInfo &t_tool,
      const std::vector<std::pair<openstudio::path, openstudio::path> > &t_requiredFiles,
      const std::vector<std::string> &t_parameters,
      const openstudio::path &t_outdir,
      const std::vector<openstudio::path> &t_expectedOutputFiles,
      const std::string &t_stdin,
      const openstudio::path &t_basePath,
      const boost::optional<std::pair<int,int> > &t_remoteId)
  {
    if (t_remoteId) { throw std::runtime_error("remote id set when creating local process"); }

    return std::shared_ptr<Process>(
        new detail::LocalProcess(
          t_tool,
          t_requiredFiles,
          t_parameters,
          t_outdir,
          t_expectedOutputFiles,
          t_stdin,
          t_basePath));
  }



}
}

