/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2022, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef MODEL_COILUSERDEFINED_IMPL_HPP
#define MODEL_COILUSERDEFINED_IMPL_HPP

#include "EnergyManagementSystemProgramCallingManager_Impl.hpp"
#include "EnergyManagementSystemProgram_Impl.hpp"
#include "EnergyManagementSystemActuator_Impl.hpp"
#include "WaterToAirComponent_Impl.hpp"
#include "ThermalZone_Impl.hpp"

namespace openstudio {
namespace model {

  class EnergyManagementSystemProgramCallingManager;
  class EnergyManagementSystemProgram;
  class EnergyManagementSystemActuator;
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

      virtual ~CoilUserDefined_Impl() {}

      //@}
      /** @name Virtual Methods */
      //@{

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual unsigned airInletPort() const override;

      virtual unsigned airOutletPort() const override;

      virtual unsigned waterInletPort() const override;

      virtual unsigned waterOutletPort() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<EnergyManagementSystemProgramCallingManager> overallModelSimulationProgramCallingManager() const;

      boost::optional<EnergyManagementSystemProgramCallingManager> modelSetupandSizingProgramCallingManager() const;

      boost::optional<EnergyManagementSystemProgram> overallSimulationProgram() const;

      boost::optional<EnergyManagementSystemProgram> initializationSimulationProgram() const;

      boost::optional<EnergyManagementSystemActuator> airOutletTemperatureActuator() const;

      boost::optional<EnergyManagementSystemActuator> airOutletHumidityRatioActuator() const;

      boost::optional<EnergyManagementSystemActuator> airMassFlowRateActuator() const;

      boost::optional<EnergyManagementSystemActuator> plantMinimumMassFlowRateActuator() const;

      boost::optional<EnergyManagementSystemActuator> plantMaximumMassFlowRateActuator() const;

      boost::optional<EnergyManagementSystemActuator> plantDesignVolumeFlowRateActuator() const;

      boost::optional<EnergyManagementSystemActuator> plantMassFlowRateActuator() const;

      boost::optional<EnergyManagementSystemActuator> plantOutletTemperatureActuator() const;

      boost::optional<ThermalZone> ambientZone() const;

      //@}
      /** @name Setters */
      //@{

      bool setOverallModelSimulationProgramCallingManager(const EnergyManagementSystemProgramCallingManager& erlProgram);

      void resetOverallModelSimulationProgramCallingManager();

      bool setModelSetupandSizingProgramCallingManager(const EnergyManagementSystemProgramCallingManager& erlProgram);

      void resetModelSetupandSizingProgramCallingManager();

      bool setOverallSimulationProgram(const EnergyManagementSystemProgram& energyManagementSystemProgram);

      void resetOverallSimulationProgram();

      bool setInitializationSimulationProgram(const EnergyManagementSystemProgram& energyManagementSystemProgram);

      void resetInitializationSimulationProgram();

      bool setAmbientZone(const ThermalZone& thermalZone);

      void resetAmbientZone();

      bool setAirOutletTemperatureActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

      bool setAirOutletHumidityRatioActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

      bool setAirMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

      bool setPlantMinimumMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

      bool setPlantMaximumMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

      bool setPlantDesignVolumeFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

      bool setPlantMassFlowRateActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

      bool setPlantOutletTemperatureActuator(const EnergyManagementSystemActuator& energyManagementSystemActuator);

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilUserDefined");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILUSERDEFINED_IMPL_HPP
