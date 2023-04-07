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

#ifndef MODEL_COILHEATINGELECTRICMULTISTAGE_IMPL_HPP
#define MODEL_COILHEATINGELECTRICMULTISTAGE_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class CoilHeatingElectricMultiStageStageData;

  namespace detail {

    /** CoilHeatingElectricMultiStage_Impl is a StraightComponent_Impl that is the implementation class for CoilHeatingElectricMultiStage.*/
    class MODEL_API CoilHeatingElectricMultiStage_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilHeatingElectricMultiStage_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilHeatingElectricMultiStage_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilHeatingElectricMultiStage_Impl(const CoilHeatingElectricMultiStage_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilHeatingElectricMultiStage_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      ModelObject clone(Model model) const override;

      std::vector<ModelObject> children() const override;

      boost::optional<HVACComponent> containingHVACComponent() const override;

      bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      Schedule availabilitySchedule() const;

      //@}
      /** @name Setters */
      //@{

      bool setAvailabilitySchedule(Schedule& schedule);

      //@}
      /** @name Other */
      //@{

      std::vector<CoilHeatingElectricMultiStageStageData> stages() const;
      unsigned numberOfStages() const;
      boost::optional<unsigned> stageIndex(const CoilHeatingElectricMultiStageStageData& stage) const;

      bool addStage(const CoilHeatingElectricMultiStageStageData& stage);
      bool addStage(const CoilHeatingElectricMultiStageStageData& stage, unsigned index);
      bool setStageIndex(const CoilHeatingElectricMultiStageStageData& stage, unsigned index);
      bool setStages(const std::vector<CoilHeatingElectricMultiStageStageData>& stages);
      void removeAllStages();
      bool removeStage(const CoilHeatingElectricMultiStageStageData& stage);
      bool removeStage(unsigned index);

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilHeatingElectricMultiStage");

      boost::optional<Schedule> optionalAvailabilitySchedule() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILHEATINGELECTRICMULTISTAGE_IMPL_HPP
