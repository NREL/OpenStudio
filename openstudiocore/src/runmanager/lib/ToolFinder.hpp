/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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

