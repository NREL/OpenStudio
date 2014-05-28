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

#ifndef MODEL_SCHEDULEBASE_HPP
#define MODEL_SCHEDULEBASE_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

class ScheduleTypeLimits;

namespace detail {

  class ScheduleBase_Impl;

} // detail

/** ScheduleBase is a ResourceObject that serves as a base class for Schedule and ScheduleDay,
 *  that is, objects with ScheduleTypeLimits. This class provides getters and setters for 
 *  ScheduleTypeLimits. Note that while users can explicitly set their own ScheduleTypeLimits, 
 *  we recommend using the \link ScheduleTypeRegistrySingleton ScheduleTypeRegistry\endlink and 
 *  related non-member functions, or letting user \link ModelObject ModelObjects\endlink set 
 *  this field, instead. */
class MODEL_API ScheduleBase : public ResourceObject {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~ScheduleBase() {}

  //@}
  /** @name Getters */
  //@{

  /** Returns the ScheduleTypeLimits of this object, if set. */
  boost::optional<ScheduleTypeLimits> scheduleTypeLimits() const;

  //@}
  /** @name Setters */
  //@{

  /** Returns true if scheduleTypeLimits is compatible with this object and is actually set. 
   *  The operation will fail if the Schedule or ScheduleDay is in use in a way that restricts
   *  the allowable ScheduleTypeLimits and the new value (scheduleTypeLimits) is incompatible 
   *  with that use. For instance, if a Schedule is used with a Lights object, then only 
   *  properly configured fractional ScheduleTypeLimits will succeed. ScheduleDay objects used
   *  in a ScheduleRuleset or a ScheduleRule are restricted to use ScheduleTypeLimits compatible 
   *  with those of their (ScheduleRuleset or ScheduleRule) parent. */
  bool setScheduleTypeLimits(const ScheduleTypeLimits& scheduleTypeLimits);

  /** Returns true if the scheduleTypeLimits() of this object is successfully cleared. Will fail
   *  if this object is being used by an object that expects it to have a ScheduleTypeLimits. */
  bool resetScheduleTypeLimits();

  // ensure that this object does not contain the date 2/29
  void ensureNoLeapDays();

  //@}
 protected:
  /// @cond
  typedef detail::ScheduleBase_Impl ImplType;

  explicit ScheduleBase(std::shared_ptr<detail::ScheduleBase_Impl> impl);

  ScheduleBase(IddObjectType type,const Model& model);

  friend class openstudio::IdfObject;
  friend class openstudio::detail::IdfObject_Impl;
  friend class detail::ScheduleBase_Impl;
  friend class Model;

  /// @endcond
 private:
  REGISTER_LOGGER("openstudio.model.ScheduleBase");
};

/** \relates ScheduleBase*/
typedef boost::optional<ScheduleBase> OptionalScheduleBase;

/** \relates ScheduleBase*/
typedef std::vector<ScheduleBase> ScheduleBaseVector;

} // model
} // openstudio

#endif // MODEL_SCHEDULEBASE_HPP

