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

#ifndef MODEL_REFRIGERATIONGASCOOLERAIRCOOLED_IMPL_HPP
#define MODEL_REFRIGERATIONGASCOOLERAIRCOOLED_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

class CurveLinear;
// class ThermalZone;

namespace detail {

  /** RefrigerationGasCoolerAirCooled_Impl is a ParentObject_Impl that is the implementation class for RefrigerationGasCoolerAirCooled.*/
  class MODEL_API RefrigerationGasCoolerAirCooled_Impl : public ParentObject_Impl {
//    Q_OBJECT;
   public:
    /** @name Constructors and Destructors */
    //@{

    RefrigerationGasCoolerAirCooled_Impl(const IdfObject& idfObject,
                                         Model_Impl* model,
                                         bool keepHandle);

    RefrigerationGasCoolerAirCooled_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    RefrigerationGasCoolerAirCooled_Impl(const RefrigerationGasCoolerAirCooled_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    virtual ~RefrigerationGasCoolerAirCooled_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual ModelObject clone(Model model) const;

    virtual std::vector<IddObjectType> allowableChildTypes() const;

    virtual std::vector<ModelObject> children() const;

    //@}
    /** @name Getters */
    //@{

    boost::optional<CurveLinear> ratedTotalHeatRejectionRateCurve() const;

    std::string gasCoolerFanSpeedControlType() const;

    bool isGasCoolerFanSpeedControlTypeDefaulted() const;

    double ratedFanPower() const;

    bool isRatedFanPowerDefaulted() const;

    double minimumFanAirFlowRatio() const;

    bool isMinimumFanAirFlowRatioDefaulted() const;

    double transitionTemperature() const;

    bool isTransitionTemperatureDefaulted() const;

    double transcriticalApproachTemperature() const;

    bool isTranscriticalApproachTemperatureDefaulted() const;

    double subcriticalTemperatureDifference() const;

    bool isSubcriticalTemperatureDifferenceDefaulted() const;

    double minimumCondensingTemperature() const;

    bool isMinimumCondensingTemperatureDefaulted() const;

    // boost::optional<ThermalZone> airInletNode() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    double gasCoolerRefrigerantOperatingChargeInventory() const;

    bool isGasCoolerRefrigerantOperatingChargeInventoryDefaulted() const;

    double gasCoolerReceiverRefrigerantInventory() const;

    bool isGasCoolerReceiverRefrigerantInventoryDefaulted() const;

    double gasCoolerOutletPipingRefrigerantInventory() const;

    bool isGasCoolerOutletPipingRefrigerantInventoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedTotalHeatRejectionRateCurve(const boost::optional<CurveLinear>& curveLinear);

    void resetRatedTotalHeatRejectionRateCurve();

    bool setGasCoolerFanSpeedControlType(std::string gasCoolerFanSpeedControlType);

    void resetGasCoolerFanSpeedControlType();

    bool setRatedFanPower(double ratedFanPower);

    void resetRatedFanPower();

    bool setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio);

    void resetMinimumFanAirFlowRatio();

    void setTransitionTemperature(double transitionTemperature);

    void resetTransitionTemperature();

    void setTranscriticalApproachTemperature(double transcriticalApproachTemperature);

    void resetTranscriticalApproachTemperature();

    void setSubcriticalTemperatureDifference(double subcriticalTemperatureDifference);

    void resetSubcriticalTemperatureDifference();

    void setMinimumCondensingTemperature(double minimumCondensingTemperature);

    void resetMinimumCondensingTemperature();

    // bool setAirInletNode(const boost::optional<ThermalZone>& thermalZone);

    // void resetAirInletNode();

    void setEndUseSubcategory(std::string endUseSubcategory);

    void resetEndUseSubcategory();

    void setGasCoolerRefrigerantOperatingChargeInventory(double gasCoolerRefrigerantOperatingChargeInventory);

    void resetGasCoolerRefrigerantOperatingChargeInventory();

    void setGasCoolerReceiverRefrigerantInventory(double gasCoolerReceiverRefrigerantInventory);

    void resetGasCoolerReceiverRefrigerantInventory();

    void setGasCoolerOutletPipingRefrigerantInventory(double gasCoolerOutletPipingRefrigerantInventory);

    void resetGasCoolerOutletPipingRefrigerantInventory();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationGasCoolerAirCooled");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONGASCOOLERAIRCOOLED_IMPL_HPP

