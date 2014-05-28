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

#ifndef ANALYSIS_OUTPUTATTRIBUTEVARIABLE_HPP
#define ANALYSIS_OUTPUTATTRIBUTEVARIABLE_HPP

#include "AnalysisAPI.hpp"
#include "OutputVariable.hpp"

namespace openstudio {
namespace analysis {

namespace detail {

  class OutputAttributeVariable_Impl;

} // detail

/** OutputAttributeVariable is an OutputVariable that accesses attributes written to
 *  XML as part of a simulation post-processing step. For instance, add an EnergyPlusPostProcess job
 *  or one of the Ruby post-process scripts (in ruby/openstudio/runmanager/rubyscripts) to your
 *  Problem's workflow and take a look at the resulting report.xml files. The value of an
 *  OutputAttributeVariable can be retrieved, but not set, and it cannot be retrieved until
 *  the DataPoint of interest has been successfully simulated. Therefore,
 *  \link OutputAttributeVariable OutputAttributeVariables \endlink can be used in
 *  \link Function Functions \endlink, but not in Problem workflows. */
class ANALYSIS_API OutputAttributeVariable : public OutputVariable {
 public:
  /** @name Constructors and Destructors */
  //@{

  OutputAttributeVariable(const std::string& name,
                          const std::string& attributeName);

  /** Constructor provided for deserialization; not for general use. */
  OutputAttributeVariable(const UUID& uuid,
                          const UUID& versionUUID,
                          const std::string& name,
                          const std::string& displayName,
                          const std::string& description,
                          const std::string& attributeName);

  virtual ~OutputAttributeVariable() {}

  //@}
  /** @name Getters and Queries */
  //@{

  std::string attributeName() const;

  //@}
  /** @name Setters */
  //@{

  void setAttributeName(const std::string& attributeName);

  //@}
 protected:
  /// @cond
  typedef detail::OutputAttributeVariable_Impl ImplType;

  friend class AnalysisObject;
  friend class detail::AnalysisObject_Impl;
  friend class detail::OutputAttributeVariable_Impl;

  explicit OutputAttributeVariable(std::shared_ptr<detail::OutputAttributeVariable_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.analysis.OutputAttributeVariable");
};

/** \relates OutputAttributeVariable*/
typedef boost::optional<OutputAttributeVariable> OptionalOutputAttributeVariable;

/** \relates OutputAttributeVariable*/
typedef std::vector<OutputAttributeVariable> OutputAttributeVariableVector;

/** \relates OutputAttributeVariable \deprecated */
typedef OutputAttributeVariable OutputAttributeContinuousVariable;

/** \relates OutputAttributeVariable \deprecated */
typedef boost::optional<OutputAttributeVariable> OptionalOutputAttributeContinuousVariable;

/** \relates OutputAttributeVariable \deprecated */
typedef std::vector<OutputAttributeVariable> OutputAttributeContinuousVariableVector;

} // analysis
} // openstudio

#endif // ANALYSIS_OUTPUTATTRIBUTEVARIABLE_HPP
