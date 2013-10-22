/**********************************************************************
 *  Copyright (c) 2008-2013, Alliance for Sustainable Energy.
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

#ifndef MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMECOOLEDBEAM_HPP
#define MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMECOOLEDBEAM_HPP

#include <model/ModelAPI.hpp>
#include <model/StraightComponent.hpp>
//#include <model/ThermalZone.hpp>
#include <model/ModelObject.hpp>
#include <model/Connection.hpp>
//#include <model/Model.hpp>

namespace openstudio {

namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  class AirTerminalSingleDuctConstantVolumeCooledBeam_Impl;

} // detail

/** AirTerminalSingleDuctConstantVolumeCooledBeam is a ZoneHAVC that wraps the OpenStudio IDD object 'OS:AirTerminal:SingleDuct:ConstantVolume:CooledBeam'. */
class MODEL_API AirTerminalSingleDuctConstantVolumeCooledBeam : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AirTerminalSingleDuctConstantVolumeCooledBeam(const Model& model,
																																																															Schedule & availabilitySchedule,
																																																															HVACComponent & coilCoolingCooledBeam);

  virtual ~AirTerminalSingleDuctConstantVolumeCooledBeam() {}

  //@}

  static IddObjectType iddObjectType();
  
  /** @name Getters */
  //@{
		
		Schedule availabilitySchedule() const;
  
		HVACComponent coilCoolingCooledBeam() const;
  
  static std::vector<std::string> cooledBeamTypeValues();
  
		std::string cooledBeamType() const;
  
  boost::optional<double> supplyAirVolumetricFlowRate() const;

  bool isSupplyAirVolumetricFlowRateDefaulted() const;

  bool isSupplyAirVolumetricFlowRateAutosized() const;

  boost::optional<double> maximumTotalChilledWaterVolumetricFlowRate() const;

  bool isMaximumTotalChilledWaterVolumetricFlowRateDefaulted() const;

  bool isMaximumTotalChilledWaterVolumetricFlowRateAutosized() const;

  boost::optional<int> numberofBeams() const;

  bool isNumberofBeamsDefaulted() const;

  bool isNumberofBeamsAutosized() const;

  boost::optional<double> beamLength() const;

  bool isBeamLengthDefaulted() const;

  bool isBeamLengthAutosized() const;

  double designInletWaterTemperature() const;

  bool isDesignInletWaterTemperatureDefaulted() const;

  double designOutletWaterTemperature() const;

  bool isDesignOutletWaterTemperatureDefaulted() const;

  boost::optional<double> coefficientofInductionKin() const;

  bool isCoefficientofInductionKinDefaulted() const;

  bool isCoefficientofInductionKinAutocalculated() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);
  
  bool setCoolingCoil(HVACComponent& coilCoolingCooledBeam);

  bool setCooledBeamType(std::string cooledBeamType);
  
  bool setSupplyAirVolumetricFlowRate(double supplyAirVolumetricFlowRate);

  void resetSupplyAirVolumetricFlowRate();

  void autosizeSupplyAirVolumetricFlowRate();

  bool setMaximumTotalChilledWaterVolumetricFlowRate(double maximumTotalChilledWaterVolumetricFlowRate);

  void resetMaximumTotalChilledWaterVolumetricFlowRate();

  void autosizeMaximumTotalChilledWaterVolumetricFlowRate();

  bool setNumberofBeams(int numberofBeams);

  void resetNumberofBeams();

  void autosizeNumberofBeams();

  bool setBeamLength(double beamLength);

  void resetBeamLength();

  void autosizeBeamLength();

  bool setDesignInletWaterTemperature(double designInletWaterTemperature);

  void resetDesignInletWaterTemperature();

  bool setDesignOutletWaterTemperature(double designOutletWaterTemperature);

  void resetDesignOutletWaterTemperature();

  bool setCoefficientofInductionKin(double coefficientofInductionKin);

  void resetCoefficientofInductionKin();

  void autocalculateCoefficientofInductionKin();
  
  // boost::optional<ThermalZone> thermalZone();
  
  // bool addToThermalZone(ThermalZone & thermalZone);
  
  // void removeFromThermalZone();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl ImplType;

  explicit AirTerminalSingleDuctConstantVolumeCooledBeam(boost::shared_ptr<detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl> impl);

  friend class detail::AirTerminalSingleDuctConstantVolumeCooledBeam_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctConstantVolumeCooledBeam");
};

/** \relates AirTerminalSingleDuctConstantVolumeCooledBeam*/
typedef boost::optional<AirTerminalSingleDuctConstantVolumeCooledBeam> OptionalAirTerminalSingleDuctConstantVolumeCooledBeam;

/** \relates AirTerminalSingleDuctConstantVolumeCooledBeam*/
typedef std::vector<AirTerminalSingleDuctConstantVolumeCooledBeam> AirTerminalSingleDuctConstantVolumeCooledBeamVector;



} // model
} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTCONSTANTVOLUMECOOLEDBEAM_HPP

