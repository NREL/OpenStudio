/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef UTILITIES_FILETYPES_WORKFLOWSTEP_HPP
#define UTILITIES_FILETYPES_WORKFLOWSTEP_HPP

#include "../UtilitiesAPI.hpp"

#include "../core/Logger.hpp"
#include "../data/Variant.hpp"
#include "../data/Attribute.hpp"

#include <jsoncpp/json.h>

namespace openstudio{
namespace detail{
  class WorkflowStep_Impl;
  class WorkflowJSON_Impl;
  class MeasureStep_Impl;
}

class MeasureType;
class WorkflowStepResult;

/** Base class for defining a step in an OpenStudio Workflow. */
class UTILITIES_API WorkflowStep
{
public:

  virtual ~WorkflowStep();

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
  template<typename T>
  T cast() const{
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (!impl){
      throw(std::bad_cast());
    }
    return T(std::move(impl));
  }

  /// cast to optional of type T
  template<typename T>
  boost::optional<T> optionalCast() const{
    boost::optional<T> result;
    std::shared_ptr<typename T::ImplType> impl = this->getImpl<typename T::ImplType>();
    if (impl){
      result = T(std::move(impl));
    }
    return result;
  }

protected:

  // get the impl
  template<typename T>
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

  typedef detail::MeasureStep_Impl ImplType;

  friend class WorkflowStep;
  friend class detail::WorkflowJSON_Impl;

  // configure logging
  REGISTER_LOGGER("openstudio.MeasureStep");

};

UTILITIES_API std::ostream& operator<<(std::ostream& os, const WorkflowStep& workflowStep);

} // openstudio

#endif //UTILITIES_FILETYPES_WORKFLOWSTEP_HPP
