/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#ifndef MODEL_INTERNALMASSDEFINITION_IMPL_HPP
#define MODEL_INTERNALMASSDEFINITION_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition_Impl.hpp"
#include "attributes.hpp"

namespace openstudio {
namespace model {

class ConstructionBase;
class InternalMassDefinition;

namespace detail {

  /** InternalMassDefinition_Impl is a SpaceLoadDefinition_Impl that is the implementation class for InternalMassDefinition.*/
  class MODEL_API InternalMassDefinition_Impl : public SpaceLoadDefinition_Impl {





    Q_PROPERTY(openstudio::OSOptionalQuantity surfaceArea_SI
               READ surfaceArea_SI
               WRITE setSurfaceArea);
    Q_PROPERTY(openstudio::OSOptionalQuantity surfaceArea_IP
               READ surfaceArea_IP
               WRITE setSurfaceArea);

    Q_PROPERTY(boost::optional<double> surfaceAreaperSpaceFloorArea
               READ surfaceAreaperSpaceFloorArea
               WRITE setSurfaceAreaperSpaceFloorArea);
    Q_PROPERTY(openstudio::OSOptionalQuantity surfaceAreaperSpaceFloorArea_SI
               READ surfaceAreaperSpaceFloorArea_SI
               WRITE setSurfaceAreaperSpaceFloorArea);
    Q_PROPERTY(openstudio::OSOptionalQuantity surfaceAreaperSpaceFloorArea_IP
               READ surfaceAreaperSpaceFloorArea_IP
               WRITE setSurfaceAreaperSpaceFloorArea);

    Q_PROPERTY(boost::optional<double> surfaceAreaperPerson
               READ surfaceAreaperPerson
               WRITE setSurfaceAreaperPerson);
    Q_PROPERTY(openstudio::OSOptionalQuantity surfaceAreaperPerson_SI
               READ surfaceAreaperPerson_SI
               WRITE setSurfaceAreaperPerson);
    Q_PROPERTY(openstudio::OSOptionalQuantity surfaceAreaperPerson_IP
               READ surfaceAreaperPerson_IP
               WRITE setSurfaceAreaperPerson);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> construction
               READ constructionAsModelObject
               WRITE setConstructionAsModelObject
               RESET resetConstruction);


//
//
//
//


   public:
    /** @name Constructors and Destructors */
    //@{

    InternalMassDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    InternalMassDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                Model_Impl* model,
                                bool keepHandle);

    InternalMassDefinition_Impl(const InternalMassDefinition_Impl& other,
                                Model_Impl* model,
                                bool keepHandle);

    virtual ~InternalMassDefinition_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{

    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    // Attributes

    ATTRIBUTE_DEFINITION(1,0,0,surfaceArea,SurfaceArea)
    ATTRIBUTE_DEFINITION(1,0,0,surfaceAreaperSpaceFloorArea,SurfaceAreaperSpaceFloorArea)
    ATTRIBUTE_DEFINITION(1,0,0,surfaceAreaperPerson,SurfaceAreaperPerson)

    //@}
    /** @name Getters */
    //@{

    /// Returns the construction object, if this object does not directly specify construction the hierarchy is searched.
    boost::optional<ConstructionBase> construction() const;

    /// Returns true if the construction is not directly referenced by this surface .
    bool isConstructionDefaulted() const;

    std::string designLevelCalculationMethod() const;

    //boost::optional<double> surfaceArea() const;

    //boost::optional<double> surfaceAreaperSpaceFloorArea() const;

    //boost::optional<double> surfaceAreaperPerson() const;

    //@}
    /** @name Setters */
    //@{

    /// Sets the construction object.
    bool setConstruction(const ConstructionBase& construction);

    /// Resets the construction object.
    void resetConstruction();

    /// Sets designLevelCalculationMethod to 'SurfaceArea', clears surfaceAreaperSpaceFloorArea
    /// and surfaceAreaperPerson, and sets surfaceArea.
    //bool setSurfaceArea(boost::optional<double> surfaceArea);

    /// Sets designLevelCalculationMethod to 'SurfaceArea/Area', clears surfaceArea
    /// and surfaceAreaperPerson, and sets surfaceAreaperSpaceFloorArea.
    //bool setSurfaceAreaperSpaceFloorArea(boost::optional<double> surfaceAreaperSpaceFloorArea);

    /// Sets designLevelCalculationMethod to 'SurfaceArea/Person', clears surfaceArea
    /// and surfaceAreaperSpaceFloorArea, and sets surfaceAreaperPerson.
    //bool setSurfaceAreaperPerson(boost::optional<double> surfaceAreaperPerson);

    //@}
    /** @name Other */
    //@{

    /** Returns the construction surface area represented by this definition, assuming floorArea
     *  (m^2) and numPeople. */
    double getSurfaceArea(double floorArea, double numPeople) const;

    /** Returns the m^2/m^2 of construction surface area per floor area represented by this
     *  definition, assuming floorArea (m^2) and numPeople. */
    double getSurfaceAreaPerFloorArea(double floorArea, double numPeople) const;

    /** Returns the m^2/person of construction surface area represented by this definition, assuming
     *  floorArea (m^2) and numPeople. */
    double getSurfaceAreaPerPerson(double floorArea, double numPeople) const;

    /** If method is a \link validDesignLevelCalculationMethodValues valid design level calculation
     *  method \endlink, changes this definition to an equivalent construction surface area, under
     *  the assumptions of floorArea (m^2) and numPeople. */
    bool setDesignLevelCalculationMethod(const std::string& method,
                                         double floorArea,
                                         double numPeople);

    //@}
   protected:
   private:
    REGISTER_LOGGER("openstudio.model.InternalMassDefinition");

    boost::optional<ModelObject> constructionAsModelObject() const;

    bool setConstructionAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_INTERNALMASSDEFINITION_IMPL_HPP

