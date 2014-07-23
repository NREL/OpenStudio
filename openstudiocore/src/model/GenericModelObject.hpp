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

#ifndef MODEL_GENERICMODELOBJECT_HPP
#define MODEL_GENERICMODELOBJECT_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"

namespace openstudio {
namespace model {
namespace detail {
  class GenericModelObject_Impl;
} // detail

/** GenericModelObject is a generic interface for \link ModelObject ModelObjects\endlink 
 *  that do not yet have a customized interface. */

/** GenericModelObject derives from ModelObject and is a generic interface to any OpenStudio IDD object..
 *
 *  GenericModelObject is different from all other \link ModelObject ModelObjects \endlink in that
 *  it does not wrap a specific OpenStudio IDD object.  Instead it is a generic wrapper that can hold
 *  OpenStudio IDD objects of any type.  When objects are added to Model, a large switch statement is
 *  used to construct wrappers of the correct type for each new object.  If the type of a newly added
 *  object has not yet been wrapped by a specific ModelObject then a GenericModelObject is used to hold 
 *  a reference to the data.  GenericModelObject is planned to be deprecated once all OpenStudio IDD types
 *  are wrapped by specific \link ModelObject ModelObjects \endlink, this class only exists in the interim
 *  so translation to and from EnergyPlus via the implicit translator does not lose data objects.
 *   
 */
class MODEL_API GenericModelObject : public ModelObject { 
 public:
  virtual ~GenericModelObject() {}

 protected:

  /// @cond 

  typedef detail::GenericModelObject_Impl ImplType;

  friend class Model;
  friend class openstudio::IdfObject;

  // constructor
  explicit GenericModelObject(std::shared_ptr<detail::GenericModelObject_Impl> impl);

 private:

  REGISTER_LOGGER("openstudio.model.GenericModelObject");

  /// @endcond 

};

// optional GenericModelObject
typedef boost::optional<GenericModelObject> OptionalGenericModelObject;

// vector of GenericModelObject
typedef std::vector<GenericModelObject> GenericModelObjectVector;

} // model
} // openstudio

#endif //MODEL_GENERICMODELOBJECT_HPP
