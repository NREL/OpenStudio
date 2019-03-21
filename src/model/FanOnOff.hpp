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

#ifndef MODEL_FANONOFF_HPP
#define MODEL_FANONOFF_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class Curve;
class AirflowNetworkFan;

namespace detail {

  class FanOnOff_Impl;

} // detail

/** FanOnOff is a StraightComponent that wraps the OpenStudio IDD object 'OS:Fan:OnOff'. */
class MODEL_API FanOnOff : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit FanOnOff(const Model& model,
                          Schedule& availabilitySchedule,
                          Curve& fanPowerRatioFunctionofSpeedRatioCurve,
                          Curve& fanEfficiencyRatioFunctionofSpeedRatioCurve
                    );

  explicit FanOnOff(const Model& model, Schedule& availabilitySchedule);

  explicit FanOnOff(const Model& model);

  virtual ~FanOnOff() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

  double fanTotalEfficiency() const;
  bool isFanTotalEfficiencyDefaulted() const;

  /** Deprecrated in favor of fanTotalEfficiency **/
  double fanEfficiency() const;
  bool isFanEfficiencyDefaulted() const;

  double pressureRise() const;

  boost::optional<double> maximumFlowRate() const;

  bool isMaximumFlowRateAutosized() const;

  double motorEfficiency() const;

  bool isMotorEfficiencyDefaulted() const;

  boost::optional<double> motorInAirstreamFraction() const;

  std::string endUseSubcategory() const;

  bool isEndUseSubcategoryDefaulted() const;


  /** @name Setters */

  bool setAvailabilitySchedule(Schedule& schedule);


  bool setFanTotalEfficiency(double fanTotalEfficiency);
  void resetFanTotalEfficiency();

  bool setFanEfficiency(double fanTotalEfficiency);
  void resetFanEfficiency();

  bool setPressureRise(double pressureRise);

  bool setMaximumFlowRate(double maximumFlowRate);

  void resetMaximumFlowRate();

  void autosizeMaximumFlowRate();

  bool setMotorEfficiency(double motorEfficiency);

  void resetMotorEfficiency();

  bool setMotorInAirstreamFraction(double motorInAirstreamFraction);

  void resetMotorInAirstreamFraction();

  bool setEndUseSubcategory(std::string endUseSubcategory);

  void resetEndUseSubcategory();

  //@}

  /** @name Curves */

  Curve fanPowerRatioFunctionofSpeedRatioCurve() const;
  bool setFanPowerRatioFunctionofSpeedRatioCurve( const Curve& curve );

  Curve fanEfficiencyRatioFunctionofSpeedRatioCurve() const;
  bool setFanEfficiencyRatioFunctionofSpeedRatioCurve( const Curve& curve );

  //@{

  AirflowNetworkFan getAirflowNetworkFan();
  boost::optional<AirflowNetworkFan> airflowNetworkFan() const;

  boost::optional<double> autosizedMaximumFlowRate() const ;
  //@}
 protected:
  /// @cond
  typedef detail::FanOnOff_Impl ImplType;

  explicit FanOnOff(std::shared_ptr<detail::FanOnOff_Impl> impl);

  friend class detail::FanOnOff_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.FanOnOff");
};

/** \relates FanOnOff*/
typedef boost::optional<FanOnOff> OptionalFanOnOff;

/** \relates FanOnOff*/
typedef std::vector<FanOnOff> FanOnOffVector;

} // model
} // openstudio

#endif // MODEL_FANONOFF_HPP
