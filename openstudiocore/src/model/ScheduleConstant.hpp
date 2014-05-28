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

#ifndef MODEL_SCHEDULECONSTANT_HPP
#define MODEL_SCHEDULECONSTANT_HPP

#include "ModelAPI.hpp"
#include "Schedule.hpp"

namespace openstudio {
namespace model {

namespace detail {
  class ScheduleConstant_Impl;
} // detail

/** ScheduleConstant is a Schedule that wraps the OpenStudio IDD object 'OS:Schedule:Constant'. */
class MODEL_API ScheduleConstant : public Schedule {
 public:
  /** @name Constructors and Destructors */
  //@{

  /** Creates new ScheduleConstant in model and sets its value to 0.0. */
  explicit ScheduleConstant(const Model& model);

  virtual ~ScheduleConstant() {}

  //@}

  static IddObjectType iddObjectType();

  /** @name Getters */
  //@{

  double value() const;

  /** Returns the value of this ScheduleConstant as a Quantity if scheduleTypeLimits() is set. */
  boost::optional<Quantity> getValue(bool returnIP=false) const;

  //@}
  /** @name Setters */
  //@{

  void setValue(double value);

  /** Sets this ScheduleConstant's value if scheduleTypeLimits() is set and value can be properly 
   *  converted to the 'SI' units so implied. */
  bool setValue(const Quantity& value);

  //@}
 protected:

  typedef detail::ScheduleConstant_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  // constructor
  explicit ScheduleConstant(std::shared_ptr<detail::ScheduleConstant_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.ScheduleConstant");
};

/** \relates ScheduleConstant */
typedef boost::optional<ScheduleConstant> OptionalScheduleConstant;

/** \relates ScheduleConstant */
typedef std::vector<ScheduleConstant> ScheduleConstantVector;

} // model
} // openstudio

#endif // MODEL_SCHEDULECONSTANT_HPP

