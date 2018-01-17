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

#ifndef MODEL_EXTERIORFUELEQUIPMENT_HPP
#define MODEL_EXTERIORFUELEQUIPMENT_HPP

#include "ModelAPI.hpp"
#include "ExteriorLoadInstance.hpp"

namespace openstudio {
namespace model {

class ExteriorFuelEquipmentDefinition;
class Schedule;
class Facility;

namespace detail {

  class ExteriorFuelEquipment_Impl;

} // detail

/** ExteriorFuelEquipment is a ExteriorLoadInstance that wraps the OpenStudio IDD object
 *  'OS:Exterior:FuelEquipment'. ExteriorFuelEquipment is a child of Facility and instances
 *  ExteriorFuelEquipmentDefinition. */
class MODEL_API ExteriorFuelEquipment : public ExteriorLoadInstance {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** This constructor sets schedule to always on, defaults to Fuel Type = Electricity */
  ExteriorFuelEquipment(const ExteriorFuelEquipmentDefinition& definition);

  /** This constructor requires a user-specified schedule. */
  ExteriorFuelEquipment(const ExteriorFuelEquipmentDefinition& definition,
                 Schedule& schedule);

  virtual ~ExteriorFuelEquipment() {}

  //@}

  static IddObjectType iddObjectType();

  static std::vector<std::string> fuelTypeValues();

  /** \deprecated */
  static std::vector<std::string> validFuelTypeValues();

  /** @name Getters */
  //@{

  ExteriorFuelEquipmentDefinition exteriorFuelEquipmentDefinition() const;

  Schedule schedule() const;

  std::string fuelType() const;

  double multiplier() const;

  bool isMultiplierDefaulted() const;

  std::string endUseSubcategory() const;

  bool isEndUseSubcategoryDefaulted() const;

  //@}
  /** @name Setters */
  //@{

  bool setExteriorFuelEquipmentDefinition(const ExteriorFuelEquipmentDefinition& exteriorFuelEquipmentDefinition);

  bool setSchedule(Schedule& schedule);

  void resetSchedule();

  bool setFuelType(std::string fuelType);

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
  typedef detail::ExteriorFuelEquipment_Impl ImplType;

  explicit ExteriorFuelEquipment(std::shared_ptr<detail::ExteriorFuelEquipment_Impl> impl);

  friend class detail::ExteriorFuelEquipment_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ExteriorFuelEquipment");
};

/** \relates ExteriorFuelEquipment*/
typedef boost::optional<ExteriorFuelEquipment> OptionalExteriorFuelEquipment;

/** \relates ExteriorFuelEquipment*/
typedef std::vector<ExteriorFuelEquipment> ExteriorFuelEquipmentVector;

} // model
} // openstudio

#endif // MODEL_EXTERIORFUELEQUIPMENT_HPP

