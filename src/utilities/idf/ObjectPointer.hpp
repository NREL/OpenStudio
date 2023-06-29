/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef UTILITIES_IDF_OBJECTPOINTER_HPP
#define UTILITIES_IDF_OBJECTPOINTER_HPP

#include "Handle.hpp"

namespace openstudio {

/** Data structure for specifying connections between IdfWorkspace and other object collections.
 *  Other object collections include other IdfWorkspaces, IdfFile, IdfObjectVector. */
template <class T, class U>
struct ObjectPointer
{
  using source_type = T;
  using target_type = U;

  T source;
  unsigned fieldIndex;
  U target;

  ObjectPointer() = default;
  ObjectPointer(const T& s, unsigned fi, const U& t) : source(s), fieldIndex(fi), target(t) {}
};

template <class T>
struct FieldIndexLess
{
  bool operator()(const T& left, const T& right) const {
    return (left.fieldIndex < right.fieldIndex);
  }
};

template <class T>
bool fieldIndexEqualTo(const T& object, unsigned value) {
  return (object.fieldIndex == value);
}

/** ObjectPointer with source as index in IdfObjectVector, target as Handle. For Workspace, source
 *  will be outside of workspace, target will be within workspace. */
using UHPointer = ObjectPointer<unsigned int, Handle>;
/** ObjectPointer with source as Handle, target as index in IdfObjectVector. For Workspace, source
 *  will be within workspace, target will be outside of workspace. */
using HUPointer = ObjectPointer<Handle, unsigned int>;
/** ObjectPointer with source and target as Handles. For Workspace, both objects must be in a
 *  workspace. */
using HHPointer = ObjectPointer<Handle, Handle>;

using UHPointerVector = std::vector<UHPointer>;
using HUPointerVector = std::vector<HUPointer>;
using HHPointerVector = std::vector<HHPointer>;

}  // namespace openstudio

#endif  // UTILITIES_IDF_OBJECTPOINTER_HPP
