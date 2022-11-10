#ifndef WORKFLOW_UTIL_HPP
#define WORKFLOW_UTIL_HPP

#include "../utilities/core/Filesystem.hpp"

namespace openstudio {

namespace model {
  class Model;
}
class Workspace;

namespace workflow {

  namespace util {

    model::Model loadOSM(const openstudio::filesystem::path& osmPath);
    Workspace loadIDF(const openstudio::filesystem::path& idfPath);

    void gatherReports(const openstudio::filesystem::path& runDirPath, const openstudio::filesystem::path& rootDirPath);

    // Cleans up the run directory (remove epw, .mtr)
    void cleanup(const openstudio::filesystem::path& runDirPath);

  }  // namespace util
}  // namespace workflow
}  // namespace openstudio

#endif  // WORKFLOW_UTIL_HPP