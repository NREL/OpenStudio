/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "ComponentCostAdjustments.hpp"
#include "ComponentCostAdjustments_Impl.hpp"

#include "Model_Impl.hpp"

#include <utilities/idd/OS_ComponentCost_Adjustments_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <boost/algorithm/string.hpp>

namespace openstudio {
namespace model {
  namespace detail {

    ComponentCostAdjustments_Impl::ComponentCostAdjustments_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == ComponentCostAdjustments::iddObjectType());
    }

    ComponentCostAdjustments_Impl::ComponentCostAdjustments_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                 bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == ComponentCostAdjustments::iddObjectType());
    }

    ComponentCostAdjustments_Impl::ComponentCostAdjustments_Impl(const ComponentCostAdjustments_Impl& other, Model_Impl* model, bool keepHandle)
      : ParentObject_Impl(other, model, keepHandle) {}

    OptionalDouble ComponentCostAdjustments_Impl::miscellaneousCostPerConditionedArea() const {
      return getDouble(OS_ComponentCost_AdjustmentsFields::MiscellaneousCostperConditionedArea);
    }

    bool ComponentCostAdjustments_Impl::setMiscellaneousCostPerConditionedArea(double num) {
      return setDouble(OS_ComponentCost_AdjustmentsFields::MiscellaneousCostperConditionedArea, num);
      ;
    }

    OptionalDouble ComponentCostAdjustments_Impl::designAndEngineeringFees() const {
      return getDouble(OS_ComponentCost_AdjustmentsFields::DesignandEngineeringFees);
    }

    bool ComponentCostAdjustments_Impl::setDesignAndEngineeringFees(double num) {
      return setDouble(OS_ComponentCost_AdjustmentsFields::DesignandEngineeringFees, num);
      ;
    }

    OptionalDouble ComponentCostAdjustments_Impl::contractorFee() const {
      return getDouble(OS_ComponentCost_AdjustmentsFields::ContractorFee);
    }

    bool ComponentCostAdjustments_Impl::setContractorFee(double num) {
      return setDouble(OS_ComponentCost_AdjustmentsFields::ContractorFee, num);
      ;
    }

    OptionalDouble ComponentCostAdjustments_Impl::contingency() const {
      return getDouble(OS_ComponentCost_AdjustmentsFields::Contingency);
    }

    bool ComponentCostAdjustments_Impl::setContingency(double num) {
      return setDouble(OS_ComponentCost_AdjustmentsFields::Contingency, num);
      ;
    }

    OptionalDouble ComponentCostAdjustments_Impl::permitsBondingAndInsurance() const {
      return getDouble(OS_ComponentCost_AdjustmentsFields::PermitsBondingandInsurance);
    }

    bool ComponentCostAdjustments_Impl::setPermitsBondingAndInsurance(double num) {
      return setDouble(OS_ComponentCost_AdjustmentsFields::PermitsBondingandInsurance, num);
      ;
    }

    OptionalDouble ComponentCostAdjustments_Impl::commissioningFee() const {
      return getDouble(OS_ComponentCost_AdjustmentsFields::CommissioningFee);
    }

    bool ComponentCostAdjustments_Impl::setCommissioningFee(double num) {
      return setDouble(OS_ComponentCost_AdjustmentsFields::CommissioningFee, num);
      ;
    }

    OptionalDouble ComponentCostAdjustments_Impl::regionalAdjustmentFactor() const {
      return getDouble(OS_ComponentCost_AdjustmentsFields::RegionalAdjustmentFactor);
    }

    bool ComponentCostAdjustments_Impl::setRegionalAdjustmentFactor(double num) {
      return setDouble(OS_ComponentCost_AdjustmentsFields::RegionalAdjustmentFactor, num);
      ;
    }

    // return the parent object in the hierarchy
    boost::optional<ParentObject> ComponentCostAdjustments_Impl::parent() const {
      return {};
    }

    // set the parent, child may have to call methods on the parent
    bool ComponentCostAdjustments_Impl::setParent(ParentObject& /*newParent*/) {
      //if (newParent.optionalCast<SomeParentClass>() && (newParent.model() == model())) {
      //  return true;
      //}
      return false;
    }

    // return any children objects in the hierarchy
    std::vector<ModelObject> ComponentCostAdjustments_Impl::children() const {
      std::vector<ModelObject> result;
      return result;
    }

    std::vector<IddObjectType> ComponentCostAdjustments_Impl::allowableChildTypes() const {
      IddObjectTypeVector result;
      return result;
    }

    // Get all output variable names that could be associated with this object.
    const std::vector<std::string>& ComponentCostAdjustments_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

  }  // namespace detail

  /// constructor
  ComponentCostAdjustments::ComponentCostAdjustments(const Model& model) : ParentObject(ComponentCostAdjustments::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::ComponentCostAdjustments_Impl>());
  }

  // constructor
  ComponentCostAdjustments::ComponentCostAdjustments(std::shared_ptr<detail::ComponentCostAdjustments_Impl> impl) : ParentObject(std::move(impl)) {}

  OptionalDouble ComponentCostAdjustments::miscellaneousCostPerConditionedArea() const {
    return getImpl<detail::ComponentCostAdjustments_Impl>()->miscellaneousCostPerConditionedArea();
  }

  bool ComponentCostAdjustments::setMiscellaneousCostPerConditionedArea(double num) {
    return getImpl<detail::ComponentCostAdjustments_Impl>()->setMiscellaneousCostPerConditionedArea(num);
  }

  OptionalDouble ComponentCostAdjustments::designAndEngineeringFees() const {
    return getImpl<detail::ComponentCostAdjustments_Impl>()->designAndEngineeringFees();
  }

  bool ComponentCostAdjustments::setDesignAndEngineeringFees(double num) {
    return getImpl<detail::ComponentCostAdjustments_Impl>()->setDesignAndEngineeringFees(num);
  }

  OptionalDouble ComponentCostAdjustments::contractorFee() const {
    return getImpl<detail::ComponentCostAdjustments_Impl>()->contractorFee();
  }

  bool ComponentCostAdjustments::setContractorFee(double num) {
    return getImpl<detail::ComponentCostAdjustments_Impl>()->setContractorFee(num);
  }

  OptionalDouble ComponentCostAdjustments::contingency() const {
    return getImpl<detail::ComponentCostAdjustments_Impl>()->contingency();
  }

  bool ComponentCostAdjustments::setContingency(double num) {
    return getImpl<detail::ComponentCostAdjustments_Impl>()->setContingency(num);
  }

  OptionalDouble ComponentCostAdjustments::permitsBondingAndInsurance() const {
    return getImpl<detail::ComponentCostAdjustments_Impl>()->permitsBondingAndInsurance();
  }

  bool ComponentCostAdjustments::setPermitsBondingAndInsurance(double num) {
    return getImpl<detail::ComponentCostAdjustments_Impl>()->setPermitsBondingAndInsurance(num);
  }

  OptionalDouble ComponentCostAdjustments::commissioningFee() const {
    return getImpl<detail::ComponentCostAdjustments_Impl>()->commissioningFee();
  }

  bool ComponentCostAdjustments::setCommissioningFee(double num) {
    return getImpl<detail::ComponentCostAdjustments_Impl>()->setCommissioningFee(num);
  }

  OptionalDouble ComponentCostAdjustments::regionalAdjustmentFactor() const {
    return getImpl<detail::ComponentCostAdjustments_Impl>()->regionalAdjustmentFactor();
  }

  bool ComponentCostAdjustments::setRegionalAdjustmentFactor(double num) {
    return getImpl<detail::ComponentCostAdjustments_Impl>()->setRegionalAdjustmentFactor(num);
  }

  IddObjectType ComponentCostAdjustments::iddObjectType() {
    IddObjectType result(IddObjectType::OS_ComponentCost_Adjustments);
    return result;
  }

}  // namespace model
}  // namespace openstudio
