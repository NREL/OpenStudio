/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef INITRUBYBINDINGS_HPP
#define INITRUBYBINDINGS_HPP

#include "../../src/utilities/core/Filesystem.hpp"

#include <string>

namespace openstudio {
namespace ruby {

  void initRubyBindings();
  void initBasicRubyBindings();
  void initExtendedRubyBindings();
  void setupEmbeddedGems(const std::vector<openstudio::path>& includeDirs, const std::vector<openstudio::path>& gemPathDirs,
                         const openstudio::path& gemHomeDir, const openstudio::path& bundleGemFilePath, const openstudio::path& bundleGemDirPath,
                         const std::string& bundleWithoutGroups);

}  // namespace ruby
}  // namespace openstudio

#endif  // INITRUBYBINDINGS_HPP
