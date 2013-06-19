/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#ifndef STANDARDSINTERFACE_AGNOSTICINTERFACE_HPP
#define STANDARDSINTERFACE_AGNOSTICINTERFACE_HPP

#include <standardsinterface/Result.hpp>

#include <vector>
#include <string>

namespace openstudio {
namespace standardsinterface {

/** The AgnosticInterface is an abstract template class that defines simulation engine-agnostic 
 *  communications between a single building model and standards rules written using a 
 *  DataDictionary. AgnosticInterface assumes that the building model consists of a number of 
 *  objects endowed with attributes of four basic types (boolean, integer, double, and 
 *  enum/string), and provides a unified interface for:
 *
 *  \li Getting objects by ObjectType and (optional) relationship.
 *  \li Querying and setting attributes using DataDictionary terms.
 *
 *  AgnosticInterface has one template argument:
 *
 *  \li ModelObjectBaseClass - Base class used to represent individual model objects.
 */
template<class ModelObjectBaseClass>
class AgnosticInterface {
 public:
  /** @name Constructors and Destructors */
  //@{

  virtual ~AgnosticInterface() {}

  //@}
  /** @name Getters */
  //@{

  /** Return the name of the wrapped model. */
  virtual std::string name() const=0;

  /** Get all objects of type ddObjectType in the model. */
  virtual Result<typename std::vector<ModelObjectBaseClass> > getObjects(
      const std::string& ddObjectType) const=0;

  /** Get objects of type ddObjectTypeTargetOrChild that are related to ddObjectTypeSourceOrParent 
   *  sopObject.
   *
   *  \code 
   *  Result<std::vector<ModelObjectBaseClass> > returnedSchedules;
   *  returnedSchedules = getObject("zone",myZone,"lighting schedule");
   *  \endcode */
  virtual Result<typename std::vector<ModelObjectBaseClass> > getObjects(
      const std::string& ddObjectTypeSourceOrParent,
      const ModelObjectBaseClass& sopObject,
      const std::string& ddObjectTypeTargetOrChild) const=0;

  /** Get the value of object's ddBooleanAttribute. ddBooleanAttribute must be registered as a boolean
   *  attribute for objects of type ddObjectType. */
  virtual Result<bool> booleanAttribute(const std::string& ddObjectType,
                                        const ModelObjectBaseClass& object,
                                        const std::string& ddBooleanAttribute) const=0;

  /** Get the value of object's ddIntegerAttribute. ddIntegerAttribute must be registered as an integer
   *  attribute for objects of type ddObjectType. */
  virtual Result<int> integerAttribute(const std::string& ddObjectType,
                                       const ModelObjectBaseClass& object,
                                       const std::string& ddIntegerAttribute) const=0;

  /** Get the value of object's ddDoubleAttribute. ddDoubleAttribute must be registered as a double
   *  attribute for objects of type ddObjectType. */
  virtual Result<double> doubleAttribute(const std::string& ddObjectType,
                                         const ModelObjectBaseClass& object,
                                         const std::string& ddDoubleAttribute) const=0;


  /** Get the value of object's ddEnumAttribute. ddEnumAttribute must be registered as an enum attribute
   *  for objects of type ddObjectType. */
  virtual Result<std::string> enumAttribute(const std::string& ddObjectType,
                                            const ModelObjectBaseClass& object,
                                            const std::string& ddEnumAttribute) const=0;

  //@}
  /** @name Setters */
  //@{

  /** Set the name of the wrapped model, if possible. */
  virtual Result<result::none> setName(const std::string newName)=0;

  /** Set sopObject to use tocObject as its object of type ddObjectTypeTargetOrChild, if that is a 
   *  reasonable request.
   *
   *  \code 
   *  Result<result::none> success = setObject("zone",myZone,"lighting schedule",myLightingSchedule);
   *  \endcode */
  virtual Result<result::none> setObject(const std::string& ddObjectTypeSourceOrParent, 
                                         ModelObjectBaseClass& sopObject,
                                         const std::string& ddObjectTypeTargetOrChild,
                                         ModelObjectBaseClass& tocObject)=0;

  /** Set object's ddBooleanAttribute to value, if possible. Return value evaluates to false if the 
   *  action cannot be completed for any reason. */
  virtual Result<result::none> setAttribute(const std::string& ddObjectType,
                                            ModelObjectBaseClass& object,
                                            const std::string& ddBooleanAttribute,
                                            bool value)=0;

  /** Set object's ddIntegerAttribute to value, if possible. Returns false if the action cannot be
   *  completed for any reason. */
  virtual Result<result::none> setAttribute(const std::string& ddObjectType,
                                            ModelObjectBaseClass& object,
                                            const std::string& ddIntegerAttribute,
                                            int value)=0;

  /** Set object's ddDoubleAttribute to value, if possible. Returns false if the action cannot be
   *  completed for any reason. */
  virtual Result<result::none> setAttribute(const std::string& ddObjectType,
                                            ModelObjectBaseClass& object,
                                            const std::string& ddDoubleAttribute,
                                            double value)=0;

  /** Set object's ddEnumAttribute to value, if possible. Returns false if the action cannot be
  / *  completed for any reason. */
  virtual Result<result::none> setAttribute(const std::string& ddObjectType,
                                            ModelObjectBaseClass& object,
                                            const std::string& ddEnumAttribute,
                                            std::string value)=0;

  /** Set object's ddEnumAttribute to value, if possible. Returns false if the action cannot be
  / *  completed for any reason. */
  virtual Result<result::none> setAttribute(const std::string& ddObjectType,
                                            ModelObjectBaseClass& object,
                                            const std::string& ddEnumAttribute,
                                            const char* value)=0;

  //@}
  /** @name Simulation */
  //@{

  /** Before simulating, ask interface to ensure that ddObjectType's ddAttribute will be available 
   *  after simulation. Task is trivially completed if simulation not necessary to get 
   *  ddAttribute. If ddAttribute can be requested at different frequencies, this function 
   *  assumes that an annual number is desired. */
  virtual Result<result::none> requestAttribute(const std::string& ddObjectType,
                                                const std::string& ddAttribute)=0;

  /** Simulate the wrapped model. Depending on implementation, request may take awhile to 
   *  complete. */
  virtual Result<result::none> simulate(const std::string& enginePath,
                                        const std::string& auxPath1=std::string())=0;

  //@}

  protected:
    AgnosticInterface() {}
};

} // standardsinterface
} // openstudio

#endif // STANDARDSINTERFACE_AGNOSTICINTERFACE_HPP
