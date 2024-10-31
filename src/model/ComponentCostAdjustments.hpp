/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~ComponentCostAdjustments() override = default;
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
