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

#ifndef MODEL_REFRIGERATIONSECONDARYSYSTEM_HPP
#define MODEL_REFRIGERATIONSECONDARYSYSTEM_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

  class CurveCubic;
  class ThermalZone;
  class RefrigerationSystem;
  class RefrigerationAirChiller;
  class RefrigerationCase;
  class RefrigerationWalkIn;

  namespace detail {

    class RefrigerationSecondarySystem_Impl;

  }  // namespace detail

  /** RefrigerationSecondarySystem is a ParentObject that wraps the OpenStudio IDD object 'OS:Refrigeration:SecondarySystem'. */
  class MODEL_API RefrigerationSecondarySystem : public ParentObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit RefrigerationSecondarySystem(const Model& model);

    virtual ~RefrigerationSecondarySystem() = default;
    // Default the copy and move operators because the virtual dtor is explicit
    RefrigerationSecondarySystem(const RefrigerationSecondarySystem& other) = default;
    RefrigerationSecondarySystem(RefrigerationSecondarySystem&& other) = default;
    RefrigerationSecondarySystem& operator=(const RefrigerationSecondarySystem&) = default;
    RefrigerationSecondarySystem& operator=(RefrigerationSecondarySystem&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> circulatingFluidNameValues();

    static std::vector<std::string> pumpDriveTypeValues();

    bool addCase(const RefrigerationCase& refrigerationCase);

    void removeCase(const RefrigerationCase& refrigerationCase);

    void removeAllCases();

    std::vector<RefrigerationCase> cases() const;

    bool addWalkin(const RefrigerationWalkIn& refrigerationWalkin);

    void removeWalkin(const RefrigerationWalkIn& refrigerationWalkin);

    void removeAllWalkins();

    std::vector<RefrigerationWalkIn> walkins() const;

    bool addAirChiller(const RefrigerationAirChiller& airChiller);

    void removeAirChiller(const RefrigerationAirChiller& airChiller);

    void removeAllAirChillers();

    std::vector<RefrigerationAirChiller> airChillers() const;

    /** @name Getters */
    //@{

    std::string circulatingFluidName() const;

    boost::optional<int> glycolConcentration() const;

    boost::optional<double> evaporatorCapacity() const;

    boost::optional<double> evaporatorFlowRateforSecondaryFluid() const;

    double evaporatorEvaporatingTemperature() const;

    double evaporatorApproachTemperatureDifference() const;

    boost::optional<double> evaporatorRangeTemperatureDifference() const;

    int numberofPumpsinLoop() const;

    bool isNumberofPumpsinLoopDefaulted() const;

    boost::optional<double> totalPumpFlowRate() const;

    boost::optional<double> totalPumpPower() const;

    boost::optional<double> totalPumpHead() const;

    double phaseChangeCirculatingRate() const;

    bool isPhaseChangeCirculatingRateDefaulted() const;

    std::string pumpDriveType() const;

    bool isPumpDriveTypeDefaulted() const;

    boost::optional<CurveCubic> variableSpeedPumpCubicCurve() const;

    double pumpMotorHeattoFluid() const;

    bool isPumpMotorHeattoFluidDefaulted() const;

    double sumUADistributionPiping() const;

    bool isSumUADistributionPipingDefaulted() const;

    boost::optional<ThermalZone> distributionPipingZone() const;

    double sumUAReceiverSeparatorShell() const;

    bool isSumUAReceiverSeparatorShellDefaulted() const;

    boost::optional<ThermalZone> receiverSeparatorZone() const;

    double evaporatorRefrigerantInventory() const;

    bool isEvaporatorRefrigerantInventoryDefaulted() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    // Returns the parent RefrigerationSystem if any
    boost::optional<RefrigerationSystem> system() const;

    //@}
    /** @name Setters */
    //@{

    void resetRefrigeratedCaseAndWalkInList();

    bool setCirculatingFluidName(const std::string& circulatingFluidName);

    bool setGlycolConcentration(int glycolConcentration);

    void resetGlycolConcentration();

    bool setEvaporatorCapacity(double evaporatorCapacity);

    void resetEvaporatorCapacity();

    bool setEvaporatorFlowRateforSecondaryFluid(double evaporatorFlowRateforSecondaryFluid);

    void resetEvaporatorFlowRateforSecondaryFluid();

    bool setEvaporatorEvaporatingTemperature(double evaporatorEvaporatingTemperature);

    bool setEvaporatorApproachTemperatureDifference(double evaporatorApproachTemperatureDifference);

    bool setEvaporatorRangeTemperatureDifference(double evaporatorRangeTemperatureDifference);

    void resetEvaporatorRangeTemperatureDifference();

    bool setNumberofPumpsinLoop(int numberofPumpsinLoop);

    void resetNumberofPumpsinLoop();

    bool setTotalPumpFlowRate(double totalPumpFlowRate);

    void resetTotalPumpFlowRate();

    bool setTotalPumpPower(double totalPumpPower);

    void resetTotalPumpPower();

    bool setTotalPumpHead(double totalPumpHead);

    void resetTotalPumpHead();

    bool setPhaseChangeCirculatingRate(double phaseChangeCirculatingRate);

    void resetPhaseChangeCirculatingRate();

    bool setPumpDriveType(const std::string& pumpDriveType);

    void resetPumpDriveType();

    bool setVariableSpeedPumpCubicCurve(const CurveCubic& curveCubic);

    void resetVariableSpeedPumpCubicCurve();

    bool setPumpMotorHeattoFluid(double pumpMotorHeattoFluid);

    void resetPumpMotorHeattoFluid();

    bool setSumUADistributionPiping(double sumUADistributionPiping);

    void resetSumUADistributionPiping();

    bool setDistributionPipingZone(const ThermalZone& thermalZone);

    void resetDistributionPipingZone();

    bool setSumUAReceiverSeparatorShell(double sumUAReceiverSeparatorShell);

    void resetSumUAReceiverSeparatorShell();

    bool setReceiverSeparatorZone(const ThermalZone& thermalZone);

    void resetReceiverSeparatorZone();

    bool setEvaporatorRefrigerantInventory(double evaporatorRefrigerantInventory);

    void resetEvaporatorRefrigerantInventory();

    bool setEndUseSubcategory(const std::string& endUseSubcategory);

    void resetEndUseSubcategory();

    // Remove from parent system if any
    void removeFromSystem();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::RefrigerationSecondarySystem_Impl;

    explicit RefrigerationSecondarySystem(std::shared_ptr<detail::RefrigerationSecondarySystem_Impl> impl);

    friend class detail::RefrigerationSecondarySystem_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationSecondarySystem");
  };

  /** \relates RefrigerationSecondarySystem*/
  using OptionalRefrigerationSecondarySystem = boost::optional<RefrigerationSecondarySystem>;

  /** \relates RefrigerationSecondarySystem*/
  using RefrigerationSecondarySystemVector = std::vector<RefrigerationSecondarySystem>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONSECONDARYSYSTEM_HPP
