/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SIZINGPERIOD_IMPL_HPP
#define MODEL_SIZINGPERIOD_IMPL_HPP

#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API SizingPeriod_Impl : public ParentObject_Impl
    {
     public:
      // constructor
      SizingPeriod_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      SizingPeriod_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      SizingPeriod_Impl(const SizingPeriod_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~SizingPeriod_Impl() override = default;

      // ensure that this object does not contain the date 2/29
      virtual void ensureNoLeapDays() = 0;

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      // return any children objects in the hierarchy
      virtual std::vector<ModelObject> children() const override;

      /// get a vector of allowable children types
      virtual std::vector<IddObjectType> allowableChildTypes() const override;

     private:
      REGISTER_LOGGER("openstudio.model.SizingPeriod");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SIZINGPERIOD_IMPL_HPP
