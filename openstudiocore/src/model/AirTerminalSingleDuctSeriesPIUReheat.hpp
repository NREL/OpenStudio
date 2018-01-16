/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_AIRTERMINALSINGLEDUCTSERIESPIUREHEAT_HPP
#define MODEL_AIRTERMINALSINGLEDUCTSERIESPIUREHEAT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class HVACComponent;

namespace detail {

  class AirTerminalSingleDuctSeriesPIUReheat_Impl;

} // detail

/** AirTerminalSingleDuctSeriesPIUReheat is a StraightComponent that wraps the OpenStudio IDD object 'OS:AirTerminal:SingleDuct:SeriesPIU:Reheat'. */
class MODEL_API AirTerminalSingleDuctSeriesPIUReheat : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit AirTerminalSingleDuctSeriesPIUReheat(const Model& model,
                                                HVACComponent & fan,
                                                HVACComponent & reheatCoil);

  virtual ~AirTerminalSingleDuctSeriesPIUReheat() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  boost::optional<double> maximumAirFlowRate() const;

  bool isMaximumAirFlowRateAutosized() const;

  boost::optional<double> maximumPrimaryAirFlowRate() const;

  bool isMaximumPrimaryAirFlowRateAutosized() const;

  boost::optional<double> minimumPrimaryAirFlowFraction() const;

  bool isMinimumPrimaryAirFlowFractionAutosized() const;

  HVACComponent fan() const;

  HVACComponent reheatCoil() const;

  boost::optional<double> maximumHotWaterorSteamFlowRate() const;

  bool isMaximumHotWaterorSteamFlowRateAutosized() const;

  double minimumHotWaterorSteamFlowRate() const;

  double convergenceTolerance() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setMaximumAirFlowRate(double maximumAirFlowRate);

  void autosizeMaximumAirFlowRate();

  bool setMaximumPrimaryAirFlowRate(double maximumPrimaryAirFlowRate);

  void autosizeMaximumPrimaryAirFlowRate();

  bool setMinimumPrimaryAirFlowFraction(double minimumPrimaryAirFlowFraction);

  void autosizeMinimumPrimaryAirFlowFraction();

  bool setFan(const HVACComponent& fan);

  bool setReheatCoil(const HVACComponent& coil);

  bool setMaximumHotWaterorSteamFlowRate(double maximumHotWaterorSteamFlowRate);

  void resetMaximumHotWaterorSteamFlowRate();

  void autosizeMaximumHotWaterorSteamFlowRate();

  bool setMinimumHotWaterorSteamFlowRate(double minimumHotWaterorSteamFlowRate);

  bool setConvergenceTolerance(double convergenceTolerance);

  //@}
  /** @name Other */
  //@{

  unsigned secondaryAirInletPort() const;
  boost::optional<Node> secondaryAirInletNode() const;

  boost::optional<double> autosizedMaximumAirFlowRate() const ;

  boost::optional<double> autosizedMaximumPrimaryAirFlowRate() const ;

  boost::optional<double> autosizedMinimumPrimaryAirFlowFraction() const ;

  boost::optional<double> autosizedMaximumHotWaterorSteamFlowRate() const ;



  //@}
 protected:
  /// @cond
  typedef detail::AirTerminalSingleDuctSeriesPIUReheat_Impl ImplType;

  explicit AirTerminalSingleDuctSeriesPIUReheat(std::shared_ptr<detail::AirTerminalSingleDuctSeriesPIUReheat_Impl> impl);

  friend class detail::AirTerminalSingleDuctSeriesPIUReheat_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.AirTerminalSingleDuctSeriesPIUReheat");
};

/** \relates AirTerminalSingleDuctSeriesPIUReheat*/
typedef boost::optional<AirTerminalSingleDuctSeriesPIUReheat> OptionalAirTerminalSingleDuctSeriesPIUReheat;

/** \relates AirTerminalSingleDuctSeriesPIUReheat*/
typedef std::vector<AirTerminalSingleDuctSeriesPIUReheat> AirTerminalSingleDuctSeriesPIUReheatVector;

} // model
} // openstudio

#endif // MODEL_AIRTERMINALSINGLEDUCTSERIESPIUREHEAT_HPP
