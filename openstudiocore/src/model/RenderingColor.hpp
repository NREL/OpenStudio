/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

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

