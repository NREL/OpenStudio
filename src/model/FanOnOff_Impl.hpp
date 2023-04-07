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

#ifndef MODEL_FANONOFF_IMPL_HPP
#define MODEL_FANONOFF_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Schedule;
  class Curve;
  class AirflowNetworkFan;

  namespace detail {

    /** FanOnOff_Impl is a StraightComponent_Impl that is the implementation class for FanOnOff.*/
    class MODEL_API FanOnOff_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      FanOnOff_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      FanOnOff_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      FanOnOff_Impl(const FanOnOff_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~FanOnOff_Impl() = default;

      //@}

      /** @name Virtual Methods */
      //@{

      virtual std::vector<openstudio::IdfObject> remove() override;

      virtual ModelObject clone(Model model) const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual bool addToNode(Node& node) override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters and Setters */
      //@{

      // Field Availability Schedule Name

      Schedule availabilitySchedule() const;

      bool setAvailabilitySchedule(Schedule& schedule);

      // Field Fan Efficiency

      double fanTotalEfficiency() const;

      bool isFanTotalEfficiencyDefaulted() const;

      bool setFanTotalEfficiency(double fanTotalEfficiency);

      void resetFanTotalEfficiency();

      // Field Pressure Rise

      double pressureRise() const;

      bool setPressureRise(double pressureRise);

      // Field Maximum Flow Rate

      boost::optional<double> maximumFlowRate() const;

      bool isMaximumFlowRateAutosized() const;

      bool setMaximumFlowRate(boost::optional<double> maximumFlowRate);

      void resetMaximumFlowRate();

      void autosizeMaximumFlowRate();

      // Field Motor Efficiency

      double motorEfficiency() const;

      bool isMotorEfficiencyDefaulted() const;

      bool setMotorEfficiency(double motorEfficiency);

      void resetMotorEfficiency();

      // Field Motor In Airstream Fraction

      boost::optional<double> motorInAirstreamFraction() const;

      bool setMotorInAirstreamFraction(boost::optional<double> motorInAirstreamFraction);

      void resetMotorInAirstreamFraction();

      // Field End-Use Subcategory

      std::string endUseSubcategory() const;

      bool isEndUseSubcategoryDefaulted() const;

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      void resetEndUseSubcategory();

      //@}
      /** @name Curves */
      //@{

      Curve fanPowerRatioFunctionofSpeedRatioCurve() const;
      bool setFanPowerRatioFunctionofSpeedRatioCurve(const Curve& curve);

      Curve fanEfficiencyRatioFunctionofSpeedRatioCurve() const;
      bool setFanEfficiencyRatioFunctionofSpeedRatioCurve(const Curve& curve);

      boost::optional<double> autosizedMaximumFlowRate() const;

      virtual void autosize() override;

      virtual void applySizingValues() override;

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}

      AirflowNetworkFan getAirflowNetworkFan();
      boost::optional<AirflowNetworkFan> airflowNetworkFan() const;

      //protected:
     private:
      REGISTER_LOGGER("openstudio.model.FanOnOff");

      boost::optional<Schedule> optionalAvailabilitySchedule() const;
      boost::optional<ModelObject> availabilityScheduleAsModelObject() const;

      virtual boost::optional<ZoneHVACComponent> containingZoneHVACComponent() const override;

      virtual boost::optional<HVACComponent> containingHVACComponent() const override;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FANONOFF_IMPL_HPP
