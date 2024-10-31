/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_INTERIORPARTITIONSURFACE_HPP
#define MODEL_INTERIORPARTITIONSURFACE_HPP

#include "ModelAPI.hpp"
#include "PlanarSurface.hpp"

namespace openstudio {
namespace model {

  class InteriorPartitionSurfaceGroup;
  class DaylightingDeviceShelf;

  namespace detail {

    class InteriorPartitionSurface_Impl;

  }  // namespace detail

  /** InteriorPartitionSurface is a PlanarSurface that wraps the OpenStudio IDD object 'OS_InteriorPartitionSurface'. */
  class MODEL_API InteriorPartitionSurface : public PlanarSurface
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit InteriorPartitionSurface(const std::vector<Point3d>& vertices, const Model& model);

    virtual ~InteriorPartitionSurface() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    InteriorPartitionSurface(const InteriorPartitionSurface& other) = default;
    InteriorPartitionSurface(InteriorPartitionSurface&& other) = default;
    InteriorPartitionSurface& operator=(const InteriorPartitionSurface&) = default;
    InteriorPartitionSurface& operator=(InteriorPartitionSurface&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    bool converttoInternalMass() const;

    bool isConverttoInternalMassDefaulted() const;

    boost::optional<double> surfaceArea() const;

    boost::optional<double> numberofVertices() const;

    bool isNumberofVerticesDefaulted() const;

    bool isNumberofVerticesAutocalculated() const;

    // TODO: Handle this object's extensible fields.

    //@}
    /** @name Setters */
    //@{

    bool setConverttoInternalMass(bool converttoInternalMass);

    void resetConverttoInternalMass();

    bool setSurfaceArea(boost::optional<double> surfaceArea);

    bool setSurfaceArea(double surfaceArea);

    void resetSurfaceArea();

    bool setNumberofVertices(boost::optional<double> numberofVertices);

    bool setNumberofVertices(double numberofVertices);

    void resetNumberofVertices();

    void autocalculateNumberofVertices();

    // TODO: Handle this object's extensible fields.

    //@}

    /// get the interior partition surface group
    boost::optional<InteriorPartitionSurfaceGroup> interiorPartitionSurfaceGroup() const;

    /// set the interior partition surface group
    bool setInteriorPartitionSurfaceGroup(const InteriorPartitionSurfaceGroup& interiorPartitionSurfaceGroup);

    void resetInteriorPartitionSurfaceGroup();

    boost::optional<DaylightingDeviceShelf> daylightingDeviceShelf() const;

   protected:
    /// @cond
    using ImplType = detail::InteriorPartitionSurface_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    explicit InteriorPartitionSurface(std::shared_ptr<detail::InteriorPartitionSurface_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.InteriorPartitionSurface");
  };

  /** \relates InteriorPartitionSurface*/
  using OptionalInteriorPartitionSurface = boost::optional<InteriorPartitionSurface>;

  /** \relates InteriorPartitionSurface*/
  using InteriorPartitionSurfaceVector = std::vector<InteriorPartitionSurface>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_INTERIORPARTITIONSURFACE_HPP
