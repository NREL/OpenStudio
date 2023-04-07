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

#ifndef MODEL_REFRIGERATIONAIRCHILLER_HPP
#define MODEL_REFRIGERATIONAIRCHILLER_HPP

#include "ModelAPI.hpp"
#include "ZoneHVACComponent.hpp"

namespace openstudio {

namespace model {

  class RefrigerationSystem;
  class RefrigerationSecondarySystem;
  class RefrigerationCompressorRack;
  class Schedule;
  // class CurveLinear;

  namespace detail {

    class RefrigerationAirChiller_Impl;

  }  // namespace detail

  /** RefrigerationAirChiller is a ZoneHVACComponent that wraps the OpenStudio IDD object 'OS:ZoneHVAC:Refrigeration:AirChiller'. */
  class MODEL_API RefrigerationAirChiller : public ZoneHVACComponent
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit RefrigerationAirChiller(const Model& model, Schedule& defrostSchedule);

    virtual ~RefrigerationAirChiller() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RefrigerationAirChiller(const RefrigerationAirChiller& other) = default;
    RefrigerationAirChiller(RefrigerationAirChiller&& other) = default;
    RefrigerationAirChiller& operator=(const RefrigerationAirChiller&) = default;
    RefrigerationAirChiller& operator=(RefrigerationAirChiller&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> capacityRatingTypeValues();

    // static std::vector<std::string> capacityCorrectionCurveTypeValues();

    static std::vector<std::string> fanSpeedControlTypeValues();

    static std::vector<std::string> defrostTypeValues();

    static std::vector<std::string> defrostControlTypeValues();

    static std::vector<std::string> verticalLocationValues();

    /** @name Getters */
    //@{

    boost::optional<Schedule> availabilitySchedule() const;

    std::string capacityRatingType() const;

    boost::optional<double> ratedUnitLoadFactor() const;

    boost::optional<double> ratedCapacity() const;

    double ratedRelativeHumidity() const;

    bool isRatedRelativeHumidityDefaulted() const;

    double ratedCoolingSourceTemperature() const;

    double ratedTemperatureDifferenceDT1() const;

    boost::optional<double> maximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature() const;

    double coilMaterialCorrectionFactor() const;

    bool isCoilMaterialCorrectionFactorDefaulted() const;

    double refrigerantCorrectionFactor() const;

    bool isRefrigerantCorrectionFactorDefaulted() const;

    // std::string capacityCorrectionCurveType() const;

    // bool isCapacityCorrectionCurveTypeDefaulted() const;

    // boost::optional<CurveLinear> capacityCorrectionCurve() const;

    double sHR60CorrectionFactor() const;

    bool isSHR60CorrectionFactorDefaulted() const;

    double ratedTotalHeatingPower() const;

    boost::optional<Schedule> heatingPowerSchedule() const;

    std::string fanSpeedControlType() const;

    bool isFanSpeedControlTypeDefaulted() const;

    double ratedFanPower() const;

    bool isRatedFanPowerDefaulted() const;

    double ratedAirFlow() const;

    double minimumFanAirFlowRatio() const;

    bool isMinimumFanAirFlowRatioDefaulted() const;

    std::string defrostType() const;

    bool isDefrostTypeDefaulted() const;

    std::string defrostControlType() const;

    bool isDefrostControlTypeDefaulted() const;

    Schedule defrostSchedule() const;

    boost::optional<Schedule> defrostDripDownSchedule() const;

    boost::optional<double> defrostPower() const;

    boost::optional<double> temperatureTerminationDefrostFractiontoIce() const;

    std::string verticalLocation() const;

    bool isVerticalLocationDefaulted() const;

    double averageRefrigerantChargeInventory() const;

    bool isAverageRefrigerantChargeInventoryDefaulted() const;

    // Returns the parent RefrigerationSystem if any
    boost::optional<RefrigerationSystem> system() const;

    // Returns the parent RefrigerationSecondarySystem if any
    boost::optional<RefrigerationSecondarySystem> secondarySystem() const;

    // Returns the parent RefrigerationCompresorRack if any
    boost::optional<RefrigerationCompressorRack> compressorRack() const;

    //@}
    /** @name Setters */
    //@{

    bool setAvailabilitySchedule(Schedule& schedule);

    void resetAvailabilitySchedule();

    bool setCapacityRatingType(const std::string& capacityRatingType);

    bool setRatedUnitLoadFactor(double ratedUnitLoadFactor);

    void resetRatedUnitLoadFactor();

    bool setRatedCapacity(double ratedCapacity);

    void resetRatedCapacity();

    bool setRatedRelativeHumidity(double ratedRelativeHumidity);

    void resetRatedRelativeHumidity();

    bool setRatedCoolingSourceTemperature(double ratedCoolingSourceTemperature);

    bool setRatedTemperatureDifferenceDT1(double ratedTemperatureDifferenceDT1);

    bool setMaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature(
      double maximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature);

    void resetMaximumTemperatureDifferenceBetweenInletAirandEvaporatingTemperature();

    bool setCoilMaterialCorrectionFactor(double coilMaterialCorrectionFactor);

    void resetCoilMaterialCorrectionFactor();

    bool setRefrigerantCorrectionFactor(double refrigerantCorrectionFactor);

    void resetRefrigerantCorrectionFactor();

    // bool setCapacityCorrectionCurveType(const std::string& capacityCorrectionCurveType);

    // void resetCapacityCorrectionCurveType();

    // bool setCapacityCorrectionCurve(const CurveLinear& curveLinear);

    // void resetCapacityCorrectionCurve();

    bool setSHR60CorrectionFactor(double sHR60CorrectionFactor);

    void resetSHR60CorrectionFactor();

    bool setRatedTotalHeatingPower(double ratedTotalHeatingPower);

    bool setHeatingPowerSchedule(Schedule& schedule);

    void resetHeatingPowerSchedule();

    bool setFanSpeedControlType(const std::string& fanSpeedControlType);

    void resetFanSpeedControlType();

    bool setRatedFanPower(double ratedFanPower);

    void resetRatedFanPower();

    bool setRatedAirFlow(double ratedAirFlow);

    bool setMinimumFanAirFlowRatio(double minimumFanAirFlowRatio);

    void resetMinimumFanAirFlowRatio();

    bool setDefrostType(const std::string& defrostType);

    void resetDefrostType();

    bool setDefrostControlType(const std::string& defrostControlType);

    void resetDefrostControlType();

    bool setDefrostSchedule(Schedule& schedule);

    bool setDefrostDripDownSchedule(Schedule& schedule);

    void resetDefrostDripDownSchedule();

    bool setDefrostPower(double defrostPower);

    void resetDefrostPower();

    bool setTemperatureTerminationDefrostFractiontoIce(double temperatureTerminationDefrostFractiontoIce);

    void resetTemperatureTerminationDefrostFractiontoIce();

    // bool setVerticalLocation(const std::string& verticalLocation);

    // void resetVerticalLocation();

    bool setAverageRefrigerantChargeInventory(double averageRefrigerantChargeInventory);

    void resetAverageRefrigerantChargeInventory();

    bool addToSystem(RefrigerationSystem& system);

    bool addToSecondarySystem(RefrigerationSecondarySystem& secondarySystem);

    bool addToCompressorRack(RefrigerationCompressorRack& compressorRack);

    // Remove from parent system if any
    void removeFromSystem();

    // Remove from parent secondary system if any
    void removeFromSecondarySystem();

    // Remove from parent compressor rack if any
    void removeFromCompressorRack();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::RefrigerationAirChiller_Impl;

    explicit RefrigerationAirChiller(std::shared_ptr<detail::RefrigerationAirChiller_Impl> impl);

    friend class detail::RefrigerationAirChiller_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationAirChiller");
  };

  /** \relates RefrigerationAirChiller*/
  using OptionalRefrigerationAirChiller = boost::optional<RefrigerationAirChiller>;

  /** \relates RefrigerationAirChiller*/
  using RefrigerationAirChillerVector = std::vector<RefrigerationAirChiller>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONAIRCHILLER_HPP
