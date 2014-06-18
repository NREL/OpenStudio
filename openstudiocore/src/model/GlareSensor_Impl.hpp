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

#ifndef MODEL_GLARESENSOR_IMPL_HPP
#define MODEL_GLARESENSOR_IMPL_HPP

#include "ModelAPI.hpp"
#include "SpaceItem_Impl.hpp"

namespace openstudio {

class Point3d;
class Transformation;

namespace model {

class GlareSensor;

namespace detail {

  /** GlareSensor_Impl is a SpaceItem_Impl that is the implementation class for GlareSensor.*/
  class MODEL_API GlareSensor_Impl : public SpaceItem_Impl {
    Q_OBJECT;
    Q_PROPERTY(double positionXCoordinate READ positionXCoordinate WRITE setPositionXCoordinate);
    Q_PROPERTY(double positionYCoordinate READ positionYCoordinate WRITE setPositionYCoordinate);
    Q_PROPERTY(double positionZCoordinate READ positionZCoordinate WRITE setPositionZCoordinate);
    Q_PROPERTY(double psiRotationAroundXAxis READ psiRotationAroundXAxis WRITE setPsiRotationAroundXAxis RESET resetPsiRotationAroundXAxis);
    Q_PROPERTY(bool isPsiRotationAroundXAxisDefaulted READ isPsiRotationAroundXAxisDefaulted);
    Q_PROPERTY(double thetaRotationAroundYAxis READ thetaRotationAroundYAxis WRITE setThetaRotationAroundYAxis RESET resetThetaRotationAroundYAxis);
    Q_PROPERTY(bool isThetaRotationAroundYAxisDefaulted READ isThetaRotationAroundYAxisDefaulted);
    Q_PROPERTY(double phiRotationAroundZAxis READ phiRotationAroundZAxis WRITE setPhiRotationAroundZAxis RESET resetPhiRotationAroundZAxis);
    Q_PROPERTY(bool isPhiRotationAroundZAxisDefaulted READ isPhiRotationAroundZAxisDefaulted);
    Q_PROPERTY(int numberofGlareViewVectors READ numberofGlareViewVectors WRITE setNumberofGlareViewVectors RESET resetNumberofGlareViewVectors);
    Q_PROPERTY(boost::optional<double> maximumAllowableDaylightGlareProbability READ maximumAllowableDaylightGlareProbability WRITE setMaximumAllowableDaylightGlareProbability RESET resetMaximumAllowableDaylightGlareProbability);
   public:
    /** @name Constructors and Destructors */
    //@{

    GlareSensor_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

    GlareSensor_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                            Model_Impl* model,
                            bool keepHandle);

    GlareSensor_Impl(const GlareSensor_Impl& other,
                            Model_Impl* model,
                            bool keepHandle);

    virtual ~GlareSensor_Impl() {}

    //@}

    virtual const std::vector<std::string>& outputVariableNames() const;

    virtual IddObjectType iddObjectType() const;

    /** @name Getters */
    //@{

    double positionXCoordinate() const;

    double positionYCoordinate() const;

    double positionZCoordinate() const;

    double psiRotationAroundXAxis() const;

    bool isPsiRotationAroundXAxisDefaulted() const;

    double thetaRotationAroundYAxis() const;

    bool isThetaRotationAroundYAxisDefaulted() const;

    double phiRotationAroundZAxis() const;

    bool isPhiRotationAroundZAxisDefaulted() const;

    int numberofGlareViewVectors() const;

    bool isNumberofGlareViewVectorsDefaulted() const;

    boost::optional<double> maximumAllowableDaylightGlareProbability() const;
    
    bool isMaximumAllowableDaylightGlareProbabilityDefaulted() const;   

    //@}
    /** @name Setters */
    //@{

    void setPositionXCoordinate(double positionXCoordinate);

    void setPositionYCoordinate(double positionYCoordinate);

    void setPositionZCoordinate(double positionZCoordinate);

    void setPsiRotationAroundXAxis(double psiRotationAroundXAxis);

    void resetPsiRotationAroundXAxis();

    void setThetaRotationAroundYAxis(double thetaRotationAroundYAxis);

    void resetThetaRotationAroundYAxis();

    void setPhiRotationAroundZAxis(double phiRotationAroundZAxis);

    void resetPhiRotationAroundZAxis();

    bool setNumberofGlareViewVectors(int numberofGlareViewVectors);

    void resetNumberofGlareViewVectors();

    bool setMaximumAllowableDaylightGlareProbability(boost::optional<double> maximumAllowableDaylightGlareProbability);

    void resetMaximumAllowableDaylightGlareProbability();

    //@}

    openstudio::Point3d position() const;

    bool setPosition(const openstudio::Point3d& position);

    openstudio::Transformation transformation() const;

    bool setTransformation(const openstudio::Transformation& transformation);

    bool aimAt(const Point3d& target);

   protected: 

    // index of the space name
    virtual int spaceIndex() const;

   private:
    REGISTER_LOGGER("openstudio.model.GlareSensor");
  };

} // detail

} // model
} // openstudio

#endif // MODEL_GLARESENSOR_IMPL_HPP

