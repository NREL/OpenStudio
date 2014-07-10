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

#ifndef MODEL_INTERNALMASS_IMPL_HPP
#define MODEL_INTERNALMASS_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance_Impl.hpp"

namespace openstudio {
namespace model {

class InternalMass;
class InternalMassDefinition;

namespace detail {

  /** InternalMass_Impl is a SpaceLoadInstance_Impl that is the implementation class for InternalMass.*/
  class MODEL_API InternalMass_Impl : public SpaceLoadInstance_Impl {
    Q_OBJECT;
    Q_PROPERTY(double multiplier READ multiplier WRITE setMultiplier RESET resetMultiplier);
    Q_PROPERTY(bool isMultiplierDefaulted READ isMultiplierDefaulted);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> internalMassDefinition READ internalMassDefinitionAsModelObject WRITE setInternalMassDefinitionAsModelObject);
   public:
    /** @name Constructors and Destructors */
    //@{

    InternalMass_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    InternalMass_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                      Model_Impl* model,
                      bool keepHandle);

    InternalMass_Impl(const InternalMass_Impl& other,
                      Model_Impl* model,
                      bool keepHandle);

    virtual ~InternalMass_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    virtual bool hardSize();

    virtual bool hardApplySchedules();

    virtual double multiplier() const;

    virtual bool isMultiplierDefaulted() const;

    //@}
    /** @name Getters */
    //@{

    /** Gets the InternalMassDefinition object. */
    InternalMassDefinition internalMassDefinition() const;

    //@}
    /** @name Setters */
    //@{

    /** Sets the InternalMassDefinition object. */
    bool setInternalMassDefinition(const InternalMassDefinition& definition);

    /** Sets the definition of this instance. */
    virtual bool setDefinition(const SpaceLoadDefinition& definition);

    bool setMultiplier(double multiplier);

    void resetMultiplier();

    //@}
    /** @name Other */
    //@{

    boost::optional<double> surfaceArea() const;

    boost::optional<double> surfaceAreaPerFloorArea() const;

    boost::optional<double> surfaceAreaPerPerson() const;

    /** Returns the construction surface area represented by this instance, assuming floorArea (m^2)
     *  and numPeople. */
    double getSurfaceArea(double floorArea, double numPeople) const;

    /** Returns the m^2/m^2 of construction surface area per floor area represented by this 
     *  instance, assuming floorArea (m^2) and numPeople. */
    double getSurfaceAreaPerFloorArea(double floorArea, double numPeople) const;

    /** Returns the m^2/person of construction surface area represented by this instance, assuming 
     *  floorArea (m^2) and numPeople. */
    double getSurfaceAreaPerPerson(double floorArea, double numPeople) const;

    //@}
   protected:

    // index of the space name
    virtual int spaceIndex() const;

    // index of the definition name
    virtual int definitionIndex() const;

   private:
    REGISTER_LOGGER("openstudio.model.InternalMass");

    boost::optional<ModelObject> internalMassDefinitionAsModelObject() const;

    bool setInternalMassDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_INTERNALMASS_IMPL_HPP

