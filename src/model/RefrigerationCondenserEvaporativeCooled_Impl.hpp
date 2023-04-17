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

#ifndef MODEL_REFRIGERATIONCONDENSEREVAPORATIVECOOLED_IMPL_HPP
#define MODEL_REFRIGERATIONCONDENSEREVAPORATIVECOOLED_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  // WaterStorageTank object not used
  //class WaterStorageTank;
  class RefrigerationSystem;
  class Schedule;

  namespace detail {

    /** RefrigerationCondenserEvaporativeCooled_Impl is a ParentObject_Impl that is the implementation class for RefrigerationCondenserEvaporativeCooled.*/
    class MODEL_API RefrigerationCondenserEvaporativeCooled_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      RefrigerationCondenserEvaporativeCooled_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RefrigerationCondenserEvaporativeCooled_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RefrigerationCondenserEvaporativeCooled_Impl(const RefrigerationCondenserEvaporativeCooled_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RefrigerationCondenserEvaporativeCooled_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      //@}
      /** @name Getters */
      //@{

      double ratedEffectiveTotalHeatRejectionRate() const;

      double ratedSubcoolingTemperatureDifference() const;

      bool isRatedSubcoolingTemperatureDifferenceDefaulted() const;

      std::string fanSpeedControlType() const;

      bool isFanSpeedControlTypeDefaulted() const;

      double ratedFanPower() const;

      double minimumFanAirFlowRatio() const;

      bool isMinimumFanAirFlowRatioDefaulted() const;

      double approachTemperatureConstantTerm() const;

      bool isApproachTemperatureConstantTermDefaulted() const;

      double approachTemperatureCoefficient2() const;

      bool isApproachTemperatureCoefficient2Defaulted() const;

      double approachTemperatureCoefficient3() const;

      bool isApproachTemperatureCoefficient3Defaulted() const;

      double approachTemperatureCoefficient4() const;

      bool isApproachTemperatureCoefficient4Defaulted() const;

      double minimumCapacityFactor() const;

      bool isMinimumCapacityFactorDefaulted() const;

      double maximumCapacityFactor() const;

      bool isMaximumCapacityFactorDefaulted() const;

      //std::string airInletNode() const;

      boost::optional<double> ratedAirFlowRate() const;

      bool isRatedAirFlowRateDefaulted() const;

      bool isRatedAirFlowRateAutocalculated() const;

      double basinHeaterCapacity() const;

      bool isBasinHeaterCapacityDefaulted() const;

      double basinHeaterSetpointTemperature() const;

      bool isBasinHeaterSetpointTemperatureDefaulted() const;

      boost::optional<double> ratedWaterPumpPower() const;

      bool isRatedWaterPumpPowerDefaulted() const;

      bool isRatedWaterPumpPowerAutocalculated() const;

      //boost::optional<WaterStorageTank> evaporativeWaterSupplyTank() const;

      boost::optional<Schedule> evaporativeCondenserAvailabilitySchedule() const;

      std::string endUseSubcategory() const;

      bool isEndUseSubcategoryDefaulted() const;

      double condenserRefrigerantOperatingChargeInventory() const;

      bool isCondenserRefrigerantOperatingChargeInventoryDefaulted() const;

      double condensateReceiverRefrigerantInventory() const;

      bool isCondensateReceiverRefrigerantInventoryDefaulted() const;

      double condensatePipingRefrigerantInventory() const;

      bool isCondensatePipingRefrigerantInventoryDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setRatedEffectiveTotalHeatRejectionRate(double ratedEffectiveTotalHeatRejectionRate);

      bool setRatedSubcoolingTemperatureDifference(double ratedSubcoolingTemperatureDifference);

      void resetRatedSubcoolingTemperatureDifference();

      bool setFanSpeedControlType(const std::string& fanSpeedControlType);

      void resetFanSpeedControlType();

      bool setRatedFanPower(double ratedFanPower);

      bool setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio);

      void resetMinimumFanAirFlowRatio();

      bool setApproachTemperatureConstantTerm(double approachTemperatureConstantTerm);

      void resetApproachTemperatureConstantTerm();

      bool setApproachTemperatureCoefficient2(double approachTemperatureCoefficient2);

      void resetApproachTemperatureCoefficient2();

      bool setApproachTemperatureCoefficient3(double approachTemperatureCoefficient3);

      void resetApproachTemperatureCoefficient3();

      bool setApproachTemperatureCoefficient4(double approachTemperatureCoefficient4);

      void resetApproachTemperatureCoefficient4();

      bool setMinimumCapacityFactor(double minimumCapacityFactor);

      void resetMinimumCapacityFactor();

      bool setMaximumCapacityFactor(double maximumCapacityFactor);

      void resetMaximumCapacityFactor();

      //bool setAirInletNode(const std::string& airInletNode);

      //void resetAirInletNode();

      bool setRatedAirFlowRate(boost::optional<double> ratedAirFlowRate);

      void resetRatedAirFlowRate();

      void autocalculateRatedAirFlowRate();

      bool setBasinHeaterCapacity(double basinHeaterCapacity);

      void resetBasinHeaterCapacity();

      bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

      void resetBasinHeaterSetpointTemperature();

      bool setRatedWaterPumpPower(boost::optional<double> ratedWaterPumpPower);

      void resetRatedWaterPumpPower();

      void autocalculateRatedWaterPumpPower();

      //bool setEvaporativeWaterSupplyTank(const boost::optional<WaterStorageTank>& waterStorageTank);

      //void resetEvaporativeWaterSupplyTank();

      bool setEvaporativeCondenserAvailabilitySchedule(Schedule& schedule);

      void resetEvaporativeCondenserAvailabilitySchedule();

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      void resetEndUseSubcategory();

      bool setCondenserRefrigerantOperatingChargeInventory(double condenserRefrigerantOperatingChargeInventory);

      void resetCondenserRefrigerantOperatingChargeInventory();

      bool setCondensateReceiverRefrigerantInventory(double condensateReceiverRefrigerantInventory);

      void resetCondensateReceiverRefrigerantInventory();

      bool setCondensatePipingRefrigerantInventory(double condensatePipingRefrigerantInventory);

      void resetCondensatePipingRefrigerantInventory();

      //@}
      /** @name Other */
      //@{

      boost::optional<RefrigerationSystem> system() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.RefrigerationCondenserEvaporativeCooled");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONCONDENSEREVAPORATIVECOOLED_IMPL_HPP
