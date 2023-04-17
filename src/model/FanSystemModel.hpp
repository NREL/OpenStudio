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

#ifndef MODEL_FANSYSTEMMODEL_HPP
#define MODEL_FANSYSTEMMODEL_HPP

#include <model/ModelAPI.hpp>
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

  class Schedule;
  class Curve;
  class ThermalZone;
  class AirflowNetworkFan;

  namespace detail {

    class FanSystemModel_Impl;

  }  // namespace detail

  /** This class implements a speed */
  class MODEL_API FanSystemModelSpeed
  {
   public:
    FanSystemModelSpeed(double flowFraction);

    FanSystemModelSpeed(double flowFraction, double electricPowerFraction);

    double flowFraction() const;
    boost::optional<double> electricPowerFraction() const;

    bool operator<(const FanSystemModelSpeed& other) const;

   private:
    double m_flowFraction;
    boost::optional<double> m_electricPowerFraction;
    REGISTER_LOGGER("openstudio.model.FanSystemModelSpeed");
  };

  // Overload operator<<
  MODEL_API std::ostream& operator<<(std::ostream& out, const openstudio::model::FanSystemModelSpeed& speed);

  /** FanSystemModel is a StraightComponent that wraps the OpenStudio IDD object 'OS:Fan:SystemModel'. */
  class MODEL_API FanSystemModel : public StraightComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit FanSystemModel(const Model& model);

    virtual ~FanSystemModel() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    FanSystemModel(const FanSystemModel& other) = default;
    FanSystemModel(FanSystemModel&& other) = default;
    FanSystemModel& operator=(const FanSystemModel&) = default;
    FanSystemModel& operator=(FanSystemModel&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> speedControlMethodValues();

    static std::vector<std::string> designPowerSizingMethodValues();

    /** @name Getters */
    //@{

    Schedule availabilitySchedule() const;

    boost::optional<double> designMaximumAirFlowRate() const;
    bool isDesignMaximumAirFlowRateAutosized() const;
    boost::optional<double> autosizedDesignMaximumAirFlowRate();

    std::string speedControlMethod() const;

    double electricPowerMinimumFlowRateFraction() const;

    double designPressureRise() const;

    double motorEfficiency() const;

    double motorInAirStreamFraction() const;

    boost::optional<double> designElectricPowerConsumption() const;
    bool isDesignElectricPowerConsumptionAutosized() const;
    boost::optional<double> autosizedDesignElectricPowerConsumption();

    std::string designPowerSizingMethod() const;

    double electricPowerPerUnitFlowRate() const;

    double electricPowerPerUnitFlowRatePerUnitPressure() const;

    double fanTotalEfficiency() const;

    boost::optional<Curve> electricPowerFunctionofFlowFractionCurve() const;

    boost::optional<double> nightVentilationModePressureRise() const;

    boost::optional<double> nightVentilationModeFlowFraction() const;

    boost::optional<ThermalZone> motorLossZone() const;

    double motorLossRadiativeFraction() const;

    std::string endUseSubcategory() const;

    unsigned numberofSpeeds() const;
    std::vector<FanSystemModelSpeed> speeds() const;

    /** Find a given speed by the flowFraction (key) */
    boost::optional<unsigned> speedIndex(const FanSystemModelSpeed& speed) const;

    boost::optional<FanSystemModelSpeed> getSpeed(unsigned speedIndex) const;

    //@}
    /** @name Setters */
    //@{

    /** If a speed group is already present (= the flowFraction already exists) (cf `speedIndex()`), it will Warn and override the electricPowerFraction value */
    bool addSpeed(const FanSystemModelSpeed& speed);

    // This one leaves the electricPowerFraction blank, so the electricPowerFunctionofFlowFractionCurve is used.
    bool addSpeed(double flowFraction);

    // Overloads, it creates a FanSystemModelSpeed wrapper, then call `addSpeed(const FanSystemModelSpeed&)`
    // This one specifies both
    bool addSpeed(double flowFraction, double electricPowerFraction);

    bool removeSpeed(unsigned speedIndex);

    void removeAllSpeeds();

    // Directly set the speeds from a vector, will delete any existing speeds. Will sort out the speeds too, ascending by flowFraction
    bool setSpeeds(const std::vector<FanSystemModelSpeed>& speeds);

    bool setAvailabilitySchedule(Schedule& schedule);

    bool setDesignMaximumAirFlowRate(double designMaximumAirFlowRate);
    void autosizeDesignMaximumAirFlowRate();

    bool setSpeedControlMethod(const std::string& speedControlMethod);

    bool setElectricPowerMinimumFlowRateFraction(double electricPowerMinimumFlowRateFraction);

    bool setDesignPressureRise(double designPressureRise);

    bool setMotorEfficiency(double motorEfficiency);

    bool setMotorInAirStreamFraction(double motorInAirStreamFraction);

    bool setDesignElectricPowerConsumption(double designElectricPowerConsumption);
    void autosizeDesignElectricPowerConsumption();

    bool setDesignPowerSizingMethod(const std::string& designPowerSizingMethod);

    bool setElectricPowerPerUnitFlowRate(double electricPowerPerUnitFlowRate);

    bool setElectricPowerPerUnitFlowRatePerUnitPressure(double electricPowerPerUnitFlowRatePerUnitPressure);

    bool setFanTotalEfficiency(double fanTotalEfficiency);

    bool setElectricPowerFunctionofFlowFractionCurve(const Curve& univariateFunctions);
    void resetElectricPowerFunctionofFlowFractionCurve();

    bool setNightVentilationModePressureRise(double nightVentilationModePressureRise);
    void resetNightVentilationModePressureRise();

    bool setNightVentilationModeFlowFraction(double nightVentilationModeFlowFraction);
    void resetNightVentilationModeFlowFraction();

    bool setMotorLossZone(const ThermalZone& thermalZone);
    void resetMotorLossZone();

    bool setMotorLossRadiativeFraction(double motorLossRadiativeFraction);

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    //@}
    /** @name Other */
    //@{

    AirflowNetworkFan getAirflowNetworkFan();
    boost::optional<AirflowNetworkFan> airflowNetworkFan() const;

    //@}
   protected:
    /// @cond
    using ImplType = detail::FanSystemModel_Impl;

    explicit FanSystemModel(std::shared_ptr<detail::FanSystemModel_Impl> impl);

    friend class detail::FanSystemModel_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.FanSystemModel");
  };

  /** \relates FanSystemModel*/
  using OptionalFanSystemModel = boost::optional<FanSystemModel>;

  /** \relates FanSystemModel*/
  using FanSystemModelVector = std::vector<FanSystemModel>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_FANSYSTEMMODEL_HPP
