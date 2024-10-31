/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_INTERNALMASSDEFINITION_IMPL_HPP
#define MODEL_INTERNALMASSDEFINITION_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition_Impl.hpp"

namespace openstudio {
namespace model {

  class ConstructionBase;
  class InternalMassDefinition;

  namespace detail {

    /** InternalMassDefinition_Impl is a SpaceLoadDefinition_Impl that is the implementation class for InternalMassDefinition.*/
    class MODEL_API InternalMassDefinition_Impl : public SpaceLoadDefinition_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      InternalMassDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      InternalMassDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      InternalMassDefinition_Impl(const InternalMassDefinition_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~InternalMassDefinition_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      /// Returns the construction object, if this object does not directly specify construction the hierarchy is searched.
      boost::optional<ConstructionBase> construction() const;

      /// Returns true if the construction is not directly referenced by this surface .
      bool isConstructionDefaulted() const;

      std::string designLevelCalculationMethod() const;
      boost::optional<double> surfaceArea() const;
      boost::optional<double> surfaceAreaperSpaceFloorArea() const;
      boost::optional<double> surfaceAreaperPerson() const;

      //@}
      /** @name Setters */
      //@{

      /// Sets the construction object.
      bool setConstruction(const ConstructionBase& construction);

      /// Resets the construction object.
      void resetConstruction();

      /// Sets designLevelCalculationMethod to 'SurfaceArea', clears surfaceAreaperSpaceFloorArea
      /// and surfaceAreaperPerson, and sets surfaceArea.
      bool setSurfaceArea(boost::optional<double> surfaceArea);

      /// Sets designLevelCalculationMethod to 'SurfaceArea/Area', clears surfaceArea
      /// and surfaceAreaperPerson, and sets surfaceAreaperSpaceFloorArea.
      bool setSurfaceAreaperSpaceFloorArea(boost::optional<double> surfaceAreaperSpaceFloorArea);

      /// Sets designLevelCalculationMethod to 'SurfaceArea/Person', clears surfaceArea
      /// and surfaceAreaperSpaceFloorArea, and sets surfaceAreaperPerson.
      bool setSurfaceAreaperPerson(boost::optional<double> surfaceAreaperPerson);

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
      bool setDesignLevelCalculationMethod(const std::string& method, double floorArea, double numPeople);

      //@}
     protected:
     private:
      REGISTER_LOGGER("openstudio.model.InternalMassDefinition");

      boost::optional<ModelObject> constructionAsModelObject() const;

      bool setConstructionAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_INTERNALMASSDEFINITION_IMPL_HPP
