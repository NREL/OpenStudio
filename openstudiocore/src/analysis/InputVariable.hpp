/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef ANALYSIS_INPUTVARIABLE_HPP
#define ANALYSIS_INPUTVARIABLE_HPP

#include "AnalysisAPI.hpp"
#include "Variable.hpp"

#include "../utilities/core/Path.hpp"

#include <QVariant>

namespace openstudio {

class FileReferenceType;

namespace runmanager {
  class WorkItem;
}

namespace analysis {

class UncertaintyDescription;
class WorkflowStep;

namespace detail {
  class InputVariable_Impl;
  class Problem_Impl;
  class WorkflowStep_Impl;
}

class ANALYSIS_API InputVariable : public Variable {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~InputVariable() {}

  //@}
  /** @name Getters and Queries */
  //@{

  boost::optional<FileReferenceType> inputFileType() const;

  boost::optional<FileReferenceType> outputFileType() const;

  // ETH@20130215 - Might be appropriate to move this up to Variable if at some point
  // OutputVariables can return UQ results this way.
  boost::optional<UncertaintyDescription> uncertaintyDescription() const;

  /** Returns true if value is valid for this Variable. If returns false, createWorkItem(value)
   *  should throw. */
  bool isValid(const QVariant& value) const;

  /** Returns true if udesc is valid for this Variable. If returns false,
   *  setUncertaintyDescription(udesc) will fail. */
  bool isValid(const UncertaintyDescription& udesc) const;

  WorkflowStep asWorkflowStep() const;

  //@}
  /** @name Setters */
  //@{

  bool setUncertaintyDescription(const UncertaintyDescription& udesc);

  void resetUncertaintyDescription();

  //@}
  /** @name Actions */
  //@{

  /** Create a runmanager::WorkItem for instantiating this Variable at value. Will throw an
   *  openstudio::Exception if value is invalid for the variable. */
  runmanager::WorkItem createWorkItem(const QVariant& value,
                                      const openstudio::path& rubyIncludeDirectory) const;

  //@}
 protected:
  /// @cond
  typedef detail::InputVariable_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;

  explicit InputVariable(std::shared_ptr<detail::InputVariable_Impl> impl);

  friend class detail::InputVariable_Impl; // calls setParent
  friend class WorkflowStep; // calls setParent
  friend class detail::WorkflowStep_Impl; // calls onChange
  friend class detail::Problem_Impl; // calls toServerFormulationVariant
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.analysis.InputVariable");
};

/** \relates InputVariable */
typedef boost::optional<InputVariable> OptionalInputVariable;

/** \relates InputVariable */
typedef std::vector<InputVariable> InputVariableVector;

} // analysis
} // openstudio

#endif // ANALYSIS_INPUTVARIABLE_HPP

