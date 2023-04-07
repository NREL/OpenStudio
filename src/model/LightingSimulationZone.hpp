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

#ifndef MODEL_LIGHTINGSIMULATIONZONE_HPP
#define MODEL_LIGHTINGSIMULATIONZONE_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class RenderingColor;
  class Space;

  namespace detail {

    class LightingSimulationZone_Impl;

  }  // namespace detail

  // ETH@20120328 Should LightingSimulationZone be parented by Building,
  // similar to ThermalZone?

  /** LightingSimulationZone is a ModelObject that wraps the OpenStudio IDD object 'OS_LightingSimulationZone'. */
  class MODEL_API LightingSimulationZone : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit LightingSimulationZone(const Model& model);

    virtual ~LightingSimulationZone() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    LightingSimulationZone(const LightingSimulationZone& other) = default;
    LightingSimulationZone(LightingSimulationZone&& other) = default;
    LightingSimulationZone& operator=(const LightingSimulationZone&) = default;
    LightingSimulationZone& operator=(LightingSimulationZone&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
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
    /// @cond
    using ImplType = detail::LightingSimulationZone_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit LightingSimulationZone(std::shared_ptr<detail::LightingSimulationZone_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.LightingSimulationZone");
  };

  /** \relates LightingSimulationZone*/
  using OptionalLightingSimulationZone = boost::optional<LightingSimulationZone>;

  /** \relates LightingSimulationZone*/
  using LightingSimulationZoneVector = std::vector<LightingSimulationZone>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LIGHTINGSIMULATIONZONE_HPP
