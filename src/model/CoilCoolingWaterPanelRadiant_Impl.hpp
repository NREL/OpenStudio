/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILCOOLINGWATERPANELRADIANT_IMPL_HPP
#define MODEL_COILCOOLINGWATERPANELRADIANT_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** CoilCoolingWaterPanelRadiant_Impl is a StraightComponent_Impl that is the implementation class for CoilCoolingWaterPanelRadiant.*/
    class MODEL_API CoilCoolingWaterPanelRadiant_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingWaterPanelRadiant_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingWaterPanelRadiant_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingWaterPanelRadiant_Impl(const CoilCoolingWaterPanelRadiant_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingWaterPanelRadiant_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      double ratedInletWaterTemperature() const;

      double ratedInletSpaceTemperature() const;

      double ratedWaterMassFlowRate() const;

      std::string coolingDesignCapacityMethod() const;

      boost::optional<double> coolingDesignCapacity() const;

      bool isCoolingDesignCapacityAutosized() const;

      double coolingDesignCapacityPerFloorArea() const;

      double fractionofAutosizedCoolingDesignCapacity() const;

      boost::optional<double> maximumChilledWaterFlowRate() const;

      bool isMaximumChilledWaterFlowRateAutosized() const;

      std::string controlType() const;

      double coolingControlThrottlingRange() const;

      boost::optional<Schedule> coolingControlTemperatureSchedule() const;

      std::string condensationControlType() const;

      double condensationControlDewpointOffset() const;

      boost::optional<double> autosizedCoolingDesignCapacity() const;

      boost::optional<double> autosizedMaximumChilledWaterFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setRatedInletWaterTemperature(double ratedInletWaterTemperature);

      bool setRatedInletSpaceTemperature(double ratedInletSpaceTemperature);

      bool setRatedWaterMassFlowRate(double ratedWaterMassFlowRate);

      bool setCoolingDesignCapacityMethod(const std::string& coolingDesignCapacityMethod);

      bool setCoolingDesignCapacity(boost::optional<double> coolingDesignCapacity);

      void autosizeCoolingDesignCapacity();

      bool setCoolingDesignCapacityPerFloorArea(double coolingDesignCapacityPerFloorArea);

      bool setFractionofAutosizedCoolingDesignCapacity(double fractionofAutosizedCoolingDesignCapacity);

      bool setMaximumChilledWaterFlowRate(boost::optional<double> maximumChilledWaterFlowRate);

      void autosizeMaximumChilledWaterFlowRate();

      bool setControlType(const std::string& controlType);

      bool setCoolingControlThrottlingRange(double coolingControlThrottlingRange);

      bool setCoolingControlTemperatureSchedule(Schedule& coolingControlTemperatureSchedule);

      void resetCoolingControlTemperatureSchedule();

      bool setCondensationControlType(const std::string& condensationControlType);

      bool setCondensationControlDewpointOffset(double condensationControlDewpointOffset);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingWaterPanelRadiant");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGWATERPANELRADIANT_IMPL_HPP
