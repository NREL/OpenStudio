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

#ifndef MODEL_WATERUSECONNECTIONS_IMPL_HPP
#define MODEL_WATERUSECONNECTIONS_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class WaterUseEquipment;

  namespace detail {

    /** WaterUseConnections_Impl is a StraightComponent_Impl that is the implementation class for WaterUseConnections.*/
    class MODEL_API WaterUseConnections_Impl : public StraightComponent_Impl
    {

     public:
      WaterUseConnections_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      WaterUseConnections_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      WaterUseConnections_Impl(const WaterUseConnections_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~WaterUseConnections_Impl() = default;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      boost::optional<Schedule> hotWaterSupplyTemperatureSchedule() const;

      boost::optional<Schedule> coldWaterSupplyTemperatureSchedule() const;

      bool setHotWaterSupplyTemperatureSchedule(Schedule& hotWaterSupplyTemperatureSchedule);

      void resetHotWaterSupplyTemperatureSchedule();

      bool setColdWaterSupplyTemperatureSchedule(Schedule& coldWaterSupplyTemperatureSchedule);

      void resetColdWaterSupplyTemperatureSchedule();

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      std::vector<WaterUseEquipment> waterUseEquipment() const;

      bool addWaterUseEquipment(const WaterUseEquipment& waterUseEquipment);

      bool removeWaterUseEquipment(WaterUseEquipment& waterUseEquipment);

      bool addToNode(Node& node) override;

      std::string drainWaterHeatExchangerType() const;

      bool setDrainWaterHeatExchangerType(const std::string& drainWaterHeatExchangerType);

      std::string drainWaterHeatExchangerDestination() const;

      bool setDrainWaterHeatExchangerDestination(const std::string& drainWaterHeatExchangerDestination);

      boost::optional<double> drainWaterHeatExchangerUFactorTimesArea() const;

      bool setDrainWaterHeatExchangerUFactorTimesArea(double drainWaterHeatExchangerUFactorTimesArea);

      void resetDrainWaterHeatExchangerUFactorTimesArea();

     protected:
     private:
      REGISTER_LOGGER("openstudio.model.WaterUseConnections");

      boost::optional<ModelObject> hotWaterSupplyTemperatureScheduleAsModelObject() const;
      boost::optional<ModelObject> coldWaterSupplyTemperatureScheduleAsModelObject() const;

      bool setHotWaterSupplyTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setColdWaterSupplyTemperatureScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_WATERUSECONNECTIONS_IMPL_HPP
