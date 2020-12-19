/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2020, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_COILCHILLERAIRSOURCEVARIABLESPEED_IMPL_HPP
#define MODEL_COILCHILLERAIRSOURCEVARIABLESPEED_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;
  class Schedule;
  class CoilChillerAirSourceVariableSpeedSpeedData;
  class ModelObjectList;

  namespace detail {

    /** CoilChillerAirSourceVariableSpeed_Impl is a StraightComponent_Impl that is the implementation class for CoilChillerAirSourceVariableSpeed.*/
    class MODEL_API CoilChillerAirSourceVariableSpeed_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      CoilChillerAirSourceVariableSpeed_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      CoilChillerAirSourceVariableSpeed_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      CoilChillerAirSourceVariableSpeed_Impl(const CoilChillerAirSourceVariableSpeed_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~CoilChillerAirSourceVariableSpeed_Impl() {}

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual bool addToNode(Node& node) override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

      virtual std::vector<ModelObject> children() const override;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      //@}
      /** @name Getters */
      //@{

      int nominalSpeedLevel() const;

      boost::optional<double> ratedChilledWaterCapacity() const;

      bool isRatedChilledWaterCapacityAutosized() const;

      double ratedEvaporatorInletWaterTemperature() const;

      double ratedCondenserInletAirTemperature() const;

      boost::optional<double> ratedEvaporatorWaterFlowRate() const;

      bool isRatedEvaporatorWaterFlowRateAutocalculated() const;

      std::string evaporatorPumpPowerIncludedinRatedCOP() const;

      std::string evaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP() const;

      double fractionofEvaporatorPumpHeattoWater() const;

      double crankcaseHeaterCapacity() const;

      double maximumAmbientTemperatureforCrankcaseHeaterOperation() const;

      boost::optional<Curve> partLoadFractionCorrelationCurve() const;

      boost::optional<Schedule> gridSignalSchedule() const;

      double lowerBoundToApplyGridResponsiveControl() const;

      bool isLowerBoundToApplyGridResponsiveControlDefaulted() const;

      double upperBoundToApplyGridResponsiveControl() const;

      bool isUpperBoundToApplyGridResponsiveControlDefaulted() const;

      int maxSpeedLevelDuringGridResponsiveControl() const;

      bool isMaxSpeedLevelDuringGridResponsiveControlDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setNominalSpeedLevel(int nominalSpeedLevel);

      bool setRatedChilledWaterCapacity(boost::optional<double> ratedChilledWaterCapacity);

      void autosizeRatedChilledWaterCapacity();

      bool setRatedEvaporatorInletWaterTemperature(double ratedEvaporatorInletWaterTemperature);

      bool setRatedCondenserInletAirTemperature(double ratedCondenserInletAirTemperature);

      bool setRatedEvaporatorWaterFlowRate(boost::optional<double> ratedEvaporatorWaterFlowRate);

      void autocalculateRatedEvaporatorWaterFlowRate();

      bool setEvaporatorPumpPowerIncludedinRatedCOP(std::string evaporatorPumpPowerIncludedinRatedCOP);

      bool setEvaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP(std::string evaporatorPumpHeatIncludedinRatedCoolingCapacityandRatedCOP);

      bool setFractionofEvaporatorPumpHeattoWater(double fractionofEvaporatorPumpHeattoWater);

      bool setCrankcaseHeaterCapacity(double crankcaseHeaterCapacity);

      bool setMaximumAmbientTemperatureforCrankcaseHeaterOperation(double maximumAmbientTemperatureforCrankcaseHeaterOperation);

      bool setPartLoadFractionCorrelationCurve(const boost::optional<Curve>& partLoadFractionCorrelationCurve);

      void resetPartLoadFractionCorrelationCurve();

      bool setGridSignalSchedule(Schedule& schedule);

      void resetGridSignalSchedule();

      bool setLowerBoundToApplyGridResponsiveControl(double lowerBoundToApplyGridResponsiveControl);

      void resetLowerBoundToApplyGridResponsiveControl();

      bool setUpperBoundToApplyGridResponsiveControl(double upperBoundToApplyGridResponsiveControl);

      void resetUpperBoundToApplyGridResponsiveControl();

      bool setMaxSpeedLevelDuringGridResponsiveControl(int maxSpeedlevelDuringGridResponsiveControl);

      void resetMaxSpeedLevelDuringGridResponsiveControl();

      //@}
      /** @name Other */
      //@{

      bool setSpeedDataList(const boost::optional<ModelObjectList>& modelObjectList);

      void resetSpeedDataList();

      boost::optional<ModelObjectList> speedDataList() const;

      std::vector<CoilChillerAirSourceVariableSpeedSpeedData> speeds() const;

      bool addSpeed(const CoilChillerAirSourceVariableSpeedSpeedData& speed);

      void removeSpeed(const CoilChillerAirSourceVariableSpeedSpeedData& speed);

      void removeAllSpeeds();

      // Autosize methods
      boost::optional<double> autosizedRatedChilledWaterCapacity() const;

      boost::optional<double> autocalculatedRatedEvaporatorWaterFlowRate() const;

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.CoilChillerAirSourceVariableSpeed");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_COILCHILLERAIRSOURCEVARIABLESPEED_IMPL_HPP
