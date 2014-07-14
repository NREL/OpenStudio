/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

