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

#ifndef MODEL_LIGHTINGSIMULATIONZONE_IMPL_HPP
#define MODEL_LIGHTINGSIMULATIONZONE_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

class RenderingColor;
class Space;
class LightingSimulationZone;

namespace detail {

  /** LightingSimulationZone_Impl is a ModelObject_Impl that is the implementation class for LightingSimulationZone.*/
  class MODEL_API LightingSimulationZone_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    LightingSimulationZone_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    LightingSimulationZone_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                Model_Impl* model,
                                bool keepHandle);

    LightingSimulationZone_Impl(const LightingSimulationZone_Impl& other,
                                Model_Impl* model,
                                bool keepHandle);

    virtual ~LightingSimulationZone_Impl() {}

    //@}
    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    /** @name Getters */
    //@{

    //@}
    /** @name Setters */
    //@{

    //@}

    /// Returns the rendering color.
    boost::optional<RenderingColor> renderingColor() const;

    /// Sets the rendering color.
    bool setRenderingColor(const RenderingColor& renderingColor);

    /// Resets the rendering color.
    void resetRenderingColor();

    /// Returns all spaces in this lighting simulation zone.
    std::vector<Space> spaces() const;

    /// Adds a spaces to this lighting simulation zone.
    bool addSpace(const Space& space);

    /// Resets spaces in this lighting simulation zone.
    void resetSpaces();

   protected:
   private:
    REGISTER_LOGGER("openstudio.model.LightingSimulationZone");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_LIGHTINGSIMULATIONZONE_IMPL_HPP

