/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGWATERBASEBOARD_IMPL_HPP
#define MODEL_COILHEATINGWATERBASEBOARD_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** CoilHeatingWaterBaseboard_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingWaterBaseboard.*/
    class MODEL_API CoilHeatingWaterBaseboard_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingWaterBaseboard_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingWaterBaseboard_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingWaterBaseboard_Impl(const CoilHeatingWaterBaseboard_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingWaterBaseboard_Impl() override = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      std::string heatingDesignCapacityMethod() const;

      boost::optional<double> heatingDesignCapacity() const;

      bool isHeatingDesignCapacityAutosized() const;

      double heatingDesignCapacityPerFloorArea() const;

      double fractionofAutosizedHeatingDesignCapacity() const;

      boost::optional<double> uFactorTimesAreaValue() const;

      bool isUFactorTimesAreaValueDefaulted() const;

      bool isUFactorTimesAreaValueAutosized() const;

      boost::optional<double> maximumWaterFlowRate() const;

      bool isMaximumWaterFlowRateDefaulted() const;

      bool isMaximumWaterFlowRateAutosized() const;

      double convergenceTolerance() const;

      bool isConvergenceToleranceDefaulted() const;

      boost::optional<double> autosizedHeatingDesignCapacity() const;

      boost::optional<double> autosizedUFactorTimesAreaValue() const;

      boost::optional<double> autosizedMaximumWaterFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{
      bool setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod);

      bool setHeatingDesignCapacity(boost::optional<double> heatingDesignCapacity);

      void autosizeHeatingDesignCapacity();

      bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

      bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

      bool setUFactorTimesAreaValue(boost::optional<double> uFactorTimesAreaValue);

      void resetUFactorTimesAreaValue();

      void autosizeUFactorTimesAreaValue();

      bool setMaximumWaterFlowRate(boost::optional<double> maximumWaterFlowRate);

      void resetMaximumWaterFlowRate();

      void autosizeMaximumWaterFlowRate();

      bool setConvergenceTolerance(double convergenceTolerance);

      void resetConvergenceTolerance();

      //@}
      /** @name Other */
      //@{

      bool addToNode(Node& node) override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingWaterBaseboard");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGWATERBASEBOARD_IMPL_HPP
