/**********************************************************************
 *  Copyright (c) 2008-2015, Alliance for Sustainable Energy.
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

#ifndef MODEL_EVAPORATIVECOOLERINDIRECTRESEARCHSPECIAL_HPP
#define MODEL_EVAPORATIVECOOLERINDIRECTRESEARCHSPECIAL_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  class EvaporativeCoolerIndirectResearchSpecial_Impl;

} // detail

/** EvaporativeCoolerIndirectResearchSpecial is a StraightComponent that wraps the OpenStudio IDD object 'OS:EvaporativeCooler:Indirect:ResearchSpecial'. */
class MODEL_API EvaporativeCoolerIndirectResearchSpecial : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit EvaporativeCoolerIndirectResearchSpecial(const Model& model);

  virtual ~EvaporativeCoolerIndirectResearchSpecial() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  double coolerMaximumEffectiveness() const;

  double recirculatingWaterPumpPowerConsumption() const;

  boost::optional<double> secondaryFanFlowRate() const;

  bool isSecondaryFanFlowRateAutosized() const;

  double secondaryFanTotalEfficiency() const;

  double secondaryFanDeltaPressure() const;

  double dewpointEffectivenessFactor() const;

  boost::optional<double> driftLossFraction() const;

  boost::optional<double> blowdownConcentrationRatio() const;

  //@}
  /** @name Setters */
  //@{

  // TODO: Check argument type. From object lists, some candidates are: Schedule.
  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setCoolerMaximumEffectiveness(double coolerMaximumEffectiveness);

  void setRecirculatingWaterPumpPowerConsumption(double recirculatingWaterPumpPowerConsumption);

  bool setSecondaryFanFlowRate(double secondaryFanFlowRate);

  void resetSecondaryFanFlowRate();

  void autosizeSecondaryFanFlowRate();

  bool setSecondaryFanTotalEfficiency(double secondaryFanTotalEfficiency);

  bool setSecondaryFanDeltaPressure(double secondaryFanDeltaPressure);

  void setDewpointEffectivenessFactor(double dewpointEffectivenessFactor);

  bool setDriftLossFraction(double driftLossFraction);

  void resetDriftLossFraction();

  bool setBlowdownConcentrationRatio(double blowdownConcentrationRatio);

  void resetBlowdownConcentrationRatio();

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::EvaporativeCoolerIndirectResearchSpecial_Impl ImplType;

  explicit EvaporativeCoolerIndirectResearchSpecial(std::shared_ptr<detail::EvaporativeCoolerIndirectResearchSpecial_Impl> impl);

  friend class detail::EvaporativeCoolerIndirectResearchSpecial_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.EvaporativeCoolerIndirectResearchSpecial");
};

/** \relates EvaporativeCoolerIndirectResearchSpecial*/
typedef boost::optional<EvaporativeCoolerIndirectResearchSpecial> OptionalEvaporativeCoolerIndirectResearchSpecial;

/** \relates EvaporativeCoolerIndirectResearchSpecial*/
typedef std::vector<EvaporativeCoolerIndirectResearchSpecial> EvaporativeCoolerIndirectResearchSpecialVector;

} // model
} // openstudio

#endif // MODEL_EVAPORATIVECOOLERINDIRECTRESEARCHSPECIAL_HPP

