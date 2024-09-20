#ifndef UTILITIES_FILETYPES_ALFALFAJSON_IMPL_HPP
#define UTILITIES_FILETYPES_ALFALFAJSON_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "../alfalfa/AlfalfaPoint.hpp"
#include "../core/Path.hpp"

#include <vector>
#include <json/json.h>

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

      Json::Value toJSON() const;

      void exposePoint(const AlfalfaPoint& point);

      std::vector<AlfalfaPoint> getPoints();

     private:
      openstudio::path m_JSONPath;

      std::vector<AlfalfaPoint> m_points;

      // configure logging
      REGISTER_LOGGER("openstudio.AlfalfaJSON");
    };
  }  // namespace detail
}  // namespace alfalfa
}  // namespace openstudio

#endif
