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

#ifndef MODEL_REFRIGERATIONCONDENSERAIRCOOLED_IMPL_HPP
#define MODEL_REFRIGERATIONCONDENSERAIRCOOLED_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

class CurveLinear;
class ThermalZone;

namespace detail {

  /** RefrigerationCondenserAirCooled_Impl is a ParentObject that is the implementation class for RefrigerationCondenserAirCooled.*/
  class MODEL_API RefrigerationCondenserAirCooled_Impl : public ParentObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    RefrigerationCondenserAirCooled_Impl(const IdfObject& idfObject,
                                         Model_Impl* model,
                                         bool keepHandle);

    RefrigerationCondenserAirCooled_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    RefrigerationCondenserAirCooled_Impl(const RefrigerationCondenserAirCooled_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle);

    virtual ~RefrigerationCondenserAirCooled_Impl() {}

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

    boost::optional<CurveLinear> ratedEffectiveTotalHeatRejectionRateCurve() const;

    double ratedSubcoolingTemperatureDifference() const;

    bool isRatedSubcoolingTemperatureDifferenceDefaulted() const;

    std::string condenserFanSpeedControlType() const;

    bool isCondenserFanSpeedControlTypeDefaulted() const;

    double ratedFanPower() const;

    bool isRatedFanPowerDefaulted() const;

    double minimumFanAirFlowRatio() const;

    bool isMinimumFanAirFlowRatioDefaulted() const;

    boost::optional<ThermalZone> airInletZone() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    double condenserRefrigerantOperatingChargeInventory() const;

    bool isCondenserRefrigerantOperatingChargeInventoryDefaulted() const;

    double condensateReceiverRefrigerantInventory() const;

    bool isCondensateReceiverRefrigerantInventoryDefaulted() const;

    double condensatePipingRefrigerantInventory() const;

    bool isCondensatePipingRefrigerantInventoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedEffectiveTotalHeatRejectionRateCurve(const boost::optional<CurveLinear>& curveLinear);

    void resetRatedEffectiveTotalHeatRejectionRateCurve();

    bool setRatedSubcoolingTemperatureDifference(double ratedSubcoolingTemperatureDifference);

    void resetRatedSubcoolingTemperatureDifference();

    bool setCondenserFanSpeedControlType(std::string condenserFanSpeedControlType);

    void resetCondenserFanSpeedControlType();

    bool setRatedFanPower(double ratedFanPower);

    void resetRatedFanPower();

    bool setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio);

    void resetMinimumFanAirFlowRatio();

    bool setAirInletZone(const boost::optional<ThermalZone>& thermalZone);

    void resetAirInletZone();

    void setEndUseSubcategory(std::string endUseSubcategory);

    void resetEndUseSubcategory();

    void setCondenserRefrigerantOperatingChargeInventory(double condenserRefrigerantOperatingChargeInventory);

    void resetCondenserRefrigerantOperatingChargeInventory();

    void setCondensateReceiverRefrigerantInventory(double condensateReceiverRefrigerantInventory);

    void resetCondensateReceiverRefrigerantInventory();

    void setCondensatePipingRefrigerantInventory(double condensatePipingRefrigerantInventory);

    void resetCondensatePipingRefrigerantInventory();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationCondenserAirCooled");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONCONDENSERAIRCOOLED_IMPL_HPP

