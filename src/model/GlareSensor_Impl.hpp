/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
    class MODEL_API GlareSensor_Impl : public SpaceItem_Impl
    {

     public:
      /** @name Constructors and Destructors */
      //@{

      GlareSensor_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle);

      GlareSensor_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle);

      GlareSensor_Impl(const GlareSensor_Impl& other, Model_Impl* model, bool keepHandle);

      virtual ~GlareSensor_Impl() override = default;

      //@}

      virtual const std::vector<std::string>& outputVariableNames() const override;

      virtual IddObjectType iddObjectType() const override;

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

      bool setPositionXCoordinate(double positionXCoordinate);

      bool setPositionYCoordinate(double positionYCoordinate);

      bool setPositionZCoordinate(double positionZCoordinate);

      bool setPsiRotationAroundXAxis(double psiRotationAroundXAxis);

      void resetPsiRotationAroundXAxis();

      bool setThetaRotationAroundYAxis(double thetaRotationAroundYAxis);

      void resetThetaRotationAroundYAxis();

      bool setPhiRotationAroundZAxis(double phiRotationAroundZAxis);

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
      virtual int spaceIndex() const override;

     private:
      REGISTER_LOGGER("openstudio.model.GlareSensor");
    };

  }  // namespace detail

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GLARESENSOR_IMPL_HPP
