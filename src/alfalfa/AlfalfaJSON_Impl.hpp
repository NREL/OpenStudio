/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_FILETYPES_ALFALFAJSON_IMPL_HPP
#define UTILITIES_FILETYPES_ALFALFAJSON_IMPL_HPP

#include "AlfalfaAPI.hpp"

#include "AlfalfaPoint.hpp"

#include "../utilities/core/Path.hpp"

#include <vector>
#include <json/json.h>

namespace openstudio {
namespace alfalfa {
  namespace detail {
    class ALFALFA_API AlfalfaJSON_Impl
    {
     public:
      AlfalfaJSON_Impl();

      AlfalfaJSON_Impl(const openstudio::path& p);

      bool save() const;

      bool saveAs(const openstudio::path& p);

      void setJSONPath(const openstudio::path& p);

      Json::Value toJSON() const;

      void exposePoint(const AlfalfaPoint& point);

      std::vector<AlfalfaPoint> points() const;

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
