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

#ifndef ANALYSIS_RUBYCONTINUOUSVARIABLE_HPP
#define ANALYSIS_RUBYCONTINUOUSVARIABLE_HPP

#include "AnalysisAPI.hpp"
#include "ContinuousVariable.hpp"

namespace openstudio {

namespace ruleset {
  class OSArgument;
}

namespace analysis {

class RubyMeasure;

namespace detail {

  class RubyContinuousVariable_Impl;
  class RubyMeasure_Impl;

} // detail

/** RubyContinuousVariable is a ContinuousVariable. */
class ANALYSIS_API RubyContinuousVariable : public ContinuousVariable {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Construct from argument and measure. The RubyMeasure should not yet have argument
   *  set. It should have all fixed (non-variable) arguments set. */
  RubyContinuousVariable(const std::string& name,
                         const ruleset::OSArgument& argument,
                         const RubyMeasure& measure);

  /** Constructor provided for deserialization; not for general use. */
  RubyContinuousVariable(const UUID& uuid,
                         const UUID& versionUUID,
                         const std::string& name,
                         const std::string& displayName,
                         const std::string& description,
                         const boost::optional<UncertaintyDescription>& udesc,
                         boost::optional<double> minimum,
                         boost::optional<double> maximum,
                         boost::optional<double> increment,
                         boost::optional<int> nSteps,
                         const ruleset::OSArgument& argument,
                         const RubyMeasure& measure);

  virtual ~RubyContinuousVariable() {}

  //@}
  /** @name Getters and Queries */
  //@{

  ruleset::OSArgument argument() const;

  RubyMeasure measure() const;

  /** \deprecated */
  RubyMeasure perturbation() const;

  //@}
  /** @name Setters */
  //@{

  void setArgument(const ruleset::OSArgument& argument);

  bool setRubyMeasure(const RubyMeasure& measure);

  /** \deprecated */
  bool setRubyPerturbation(const RubyMeasure& measure);

  //@}
 protected:
  /// @cond
  typedef detail::RubyContinuousVariable_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::RubyContinuousVariable_Impl;

  explicit RubyContinuousVariable(std::shared_ptr<detail::RubyContinuousVariable_Impl> impl);

  friend class detail::RubyMeasure_Impl;

  bool fileTypesAreCompatible(const RubyMeasure& childRubyMeasure,
                              const boost::optional<FileReferenceType>& proposedInputFileType,
                              const boost::optional<FileReferenceType>& proposedOutputFileType) const;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.analysis.RubyContinuousVariable");
};

/** \relates RubyContinuousVariable*/
typedef boost::optional<RubyContinuousVariable> OptionalRubyContinuousVariable;

/** \relates RubyContinuousVariable*/
typedef std::vector<RubyContinuousVariable> RubyContinuousVariableVector;

} // analysis
} // openstudio

#endif // ANALYSIS_RUBYCONTINUOUSVARIABLE_HPP

