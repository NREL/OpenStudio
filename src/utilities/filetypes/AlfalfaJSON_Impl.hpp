#ifndef UTILITIES_FILETYPES_ALFALFAJSON_IMPL_HPP
#define UTILITIES_FILETYPES_ALFALFAJSON_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Path.hpp"
#include "../alfalfa/AlfalfaPoint.hpp"

#include <vector>

namespace openstudio {
namespace alfalfa {
namespace detail {
  class UTILITIES_API AlfalfaJSON_Impl
  {
    public:
      AlfalfaJSON_Impl();

      AlfalfaJSON_Impl(const std::string& s);

      AlfalfaJSON_Impl(const openstudio::path& p);

      bool save() const;

      bool saveAs(const openstudio::path& p);

      void exposePoint(const AlfalfaPoint& point);

      std::vector<AlfalfaPoint> getPoints();

    private:
      openstudio::path m_JSONPath;

      std::vector<AlfalfaPoint> m_points;

      // configure logging
      REGISTER_LOGGER("openstudio.AlfalfaJSON");
  };
}
}
}

#endif
