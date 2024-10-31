/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_RENDERINGCOLOR_IMPL_HPP
#define MODEL_RENDERINGCOLOR_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

  class RenderingColor;

  namespace detail {

    /** RenderingColor_Impl is a ResourceObject_Impl that is the implementation class for RenderingColor.*/
    class MODEL_API RenderingColor_Impl : public ResourceObject_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      RenderingColor_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RenderingColor_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RenderingColor_Impl(const RenderingColor_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RenderingColor_Impl() override = default;

      //@}
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      /** @name Getters */
      //@{

      int renderingRedValue() const;

      int renderingGreenValue() const;

      int renderingBlueValue() const;

      int renderingAlphaValue() const;

      bool isRenderingAlphaValueDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setRenderingRedValue(int renderingRedValue);
      bool setRenderingRedValue(int renderingRedValue, bool driverMethod);

      bool setRenderingGreenValue(int renderingGreenValue);
      bool setRenderingGreenValue(int renderingGreenValue, bool driverMethod);

      bool setRenderingBlueValue(int renderingBlueValue);
      bool setRenderingBlueValue(int renderingBlueValue, bool driverMethod);

      bool setRenderingAlphaValue(int renderingAlphaValue);
      bool setRenderingAlphaValue(int renderingAlphaValue, bool driverMethod);

      void resetRenderingAlphaValue();

      //@}

      std::string colorString() const;

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.RenderingColor");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_RENDERINGCOLOR_IMPL_HPP
