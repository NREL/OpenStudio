/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LIFECYCLECOSTUSEPRICEESCALATION_IMPL_HPP
#define MODEL_LIFECYCLECOSTUSEPRICEESCALATION_IMPL_HPP

#include "ParentObject_Impl.hpp"
#include "LifeCycleCostUsePriceEscalation.hpp"
#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace model {
  namespace detail {

    class MODEL_API LifeCycleCostUsePriceEscalation_Impl : public ParentObject_Impl
    {
     public:
      // constructor
      LifeCycleCostUsePriceEscalation_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      LifeCycleCostUsePriceEscalation_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      LifeCycleCostUsePriceEscalation_Impl(const LifeCycleCostUsePriceEscalation_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~LifeCycleCostUsePriceEscalation_Impl() override = default;

      OptionalString resource() const;
      bool setResource(const std::string& str);

      OptionalString escalationStartMonth() const;
      bool setEscalationStartMonth(const std::string& str);

      OptionalInt escalationStartYear() const;
      bool setEscalationStartYear(int num);

      boost::optional<double> yearEscalation(unsigned index) const;
      bool setYearEscalation(unsigned index, double num);

      unsigned numYears() const;

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
        return LifeCycleCostUsePriceEscalation::iddObjectType();
      }

     private:
      REGISTER_LOGGER("openstudio.model.LifeCycleCostUsePriceEscalation");
    };

  }  // namespace detail
}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LIFECYCLECOSTUSEPRICEESCALATION_IMPL_HPP
