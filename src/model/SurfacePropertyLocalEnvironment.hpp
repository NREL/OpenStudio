/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SURFACEPROPERTYLOCALENVIRONMENT_HPP
#define MODEL_SURFACEPROPERTYLOCALENVIRONMENT_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

  class Surface;
  class SubSurface;
  class PlanarSurface;
  class Schedule;
  class SurfacePropertySurroundingSurfaces;
  class SurfacePropertyGroundSurfaces;
  // class OutdoorAirNode;

  namespace detail {

    class SurfacePropertyLocalEnvironment_Impl;

  }  // namespace detail

  /** SurfacePropertyLocalEnvironment is a ModelObject that wraps the OpenStudio IDD object 'OS:SurfaceProperty:LocalEnvironment'. */
  class MODEL_API SurfacePropertyLocalEnvironment : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SurfacePropertyLocalEnvironment(const Surface& surface);
    explicit SurfacePropertyLocalEnvironment(const SubSurface& surface);

    virtual ~SurfacePropertyLocalEnvironment() override = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    PlanarSurface exteriorSurface() const;
    boost::optional<Surface> exteriorSurfaceAsSurface() const;
    boost::optional<SubSurface> exteriorSurfaceAsSubSurface() const;

    boost::optional<Schedule> externalShadingFractionSchedule() const;

    boost::optional<SurfacePropertySurroundingSurfaces> surfacePropertySurroundingSurfaces() const;

    boost::optional<SurfacePropertyGroundSurfaces> surfacePropertyGroundSurfaces() const;

    // boost::optional<OutdoorAirNode> outdoorAirNode() const;

    //@}
    /** @name Setters */
    //@{

    // Note that unicity is enforced: if the surface to point to already has a SurfacePropertyLocalEnvironment object attached, it will be removed
    // first
    bool setExteriorSurface(const PlanarSurface& surface);

    bool setExternalShadingFractionSchedule(Schedule& schedule);
    void resetExternalShadingFractionSchedule();

    bool setSurfacePropertySurroundingSurfaces(const SurfacePropertySurroundingSurfaces& surfacePropertySurroundingSurfaces);
    void resetSurfacePropertySurroundingSurfaces();

    bool setSurfacePropertyGroundSurfaces(const SurfacePropertyGroundSurfaces& surfacePropertyGroundSurfaces);
    void resetSurfacePropertyGroundSurfaces();

    // bool setOutdoorAirNode(const OutdoorAirNode& outdoorAirNode);
    // void resetOutdoorAirNode();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SurfacePropertyLocalEnvironment_Impl;

    explicit SurfacePropertyLocalEnvironment(std::shared_ptr<detail::SurfacePropertyLocalEnvironment_Impl> impl);

    friend class detail::SurfacePropertyLocalEnvironment_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SurfacePropertyLocalEnvironment");
  };

  /** \relates SurfacePropertyLocalEnvironment*/
  using OptionalSurfacePropertyLocalEnvironment = boost::optional<SurfacePropertyLocalEnvironment>;

  /** \relates SurfacePropertyLocalEnvironment*/
  using SurfacePropertyLocalEnvironmentVector = std::vector<SurfacePropertyLocalEnvironment>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYLOCALENVIRONMENT_HPP
