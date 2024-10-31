/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COMPONENTCOSTADJUSTMENTS_IMPL_HPP
#define MODEL_COMPONENTCOSTADJUSTMENTS_IMPL_HPP

#include "ParentObject_Impl.hpp"
#include "ComponentCostAdjustments.hpp"

#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace model {
  namespace detail {

    class MODEL_API ComponentCostAdjustments_Impl : public ParentObject_Impl
    {
     public:
      // constructor
      ComponentCostAdjustments_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      ComponentCostAdjustments_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      ComponentCostAdjustments_Impl(const ComponentCostAdjustments_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~ComponentCostAdjustments_Impl() override = default;

      boost::optional<double> miscellaneousCostPerConditionedArea() const;
      bool setMiscellaneousCostPerConditionedArea(double num);

      boost::optional<double> designAndEngineeringFees() const;
      bool setDesignAndEngineeringFees(double num);

      boost::optional<double> contractorFee() const;
      bool setContractorFee(double num);

      boost::optional<double> contingency() const;
      bool setContingency(double num);

      boost::optional<double> permitsBondingAndInsurance() const;
      bool setPermitsBondingAndInsurance(double num);

      boost::optional<double> commissioningFee() const;
      bool setCommissioningFee(double num);

      boost::optional<double> regionalAdjustmentFactor() const;
      bool setRegionalAdjustmentFactor(double num);

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      /// get a vector of allowable children types
      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override {
        return ComponentCostAdjustments::iddObjectType();
      }

     private:
      REGISTER_LOGGER("openstudio.model.ComponentCostAdjustments");
    };

  }  // namespace detail
}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COMPONENTCOSTADJUSTMENTS_IMPL_HPP
