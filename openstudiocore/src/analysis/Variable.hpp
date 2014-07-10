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

#ifndef ANALYSIS_VARIABLE_HPP
#define ANALYSIS_VARIABLE_HPP

#include "AnalysisAPI.hpp"
#include "AnalysisObject.hpp"

namespace openstudio {
namespace analysis {

class DataPoint;
class LinearFunction;

namespace detail {

  class Function_Impl;
  class LinearFunction_Impl;
  class Variable_Impl;

} // detail

/** Variable is an AnalysisObject that serves as a base class for InputVariable and
 *  OutputVariable. */
class ANALYSIS_API Variable : public AnalysisObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~Variable() {}

  //@}
  /** @name Getters and Queries */
  //@{

  // TODO - Think about generalizing this to return an Attribute. Then we could more properly
  // handle integer values and Quantities.
  /** Get the variable value from a dataPoint. Throws an openstudio::Exception if the Variable
   *  cannot be evaluated for dataPoint. */
  double getValue(const DataPoint& dataPoint) const;

  //@}
 protected:
  /// @cond
  typedef detail::Variable_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;

  explicit Variable(std::shared_ptr<detail::Variable_Impl> impl);

  friend class detail::Function_Impl; // calls Variable::onChange
  friend class LinearFunction; // calls Variable::setParent
  friend class detail::LinearFunction_Impl; // calls Variable::setParent
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.analysis.Variable");
};

/** \relates Variable*/
typedef boost::optional<Variable> OptionalVariable;

/** \relates Variable*/
typedef std::vector<Variable> VariableVector;

} // analysis
} // openstudio

#endif // ANALYSIS_VARIABLE_HPP

