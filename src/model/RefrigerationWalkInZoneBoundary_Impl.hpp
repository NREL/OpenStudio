/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONWALKINZONEBOUNDARY_IMPL_HPP
#define MODEL_REFRIGERATIONWALKINZONEBOUNDARY_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"

namespace openstudio {
namespace model {

  class ThermalZone;
  class Schedule;

  namespace detail {

    /** RefrigerationWalkInZoneBoundary_Impl is a ModelObject_Impl that is the implementation class for RefrigerationWalkInZoneBoundary.*/
    class MODEL_API RefrigerationWalkInZoneBoundary_Impl : public ModelObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      RefrigerationWalkInZoneBoundary_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RefrigerationWalkInZoneBoundary_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RefrigerationWalkInZoneBoundary_Impl(const RefrigerationWalkInZoneBoundary_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RefrigerationWalkInZoneBoundary_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual ModelObject clone(Model model) const override;

      //@}
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
     private:
      REGISTER_LOGGER("openstudio.model.RefrigerationWalkInZoneBoundary");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONWALKINZONEBOUNDARY_IMPL_HPP
