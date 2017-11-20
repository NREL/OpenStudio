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

#include "RenderingColor.hpp"
#include "RenderingColor_Impl.hpp"

#include <utilities/idd/OS_Rendering_Color_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <QColor>

#include <cstdlib>

namespace openstudio {
namespace model {

namespace detail {

  RenderingColor_Impl::RenderingColor_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ResourceObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == RenderingColor::iddObjectType());

    QColor color = RenderingColor::randomColor();
    bool test;

    if (!getInt(OS_Rendering_ColorFields::RenderingRedValue,true)) {
      test = setRenderingRedValue(color.red(), false);
      OS_ASSERT(test);
    }
    if (!getInt(OS_Rendering_ColorFields::RenderingGreenValue,true)) {
      test = setRenderingGreenValue(color.green(), false);
      OS_ASSERT(test);
    }
    if (!getInt(OS_Rendering_ColorFields::RenderingBlueValue,true)) {
      test = setRenderingBlueValue(color.blue(), false);
      OS_ASSERT(test);
    }
    //getImpl<detail::RenderingColor_Impl>()->emitChangeSignals(); // emit signals here
  }

  RenderingColor_Impl::RenderingColor_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == RenderingColor::iddObjectType());
  }

  RenderingColor_Impl::RenderingColor_Impl(const RenderingColor_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ResourceObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& RenderingColor_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType RenderingColor_Impl::iddObjectType() const {
    return RenderingColor::iddObjectType();
  }

  int RenderingColor_Impl::renderingRedValue() const {
    boost::optional<int> value = getInt(OS_Rendering_ColorFields::RenderingRedValue,true);
    OS_ASSERT(value);
    return value.get();
  }

  int RenderingColor_Impl::renderingGreenValue() const {
    boost::optional<int> value = getInt(OS_Rendering_ColorFields::RenderingGreenValue,true);
    OS_ASSERT(value);
    return value.get();
  }

  int RenderingColor_Impl::renderingBlueValue() const {
    boost::optional<int> value = getInt(OS_Rendering_ColorFields::RenderingBlueValue,true);
    OS_ASSERT(value);
    return value.get();
  }

  int RenderingColor_Impl::renderingAlphaValue() const {
    boost::optional<int> value = getInt(OS_Rendering_ColorFields::RenderingAlphaValue,true);
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

  std::string RenderingColor_Impl::colorString() const
  {
    return "";
  }

} // detail

RenderingColor::RenderingColor(const Model& model)
  : ResourceObject(RenderingColor::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::RenderingColor_Impl>());
}

boost::optional<RenderingColor> RenderingColor::fromColorString(const std::string& s, const Model& model)
{
  return boost::none;
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

std::string RenderingColor::colorString() const
{
  return getImpl<detail::RenderingColor_Impl>()->colorString();
}

/// @cond
RenderingColor::RenderingColor(std::shared_ptr<detail::RenderingColor_Impl> impl)
  : ResourceObject(std::move(impl))
{}

QColor RenderingColor::randomColor()
{
  static std::vector<QColor> colors;
  if (colors.empty()){
    colors.push_back(QColor(240,248,255)); // AliceBlue
    colors.push_back(QColor(250,235,215)); // AntiqueWhite
    colors.push_back(QColor(0,255,255)); // Aqua
    colors.push_back(QColor(127,255,212)); // Aquamarine
    colors.push_back(QColor(240,255,255)); // Azure
    colors.push_back(QColor(245,245,220)); // Beige
    colors.push_back(QColor(255,228,196)); // Bisque
    colors.push_back(QColor(0,0,0)); // Black
    colors.push_back(QColor(255,235,205)); // BlanchedAlmond
    colors.push_back(QColor(0,0,255)); // Blue
    colors.push_back(QColor(138,43,226)); // BlueViolet
    colors.push_back(QColor(165,42,42)); // Brown
    colors.push_back(QColor(222,184,135)); // BurlyWood
    colors.push_back(QColor(95,158,160)); // CadetBlue
    colors.push_back(QColor(127,255,0)); // Chartreuse
    colors.push_back(QColor(210,105,30)); // Chocolate
    colors.push_back(QColor(255,127,80)); // Coral
    colors.push_back(QColor(100,149,237)); // CornflowerBlue
    colors.push_back(QColor(255,248,220)); // Cornsilk
    colors.push_back(QColor(220,20,60)); // Crimson
    colors.push_back(QColor(0,255,255)); // Cyan
    colors.push_back(QColor(0,0,139)); // DarkBlue
    colors.push_back(QColor(0,139,139)); // DarkCyan
    colors.push_back(QColor(184,134,11)); // DarkGoldenrod
    colors.push_back(QColor(169,169,169)); // DarkGray
    colors.push_back(QColor(0,100,0)); // DarkGreen
    colors.push_back(QColor(189,183,107)); // DarkKhaki
    colors.push_back(QColor(139,0,139)); // DarkMagenta
    colors.push_back(QColor(85,107,47)); // DarkOliveGreen
    colors.push_back(QColor(255,140,0)); // DarkOrange
    colors.push_back(QColor(153,50,204)); // DarkOrchid
    colors.push_back(QColor(139,0,0)); // DarkRed
    colors.push_back(QColor(233,150,122)); // DarkSalmon
    colors.push_back(QColor(143,188,143)); // DarkSeaGreen
    colors.push_back(QColor(72,61,139)); // DarkSlateBlue
    colors.push_back(QColor(47,79,79)); // DarkSlateGray
    colors.push_back(QColor(0,206,209)); // DarkTurquoise
    colors.push_back(QColor(148,0,211)); // DarkViolet
    colors.push_back(QColor(255,20,147)); // DeepPink
    colors.push_back(QColor(0,191,255)); // DeepSkyBlue
    colors.push_back(QColor(105,105,105)); // DimGray
    colors.push_back(QColor(30,144,255)); // DodgerBlue
    colors.push_back(QColor(178,34,34)); // FireBrick
    colors.push_back(QColor(255,250,240)); // FloralWhite
    colors.push_back(QColor(34,139,34)); // ForestGreen
    colors.push_back(QColor(255,0,255)); // Fuchsia
    colors.push_back(QColor(220,220,220)); // Gainsboro
    colors.push_back(QColor(248,248,255)); // GhostWhite
    colors.push_back(QColor(255,215,0)); // Gold
    colors.push_back(QColor(218,165,32)); // Goldenrod
    colors.push_back(QColor(128,128,128)); // Gray
    colors.push_back(QColor(0,128,0)); // Green
    colors.push_back(QColor(173,255,47)); // GreenYellow
    colors.push_back(QColor(240,255,240)); // Honeydew
    colors.push_back(QColor(255,105,180)); // HotPink
    colors.push_back(QColor(205,92,92)); // IndianRed
    colors.push_back(QColor(75,0,130)); // Indigo
    colors.push_back(QColor(255,255,240)); // Ivory
    colors.push_back(QColor(240,230,140)); // Khaki
    colors.push_back(QColor(230,230,250)); // Lavender
    colors.push_back(QColor(255,240,245)); // LavenderBlush
    colors.push_back(QColor(124,252,0)); // LawnGreen
    colors.push_back(QColor(255,250,205)); // LemonChiffon
    colors.push_back(QColor(173,216,230)); // LightBlue
    colors.push_back(QColor(240,128,128)); // LightCoral
    colors.push_back(QColor(224,255,255)); // LightCyan
    colors.push_back(QColor(250,250,210)); // LightGoldenrodYellow
    colors.push_back(QColor(144,238,144)); // LightGreen
    colors.push_back(QColor(211,211,211)); // LightGrey
    colors.push_back(QColor(255,182,193)); // LightPink
    colors.push_back(QColor(255,160,122)); // LightSalmon
    colors.push_back(QColor(32,178,170)); // LightSeaGreen
    colors.push_back(QColor(135,206,250)); // LightSkyBlue
    colors.push_back(QColor(119,136,153)); // LightSlateGray
    colors.push_back(QColor(176,196,222)); // LightSteelBlue
    colors.push_back(QColor(255,255,224)); // LightYellow
    colors.push_back(QColor(0,255,0)); // Lime
    colors.push_back(QColor(50,205,50)); // LimeGreen
    colors.push_back(QColor(250,240,230)); // Linen
    colors.push_back(QColor(255,0,255)); // Magenta
    colors.push_back(QColor(128,0,0)); // Maroon
    colors.push_back(QColor(102,205,170)); // MediumAquamarine
    colors.push_back(QColor(0,0,205)); // MediumBlue
    colors.push_back(QColor(186,85,211)); // MediumOrchid
    colors.push_back(QColor(147,112,219)); // MediumPurple
    colors.push_back(QColor(60,179,113)); // MediumSeaGreen
    colors.push_back(QColor(123,104,238)); // MediumSlateBlue
    colors.push_back(QColor(0,250,154)); // MediumSpringGreen
    colors.push_back(QColor(72,209,204)); // MediumTurquoise
    colors.push_back(QColor(199,21,133)); // MediumVioletRed
    colors.push_back(QColor(25,25,112)); // MidnightBlue
    colors.push_back(QColor(245,255,250)); // MintCream
    colors.push_back(QColor(255,228,225)); // MistyRose
    colors.push_back(QColor(255,228,181)); // Moccasin
    colors.push_back(QColor(255,222,173)); // NavajoWhite
    colors.push_back(QColor(0,0,128)); // Navy
    colors.push_back(QColor(253,245,230)); // OldLace
    colors.push_back(QColor(128,128,0)); // Olive
    colors.push_back(QColor(128,128,0)); // OliveDrab
    colors.push_back(QColor(255,165,0)); // Orange
    colors.push_back(QColor(255,69,0)); // OrangeRed
    colors.push_back(QColor(218,112,214)); // Orchid
    colors.push_back(QColor(238,232,170)); // PaleGoldenrod
    colors.push_back(QColor(152,251,152)); // PaleGreen
    colors.push_back(QColor(175,238,238)); // PaleTurquoise
    colors.push_back(QColor(219,112,147)); // PaleVioletRed
    colors.push_back(QColor(255,239,213)); // PapayaWhip
    colors.push_back(QColor(255,218,185)); // PeachPuff
    colors.push_back(QColor(205,133,63)); // Peru
    colors.push_back(QColor(255,192,203)); // Pink
    colors.push_back(QColor(221,160,221)); // Plum
    colors.push_back(QColor(176,224,230)); // PowderBlue
    colors.push_back(QColor(128,0,128)); // Purple
    colors.push_back(QColor(255,0,0)); // Red
    colors.push_back(QColor(188,143,143)); // RosyBrown
    colors.push_back(QColor(65,105,225)); // RoyalBlue
    colors.push_back(QColor(139,69,19)); // SaddleBrown
    colors.push_back(QColor(250,128,114)); // Salmon
    colors.push_back(QColor(244,164,96)); // SandyBrown
    colors.push_back(QColor(46,139,87)); // SeaGreen
    colors.push_back(QColor(255,245,238)); // Seashell
    colors.push_back(QColor(160,82,45)); // Sienna
    colors.push_back(QColor(192,192,192)); // Silver
    colors.push_back(QColor(135,206,235)); // SkyBlue
    colors.push_back(QColor(106,90,205)); // SlateBlue
    colors.push_back(QColor(112,128,144)); // SlateGray
    colors.push_back(QColor(255,250,250)); // Snow
    colors.push_back(QColor(0,255,127)); // SpringGreen
    colors.push_back(QColor(70,130,180)); // SteelBlue
    colors.push_back(QColor(210,180,140)); // Tan
    colors.push_back(QColor(210,180,140)); // Teal
    colors.push_back(QColor(216,191,216)); // Thistle
    colors.push_back(QColor(255,99,71)); // Tomato
    colors.push_back(QColor(64,224,208)); // Turquoise
    colors.push_back(QColor(238,130,238)); // Violet
    colors.push_back(QColor(245,222,179)); // Wheat
    //colors.push_back(QColor(255,255,255)); // White
    colors.push_back(QColor(245,245,245)); // WhiteSmoke
    colors.push_back(QColor(255,255,0)); // Yellow
    colors.push_back(QColor(154,205,50)); // YellowGreen
  }
  int index = rand() % colors.size();
  return colors[index];
}

/// @endcond


} // model
} // openstudio

