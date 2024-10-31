/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_COILUSERDEFINED_IMPL_HPP
#define MODEL_COILUSERDEFINED_IMPL_HPP

#include <model/ModelAPI.hpp>
#include "WaterToAirComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class EnergyManagementSystemActuator;
  class EnergyManagementSystemProgram;
  class EnergyManagementSystemProgramCallingManager;
  class ThermalZone;

  namespace detail {

    /** CoilUserDefined_Impl is a WaterToAirComponent_Impl that is the implementation class for CoilUserDefined.*/
    class MODEL_API CoilUserDefined_Impl : public WaterToAirComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilUserDefined_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilUserDefined_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilUserDefined_Impl(const CoilUserDefined_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilUserDefined_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned airInletPort() const override;

      virtual unsigned airOutletPort() const override;

      virtual unsigned waterInletPort() const override;

      virtual unsigned waterOutletPort() const override;

      // virtual std::vector<openstudio::IdfObject> remove() override;

      // HVACComponent skips the ParentObject and goes straight to ModelObject so children aren't clone
      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      EnergyManagementSystemProgramCallingManager overallModelSimulationProgramCallingManager() const;

      EnergyManagementSystemProgramCallingManager modelSetupandSizingProgramCallingManager() const;

      boost::optional<ThermalZone> ambientZone() const;

      EnergyManagementSystemProgram overallSimulationProgram() const;

      EnergyManagementSystemProgram initializationSimulationProgram() const;

      EnergyManagementSystemActuator airOutletTemperatureActuator() const;

      EnergyManagementSystemActuator airOutletHumidityRatioActuator() const;

      EnergyManagementSystemActuator airMassFlowRateActuator() const;

      EnergyManagementSystemActuator plantMinimumMassFlowRateActuator() const;

      EnergyManagementSystemActuator plantMaximumMassFlowRateActuator() const;

      EnergyManagementSystemActuator plantDesignVolumeFlowRateActuator() const;

      EnergyManagementSystemActuator plantOutletTemperatureActuator() const;

      EnergyManagementSystemActuator plantMassFlowRateActuator() const;

      //@}
      /** @name Setters */
      //@{

      bool setOverallModelSimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& emsProgramCallingManager);

      bool setModelSetupandSizingProgramCallingManager(const EnergyManagementSystemProgramCallingManager& emsProgramCallingManager);

      bool setAmbientZone(const ThermalZone& thermalZone);

      void resetAmbientZone();

      bool setOverallSimulationProgram(const EnergyManagementSystemProgram& emsProgram);

      bool setInitializationSimulationProgram(const EnergyManagementSystemProgram& emsProgram);

      bool setAirOutletTemperatureActuator(const EnergyManagementSystemActuator& emsActuator);

      bool setAirOutletHumidityRatioActuator(const EnergyManagementSystemActuator& emsActuator);

      bool setAirMassFlowRateActuator(const EnergyManagementSystemActuator& emsActuator);

      bool setPlantMinimumMassFlowRateActuator(const EnergyManagementSystemActuator& emsActuator);

      bool setPlantMaximumMassFlowRateActuator(const EnergyManagementSystemActuator& emsActuator);

      bool setPlantDesignVolumeFlowRateActuator(const EnergyManagementSystemActuator& emsActuator);

      bool setPlantOutletTemperatureActuator(const EnergyManagementSystemActuator& emsActuator);

      bool setPlantMassFlowRateActuator(const EnergyManagementSystemActuator& emsActuator);

      //@}
      /** @name Other */
      //@{

      int numberofAirConnections() const;

      void renameEMSSubComponents();

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilUserDefined");

      // Optional getters for use by methods like children() so can remove() if the constructor fails.
      // There are other ways for the public versions of these getters to fail--perhaps all required
      // objects should be returned as boost::optionals
      boost::optional<EnergyManagementSystemProgramCallingManager> optionalOverallModelSimulationProgramCallingManager() const;
      boost::optional<EnergyManagementSystemProgramCallingManager> optionalModelSetupandSizingProgramCallingManager() const;
      boost::optional<EnergyManagementSystemProgram> optionalOverallSimulationProgram() const;
      boost::optional<EnergyManagementSystemProgram> optionalInitializationSimulationProgram() const;
      boost::optional<EnergyManagementSystemActuator> optionalAirOutletTemperatureActuator() const;
      boost::optional<EnergyManagementSystemActuator> optionalAirOutletHumidityRatioActuator() const;
      boost::optional<EnergyManagementSystemActuator> optionalAirMassFlowRateActuator() const;
      boost::optional<EnergyManagementSystemActuator> optionalPlantMinimumMassFlowRateActuator() const;
      boost::optional<EnergyManagementSystemActuator> optionalPlantMaximumMassFlowRateActuator() const;
      boost::optional<EnergyManagementSystemActuator> optionalPlantDesignVolumeFlowRateActuator() const;
      boost::optional<EnergyManagementSystemActuator> optionalPlantOutletTemperatureActuator() const;
      boost::optional<EnergyManagementSystemActuator> optionalPlantMassFlowRateActuator() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILUSERDEFINED_IMPL_HPP
