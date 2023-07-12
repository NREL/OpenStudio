/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#include "SkyTemperature.hpp"
#include "SkyTemperature_Impl.hpp"
#include "Site.hpp"
#include "Site_Impl.hpp"
#include "RunPeriod.hpp"
#include "RunPeriod_Impl.hpp"
#include "SizingPeriod.hpp"
#include "SizingPeriod_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include <utilities/idd/OS_WeatherProperty_SkyTemperature_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    SkyTemperature_Impl::SkyTemperature_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == SkyTemperature::iddObjectType());
    }

    SkyTemperature_Impl::SkyTemperature_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == SkyTemperature::iddObjectType());
    }

    SkyTemperature_Impl::SkyTemperature_Impl(const SkyTemperature_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    // return the parent object in the hierarchy
    boost::optional<ParentObject> SkyTemperature_Impl::parent() const {
      // if Name field not blank, that is the parent
      OptionalParentObject result = getObject<ModelObject>().getModelObjectTarget<ParentObject>(OS_WeatherProperty_SkyTemperatureFields::Name);
      // if it is blank, parent is Site
      if (!result) {
        result = OptionalParentObject(model().getOptionalUniqueModelObject<Site>());
      }
      return result;
    }

    // set the parent, child may have to call methods on the parent
    bool SkyTemperature_Impl::setParent(ParentObject& newParent) {
      if (!(newParent.model() == model())) {
        return false;
      }

      if (newParent.optionalCast<Site>()) {
        // nullify the field
        return setPointer(OS_WeatherProperty_SkyTemperatureFields::Name, Handle());
      } else if (newParent.optionalCast<RunPeriod>() || newParent.optionalCast<SizingPeriod>()) {
        return setPointer(OS_WeatherProperty_SkyTemperatureFields::Name, newParent.handle());
      }

      return false;
    }

    // Get all output variable names that could be associated with this object.
    const std::vector<std::string>& SkyTemperature_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType SkyTemperature_Impl::iddObjectType() const {
      return SkyTemperature::iddObjectType();
    }

  }  // namespace detail

  /// constructor
  SkyTemperature::SkyTemperature(const Model& model) : ModelObject(SkyTemperature::iddObjectType(), model) {
    OS_ASSERT(getImpl<detail::SkyTemperature_Impl>());
  }

  // constructor
  SkyTemperature::SkyTemperature(std::shared_ptr<detail::SkyTemperature_Impl> impl) : ModelObject(std::move(impl)) {}

  IddObjectType SkyTemperature::iddObjectType() {
    IddObjectType result(IddObjectType::OS_WeatherProperty_SkyTemperature);
    return result;
  }

}  // namespace model
}  // namespace openstudio
