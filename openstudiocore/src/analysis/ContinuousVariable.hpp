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

#ifndef ANALYSIS_CONTINUOUSVARIABLE_HPP
#define ANALYSIS_CONTINUOUSVARIABLE_HPP

#include "AnalysisAPI.hpp"
#include "InputVariable.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  class ContinuousVariable_Impl;

} // detail

/** A ContinuousVariable is an InputVariable whose value is a real number, perhaps
 *  restricted to lie within some bounds. */
class ANALYSIS_API ContinuousVariable : public InputVariable {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~ContinuousVariable() {}

  //@}
  /** @name Getters and Queries */
  //@{

  /** Returns the minimum, or lower bound, on the ContinuousVariable, if it exists. */
  boost::optional<double> minimum() const;

  /** Returns the maximum, or upper bound, on the ContinuousVariable, if it exists. */
  boost::optional<double> maximum() const;

  /** Returns the increment to be applied to this ContinuousVariable when using minimum() and
   *  maximum() to create discrete values. */
  boost::optional<double> increment() const;

  /** Returns the number of steps to be used with minimum() and maximum() to create discrete
   *  values. */
  boost::optional<int> nSteps() const;

  /** Returns true if value in [minimum,maximum]. */
  bool isFeasible(double value) const;

  /** Returns a vector of incremental values, if minimum() && maximum() &&
   *  (increment() || nSteps()). There are two exceptional cases. If minimum() > maximum(),
   *  then an empty vector is returned. If equal(minimum(),maximum()), then a single value
   *  is returned. */
  std::vector<double> incrementalValues() const;

  //@}
  /** @name Setters */
  //@{

  void setMinimum(double minimum);

  void setMaximum(double maximum);

  /** Sets increment and nullifies nSteps. */
  void setIncrement(double increment);

  /** Sets nSteps and nullifies increment. */
  void setNSteps(int nSteps);

  void clearMinimum();

  void clearMaximum();

  void clearIncrement();

  void clearNSteps();

  //@}
  /** @name Actions */
  //@{

  /** Truncates value to [minimum,maximum]. If minimum > maximum, returns boost::none. */
  boost::optional<double> truncate(double value) const;

  //@}
 protected:
  /// @cond
  typedef detail::ContinuousVariable_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;

  explicit ContinuousVariable(std::shared_ptr<detail::ContinuousVariable_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.analysis.ContinuousVariable");
};

/** \relates ContinuousVariable */
typedef boost::optional<ContinuousVariable> OptionalContinuousVariable;

/** \relates ContinuousVariable */
typedef std::vector<ContinuousVariable> ContinuousVariableVector;

} // analysis
} // openstudio

#endif // ANALYSIS_CONTINUOUSVARIABLE_HPP
