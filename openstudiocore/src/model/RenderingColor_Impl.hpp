/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_RENDERINGCOLOR_IMPL_HPP
#define MODEL_RENDERINGCOLOR_IMPL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject_Impl.hpp"

namespace openstudio {
namespace model {

class RenderingColor;

namespace detail {

  /** RenderingColor_Impl is a ResourceObject_Impl that is the implementation class for RenderingColor.*/
  class MODEL_API RenderingColor_Impl : public ResourceObject_Impl {
    
    
    
    
    
    
   public:
    /** @name Constructors and Destructors */
    //@{

    RenderingColor_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    RenderingColor_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    RenderingColor_Impl(const RenderingColor_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    virtual ~RenderingColor_Impl() {}

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

} // detail

} // model
} // openstudio

#endif // MODEL_RENDERINGCOLOR_IMPL_HPP

