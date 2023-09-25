/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_FILETYPES_WORKFLOWSTEP_IMPL_HPP
#define UTILITIES_FILETYPES_WORKFLOWSTEP_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "WorkflowStepResult.hpp"

#include "../core/Logger.hpp"
#include "../core/Path.hpp"
#include "../data/Variant.hpp"
#include "../bcl/BCLMeasure.hpp"

#include <json/json.h>

#include <nano/nano_signal_slot.hpp>

namespace openstudio {
namespace detail {

  class UTILITIES_API WorkflowStep_Impl
  {
   public:
    WorkflowStep_Impl();

    virtual ~WorkflowStep_Impl() = default;

    std::string string() const;
    virtual Json::Value toJSON() const = 0;

    boost::optional<WorkflowStepResult> result() const;

    void setResult(const WorkflowStepResult& result);

    void resetResult();

    // Emitted on any change
    Nano::Signal<void()> onChange;

   protected:
    void onUpdate();

   private:
    // configure logging
    REGISTER_LOGGER("openstudio.WorkflowStep");

    boost::optional<WorkflowStepResult> m_result;
  };

  class UTILITIES_API MeasureStep_Impl : public WorkflowStep_Impl
  {
   public:
    MeasureStep_Impl(const std::string& measureDirName);

    virtual Json::Value toJSON() const override;

    std::string measureDirName() const;
    bool setMeasureDirName(const std::string& measureDirName);

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
    void setArgument(const std::string& name, const std::string& value);

    void removeArgument(const std::string& name);

    void clearArguments();

   private:
    // configure logging
    REGISTER_LOGGER("openstudio.MeasureStep");

    std::string m_measureDirName;
    boost::optional<std::string> m_name;
    boost::optional<std::string> m_description;
    boost::optional<std::string> m_modelerDescription;
    std::map<std::string, Variant> m_arguments;
  };

}  // namespace detail
}  // namespace openstudio

#endif  //UTILITIES_FILETYPES_WORKFLOWSTEP_IMPL_HPP
