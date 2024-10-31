/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_DAYLIGHTINGDEVICETUBULAR_HPP
#define MODEL_DAYLIGHTINGDEVICETUBULAR_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"

namespace openstudio {
namespace model {

  class SubSurface;
  class ConstructionBase;
  class ThermalZone;

  // Note: TransitionZone is a class defined in ThermalZone.hpp so that the order things are exposed to SWIG makes sense
  class TransitionZone;

  namespace detail {

    class DaylightingDeviceTubular_Impl;

  }  // namespace detail

  /** DaylightingDeviceTubular is a ModelObject that wraps the OpenStudio IDD object 'OS_DaylightingDevice_Tubular'. */
  class MODEL_API DaylightingDeviceTubular : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit DaylightingDeviceTubular(const SubSurface& dome, const SubSurface& diffuser, const ConstructionBase& construction, double diameter,
                                      double totalLength, double effectiveThermalResistance);

    explicit DaylightingDeviceTubular(const SubSurface& dome, const SubSurface& diffuser, const ConstructionBase& construction);

    virtual ~DaylightingDeviceTubular() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    DaylightingDeviceTubular(const DaylightingDeviceTubular& other) = default;
    DaylightingDeviceTubular(DaylightingDeviceTubular&& other) = default;
    DaylightingDeviceTubular& operator=(const DaylightingDeviceTubular&) = default;
    DaylightingDeviceTubular& operator=(DaylightingDeviceTubular&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    SubSurface subSurfaceDome() const;

    SubSurface subSurfaceDiffuser() const;

    ConstructionBase construction() const;

    double diameter() const;

    double totalLength() const;

    double effectiveThermalResistance() const;

    std::vector<TransitionZone> transitionZones() const;

    unsigned int numberofTransitionZones() const;

    //@}
    /** @name Setters */
    //@{

    bool setConstruction(const ConstructionBase& construction);

    bool setDiameter(double diameter);

    bool setTotalLength(double totalLength);

    bool setEffectiveThermalResistance(double effectiveThermalResistance);

    bool addTransitionZone(const TransitionZone& transitionZone);

    bool addTransitionZone(const ThermalZone& zone, double length);

    void removeTransitionZone(int groupIndex);

    void removeAllTransitionZones();

    bool addTransitionZones(const std::vector<TransitionZone>& transitionZones);

    //@}

   protected:
    /// @cond
    using ImplType = detail::DaylightingDeviceTubular_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    explicit DaylightingDeviceTubular(std::shared_ptr<detail::DaylightingDeviceTubular_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.DaylightingDeviceTubular");
  };

  /** \relates DaylightingDeviceTubular*/
  using OptionalDaylightingDeviceTubular = boost::optional<DaylightingDeviceTubular>;

  /** \relates DaylightingDeviceTubular*/
  using DaylightingDeviceTubularVector = std::vector<DaylightingDeviceTubular>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_DAYLIGHTINGDEVICETUBULAR_HPP
