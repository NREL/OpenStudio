/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_PLANTCOMPONENTUSERDEFINED_IMPL_HPP
#define MODEL_PLANTCOMPONENTUSERDEFINED_IMPL_HPP

#include "ModelAPI.hpp"
#include "ModelObject_Impl.hpp"
#include "EnergyManagementSystemProgramCallingManager_Impl.hpp"
#include "EnergyManagementSystemProgram_Impl.hpp"
#include "EnergyManagementSystemActuator_Impl.hpp"
#include "StraightComponent_Impl.hpp"
#include "ThermalZone_Impl.hpp"

namespace openstudio {

namespace model {

  // TODO: Check the following class names against object getters and setters.
  class EnergyManagementSystemProgramCallingManager;
  class EnergyManagementSystemProgram;
  class EnergyManagementSystemActuator;
  class Connection;
  class Node;
  class ThermalZone;

  namespace detail {

    /** PlantComponentUserDefined_Impl is a StraightComponent_Impl that is the implementation class for PlantComponentUserDefined.*/
    class MODEL_API PlantComponentUserDefined_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      PlantComponentUserDefined_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      PlantComponentUserDefined_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      PlantComponentUserDefined_Impl(const PlantComponentUserDefined_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~PlantComponentUserDefined_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      std::string plantLoadingMode() const;

      std::string plantLoopFlowRequestMode() const;

      boost::optional<EnergyManagementSystemProgramCallingManager> mainModelProgramCallingManager() const;

      boost::optional<EnergyManagementSystemProgramCallingManager> plantInitializationProgramCallingManager() const;

      boost::optional<EnergyManagementSystemProgramCallingManager> plantSimulationProgramCallingManager() const;

      boost::optional<EnergyManagementSystemProgram> mainModelProgram() const;

      boost::optional<EnergyManagementSystemProgram> plantInitializationProgram() const;

      boost::optional<EnergyManagementSystemProgram> plantSimulationProgram() const;

      boost::optional<ThermalZone> ambientZone() const;

      boost::optional<EnergyManagementSystemActuator> designVolumeFlowRateActuator() const;

      boost::optional<EnergyManagementSystemActuator> minimumMassFlowRateActuator() const;

      boost::optional<EnergyManagementSystemActuator> maximumMassFlowRateActuator() const;

      boost::optional<EnergyManagementSystemActuator> minimumLoadingCapacityActuator() const;

      boost::optional<EnergyManagementSystemActuator> maximumLoadingCapacityActuator() const;

      boost::optional<EnergyManagementSystemActuator> optimalLoadingCapacityActuator() const;

      boost::optional<EnergyManagementSystemActuator> outletTemperatureActuator() const;

      boost::optional<EnergyManagementSystemActuator> massFlowRateActuator() const;

      //@}
      /** @name Setters */
      //@{

      bool setPlantLoadingMode(const std::string& plantLoadingMode);

      bool setPlantLoopFlowRequestMode(const std::string& plantLoopFlowRequestMode);

      bool setMainModelProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager);

      void resetMainModelProgramCallingManager();

      bool
        setPlantInitializationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager);

      void resetPlantInitializationProgramCallingManager();

      bool setPlantSimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& energyManagementSystemProgramCallingManager);

      void resetPlantSimulationProgramCallingManager();

      bool setMainModelProgram(const EnergyManagementSystemProgram& energyManagementSystemProgram);

      void resetMainModelProgram();

      bool setPlantInitializationProgram(const EnergyManagementSystemProgram& energyManagementSystemProgram);

      void resetPlantInitializationProgram();

      bool setPlantSimulationProgram(const EnergyManagementSystemProgram& energyManagementSystemProgram);

      void resetPlantSimulationProgram();

      bool setAmbientZone(const ThermalZone& thermalZone);

      void resetAmbientZone();

      bool setDesignVolumeFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

      bool setMinimumMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

      bool setMaximumMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

      bool setMinimumLoadingCapacityActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

      bool setMaximumLoadingCapacityActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

      bool setOptimalLoadingCapacityActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

      bool setOutletTemperatureActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

      bool setMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

      EnergyManagementSystemProgram
        createInitProgram(const EnergyManagementSystemActuator& dvfrActuator, const EnergyManagementSystemActuator& mmfrActuator,
                          const EnergyManagementSystemActuator& mxfrActuator, const EnergyManagementSystemActuator& mlcActuator,
                          const EnergyManagementSystemActuator& mxlcActuator, const EnergyManagementSystemActuator& olcActuator, const Model& model);

      EnergyManagementSystemProgram createSimProgram(const EnergyManagementSystemActuator& otActuator,
                                                     const EnergyManagementSystemActuator& mfrActuator, const Model& model);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.PlantComponentUserDefined");

      boost::optional<Connection> optionalInletNode() const;
      boost::optional<Connection> optionalOutletNode() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_PLANTCOMPONENTUSERDEFINED_IMPL_HPP
