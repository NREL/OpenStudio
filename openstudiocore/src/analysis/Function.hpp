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

#ifndef ANALYSIS_FUNCTION_HPP
#define ANALYSIS_FUNCTION_HPP

#include "AnalysisAPI.hpp"
#include "AnalysisObject.hpp"

namespace openstudio {
namespace analysis {

class Variable;
class DataPoint;
class Problem;
class OptimizationProblem;

namespace detail {

  class Function_Impl;
  class Problem_Impl;
  class OptimizationProblem_Impl;

} // detail

/** Function is an abstract AnalysisObject that registers a set of \link Variable Variables
 *  \endlink whose values can be retrieved from a DataPoint, and requires classes derived from it
 *  to implement Function::getValue. Function values are generally evaluated after a DataPoint
 *  is simulated, and may be used to direct the course of an Algorithm. */
class ANALYSIS_API Function : public AnalysisObject {
 public:
  /** @name Constructors */
  //@{

  virtual ~Function() {}

  //@}
  /** @name Getters */
  //@{

  std::vector<Variable> variables() const;

  double getValue(const DataPoint& dataPoint) const;

  //@}
 protected:
  /// @cond
  typedef detail::Function_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;

  explicit Function(std::shared_ptr<detail::Function_Impl> impl);

  friend class Problem; // to call setParent
  friend class OptimizationProblem; // to call setParent
  friend class detail::Problem_Impl; // to call setParent
  friend class detail::OptimizationProblem_Impl; // to call setParent
  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.Function");
};

/** \relates Function*/
typedef boost::optional<Function> OptionalFunction;

/** \relates Function*/
typedef std::vector<Function> FunctionVector;

} // analysis
} // openstudio

#endif // ANALYSIS_FUNCTION_HPP
