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

#ifndef MODEL_LOADPROFILEPLANT_HPP
#define MODEL_LOADPROFILEPLANT_HPP

#include "ModelAPI.hpp"
#include "StraightComponent.hpp"

namespace openstudio {

namespace model {

class Schedule;

namespace detail {

  class LoadProfilePlant_Impl;

} // detail

/** LoadProfilePlant is a StraightComponent that wraps the OpenStudio IDD object 'OS:LoadProfile:Plant'. */
class MODEL_API LoadProfilePlant : public StraightComponent {
 public:
  /** @name Constructors and Destructors */
  //@{

  explicit LoadProfilePlant(const Model& model);

  virtual ~LoadProfilePlant() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  Schedule loadSchedule() const;

  double peakFlowRate() const;

  Schedule flowRateFractionSchedule() const;

  //@}
  /** @name Setters */
  //@{

  bool setLoadSchedule(Schedule& schedule);

  void setPeakFlowRate(double peakFlowRate);

  bool setFlowRateFractionSchedule(Schedule& schedule);

  //@}
  /** @name Other */
  //@{

  //@}
 protected:
  /// @cond
  typedef detail::LoadProfilePlant_Impl ImplType;

  explicit LoadProfilePlant(std::shared_ptr<detail::LoadProfilePlant_Impl> impl);

  friend class detail::LoadProfilePlant_Impl;
  friend class Model;
  friend class IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.LoadProfilePlant");
};

/** \relates LoadProfilePlant*/
typedef boost::optional<LoadProfilePlant> OptionalLoadProfilePlant;

/** \relates LoadProfilePlant*/
typedef std::vector<LoadProfilePlant> LoadProfilePlantVector;

} // model
} // openstudio

#endif // MODEL_LOADPROFILEPLANT_HPP

