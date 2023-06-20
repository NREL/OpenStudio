/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_FLOORSPACE_FSVISITOR_HPP
#define UTILITIES_FLOORSPACE_FSVISITOR_HPP

#include "../UtilitiesAPI.hpp"

namespace openstudio {

// forward declaration
class FSBuildingUnit;
class FSBuildingType;
class FSConfig;
class FSConstructionSet;
class FSDaylightingControl;
class FSDaylightingControlDefinition;
class FSDoor;
class FSDoorDefinition;
class FSGround;
class FSModel;
class FSProject;
class FSShading;
class FSSpace;
class FSSpaceType;
class FSStory;
class FSThermalZone;
class FSWindow;
class FSWindowDefinition;

class UTILITIES_API FSVisitor
{
 public:
  virtual void Dispatch(const FSBuildingUnit& entity) = 0;
  virtual void Dispatch(const FSBuildingType& entity) = 0;
  virtual void Dispatch(const FSConfig& entity) = 0;
  virtual void Dispatch(const FSConstructionSet& entity) = 0;
  virtual void Dispatch(const FSDaylightingControl& entity) = 0;
  virtual void Dispatch(const FSDaylightingControlDefinition& entity) = 0;
  virtual void Dispatch(const FSDoor& entity) = 0;
  virtual void Dispatch(const FSDoorDefinition& entity) = 0;
  virtual void Dispatch(const FSGround& entity) = 0;
  virtual void Dispatch(const FSModel& entity) = 0;
  virtual void Dispatch(const FSProject& entity) = 0;
  virtual void Dispatch(const FSShading& entity) = 0;
  virtual void Dispatch(const FSSpace& entity) = 0;
  virtual void Dispatch(const FSSpaceType& entity) = 0;
  virtual void Dispatch(const FSStory& entity) = 0;
  virtual void Dispatch(const FSThermalZone& entity) = 0;
  virtual void Dispatch(const FSWindow& entity) = 0;
  virtual void Dispatch(const FSWindowDefinition& entity) = 0;

  virtual ~FSVisitor() = default;
};

// All visitable classes must implement the Accept method
class UTILITIES_API FSDispatchable
{
 public:
  virtual void Accept(FSVisitor& visitor) const = 0;
  virtual ~FSDispatchable() = default;
};

}  // namespace openstudio
#endif
