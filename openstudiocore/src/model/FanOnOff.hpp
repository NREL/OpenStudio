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

#ifndef MODEL_FANONOFF_HPP
#define MODEL_FANONOFF_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class Curve;

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

  virtual ~FanOnOff() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  Schedule availabilitySchedule() const;

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

  bool setFanEfficiency(double fanEfficiency);

  void resetFanEfficiency();

  void setPressureRise(double pressureRise);

  bool setMaximumFlowRate(double maximumFlowRate);

  void resetMaximumFlowRate();

  void autosizeMaximumFlowRate();

  bool setMotorEfficiency(double motorEfficiency);

  void resetMotorEfficiency();

  bool setMotorInAirstreamFraction(double motorInAirstreamFraction);

  void resetMotorInAirstreamFraction();

  void setEndUseSubcategory(std::string endUseSubcategory);

  void resetEndUseSubcategory();

  //@}
  
  /** @name Curves */
  
  Curve fanPowerRatioFunctionofSpeedRatioCurve() const;
  bool setFanPowerRatioFunctionofSpeedRatioCurve( const Curve& curve );
  
  Curve fanEfficiencyRatioFunctionofSpeedRatioCurve() const;
  bool setFanEfficiencyRatioFunctionofSpeedRatioCurve( const Curve& curve );
  
  //@{

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

