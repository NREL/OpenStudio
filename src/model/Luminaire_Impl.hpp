/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_LUMINAIRE_IMPL_HPP
#define MODEL_LUMINAIRE_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance_Impl.hpp"

namespace openstudio {

class Point3d;
class Transformation;

namespace model {

  class Schedule;
  class LuminaireDefinition;
  class Luminaire;

  namespace detail {

    /** Luminaire_Impl is a SpaceLoadInstance_Impl that is the implementation class for Luminaire.*/
    class MODEL_API Luminaire_Impl : public SpaceLoadInstance_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      Luminaire_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      Luminaire_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      Luminaire_Impl(const Luminaire_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~Luminaire_Impl() override = default;

      //@}
      /** @name Virtual Methods */
      //@{

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

      virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const override;

      virtual bool hardSize() override;

      virtual bool hardApplySchedules() override;

      virtual double multiplier() const override;

      virtual bool isMultiplierDefaulted() const override;

      virtual bool isAbsolute() const override;

      //@}
      /** @name Getters */
      //@{

      LuminaireDefinition luminaireDefinition() const;

      boost::optional<Schedule> schedule() const;

      bool isScheduleDefaulted() const;

      double positionXcoordinate() const;

      double positionYcoordinate() const;

      double positionZcoordinate() const;

      double psiRotationAroundXaxis() const;

      bool isPsiRotationAroundXaxisDefaulted() const;

      double thetaRotationAroundYaxis() const;

      bool isThetaRotationAroundYaxisDefaulted() const;

      double phiRotationAroundZaxis() const;

      bool isPhiRotationAroundZaxisDefaulted() const;

      double fractionReplaceable() const;

      bool isFractionReplaceableDefaulted() const;

      std::string endUseSubcategory() const;

      bool isEndUseSubcategoryDefaulted() const;

      //@}
      /** @name Setters */
      //@{

      bool setLuminaireDefinition(const LuminaireDefinition& definition);

      virtual bool setDefinition(const SpaceLoadDefinition& definition) override;

      bool setSchedule(Schedule& schedule);

      void resetSchedule();

      bool setPositionXcoordinate(double positionXcoordinate);

      bool setPositionYcoordinate(double positionYcoordinate);

      bool setPositionZcoordinate(double positionZcoordinate);

      bool setPsiRotationAroundXaxis(double psiRotationAroundXaxis);

      void resetPsiRotationAroundXaxis();

      bool setThetaRotationAroundYaxis(double thetaRotationAroundYaxis);

      void resetThetaRotationAroundYaxis();

      bool setPhiRotationAroundZaxis(double phiRotationAroundZaxis);

      void resetPhiRotationAroundZaxis();

      bool setFractionReplaceable(double fractionReplaceable);

      void resetFractionReplaceable();

      bool setMultiplier(double multiplier);

      void resetMultiplier();

      bool setEndUseSubcategory(const std::string& endUseSubcategory);

      void resetEndUseSubcategory();

      //@}
      /** @name Other */
      //@{

      openstudio::Point3d position() const;

      bool setPosition(const openstudio::Point3d& position);

      openstudio::Transformation transformation() const;

      bool setTransformation(const openstudio::Transformation& transformation);

      double lightingPower() const;

      double getPowerPerFloorArea(double floorArea) const;

      double getPowerPerPerson(double numPeople) const;

      //@}
     protected:
      // index of the space name
      virtual int spaceIndex() const override;

      // index of the definition name
      virtual int definitionIndex() const override;

     private:
      REGISTER_LOGGER("openstudio.model.Luminaire");

      boost::optional<ModelObject> luminaireDefinitionAsModelObject() const;
      boost::optional<ModelObject> scheduleAsModelObject() const;

      bool setLuminaireDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject);
      bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_LUMINAIRE_IMPL_HPP
