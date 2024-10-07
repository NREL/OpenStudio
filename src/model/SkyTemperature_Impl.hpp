/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SKYTEMPERATURE_IMPL_HPP
#define MODEL_SKYTEMPERATURE_IMPL_HPP

#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class MODEL_API SkyTemperature_Impl : public ModelObject_Impl
    {
     public:
      // constructor
      SkyTemperature_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      SkyTemperature_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // clone copy constructor
      SkyTemperature_Impl(const SkyTemperature_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~SkyTemperature_Impl() override = default;

      // return the parent object in the hierarchy
      virtual boost::optional<ParentObject> parent() const override;

      // set the parent, child may have to call methods on the parent
      virtual bool setParent(ParentObject& newParent) override;

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

     private:
      REGISTER_LOGGER("openstudio.model.SkyTemperature");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SKYTEMPERATURE_IMPL_HPP
