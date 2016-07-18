/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
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

