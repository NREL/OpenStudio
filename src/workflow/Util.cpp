#include "../utilities/core/Logger.hpp"
#include "../model/Model.hpp"
#include "../utilities/idf/Workspace.hpp"
#include "../osversion/VersionTranslator.hpp"

#include <fmt/format.h>

namespace openstudio::workflow::util {

model::Model loadOSM(const openstudio::filesystem::path& osmPath) {

  LOG_FREE(Info, "openstudio.worklow.Util", "Loading OSM model");
  LOG_FREE(Info, "openstudio.worklow.Util", "Reading in OSM model " << osmPath);

  openstudio::osversion::VersionTranslator vt;
  auto m_ = vt.loadModel(osmPath);
  if (!m_) {
    throw std::runtime_error(fmt::format("Failed to load OSM file {}\n", openstudio::toString(osmPath)));
  }
  return m_.get();
}

Workspace loadIDF(const openstudio::filesystem::path& idfPath) {
  LOG_FREE(Info, "openstudio.worklow.Util", "Loading IDF workspace");
  LOG_FREE(Info, "openstudio.worklow.Util", "Reading in IDF workspace " << idfPath);

  auto idf_ = Workspace::load(idfPath);
  if (!idf_) {
    throw std::runtime_error(fmt::format("Failed to load IDF file {}\n", openstudio::toString(idfPath)));
  }
  return idf_.get();
};

}  // namespace openstudio::workflow::util
