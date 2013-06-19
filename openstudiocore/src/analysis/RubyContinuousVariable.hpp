/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#include <analysis/AnalysisAPI.hpp>
#include <analysis/ContinuousVariable.hpp>

namespace openstudio {

namespace ruleset {
  class OSArgument;
}

namespace analysis {

class RubyPerturbation;

namespace detail {

  class RubyContinuousVariable_Impl;
  class RubyPerturbation_Impl;

} // detail

/** RubyContinuousVariable is a ContinuousVariable. */
class ANALYSIS_API RubyContinuousVariable : public ContinuousVariable {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Construct from argument and perturbation. The RubyPerturbation should not yet have argument
   *  set. It should have all fixed (non-variable) arguments set. */
  RubyContinuousVariable(const std::string& name,
                         const ruleset::OSArgument& argument,
                         const RubyPerturbation& perturbation);

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
                         const RubyPerturbation& perturbation);

  virtual ~RubyContinuousVariable() {}

  //@}
  /** @name Getters and Queries */
  //@{

  ruleset::OSArgument argument() const;

  RubyPerturbation perturbation() const;

  //@}
  /** @name Setters */
  //@{

  void setArgument(const ruleset::OSArgument& argument);

  bool setRubyPerturbation(const RubyPerturbation& perturbation);

  //@}
 protected:
  /// @cond
  typedef detail::RubyContinuousVariable_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::RubyContinuousVariable_Impl;

  explicit RubyContinuousVariable(boost::shared_ptr<detail::RubyContinuousVariable_Impl> impl);

  friend class detail::RubyPerturbation_Impl;

  bool fileTypesAreCompatible(const RubyPerturbation& childRubyPerturbation,
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

