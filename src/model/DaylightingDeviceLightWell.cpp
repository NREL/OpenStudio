/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#include "DaylightingDeviceLightWell.hpp"
#include "DaylightingDeviceLightWell_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"
#include "Space.hpp"
#include "Space_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "InteriorPartitionSurface.hpp"
#include "InteriorPartitionSurface_Impl.hpp"
#include "ShadingSurface.hpp"
#include "ShadingSurface_Impl.hpp"

#include <utilities/idd/OS_DaylightingDevice_LightWell_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

  namespace detail {

    DaylightingDeviceLightWell_Impl::DaylightingDeviceLightWell_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(idfObject, model, keepHandle) {
      OS_ASSERT(idfObject.iddObject().type() == DaylightingDeviceLightWell::iddObjectType());
    }

    DaylightingDeviceLightWell_Impl::DaylightingDeviceLightWell_Impl(const openstudio::detail::WorkspaceObject_Impl& other, Model_Impl* model,
                                                                     bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {
      OS_ASSERT(other.iddObject().type() == DaylightingDeviceLightWell::iddObjectType());
    }

    DaylightingDeviceLightWell_Impl::DaylightingDeviceLightWell_Impl(const DaylightingDeviceLightWell_Impl& other, Model_Impl* model, bool keepHandle)
      : ModelObject_Impl(other, model, keepHandle) {}

    const std::vector<std::string>& DaylightingDeviceLightWell_Impl::outputVariableNames() const {
      static const std::vector<std::string> result;
      return result;
    }

    IddObjectType DaylightingDeviceLightWell_Impl::iddObjectType() const {
      return DaylightingDeviceLightWell::iddObjectType();
    }

    SubSurface DaylightingDeviceLightWell_Impl::subSurface() const {
      OptionalSubSurface subSurface =
        getObject<ModelObject>().getModelObjectTarget<SubSurface>(OS_DaylightingDevice_LightWellFields::ExteriorWindowName);
      OS_ASSERT(subSurface);
      return subSurface.get();
    }

    double DaylightingDeviceLightWell_Impl::heightofWell() const {
      boost::optional<double> value = getDouble(OS_DaylightingDevice_LightWellFields::HeightofWell, true);
      OS_ASSERT(value);
      return value.get();
    }

    double DaylightingDeviceLightWell_Impl::perimeterofBottomofWell() const {
      boost::optional<double> value = getDouble(OS_DaylightingDevice_LightWellFields::PerimeterofBottomofWell, true);
      OS_ASSERT(value);
      return value.get();
    }

    double DaylightingDeviceLightWell_Impl::areaofBottomofWell() const {
      boost::optional<double> value = getDouble(OS_DaylightingDevice_LightWellFields::AreaofBottomofWell, true);
      OS_ASSERT(value);
      return value.get();
    }

    double DaylightingDeviceLightWell_Impl::visibleReflectanceofWellWalls() const {
      boost::optional<double> value = getDouble(OS_DaylightingDevice_LightWellFields::VisibleReflectanceofWellWalls, true);
      OS_ASSERT(value);
      return value.get();
    }

    bool DaylightingDeviceLightWell_Impl::setHeightofWell(double heightofWell) {
      bool result = setDouble(OS_DaylightingDevice_LightWellFields::HeightofWell, heightofWell);
      OS_ASSERT(result);
      return result;
    }

    bool DaylightingDeviceLightWell_Impl::setPerimeterofBottomofWell(double perimeterofBottomofWell) {
      bool result = setDouble(OS_DaylightingDevice_LightWellFields::PerimeterofBottomofWell, perimeterofBottomofWell);
      OS_ASSERT(result);
      return result;
    }

    bool DaylightingDeviceLightWell_Impl::setAreaofBottomofWell(double areaofBottomofWell) {
      bool result = setDouble(OS_DaylightingDevice_LightWellFields::AreaofBottomofWell, areaofBottomofWell);
      OS_ASSERT(result);
      return result;
    }

    bool DaylightingDeviceLightWell_Impl::setVisibleReflectanceofWellWalls(double visibleReflectanceofWellWalls) {
      bool result = setDouble(OS_DaylightingDevice_LightWellFields::VisibleReflectanceofWellWalls, visibleReflectanceofWellWalls);
      OS_ASSERT(result);
      return result;
    }

  }  // namespace detail

  DaylightingDeviceLightWell::DaylightingDeviceLightWell(const SubSurface& subSurface, double heightofWell, double perimeterofBottomofWell,
                                                         double areaofBottomofWell, double visibleReflectanceofWellWalls)
    : ModelObject(DaylightingDeviceLightWell::iddObjectType(), subSurface.model()) {
    OS_ASSERT(getImpl<detail::DaylightingDeviceLightWell_Impl>());

    bool subSurfaceOk = false;
    if (subSurface.allowDaylightingDeviceLightWell()) {
      if (!subSurface.daylightingDeviceLightWell()) {
        subSurfaceOk = true;
      }
    }

    if (!subSurfaceOk) {
      this->remove();
      LOG_AND_THROW("Cannot create DaylightingDeviceLightWell for SubSurface '" << subSurface.name().get() << "'");
    }

    bool test = setPointer(OS_DaylightingDevice_LightWellFields::ExteriorWindowName, subSurface.handle());
    OS_ASSERT(test);

    setHeightofWell(heightofWell);

    setPerimeterofBottomofWell(perimeterofBottomofWell);

    setAreaofBottomofWell(areaofBottomofWell);

    setVisibleReflectanceofWellWalls(visibleReflectanceofWellWalls);
  }

  DaylightingDeviceLightWell::DaylightingDeviceLightWell(const SubSurface& subSurface)
    : ModelObject(DaylightingDeviceLightWell::iddObjectType(), subSurface.model()) {
    OS_ASSERT(getImpl<detail::DaylightingDeviceLightWell_Impl>());

    bool subSurfaceOk = false;
    if (subSurface.allowDaylightingDeviceLightWell()) {
      if (!subSurface.daylightingDeviceLightWell()) {
        subSurfaceOk = true;
      }
    }

    if (!subSurfaceOk) {
      this->remove();
      LOG_AND_THROW("Cannot create DaylightingDeviceLightWell for SubSurface '" << subSurface.name().get() << "'");
    }

    bool test = setPointer(OS_DaylightingDevice_LightWellFields::ExteriorWindowName, subSurface.handle());
    OS_ASSERT(test);

    setHeightofWell(1.2);  // GeometryTest.idf

    setPerimeterofBottomofWell(12.0);  // GeometryTest.idf

    setAreaofBottomofWell(9.0);  // GeometryTest.idf

    setVisibleReflectanceofWellWalls(0.7);  // GeometryTest.idf
  }

  IddObjectType DaylightingDeviceLightWell::iddObjectType() {
    IddObjectType result(IddObjectType::OS_DaylightingDevice_LightWell);
    return result;
  }

  SubSurface DaylightingDeviceLightWell::subSurface() const {
    return getImpl<detail::DaylightingDeviceLightWell_Impl>()->subSurface();
  }

  double DaylightingDeviceLightWell::heightofWell() const {
    return getImpl<detail::DaylightingDeviceLightWell_Impl>()->heightofWell();
  }

  double DaylightingDeviceLightWell::perimeterofBottomofWell() const {
    return getImpl<detail::DaylightingDeviceLightWell_Impl>()->perimeterofBottomofWell();
  }

  double DaylightingDeviceLightWell::areaofBottomofWell() const {
    return getImpl<detail::DaylightingDeviceLightWell_Impl>()->areaofBottomofWell();
  }

  double DaylightingDeviceLightWell::visibleReflectanceofWellWalls() const {
    return getImpl<detail::DaylightingDeviceLightWell_Impl>()->visibleReflectanceofWellWalls();
  }

  bool DaylightingDeviceLightWell::setHeightofWell(double heightofWell) {
    return getImpl<detail::DaylightingDeviceLightWell_Impl>()->setHeightofWell(heightofWell);
  }

  bool DaylightingDeviceLightWell::setPerimeterofBottomofWell(double perimeterofBottomofWell) {
    return getImpl<detail::DaylightingDeviceLightWell_Impl>()->setPerimeterofBottomofWell(perimeterofBottomofWell);
  }

  bool DaylightingDeviceLightWell::setAreaofBottomofWell(double areaofBottomofWell) {
    return getImpl<detail::DaylightingDeviceLightWell_Impl>()->setAreaofBottomofWell(areaofBottomofWell);
  }

  bool DaylightingDeviceLightWell::setVisibleReflectanceofWellWalls(double visibleReflectanceofWellWalls) {
    return getImpl<detail::DaylightingDeviceLightWell_Impl>()->setVisibleReflectanceofWellWalls(visibleReflectanceofWellWalls);
  }

  /// @cond
  DaylightingDeviceLightWell::DaylightingDeviceLightWell(std::shared_ptr<detail::DaylightingDeviceLightWell_Impl> impl)
    : ModelObject(std::move(impl)) {}
  /// @endcond

}  // namespace model
}  // namespace openstudio
