/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_FILETYPES_STANDARDSJSON_IMPL_HPP
#define UTILITIES_FILETYPES_STANDARDSJSON_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "RunOptions.hpp"

#include "../core/Logger.hpp"

#include <nano/nano_signal_slot.hpp>
#include <json/json.h>

namespace openstudio {

class StandardsJSON;

namespace detail {

  class UTILITIES_API StandardsJSON_Impl
  {
   public:
    StandardsJSON_Impl();

    StandardsJSON_Impl(const std::string& s);

    StandardsJSON clone() const;

    std::string string() const;

    boost::optional<Json::Value> getPrimaryKey(const std::string& primaryKey) const;

    /** A crude schema validation, will only check the first entry */
    bool isValidStandardsJSON() const;

   private:
    REGISTER_LOGGER("openstudio.StandardsJSON");

    // Store the entire standards JSON that is passed
    Json::Value m_standardsRoot;
  };

}  // namespace detail
}  // namespace openstudio

#endif  //UTILITIES_FILETYPES_STANDARDSJSON_IMPL_HPP
