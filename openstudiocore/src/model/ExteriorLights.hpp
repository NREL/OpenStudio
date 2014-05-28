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

  Schedule schedule() const;

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

  bool setControlOption(std::string controlOption);

  void resetControlOption();

  bool setMultiplier(double multiplier);

  void resetMultiplier();

  void setEndUseSubcategory(std::string endUseSubcategory);

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

