/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_OTHEREQUIPMENTDEFINITION_IMPL_HPP
#define MODEL_OTHEREQUIPMENTDEFINITION_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadDefinition_Impl.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    /** OtherEquipmentDefinition_Impl is a SpaceLoadDefinition_Impl that is the implementation class for OtherEquipmentDefinition.*/
    class MODEL_API OtherEquipmentDefinition_Impl : public SpaceLoadDefinition_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      OtherEquipmentDefinition_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      OtherEquipmentDefinition_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      OtherEquipmentDefinition_Impl(const OtherEquipmentDefinition_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~OtherEquipmentDefinition_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      //@}
      /** @name Getters */
      //@{

      std::string designLevelCalculationMethod() const;
      boost::optional<double> designLevel() const;
      boost::optional<double> wattsperSpaceFloorArea() const;
      boost::optional<double> wattsperPerson() const;

      double fractionLatent() const;
      bool isFractionLatentDefaulted() const;

      double fractionRadiant() const;
      bool isFractionRadiantDefaulted() const;

      double fractionLost() const;
      bool isFractionLostDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      /// Sets designLevelCalculationMethod to 'EquipmentLevel', clears wattsperSpaceFloorArea
      /// and wattsperPerson, and sets designLevel.
      bool setDesignLevel(boost::optional<double> designLevel);

      /// Sets designLevelCalculationMethod to 'Watts/Area', clears designLevel
      /// and wattsperPerson, and sets wattsperSpaceFloorArea.
      bool setWattsperSpaceFloorArea(boost::optional<double> wattsperSpaceFloorArea);

      /// Sets designLevelCalculationMethod to 'Watts/Person', clears designLevel
      /// and wattsperSpaceFloorArea, and sets wattsperPerson.
      bool setWattsperPerson(boost::optional<double> wattsperPerson);

      bool setFractionLatent(double fractionLatent);
      void resetFractionLatent();

      bool setFractionRadiant(double fractionRadiant);
      void resetFractionRadiant();

      bool setFractionLost(double fractionLost);
      void resetFractionLost();

      //@}
      /** @name Other */
      //@{

      /** Returns the design level represented by this definition, assuming floorArea (m^2) and
     *  numPeople. */
      double getDesignLevel(double floorArea, double numPeople) const;

      /** Returns the watts/m^2 represented by this definition, assuming floorArea (m^2) and
     *  numPeople. */
      double getPowerPerFloorArea(double floorArea, double numPeople) const;

      /** Returns the watts/person represented by this definition, assuming floorArea (m^2) and
     *  numPeople. */
      double getPowerPerPerson(double floorArea, double numPeople) const;

      /** If method is a \link validDesignLevelCalculationMethodValues valid design level
     *  calculation method \endlink, changes this definition to an equivalent power level, under
     *  the assumptions of floorArea (m^2) and numPeople. */
      bool setDesignLevelCalculationMethod(const std::string& method, double floorArea, double numPeople);

      virtual std::vector<EMSActuatorNames> emsActuatorNames() const override;

      virtual std::vector<std::string> emsInternalVariableNames() const override;

      //@}
     private:
      REGISTER_LOGGER("openstudio.model.OtherEquipmentDefinition");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_OTHEREQUIPMENTDEFINITION_IMPL_HPP
