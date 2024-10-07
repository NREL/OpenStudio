/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SURFACEPROPERTYEXPOSEDFOUNDATIONPERIMETER_HPP
#define MODEL_SURFACEPROPERTYEXPOSEDFOUNDATIONPERIMETER_HPP

#include <vector>
#include "ModelAPI.hpp"
#include "ModelObject.hpp"
#include "../utilities/core/Deprecated.hpp"

namespace openstudio {
namespace model {

  class Surface;

  namespace detail {

    class SurfacePropertyExposedFoundationPerimeter_Impl;

  }  // namespace detail

  /** SurfacePropertyExposedFoundationPerimeter is a ModelObject that wraps the OpenStudio IDD object 'OS:SurfaceProperty:ExposedFoundationPerimeter'. */
  class MODEL_API SurfacePropertyExposedFoundationPerimeter : public ModelObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SurfacePropertyExposedFoundationPerimeter(Surface& surface, std::string exposedPerimeterCalculationMethod, double exposedPerimeter);

    virtual ~SurfacePropertyExposedFoundationPerimeter() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SurfacePropertyExposedFoundationPerimeter(const SurfacePropertyExposedFoundationPerimeter& other) = default;
    SurfacePropertyExposedFoundationPerimeter(SurfacePropertyExposedFoundationPerimeter&& other) = default;
    SurfacePropertyExposedFoundationPerimeter& operator=(const SurfacePropertyExposedFoundationPerimeter&) = default;
    SurfacePropertyExposedFoundationPerimeter& operator=(SurfacePropertyExposedFoundationPerimeter&&) = default;

    //@}

    static IddObjectType iddObjectType();

    /** @name Getters */
    //@{

    OS_DEPRECATED(3, 7, 0) std::string surfaceName() const;

    Surface surface() const;

    std::string exposedPerimeterCalculationMethod() const;

    boost::optional<double> totalExposedPerimeter();

    double exposedPerimeterFraction() const;

    bool isExposedPerimeterFractionDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setExposedPerimeterCalculationMethod(const std::string& exposedPerimeterCalculationMethod);

    bool setTotalExposedPerimeter(double totalExposedPerimeter);

    bool setExposedPerimeterFraction(double exposedPerimeterFraction);

    void resetExposedPerimeterFraction();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SurfacePropertyExposedFoundationPerimeter_Impl;

    explicit SurfacePropertyExposedFoundationPerimeter(std::shared_ptr<detail::SurfacePropertyExposedFoundationPerimeter_Impl> impl);

    friend class detail::SurfacePropertyExposedFoundationPerimeter_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SurfacePropertyExposedFoundationPerimeter");
  };

  /** \relates SurfacePropertyExposedFoundationPerimeter*/
  using OptionalSurfacePropertyExposedFoundationPerimeter = boost::optional<SurfacePropertyExposedFoundationPerimeter>;

  /** \relates SurfacePropertyExposedFoundationPerimeter*/
  using SurfacePropertyExposedFoundationPerimeterVector = std::vector<SurfacePropertyExposedFoundationPerimeter>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYEXPOSEDFOUNDATIONPERIMETER_HPP
