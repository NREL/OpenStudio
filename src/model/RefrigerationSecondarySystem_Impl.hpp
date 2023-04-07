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

#ifndef MODEL_REFRIGERATIONSECONDARYSYSTEM_IMPL_HPP
#define MODEL_REFRIGERATIONSECONDARYSYSTEM_IMPL_HPP

#include "ModelAPI.hpp"
#include "ParentObject_Impl.hpp"

namespace openstudio {
namespace model {

  class ModelObjectList;
  class CurveCubic;
  class ThermalZone;
  class RefrigerationSystem;
  class RefrigerationAirChiller;
  class RefrigerationCase;
  class RefrigerationWalkIn;

  namespace detail {

    /** RefrigerationSecondarySystem_Impl is a ParentObject_Impl that is the implementation class for RefrigerationSecondarySystem.*/
    class MODEL_API RefrigerationSecondarySystem_Impl : public ParentObject_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      RefrigerationSecondarySystem_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RefrigerationSecondarySystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RefrigerationSecondarySystem_Impl(const RefrigerationSecondarySystem_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RefrigerationSecondarySystem_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IdfObject> remove() override;

      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      virtual std::vector<ModelObject> children() const override;

      //@}
      /** @name Getters */
      //@{

      std::vector<RefrigerationCase> cases() const;

      std::vector<RefrigerationWalkIn> walkins() const;

      std::vector<RefrigerationAirChiller> airChillers() const;

      template <class T>
      std::vector<T> listTemplate(const boost::optional<ModelObjectList>& modelObjectList) const;

      boost::optional<ModelObjectList> refrigeratedCaseAndWalkInList() const;

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

      boost::optional<RefrigerationSystem> system() const;

      //@}
      /** @name Setters */
      //@{

      template <class T>
      void removeAllTemplate(boost::optional<ModelObjectList>& modelObjectList);

      template <class T>
      void removeTemplate(const T& modelObject, boost::optional<ModelObjectList>& modelObjectList);

      template <class T>
      bool addTemplate(const T& modelObject, boost::optional<ModelObjectList>& modelObjectList);

      bool addCase(const RefrigerationCase& refrigerationCase);

      void removeCase(const RefrigerationCase& refrigerationCase);

      void removeAllCases();

      bool addWalkin(const RefrigerationWalkIn& refrigerationWalkin);

      void removeWalkin(const RefrigerationWalkIn& refrigerationWalkin);

      void removeAllWalkins();

      bool addAirChiller(const RefrigerationAirChiller& airChiller);

      void removeAirChiller(const RefrigerationAirChiller& airChiller);

      void removeAllAirChillers();

      bool setRefrigeratedCaseAndWalkInList(const boost::optional<ModelObjectList>& modelObjectList);

      void resetRefrigeratedCaseAndWalkInList();

      bool setCirculatingFluidName(const std::string& circulatingFluidName);

      bool setGlycolConcentration(boost::optional<int> glycolConcentration);

      void resetGlycolConcentration();

      bool setEvaporatorCapacity(boost::optional<double> evaporatorCapacity);

      void resetEvaporatorCapacity();

      bool setEvaporatorFlowRateforSecondaryFluid(boost::optional<double> evaporatorFlowRateforSecondaryFluid);

      void resetEvaporatorFlowRateforSecondaryFluid();

      bool setEvaporatorEvaporatingTemperature(double evaporatorEvaporatingTemperature);

      bool setEvaporatorApproachTemperatureDifference(double evaporatorApproachTemperatureDifference);

      bool setEvaporatorRangeTemperatureDifference(boost::optional<double> evaporatorRangeTemperatureDifference);

      void resetEvaporatorRangeTemperatureDifference();

      bool setNumberofPumpsinLoop(int numberofPumpsinLoop);

      void resetNumberofPumpsinLoop();

      bool setTotalPumpFlowRate(boost::optional<double> totalPumpFlowRate);

      void resetTotalPumpFlowRate();

      bool setTotalPumpPower(boost::optional<double> totalPumpPower);

      void resetTotalPumpPower();

      bool setTotalPumpHead(boost::optional<double> totalPumpHead);

      void resetTotalPumpHead();

      bool setPhaseChangeCirculatingRate(double phaseChangeCirculatingRate);

      void resetPhaseChangeCirculatingRate();

      bool setPumpDriveType(const std::string& pumpDriveType);

      void resetPumpDriveType();

      bool setVariableSpeedPumpCubicCurve(const boost::optional<CurveCubic>& curveCubic);

      void resetVariableSpeedPumpCubicCurve();

      bool setPumpMotorHeattoFluid(double pumpMotorHeattoFluid);

      void resetPumpMotorHeattoFluid();

      bool setSumUADistributionPiping(double sumUADistributionPiping);

      void resetSumUADistributionPiping();

      bool setDistributionPipingZone(const boost::optional<ThermalZone>& thermalZone);

      void resetDistributionPipingZone();

      bool setSumUAReceiverSeparatorShell(double sumUAReceiverSeparatorShell);

      void resetSumUAReceiverSeparatorShell();

      bool setReceiverSeparatorZone(const boost::optional<ThermalZone>& thermalZone);

      void resetReceiverSeparatorZone();

      bool setEvaporatorRefrigerantInventory(double evaporatorRefrigerantInventory);

      void resetEvaporatorRefrigerantInventory();

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      void resetEndUseSubcategory();

      void removeFromSystem();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.RefrigerationSecondarySystem");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONSECONDARYSYSTEM_IMPL_HPP
