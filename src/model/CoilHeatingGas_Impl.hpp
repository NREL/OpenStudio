/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILHEATINGGAS_IMPL_HPP
#define MODEL_COILHEATINGGAS_IMPL_HPP

#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;

  namespace detail {

    class MODEL_API CoilHeatingGas_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //2{

      // constructor
      CoilHeatingGas_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      // construct from workspace
      CoilHeatingGas_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      // copy constructor
      CoilHeatingGas_Impl(const CoilHeatingGas_Impl& other, Model_Impl* model, bool keepHandle);

      // virtual destructor
      virtual ~CoilHeatingGas_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      // Get all output variable names that could be associated with this object.
      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;
      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters and Setters */
      //@{

      Schedule availabilitySchedule() const;

      bool setAvailabilitySchedule(Schedule& schedule);

      std::string fuelType() const;

      bool setFuelType(const std::string& fuelType);

      void resetFuelType();

      // Get GasBurnerEfficiency
      double gasBurnerEfficiency() const;

      // Set GasBurnerEfficiency
      bool setGasBurnerEfficiency(double val);

      // Get ParasiticElectricLoad
      double onCycleParasiticElectricLoad() const;

      // Set OnCycleParasiticElectricLoad
      bool setOnCycleParasiticElectricLoad(double val);

      // Get OffCycleParasiticGasLoad
      double offCycleParasiticGasLoad() const;

      // Set OffCycleParasiticGasLoad
      bool setOffCycleParasiticGasLoad(double val);

      boost::optional<double> nominalCapacity() const;

      bool isNominalCapacityAutosized() const;

      bool setNominalCapacity(boost::optional<double> nominalCapacity);

      void resetNominalCapacity();

      void autosizeNominalCapacity();

      boost::optional<Curve> partLoadFractionCorrelationCurve() const;

      bool setPartLoadFractionCorrelationCurve(const Curve& curve);

      void resetPartLoadFractionCorrelationCurve();

      std::vector<ModelObject> children() const override;

      ModelObject clone(Model model) const override;

      bool addToNode(Node& node) override;

      AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);
      boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

      boost::optional<double> autosizedNominalCapacity() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingGas");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<Schedule> optionalAvailabilitySchedule() const;

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGGAS_IMPL_HPP
