/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_EXTERIORLIGHTS_HPP
#define MODEL_EXTERIORLIGHTS_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

class ExteriorLightsDefinition;
class Schedule;
class Facility;

namespace detail {

  class ExteriorLights_Impl;

} // detail

/** ExteriorLights is a ModelObject that wraps the OpenStudio IDD object
 *  'OS:Exterior:Lights'. ExteriorLights is a child of Facility and instances
 *  ExteriorLightsDefinition. */
class MODEL_API ExteriorLights : public ModelObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** This constructor sets schedule to always on, and provides the option to use the
   *  astronomical clock control option. */
  ExteriorLights(const ExteriorLightsDefinition& definition,
                 bool useControlOptionAstronomicalClock = false);

  /** This constructor requires a user-specified schedule. */
  ExteriorLights(const ExteriorLightsDefinition& definition,
                 Schedule& schedule);

  virtual ~ExteriorLights() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> controlOptionValues();

  /** \deprecated */
  static std::vector<std::string> validControlOptionValues();

  /** @name Getters */
  //@{

  ExteriorLightsDefinition exteriorLightsDefinition() const;

  boost::optional<Schedule> schedule() const;

  std::string controlOption() const;

  bool isControlOptionDefaulted() const;

  double multiplier() const;

  bool isMultiplierDefaulted() const;

  std::string endUseSubcategory() const;

  bool isEndUseSubcategoryDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setExteriorLightsDefinition(const ExteriorLightsDefinition& exteriorLightsDefinition);

  bool setSchedule(Schedule& schedule);

  void resetSchedule();

  bool setControlOption(std::string controlOption);

  void resetControlOption();

  bool setMultiplier(double multiplier);

  void resetMultiplier();

  bool setEndUseSubcategory(std::string endUseSubcategory);

  void resetEndUseSubcategory();

  //@}
  /** @name Other */
  //@{

  Facility facility() const;

  //@}
 protected:
  /// @cond
  typedef detail::ExteriorLights_Impl ImplType;

  explicit ExteriorLights(std::shared_ptr<detail::ExteriorLights_Impl> impl);

  friend class detail::ExteriorLights_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ExteriorLights");
};

/** \relates ExteriorLights*/
typedef boost::optional<ExteriorLights> OptionalExteriorLights;

/** \relates ExteriorLights*/
typedef std::vector<ExteriorLights> ExteriorLightsVector;

} // model
} // openstudio

#endif // MODEL_EXTERIORLIGHTS_HPP

