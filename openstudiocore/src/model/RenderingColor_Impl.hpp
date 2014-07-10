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
    Q_OBJECT;
    Q_PROPERTY(int renderingRedValue READ renderingRedValue WRITE setRenderingRedValue);
    Q_PROPERTY(int renderingGreenValue READ renderingGreenValue WRITE setRenderingGreenValue);
    Q_PROPERTY(int renderingBlueValue READ renderingBlueValue WRITE setRenderingBlueValue);
    Q_PROPERTY(int renderingAlphaValue READ renderingAlphaValue WRITE setRenderingAlphaValue RESET resetRenderingAlphaValue);
    Q_PROPERTY(bool isRenderingAlphaValueDefaulted READ isRenderingAlphaValueDefaulted);
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
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

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
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.RenderingColor");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_RENDERINGCOLOR_IMPL_HPP

