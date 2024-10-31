/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGWATERBASEBOARDRADIANT_IMPL_HPP
#define MODEL_COILHEATINGWATERBASEBOARDRADIANT_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"
#include "ZoneHVACComponent.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** CoilHeatingWaterBaseboardRadiant_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingWaterBaseboardRadiant.*/
    class MODEL_API CoilHeatingWaterBaseboardRadiant_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingWaterBaseboardRadiant_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingWaterBaseboardRadiant_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingWaterBaseboardRadiant_Impl(const CoilHeatingWaterBaseboardRadiant_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingWaterBaseboardRadiant_Impl() override = default;

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

      double ratedAverageWaterTemperature() const;

      double ratedWaterMassFlowRate() const;

      std::string heatingDesignCapacityMethod() const;

      boost::optional<double> heatingDesignCapacity() const;

      bool isHeatingDesignCapacityAutosized() const;

      double heatingDesignCapacityPerFloorArea() const;

      double fractionofAutosizedHeatingDesignCapacity() const;

      boost::optional<double> maximumWaterFlowRate() const;

      bool isMaximumWaterFlowRateAutosized() const;

      double convergenceTolerance() const;

      boost::optional<double> autosizedHeatingDesignCapacity() const;

      boost::optional<double> autosizedMaximumWaterFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setRatedAverageWaterTemperature(double ratedAverageWaterTemperature);

      bool setRatedWaterMassFlowRate(double ratedWaterMassFlowRate);

      bool setHeatingDesignCapacityMethod(const std::string& heatingDesignCapacityMethod);

      bool setHeatingDesignCapacity(boost::optional<double> heatingDesignCapacity);

      void autosizeHeatingDesignCapacity();

      bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

      bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

      bool setMaximumWaterFlowRate(boost::optional<double> maximumWaterFlowRate);

      void autosizeMaximumWaterFlowRate();

      bool setConvergenceTolerance(double convergenceTolerance);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingWaterBaseboardRadiant");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGWATERBASEBOARDRADIANT_IMPL_HPP
