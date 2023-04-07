/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_RENDERINGCOLOR_HPP
#define MODEL_RENDERINGCOLOR_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class RenderingColor_Impl;

  }  // namespace detail

  class MODEL_API ColorRGB
  {
   public:
    /// Integer components of RGB color from 0-255
    ColorRGB(int r, int g, int b);
    int red() const;
    int green() const;
    int blue() const;

   private:
    int m_red;
    int m_green;
    int m_blue;
  };

  /** RenderingColor is a ResourceObject that wraps the OpenStudio IDD object 'OS_Rendering_Color'. */
  class MODEL_API RenderingColor : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit RenderingColor(const Model& model);

    virtual ~RenderingColor() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RenderingColor(const RenderingColor& other) = default;
    RenderingColor(RenderingColor&& other) = default;
    RenderingColor& operator=(const RenderingColor&) = default;
    RenderingColor& operator=(RenderingColor&&) = default;

    static boost::optional<RenderingColor> fromColorString(const std::string& s, const Model& model);

    //@}
    /** @name Static Methods */
    //@{

    static ColorRGB randomColor();

    static IddObjectType iddObjectType();

    //@}
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

    bool setRenderingGreenValue(int renderingGreenValue);

    bool setRenderingBlueValue(int renderingBlueValue);

    bool setRenderingAlphaValue(int renderingAlphaValue);

    void resetRenderingAlphaValue();

    //@}

    std::string colorString() const;

   protected:
    /// @cond
    using ImplType = detail::RenderingColor_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit RenderingColor(std::shared_ptr<detail::RenderingColor_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.RenderingColor");
  };

  /** \relates RenderingColor*/
  using OptionalRenderingColor = boost::optional<RenderingColor>;

  /** \relates RenderingColor*/
  using RenderingColorVector = std::vector<RenderingColor>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_RENDERINGCOLOR_HPP
