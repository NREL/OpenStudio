/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONWALKINZONEBOUNDARY_HPP
#define MODEL_REFRIGERATIONWALKINZONEBOUNDARY_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

namespace model {

  class ThermalZone;
  class Schedule;

  namespace detail {

    class RefrigerationWalkInZoneBoundary_Impl;

  }  // namespace detail

  /** RefrigerationWalkInZoneBoundary is a ModelObject that wraps the OpenStudio IDD object 'OS:Refrigeration:WalkIn:ZoneBoundary'. */
  class MODEL_API RefrigerationWalkInZoneBoundary : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit RefrigerationWalkInZoneBoundary(const Model& model);

    virtual ~RefrigerationWalkInZoneBoundary() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RefrigerationWalkInZoneBoundary(const RefrigerationWalkInZoneBoundary& other) = default;
    RefrigerationWalkInZoneBoundary(RefrigerationWalkInZoneBoundary&& other) = default;
    RefrigerationWalkInZoneBoundary& operator=(const RefrigerationWalkInZoneBoundary&) = default;
    RefrigerationWalkInZoneBoundary& operator=(RefrigerationWalkInZoneBoundary&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> stockingDoorOpeningProtectionTypeFacingZoneValues();

    /** @name Getters */
    //@{

    boost::optional<ThermalZone> thermalZone() const;

    double totalInsulatedSurfaceAreaFacingZone() const;

    double insulatedSurfaceUValueFacingZone() const;

    bool isInsulatedSurfaceUValueFacingZoneDefaulted() const;

    double areaofGlassReachInDoorsFacingZone() const;

    bool isAreaofGlassReachInDoorsFacingZoneDefaulted() const;

    double heightofGlassReachInDoorsFacingZone() const;

    bool isHeightofGlassReachInDoorsFacingZoneDefaulted() const;

    double glassReachInDoorUValueFacingZone() const;

    bool isGlassReachInDoorUValueFacingZoneDefaulted() const;

    boost::optional<Schedule> glassReachInDoorOpeningScheduleFacingZone() const;

    double areaofStockingDoorsFacingZone() const;

    bool isAreaofStockingDoorsFacingZoneDefaulted() const;

    double heightofStockingDoorsFacingZone() const;

    bool isHeightofStockingDoorsFacingZoneDefaulted() const;

    double stockingDoorUValueFacingZone() const;

    bool isStockingDoorUValueFacingZoneDefaulted() const;

    boost::optional<Schedule> stockingDoorOpeningScheduleFacingZone() const;

    std::string stockingDoorOpeningProtectionTypeFacingZone() const;

    bool isStockingDoorOpeningProtectionTypeFacingZoneDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setThermalZone(const ThermalZone& thermalZone);

    void resetThermalZone();

    bool setTotalInsulatedSurfaceAreaFacingZone(double totalInsulatedSurfaceAreaFacingZone);

    bool setInsulatedSurfaceUValueFacingZone(double insulatedSurfaceUValueFacingZone);

    void resetInsulatedSurfaceUValueFacingZone();

    bool setAreaofGlassReachInDoorsFacingZone(double areaofGlassReachInDoorsFacingZone);

    void resetAreaofGlassReachInDoorsFacingZone();

    bool setHeightofGlassReachInDoorsFacingZone(double heightofGlassReachInDoorsFacingZone);

    void resetHeightofGlassReachInDoorsFacingZone();

    bool setGlassReachInDoorUValueFacingZone(double glassReachInDoorUValueFacingZone);

    void resetGlassReachInDoorUValueFacingZone();

    bool setGlassReachInDoorOpeningScheduleFacingZone(Schedule& schedule);

    void resetGlassReachInDoorOpeningScheduleFacingZone();

    bool setAreaofStockingDoorsFacingZone(double areaofStockingDoorsFacingZone);

    void resetAreaofStockingDoorsFacingZone();

    bool setHeightofStockingDoorsFacingZone(double heightofStockingDoorsFacingZone);

    void resetHeightofStockingDoorsFacingZone();

    bool setStockingDoorUValueFacingZone(double stockingDoorUValueFacingZone);

    void resetStockingDoorUValueFacingZone();

    bool setStockingDoorOpeningScheduleFacingZone(Schedule& schedule);

    void resetStockingDoorOpeningScheduleFacingZone();

    bool setStockingDoorOpeningProtectionTypeFacingZone(const std::string& stockingDoorOpeningProtectionTypeFacingZone);

    void resetStockingDoorOpeningProtectionTypeFacingZone();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::RefrigerationWalkInZoneBoundary_Impl;

    explicit RefrigerationWalkInZoneBoundary(std::shared_ptr<detail::RefrigerationWalkInZoneBoundary_Impl> impl);

    friend class detail::RefrigerationWalkInZoneBoundary_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationWalkInZoneBoundary");
  };

  /** \relates RefrigerationWalkInZoneBoundary*/
  using OptionalRefrigerationWalkInZoneBoundary = boost::optional<RefrigerationWalkInZoneBoundary>;

  /** \relates RefrigerationWalkInZoneBoundary*/
  using RefrigerationWalkInZoneBoundaryVector = std::vector<RefrigerationWalkInZoneBoundary>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONWALKINZONEBOUNDARY_HPP
