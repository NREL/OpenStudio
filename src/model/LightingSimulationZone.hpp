/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
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

    virtual ~LightingSimulationZone() override = default;
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
