/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

    virtual bool setDefinition(const SpaceLoadDefinition& definition) override;

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

    bool setEndUseSubcategory(std::string endUseSubcategory);

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

