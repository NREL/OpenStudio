/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#ifndef MODEL_ILLUMINANCEMAP_IMPL_HPP
#define MODEL_ILLUMINANCEMAP_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceItem_Impl.hpp"

namespace openstudio {

class Point3d;
class Transformation;

namespace model {

class IlluminanceMap;

namespace detail {

  /** IlluminanceMap_Impl is a SpaceItem_Impl that is the implementation class for IlluminanceMap.*/
  class MODEL_API IlluminanceMap_Impl : public SpaceItem_Impl {
    Q_OBJECT;
    Q_PROPERTY(double originXCoordinate READ originXCoordinate WRITE setOriginXCoordinate);
    Q_PROPERTY(double originYCoordinate READ originYCoordinate WRITE setOriginYCoordinate);
    Q_PROPERTY(double originZCoordinate READ originZCoordinate WRITE setOriginZCoordinate);
    Q_PROPERTY(double psiRotationAroundXAxis READ psiRotationAroundXAxis WRITE setPsiRotationAroundXAxis RESET resetPsiRotationAroundXAxis);
    Q_PROPERTY(bool isPsiRotationAroundXAxisDefaulted READ isPsiRotationAroundXAxisDefaulted);
    Q_PROPERTY(double thetaRotationAroundYAxis READ thetaRotationAroundYAxis WRITE setThetaRotationAroundYAxis RESET resetThetaRotationAroundYAxis);
    Q_PROPERTY(bool isThetaRotationAroundYAxisDefaulted READ isThetaRotationAroundYAxisDefaulted);
    Q_PROPERTY(double phiRotationAroundZAxis READ phiRotationAroundZAxis WRITE setPhiRotationAroundZAxis RESET resetPhiRotationAroundZAxis);
    Q_PROPERTY(bool isPhiRotationAroundZAxisDefaulted READ isPhiRotationAroundZAxisDefaulted);
    Q_PROPERTY(double xLength READ xLength WRITE setXLength RESET resetXLength);
    Q_PROPERTY(bool isXLengthDefaulted READ isXLengthDefaulted);
    Q_PROPERTY(int numberofXGridPoints READ numberofXGridPoints WRITE setNumberofXGridPoints RESET resetNumberofXGridPoints);
    Q_PROPERTY(bool isNumberofXGridPointsDefaulted READ isNumberofXGridPointsDefaulted);
    Q_PROPERTY(double yLength READ yLength WRITE setYLength RESET resetYLength);
    Q_PROPERTY(bool isYLengthDefaulted READ isYLengthDefaulted);
    Q_PROPERTY(int numberofYGridPoints READ numberofYGridPoints WRITE setNumberofYGridPoints RESET resetNumberofYGridPoints);
    Q_PROPERTY(bool isNumberofYGridPointsDefaulted READ isNumberofYGridPointsDefaulted);
   public:
    /** @name Constructors and Destructors */
    //@{

    IlluminanceMap_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    IlluminanceMap_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    IlluminanceMap_Impl(const IlluminanceMap_Impl& other,
                        Model_Impl* model,
                        bool keepHandle);

    virtual ~IlluminanceMap_Impl() {}

    //@}

    // Get all output variable names that could be associated with this object.
    virtual const std::vector<std::string>& outputVariableNames() const override;

    virtual IddObjectType iddObjectType() const override;

    /** @name Getters */
    //@{

    double originXCoordinate() const;

    double originYCoordinate() const;

    double originZCoordinate() const;

    double psiRotationAroundXAxis() const;

    bool isPsiRotationAroundXAxisDefaulted() const;

    double thetaRotationAroundYAxis() const;

    bool isThetaRotationAroundYAxisDefaulted() const;

    double phiRotationAroundZAxis() const;

    bool isPhiRotationAroundZAxisDefaulted() const;

    double xLength() const;

    bool isXLengthDefaulted() const;

    int numberofXGridPoints() const;

    bool isNumberofXGridPointsDefaulted() const;

    double yLength() const;

    bool isYLengthDefaulted() const;

    int numberofYGridPoints() const;

    bool isNumberofYGridPointsDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    void setOriginXCoordinate(double originXCoordinate);

    void setOriginYCoordinate(double originYCoordinate);

    void setOriginZCoordinate(double originZCoordinate);

    bool setPsiRotationAroundXAxis(double psiRotationAroundXAxis);

    void resetPsiRotationAroundXAxis();

    bool setThetaRotationAroundYAxis(double thetaRotationAroundYAxis);

    void resetThetaRotationAroundYAxis();

    bool setPhiRotationAroundZAxis(double phiRotationAroundZAxis);

    void resetPhiRotationAroundZAxis();

    void setXLength(double xLength);

    void resetXLength();

    bool setNumberofXGridPoints(int numberofXGridPoints);

    void resetNumberofXGridPoints();

    void setYLength(double yLength);

    void resetYLength();

    bool setNumberofYGridPoints(int numberofYGridPoints);

    void resetNumberofYGridPoints();

    //@}

    openstudio::Transformation transformation() const;

    bool setTransformation(const openstudio::Transformation& transformation);

    std::vector<Point3d> referencePoints() const;
    
    std::vector<Point3d> corners() const;

   protected:

    // index of the space name
    virtual int spaceIndex() const override;

   private:
    REGISTER_LOGGER("openstudio.model.IlluminanceMap");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_ILLUMINANCEMAP_IMPL_HPP

