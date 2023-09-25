/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_FILETYPES_STANDARDSJSON_HPP
#define UTILITIES_FILETYPES_STANDARDSJSON_HPP

#include "../UtilitiesAPI.hpp"
#include "../core/Logger.hpp"

namespace Json {
class Value;
}

namespace openstudio {

namespace detail {
  class StandardsJSON_Impl;
}

/** Class for accessing the OpenStudio Standards (OSW) JSON format. */
class UTILITIES_API StandardsJSON
{
 public:
  // TODO: JM 2018-11-09
  // Makes no sense really, providing it for SWIG only now
  StandardsJSON();

  /** Constructor with string, will throw if string is incorrect. */
  StandardsJSON(const std::string& s);

  /** Clones this StandardsJSON into a separate one. */
  StandardsJSON clone() const;

  Json::Value toJSON() const;

  /** Get the json as a string. */
  std::string string() const;

  /** Tries to get the primaryKey, checking that the resulting object is indeed an arrray */
  boost::optional<Json::Value> getPrimaryKey(const std::string& primaryKey) const;

  /** Attempt to load a StandardsJSON from string */
  static boost::optional<StandardsJSON> load(const std::string& s);

 protected:
  // get the impl
  template <typename T>
  std::shared_ptr<T> getImpl() const {
    return std::dynamic_pointer_cast<T>(m_impl);
  }

  friend class detail::StandardsJSON_Impl;

  /** Protected constructor from impl. */
  StandardsJSON(std::shared_ptr<detail::StandardsJSON_Impl> impl);

 private:
  // configure logging
  REGISTER_LOGGER("openstudio.StandardsJSON");

  // pointer to implementation
  std::shared_ptr<detail::StandardsJSON_Impl> m_impl;
};

UTILITIES_API std::ostream& operator<<(std::ostream& os, const StandardsJSON& standardsJSON);

}  // namespace openstudio

#endif  //UTILITIES_FILETYPES_STANDARDSJSON_HPP
