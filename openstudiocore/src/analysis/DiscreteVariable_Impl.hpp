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

#ifndef ANALYSIS_DISCRETEVARIABLE_IMPL_HPP
#define ANALYSIS_DISCRETEVARIABLE_IMPL_HPP

#include "AnalysisAPI.hpp"
#include "InputVariable_Impl.hpp"

namespace openstudio {
namespace analysis {

class DiscreteVariable;

namespace detail {

  /** DiscreteVariable_Impl is an InputVariable_Impl that is the implementation class
   *  for DiscreteVariable. */
  class ANALYSIS_API DiscreteVariable_Impl : public InputVariable_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    DiscreteVariable_Impl(const std::string& name);

    /** Constructor provided for deserialization; not for general use. */
    DiscreteVariable_Impl(const UUID& uuid,
                          const UUID& versionUUID,
                          const std::string& name,
                          const std::string& displayName,
                          const std::string& description,
                          const boost::optional<UncertaintyDescription>& udesc);

    DiscreteVariable_Impl(const DiscreteVariable_Impl& other);

    virtual ~DiscreteVariable_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    /** Get the variable value from a dataPoint. */
    virtual double getValue(const DataPoint& dataPoint) const override;

    /** Returns true if udesc is valid for this Variable. If returns false,
     *  setUncertaintyDescription will fail. */
    virtual bool isValid(const UncertaintyDescription& udesc) const override;

    //@}
    /** @name Getters and Queries */
    //@{

    /** Returns the valid integer values for this variable. (All discrete variables
     *  are mappable to integers. Some discrete variables allow users to downselect from
     *  their overall range.) */
    virtual std::vector<int> validValues(bool selectedOnly) const = 0;

    int numValidValues(bool selectedOnly) const;

    //@}
    /** @name Serialization */
    //@{

    virtual QVariant toServerFormulationVariant() const override;

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.analysis.DiscreteVariable");
  };

} // detail

} // model
} // openstudio

#endif // ANALYSIS_DISCRETEVARIABLE_IMPL_HPP
