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

#ifndef UTILITIES_IDF_OBJECTPOINTER_HPP
#define UTILITIES_IDF_OBJECTPOINTER_HPP

#include "Handle.hpp"

namespace openstudio {

/** Data structure for specifying connections between IdfWorkspace and other object collections.
 *  Other object collections include other IdfWorkspaces, IdfFile, IdfObjectVector. */
template<class T, class U>
struct ObjectPointer {
  typedef T source_type;
  typedef U target_type;

  T        source;
  unsigned fieldIndex;
  U        target;

  ObjectPointer() {}
  ObjectPointer(const T& s,unsigned fi, const U& t) : source(s), fieldIndex(fi), target(t) {}
};

template<class T>
struct FieldIndexLess {
  bool operator()(const T& left, const T& right) const {
    return (left.fieldIndex < right.fieldIndex);
  }
};

template<class T>
bool fieldIndexEqualTo(const T& object, unsigned value) {
  return (object.fieldIndex == value);
}

/** ObjectPointer with source as index in IdfObjectVector, target as Handle. For Workspace, source
 *  will be outside of workspace, target will be within workspace. */
typedef ObjectPointer<unsigned,Handle>  UHPointer;
/** ObjectPointer with source as Handle, target as index in IdfObjectVector. For Workspace, source
 *  will be within workspace, target will be outside of workspace. */
typedef ObjectPointer<Handle,unsigned>  HUPointer;
/** ObjectPointer with source and target as Handles. For Workspace, both objects must be in a
 *  workspace. */
typedef ObjectPointer<Handle,Handle>    HHPointer;

typedef std::vector<UHPointer> UHPointerVector;
typedef std::vector<HUPointer> HUPointerVector;
typedef std::vector<HHPointer> HHPointerVector;

} // openstudio

#endif // UTILITIES_IDF_OBJECTPOINTER_HPP
