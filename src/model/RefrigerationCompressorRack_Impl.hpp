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

#ifndef MODEL_REFRIGERATIONCOMPRESSORRACK_IMPL_HPP
#define MODEL_REFRIGERATIONCOMPRESSORRACK_IMPL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent_Impl.hpp"

namespace openstudio {
namespace model {

  class Curve;
  class Schedule;
  class ThermalZone;
  class ModelObjectList;
  class RefrigerationAirChiller;
  class RefrigerationCase;
  class RefrigerationWalkIn;

  namespace detail {

    /** RefrigerationCompressorRack_Impl is a StraightComponent_Impl that is the implementation class for RefrigerationCompressorRack.*/
    class MODEL_API RefrigerationCompressorRack_Impl : public StraightComponent_Impl
    {
     public:
      /** @name Constructors and Destructors */
      //@{

      RefrigerationCompressorRack_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      RefrigerationCompressorRack_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      RefrigerationCompressorRack_Impl(const RefrigerationCompressorRack_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~RefrigerationCompressorRack_Impl() = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual unsigned inletPort() const override;

      virtual unsigned outletPort() const override;

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual bool addToNode(Node& node) override;

      virtual ModelObject clone(Model model) const override;

      virtual std::vector<IddObjectType> allowableChildTypes() const override;

      virtual std::vector<ModelObject> children() const override;

      virtual std::vector<IdfObject> remove() override;

      virtual ComponentType componentType() const override;
      virtual std::vector<FuelType> coolingFuelTypes() const override;
      virtual std::vector<FuelType> heatingFuelTypes() const override;
      virtual std::vector<AppGFuelType> appGHeatingFuelTypes() const override;

      //@}
      /** @name Getters */
      //@{

      std::string heatRejectionLocation() const;

      double designCompressorRackCOP() const;

      Curve compressorRackCOPFunctionofTemperatureCurve() const;

      double designCondenserFanPower() const;

      boost::optional<Curve> condenserFanPowerFunctionofTemperatureCurve() const;

      std::string condenserType() const;

      std::string waterCooledLoopFlowType() const;

      boost::optional<Schedule> waterCooledCondenserOutletTemperatureSchedule() const;

      boost::optional<double> waterCooledCondenserDesignFlowRate() const;

      boost::optional<double> waterCooledCondenserMaximumFlowRate() const;

      double waterCooledCondenserMaximumWaterOutletTemperature() const;

      double waterCooledCondenserMinimumWaterInletTemperature() const;

      boost::optional<Schedule> evaporativeCondenserAvailabilitySchedule() const;

      double evaporativeCondenserEffectiveness() const;

      boost::optional<double> evaporativeCondenserAirFlowRate() const;

      bool isEvaporativeCondenserAirFlowRateAutocalculated() const;

      double basinHeaterCapacity() const;

      double basinHeaterSetpointTemperature() const;

      boost::optional<double> designEvaporativeCondenserWaterPumpPower() const;

      bool isDesignEvaporativeCondenserWaterPumpPowerAutocalculated() const;

      // boost::optional<HVACComponent> evaporativeWaterSupplyTank() const;

      // boost::optional<std::string> condenserAirInletNodeName() const;

      std::string endUseSubcategory() const;

      bool isEndUseSubcategoryDefaulted() const;

      std::vector<RefrigerationCase> cases() const;

      std::vector<RefrigerationWalkIn> walkins() const;

      std::vector<RefrigerationAirChiller> airChillers() const;

      template <class T>
      std::vector<T> listTemplate(const boost::optional<ModelObjectList>& modelObjectList) const;

      boost::optional<ModelObjectList> refrigeratedCaseAndWalkInList() const;

      boost::optional<ThermalZone> heatRejectionZone() const;

      //@}
      /** @name Setters */
      //@{

      bool setHeatRejectionLocation(const std::string& heatRejectionLocation);

      bool setDesignCompressorRackCOP(double designCompressorRackCOP);

      bool setCompressorRackCOPFunctionofTemperatureCurve(const Curve& curve);

      bool setDesignCondenserFanPower(double designCondenserFanPower);

      bool setCondenserFanPowerFunctionofTemperatureCurve(const Curve& curve);

      void resetCondenserFanPowerFunctionofTemperatureCurve();

      bool setCondenserType(const std::string& condenserType);

      bool setWaterCooledLoopFlowType(const std::string& waterCooledLoopFlowType);

      bool setWaterCooledCondenserOutletTemperatureSchedule(Schedule& schedule);

      void resetWaterCooledCondenserOutletTemperatureSchedule();

      bool setWaterCooledCondenserDesignFlowRate(double waterCooledCondenserDesignFlowRate);

      void resetWaterCooledCondenserDesignFlowRate();

      bool setWaterCooledCondenserMaximumFlowRate(double waterCooledCondenserMaximumFlowRate);

      void resetWaterCooledCondenserMaximumFlowRate();

      bool setWaterCooledCondenserMaximumWaterOutletTemperature(double waterCooledCondenserMaximumWaterOutletTemperature);

      bool setWaterCooledCondenserMinimumWaterInletTemperature(double waterCooledCondenserMinimumWaterInletTemperature);

      bool setEvaporativeCondenserAvailabilitySchedule(Schedule& schedule);

      void resetEvaporativeCondenserAvailabilitySchedule();

      bool setEvaporativeCondenserEffectiveness(double evaporativeCondenserEffectiveness);

      bool setEvaporativeCondenserAirFlowRate(double evaporativeCondenserAirFlowRate);

      bool autocalculateEvaporativeCondenserAirFlowRate();

      bool setBasinHeaterCapacity(double basinHeaterCapacity);

      bool setBasinHeaterSetpointTemperature(double basinHeaterSetpointTemperature);

      bool setDesignEvaporativeCondenserWaterPumpPower(double designEvaporativeCondenserWaterPumpPower);

      bool autocalculateDesignEvaporativeCondenserWaterPumpPower();

      // bool setEvaporativeWaterSupplyTank(const HVACComponent& evaporativeWaterSupplyTank);

      // void resetEvaporativeWaterSupplyTank();

      // bool setCondenserAirInletNodeName(const boost::optional<std::string>& condenserAirInletNodeName);

      // void resetCondenserAirInletNodeName();

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      void resetEndUseSubcategory();

      bool addCase(const RefrigerationCase& refrigerationCase);

      void removeCase(const RefrigerationCase& refrigerationCase);

      void removeAllCases();

      bool addWalkin(const RefrigerationWalkIn& refrigerationWalkin);

      void removeWalkin(const RefrigerationWalkIn& refrigerationWalkin);

      void removeAllWalkins();

      bool addAirChiller(const RefrigerationAirChiller& airChiller);

      void removeAirChiller(const RefrigerationAirChiller& airChiller);

      void removeAllAirChillers();

      template <class T>
      void removeAllTemplate(boost::optional<ModelObjectList>& modelObjectList);

      template <class T>
      void removeTemplate(const T& modelObject, boost::optional<ModelObjectList>& modelObjectList);

      template <class T>
      bool addTemplate(const T& modelObject, boost::optional<ModelObjectList>& modelObjectList);

      bool setRefrigeratedCaseAndWalkInList(const boost::optional<ModelObjectList>& modelObjectList);

      void resetRefrigeratedCaseAndWalkInList();

      bool setHeatRejectionZone(const boost::optional<ThermalZone>& thermalZone);

      void resetHeatRejectionZone();

      //@}
      /** @name Other */
      //@{

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.RefrigerationCompressorRack");

      // optional getters for use by children so can remove() this object if the constructor fails
      boost::optional<Curve> optionalCompressorRackCOPFunctionofTemperatureCurve() const;
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_REFRIGERATIONCOMPRESSORRACK_IMPL_HPP
