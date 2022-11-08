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

    model::Model loadOSM(const openstudio::filesystem::path& osm_path);
    Workspace loadIDF(const openstudio::filesystem::path& idf_path);

  }  // namespace util
}  // namespace workflow
}  // namespace openstudio

#endif  // WORKFLOW_UTIL_HPP
