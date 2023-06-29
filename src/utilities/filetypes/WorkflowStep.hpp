/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_FILETYPES_WORKFLOWSTEP_HPP
#define UTILITIES_FILETYPES_WORKFLOWSTEP_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Logger.hpp"
#include "../data/Variant.hpp"

#include <json/json.h>

namespace openstudio {
namespace detail {
  class WorkflowStep_Impl;
  class WorkflowJSON_Impl;
  class MeasureStep_Impl;
}  // namespace detail

class MeasureType;
class WorkflowStepResult;

/** Base class for defining a step in an OpenStudio Workflow. */
class UTILITIES_API WorkflowStep
{
 public:
  virtual ~WorkflowStep() = default;

  /// Construct from JSON formatted string
  static boost::optional<WorkflowStep> fromString(const std::string& s);

  /// Serialize to JSON formatted string
  std::string string() const;

  /// Returns the optional WorkflowStepResult
  boost::optional<WorkflowStepResult> result() const;

  /// Sets the optional WorkflowStepResult
  void setResult(const WorkflowStepResult& result);

  /// Resets the optional WorkflowStepResult
  void resetResult();

  /// Tests pointer equality
  bool operator==(const WorkflowStep& other) const;

  /// cast to type T, can throw std::bad_cast
  template <typename T>
  T cast() const {
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (!impl) {
      throw(std::bad_cast());
    }
    return T(std::move(impl));
  }

  /// cast to optional of type T
  template <typename T>
  boost::optional<T> optionalCast() const {
    boost::optional<T> thisResult;
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (impl) {
      thisResult = T(std::move(impl));
    }
    return thisResult;
  }

 protected:
  // get the impl
  template <typename T>
  std::shared_ptr<T> getImpl() const {
    return std::dynamic_pointer_cast<T>(m_impl);
  }

  friend class detail::WorkflowStep_Impl;
  friend class detail::WorkflowJSON_Impl;

  /** Protected constructor from impl. */
  WorkflowStep(std::shared_ptr<detail::WorkflowStep_Impl> impl);

 private:
  // configure logging
  REGISTER_LOGGER("openstudio.WorkflowStep");

  // pointer to implementation
  std::shared_ptr<detail::WorkflowStep_Impl> m_impl;
};

/** Base class for defining a step in an OpenStudio Workflow. */
class UTILITIES_API MeasureStep : public WorkflowStep
{
 public:
  MeasureStep(const std::string& measureDirName);

  std::string measureDirName() const;

  boost::optional<std::string> name() const;
  bool setName(const std::string& name);
  void resetName();

  boost::optional<std::string> description() const;
  bool setDescription(const std::string& description);
  void resetDescription();

  boost::optional<std::string> modelerDescription() const;
  bool setModelerDescription(const std::string& modelerDescription);
  void resetModelerDescription();

  std::map<std::string, Variant> arguments() const;
  boost::optional<Variant> getArgument(const std::string& name) const;

  void setArgument(const std::string& name, const Variant& value);
  void setArgument(const std::string& name, bool value);
  void setArgument(const std::string& name, double value);
  void setArgument(const std::string& name, int value);
  void setArgument(const std::string& name, const char* value);
  void setArgument(const std::string& name, const std::string& value);

  void removeArgument(const std::string& name);

  void clearArguments();

 protected:
  MeasureStep(std::shared_ptr<detail::MeasureStep_Impl> impl);

  bool setMeasureDirName(const std::string& measureDirName);

 private:
  using ImplType = detail::MeasureStep_Impl;

  friend class WorkflowStep;
  friend class detail::WorkflowJSON_Impl;

  // configure logging
  REGISTER_LOGGER("openstudio.MeasureStep");
};

UTILITIES_API std::ostream& operator<<(std::ostream& os, const WorkflowStep& workflowStep);

}  // namespace openstudio

#endif  //UTILITIES_FILETYPES_WORKFLOWSTEP_HPP
