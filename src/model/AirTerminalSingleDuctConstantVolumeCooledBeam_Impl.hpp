/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMECOOLEDBEAM_IMPL_HPP
#define MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMECOOLEDBEAM_IMPL_HPP

//#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class StraightComponent;

  namespace detail {

    /** AirTerminalSingleDuctConstantVolumeCooledBeam_Impl is a ZoneHAVC_Impl that is the implementation class for AirTerminalSingleDuctConstantVolumeCooledBeam.*/
    class MODEL_API AirTerminalSingleDuctConstantVolumeCooledBeam_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      AirTerminalSingleDuctConstantVolumeCooledBeam_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctConstantVolumeCooledBeam_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      AirTerminalSingleDuctConstantVolumeCooledBeam_Impl(const AirTerminalSingleDuctConstantVolumeCooledBeam_Impl& other, Model_Impl* model,
                                                         bool keepHandle);

      virtual ~AirTerminalSingleDuctConstantVolumeCooledBeam_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

      virtual bool isRemovable() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual bool addToNode(Node& node) override;

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

      HVACComponent coilCoolingCooledBeam() const;

      std::string cooledBeamType() const;

      boost::optional<double> supplyAirVolumetricFlowRate() const;

      bool isSupplyAirVolumetricFlowRateDefaulted() const;

      bool isSupplyAirVolumetricFlowRateAutosized() const;

      boost::optional<double> maximumTotalChilledWaterVolumetricFlowRate() const;

      bool isMaximumTotalChilledWaterVolumetricFlowRateDefaulted() const;

      bool isMaximumTotalChilledWaterVolumetricFlowRateAutosized() const;

      boost::optional<int> numberofBeams() const;

      bool isNumberofBeamsDefaulted() const;

      bool isNumberofBeamsAutosized() const;

      boost::optional<double> beamLength() const;

      bool isBeamLengthDefaulted() const;

      bool isBeamLengthAutosized() const;

      double designInletWaterTemperature() const;

      bool isDesignInletWaterTemperatureDefaulted() const;

      double designOutletWaterTemperature() const;

      bool isDesignOutletWaterTemperatureDefaulted() const;

      boost::optional<double> coefficientofInductionKin() const;

      bool isCoefficientofInductionKinDefaulted() const;

      bool isCoefficientofInductionKinAutocalculated() const;

      boost::optional<double> autosizedSupplyAirVolumetricFlowRate() const;

      boost::optional<double> autosizedMaximumTotalChilledWaterVolumetricFlowRate() const;

      boost::optional<double> autosizedNumberofBeams() const;

      boost::optional<double> autosizedBeamLength() const;

      boost::optional<double> autosizedCoefficientofInductionKin() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      bool setCoolingCoil(HVACComponent& coilCoolingCooledBeam);

      bool setCooledBeamType(const std::string& cooledBeamType);

      bool setSupplyAirVolumetricFlowRate(boost::optional<double> supplyAirVolumetricFlowRate);

      void resetSupplyAirVolumetricFlowRate();

      void autosizeSupplyAirVolumetricFlowRate();

      bool setMaximumTotalChilledWaterVolumetricFlowRate(boost::optional<double> maximumTotalChilledWaterVolumetricFlowRate);

      void resetMaximumTotalChilledWaterVolumetricFlowRate();

      void autosizeMaximumTotalChilledWaterVolumetricFlowRate();

      bool setNumberofBeams(boost::optional<int> numberofBeams);

      void resetNumberofBeams();

      void autosizeNumberofBeams();

      bool setBeamLength(boost::optional<double> beamLength);

      void resetBeamLength();

      void autosizeBeamLength();

      bool setDesignInletWaterTemperature(double designInletWaterTemperature);

      void resetDesignInletWaterTemperature();

      bool setDesignOutletWaterTemperature(double designOutletWaterTemperature);

      void resetDesignOutletWaterTemperature();

      bool setCoefficientofInductionKin(boost::optional<double> coefficientofInductionKin);

      void resetCoefficientofInductionKin();

      void autocalculateCoefficientofInductionKin();

      // boost::optional<ThermalZone> thermalZone();

      // bool addToThermalZone(ThermalZone & thermalZone);

      // void removeFromThermalZone();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctConstantVolumeCooledBeam");

      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<HVACComponent> optionalCoolingCoil() const;

      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;
      boost::optional<ModelObject> coolingCoilAsModelObject() const;

      bool setAvailabilityScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setCoolingCoilAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMECOOLEDBEAM_IMPL_HPP
