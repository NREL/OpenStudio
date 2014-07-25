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

#ifndef MODEL_REFRIGERATIONAIRCHILLER_IMPL_HPP
#define MODEL_REFRIGERATIONAIRCHILLER_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;
// class CurveLinear;

namespace detail {

  /** RefrigerationAirChiller_Impl is a ZoneHVACComponent_Impl that is the implementation class for RefrigerationAirChiller.*/
  class MODEL_API RefrigerationAirChiller_Impl : public ZoneHVACComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    RefrigerationAirChiller_Impl(const IdfObject& idfObject,
                                         Model_Impl* model,
                                         bool keepHandle);

    RefrigerationAirChiller_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    RefrigerationAirChiller_Impl(const RefrigerationAirChiller_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    virtual ~RefrigerationAirChiller_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual ModelObject clone(Model model) const;

    virtual std::vector<IdfObject> remove();

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    // virtual std::vector<ModelObject> children() const;

    virtual unsigned inletPort();

    virtual unsigned outletPort();

    virtual bool addToThermalZone(ThermalZone & thermalZone);

    virtual void removeFromThermalZone();

    virtual boost::optional<ThermalZone> thermalZone();

    //@}
    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    std::string capacityRatingType() const;

    boost::optional<double> ratedUnitLoadFactor() const;

    boost::optional<double> ratedCapacity() const;

    double ratedRelativeHumidity() const;

    bool isRatedRelativeHumidityDefaulted() const;

    double ratedCoolingSourceTemperature() const;

    double ratedTemperatureDifferenceDT1() const;

    boost::optional<double> maximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature() const;

    double coilMaterialCorrectionFactor() const;

    bool isCoilMaterialCorrectionFactorDefaulted() const;

    double refrigerantCorrectionFactor() const;

    bool isRefrigerantCorrectionFactorDefaulted() const;

    // std::string capacityCorrectionCurveType() const;

    // bool isCapacityCorrectionCurveTypeDefaulted() const;

    // boost::optional<CurveLinear> capacityCorrectionCurve() const;

    double sHR60CorrectionFactor() const;

    bool isSHR60CorrectionFactorDefaulted() const;

    double ratedTotalHeatingPower() const;

    boost::optional<Schedule> heatingPowerSchedule() const;

    std::string fanSpeedControlType() const;

    bool isFanSpeedControlTypeDefaulted() const;

    double ratedFanPower() const;

    bool isRatedFanPowerDefaulted() const;

    double ratedAirFlow() const;

    double minimumFanAirFlowRatio() const;

    bool isMinimumFanAirFlowRatioDefaulted() const;

    std::string defrostType() const;

    bool isDefrostTypeDefaulted() const;

    std::string defrostControlType() const;

    bool isDefrostControlTypeDefaulted() const;

    Schedule defrostSchedule() const;

    boost::optional<Schedule> defrostDripDownSchedule() const;

    boost::optional<double> defrostPower() const;

    boost::optional<double> temperatureTerminationDefrostFractiontoIce() const;

    std::string verticalLocation() const;

    bool isVerticalLocationDefaulted() const;

    double averageRefrigerantChargeInventory() const;

    bool isAverageRefrigerantChargeInventoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setCapacityRatingType(std::string capacityRatingType);

    void setRatedUnitLoadFactor(boost::optional<double> ratedUnitLoadFactor);

    void resetRatedUnitLoadFactor();

    void setRatedCapacity(boost::optional<double> ratedCapacity);

    void resetRatedCapacity();

    bool setRatedRelativeHumidity(double ratedRelativeHumidity);

    void resetRatedRelativeHumidity();

    bool setRatedCoolingSourceTemperature(double ratedCoolingSourceTemperature);

    bool setRatedTemperatureDifferenceDT1(double ratedTemperatureDifferenceDT1);

    bool setMaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature(boost::optional<double> maximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature);

    void resetMaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature();

    void setCoilMaterialCorrectionFactor(double coilMaterialCorrectionFactor);

    void resetCoilMaterialCorrectionFactor();

    void setRefrigerantCorrectionFactor(double refrigerantCorrectionFactor);

    void resetRefrigerantCorrectionFactor();

    // bool setCapacityCorrectionCurveType(std::string capacityCorrectionCurveType);

    // void resetCapacityCorrectionCurveType();

    // bool setCapacityCorrectionCurve(const boost::optional<CurveLinear>& curveLinear);

    // void resetCapacityCorrectionCurve();

    bool setSHR60CorrectionFactor(double sHR60CorrectionFactor);

    void resetSHR60CorrectionFactor();

    void setRatedTotalHeatingPower(double ratedTotalHeatingPower);

    bool setHeatingPowerSchedule(Schedule& schedule);

    void resetHeatingPowerSchedule();

    bool setFanSpeedControlType(std::string fanSpeedControlType);

    void resetFanSpeedControlType();

    bool setRatedFanPower(double ratedFanPower);

    void resetRatedFanPower();

    void setRatedAirFlow(double ratedAirFlow);

    bool setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio);

    void resetMinimumFanAirFlowRatio();

    bool setDefrostType(std::string defrostType);

    void resetDefrostType();

    bool setDefrostControlType(std::string defrostControlType);

    void resetDefrostControlType();

    bool setDefrostSchedule(Schedule& schedule);

    bool setDefrostDripDownSchedule(Schedule& schedule);

    void resetDefrostDripDownSchedule();

    bool setDefrostPower(boost::optional<double> defrostPower);

    void resetDefrostPower();

    bool setTemperatureTerminationDefrostFractiontoIce(boost::optional<double> temperatureTerminationDefrostFractiontoIce);

    void resetTemperatureTerminationDefrostFractiontoIce();

    bool setVerticalLocation(std::string verticalLocation);

    void resetVerticalLocation();

    void setAverageRefrigerantChargeInventory(double averageRefrigerantChargeInventory);

    void resetAverageRefrigerantChargeInventory();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationAirChiller");

    // Optional getters for use by methods like children() so can remove() if the constructor fails.
    // There are other ways for the public versions of these getters to fail--perhaps all required
    // objects should be returned as boost::optionals
    boost::optional<Schedule> optionalDefrostSchedule() const;
  };

} // detail

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONAIRCHILLER_IMPL_HPP

