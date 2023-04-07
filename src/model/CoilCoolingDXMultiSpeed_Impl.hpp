/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_COILCOOLINGDXMULTISPEED_IMPL_HPP
#define MODEL_COILCOOLINGDXMULTISPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class CoilCoolingDXMultiSpeedStageData;

  namespace detail {

    /** CoilCoolingDXMultiSpeed_Impl is a StraightComponent_Impl that is the implementation class for CoilCoolingDXMultiSpeed.*/
    class MODEL_API CoilCoolingDXMultiSpeed_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilCoolingDXMultiSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilCoolingDXMultiSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilCoolingDXMultiSpeed_Impl(const CoilCoolingDXMultiSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilCoolingDXMultiSpeed_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ModelObject> children() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      boost::optional<Schedule> availabilitySchedule() const;

      std::string condenserType() const;

      bool applyPartLoadFractiontoSpeedsGreaterthan1() const;

      bool applyLatentDegradationtoSpeedsGreaterthan1() const;

      bool isApplyLatentDegradationtoSpeedsGreaterthan1Defaulted() const;

      double crankcaseHeaterCapacity() const;

      double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation() const;

      double basinHeaterCapacity() const;

      double basinHeaterSetpointTemperature() const;

      boost::optional<Schedule> basinHeaterOperatingSchedule() const;

      std::string fuelType() const;

      double minimumOutdoorDryBulbTemperatureforCompressorOperation() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      void resetAvailabilitySchedule();

      bool setCondenserType(const std::string& condenserType);

      bool setApplyPartLoadFractiontoSpeedsGreaterthan1(bool applyPartLoadFractiontoSpeedsGreaterthan1);

      bool setApplyLatentDegradationtoSpeedsGreaterthan1(bool applyLatentDegradationtoSpeedsGreaterthan1);

      void resetApplyLatentDegradationtoSpeedsGreaterthan1();

      bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

      bool setMaximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation(double maximumOutdoorDryBulbTemperatureforCrankcaseHeaterOperation);

      bool setBasinHeaterCapacity(double basinHeaterCapacity);

      bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

      bool setBasinHeaterOperatingSchedule(Schedule& schedule);

      void resetBasinHeaterOperatingSchedule();

      bool setFuelType(const std::string& fuelType);

      bool setMinimumOutdoorDryBulbTemperatureforCompressorOperation(double minimumOutdoorDryBulbTemperatureforCompressorOperation);

      //@}
      /** @name Other */
      //@{

      // Extensible: Stages
      std::vector<CoilCoolingDXMultiSpeedStageData> stages() const;
      unsigned numberOfStages() const;
      boost::optional<unsigned> stageIndex(const CoilCoolingDXMultiSpeedStageData& stage) const;

      // Note: a CoilCoolingDXMultiSpeedStageData can be used only by one CoilCoolingDXMultiSpeed
      bool addStage(const CoilCoolingDXMultiSpeedStageData& stage);
      bool addStage(const CoilCoolingDXMultiSpeedStageData& stage, unsigned index);
      bool setStageIndex(const CoilCoolingDXMultiSpeedStageData& stage, unsigned index);
      bool setStages(const std::vector<CoilCoolingDXMultiSpeedStageData>& stages);
      void removeAllStages();
      bool removeStage(const CoilCoolingDXMultiSpeedStageData& stage);
      bool removeStage(unsigned index);

      AirflowNetworkEquivalentDuct getAirflowNetworkEquivalentDuct(double length, double diameter);

      boost::optional<AirflowNetworkEquivalentDuct> airflowNetworkEquivalentDuct() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilCoolingDXMultiSpeed");

      boost::optional<Connection> optionalAirInletNode() const;
      boost::optional<Connection> optionalAirOutletNode() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCOOLINGDXMULTISPEED_IMPL_HPP
