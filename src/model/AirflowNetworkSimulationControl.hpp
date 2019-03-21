/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_AIRFLOWNETWORKSIMULATIONCONTROL_HPP
#define MODEL_AIRFLOWNETWORKSIMULATIONCONTROL_HPP

#include "ModelAPI.hpp"
#include "ParentObject.hpp"

namespace openstudio {


namespace model {

namespace detail {
  class AirflowNetworkSimulationControl_Impl;
}

/** AirflowNetworkSimulationControl derives from ParentObject and is an interface to the OpenStudio IDD object named "AirflowNetwork:SimulationControl".
 *
 *  AirflowNetworkSimulationControl defines what types of multizone airflow calculations are part of an EnergyPlus simulation.
 *  AirflowNetworkSimulationControl parents several other ModelObject types that configure simulation parameters for EnergyPlus.
 *  AirflowNetworkSimulationControl does not have a public constructor because it is a unique ModelObject.  
 *  To get the AirflowNetworkSimulationControl object for a Model or create one if it does not yet exist use model.getUniqueObject<AirflowNetworkSimulationControl>().  
 *  To get the AirflowNetworkSimulationControl object for a Model but not create one if it does not yet exist use model.getOptionalUniqueObject<AirflowNetworkSimulationControl>().
 */
class MODEL_API AirflowNetworkSimulationControl : public ParentObject {
 public:
  virtual ~AirflowNetworkSimulationControl() {}


  //@}
  /** @name Static Methods */
  //@{
  static IddObjectType iddObjectType();

  //@}
  /** @name Getters */
  //@{

  /** Determines the type of AirflowNetwork simulation that will be run.
   *
   *  NoMultizoneOrDistribution: Only perform Simple calculations (objects ZoneInfiltration:*,
   *  ZoneVentilation:*, ZoneMixing, ZoneCrossMixing, ZoneRefrigerationDoorMixing, 
   *  ZoneAirBalance:OutdoorAir, ZoneEarthtube, ZoneThermalChimney, and ZoneCoolTower:Shower);
   *
   *  MultizoneWithoutDistribution: Use AirflowNetwork objects to simulate multizone
   *  Airflows driven by wind during simulation time, and objects of ZoneInfiltration:*,
   *  ZoneVentilation:*, ZoneMixing, ZoneCrossMixing, ZoneRefrigerationDoorMixing, ZoneAirBalance:OutdoorAir, 
   *  ZoneEarthtube, ZoneThermalChimney, and ZoneCoolTower:Shower are ignored;
   *
   *  MultizoneWithDistributionOnlyDuringFanOperation: Perform distribution system
   *  calculations during system fan on time and Simple calculations during system Fan off time;
   *
   *  MultizoneWithDistribution: Perform distribution system calculations during system
   *  fan on time and multizone Airflow driven by wind during system fan off time.
   */
  boost::optional<std::string> airflowNetworkControl() const;
  /** Returns true if the type of AirflowNetwork simulation is defaulted. */
  bool isAirflowNetworkControlDefaulted() const;

  /* Determines the wind pressure treatment used by AirflowNetork.
   *
   *  Input: User must enter AirflowNetwork:MultiZone:WindPressureCoefficientArray,
   *  AirflowNetwork:MultiZone:ExternalNode, and AirflowNetwork:MultiZone:WindPressureCoefficientValues objects.
   *
   *  SurfaceAverageCalculation: used only for rectangular buildings. AirflowNetwork:MultiZone:WindPressureCoefficientArray, 
   *  AirflowNetwork:MultiZone:ExternalNode, and AirflowNetwork:MultiZone:WindPressureCoefficientValues objects are not used.
   */
  boost::optional<std::string> windPressureCoefficientType() const;
  /** Returns true if the wind pressure treatment is defaulted. */
  bool isWindPressureCoefficientTypeDefaulted() const;

  /* Determines how height is selected for wind pressure.
   *  If ExternalNode is selected, the height given in the AirflowNetwork:MultiZone:ExternalNode object will be used.
   *
   *  If OpeningHeight is selected, the surface opening height (centroid) will be used to calculate local wind pressure
   *
   *  This field is ignored when the choice of the Wind Pressure Coefficient Type field is SurfaceAverageCalculation.
   */
  boost::optional<std::string> heightSelectionforLocalWindPressureCalculation() const;
  /** Returns true if the height selection procedure is defaulted. */
  bool isHeightSelectionforLocalWindPressureCalculationDefaulted() const;

  /** General building type, used only if Wind Pressure Coefficient Type = SurfaceAverageCalculation.
   */
  boost::optional<std::string> buildingType() const;
  /** Returns true if the building type is defaulted. */
  bool isBuildingTypeDefaulted() const;

  /** Determines the maximum number of iterations used to converge on a solution. If this limit
   *  is exceeded, the program terminates.
   */
  boost::optional<int> maximumNumberofIterations() const;
  /** Returns true if the maximum number of iterations is defaulted. */
  bool isMaximumNumberofIterationsDefaulted() const;

  /** Determines how the network equations are initialized.
   */
  boost::optional<std::string> initializationType() const;
  /** Returns true if the initialization type is defaulted. */
  bool isInitializationTypeDefaulted() const;

  /** This tolerance is defined as the absolute value of the sum of the mass flow rates divided by the sum of the absolute value 
   *  of the mass Flow Rates. The mass flow rates described here refer to the mass Flow Rates at all Nodes in the AirflowNetwork 
   *  model. The solution converges when both this tolerance and the tolerance in the Absolute Airflow Convergence Tolerance
   *  field are satisfied.
   */
  boost::optional<double> relativeAirflowConvergenceTolerance() const;
  /** Returns true if the relative convergence tolerance is defaulted. */
  bool isRelativeAirflowConvergenceToleranceDefaulted() const;

  /** This tolerance is defined as the absolute value of the sum of the mass flow rates. The mass flow rates described here refer
   *  to the mass flow rates at all nodes in the AirflowNetwork model. The solution converges when both this tolerance and the 
   *  tolerance in the previous field (Relative Airflow Convergence Tolerance) are satisfied.
   */
  boost::optional<double> absoluteAirflowConvergenceTolerance() const;
  /** Returns true if the absolute convergence tolerance is defaulted. */
  bool isAbsoluteAirflowConvergenceToleranceDefaulted() const;

  /** Used only for AirflowNetwork:SimulationControl
   */
  boost::optional<double> convergenceAccelerationLimit() const;
  /** Returns true if the convergence acceleration limit is defaulted. */
  bool isConvergenceAccelerationLimitDefaulted() const;
  
  /** Azimuth angle of long axis of building in degrees clockwise from true North.
   *  Used only if Wind Pressure Coefficient Type = SurfaceAverageCalculation.
   */
  boost::optional<double> azimuthAngleofLongAxisofBuilding() const;
  /** Returns true if the azimuth angle of long axis is defaulted. */
  bool isAzimuthAngleofLongAxisofBuildingDefaulted() const;

  /** Ratio of building width along short axis to width along long axis.
   *  Used only if Wind Pressure Coefficient Type = SurfaceAverageCalculation.
   */
  boost::optional<double> buildingAspectRatio() const;
  /** Returns true if the ratio of the building width to length is defaulted. */
  bool isBuildingAspectRatioDefaulted() const;

  /** Returns true if the temperature of external nodes will be computed based on height.*/
  bool heightDependenceofExternalNodeTemperature() const;
  /** Returns true if the external nodes temperature computation type is defaulted. */
  bool isHeightDependenceofExternalNodeTemperatureDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  /** Sets the airflow network control type. */
  bool setAirflowNetworkControl(std::string airflowNetworkControl);
  /** Resets the airflow network control type. */
  void resetAirflowNetworkControl();

  /** Sets the wind pressure coefficient type. */
  bool setWindPressureCoefficientType(std::string type);
  /** Resets the wind pressure coefficient type. */
  void resetWindPressureCoefficientType();

  /** Sets the height selection procedure. */
  bool setHeightSelectionforLocalWindPressureCalculation(std::string height);
  /** Resets the height selection procedure. */
  void resetHeightSelectionforLocalWindPressureCalculation();

  /** Sets the building type. */
  bool setBuildingType(std::string type);
  /** Resets the building type. */
  void resetBuildingType();

  /** Sets the maximum number of iteration. */
  bool setMaximumNumberofIterations(int number);
  /** Resets the maximum number of iteration. */
  void resetMaximumNumberofIterations();

  /** Sets the initialization type. */
  bool setInitializationType(std::string type);
  /** Resets the initialization type. */
  void resetInitializationType();

  /** Sets the relative airflow convergence tolerance. */
  bool setRelativeAirflowConvergenceTolerance(double tolerance);
  /** Resets the relative airflow convergence tolerance. */
  void resetRelativeAirflowConvergenceTolerance();

  /** Sets the absolute airflow convergence tolerance. */
  bool setAbsoluteAirflowConvergenceTolerance(double tolerance);
  /** Resets the absolute airflow convergence tolerance. */
  void resetAbsoluteAirflowConvergenceTolerance();

  /** Sets the convergence acceleration limit. */
  bool setConvergenceAccelerationLimit(double limit);
  /** Resets the convergence acceleration limit. */
  void resetConvergenceAccelerationLimit();

  /** Sets the azimuthal angle of the building. */
  bool setAzimuthAngleofLongAxisofBuilding(double angle);
  /** Resets the azimuthal angle of the building. */
  void resetAzimuthAngleofLongAxisofBuilding();

  /** Sets the building aspect ratio. */
  bool setBuildingAspectRatio(double ratio);
  /** Resets the building aspect ratio. */
  void resetBuildingAspectRatio();

  /** Sets the external nodes temperature computation type. */
  bool setHeightDependenceofExternalNodeTemperature(bool tf);
  /** Resets the external nodes temperature computation type. */
  void resetHeightDependenceofExternalNodeTemperature();

  //@}

 protected:

  /** @name Constructors and Destructors */
  //@{

  /// Constructs a new SimulationControl object in the model.
  explicit AirflowNetworkSimulationControl(const Model& model);

  //@}

  /// @cond

  friend class detail::AirflowNetworkSimulationControl_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;

  typedef detail::AirflowNetworkSimulationControl_Impl ImplType;

  explicit AirflowNetworkSimulationControl(std::shared_ptr<ImplType> impl);

 private:

  REGISTER_LOGGER("openstudio.model.AirflowNetworkSimulationControl");

  /// @endcond
};

/** \relates AirflowNetworkSimulationControl */
typedef boost::optional<AirflowNetworkSimulationControl> OptionalAirflowNetworkSimulationControl;

}
}

#endif