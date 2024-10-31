/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~RenderingColor() override = default;
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
