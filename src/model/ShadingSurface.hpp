/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SHADINGSURFACE_HPP
#define MODEL_SHADINGSURFACE_HPP

#include "ModelAPI.hpp"
#include "PlanarSurface.hpp"

namespace openstudio {
namespace model {

  class ShadingSurfaceGroup;
  class Schedule;
  class DaylightingDeviceShelf;

  namespace detail {

    class ShadingSurface_Impl;

  }  // namespace detail

  /** ShadingSurface is a PlanarSurface that wraps the OpenStudio IDD object
 *  'OS:ShadingSurface'. */
  class MODEL_API ShadingSurface : public PlanarSurface
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit ShadingSurface(const std::vector<Point3d>& vertices, const Model& model);

    virtual ~ShadingSurface() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    ShadingSurface(const ShadingSurface& other) = default;
    ShadingSurface(ShadingSurface&& other) = default;
    ShadingSurface& operator=(const ShadingSurface&) = default;
    ShadingSurface& operator=(ShadingSurface&&) = default;

    //@}
    /** @name Static Methods */
    //@{

    static IddObjectType iddObjectType();

    //@}
    /** @name Getters */
    //@{

    /// get the shading group
    boost::optional<ShadingSurfaceGroup> shadingSurfaceGroup() const;

    // get the transmittance schedule
    boost::optional<Schedule> transmittanceSchedule() const;

    boost::optional<double> numberofVertices() const;

    bool isNumberofVerticesDefaulted() const;

    bool isNumberofVerticesAutocalculated() const;

    //@}
    /** @name Setters */
    //@{

    /// set the exterior shading group
    bool setShadingSurfaceGroup(const ShadingSurfaceGroup& shadingSurfaceGroup);

    void resetShadingSurfaceGroup();

    /// set the transmittance schedule
    bool setTransmittanceSchedule(Schedule& transmittanceSchedule);

    /// reset the transmittance schedule
    void resetTransmittanceSchedule();

    bool setNumberofVertices(boost::optional<double> numberofVertices);

    bool setNumberofVertices(double numberofVertices);

    void resetNumberofVertices();

    void autocalculateNumberofVertices();

    //@}

    boost::optional<DaylightingDeviceShelf> daylightingDeviceShelf() const;

   protected:
    /// @cond
    using ImplType = detail::ShadingSurface_Impl;

    friend class Model;
    friend class openstudio::IdfObject;
    friend class openstudio::detail::IdfObject_Impl;

    explicit ShadingSurface(std::shared_ptr<detail::ShadingSurface_Impl> impl);

    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.ShadingSurface");
  };

  /** \relates ShadingSurface*/
  using OptionalShadingSurface = boost::optional<ShadingSurface>;

  /** \relates ShadingSurface*/
  using ShadingSurfaceVector = std::vector<ShadingSurface>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SHADINGSURFACE_HPP
