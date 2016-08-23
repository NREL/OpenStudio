/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef UTILITIES_FILETYPES_WORKFLOWSTEP_IMPL_HPP
#define UTILITIES_FILETYPES_WORKFLOWSTEP_IMPL_HPP

#include "../UtilitiesAPI.hpp"

#include "WorkflowStepResult.hpp"

#include "../core/Logger.hpp"
#include "../core/Path.hpp"
#include "../data/Variant.hpp"

#include <jsoncpp/json.h>

// DLM: this should be somewhere different
#include <model/nano_signal_slot.hpp> 

namespace openstudio{
namespace detail {

  class UTILITIES_API WorkflowStep_Impl
  {
  public:

    WorkflowStep_Impl();

    virtual ~WorkflowStep_Impl();

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

    virtual std::string string() const;

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

} // detail
} // openstudio

#endif //UTILITIES_FILETYPES_WORKFLOWSTEP_IMPL_HPP
