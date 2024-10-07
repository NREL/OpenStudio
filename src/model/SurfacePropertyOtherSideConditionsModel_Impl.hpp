/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SURFACEPROPERTYOTHERSIDECONDITIONSMODEL_IMPL_HPP
#define MODEL_SURFACEPROPERTYOTHERSIDECONDITIONSMODEL_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** SurfacePropertyOtherSideConditionsModel_Impl is a ResourceObject_Impl that is the implementation class for SurfacePropertyOtherSideConditionsModel.*/
    class MODEL_API SurfacePropertyOtherSideConditionsModel_Impl : public ResourceObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      SurfacePropertyOtherSideConditionsModel_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      SurfacePropertyOtherSideConditionsModel_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      SurfacePropertyOtherSideConditionsModel_Impl(const SurfacePropertyOtherSideConditionsModel_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~SurfacePropertyOtherSideConditionsModel_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string typeOfModeling() const;

      bool isTypeOfModelingDefaulted() const;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      bool setTypeOfModeling(const std::string& typeOfModeling);

      void resetTypeOfModeling();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.SurfacePropertyOtherSideConditionsModel");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYOTHERSIDECONDITIONSMODEL_IMPL_HPP
