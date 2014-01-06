/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#ifndef MODEL_REFRIGERATIONSECONDARYSYSTEM_IMPL_HPP
#define MODEL_REFRIGERATIONSECONDARYSYSTEM_IMPL_HPP

#include <model/ModelAPI.hpp>
#include <model/ParentObject_Impl.hpp>

namespace openstudio {
namespace model {

class ModelObjectList;
class CurveCubic;
class ThermalZone;
class RefrigerationAirChiller;
class RefrigerationCase;
class RefrigerationWalkIn;

namespace detail {

  /** RefrigerationSecondarySystem_Impl is a ParentObject_Impl that is the implementation class for RefrigerationSecondarySystem.*/
  class MODEL_API RefrigerationSecondarySystem_Impl : public ParentObject_Impl {
   public:
    /** @name Constructors and Destructors */
    //@{

    RefrigerationSecondarySystem_Impl(const IdfObject& idfObject,
                                      Model_Impl* model,
                                      bool keepHandle);

    RefrigerationSecondarySystem_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                      Model_Impl* model,
                                      bool keepHandle);

    RefrigerationSecondarySystem_Impl(const RefrigerationSecondarySystem_Impl& other,
                                      Model_Impl* model,
                                      bool keepHandle);

    virtual ~RefrigerationSecondarySystem_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual ModelObject clone(Model model) const;

    virtual std::vector<IdfObject> remove();

    virtual std::vector<IddObjectType> allowableChildTypes() const;

    virtual std::vector<ModelObject> children() const;

    //@}
    /** @name Getters */
    //@{

    std::vector<RefrigerationCase> cases() const;

    std::vector<RefrigerationWalkIn> walkins() const;

    std::vector<RefrigerationAirChiller> airChillers() const;

    template <class T>
    std::vector<T> listTemplate( const boost::optional<ModelObjectList>& modelObjectList ) const;

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

    //@}
    /** @name Setters */
    //@{

    template <class T>
    void removeAllTemplate( boost::optional<ModelObjectList>& modelObjectList );

    template <class T>
    void removeTemplate( const T & modelObject, boost::optional<ModelObjectList>& modelObjectList );

    template <class T>
    bool addTemplate( const T & modelObject, boost::optional<ModelObjectList>& modelObjectList );

    bool addCase( const RefrigerationCase & refrigerationCase);

    void removeCase( const RefrigerationCase & refrigerationCase);

    void removeAllCases();

    bool addWalkin( const RefrigerationWalkIn & refrigerationWalkin);

    void removeWalkin( const RefrigerationWalkIn & refrigerationWalkin);

    void removeAllWalkins();

    bool addAirChiller( const RefrigerationAirChiller & airChiller);

    void removeAirChiller( const RefrigerationAirChiller & airChiller);

    void removeAllAirChillers();

    bool setRefrigeratedCaseAndWalkInList(const boost::optional<ModelObjectList>& modelObjectList);

    void resetRefrigeratedCaseAndWalkInList();

    bool setCirculatingFluidName(std::string circulatingFluidName);

    bool setGlycolConcentration(boost::optional<int> glycolConcentration);

    void resetGlycolConcentration();

    bool setEvaporatorCapacity(boost::optional<double> evaporatorCapacity);

    void resetEvaporatorCapacity();

    bool setEvaporatorFlowRateforSecondaryFluid(boost::optional<double> evaporatorFlowRateforSecondaryFluid);

    void resetEvaporatorFlowRateforSecondaryFluid();

    void setEvaporatorEvaporatingTemperature(double evaporatorEvaporatingTemperature);

    void setEvaporatorApproachTemperatureDifference(double evaporatorApproachTemperatureDifference);

    void setEvaporatorRangeTemperatureDifference(boost::optional<double> evaporatorRangeTemperatureDifference);

    void resetEvaporatorRangeTemperatureDifference();

    void setNumberofPumpsinLoop(int numberofPumpsinLoop);

    void resetNumberofPumpsinLoop();

    bool setTotalPumpFlowRate(boost::optional<double> totalPumpFlowRate);

    void resetTotalPumpFlowRate();

    bool setTotalPumpPower(boost::optional<double> totalPumpPower);

    void resetTotalPumpPower();

    bool setTotalPumpHead(boost::optional<double> totalPumpHead);

    void resetTotalPumpHead();

    bool setPhaseChangeCirculatingRate(double phaseChangeCirculatingRate);

    void resetPhaseChangeCirculatingRate();

    bool setPumpDriveType(std::string pumpDriveType);

    void resetPumpDriveType();

    bool setVariableSpeedPumpCubicCurve(const boost::optional<CurveCubic>& curveCubic);

    void resetVariableSpeedPumpCubicCurve();

    bool setPumpMotorHeattoFluid(double pumpMotorHeattoFluid);

    void resetPumpMotorHeattoFluid();

    void setSumUADistributionPiping(double sumUADistributionPiping);

    void resetSumUADistributionPiping();

    bool setDistributionPipingZone(const boost::optional<ThermalZone>& thermalZone);

    void resetDistributionPipingZone();

    void setSumUAReceiverSeparatorShell(double sumUAReceiverSeparatorShell);

    void resetSumUAReceiverSeparatorShell();

    bool setReceiverSeparatorZone(const boost::optional<ThermalZone>& thermalZone);

    void resetReceiverSeparatorZone();

    void setEvaporatorRefrigerantInventory(double evaporatorRefrigerantInventory);

    void resetEvaporatorRefrigerantInventory();

    void setEndUseSubcategory(std::string endUseSubcategory);

    void resetEndUseSubcategory();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.RefrigerationSecondarySystem");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONSECONDARYSYSTEM_IMPL_HPP

