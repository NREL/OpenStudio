/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
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

    virtual std::string string() const = 0;

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

    virtual std::string string() const override;

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
