/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

#ifndef MODEL_COILHEATINGGASMULTISTAGE_HPP
#define MODEL_COILHEATINGGASMULTISTAGE_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;
class CoilHeatingGasMultiStageStageData;
class Curve;

namespace detail {

  class CoilHeatingGasMultiStage_Impl;

} // detail

/** CoilHeatingGasMultiStage is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:Gas:MultiStage'.
 * Performance data for each stage is contained in a separate ModelObject called OS:Coil:Heating:Gas:MultiStage:StageData **/
class MODEL_API CoilHeatingGasMultiStage : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilHeatingGasMultiStage(const Model& model);

  virtual ~CoilHeatingGasMultiStage() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  boost::optional<Schedule> availabilitySchedule() const;

  boost::optional<Curve> partLoadFractionCorrelationCurve() const;

  boost::optional<double> parasiticGasLoad() const;

  //@}
  /** @name Setters */
  //@{

  bool setAvailabilitySchedule(Schedule& schedule);

  void resetAvailabilitySchedule();

  bool setPartLoadFractionCorrelationCurve(const Curve& curve);

  void resetPartLoadFractionCorrelationCurve();

  void setParasiticGasLoad(double parasiticGasLoad);

  void resetParasiticGasLoad();

  //@}
  /** @name Other */
  //@{

  std::vector<CoilHeatingGasMultiStageStageData> stages() const;

  void addStage(CoilHeatingGasMultiStageStageData& stage);

  //@}
 protected:
  /// @cond
  typedef detail::CoilHeatingGasMultiStage_Impl ImplType;

  explicit CoilHeatingGasMultiStage(std::shared_ptr<detail::CoilHeatingGasMultiStage_Impl> impl);

  friend class detail::CoilHeatingGasMultiStage_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilHeatingGasMultiStage");
};

/** \relates CoilHeatingGasMultiStage*/
typedef boost::optional<CoilHeatingGasMultiStage> OptionalCoilHeatingGasMultiStage;

/** \relates CoilHeatingGasMultiStage*/
typedef std::vector<CoilHeatingGasMultiStage> CoilHeatingGasMultiStageVector;

} // model
} // openstudio

#endif // MODEL_COILHEATINGGASMULTISTAGE_HPP

