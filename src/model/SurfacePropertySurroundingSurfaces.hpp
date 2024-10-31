/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SURFACEPROPERTYSURROUNDINGSURFACES_HPP
#define MODEL_SURFACEPROPERTYSURROUNDINGSURFACES_HPP

#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "Schedule.hpp"

namespace openstudio {

namespace model {

  class SurfacePropertyLocalEnvironment;

  namespace detail {

    class SurfacePropertySurroundingSurfaces_Impl;

  }  // namespace detail

  /** This class implements an extensible group */
  class MODEL_API SurroundingSurfaceGroup
  {
   public:
    SurroundingSurfaceGroup(std::string surroundingSurfaceName, double viewFactor, const Schedule& temperatureSchedule);

    std::string surroundingSurfaceName() const;
    double viewFactor() const;
    Schedule temperatureSchedule() const;

   private:
    // From
    std::string m_surroundingSurfaceName;
    double m_viewFactor;
    Schedule m_tempSch;
    REGISTER_LOGGER("openstudio.model.SurroundingSurfaceGroup");
  };

  // Overload operator<<
  MODEL_API std::ostream& operator<<(std::ostream& out, const openstudio::model::SurroundingSurfaceGroup& surroundingSurfaceGroup);

  /** SurfacePropertySurroundingSurfaces is a ModelObject that wraps the OpenStudio IDD object 'OS:SurfaceProperty:SurroundingSurfaces'. */
  class MODEL_API SurfacePropertySurroundingSurfaces : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SurfacePropertySurroundingSurfaces(const Model& model);

    virtual ~SurfacePropertySurroundingSurfaces() override = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    boost::optional<double> skyViewFactor() const;

    bool isSkyViewFactorAutocalculated() const;

    boost::optional<Schedule> skyTemperatureSchedule() const;

    boost::optional<double> groundViewFactor() const;

    bool isGroundViewFactorAutocalculated() const;

    boost::optional<Schedule> groundTemperatureSchedule() const;

    //@}
    /** @name Setters */
    //@{

    bool setSkyViewFactor(double skyViewFactor);
    void autocalculateSkyViewFactor();

    bool setSkyTemperatureSchedule(Schedule& schedule);
    void resetSkyTemperatureSchedule();

    bool setGroundViewFactor(double groundViewFactor);
    void autocalculateGroundViewFactor();

    bool setGroundTemperatureSchedule(Schedule& schedule);
    void resetGroundTemperatureSchedule();

    //@}
    /** @name Other */
    //@{

    // Helper to get the parent SurfaceProperty:LocalEnvironment object
    boost::optional<SurfacePropertyLocalEnvironment> surfacePropertyLocalEnvironment() const;

    // Handle this object's extensible fields.
    std::vector<SurroundingSurfaceGroup> surroundingSurfaceGroups() const;

    unsigned int numberofSurroundingSurfaceGroups() const;

    /** Looks up by Surface Name (case-insensitive)  */
    boost::optional<unsigned> surroundingSurfaceGroupIndex(const SurroundingSurfaceGroup& surroundingSurfaceGroup) const;
    boost::optional<unsigned> surroundingSurfaceGroupIndex(const std::string& surroundingSurfaceName) const;

    boost::optional<SurroundingSurfaceGroup> getSurroundingSurfaceGroup(unsigned groupIndex) const;

    /** If a surroundingSurfaceGroup group is already present (cf `surroundingSurfaceGroupIndex()`), it will Warn and override the surroundingSurfaceGroup value */
    bool addSurroundingSurfaceGroup(const SurroundingSurfaceGroup& surroundingSurfaceGroup);

    // Overloads, it creates a SurroundingSurfaceGroup wrapper, then call `addSurroundingSurfaceGroup(const SurroundingSurfaceGroup& surroundingSurfaceGroup)`
    bool addSurroundingSurfaceGroup(const std::string& surroundingSurfaceName, double viewFactor, const Schedule& temperatureSchedule);

    bool addSurroundingSurfaceGroups(const std::vector<SurroundingSurfaceGroup>& surroundingSurfaceGroups);

    bool removeSurroundingSurfaceGroup(unsigned groupIndex);

    void removeAllSurroundingSurfaceGroups();

    //@}
   protected:
    /// @cond
    using ImplType = detail::SurfacePropertySurroundingSurfaces_Impl;

    explicit SurfacePropertySurroundingSurfaces(std::shared_ptr<detail::SurfacePropertySurroundingSurfaces_Impl> impl);

    friend class detail::SurfacePropertySurroundingSurfaces_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SurfacePropertySurroundingSurfaces");
  };

  /** \relates SurfacePropertySurroundingSurfaces*/
  using OptionalSurfacePropertySurroundingSurfaces = boost::optional<SurfacePropertySurroundingSurfaces>;

  /** \relates SurfacePropertySurroundingSurfaces*/
  using SurfacePropertySurroundingSurfacesVector = std::vector<SurfacePropertySurroundingSurfaces>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYSURROUNDINGSURFACES_HPP
