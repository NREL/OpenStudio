/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_ILLUMINANCEMAP_HPP
#define MODEL_ILLUMINANCEMAP_HPP

#include "ModelAPI.hpp"
#include "SpaceItem.hpp"

namespace openstudio {

class Point3d;
class Transformation;

namespace model {

  namespace detail {

    class IlluminanceMap_Impl;

  }  // namespace detail

  /** IlluminanceMap is a SpaceItem and is an interface to the OpenStudio IDD object named "OS:IlluminanceMap".
 *
 *  IlluminanceMap is a child of the Space object and is used to report daylighting illuminance values
 *  over a grid.  The grid of points to calculate daylight
 *  illuminance over is defined by the minimum and maximum corners of a rectangle aligned with the parent \link Space Space's \endlink
 *  coordinate system.
 */
  class MODEL_API IlluminanceMap : public SpaceItem
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit IlluminanceMap(const Model& model);

    virtual ~IlluminanceMap() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    IlluminanceMap(const IlluminanceMap& other) = default;
    IlluminanceMap(IlluminanceMap&& other) = default;
    IlluminanceMap& operator=(const IlluminanceMap&) = default;
    IlluminanceMap& operator=(IlluminanceMap&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
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

    /// Get the transformation from illuminance map coordinates to space coordinates
    openstudio::Transformation transformation() const;

    /// Set the transformation from illuminance map coordinates to space coordinates
    bool setTransformation(const openstudio::Transformation& transformation);

    /// Returns a vector of all the reference points in the calculation grid in illuminance map coordinates.
    /// The points are returned in order such as [0, 0, 0], ..., [xLength, 0, 0], [0, yLength/numberofYGridPoints, 0], ..., [xLength, yLength, 0]
    /// To get the points in space coordinates use transformation * referencePoints
    std::vector<Point3d> referencePoints() const;

    /// Returns a vector of the 4 corner reference points in the calculation grid in illuminance map coordinates.
    /// The points returned are [0, yLength, 0], [0, 0, 0], [xLength, 0, 0], [xLength, yLength, 0]
    /// To get the points in space coordinates use transformation * referencePoints
    std::vector<Point3d> corners() const;

   protected:
    /// @cond
    using ImplType = detail::IlluminanceMap_Impl;

    friend class Model;
    friend class openstudio::IdfObject;

    explicit IlluminanceMap(std::shared_ptr<detail::IlluminanceMap_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.IlluminanceMap");
  };

  /** \relates IlluminanceMap*/
  using OptionalIlluminanceMap = boost::optional<IlluminanceMap>;

  /** \relates IlluminanceMap*/
  using IlluminanceMapVector = std::vector<IlluminanceMap>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_ILLUMINANCEMAP_HPP
