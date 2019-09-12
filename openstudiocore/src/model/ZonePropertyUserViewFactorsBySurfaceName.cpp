/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#include <vector>
#include <string>
#include "ZonePropertyUserViewFactorsBySurfaceName.hpp"
#include "ZonePropertyUserViewFactorsBySurfaceName_Impl.hpp"
#include "ThermalZone.hpp"
#include "ThermalZone_Impl.hpp"
#include "Surface.hpp"
#include "Surface_Impl.hpp"
#include "SubSurface.hpp"
#include "SubSurface_Impl.hpp"
#include "InternalMass.hpp"
#include "InternalMass_Impl.hpp"

#include "Model.hpp"
#include "Model_Impl.hpp"

#include "../utilities/idf/WorkspaceExtensibleGroup.hpp"

#include <utilities/idd/IddEnums.hxx>
#include <utilities/idd/OS_ZoneProperty_UserViewFactors_BySurfaceName_FieldEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {


/*****************************************************************************************************************************************************
*                                                      V I E W    F A C T O R    W R A P P E R                                                      *
*****************************************************************************************************************************************************/


ViewFactor::ViewFactor(const Surface& fromSurface, const Surface& toSurface, double viewFactor)
  : m_from_surface(fromSurface), m_to_surface(toSurface), m_view_factor(viewFactor) {

  if (m_view_factor > 1) {
    LOG_AND_THROW("Unable to create view factor, factor of " << m_view_factor << " more than 1");
  }
}

ViewFactor::ViewFactor(const Surface& fromSurface, const SubSurface& toSubSurface, double viewFactor)
  : m_from_surface(fromSurface), m_to_sub_surface(toSubSurface), m_view_factor(viewFactor) {

  if (m_view_factor > 1) {
    LOG_AND_THROW("Unable to create view factor, factor of " << m_view_factor << " more than 1");
  }
}

ViewFactor::ViewFactor(const Surface& fromSurface, const InternalMass& toInternalMass, double viewFactor)
  : m_from_surface(fromSurface), m_to_internal_mass(toInternalMass), m_view_factor(viewFactor) {

  if (m_view_factor > 1) {
    LOG_AND_THROW("Unable to create view factor, factor of " << m_view_factor << " more than 1");
  }
}

ViewFactor::ViewFactor(const SubSurface& fromSubSurface, const SubSurface& toSubSurface, double viewFactor)
  : m_from_sub_surface(fromSubSurface), m_to_sub_surface(toSubSurface), m_view_factor(viewFactor) {

  if (m_view_factor > 1) {
    LOG_AND_THROW("Unable to create view factor, factor of " << m_view_factor << " more than 1");
  }
}

ViewFactor::ViewFactor(const SubSurface& fromSubSurface, const Surface& toSurface, double viewFactor)
  : m_from_sub_surface(fromSubSurface), m_to_surface(toSurface), m_view_factor(viewFactor) {

  if (m_view_factor > 1) {
    LOG_AND_THROW("Unable to create view factor, factor of " << m_view_factor << " more than 1");
  }
}

ViewFactor::ViewFactor(const SubSurface& fromSubSurface, const InternalMass& toInternalMass, double viewFactor)
  : m_from_sub_surface(fromSubSurface), m_to_internal_mass(toInternalMass), m_view_factor(viewFactor) {

  if (m_view_factor > 1) {
    LOG_AND_THROW("Unable to create view factor, factor of " << m_view_factor << " more than 1");
  }
}

ViewFactor::ViewFactor(const InternalMass& fromInternalMass, const InternalMass& toInternalMass, double viewFactor)
  : m_from_internal_mass(fromInternalMass), m_to_internal_mass(toInternalMass), m_view_factor(viewFactor) {

  if (m_view_factor > 1) {
    LOG_AND_THROW("Unable to create view factor, factor of " << m_view_factor << " more than 1");
  }
}

ViewFactor::ViewFactor(const InternalMass& fromInternalMass, const Surface& toSurface, double viewFactor)
  : m_from_internal_mass(fromInternalMass), m_to_surface(toSurface), m_view_factor(viewFactor) {

  if (m_view_factor > 1) {
    LOG_AND_THROW("Unable to create view factor, factor of " << m_view_factor << " more than 1");
  }
}

ViewFactor::ViewFactor(const InternalMass& fromInternalMass, const SubSurface& toSubSurface, double viewFactor)
  : m_from_internal_mass(fromInternalMass), m_to_sub_surface(toSubSurface), m_view_factor(viewFactor) {

  if (m_view_factor > 1) {
    LOG_AND_THROW("Unable to create view factor, factor of " << m_view_factor << " more than 1");
  }
}

boost::optional<Surface> ViewFactor::fromSurface() const {
  return m_from_surface;
}

boost::optional<Surface> ViewFactor::toSurface() const {
  return m_to_surface;
}

boost::optional<SubSurface> ViewFactor::fromSubSurface() const {
  return m_from_sub_surface;
}

boost::optional<SubSurface> ViewFactor::toSubSurface() const {
  return m_to_sub_surface;
}

boost::optional<InternalMass> ViewFactor::fromInternalMass() const {
  return m_from_internal_mass;
}

boost::optional<InternalMass> ViewFactor::toInternalMass() const {
  return m_to_internal_mass;
}

double ViewFactor::viewFactor() const {
  return m_view_factor;
}

std::ostream& operator<< (std::ostream& out, const openstudio::model::ViewFactor& viewFactor) {
  std::string from;
  std::string to;
  if (viewFactor.fromSurface()) {
    from = viewFactor.fromSurface().get().nameString();
  } else if (viewFactor.fromSubSurface()) {
    from = viewFactor.fromSubSurface().get().nameString();
  } else if (viewFactor.fromInternalMass()) {
    from = viewFactor.fromInternalMass().get().nameString();
  }
  if (viewFactor.toSurface()) {
    to = viewFactor.toSurface().get().nameString();
  } else if (viewFactor.toSubSurface()) {
    to = viewFactor.toSubSurface().get().nameString();
  } else if (viewFactor.toInternalMass()) {
    to = viewFactor.toInternalMass().get().nameString();
  }
  out << "(from " << from << ", " << "to " << to << ", view factor " << viewFactor.viewFactor() << ")";
  return out;
}

/*****************************************************************************************************************************************************
*                                  Z O N E P R O P E R T Y U S E R V I E W F A C T O R S B Y S U R F A C E N A M E                                  *
*****************************************************************************************************************************************************/

namespace detail {

  ZonePropertyUserViewFactorsBySurfaceName_Impl::ZonePropertyUserViewFactorsBySurfaceName_Impl(const IdfObject& idfObject,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ZonePropertyUserViewFactorsBySurfaceName::iddObjectType());
  }

  ZonePropertyUserViewFactorsBySurfaceName_Impl::ZonePropertyUserViewFactorsBySurfaceName_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ZonePropertyUserViewFactorsBySurfaceName::iddObjectType());
  }

  ZonePropertyUserViewFactorsBySurfaceName_Impl::ZonePropertyUserViewFactorsBySurfaceName_Impl(const ZonePropertyUserViewFactorsBySurfaceName_Impl& other,
                                                                                               Model_Impl* model,
                                                                                               bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ZonePropertyUserViewFactorsBySurfaceName_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    return result;
  }

  IddObjectType ZonePropertyUserViewFactorsBySurfaceName_Impl::iddObjectType() const {
    return ZonePropertyUserViewFactorsBySurfaceName::iddObjectType();
  }

  ThermalZone ZonePropertyUserViewFactorsBySurfaceName_Impl::thermalZone() const
  {
    boost::optional<ThermalZone> thermalZone = getObject<ModelObject>().getModelObjectTarget<ThermalZone>(OS_ZoneProperty_UserViewFactors_BySurfaceNameFields::ThermalZoneName);
    OS_ASSERT(thermalZone);
    return thermalZone.get();
  }

  unsigned int ZonePropertyUserViewFactorsBySurfaceName_Impl::numberofViewFactors() const {
    return numExtensibleGroups();
  }

  bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const ViewFactor& viewFactor) {
    bool result = false;
    bool from = false;
    bool to = false;

    // Push an extensible group
    WorkspaceExtensibleGroup eg = getObject<ModelObject>().pushExtensibleGroup().cast<WorkspaceExtensibleGroup>();
    if (viewFactor.fromSurface()) {
      from = eg.setPointer(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::FromSurfaceName, viewFactor.fromSurface().get().handle());
    } else if (viewFactor.fromSubSurface()) {
      from = eg.setPointer(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::FromSurfaceName, viewFactor.fromSubSurface().get().handle());
    } else if (viewFactor.fromInternalMass()) {
      from = eg.setPointer(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::FromSurfaceName, viewFactor.fromInternalMass().get().handle());
    } else {
      LOG(Error, "Unable to add View Factor which has an incompatible fromSurface object to " << briefDescription());
      OS_ASSERT(false);
    }

    if (viewFactor.toSurface()) {
      to = eg.setPointer(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ToSurfaceName, viewFactor.toSurface().get().handle());
    } else if (viewFactor.toSubSurface()) {
      to = eg.setPointer(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ToSurfaceName, viewFactor.toSubSurface().get().handle());
    } else if (viewFactor.toInternalMass()) {
      to = eg.setPointer(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ToSurfaceName, viewFactor.toInternalMass().get().handle());
    } else {
      LOG(Error, "Unable to add View Factor which has an incompatible toSurface object to " << briefDescription());
      OS_ASSERT(false);
    }

    bool value = eg.setDouble(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ViewFactor, viewFactor.viewFactor());
    if (from && to && value) {
      result = true;
    } else {
      // Something went wrong
      // So erase the new extensible group
      getObject<ModelObject>().eraseExtensibleGroup(eg.groupIndex());
      result = false;
    }
    return result;
  }

  bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const Surface& fromSurface, const Surface& toSurface, double value) {
    ViewFactor viewFactor(fromSurface, toSurface, value);
    return addViewFactor(viewFactor);
  }

  bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const Surface& fromSurface, const SubSurface& toSubSurface, double value) {
    ViewFactor viewFactor(fromSurface, toSubSurface, value);
    return addViewFactor(viewFactor);
  }

  bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const Surface& fromSurface, const InternalMass& toInternalMass, double value) {
    ViewFactor viewFactor(fromSurface, toInternalMass, value);
    return addViewFactor(viewFactor);
  }

  bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const SubSurface& fromSubSurface, const SubSurface& toSubSurface, double value) {
    ViewFactor viewFactor(fromSubSurface, toSubSurface, value);
    return addViewFactor(viewFactor);
  }

  bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const SubSurface& fromSubSurface, const Surface& toSurface, double value) {
    ViewFactor viewFactor(fromSubSurface, toSurface, value);
    return addViewFactor(viewFactor);
  }

  bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const SubSurface& fromSubSurface, const InternalMass& toInternalMass, double value) {
    ViewFactor viewFactor(fromSubSurface, toInternalMass, value);
    return addViewFactor(viewFactor);
  }

  bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const InternalMass& fromInternalMass, const InternalMass& toSubSurface, double value) {
    ViewFactor viewFactor(fromInternalMass, toSubSurface, value);
    return addViewFactor(viewFactor);
  }

  bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const InternalMass& fromInternalMass, const Surface& toSurface, double value) {
    ViewFactor viewFactor(fromInternalMass, toSurface, value);
    return addViewFactor(viewFactor);
  }

  bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactor(const InternalMass& fromInternalMass, const SubSurface& toSubSurface, double value) {
    ViewFactor viewFactor(fromInternalMass, toSubSurface, value);
    return addViewFactor(viewFactor);
  }

  bool ZonePropertyUserViewFactorsBySurfaceName_Impl::removeViewFactor(unsigned groupIndex) {
    bool result = false;

    unsigned int num = numberofViewFactors();
    if (groupIndex < num) {
      getObject<ModelObject>().eraseExtensibleGroup(groupIndex);
      result = true;
    }
    return result;
  }

  void ZonePropertyUserViewFactorsBySurfaceName_Impl::removeAllViewFactors() {
    getObject<ModelObject>().clearExtensibleGroups();
  }

  std::vector<ViewFactor> ZonePropertyUserViewFactorsBySurfaceName_Impl::viewFactors() const {
    std::vector<ViewFactor> result;

    std::vector<IdfExtensibleGroup> groups = extensibleGroups();

    for (const auto & group : groups) {
      boost::optional<WorkspaceObject> wo1 = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::FromSurfaceName);
      boost::optional<WorkspaceObject> wo2 = group.cast<WorkspaceExtensibleGroup>().getTarget(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ToSurfaceName);
      boost::optional<double> value = group.cast<WorkspaceExtensibleGroup>().getDouble(OS_ZoneProperty_UserViewFactors_BySurfaceNameExtensibleFields::ViewFactor);

      if (wo1->optionalCast<Surface>() && wo2->optionalCast<Surface>() && value) {
        ViewFactor viewFactor(wo1->optionalCast<Surface>().get(), wo2->optionalCast<Surface>().get(), *value);
        result.push_back(viewFactor);
      } else if (wo1->optionalCast<Surface>() && wo2->optionalCast<SubSurface>() && value) {
        ViewFactor viewFactor(wo1->optionalCast<Surface>().get(), wo2->optionalCast<SubSurface>().get(), *value);
        result.push_back(viewFactor);
      } else if (wo1->optionalCast<Surface>() && wo2->optionalCast<InternalMass>() && value) {
        ViewFactor viewFactor(wo1->optionalCast<Surface>().get(), wo2->optionalCast<InternalMass>().get(), *value);
        result.push_back(viewFactor);
      } else if (wo1->optionalCast<SubSurface>() && wo2->optionalCast<SubSurface>() && value) {
        ViewFactor viewFactor(wo1->optionalCast<SubSurface>().get(), wo2->optionalCast<SubSurface>().get(), *value);
        result.push_back(viewFactor);
      } else if (wo1->optionalCast<SubSurface>() && wo2->optionalCast<Surface>() && value) {
        ViewFactor viewFactor(wo1->optionalCast<SubSurface>().get(), wo2->optionalCast<Surface>().get(), *value);
        result.push_back(viewFactor);
      } else if (wo1->optionalCast<SubSurface>() && wo2->optionalCast<InternalMass>() && value) {
        ViewFactor viewFactor(wo1->optionalCast<SubSurface>().get(), wo2->optionalCast<InternalMass>().get(), *value);
        result.push_back(viewFactor);
      } else if (wo1->optionalCast<InternalMass>() && wo2->optionalCast<InternalMass>() && value) {
        ViewFactor viewFactor(wo1->optionalCast<InternalMass>().get(), wo2->optionalCast<InternalMass>().get(), *value);
        result.push_back(viewFactor);
      } else if (wo1->optionalCast<InternalMass>() && wo2->optionalCast<Surface>() && value) {
        ViewFactor viewFactor(wo1->optionalCast<InternalMass>().get(), wo2->optionalCast<Surface>().get(), *value);
        result.push_back(viewFactor);
      } else if (wo1->optionalCast<InternalMass>() && wo2->optionalCast<SubSurface>() && value) {
        ViewFactor viewFactor(wo1->optionalCast<InternalMass>().get(), wo2->optionalCast<SubSurface>().get(), *value);
        result.push_back(viewFactor);
      }
    }

    return result;
  }

  bool ZonePropertyUserViewFactorsBySurfaceName_Impl::addViewFactors(const std::vector<ViewFactor> &viewFactors) {
    bool result = true;

    for (const ViewFactor& viewFactor : viewFactors) {
      bool thisResult = addViewFactor(viewFactor);
      if (!thisResult) {
        LOG(Error, "Could not add viewFactor " << viewFactor << " to " << briefDescription() << ". Continuing with others.");
        OS_ASSERT(false);
        result = false;
      }
    }

    return result;
  }

} // detail

ZonePropertyUserViewFactorsBySurfaceName::ZonePropertyUserViewFactorsBySurfaceName(ThermalZone& thermalZone)
  : ModelObject(ZonePropertyUserViewFactorsBySurfaceName::iddObjectType(), thermalZone.model())
{
  OS_ASSERT(getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>());

  bool ok = setPointer(OS_ZoneProperty_UserViewFactors_BySurfaceNameFields::ThermalZoneName, thermalZone.handle());
  OS_ASSERT(ok);
}

IddObjectType ZonePropertyUserViewFactorsBySurfaceName::iddObjectType() {
  return IddObjectType(IddObjectType::OS_ZoneProperty_UserViewFactors_BySurfaceName);
}

ThermalZone ZonePropertyUserViewFactorsBySurfaceName::thermalZone() const {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->thermalZone();
}

unsigned int ZonePropertyUserViewFactorsBySurfaceName::numberofViewFactors() const {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->numberofViewFactors();
}

bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const ViewFactor& viewFactor) {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(viewFactor);
}

bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const Surface& fromSurface, const Surface& toSurface, double viewFactor) {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(fromSurface, toSurface, viewFactor);
}

bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const Surface& fromSurface, const SubSurface& toSubSurface, double viewFactor) {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(fromSurface, toSubSurface, viewFactor);
}

bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const Surface& fromSurface, const InternalMass& toInternalMass, double viewFactor) {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(fromSurface, toInternalMass, viewFactor);
}

bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const SubSurface& fromSubSurface, const SubSurface& toSubSurface, double viewFactor) {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(fromSubSurface, toSubSurface, viewFactor);
}

bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const SubSurface& fromSubSurface, const Surface& toSurface, double viewFactor) {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(fromSubSurface, toSurface, viewFactor);
}

bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const SubSurface& fromSubSurface, const InternalMass& toInternalMass, double viewFactor) {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(fromSubSurface, toInternalMass, viewFactor);
}

bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const InternalMass& fromInternalMass, const InternalMass& toInternalMass, double viewFactor) {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(fromInternalMass, toInternalMass, viewFactor);
}

bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const InternalMass& fromInternalMass, const Surface& toSurface, double viewFactor) {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(fromInternalMass, toSurface, viewFactor);
}

bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactor(const InternalMass& fromInternalMass, const SubSurface& toSubSurface, double viewFactor) {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactor(fromInternalMass, toSubSurface, viewFactor);
}

void ZonePropertyUserViewFactorsBySurfaceName::removeViewFactor(int groupIndex) {
  getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->removeViewFactor(groupIndex);
}

void ZonePropertyUserViewFactorsBySurfaceName::removeAllViewFactors() {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->removeAllViewFactors();
}

std::vector<ViewFactor> ZonePropertyUserViewFactorsBySurfaceName::viewFactors() const {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->viewFactors();
}

bool ZonePropertyUserViewFactorsBySurfaceName::addViewFactors(const std::vector<ViewFactor> &viewFactors) {
  return getImpl<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl>()->addViewFactors(viewFactors);
}

/// @cond
ZonePropertyUserViewFactorsBySurfaceName::ZonePropertyUserViewFactorsBySurfaceName(std::shared_ptr<detail::ZonePropertyUserViewFactorsBySurfaceName_Impl> impl)
  : ModelObject(std::move(impl))
{}
/// @endcond

} // model
} // openstudio
