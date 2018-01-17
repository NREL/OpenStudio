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

#ifndef MODEL_PLANTCOMPONENTTEMPERATURESOURCE_HPP
#define MODEL_PLANTCOMPONENTTEMPERATURESOURCE_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  class PlantComponentTemperatureSource_Impl;

} // detail

/** PlantComponentTemperatureSource is a StraightComponent that wraps the OpenStudio IDD object 'OS:PlantComponent:TemperatureSource'. */
class MODEL_API PlantComponentTemperatureSource : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit PlantComponentTemperatureSource(const Model& model);

  virtual ~PlantComponentTemperatureSource() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> temperatureSpecificationTypeValues();

  /** @name Getters */
  //@{

  boost::optional<double> designVolumeFlowRate() const;

  bool isDesignVolumeFlowRateAutosized() const;

  std::string temperatureSpecificationType() const;

  boost::optional<double> sourceTemperature() const;

  boost::optional<Schedule> sourceTemperatureSchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setDesignVolumeFlowRate(double designVolumeFlowRate);

  void autosizeDesignVolumeFlowRate();

  bool setTemperatureSpecificationType(std::string temperatureSpecificationType);

  bool setSourceTemperature(double sourceTemperature);

  void resetSourceTemperature();

  bool setSourceTemperatureSchedule(Schedule& schedule);

  void resetSourceTemperatureSchedule();

  //@}
  /** @name Other */
  //@{

  boost::optional<double> autosizedDesignVolumeFlowRate() const ;



  //@}
 protected:
  /// @cond
  typedef detail::PlantComponentTemperatureSource_Impl ImplType;

  explicit PlantComponentTemperatureSource(std::shared_ptr<detail::PlantComponentTemperatureSource_Impl> impl);

  friend class detail::PlantComponentTemperatureSource_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.PlantComponentTemperatureSource");
};

/** \relates PlantComponentTemperatureSource*/
typedef boost::optional<PlantComponentTemperatureSource> OptionalPlantComponentTemperatureSource;

/** \relates PlantComponentTemperatureSource*/
typedef std::vector<PlantComponentTemperatureSource> PlantComponentTemperatureSourceVector;

} // model
} // openstudio

#endif // MODEL_PLANTCOMPONENTTEMPERATURESOURCE_HPP
