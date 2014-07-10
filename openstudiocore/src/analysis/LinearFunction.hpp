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

#ifndef ANALYSIS_LINEARFUNCTION_HPP
#define ANALYSIS_LINEARFUNCTION_HPP

#include "AnalysisAPI.hpp"
#include "Function.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  class LinearFunction_Impl;

} // detail

/** LinearFunction is a Function of the form a1*x1 + a2*x2 + ... + an*xn where
 *  xi is the value of a Variable at a DataPoint, and ai is a coefficient value. */
class ANALYSIS_API LinearFunction : public Function {
 public:
  /** @name Constructors */
  //@{

  /** Construct linear function of variables. Throws openstudio::Exception if
   *  !coefficients.empty() and variables.size() != coefficients.size(). */
  LinearFunction(const std::string& name,
                 const std::vector<Variable>& variables,
                 const std::vector<double>& coefficients=std::vector<double>());

  LinearFunction(const UUID& uuid,
                 const UUID& versionUUID,
                 const std::string& name,
                 const std::string& displayName,
                 const std::string& description,
                 const std::vector<Variable>& variables,
                 const std::vector<double>& coefficients);

  virtual ~LinearFunction() {}

  //@}
  /** @name Getters */
  //@{

  std::vector<double> coefficients() const;

  //@}
  /** @name Setters */
  //@{

  bool setCoefficients(const std::vector<double>& coefficients);

  //@}
 protected:
  /// @cond
  typedef detail::LinearFunction_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::LinearFunction_Impl;

  explicit LinearFunction(std::shared_ptr<detail::LinearFunction_Impl> impl);

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.analysis.LinearFunction");
};

/** \relates LinearFunction*/
typedef boost::optional<LinearFunction> OptionalLinearFunction;

/** \relates LinearFunction*/
typedef std::vector<LinearFunction> LinearFunctionVector;

} // analysis
} // openstudio

#endif // ANALYSIS_LINEARFUNCTION_HPP
