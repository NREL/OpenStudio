/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_REFRIGERATIONCONDENSERWATERCOOLED_IMPL_HPP
#define MODEL_REFRIGERATIONCONDENSERWATERCOOLED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class RefrigerationSystem;
  class Schedule;

  namespace detail {

    /** RefrigerationCondenserWaterCooled_Impl is a StraightComponent_Impl that is the implementation class for RefrigerationCondenserWaterCooled.*/
    class MODEL_API RefrigerationCondenserWaterCooled_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      RefrigerationCondenserWaterCooled_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RefrigerationCondenserWaterCooled_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RefrigerationCondenserWaterCooled_Impl(const RefrigerationCondenserWaterCooled_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RefrigerationCondenserWaterCooled_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

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

      bool setWaterCooledLoopFlowType(const std::string& waterCooledLoopFlowType);

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

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      void resetEndUseSubcategory();

      bool setCondenserRefrigerantOperatingChargeInventory(boost::optional<double> condenserRefrigerantOperatingChargeInventory);

      void resetCondenserRefrigerantOperatingChargeInventory();

      bool setCondensateReceiverRefrigerantInventory(boost::optional<double> condensateReceiverRefrigerantInventory);

      void resetCondensateReceiverRefrigerantInventory();

      bool setCondensatePipingRefrigerantInventory(boost::optional<double> condensatePipingRefrigerantInventory);

      void resetCondensatePipingRefrigerantInventory();

      //@}
      /** @name Other */
      //@{

      boost::optional<RefrigerationSystem> system() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.RefrigerationCondenserWaterCooled");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONCONDENSERWATERCOOLED_IMPL_HPP
