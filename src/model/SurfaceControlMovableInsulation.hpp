/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SURFACECONTROLMOVABLEINSULATION_HPP
#define MODEL_SURFACECONTROLMOVABLEINSULATION_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class Surface;
  class Material;
  class Schedule;

  namespace detail {

    class SurfaceControlMovableInsulation_Impl;

  }  // namespace detail

  /** SurfaceControlMovableInsulation is a ModelObject that wraps the OpenStudio IDD object 'OS:SurfaceControl:MovableInsulation'. */
  class MODEL_API SurfaceControlMovableInsulation : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    // Constructor takes required-field arguments Surface and Material. The Schedule is defaulted to alwaysOnContinuousSchedule
    explicit SurfaceControlMovableInsulation(const Surface& surface, const Material& material);

    virtual ~SurfaceControlMovableInsulation() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SurfaceControlMovableInsulation(const SurfaceControlMovableInsulation& other) = default;
    SurfaceControlMovableInsulation(SurfaceControlMovableInsulation&& other) = default;
    SurfaceControlMovableInsulation& operator=(const SurfaceControlMovableInsulation&) = default;
    SurfaceControlMovableInsulation& operator=(SurfaceControlMovableInsulation&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> insulationTypeValues();

    static std::vector<std::string> validInsulationTypeValues();

    /** @name Getters */
    //@{

    std::string insulationType() const;

    Surface surface() const;

    Material material() const;

    Schedule schedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setInsulationType(const std::string& insulationType);

    /** Note that if the target surface already has a SurfaceControlMovableInsulation attached to it, it will be removed as there can be only one
    * SurfaceControlMovableInsulation per Surface. */
    bool setSurface(const Surface& surface);

    bool setMaterial(const Material& material);

    bool setSchedule(Schedule& schedule);

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SurfaceControlMovableInsulation_Impl;

    explicit SurfaceControlMovableInsulation(std::shared_ptr<detail::SurfaceControlMovableInsulation_Impl> impl);

    friend class detail::SurfaceControlMovableInsulation_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SurfaceControlMovableInsulation");
  };

  /** \relates SurfaceControlMovableInsulation*/
  using OptionalSurfaceControlMovableInsulation = boost::optional<SurfaceControlMovableInsulation>;

  /** \relates SurfaceControlMovableInsulation*/
  using SurfaceControlMovableInsulationVector = std::vector<SurfaceControlMovableInsulation>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACECONTROLMOVABLEINSULATION_HPP
