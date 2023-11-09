/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_FILETYPES_RUNOPTIONS_HPP
#define UTILITIES_FILETYPES_RUNOPTIONS_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Logger.hpp"
#include "../core/Deprecated.hpp"

#include <json/json.h>

namespace openstudio {

class ForwardTranslatorOptions;

namespace detail {
  class WorkflowJSON_Impl;
  class RunOptions_Impl;
  class ForwardTranslatorOptions_Impl;
}  // namespace detail

class UTILITIES_API CustomOutputAdapter
{
 public:
  // Couldn't add both ctors, it's ambiguous... so retaining existing as a static factory fromString
  static CustomOutputAdapter fromString(std::string customFileName, std::string className, const std::string& options);
  explicit CustomOutputAdapter(std::string customFileName, std::string className, Json::Value options = Json::nullValue);

  std::string customFileName() const;
  std::string className() const;
  std::string options() const;
  Json::Value optionsJSON() const;

  Json::Value toJSON() const;

 private:
  REGISTER_LOGGER("openstudio.CustomOutputAdapter");

  std::string m_customFileName;
  std::string m_className;
  Json::Value m_options;
};

/** Base class for defining a run options for a OpenStudio Workflow. */
class UTILITIES_API RunOptions
{
 public:
  RunOptions();

  // TODO: uneeded
  virtual ~RunOptions();

  /// Construct from JSON formatted string
  static boost::optional<RunOptions> fromString(const std::string& s);

  Json::Value toJSON() const;

  /// Serialize to JSON formatted string
  std::string string() const;

  bool debug() const;
  bool isDebugDefaulted() const;
  bool setDebug(bool debug);
  void resetDebug();

  bool epjson() const;
  bool isEpjsonDefaulted() const;
  bool setEpjson(bool epjson);
  void resetEpjson();

  bool fast() const;
  bool isFastDefaulted() const;
  bool setFast(bool fast);
  void resetFast();

  bool preserveRunDir() const;
  bool isPreserveRunDirDefaulted() const;
  bool setPreserveRunDir(bool preserveRunDir);
  void resetPreserveRunDir();

  bool skipExpandObjects() const;
  bool isSkipExpandObjectsDefaulted() const;
  bool setSkipExpandObjects(bool skipExpandObjects);
  void resetSkipExpandObjects();

  bool skipEnergyPlusPreprocess() const;
  bool isSkipEnergyPlusPreprocessDefaulted() const;
  bool setSkipEnergyPlusPreprocess(bool skipEnergyPlusPreprocess);
  void resetSkipEnergyPlusPreprocess();

  bool cleanup() const;
  bool isCleanupDefaulted() const;
  bool setCleanup(bool cleanup);
  void resetCleanup();

  boost::optional<CustomOutputAdapter> customOutputAdapter() const;
  bool setCustomOutputAdapter(const CustomOutputAdapter& adapter);
  void resetCustomOutputAdapter();

  OS_DEPRECATED(3, 6, 0) std::string forwardTranslateOptions() const;
  OS_DEPRECATED(3, 6, 0) bool setForwardTranslateOptions(const std::string& options);
  OS_DEPRECATED(3, 6, 0) void resetForwardTranslateOptions();

  ForwardTranslatorOptions forwardTranslatorOptions() const;
  bool setForwardTranslatorOptions(const ForwardTranslatorOptions& forwardTranslatorOptions);
  void resetForwardTranslatorOptions();

  /* Any non-defaulted value from other is brought over */
  void overrideValuesWith(const RunOptions& other);

 protected:
  // get the impl
  template <typename T>
  std::shared_ptr<T> getImpl() const {
    return std::dynamic_pointer_cast<T>(m_impl);
  }

  friend class detail::WorkflowJSON_Impl;

 private:
  // configure logging
  REGISTER_LOGGER("openstudio.RunOptions");

  // pointer to implementation
  std::shared_ptr<detail::RunOptions_Impl> m_impl;
};

UTILITIES_API std::ostream& operator<<(std::ostream& os, const RunOptions& runOptions);

}  // namespace openstudio

#endif  //UTILITIES_FILETYPES_RUNOPTIONS_HPP
