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

#ifndef MODEL_REFRIGERATIONCONDENSERWATERCOOLED_IMPL_HPP
#define MODEL_REFRIGERATIONCONDENSERWATERCOOLED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

  /** RefrigerationCondenserWaterCooled_Impl is a StraightComponent_Impl that is the implementation class for RefrigerationCondenserWaterCooled.*/
  class MODEL_API RefrigerationCondenserWaterCooled_Impl : public StraightComponent_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    RefrigerationCondenserWaterCooled_Impl(const IdfObject& idfObject,
                                           Model_Impl* model,
                                           bool keepHandle);

    RefrigerationCondenserWaterCooled_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle);

    RefrigerationCondenserWaterCooled_Impl(const RefrigerationCondenserWaterCooled_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle);

    virtual ~RefrigerationCondenserWaterCooled_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

	virtual unsigned inletPort();

    virtual unsigned outletPort();

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual bool addToNode(Node & node);

    //@}
    /** @name Getters */
    //@{

    boost::optional<double> ratedEffectiveTotalHeatRejectionRate() const;

    double ratedCondensingTemperature() const;

    double ratedSubcoolingTemperatureDifference() const;

    bool isRatedSubcoolingTemperatureDifferenceDefaulted() const;

    double ratedWaterInletTemperature() const;

    std::string waterCooledLoopFlowType() const;

    bool isWaterCooledLoopFlowTypeDefaulted() const;

    boost::optional<Schedule> waterOutletTemperatureSchedule() const;

    boost::optional<double> waterDesignFlowRate() const;

    boost::optional<double> waterMaximumFlowRate() const;

    double waterMaximumWaterOutletTemperature() const;

    bool isWaterMaximumWaterOutletTemperatureDefaulted() const;

    double waterMinimumWaterInletTemperature() const;

    bool isWaterMinimumWaterInletTemperatureDefaulted() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    boost::optional<double> condenserRefrigerantOperatingChargeInventory() const;

    boost::optional<double> condensateReceiverRefrigerantInventory() const;

    boost::optional<double> condensatePipingRefrigerantInventory() const;

    //@}
    /** @name Setters */
    //@{

    bool setRatedEffectiveTotalHeatRejectionRate(boost::optional<double> ratedEffectiveTotalHeatRejectionRate);

    void resetRatedEffectiveTotalHeatRejectionRate();

    bool setRatedCondensingTemperature(double ratedCondensingTemperature);

    bool setRatedSubcoolingTemperatureDifference(double ratedSubcoolingTemperatureDifference);

    void resetRatedSubcoolingTemperatureDifference();

    bool setRatedWaterInletTemperature(double ratedWaterInletTemperature);

    bool setWaterCooledLoopFlowType(std::string waterCooledLoopFlowType);

    void resetWaterCooledLoopFlowType();

    bool setWaterOutletTemperatureSchedule(Schedule& schedule);

    void resetWaterOutletTemperatureSchedule();

    bool setWaterDesignFlowRate(boost::optional<double> waterDesignFlowRate);

    void resetWaterDesignFlowRate();

    bool setWaterMaximumFlowRate(boost::optional<double> waterMaximumFlowRate);

    void resetWaterMaximumFlowRate();

    bool setWaterMaximumWaterOutletTemperature(double waterMaximumWaterOutletTemperature);

    void resetWaterMaximumWaterOutletTemperature();

    bool setWaterMinimumWaterInletTemperature(double waterMinimumWaterInletTemperature);

    void resetWaterMinimumWaterInletTemperature();

    void setEndUseSubcategory(std::string endUseSubcategory);

    void resetEndUseSubcategory();

    void setCondenserRefrigerantOperatingChargeInventory(boost::optional<double> condenserRefrigerantOperatingChargeInventory);

    void resetCondenserRefrigerantOperatingChargeInventory();

    void setCondensateReceiverRefrigerantInventory(boost::optional<double> condensateReceiverRefrigerantInventory);

    void resetCondensateReceiverRefrigerantInventory();

    void setCondensatePipingRefrigerantInventory(boost::optional<double> condensatePipingRefrigerantInventory);

    void resetCondensatePipingRefrigerantInventory();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationCondenserWaterCooled");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONCONDENSERWATERCOOLED_IMPL_HPP

