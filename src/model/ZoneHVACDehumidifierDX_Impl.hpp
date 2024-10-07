/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACDEHUMIDIFIERDX_IMPL_HPP
#define MODEL_ZONEHVACDEHUMIDIFIERDX_IMPL_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Curve;
  // class WaterStorageTank;

  namespace detail {

    /** ZoneHVACDehumidifierDX_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACDehumidifierDX.*/
    class MODEL_API ZoneHVACDehumidifierDX_Impl : public ZoneHVACComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneHVACDehumidifierDX_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACDehumidifierDX_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACDehumidifierDX_Impl(const ZoneHVACDehumidifierDX_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneHVACDehumidifierDX_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      double ratedWaterRemoval() const;

      double ratedEnergyFactor() const;

      double ratedAirFlowRate() const;

      Curve waterRemovalCurve() const;

      Curve energyFactorCurve() const;

      Curve partLoadFractionCorrelationCurve() const;

      double minimumDryBulbTemperatureforDehumidifierOperation() const;

      double maximumDryBulbTemperatureforDehumidifierOperation() const;

      double offCycleParasiticElectricLoad() const;

      // boost::optional<WaterStorageTank> condensateCollectionWaterStorageTank() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setRatedWaterRemoval(double ratedWaterRemoval);

      bool setRatedEnergyFactor(double ratedEnergyFactor);

      bool setRatedAirFlowRate(double ratedAirFlowRate);

      bool setWaterRemovalCurve(const Curve& curve);

      bool setEnergyFactorCurve(const Curve& curve);

      bool setPartLoadFractionCorrelationCurve(const Curve& curve);

      bool setMinimumDryBulbTemperatureforDehumidifierOperation(double minimumDryBulbTemperatureforDehumidifierOperation);

      bool setMaximumDryBulbTemperatureforDehumidifierOperation(double maximumDryBulbTemperatureforDehumidifierOperation);

      bool setOffCycleParasiticElectricLoad(double offCycleParasiticElectricLoad);

      // bool setCondensateCollectionWaterStorageTank(const boost::optional<WaterStorageTank>& waterStorageTank);

      // void resetCondensateCollectionWaterStorageTank();

      //@}
      /** @name Other */
      //@{

      ModelObject clone(Model model) const override;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACDehumidifierDX");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<Curve> optionalWaterRemovalCurve() const;
      boost::optional<Curve> optionalEnergyFactorCurve() const;
      boost::optional<Curve> optionalPartLoadFractionCorrelationCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACDEHUMIDIFIERDX_IMPL_HPP
