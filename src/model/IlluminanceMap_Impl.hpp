/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
    class MODEL_API IlluminanceMap_Impl : public SpaceItem_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      IlluminanceMap_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      IlluminanceMap_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      IlluminanceMap_Impl(const IlluminanceMap_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~IlluminanceMap_Impl() override = default;

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

      bool setOriginXCoordinate(double originXCoordinate);

      bool setOriginYCoordinate(double originYCoordinate);

      bool setOriginZCoordinate(double originZCoordinate);

      bool setPsiRotationAroundXAxis(double psiRotationAroundXAxis);

      void resetPsiRotationAroundXAxis();

      bool setThetaRotationAroundYAxis(double thetaRotationAroundYAxis);

      void resetThetaRotationAroundYAxis();

      bool setPhiRotationAroundZAxis(double phiRotationAroundZAxis);

      void resetPhiRotationAroundZAxis();

      bool setXLength(double xLength);

      void resetXLength();

      bool setNumberofXGridPoints(int numberofXGridPoints);

      void resetNumberofXGridPoints();

      bool setYLength(double yLength);

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

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ILLUMINANCEMAP_IMPL_HPP
