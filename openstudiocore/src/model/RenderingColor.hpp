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

#ifndef MODEL_RENDERINGCOLOR_HPP
#define MODEL_RENDERINGCOLOR_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

class QColor;

namespace openstudio {
namespace model {

namespace detail {

  class RenderingColor_Impl;

} // detail

/** RenderingColor is a ResourceObject that wraps the OpenStudio IDD object 'OS_Rendering_Color'. */
class MODEL_API RenderingColor : public ResourceObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit RenderingColor(const Model& model);

  virtual ~RenderingColor() {}

  static boost::optional<RenderingColor> fromColorString(const std::string& s, const Model& model);

  //@}
  /** @name Static Methods */
  //@{

  static QColor randomColor();

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
  typedef detail::RenderingColor_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  explicit RenderingColor(std::shared_ptr<detail::RenderingColor_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.RenderingColor");
};

/** \relates RenderingColor*/
typedef boost::optional<RenderingColor> OptionalRenderingColor;

/** \relates RenderingColor*/
typedef std::vector<RenderingColor> RenderingColorVector;

} // model
} // openstudio

#endif // MODEL_RENDERINGCOLOR_HPP

