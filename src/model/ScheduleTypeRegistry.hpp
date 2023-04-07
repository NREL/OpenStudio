/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef MODEL_SCHEDULETYPEREGISTRY_HPP
#define MODEL_SCHEDULETYPEREGISTRY_HPP

#include "ModelAPI.hpp"

#include "../utilities/core/Singleton.hpp"
#include "../utilities/core/Logger.hpp"

#include <map>
#include <string>

#include <boost/optional.hpp>

namespace openstudio {

namespace model {

  class Model;
  class Schedule;
  class ScheduleTypeLimits;

  /** Data structure for registering and viewing a specific type of schedule that can exist in a
 *  Model. */
  struct MODEL_API ScheduleType
  {
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
  };

  /** \relates ScheduleType */
  using ScheduleTypeVector = std::vector<ScheduleType>;

  /** Returns true if candidate is consistent with scheduleType.
 * When isStringent is true, we also check that if scheduleType does not have a lower/upper bound, then candidate must not have them either.
 * \relates ScheduleType */
  MODEL_API bool isCompatible(const ScheduleType& scheduleType, const ScheduleTypeLimits& candidate, bool isStringent = false);

  /** Singleton class that contains a registry of all types of schedules that can exist in a Model.
 *  Do not use directly, but rather, use the ScheduleTypeRegistry typedef (e.g.
 *  \code
 *  ScheduleType st = ScheduleTypeRegistry::instance().getScheduleType("Lights","Lighting")
 *  \endcode
 *  ). */
  class MODEL_API ScheduleTypeRegistrySingleton
  {
    friend class Singleton<ScheduleTypeRegistrySingleton>;

   public:
    /** Returns the names of classes that have \link ScheduleType ScheduleTypes \endlink registered
   *  with the ScheduleTypeRegistry. */
    std::vector<std::string> classNames() const;

    /** Returns all the \link ScheduleType ScheduleTypes \endlink registered for className. */
    std::vector<ScheduleType> getScheduleTypesByClassName(const std::string& className) const;

    /** Returns a particular ScheduleType, based on the className and scheduleDisplayName keys. Throws
   *  if there is no such ScheduleType registered. */
    ScheduleType getScheduleType(const std::string& className, const std::string& scheduleDisplayName) const;

    /** Finds and returns a compatible ScheduleTypeLimits already in model, or creates and returns a
   *  new one. If the scheduleType is not fully specified, that is, if at least one of
   *  lowerLimitValue and upperLimitValue are not fixed, a new ScheduleTypeLimits is always
   *  created so the user can change those limits at will. */
    ScheduleTypeLimits getOrCreateScheduleTypeLimits(const ScheduleType& scheduleType, Model& model) const;

    std::string getDefaultName(const ScheduleType& scheduleType) const;

   private:
    REGISTER_LOGGER("openstudio.model.ScheduleTypeRegistry");
    ScheduleTypeRegistrySingleton();

    using ClassNameToScheduleTypesMap = std::map<std::string, std::vector<ScheduleType>>;
    ClassNameToScheduleTypesMap m_classNameToScheduleTypesMap;
  };

  /** \relates ScheduleTypeRegistrySingleton */
  using ScheduleTypeRegistry = openstudio::Singleton<ScheduleTypeRegistrySingleton>;

  /** Returns true if candidate is consistent with the ScheduleType that corresponds to className
 *  and scheduleRelationshipName. Throws if there is no such ScheduleType.
 *  \relates ScheduleTypeRegistrySingleton */
  MODEL_API bool isCompatible(const std::string& className, const std::string& scheduleDisplayName, const ScheduleTypeLimits& candidate);

  /** If schedule.scheduleTypeLimtis(), returns true if that ScheduleTypeLimits isCompatible and
 *  otherwise returns false. Otherwise, uses
 *  ScheduleTypeRegistrySingleton::getOrCreateScheduleTypeLimits to find an appropriate
 *  ScheduleTypeLimits and then calls Schedule::setScheduleTypeLimtis, which should succeed in
 *  this case. This method is used by all of the ModelObject methods that set schedules.
 *  \relates ScheduleTypeRegistrySingleton */
  MODEL_API bool checkOrAssignScheduleTypeLimits(const std::string& className, const std::string& scheduleDisplayName, Schedule& schedule);

  /** Returns all of the ScheduleTypeLimtis already in model that are \link isCompatible
 *  compatible\endlink with className and scheduleDisplayName. May be used instead of
 *  ScheduleTypeRegistrySingleton::getOrCreateScheduleTypeLimits to reduce the number of
 *  ScheduleTypeLimits that are ultimately created and present in a model.
 *  \relates ScheduleTypeRegistrySingleton */
  MODEL_API std::vector<ScheduleTypeLimits> getCompatibleScheduleTypeLimits(const Model& model, const std::string& className,
                                                                            const std::string& scheduleDisplayName);

  MODEL_API std::vector<Schedule> getCompatibleSchedules(const Model& model, const std::string& className, const std::string& scheduleDisplayName);

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SCHEDULETYPEREGISTRY_HPP
