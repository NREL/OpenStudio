/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_FILETYPES_FORWARDTRANSLATOROPTIONS_HPP
#define UTILITIES_FILETYPES_FORWARDTRANSLATOROPTIONS_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Logger.hpp"

namespace Json {
class Value;
}

namespace openstudio {
namespace detail {
  class WorkflowJSON_Impl;
  class RunOptions_Impl;
  class ForwardTranslatorOptions_Impl;
}  // namespace detail

struct ForwardTranslatorOptionKeyMethod
{
  std::string json_name;
  std::string ft_method_name;
};
UTILITIES_API std::ostream& operator<<(std::ostream& out, const ForwardTranslatorOptionKeyMethod& opt);

class UTILITIES_API ForwardTranslatorOptions
{
 public:
  ForwardTranslatorOptions();

  void reset();

  /// Construct from JSON formatted string
  static boost::optional<ForwardTranslatorOptions> fromString(const std::string& s);

  Json::Value toJSON() const;

  /// Serialize to JSON formatted string
  std::string string() const;

  bool keepRunControlSpecialDays() const;
  bool isKeepRunControlSpecialDaysDefaulted() const;
  void setKeepRunControlSpecialDays(bool keepRunControlSpecialDays);
  void resetKeepRunControlSpecialDays();

  bool iPTabularOutput() const;
  bool isIPTabularOutputDefaulted() const;
  void setIPTabularOutput(bool iPTabularOutput);
  void resetIPTabularOutput();

  bool excludeLCCObjects() const;
  bool isExcludeLCCObjectsDefaulted() const;
  void setExcludeLCCObjects(bool excludeLCCObjects);
  void resetExcludeLCCObjects();

  bool excludeSQliteOutputReport() const;
  bool isExcludeSQliteOutputReportDefaulted() const;
  void setExcludeSQliteOutputReport(bool excludeSQliteOutputReport);
  void resetExcludeSQliteOutputReport();

  bool excludeHTMLOutputReport() const;
  bool isExcludeHTMLOutputReportDefaulted() const;
  void setExcludeHTMLOutputReport(bool excludeHTMLOutputReport);
  void resetExcludeHTMLOutputReport();

  bool excludeVariableDictionary() const;
  bool isExcludeVariableDictionaryDefaulted() const;
  void setExcludeVariableDictionary(bool excludeVariableDictionary);
  void resetExcludeVariableDictionary();

  bool excludeSpaceTranslation() const;
  bool isExcludeSpaceTranslationDefaulted() const;
  void setExcludeSpaceTranslation(bool excludeSpaceTranslation);
  void resetExcludeSpaceTranslation();

  /* Any non-defaulted value from other is brought over */
  void overrideValuesWith(const ForwardTranslatorOptions& other);

  // TODO: keep?
  static std::vector<ForwardTranslatorOptionKeyMethod> forwardTranslatorOptionKeyMethods();

 protected:
  static ForwardTranslatorOptions fromJSON(const Json::Value& value);

  // get the impl
  std::shared_ptr<detail::ForwardTranslatorOptions_Impl> getImpl() const {
    return m_impl;
  }

  explicit ForwardTranslatorOptions(std::shared_ptr<detail::ForwardTranslatorOptions_Impl> impl);

  friend class detail::ForwardTranslatorOptions_Impl;
  friend class RunOptions;
  friend class detail::RunOptions_Impl;

 private:
  // configure logging
  REGISTER_LOGGER("openstudio.ForwardTranslatorOptions");

  // pointer to implementation
  std::shared_ptr<detail::ForwardTranslatorOptions_Impl> m_impl;
};

UTILITIES_API std::ostream& operator<<(std::ostream& os, const ForwardTranslatorOptions& forwardTranslatorOptions);

}  // namespace openstudio

#endif  // UTILITIES_FILETYPES_FORWARDTRANSLATOROPTIONS_HPP
