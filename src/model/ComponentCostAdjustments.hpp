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

#ifndef MODEL_COMPONENTCOSTADJUSTMENTS_HPP
#define MODEL_COMPONENTCOSTADJUSTMENTS_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace model {

  namespace detail {
    class ComponentCostAdjustments_Impl;
  }

  /** ComponentCostAdjustments derives from ParentObject and is an interface to the OpenStudio IDD object named "OS:ComponentCost:Adjustments".
 *
 *  ComponentCostAdjustments is a unique object which modifies the construction costs calculated by ComponentCost_LineItem
 *  objects.
 *
 *  \todo The IDD object "OS:ComponentCost:Adjustments" should be marked as unique
 *
 */
  class MODEL_API ComponentCostAdjustments : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    virtual ~ComponentCostAdjustments() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ComponentCostAdjustments(const ComponentCostAdjustments& other) = default;
    ComponentCostAdjustments(ComponentCostAdjustments&& other) = default;
    ComponentCostAdjustments& operator=(const ComponentCostAdjustments&) = default;
    ComponentCostAdjustments& operator=(ComponentCostAdjustments&&) = default;

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> miscellaneousCostPerConditionedArea() const;

    boost::optional<double> designAndEngineeringFees() const;

    boost::optional<double> contractorFee() const;

    boost::optional<double> contingency() const;

    boost::optional<double> permitsBondingAndInsurance() const;

    boost::optional<double> commissioningFee() const;

    boost::optional<double> regionalAdjustmentFactor() const;

    //@}
    /** @name Setters */
    //@{

    bool setMiscellaneousCostPerConditionedArea(double miscellaneousCostPerConditionedArea);

    bool setDesignAndEngineeringFees(double designAndEngineeringFees);

    bool setContractorFee(double contractorFee);

    bool setContingency(double contingency);

    bool setPermitsBondingAndInsurance(double permitsBondingAndInsurance);

    bool setCommissioningFee(double commissioningFee);

    bool setRegionalAdjustmentFactor(double regionalAdjustmentFactor);

    //@}

    /// Returns the IddObjectType.
    static IddObjectType iddObjectType();

   protected:
    /// @cond

    using ImplType = detail::ComponentCostAdjustments_Impl;

    friend class Model;
    friend class IdfObject;

    /// Constructs a new ComponentCostAdjustments object in the model.
    explicit ComponentCostAdjustments(const Model& model);

    // constructor
    explicit ComponentCostAdjustments(std::shared_ptr<detail::ComponentCostAdjustments_Impl> impl);

   private:
    REGISTER_LOGGER("openstudio.model.ComponentCostAdjustments");

    /// @endcond
  };

  /** \relates ComponentCostAdjustments */
  using OptionalComponentCostAdjustments = boost::optional<ComponentCostAdjustments>;

  /** \relates ComponentCostAdjustments */
  using ComponentCostAdjustmentsVector = std::vector<ComponentCostAdjustments>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COMPONENTCOSTADJUSTMENTS_HPP
