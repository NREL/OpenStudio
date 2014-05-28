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


#ifndef RUNMANAGER_LIB_TOOLFINDER_HPP
#define RUNMANAGER_LIB_TOOLFINDER_HPP

#include "RunManagerAPI.hpp"

#include <vector>
#include "../../utilities/core/Path.hpp"
#include <QProgressDialog>
#include "ConfigOptions.hpp"

namespace openstudio {
namespace runmanager {


  /// Class for finding Tools installed on the local system.
  ///
  /// Used internally by openstudio::runmanager::ConfigOptions::findTools and
  /// openstudio::runmanager::ConfigOptions::fastFindEnergyPlus.
  class RUNMANAGER_API ToolFinder
  {
    public:
      /// Default constructor
      ToolFinder();

      /// Constructor taking a vector of paths to search for tools
      ToolFinder(const std::vector<openstudio::path> &t_searchPaths);

      /// Adds a search path
      void addSearchPath(const openstudio::path &t_searchPath);


      /// Static convenience function that searches for tools in the given set of locations
      ///
      /// \param[in] t_searchPaths paths to search for known tools
      /// \param[in] t_showProgressDialog if true, display a progress dialog during searching
      static std::vector<std::pair<ToolVersion, ToolLocationInfo> > findTools(const std::vector<openstudio::path> &t_searchPaths, bool t_showProgressDialog = false);


      /// Searches for tools installed at the already defined search paths
      ///
      /// \param[in] t_showProgressDialog if true, display a progress dialog during searching
      ///
      /// \sa openstudio::runmanager::ToolFinder::addSearchPath
      std::vector<std::pair<ToolVersion, ToolLocationInfo> > findTools(bool t_showProgressDialog=false) const;

      static void mergeTools(std::vector<std::pair<ToolVersion, ToolLocationInfo> > &t_tools,
        const std::vector<std::pair<ToolVersion, ToolLocationInfo> > &t_newtools);

      static ToolVersion parseToolVersion(const openstudio::path &t_path);

    private:
      REGISTER_LOGGER("openstudio.runmanager.ToolFinder");

      /// \returns true if the file is both accessible and exists
      static bool safeExists(const openstudio::path &t_path);

      std::vector<openstudio::path> findExecutables(const openstudio::path &t_path, const std::vector<std::string> &t_names,  const std::shared_ptr<QProgressDialog> &t_dlg,
        std::map<openstudio::path, int> &t_searchedPaths) const;
      std::vector<openstudio::path> findExecutables(const std::vector<openstudio::path> &t_searchPaths, const std::vector<std::string> &t_names, bool t_showProgressDialog) const;

      // Returns true if any part of the path matches the given regular expression
      static bool subPathMatch(const openstudio::path &t_path, const boost::regex &t_regex);

      static void mergeTool(std::vector<std::pair<ToolVersion, ToolLocationInfo> > &t_tools,
          const std::pair<ToolVersion, ToolLocationInfo> &t_tool);
      static std::vector<std::pair<ToolVersion, ToolLocationInfo> > filterTools(
          const std::vector<std::pair<ToolVersion, ToolLocationInfo> > &t_tools);
      std::string shortenPath(const openstudio::path &t_path) const;

      std::vector<openstudio::path> m_searchPaths;



  };


}
}

#endif // RUNMANAGER_LIB_TOOLFINDER_HPP

