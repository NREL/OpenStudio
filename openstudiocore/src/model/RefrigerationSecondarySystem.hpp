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

#ifndef MODEL_REFRIGERATIONSECONDARYSYSTEM_HPP
#define MODEL_REFRIGERATIONSECONDARYSYSTEM_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {

namespace model {

class CurveCubic;
class ThermalZone;
class RefrigerationAirChiller;
class RefrigerationCase;
class RefrigerationWalkIn;

namespace detail {

  class RefrigerationSecondarySystem_Impl;

} // detail

/** RefrigerationSecondarySystem is a ParentObject that wraps the OpenStudio IDD object 'OS:Refrigeration:SecondarySystem'. */
class MODEL_API RefrigerationSecondarySystem : public ParentObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit RefrigerationSecondarySystem(const Model& model);

  virtual ~RefrigerationSecondarySystem() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> circulatingFluidNameValues();

  static std::vector<std::string> pumpDriveTypeValues();

  bool addCase( const RefrigerationCase & refrigerationCase);

  void removeCase( const RefrigerationCase & refrigerationCase);

  void removeAllCases();

  std::vector<RefrigerationCase> cases() const;

  bool addWalkin( const RefrigerationWalkIn & refrigerationWalkin);

  void removeWalkin( const RefrigerationWalkIn & refrigerationWalkin);

  void removeAllWalkins();

  std::vector<RefrigerationWalkIn> walkins() const;

  bool addAirChiller( const RefrigerationAirChiller & airChiller);

  void removeAirChiller( const RefrigerationAirChiller & airChiller);

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

  //@}
  /** @name Setters */
  //@{

  void resetRefrigeratedCaseAndWalkInList();

  bool setCirculatingFluidName(std::string circulatingFluidName);

  bool setGlycolConcentration(int glycolConcentration);

  void resetGlycolConcentration();

  bool setEvaporatorCapacity(double evaporatorCapacity);

  void resetEvaporatorCapacity();

  bool setEvaporatorFlowRateforSecondaryFluid(double evaporatorFlowRateforSecondaryFluid);

  void resetEvaporatorFlowRateforSecondaryFluid();

  void setEvaporatorEvaporatingTemperature(double evaporatorEvaporatingTemperature);

  void setEvaporatorApproachTemperatureDifference(double evaporatorApproachTemperatureDifference);

  void setEvaporatorRangeTemperatureDifference(double evaporatorRangeTemperatureDifference);

  void resetEvaporatorRangeTemperatureDifference();

  void setNumberofPumpsinLoop(int numberofPumpsinLoop);

  void resetNumberofPumpsinLoop();

  bool setTotalPumpFlowRate(double totalPumpFlowRate);

  void resetTotalPumpFlowRate();

  bool setTotalPumpPower(double totalPumpPower);

  void resetTotalPumpPower();

  bool setTotalPumpHead(double totalPumpHead);

  void resetTotalPumpHead();

  bool setPhaseChangeCirculatingRate(double phaseChangeCirculatingRate);

  void resetPhaseChangeCirculatingRate();

  bool setPumpDriveType(std::string pumpDriveType);

  void resetPumpDriveType();

  bool setVariableSpeedPumpCubicCurve(const CurveCubic& curveCubic);

  void resetVariableSpeedPumpCubicCurve();

  bool setPumpMotorHeattoFluid(double pumpMotorHeattoFluid);

  void resetPumpMotorHeattoFluid();

  void setSumUADistributionPiping(double sumUADistributionPiping);

  void resetSumUADistributionPiping();

  bool setDistributionPipingZone(const ThermalZone& thermalZone);

  void resetDistributionPipingZone();

  void setSumUAReceiverSeparatorShell(double sumUAReceiverSeparatorShell);

  void resetSumUAReceiverSeparatorShell();

  bool setReceiverSeparatorZone(const ThermalZone& thermalZone);

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
  /// @cond
  typedef detail::RefrigerationSecondarySystem_Impl ImplType;

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
typedef boost::optional<RefrigerationSecondarySystem> OptionalRefrigerationSecondarySystem;

/** \relates RefrigerationSecondarySystem*/
typedef std::vector<RefrigerationSecondarySystem> RefrigerationSecondarySystemVector;

} // model
} // openstudio

#endif // MODEL_REFRIGERATIONSECONDARYSYSTEM_HPP

