/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_GLARESENSOR_HPP
#define MODEL_GLARESENSOR_HPP

#include "ModelAPI.hpp"
#include "SpaceItem.hpp"

namespace openstudio {

class Point3d;
class Transformation;

namespace model {

  namespace detail {

    class GlareSensor_Impl;

  }  // namespace detail

  /** GlareSensor is a SpaceItem that wraps the OpenStudio IDD object 'OS:Glare:Sensor'. */
  class MODEL_API GlareSensor : public SpaceItem
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit GlareSensor(const Model& model);

    virtual ~GlareSensor() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    GlareSensor(const GlareSensor& other) = default;
    GlareSensor(GlareSensor&& other) = default;
    GlareSensor& operator=(const GlareSensor&) = default;
    GlareSensor& operator=(GlareSensor&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
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

    boost::optional<double> maximumAllowableDaylightGlareProbability() const;

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

    bool setMaximumAllowableDaylightGlareProbability(double maximumAllowableDaylightGlareProbability);

    void resetMaximumAllowableDaylightGlareProbability();

    //@}

    /// Returns a Point3d representing position only in meters relative to the Space.
    openstudio::Point3d position() const;

    /// Sets a Point3d representing position only position in meters relative to the Space.
    /// Does not change the orientation of the GlareSensor.
    bool setPosition(const openstudio::Point3d& position);

    /// Returns a Transformation representing position and orientation in meters relative to the Space.
    openstudio::Transformation transformation() const;

    /** Sets a Transformation representing position and orientation in meters relative to the Space. */
    bool setTransformation(const openstudio::Transformation& transformation);

    /// Keeps current position and orients the view towards the target, target is in space coordinates.
    bool aimAt(const Point3d& target);

   protected:
    /// @cond
    using ImplType = detail::GlareSensor_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit GlareSensor(std::shared_ptr<detail::GlareSensor_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.GlareSensor");
  };

  /** \relates GlareSensor*/
  using OptionalGlareSensor = boost::optional<GlareSensor>;

  /** \relates GlareSensor*/
  using GlareSensorVector = std::vector<GlareSensor>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_GLARESENSOR_HPP
