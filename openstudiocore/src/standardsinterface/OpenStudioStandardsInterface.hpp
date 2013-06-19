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

#ifndef STANDARDSINTERFACE_OPENSTUDIOSTANDARDSINTERFACE_HPP
#define STANDARDSINTERFACE_OPENSTUDIOSTANDARDSINTERFACE_HPP

#include <standardsinterface/StandardsInterfaceAPI.hpp>
#include <standardsinterface/AgnosticInterface.hpp>
#include <standardsinterface/DataDictionary.hpp>

#include <model/Model.hpp>

#include <utilities/core/Logger.hpp>
#include <utilities/sql/SqlFile.hpp>

#include <boost/shared_ptr.hpp>
#include <boost/optional.hpp>

#include <map>

namespace openstudio {
namespace model {
  class ModelObject;
  class Building;
  class Space;
  class Schedule;
  class Meter;
}
namespace standardsinterface {

typedef Result< std::vector<model::ModelObject> > ModelObjectVectorResult;

/** The OpenStudioStandardsInterface implements AgnosticInterface<model::ModelObject>. An 
 *  OpenStudioStandardsInterface is constructed from a model::Model, and serves as a wrapper around 
 *  that model that allows it to be queried and manipulated using DataDictionary terms. Essentially,
 *  OpenStudioStandardsInterface maps \link ObjectType ObjectTypes \endlink to specific classes 
 *  derived from model::ModelObject, and maps DataDictionary attributes to Attributes available 
 *  through model::ModelObject::getAttribute. */
class STANDARDSINTERFACE_API OpenStudioStandardsInterface 
    : public AgnosticInterface<model::ModelObject> 
{
 public:
  /** @name Constructors and Destructors*/
  //@{

  /** Construct an instance of the interface from a model. */
  OpenStudioStandardsInterface(const model::Model& model=model::Model());

  /** Create a OpenStudioStandardsInterface wrapping a disconnected copy of model(). */
  OpenStudioStandardsInterface clone() const;

  /** Virtual destructor because OpenStudioStandardsInterface is a derived class. */
  virtual ~OpenStudioStandardsInterface() {}

  //@}
  /** @name Getters */
  //@{

  /** Return wrapped Model. Since Model is a shared object, it can be wrapped and unwrapped at 
   *  will. */
  model::Model model() const;

  /** Return the name of the wrapped model. If there is no name, returned result will be .empty() 
   */
  virtual std::string name() const;

  /** Get all objects in the model of type ddObjectType. */
  virtual Result< std::vector<model::ModelObject> > getObjects(
      const std::string& ddObjectType) const;

  /** Get objects of type ddObjectTypeTargetOrChild that are related to ddObjectTypeSourceOrParent sopObject.
   *
   *  \code 
   *  Result<std::vector<ObjectType> > returnedSchedules;
   *  returnedSchedules = getObject("space",myZone,"lighting schedule");
   *  \endcode */
  virtual Result< std::vector<model::ModelObject> > getObjects(
      const std::string& ddObjectTypeSourceOrParent, 
      const model::ModelObject& sopObject,
      const std::string& ddObjectTypeTargetOrChild) const;

  /** Get the value of object's ddBooleanAttribute. ddBooleanAttribute must be registered as a boolean
   *  attribute for objects of type ddObjectType. */
  virtual Result<bool> booleanAttribute(const std::string& ddObjectType,
                                        const model::ModelObject& object,
                                        const std::string& ddBooleanAttribute) const;

  /** Get the value of object's ddIntegerAttribute. ddIntegerAttribute must be registered as an integer
   *  attribute for objects of type ddObjectType. */
  virtual Result<int> integerAttribute(const std::string& ddObjectType,
                                       const model::ModelObject& object,
                                       const std::string& ddIntegerAttribute) const;

  /** Get the value of objects's ddDoubleAttribute. ddDoubleAttribute must be registered as a double 
   *  attribute for objects of type ddObjectType. */
  virtual Result<double> doubleAttribute(const std::string& ddObjectType,
                                         const model::ModelObject& object,
                                         const std::string& ddDoubleAttribute) const;

  /** Get the value of object's ddEnumAttribute. ddEnumAttribute must be registered as an enum attribute
   *  for objects of type ddObjectType. */
  virtual Result<std::string> enumAttribute(const std::string& ddObjectType,
                                            const model::ModelObject& object,
                                            const std::string& ddEnumAttribute) const;

  //@}
  /** @name Setters */
  //@{

  /** Set the name of the wrapped model, if possible. */
  virtual Result<result::none> setName(const std::string newName);

  /** Set sopObject to use tocObject as its object of type ddObjectTypeTargetOrChild that satisfies 
   *  ddBooleanAttribute, if that is a reasonable request.
   *
   *  \code 
   *  Result<result::none> success = setObject("zone",myZone,"lighting schedule",myLightingSchedule);
   *  \endcode */
  virtual Result<result::none> setObject(const std::string& ddObjectTypeSourceOrParent, 
                                         model::ModelObject& sopObject,
                                         const std::string& ddObjectTypeTargetOrChild,
                                         model::ModelObject& tocObject);

  /** Set object's ddBooleanAttribute to value, if possible. Returns false if the action cannot be 
   *  completed for any reason. */
  virtual Result<result::none> setAttribute(const std::string& ddObjectType,
                                            model::ModelObject& object,
                                            const std::string& ddBooleanAttribute,
                                            bool value);

  /** Set object's ddIntegerAttribute to value, if possible. Returns false if the action cannot be
   *  completed for any reason. */
  virtual Result<result::none> setAttribute(const std::string& ddObjectType,
                                            model::ModelObject& object,
                                            const std::string& ddIntegerAttribute,
                                            int value);

  /** Set object's ddDoubleAttribute to value, if possible. Returns false if the action cannot be 
   *  completed for any reason. */
  virtual Result<result::none> setAttribute(const std::string& ddObjectType,
                                            model::ModelObject& object,
                                            const std::string& ddDoubleAttribute,
                                            double value);

  /** Set object's ddEnumAttribute to value, if possible. Returns false if the action cannot be 
   *  completed for any reason. */
  virtual Result<result::none> setAttribute(const std::string& ddObjectType,
                                            model::ModelObject& object,
                                            const std::string& ddEnumAttribute,
                                            std::string value);

  /** Set object's ddEnumAttribute to value, if possible. Returns false if the action cannot be
  / *  completed for any reason. */
  virtual Result<result::none> setAttribute(const std::string& ddObjectType,
                                            model::ModelObject& object,
                                            const std::string& ddEnumAttribute,
                                            const char* value);

  //@}
  /** @name Simulation */
  //@{

  /** Before simulating, ask interface to ensure that ddObjectType's ddAttribute will be available 
   *  after simulation. Task is trivially completed if simulation not necessary to get 
   *  ddAttribute. If ddAttribute can be requested at different frequencies, this function 
   *  assumes that an annual number is desired. */
  virtual Result<result::none> requestAttribute(const std::string& ddObjectType,
                                                const std::string& ddAttribute);

  /** Simulate the wrapped model. Depending on implementation, request may take awhile to 
   *  complete. */
  virtual Result<result::none> simulate(const std::string& energyPlusPath,
                                        const std::string& energyPlusIddPath=std::string());

  //@}
  /** @name Static Queries */
  //@{

  static std::vector<ObjectType> supportedObjectTypes();

  static std::vector<ObjectType> acceptedSynonyms(const ObjectType& ot);

  static std::vector<BooleanAttribute> supportedBooleanAttributes(const ObjectType& ot);

  static std::vector<IntegerAttribute> supportedIntegerAttributes(const ObjectType& ot);

  static std::vector<DoubleAttribute> supportedDoubleAttributes(const ObjectType& ot);

  static std::vector<EnumAttribute> supportedEnumAttributes(const ObjectType& ot);

  //@}

 protected:
  /// @cond
  model::Model m_model;

  // ATTRIBUTE HELPERS
  std::string dataDictionaryTermToAttributeName(const std::string& ddTerm) const;

  std::string dataDictionaryTermToRelationshipName(const std::string& ddTerm) const;

  std::string attributeValueToDataDictionaryEnumAttributeValue(const ObjectType& objectType,
                                                               const model::ModelObject& object,
                                                               const EnumAttribute& attribute,
                                                               const std::string& value) const;

  // ERROR CHECKING HELPERS

  /** Tries to verify that object is of ddObjectType. If successful, return value will be 
   *  .empty(). Otherwise, return value will contain 
   *  modelInputErrorMessage(object,ddObjectType). */
  std::string verifyObjectType(const ObjectType& objTyp,const model::ModelObject& object) const;

  // ERROR MESSAGE HELPERS

  std::string dataDictionaryErrorMessage(const std::string& entry) const;
  std::string dataDictionaryErrorMessage(const PartsOfSpeech& type,
                                         const std::string& entry) const;
  std::string dataDictionaryErrorMessage(const EnumAttribute& e,
                                         const std::string& value) const;
  std::string modelInputErrorMessage(const std::string& ddObjectType,
                                     const model::ModelObject& object) const;

  REGISTER_LOGGER("standardsinterface.OpenStudioStandardsInterface");
  /// @endcond
};

} // standardsinterface
} // openstudio

#endif // STANDARDSINTERFACE_OPENSTUDIOSTANDARDSINTERFACE_HPP
