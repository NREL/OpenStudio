/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_CURRENCYTYPE_IMPL_HPP
#define MODEL_CURRENCYTYPE_IMPL_HPP

#include "ParentObject_Impl.hpp"
#include "CurrencyType.hpp"

#include "../utilities/core/Optional.hpp"

namespace openstudio {
namespace model {
  namespace detail {

    class MODEL_API CurrencyType_Impl : public ParentObject_Impl
    {
     public:
      // constructor
      CurrencyType_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      CurrencyType_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      CurrencyType_Impl(const CurrencyType_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~CurrencyType_Impl() override = default;

      OptionalString monetaryUnit() const;
      bool setMonetaryUnit(const std::string& str);

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

      virtual IddObjectType iddObjectType() const override;

     private:
      REGISTER_LOGGER("openstudio.model.CurrencyType");
    };

  }  // namespace detail
}  // namespace model
}  // namespace openstudio

#endif  // MODEL_CURRENCYTYPE_IMPL_HPP
