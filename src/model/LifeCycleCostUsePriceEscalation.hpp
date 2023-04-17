/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_LIFECYCLECOSTUSEPRICEESCALATION_HPP
#define MODEL_LIFECYCLECOSTUSEPRICEESCALATION_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class LifeCycleCostUsePriceEscalation_Impl;
  }

  /** LifeCycleCostUsePriceEscalation derives from ParentObject and is an interface to the OpenStudio IDD object named "OS:LifeCycleCost:UsePriceEscalation".
 *
 *  LifeCycleCostUsePriceEscalation specifies escalation about energy and water costs separately from general inflation assumptions.
 */
  class MODEL_API LifeCycleCostUsePriceEscalation : public ParentObject
  {

   public:
    /** @name Constructors and Destructors */
    //@{
    virtual ~LifeCycleCostUsePriceEscalation() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    LifeCycleCostUsePriceEscalation(const LifeCycleCostUsePriceEscalation& other) = default;
    LifeCycleCostUsePriceEscalation(LifeCycleCostUsePriceEscalation&& other) = default;
    LifeCycleCostUsePriceEscalation& operator=(const LifeCycleCostUsePriceEscalation&) = default;
    LifeCycleCostUsePriceEscalation& operator=(LifeCycleCostUsePriceEscalation&&) = default;

    /// Constructs a new LifeCycleCostUsePriceEscalation object in the model.
    explicit LifeCycleCostUsePriceEscalation(const Model& model);

    //@}
    /** @name Getters */
    //@{

    boost::optional<std::string> resource() const;

    boost::optional<std::string> escalationStartMonth() const;

    boost::optional<int> escalationStartYear() const;

    /** Get the escalation for the year at index. Indexing starts at 0. */
    boost::optional<double> yearEscalation(unsigned index) const;

    //@}
    /** @name Setters */
    //@{

    bool setResource(const std::string& str);

    bool setEscalationStartMonth(const std::string& str);

    /** Set the escalation for the year at index. Indexing starts at 0. */
    bool setEscalationStartYear(int num);

    bool setYearEscalation(unsigned index, double num);

    //@}

    /// Returns the IddObjectType.
    static IddObjectType iddObjectType();

    /// Returns the number of years for which escalation data is specified, e.g. the number of extensible fields.
    unsigned numYears() const;

   protected:
    /// @cond

    using ImplType = detail::LifeCycleCostUsePriceEscalation_Impl;

    friend class Model;
    friend class IdfObject;

    // constructor
    explicit LifeCycleCostUsePriceEscalation(std::shared_ptr<detail::LifeCycleCostUsePriceEscalation_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.LifeCycleCostUsePriceEscalation");

    /// @endcond
  };

  /** \relates LifeCycleCostUsePriceEscalation */
  using OptionalLifeCycleCostUsePriceEscalation = boost::optional<LifeCycleCostUsePriceEscalation>;

  /** \relates LifeCycleCostUsePriceEscalation */
  using LifeCycleCostUsePriceEscalationVector = std::vector<LifeCycleCostUsePriceEscalation>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LIFECYCLECOSTUSEPRICEESCALATION_HPP
