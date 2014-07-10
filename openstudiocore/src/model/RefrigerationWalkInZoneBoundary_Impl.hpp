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
  class MODEL_API RefrigerationWalkInZoneBoundary_Impl : public ModelObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    RefrigerationWalkInZoneBoundary_Impl(const IdfObject& idfObject,
                                         Model_Impl* model,
                                         bool keepHandle);

    RefrigerationWalkInZoneBoundary_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    RefrigerationWalkInZoneBoundary_Impl(const RefrigerationWalkInZoneBoundary_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    virtual ~RefrigerationWalkInZoneBoundary_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual ModelObject clone(Model model) const;

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

    void setAreaofGlassReachInDoorsFacingZone(double areaofGlassReachInDoorsFacingZone);

    void resetAreaofGlassReachInDoorsFacingZone();

    void setHeightofGlassReachInDoorsFacingZone(double heightofGlassReachInDoorsFacingZone);

    void resetHeightofGlassReachInDoorsFacingZone();

    bool setGlassReachInDoorUValueFacingZone(double glassReachInDoorUValueFacingZone);

    void resetGlassReachInDoorUValueFacingZone();

    bool setGlassReachInDoorOpeningScheduleFacingZone(Schedule& schedule);

    void resetGlassReachInDoorOpeningScheduleFacingZone();

    void setAreaofStockingDoorsFacingZone(double areaofStockingDoorsFacingZone);

    void resetAreaofStockingDoorsFacingZone();

    void setHeightofStockingDoorsFacingZone(double heightofStockingDoorsFacingZone);

    void resetHeightofStockingDoorsFacingZone();

    bool setStockingDoorUValueFacingZone(double stockingDoorUValueFacingZone);

    void resetStockingDoorUValueFacingZone();

    bool setStockingDoorOpeningScheduleFacingZone(Schedule& schedule);

    void resetStockingDoorOpeningScheduleFacingZone();

    bool setStockingDoorOpeningProtectionTypeFacingZone(std::string stockingDoorOpeningProtectionTypeFacingZone);

    void resetStockingDoorOpeningProtectionTypeFacingZone();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationWalkInZoneBoundary");

  };

} // detail

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONWALKINZONEBOUNDARY_IMPL_HPP

