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

#ifndef ANALYSIS_DISCRETEVARIABLE_HPP
#define ANALYSIS_DISCRETEVARIABLE_HPP

#include "AnalysisAPI.hpp"
#include "InputVariable.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  class DiscreteVariable_Impl;

} // detail

/** A DiscreteVariable is an InputVariable whose value is a finite list of values
 *  mappable to a range of integers. */
class ANALYSIS_API DiscreteVariable : public InputVariable {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~DiscreteVariable() {}

  //@}
  /** @name Getters and Queries */
  //@{

  /** Returns the valid integer values for this variable. (All discrete variables
   *  are mappable to integers. Some discrete variables allow users to downselect from
   *  their overall range.) */
  std::vector<int> validValues(bool selectedOnly) const;

  int numValidValues(bool selectedOnly) const;

  //@}
 protected:
  /// @cond
  typedef detail::DiscreteVariable_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;

  explicit DiscreteVariable(std::shared_ptr<detail::DiscreteVariable_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.analysis.DiscreteVariable");
};

/** \relates DiscreteVariable */
typedef boost::optional<DiscreteVariable> OptionalDiscreteVariable;

/** \relates DiscreteVariable */
typedef std::vector<DiscreteVariable> DiscreteVariableVector;

} // analysis
} // openstudio

#endif // ANALYSIS_DISCRETEVARIABLE_HPP
