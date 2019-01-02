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

#ifndef MODEL_DESIGNSPECIFICATIONOUTDOORAIR_HPP
#define MODEL_DESIGNSPECIFICATIONOUTDOORAIR_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

class Schedule;

namespace detail {

class DesignSpecificationOutdoorAir_Impl;

} // detail

/** DesignSpecificationOutdoorAir is a ModelObject that wraps the OpenStudio IDD
 *  object 'OS:DesignSpecification:OutdoorAir'. */
class MODEL_API DesignSpecificationOutdoorAir : public ResourceObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit DesignSpecificationOutdoorAir(const Model& model);

  virtual ~DesignSpecificationOutdoorAir() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> outdoorAirMethodValues();

  /** \deprecated */
  static std::vector<std::string> validOutdoorAirMethodValues();

  /** @name Getters */
  //@{

  std::string outdoorAirMethod() const;

  bool isOutdoorAirMethodDefaulted() const;

  double outdoorAirFlowperPerson() const;

  Quantity getOutdoorAirFlowperPerson(bool returnIP=false) const;

  bool isOutdoorAirFlowperPersonDefaulted() const;

  double outdoorAirFlowperFloorArea() const;

  Quantity getOutdoorAirFlowperFloorArea(bool returnIP=false) const;

  bool isOutdoorAirFlowperFloorAreaDefaulted() const;

  double outdoorAirFlowRate() const;

  Quantity getOutdoorAirFlowRate(bool returnIP=false) const;

  bool isOutdoorAirFlowRateDefaulted() const;

  double outdoorAirFlowAirChangesperHour() const;

  Quantity getOutdoorAirFlowAirChangesperHour(bool returnIP=false) const;

  bool isOutdoorAirFlowAirChangesperHourDefaulted() const;

  /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Outdoor Air Schedule Name" **/
  boost::optional<Schedule> outdoorAirFlowRateFractionSchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setOutdoorAirMethod(std::string outdoorAirMethod);

  void resetOutdoorAirMethod();

  bool setOutdoorAirFlowperPerson(double outdoorAirFlowperPerson);

  bool setOutdoorAirFlowperPerson(const Quantity& outdoorAirFlowperPerson);

  void resetOutdoorAirFlowperPerson();

  bool setOutdoorAirFlowperFloorArea(double outdoorAirFlowperFloorArea);

  bool setOutdoorAirFlowperFloorArea(const Quantity& outdoorAirFlowperFloorArea);

  void resetOutdoorAirFlowperFloorArea();

  bool setOutdoorAirFlowRate(double outdoorAirFlowRate);

  bool setOutdoorAirFlowRate(const Quantity& outdoorAirFlowRate);

  void resetOutdoorAirFlowRate();

  bool setOutdoorAirFlowAirChangesperHour(double outdoorAirFlowAirChangesperHour);

  bool setOutdoorAirFlowAirChangesperHour(const Quantity& outdoorAirFlowAirChangesperHour);

  void resetOutdoorAirFlowAirChangesperHour();

  /** In EnergyPlus 8.7.0 and above this field maps to the EnergyPlus field named "Outdoor Air Schedule Name" **/
  bool setOutdoorAirFlowRateFractionSchedule(Schedule& schedule);

  void resetOutdoorAirFlowRateFractionSchedule();

  //@}
 protected:
  /// @cond
  typedef detail::DesignSpecificationOutdoorAir_Impl ImplType;

  explicit DesignSpecificationOutdoorAir(std::shared_ptr<detail::DesignSpecificationOutdoorAir_Impl> impl);

  friend class detail::DesignSpecificationOutdoorAir_Impl;

  friend class Model;

  friend class IdfObject;

  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.DesignSpecificationOutdoorAir");
};

/** \relates DesignSpecificationOutdoorAir*/
typedef boost::optional<DesignSpecificationOutdoorAir> OptionalDesignSpecificationOutdoorAir;

/** \relates DesignSpecificationOutdoorAir*/
typedef std::vector<DesignSpecificationOutdoorAir> DesignSpecificationOutdoorAirVector;

} // model
} // openstudio

#endif // MODEL_DESIGNSPECIFICATIONOUTDOORAIR_HPP

