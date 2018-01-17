/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

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
