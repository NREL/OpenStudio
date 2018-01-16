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

#ifndef MODEL_LIGHTINGDESIGNDAY_HPP
#define MODEL_LIGHTINGDESIGNDAY_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {

class DateTime;
class Date;
class Time;

namespace model {

namespace detail {

  class LightingDesignDay_Impl;

} // detail

/** LightingDesignDay is a ModelObject that wraps the OpenStudio IDD object 'OS_LightingDesignDay'. */
class MODEL_API LightingDesignDay : public ModelObject {
 public:

  /** @name Constructors and Destructors */
  //@{

  LightingDesignDay(const std::string& cieSkyModel,
                    const openstudio::Date& date,
                    const Model& model);

  virtual ~LightingDesignDay() {}

  //@}
  /** @name Static Methods */
  //@{

  static IddObjectType iddObjectType();

  static std::vector<std::string> validCIESkyModelValues();

  //@}
  /** @name Getters */
  //@{

  std::string cieSkyModel() const;

  int snowIndicator() const;

  bool isSnowIndicatorDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setCIESkyModel(std::string cIESkyModel);

  bool setSnowIndicator(int snowIndicator);

  void resetSnowIndicator();

  //@}

  openstudio::Date date() const;

  bool setDate(const openstudio::Date& date);

  std::vector<openstudio::Time> simulationTimes() const;

  std::vector<openstudio::DateTime> simulationDateTimes() const;

  bool addSimulationTime(const openstudio::Time& time);

  void clearSimulationTimes();

  // ensure that this object does not contain the date 2/29
  void ensureNoLeapDays();

 protected:
  /// @cond
  typedef detail::LightingDesignDay_Impl ImplType;

  friend class detail::LightingDesignDay_Impl;
  friend class Model;
  friend class IdfObject;

  explicit LightingDesignDay(std::shared_ptr<detail::LightingDesignDay_Impl> impl);

  /// @endcond
 private:

  REGISTER_LOGGER("openstudio.model.LightingDesignDay");
};

/** \relates LightingDesignDay*/
typedef boost::optional<LightingDesignDay> OptionalLightingDesignDay;

/** \relates LightingDesignDay*/
typedef std::vector<LightingDesignDay> LightingDesignDayVector;

} // model
} // openstudio

#endif // MODEL_LIGHTINGDESIGNDAY_HPP

