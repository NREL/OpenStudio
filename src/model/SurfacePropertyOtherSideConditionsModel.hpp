/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef MODEL_SURFACEPROPERTYOTHERSIDECONDITIONSMODEL_HPP
#define MODEL_SURFACEPROPERTYOTHERSIDECONDITIONSMODEL_HPP

#include "ModelAPI.hpp"
#include "ResourceObject.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    class SurfacePropertyOtherSideConditionsModel_Impl;

  }  // namespace detail

  /** SurfacePropertyOtherSideConditionsModel is a ResourceObject that wraps the OpenStudio IDD object 'OS:SurfaceProperty:OtherSideConditionsModel'. */
  class MODEL_API SurfacePropertyOtherSideConditionsModel : public ResourceObject
  {
   public:
    /** @name Constructors and Destructors */
    //@{

    explicit SurfacePropertyOtherSideConditionsModel(const Model& model);

    virtual ~SurfacePropertyOtherSideConditionsModel() override = default;
    // Default the copy and move operators because the virtual dtor is explicit
    SurfacePropertyOtherSideConditionsModel(const SurfacePropertyOtherSideConditionsModel& other) = default;
    SurfacePropertyOtherSideConditionsModel(SurfacePropertyOtherSideConditionsModel&& other) = default;
    SurfacePropertyOtherSideConditionsModel& operator=(const SurfacePropertyOtherSideConditionsModel&) = default;
    SurfacePropertyOtherSideConditionsModel& operator=(SurfacePropertyOtherSideConditionsModel&&) = default;

    //@}

    static IddObjectType iddObjectType();

    static std::vector<std::string> typeOfModelingValues();

    /** @name Getters */
    //@{

    std::string typeOfModeling() const;

    bool isTypeOfModelingDefaulted() const;

    //@}
    /** @name Setters */
    //@{

    bool setTypeOfModeling(const std::string& typeOfModeling);

    void resetTypeOfModeling();

    //@}
    /** @name Other */
    //@{

    //@}
   protected:
    /// @cond
    using ImplType = detail::SurfacePropertyOtherSideConditionsModel_Impl;

    explicit SurfacePropertyOtherSideConditionsModel(std::shared_ptr<detail::SurfacePropertyOtherSideConditionsModel_Impl> impl);

    friend class detail::SurfacePropertyOtherSideConditionsModel_Impl;
    friend class Model;
    friend class IdfObject;
    friend class openstudio::detail::IdfObject_Impl;
    /// @endcond
   private:
    REGISTER_LOGGER("openstudio.model.SurfacePropertyOtherSideConditionsModel");
  };

  /** \relates SurfacePropertyOtherSideConditionsModel*/
  using OptionalSurfacePropertyOtherSideConditionsModel = boost::optional<SurfacePropertyOtherSideConditionsModel>;

  /** \relates SurfacePropertyOtherSideConditionsModel*/
  using SurfacePropertyOtherSideConditionsModelVector = std::vector<SurfacePropertyOtherSideConditionsModel>;

}  // namespace model
}  // namespace openstudio

#endif  // MODEL_SURFACEPROPERTYOTHERSIDECONDITIONSMODEL_HPP
