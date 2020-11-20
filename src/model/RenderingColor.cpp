/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include "RenderingColor.hpp"
#include "RenderingColor_Impl.hpp"

#include <utilities/idd/OS_Rendering_Color_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <cstdlib>
#include <iomanip>

namespace openstudio {
namespace model {

  namespace detail {

    RenderingColor_Impl::RenderingColor_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == RenderingColor::iddObjectType());

      ColorRGB color = RenderingColor::randomColor();

      bool test;
      if (!getInt(OS_Rendering_ColorFields::RenderingRedValue, true)) {
        test = setRenderingRedValue(color.red(), false);
        OS_ASSERT(test);
      }
      if (!getInt(OS_Rendering_ColorFields::RenderingGreenValue, true)) {
        test = setRenderingGreenValue(color.blue(), false);
        OS_ASSERT(test);
      }
      if (!getInt(OS_Rendering_ColorFields::RenderingBlueValue, true)) {
        test = setRenderingBlueValue(color.green(), false);
        OS_ASSERT(test);
      }
      //getImpl<detail::RenderingColor_Impl>()->emitChangeSignals(); // emit signals here
    }

    RenderingColor_Impl::RenderingColor_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == RenderingColor::iddObjectType());
    }

    RenderingColor_Impl::RenderingColor_Impl(const RenderingColor_Impl& other, Model_Impl* model, bool keepHandle)
      : ResourceObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& RenderingColor_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType RenderingColor_Impl::iddObjectType() const {
      return RenderingColor::iddObjectType();
    }

    int RenderingColor_Impl::renderingRedValue() const {
      boost::optional<int> value = getInt(OS_Rendering_ColorFields::RenderingRedValue, true);
      OS_ASSERT(value);
      return value.get();
    }

    int RenderingColor_Impl::renderingGreenValue() const {
      boost::optional<int> value = getInt(OS_Rendering_ColorFields::RenderingGreenValue, true);
      OS_ASSERT(value);
      return value.get();
    }

    int RenderingColor_Impl::renderingBlueValue() const {
      boost::optional<int> value = getInt(OS_Rendering_ColorFields::RenderingBlueValue, true);
      OS_ASSERT(value);
      return value.get();
    }

    int RenderingColor_Impl::renderingAlphaValue() const {
      boost::optional<int> value = getInt(OS_Rendering_ColorFields::RenderingAlphaValue, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool RenderingColor_Impl::isRenderingAlphaValueDefaulted() const {
      return isEmpty(OS_Rendering_ColorFields::RenderingAlphaValue);
    }

    bool RenderingColor_Impl::setRenderingRedValue(int renderingRedValue) {
      return setRenderingRedValue(renderingRedValue, true);
    }

    bool RenderingColor_Impl::setRenderingRedValue(int renderingRedValue, bool driverMethod) {
      bool result = setInt(OS_Rendering_ColorFields::RenderingRedValue, renderingRedValue, driverMethod);
      return result;
    }

    bool RenderingColor_Impl::setRenderingGreenValue(int renderingGreenValue) {
      return setRenderingGreenValue(renderingGreenValue, true);
    }

    bool RenderingColor_Impl::setRenderingGreenValue(int renderingGreenValue, bool driverMethod) {
      bool result = setInt(OS_Rendering_ColorFields::RenderingGreenValue, renderingGreenValue);
      return result;
    }

    bool RenderingColor_Impl::setRenderingBlueValue(int renderingBlueValue) {
      return setRenderingBlueValue(renderingBlueValue, true);
    }

    bool RenderingColor_Impl::setRenderingBlueValue(int renderingBlueValue, bool driverMethod) {
      bool result = setInt(OS_Rendering_ColorFields::RenderingBlueValue, renderingBlueValue, driverMethod);
      return result;
    }

    bool RenderingColor_Impl::setRenderingAlphaValue(int renderingAlphaValue) {
      return setRenderingAlphaValue(renderingAlphaValue, true);
    }

    bool RenderingColor_Impl::setRenderingAlphaValue(int renderingAlphaValue, bool driverMethod) {
      bool result = setInt(OS_Rendering_ColorFields::RenderingAlphaValue, renderingAlphaValue, driverMethod);
      return result;
    }

    void RenderingColor_Impl::resetRenderingAlphaValue() {
      bool result = setString(OS_Rendering_ColorFields::RenderingAlphaValue, "");
      OS_ASSERT(result);
    }

    std::string RenderingColor_Impl::colorString() const {
      std::stringstream stream;
      stream << "#" << std::setfill('0') << std::setw(6) << std::uppercase << std::hex
             << renderingRedValue() * 65536 + renderingGreenValue() * 256 + renderingBlueValue();

      std::string result = stream.str();
      OS_ASSERT(result.size() == 7);
      return result;
    }

  }  // namespace detail

  ColorRGB::ColorRGB(int r, int g, int b) : m_red(r), m_green(g), m_blue(b) {}

  int ColorRGB::red() const {
    return m_red;
  }

  int ColorRGB::green() const {
    return m_green;
  }

  int ColorRGB::blue() const {
    return m_blue;
  }

  RenderingColor::RenderingColor(const Model& model) : ResourceObject(RenderingColor::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::RenderingColor_Impl>());
  }

  boost::optional<RenderingColor> RenderingColor::fromColorString(const std::string& s, const Model& model) {
    if (s.size() != 7) {
      return boost::none;
    }

    int r = std::stoi(s.substr(1, 2), 0, 16);
    int g = std::stoi(s.substr(3, 2), 0, 16);
    int b = std::stoi(s.substr(5, 2), 0, 16);

    RenderingColor result(model);
    result.setRenderingRedValue(r);
    result.setRenderingGreenValue(g);
    result.setRenderingBlueValue(b);
    return result;
  }

  IddObjectType RenderingColor::iddObjectType() {
    IddObjectType result(IddObjectType::OS_Rendering_Color);
    return result;
  }

  int RenderingColor::renderingRedValue() const {
    return getImpl<detail::RenderingColor_Impl>()->renderingRedValue();
  }

  int RenderingColor::renderingGreenValue() const {
    return getImpl<detail::RenderingColor_Impl>()->renderingGreenValue();
  }

  int RenderingColor::renderingBlueValue() const {
    return getImpl<detail::RenderingColor_Impl>()->renderingBlueValue();
  }

  int RenderingColor::renderingAlphaValue() const {
    return getImpl<detail::RenderingColor_Impl>()->renderingAlphaValue();
  }

  bool RenderingColor::isRenderingAlphaValueDefaulted() const {
    return getImpl<detail::RenderingColor_Impl>()->isRenderingAlphaValueDefaulted();
  }

  bool RenderingColor::setRenderingRedValue(int renderingRedValue) {
    return getImpl<detail::RenderingColor_Impl>()->setRenderingRedValue(renderingRedValue);
  }

  bool RenderingColor::setRenderingGreenValue(int renderingGreenValue) {
    return getImpl<detail::RenderingColor_Impl>()->setRenderingGreenValue(renderingGreenValue);
  }

  bool RenderingColor::setRenderingBlueValue(int renderingBlueValue) {
    return getImpl<detail::RenderingColor_Impl>()->setRenderingBlueValue(renderingBlueValue);
  }

  bool RenderingColor::setRenderingAlphaValue(int renderingAlphaValue) {
    return getImpl<detail::RenderingColor_Impl>()->setRenderingAlphaValue(renderingAlphaValue);
  }

  void RenderingColor::resetRenderingAlphaValue() {
    getImpl<detail::RenderingColor_Impl>()->resetRenderingAlphaValue();
  }

  std::string RenderingColor::colorString() const {
    return getImpl<detail::RenderingColor_Impl>()->colorString();
  }

  /// @cond
  RenderingColor::RenderingColor(std::shared_ptr<detail::RenderingColor_Impl> impl) : ResourceObject(std::move(impl)) {}

  ColorRGB RenderingColor::randomColor() {
    static const std::vector<ColorRGB> colors{
      ColorRGB(240, 248, 255),  // AliceBlue
      ColorRGB(250, 235, 215),  // AntiqueWhite
      ColorRGB(0, 255, 255),    // Aqua
      ColorRGB(127, 255, 212),  // Aquamarine
      ColorRGB(240, 255, 255),  // Azure
      ColorRGB(245, 245, 220),  // Beige
      ColorRGB(255, 228, 196),  // Bisque
      ColorRGB(0, 0, 0),        // Black
      ColorRGB(255, 235, 205),  // BlanchedAlmond
      ColorRGB(0, 0, 255),      // Blue
      ColorRGB(138, 43, 226),   // BlueViolet
      ColorRGB(165, 42, 42),    // Brown
      ColorRGB(222, 184, 135),  // BurlyWood
      ColorRGB(95, 158, 160),   // CadetBlue
      ColorRGB(127, 255, 0),    // Chartreuse
      ColorRGB(210, 105, 30),   // Chocolate
      ColorRGB(255, 127, 80),   // Coral
      ColorRGB(100, 149, 237),  // CornflowerBlue
      ColorRGB(255, 248, 220),  // Cornsilk
      ColorRGB(220, 20, 60),    // Crimson
      ColorRGB(0, 255, 255),    // Cyan
      ColorRGB(0, 0, 139),      // DarkBlue
      ColorRGB(0, 139, 139),    // DarkCyan
      ColorRGB(184, 134, 11),   // DarkGoldenrod
      ColorRGB(169, 169, 169),  // DarkGray
      ColorRGB(0, 100, 0),      // DarkGreen
      ColorRGB(189, 183, 107),  // DarkKhaki
      ColorRGB(139, 0, 139),    // DarkMagenta
      ColorRGB(85, 107, 47),    // DarkOliveGreen
      ColorRGB(255, 140, 0),    // DarkOrange
      ColorRGB(153, 50, 204),   // DarkOrchid
      ColorRGB(139, 0, 0),      // DarkRed
      ColorRGB(233, 150, 122),  // DarkSalmon
      ColorRGB(143, 188, 143),  // DarkSeaGreen
      ColorRGB(72, 61, 139),    // DarkSlateBlue
      ColorRGB(47, 79, 79),     // DarkSlateGray
      ColorRGB(0, 206, 209),    // DarkTurquoise
      ColorRGB(148, 0, 211),    // DarkViolet
      ColorRGB(255, 20, 147),   // DeepPink
      ColorRGB(0, 191, 255),    // DeepSkyBlue
      ColorRGB(105, 105, 105),  // DimGray
      ColorRGB(30, 144, 255),   // DodgerBlue
      ColorRGB(178, 34, 34),    // FireBrick
      ColorRGB(255, 250, 240),  // FloralWhite
      ColorRGB(34, 139, 34),    // ForestGreen
      ColorRGB(255, 0, 255),    // Fuchsia
      ColorRGB(220, 220, 220),  // Gainsboro
      ColorRGB(248, 248, 255),  // GhostWhite
      ColorRGB(255, 215, 0),    // Gold
      ColorRGB(218, 165, 32),   // Goldenrod
      ColorRGB(128, 128, 128),  // Gray
      ColorRGB(0, 128, 0),      // Green
      ColorRGB(173, 255, 47),   // GreenYellow
      ColorRGB(240, 255, 240),  // Honeydew
      ColorRGB(255, 105, 180),  // HotPink
      ColorRGB(205, 92, 92),    // IndianRed
      ColorRGB(75, 0, 130),     // Indigo
      ColorRGB(255, 255, 240),  // Ivory
      ColorRGB(240, 230, 140),  // Khaki
      ColorRGB(230, 230, 250),  // Lavender
      ColorRGB(255, 240, 245),  // LavenderBlush
      ColorRGB(124, 252, 0),    // LawnGreen
      ColorRGB(255, 250, 205),  // LemonChiffon
      ColorRGB(173, 216, 230),  // LightBlue
      ColorRGB(240, 128, 128),  // LightCoral
      ColorRGB(224, 255, 255),  // LightCyan
      ColorRGB(250, 250, 210),  // LightGoldenrodYellow
      ColorRGB(144, 238, 144),  // LightGreen
      ColorRGB(211, 211, 211),  // LightGrey
      ColorRGB(255, 182, 193),  // LightPink
      ColorRGB(255, 160, 122),  // LightSalmon
      ColorRGB(32, 178, 170),   // LightSeaGreen
      ColorRGB(135, 206, 250),  // LightSkyBlue
      ColorRGB(119, 136, 153),  // LightSlateGray
      ColorRGB(176, 196, 222),  // LightSteelBlue
      ColorRGB(255, 255, 224),  // LightYellow
      ColorRGB(0, 255, 0),      // Lime
      ColorRGB(50, 205, 50),    // LimeGreen
      ColorRGB(250, 240, 230),  // Linen
      ColorRGB(255, 0, 255),    // Magenta
      ColorRGB(128, 0, 0),      // Maroon
      ColorRGB(102, 205, 170),  // MediumAquamarine
      ColorRGB(0, 0, 205),      // MediumBlue
      ColorRGB(186, 85, 211),   // MediumOrchid
      ColorRGB(147, 112, 219),  // MediumPurple
      ColorRGB(60, 179, 113),   // MediumSeaGreen
      ColorRGB(123, 104, 238),  // MediumSlateBlue
      ColorRGB(0, 250, 154),    // MediumSpringGreen
      ColorRGB(72, 209, 204),   // MediumTurquoise
      ColorRGB(199, 21, 133),   // MediumVioletRed
      ColorRGB(25, 25, 112),    // MidnightBlue
      ColorRGB(245, 255, 250),  // MintCream
      ColorRGB(255, 228, 225),  // MistyRose
      ColorRGB(255, 228, 181),  // Moccasin
      ColorRGB(255, 222, 173),  // NavajoWhite
      ColorRGB(0, 0, 128),      // Navy
      ColorRGB(253, 245, 230),  // OldLace
      ColorRGB(128, 128, 0),    // Olive
      ColorRGB(128, 128, 0),    // OliveDrab
      ColorRGB(255, 165, 0),    // Orange
      ColorRGB(255, 69, 0),     // OrangeRed
      ColorRGB(218, 112, 214),  // Orchid
      ColorRGB(238, 232, 170),  // PaleGoldenrod
      ColorRGB(152, 251, 152),  // PaleGreen
      ColorRGB(175, 238, 238),  // PaleTurquoise
      ColorRGB(219, 112, 147),  // PaleVioletRed
      ColorRGB(255, 239, 213),  // PapayaWhip
      ColorRGB(255, 218, 185),  // PeachPuff
      ColorRGB(205, 133, 63),   // Peru
      ColorRGB(255, 192, 203),  // Pink
      ColorRGB(221, 160, 221),  // Plum
      ColorRGB(176, 224, 230),  // PowderBlue
      ColorRGB(128, 0, 128),    // Purple
      ColorRGB(255, 0, 0),      // Red
      ColorRGB(188, 143, 143),  // RosyBrown
      ColorRGB(65, 105, 225),   // RoyalBlue
      ColorRGB(139, 69, 19),    // SaddleBrown
      ColorRGB(250, 128, 114),  // Salmon
      ColorRGB(244, 164, 96),   // SandyBrown
      ColorRGB(46, 139, 87),    // SeaGreen
      ColorRGB(255, 245, 238),  // Seashell
      ColorRGB(160, 82, 45),    // Sienna
      ColorRGB(192, 192, 192),  // Silver
      ColorRGB(135, 206, 235),  // SkyBlue
      ColorRGB(106, 90, 205),   // SlateBlue
      ColorRGB(112, 128, 144),  // SlateGray
      ColorRGB(255, 250, 250),  // Snow
      ColorRGB(0, 255, 127),    // SpringGreen
      ColorRGB(70, 130, 180),   // SteelBlue
      ColorRGB(210, 180, 140),  // Tan
      ColorRGB(210, 180, 140),  // Teal
      ColorRGB(216, 191, 216),  // Thistle
      ColorRGB(255, 99, 71),    // Tomato
      ColorRGB(64, 224, 208),   // Turquoise
      ColorRGB(238, 130, 238),  // Violet
      ColorRGB(245, 222, 179),  // Wheat
      //ColorRGB(255,255,255), // White
      ColorRGB(245, 245, 245),  // WhiteSmoke
      ColorRGB(255, 255, 0),    // Yellow
      ColorRGB(154, 205, 50)    // YellowGreen
    };
    int index = rand() % colors.size();
    return colors[index];
  }

  /// @endcond

}  // namespace model
}  // namespace openstudio
