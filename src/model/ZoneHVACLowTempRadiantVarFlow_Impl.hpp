/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ZONEHVACLOWTEMPRADIANTVARFLOW_IMPL_HPP
#define MODEL_ZONEHVACLOWTEMPRADIANTVARFLOW_IMPL_HPP

#include "ModelAPI.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "ZoneHVACComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class HVACComponent;

  namespace detail {

    /** ZoneHVACLowTempRadiantVarFlow_Impl is a ZoneHVACComponent_Impl that is the implementation class for ZoneHVACLowTempRadiantVarFlow.*/
    class MODEL_API ZoneHVACLowTempRadiantVarFlow_Impl : public ZoneHVACComponent_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      ZoneHVACLowTempRadiantVarFlow_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      ZoneHVACLowTempRadiantVarFlow_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      ZoneHVACLowTempRadiantVarFlow_Impl(const ZoneHVACLowTempRadiantVarFlow_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~ZoneHVACLowTempRadiantVarFlow_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

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

      boost::optional<HVACComponent> heatingCoil() const;

      boost::optional<HVACComponent> coolingCoil() const;

      boost::optional<std::string> radiantSurfaceType() const;

      std::vector<Surface> surfaces() const;

      std::string fluidtoRadiantSurfaceHeatTransferModel() const;

      bool isFluidtoRadiantSurfaceHeatTransferModelDefaulted() const;

      double hydronicTubingInsideDiameter() const;

      bool isHydronicTubingInsideDiameterDefaulted() const;

      double hydronicTubingOutsideDiameter() const;

      bool isHydronicTubingOutsideDiameterDefaulted() const;

      boost::optional<double> hydronicTubingLength() const;

      bool isHydronicTubingLengthDefaulted() const;

      bool isHydronicTubingLengthAutosized() const;

      double hydronicTubingConductivity() const;

      bool isHydronicTubingConductivityDefaulted() const;

      std::string temperatureControlType() const;

      bool isTemperatureControlTypeDefaulted() const;

      std::string setpointControlType() const;

      bool isSetpointControlTypeDefaulted() const;

      std::string numberofCircuits() const;

      double circuitLength() const;

      boost::optional<Schedule> changeoverDelayTimePeriodSchedule() const;

      boost::optional<double> autosizedHydronicTubingLength() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setHeatingCoil(HVACComponent& heatingCoil);
      void resetHeatingCoil();

      bool setCoolingCoil(HVACComponent& coolingCoil);
      void resetCoolingCoil();

      bool setRadiantSurfaceType(boost::optional<std::string> radiantSurfaceType);

      void resetRadiantSurfaceType();

      bool setFluidtoRadiantSurfaceHeatTransferModel(const std::string& fluidtoRadiantSurfaceHeatTransferModel);

      void resetFluidtoRadiantSurfaceHeatTransferModel();

      bool setHydronicTubingInsideDiameter(double hydronicTubingInsideDiameter);

      void resetHydronicTubingInsideDiameter();

      bool setHydronicTubingOutsideDiameter(double hydronicTubingOutsideDiameter);

      void resetHydronicTubingOutsideDiameter();

      bool setHydronicTubingLength(boost::optional<double> hydronicTubingLength);

      void resetHydronicTubingLength();

      void autosizeHydronicTubingLength();

      bool setHydronicTubingConductivity(double hydronicTubingConductivity);

      void resetHydronicTubingConductivity();

      bool setTemperatureControlType(const std::string& temperatureControlType);

      void resetTemperatureControlType();

      bool setSetpointControlType(const std::string& setpointControlType);

      void resetSetpointControlType();

      bool setNumberofCircuits(const std::string& numberofCircuits);

      bool setCircuitLength(double circuitLength);

      bool setChangeoverDelayTimePeriodSchedule(Schedule& schedule);

      void resetChangeoverDelayTimePeriodSchedule();

      boost::optional<ThermalZone> thermalZone() const override;

      bool addToThermalZone(ThermalZone& thermalZone) override;

      void removeFromThermalZone() override;

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.ZoneHVACLowTempRadiantVarFlow");

      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<HVACComponent> optionalHeatingCoil() const;
      boost::optional<HVACComponent> optionalCoolingCoil() const;

      std::vector<std::string> radiantSurfaceTypeValues() const;
      std::vector<std::string> temperatureControlTypeValues() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ZONEHVACLOWTEMPRADIANTVARFLOW_IMPL_HPP
