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

#ifndef MODEL_CONTROLLERMECHANICALVENTILATION_HPP
#define MODEL_CONTROLLERMECHANICALVENTILATION_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {

class Schedule;
class ControllerOutdoorAir;

namespace detail {

  class ControllerMechanicalVentilation_Impl;

} // detail

/** ControllerMechanicalVentilation is a ModelObject that wraps the OpenStudio IDD object 'OS:Controller:MechanicalVentilation'. */
class MODEL_API ControllerMechanicalVentilation : public ModelObject {
 public:

  explicit ControllerMechanicalVentilation(const Model& model);

  virtual ~ControllerMechanicalVentilation() {}

  static IddObjectType iddObjectType();

  static std::vector<std::string> systemOutdoorAirMethodValues();

  Schedule availabilitySchedule() const;

  bool setAvailabilitySchedule(Schedule& schedule);

  bool demandControlledVentilation() const;

  bool isDemandControlledVentilationDefaulted() const;

  bool setDemandControlledVentilation(bool demandControlledVentilation);

  void resetDemandControlledVentilation();

  std::string systemOutdoorAirMethod() const;

  bool isSystemOutdoorAirMethodDefaulted() const;

  bool setSystemOutdoorAirMethod(std::string systemOutdoorAirMethod);

  void resetSystemOutdoorAirMethod();

  ControllerOutdoorAir controllerOutdoorAir() const;

 protected:
  /// @cond
  typedef detail::ControllerMechanicalVentilation_Impl ImplType;

  explicit ControllerMechanicalVentilation(std::shared_ptr<detail::ControllerMechanicalVentilation_Impl> impl);

  friend class detail::ControllerMechanicalVentilation_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ControllerMechanicalVentilation");
};

/** \relates ControllerMechanicalVentilation*/
typedef boost::optional<ControllerMechanicalVentilation> OptionalControllerMechanicalVentilation;

/** \relates ControllerMechanicalVentilation*/
typedef std::vector<ControllerMechanicalVentilation> ControllerMechanicalVentilationVector;

} // model
} // openstudio

#endif // MODEL_CONTROLLERMECHANICALVENTILATION_HPP

