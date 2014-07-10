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

#ifndef ANALYSIS_OUTPUTVARIABLE_HPP
#define ANALYSIS_OUTPUTVARIABLE_HPP

#include "AnalysisAPI.hpp"
#include "Variable.hpp"

namespace openstudio {
namespace analysis {

namespace detail {
  class OutputVariable_Impl;
} // detail

class ANALYSIS_API OutputVariable : public Variable {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~OutputVariable() {}

  //@}
 protected:
  /// @cond
  typedef detail::OutputVariable_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;

  explicit OutputVariable(std::shared_ptr<detail::OutputVariable_Impl> impl);
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.analysis.OutputVariable");
};

/** \relates OutputVariable */
typedef boost::optional<OutputVariable> OptionalOutputVariable;

/** \relates OutputVariable */
typedef std::vector<OutputVariable> OutputVariableVector;

} // analysis
} // openstudio

#endif // ANALYSIS_OUTPUTVARIABLE_HPP

