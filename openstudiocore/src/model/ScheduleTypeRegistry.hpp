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

#ifndef MODEL_SCHEDULETYPEREGISTRY_HPP
#define MODEL_SCHEDULETYPEREGISTRY_HPP

#include "ModelAPI.hpp"

#include "../utilities/core/Singleton.hpp"
#include "../utilities/core/Logger.hpp"

#include <map>
#include <string>

#include <boost/optional.hpp>

namespace openstudio {

class OSOptionalQuantity;

namespace model {

class Model;
class Schedule;
class ScheduleTypeLimits;

/** Data structure for registering and viewing a specific type of schedule that can exist in a 
 *  Model. */
struct MODEL_API ScheduleType {
  /** The name of a class that can point to a schedule. */
  std::string className;
  /** The display name of that schedule (which is used as a key in ScheduleTypeRegistrySingleton, 
   *  and is also intented for use in UI displays). */
  std::string scheduleDisplayName;
  /** The name of the Relationship that points from the user (of type className) to the Schedule. */
  std::string scheduleRelationshipName;
  /** Set to true if the schedule values are continuous (doubles); false if they are discrete 
   *  (integers). Used to set or test the ScheduleTypeLimits field "Numeric Type". */
  bool isContinuous;
  /** Key into the ScheduleTypeLimits field "Unit Type". */
  std::string unitType;
  /** Minimum value for this schedule type (or unbounded if == boost::none). */
  boost::optional<double> lowerLimitValue;
  /** Maximum value for this schedule type (or unbounded if == boost::none). */
  boost::optional<double> upperLimitValue;

  /** Returns the lowerLimitValue as an OSOptionalQuantity by using static methods on 
   *  ScheduleTypeLimits. */
  OSOptionalQuantity getLowerLimitValue(bool returnIP=false) const;
  /** Returns the upperLimitValue as an OSOptionalQuantity by using static methods on 
   *  ScheduleTypeLimits. */  
  OSOptionalQuantity getUpperLimitValue(bool returnIP=false) const;
};

/** \relates ScheduleType */
typedef std::vector<ScheduleType> ScheduleTypeVector;

/** Returns true if candidate is consistent with scheduleType. \relates ScheduleType */
MODEL_API bool isCompatible(const ScheduleType& scheduleType,
                            const ScheduleTypeLimits& candidate);
                            

/** Singleton class that contains a registry of all types of schedules that can exist in a Model. 
 *  Do not use directly, but rather, use the ScheduleTypeRegistry typedef (e.g. 
 *  \code 
 *  ScheduleType st = ScheduleTypeRegistry::instance().getScheduleType("Lights","Lighting") 
 *  \endcode
 *  ). */
class MODEL_API ScheduleTypeRegistrySingleton {
  friend class Singleton<ScheduleTypeRegistrySingleton>;
 public:

  /** Returns the names of classes that have \link ScheduleType ScheduleTypes \endlink registered
   *  with the ScheduleTypeRegistry. */
  std::vector<std::string> classNames() const;

  /** Returns all the \link ScheduleType ScheduleTypes \endlink registered for className. */
  std::vector<ScheduleType> getScheduleTypesByClassName(const std::string& className) const;

  /** Returns a particular ScheduleType, based on the className and scheduleDisplayName keys. Throws 
   *  if there is no such ScheduleType registered. */
  ScheduleType getScheduleType(const std::string& className,
                               const std::string& scheduleDisplayName) const;

  /** Finds and returns a compatible ScheduleTypeLimits already in model, or creates and returns a
   *  new one. If the scheduleType is not fully specified, that is, if at least one of
   *  lowerLimitValue and upperLimitValue are not fixed, a new ScheduleTypeLimits is always
   *  created so the user can change those limits at will. */
  ScheduleTypeLimits getOrCreateScheduleTypeLimits(const ScheduleType& scheduleType,
                                                   Model& model) const;

 private:
  REGISTER_LOGGER("openstudio.model.ScheduleTypeRegistry");
  ScheduleTypeRegistrySingleton();

  typedef std::map<std::string,std::vector<ScheduleType> > ClassNameToScheduleTypesMap;
  ClassNameToScheduleTypesMap m_classNameToScheduleTypesMap;

  std::string getDefaultName(const ScheduleType& scheduleType) const;
};

/** \relates ScheduleTypeRegistrySingleton */
typedef openstudio::Singleton<ScheduleTypeRegistrySingleton> ScheduleTypeRegistry;

/** Returns true if candidate is consistent with the ScheduleType that corresponds to className
 *  and scheduleRelationshipName. Throws if there is no such ScheduleType. 
 *  \relates ScheduleTypeRegistrySingleton */
MODEL_API bool isCompatible(const std::string& className,
                            const std::string& scheduleDisplayName,
                            const ScheduleTypeLimits& candidate);

/** If schedule.scheduleTypeLimtis(), returns true if that ScheduleTypeLimits isCompatible and 
 *  otherwise returns false. Otherwise, uses 
 *  ScheduleTypeRegistrySingleton::getOrCreateScheduleTypeLimits to find an appropriate 
 *  ScheduleTypeLimits and then calls Schedule::setScheduleTypeLimtis, which should succeed in 
 *  this case. This method is used by all of the ModelObject methods that set schedules.  
 *  \relates ScheduleTypeRegistrySingleton */
MODEL_API bool checkOrAssignScheduleTypeLimits(const std::string& className,
                                               const std::string& scheduleDisplayName,
                                               Schedule& schedule);

/** Returns all of the ScheduleTypeLimtis already in model that are \link isCompatible 
 *  compatible\endlink with className and scheduleDisplayName. May be used instead of 
 *  ScheduleTypeRegistrySingleton::getOrCreateScheduleTypeLimits to reduce the number of 
 *  ScheduleTypeLimits that are ultimately created and present in a model.
 *  \relates ScheduleTypeRegistrySingleton */
MODEL_API std::vector<ScheduleTypeLimits> getCompatibleScheduleTypeLimits(
    const Model& model,
    const std::string& className,
    const std::string& scheduleDisplayName);

MODEL_API std::vector<Schedule> getCompatibleSchedules(
    const Model& model,
    const std::string& className,
    const std::string& scheduleDisplayName);

} // model
} // openstudio

#endif // MODEL_SCHEDULETYPEREGISTRY_HPP

