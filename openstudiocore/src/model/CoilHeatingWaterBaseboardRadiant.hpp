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

#ifndef MODEL_COILHEATINGWATERBASEBOARDRADIANT_HPP
#define MODEL_COILHEATINGWATERBASEBOARDRADIANT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

namespace detail {

  class CoilHeatingWaterBaseboardRadiant_Impl;

} // detail

/** CoilHeatingWaterBaseboardRadiant is a StraightComponent that wraps the OpenStudio IDD object 'OS:Coil:Heating:Water:Baseboard:Radiant'. */
class MODEL_API CoilHeatingWaterBaseboardRadiant : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit CoilHeatingWaterBaseboardRadiant(const Model& model);

  virtual ~CoilHeatingWaterBaseboardRadiant() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> heatingDesignCapacityMethodValues();

  /** @name Getters */
  //@{

  double ratedAverageWaterTemperature() const;

  double ratedWaterMassFlowRate() const;

  std::string heatingDesignCapacityMethod() const;

  boost::optional<double> heatingDesignCapacity() const;

  bool isHeatingDesignCapacityAutosized() const;

  double heatingDesignCapacityPerFloorArea() const;

  double fractionofAutosizedHeatingDesignCapacity() const;

  boost::optional<double> maximumWaterFlowRate() const;

  bool isMaximumWaterFlowRateAutosized() const;

  double convergenceTolerance() const;

  //@}
  /** @name Setters */
  //@{

  bool setRatedAverageWaterTemperature(double ratedAverageWaterTemperature);

  bool setRatedWaterMassFlowRate(double ratedWaterMassFlowRate);

  bool setHeatingDesignCapacityMethod(std::string heatingDesignCapacityMethod);

  bool setHeatingDesignCapacity(double heatingDesignCapacity);

  void autosizeHeatingDesignCapacity();

  bool setHeatingDesignCapacityPerFloorArea(double heatingDesignCapacityPerFloorArea);

  bool setFractionofAutosizedHeatingDesignCapacity(double fractionofAutosizedHeatingDesignCapacity);

  bool setMaximumWaterFlowRate(double maximumWaterFlowRate);

  void autosizeMaximumWaterFlowRate();

  bool setConvergenceTolerance(double convergenceTolerance);

  //@}
  /** @name Other */
  //@{

  boost::optional<double> autosizedHeatingDesignCapacity() const ;

  boost::optional<double> autosizedMaximumWaterFlowRate() const ;



  //@}
 protected:
  /// @cond
  typedef detail::CoilHeatingWaterBaseboardRadiant_Impl ImplType;

  explicit CoilHeatingWaterBaseboardRadiant(std::shared_ptr<detail::CoilHeatingWaterBaseboardRadiant_Impl> impl);

  friend class detail::CoilHeatingWaterBaseboardRadiant_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.CoilHeatingWaterBaseboardRadiant");
};

/** \relates CoilHeatingWaterBaseboardRadiant*/
typedef boost::optional<CoilHeatingWaterBaseboardRadiant> OptionalCoilHeatingWaterBaseboardRadiant;

/** \relates CoilHeatingWaterBaseboardRadiant*/
typedef std::vector<CoilHeatingWaterBaseboardRadiant> CoilHeatingWaterBaseboardRadiantVector;

} // model
} // openstudio

#endif // MODEL_COILHEATINGWATERBASEBOARDRADIANT_HPP
