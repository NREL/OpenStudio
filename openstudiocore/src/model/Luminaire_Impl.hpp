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

#ifndef MODEL_LUMINAIRE_IMPL_HPP
#define MODEL_LUMINAIRE_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceLoadInstance_Impl.hpp"

#include "../utilities/units/Quantity.hpp"
#include "../utilities/units/OSOptionalQuantity.hpp"

namespace openstudio {

class Point3d;
class Transformation;

namespace model {

class Schedule;
class LuminaireDefinition;
class Luminaire;

namespace detail {

  /** Luminaire_Impl is a SpaceLoadInstance_Impl that is the implementation class for Luminaire.*/
  class MODEL_API Luminaire_Impl : public SpaceLoadInstance_Impl {
    Q_OBJECT;
    Q_PROPERTY(bool isScheduleDefaulted READ isScheduleDefaulted);

    Q_PROPERTY(double positionXcoordinate READ positionXcoordinate WRITE setPositionXcoordinate);
    Q_PROPERTY(openstudio::Quantity positionXcoordinate_SI READ positionXcoordinate_SI WRITE setPositionXcoordinate);
    Q_PROPERTY(openstudio::Quantity positionXcoordinate_IP READ positionXcoordinate_IP WRITE setPositionXcoordinate);

    Q_PROPERTY(double positionYcoordinate READ positionYcoordinate WRITE setPositionYcoordinate);
    Q_PROPERTY(openstudio::Quantity positionYcoordinate_SI READ positionYcoordinate_SI WRITE setPositionYcoordinate);
    Q_PROPERTY(openstudio::Quantity positionYcoordinate_IP READ positionYcoordinate_IP WRITE setPositionYcoordinate);

    Q_PROPERTY(double positionZcoordinate READ positionZcoordinate WRITE setPositionZcoordinate);
    Q_PROPERTY(openstudio::Quantity positionZcoordinate_SI READ positionZcoordinate_SI WRITE setPositionZcoordinate);
    Q_PROPERTY(openstudio::Quantity positionZcoordinate_IP READ positionZcoordinate_IP WRITE setPositionZcoordinate);

    Q_PROPERTY(double psiRotationAroundXaxis READ psiRotationAroundXaxis WRITE setPsiRotationAroundXaxis RESET resetPsiRotationAroundXaxis);
    Q_PROPERTY(openstudio::Quantity psiRotationAroundXaxis_SI READ psiRotationAroundXaxis_SI WRITE setPsiRotationAroundXaxis RESET resetPsiRotationAroundXaxis);
    Q_PROPERTY(openstudio::Quantity psiRotationAroundXaxis_IP READ psiRotationAroundXaxis_IP WRITE setPsiRotationAroundXaxis RESET resetPsiRotationAroundXaxis);
    Q_PROPERTY(bool isPsiRotationAroundXaxisDefaulted READ isPsiRotationAroundXaxisDefaulted);

    Q_PROPERTY(double thetaRotationAroundYaxis READ thetaRotationAroundYaxis WRITE setThetaRotationAroundYaxis RESET resetThetaRotationAroundYaxis);
    Q_PROPERTY(openstudio::Quantity thetaRotationAroundYaxis_SI READ thetaRotationAroundYaxis_SI WRITE setThetaRotationAroundYaxis RESET resetThetaRotationAroundYaxis);
    Q_PROPERTY(openstudio::Quantity thetaRotationAroundYaxis_IP READ thetaRotationAroundYaxis_IP WRITE setThetaRotationAroundYaxis RESET resetThetaRotationAroundYaxis);
    Q_PROPERTY(bool isThetaRotationAroundYaxisDefaulted READ isThetaRotationAroundYaxisDefaulted);

    Q_PROPERTY(double phiRotationAroundZaxis READ phiRotationAroundZaxis WRITE setPhiRotationAroundZaxis RESET resetPhiRotationAroundZaxis);
    Q_PROPERTY(openstudio::Quantity phiRotationAroundZaxis_SI READ phiRotationAroundZaxis_SI WRITE setPhiRotationAroundZaxis RESET resetPhiRotationAroundZaxis);
    Q_PROPERTY(openstudio::Quantity phiRotationAroundZaxis_IP READ phiRotationAroundZaxis_IP WRITE setPhiRotationAroundZaxis RESET resetPhiRotationAroundZaxis);
    Q_PROPERTY(bool isPhiRotationAroundZaxisDefaulted READ isPhiRotationAroundZaxisDefaulted);

    Q_PROPERTY(double fractionReplaceable READ fractionReplaceable WRITE setFractionReplaceable RESET resetFractionReplaceable);
    Q_PROPERTY(openstudio::Quantity fractionReplaceable_SI READ fractionReplaceable_SI WRITE setFractionReplaceable RESET resetFractionReplaceable);
    Q_PROPERTY(openstudio::Quantity fractionReplaceable_IP READ fractionReplaceable_IP WRITE setFractionReplaceable RESET resetFractionReplaceable);
    Q_PROPERTY(bool isFractionReplaceableDefaulted READ isFractionReplaceableDefaulted);

    Q_PROPERTY(double multiplier READ multiplier WRITE setMultiplier RESET resetMultiplier);
    Q_PROPERTY(bool isMultiplierDefaulted READ isMultiplierDefaulted);

    Q_PROPERTY(std::string endUseSubcategory READ endUseSubcategory WRITE setEndUseSubcategory RESET resetEndUseSubcategory);
    Q_PROPERTY(bool isEndUseSubcategoryDefaulted READ isEndUseSubcategoryDefaulted);

    Q_PROPERTY(double lightingPower READ lightingPower);

    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> luminaireDefinition READ luminaireDefinitionAsModelObject WRITE setLuminaireDefinitionAsModelObject);
    Q_PROPERTY(boost::optional<openstudio::model::ModelObject> schedule READ scheduleAsModelObject WRITE setScheduleAsModelObject RESET resetSchedule);
   public:
    /** @name Constructors and Destructors */
    //@{

    Luminaire_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    Luminaire_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                   Model_Impl* model,
                   bool keepHandle);

    Luminaire_Impl(const Luminaire_Impl& other,
                   Model_Impl* model,
                   bool keepHandle);

    virtual ~Luminaire_Impl() {}

    //@}
    /** @name Virtual Methods */
    //@{
    
    virtual const std::vector<std::string>& outputVariableNames() const;
    
    virtual IddObjectType iddObjectType() const;

    virtual std::vector<ScheduleTypeKey> getScheduleTypeKeys(const Schedule& schedule) const;

    virtual bool hardSize();

    virtual bool hardApplySchedules();

    virtual double multiplier() const;

    virtual bool isMultiplierDefaulted() const;

    //@}
    /** @name Getters */
    //@{

    LuminaireDefinition luminaireDefinition() const;

    boost::optional<Schedule> schedule() const;

    bool isScheduleDefaulted() const;

    double positionXcoordinate() const;

    Quantity getPositionXcoordinate(bool returnIP=false) const;

    double positionYcoordinate() const;

    Quantity getPositionYcoordinate(bool returnIP=false) const;

    double positionZcoordinate() const;

    Quantity getPositionZcoordinate(bool returnIP=false) const;

    double psiRotationAroundXaxis() const;

    Quantity getPsiRotationAroundXaxis(bool returnIP=false) const;

    bool isPsiRotationAroundXaxisDefaulted() const;

    double thetaRotationAroundYaxis() const;

    Quantity getThetaRotationAroundYaxis(bool returnIP=false) const;

    bool isThetaRotationAroundYaxisDefaulted() const;

    double phiRotationAroundZaxis() const;

    Quantity getPhiRotationAroundZaxis(bool returnIP=false) const;

    bool isPhiRotationAroundZaxisDefaulted() const;

    double fractionReplaceable() const;

    Quantity getFractionReplaceable(bool returnIP=false) const;

    bool isFractionReplaceableDefaulted() const;

    std::string endUseSubcategory() const;

    bool isEndUseSubcategoryDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setLuminaireDefinition(const LuminaireDefinition& definition);

    virtual bool setDefinition(const SpaceLoadDefinition& definition);

    bool setSchedule(Schedule& schedule);

    void resetSchedule();

    void setPositionXcoordinate(double positionXcoordinate);

    bool setPositionXcoordinate(const Quantity& positionXcoordinate);

    void setPositionYcoordinate(double positionYcoordinate);

    bool setPositionYcoordinate(const Quantity& positionYcoordinate);

    void setPositionZcoordinate(double positionZcoordinate);

    bool setPositionZcoordinate(const Quantity& positionZcoordinate);

    void setPsiRotationAroundXaxis(double psiRotationAroundXaxis);

    bool setPsiRotationAroundXaxis(const Quantity& psiRotationAroundXaxis);

    void resetPsiRotationAroundXaxis();

    void setThetaRotationAroundYaxis(double thetaRotationAroundYaxis);

    bool setThetaRotationAroundYaxis(const Quantity& thetaRotationAroundYaxis);

    void resetThetaRotationAroundYaxis();

    void setPhiRotationAroundZaxis(double phiRotationAroundZaxis);

    bool setPhiRotationAroundZaxis(const Quantity& phiRotationAroundZaxis);

    void resetPhiRotationAroundZaxis();

    bool setFractionReplaceable(double fractionReplaceable);

    bool setFractionReplaceable(const Quantity& fractionReplaceable);

    void resetFractionReplaceable();

    bool setMultiplier(double multiplier);

    void resetMultiplier();

    void setEndUseSubcategory(std::string endUseSubcategory);

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
    virtual int spaceIndex() const;

    // index of the definition name
    virtual int definitionIndex() const;

   private:
    REGISTER_LOGGER("openstudio.model.Luminaire");

    openstudio::Quantity positionXcoordinate_SI() const;
    openstudio::Quantity positionXcoordinate_IP() const;
    openstudio::Quantity positionYcoordinate_SI() const;
    openstudio::Quantity positionYcoordinate_IP() const;
    openstudio::Quantity positionZcoordinate_SI() const;
    openstudio::Quantity positionZcoordinate_IP() const;
    openstudio::Quantity psiRotationAroundXaxis_SI() const;
    openstudio::Quantity psiRotationAroundXaxis_IP() const;
    openstudio::Quantity thetaRotationAroundYaxis_SI() const;
    openstudio::Quantity thetaRotationAroundYaxis_IP() const;
    openstudio::Quantity phiRotationAroundZaxis_SI() const;
    openstudio::Quantity phiRotationAroundZaxis_IP() const;
    openstudio::Quantity fractionReplaceable_SI() const;
    openstudio::Quantity fractionReplaceable_IP() const;

    boost::optional<ModelObject> luminaireDefinitionAsModelObject() const;
    boost::optional<ModelObject> scheduleAsModelObject() const;

    bool setLuminaireDefinitionAsModelObject(const boost::optional<ModelObject>& modelObject);
    bool setScheduleAsModelObject(const boost::optional<ModelObject>& modelObject);
  };

} // detail

} // model
} // openstudio

#endif // MODEL_LUMINAIRE_IMPL_HPP

