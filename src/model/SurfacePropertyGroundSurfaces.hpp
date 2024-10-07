/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SURFACEPROPERTYGROUNDSURFACES_HPP
#define MODEL_SURFACEPROPERTYGROUNDSURFACES_HPP

#include <model/ModelAPI.hpp>
#include "ModelObject.hpp"
#include "Schedule.hpp"

namespace openstudio {
namespace model {

  class SurfacePropertyLocalEnvironment;

  namespace detail {

    class SurfacePropertyGroundSurfaces_Impl;

  }  // namespace detail

  /** This class implements an extensible group */
  class MODEL_API GroundSurfaceGroup
  {
   public:
    GroundSurfaceGroup(std::string groundSurfaceName, double viewFactor, boost::optional<Schedule> temperatureSchedule = boost::none,
                       boost::optional<Schedule> reflectanceSchedule = boost::none);

    std::string groundSurfaceName() const;
    double viewFactor() const;
    boost::optional<Schedule> temperatureSchedule() const;

    boost::optional<Schedule> reflectanceSchedule() const;

   private:
    // From
    std::string m_groundSurfaceName;
    double m_viewFactor;
    boost::optional<Schedule> m_temperatureSch;
    boost::optional<Schedule> m_reflectanceSch;
    REGISTER_LOGGER("openstudio.model.GroundSurfaceGroup");
  };

  // Overload operator<<
  MODEL_API std::ostream& operator<<(std::ostream& out, const openstudio::model::GroundSurfaceGroup& groundSurfaceGroup);

  /** SurfacePropertyGroundSurfaces is a ModelObject that wraps the OpenStudio IDD object 'OS:SurfaceProperty:GroundSurfaces'. */
  class MODEL_API SurfacePropertyGroundSurfaces : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SurfacePropertyGroundSurfaces(const Model& model);

    virtual ~SurfacePropertyGroundSurfaces() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SurfacePropertyGroundSurfaces(const SurfacePropertyGroundSurfaces& other) = default;
    SurfacePropertyGroundSurfaces(SurfacePropertyGroundSurfaces&& other) = default;
    SurfacePropertyGroundSurfaces& operator=(const SurfacePropertyGroundSurfaces&) = default;
    SurfacePropertyGroundSurfaces& operator=(SurfacePropertyGroundSurfaces&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    //@}
    /** @name Setters */
    //@{

    //@}
    /** @name Other */
    //@{

    // Helper to get the parent SurfaceProperty:LocalEnvironment object
    boost::optional<SurfacePropertyLocalEnvironment> surfacePropertyLocalEnvironment() const;

    // Handle this object's extensible fields.
    std::vector<GroundSurfaceGroup> groundSurfaceGroups() const;

    unsigned int numberofGroundSurfaceGroups() const;

    /** Looks up by Surface Name (case-insensitive)  */
    boost::optional<unsigned> groundSurfaceGroupIndex(const GroundSurfaceGroup& groundSurfaceGroup) const;
    boost::optional<unsigned> groundSurfaceGroupIndex(const std::string& groundSurfaceName) const;

    boost::optional<GroundSurfaceGroup> getGroundSurfaceGroup(unsigned groupIndex) const;

    /** If a groundSurfaceGroup group is already present (cf `groundSurfaceGroupIndex()`), it will Warn and override the groundSurfaceGroup value */
    bool addGroundSurfaceGroup(const GroundSurfaceGroup& groundSurfaceGroup);

    // Overloads, it creates a GroundSurfaceGroup wrapper, then call `addGroundSurfaceGroup(const GroundSurfaceGroup& groundSurfaceGroup)`
    bool addGroundSurfaceGroup(const std::string& groundSurfaceName, double viewFactor, boost::optional<Schedule> temperatureSchedule = boost::none,
                               boost::optional<Schedule> reflectanceSchedule = boost::none);

    bool addGroundSurfaceGroups(const std::vector<GroundSurfaceGroup>& groundSurfaceGroups);

    bool removeGroundSurfaceGroup(unsigned groupIndex);

    void removeAllGroundSurfaceGroups();

    //@}
   protected:
    /// @cond
    using ImplType = detail::SurfacePropertyGroundSurfaces_Impl;

    explicit SurfacePropertyGroundSurfaces(std::shared_ptr<detail::SurfacePropertyGroundSurfaces_Impl> impl);

    friend class detail::SurfacePropertyGroundSurfaces_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SurfacePropertyGroundSurfaces");
  };

  /** \relates SurfacePropertyGroundSurfaces*/
  using OptionalSurfacePropertyGroundSurfaces = boost::optional<SurfacePropertyGroundSurfaces>;

  /** \relates SurfacePropertyGroundSurfaces*/
  using SurfacePropertyGroundSurfacesVector = std::vector<SurfacePropertyGroundSurfaces>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYGROUNDSURFACES_HPP
