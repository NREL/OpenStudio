/**********************************************************************
 *  Copyright (c) 2008-2016, Alliance for Sustainable Energy.
 *  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 **********************************************************************/

#include "ComponentData.hpp"
#include "ComponentData_Impl.hpp"

#include "Model.hpp"
#include "ModelExtensibleGroup.hpp"

#include "../utilities/idf/IdfExtensibleGroup.hpp"
#include <utilities/idd/OS_ComponentData_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

#include <time.h>

namespace openstudio {
namespace model {

namespace detail {

  ComponentData_Impl::ComponentData_Impl(const IdfObject& idfObject,
                                         Model_Impl* model,
                                         bool keepHandle)
    : ModelObject_Impl(idfObject,model,keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == ComponentData::iddObjectType());
  }

  ComponentData_Impl::ComponentData_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == ComponentData::iddObjectType());
  }

  ComponentData_Impl::ComponentData_Impl(const ComponentData_Impl& other,
                                         Model_Impl* model,
                                         bool keepHandle)
    : ModelObject_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& ComponentData_Impl::outputVariableNames() const {
    static StringVector result;
    if (result.empty()) {}
    return result;
  }

  IddObjectType ComponentData_Impl::iddObjectType() const {
    return ComponentData::iddObjectType();
  }

  UUID ComponentData_Impl::uuid() const {
    OptionalString os = getString(OS_ComponentDataFields::UUID);
    OS_ASSERT(os);
    return toUUID(*os);
  }

  UUID ComponentData_Impl::versionUUID() const {
    OptionalString os = getString(OS_ComponentDataFields::VersionUUID);
    OS_ASSERT(os);
    return toUUID(*os);
  }

  boost::optional<int> ComponentData_Impl::creationTimestamp() const {
    return getInt(OS_ComponentDataFields::CreationTimestamp,true);
  }

  boost::optional<int> ComponentData_Impl::versionTimestamp() const {
    return getInt(OS_ComponentDataFields::VersionTimestamp,true);
  }

  ModelObject ComponentData_Impl::primaryComponentObject() const {
    OptionalModelObject omo = getComponentObject(0);
    if (!omo) {
      LOG_AND_THROW("ComponentData objects must always be able to return the primary object in the component.");
    }
    return *omo;
  }

  std::vector<ModelObject> ComponentData_Impl::componentObjects() const {
    ModelObjectVector result;
    for (unsigned i = 0, n = numComponentObjects(); i < n; ++i) {
      result.push_back(getComponentObject(i));
    }
    return result;
  }

  ModelObject ComponentData_Impl::getComponentObject(unsigned objectIndex) const {
    if (objectIndex >= numComponentObjects()) {
      LOG_AND_THROW("objectIndex = " << objectIndex << " >= numComponentObjects() = " << numComponentObjects() << ".");
    }

    IdfExtensibleGroup eg = getExtensibleGroup(objectIndex);
    if (eg.empty()) {
      LOG_AND_THROW("Cannot retrieve IdfExtensibleGroup at objectIndex = " << objectIndex);
    }

    ModelExtensibleGroup componentObjectData = eg.cast<ModelExtensibleGroup>();
    OptionalModelObject omo = componentObjectData.getModelObjectTarget<ModelObject>(
        OS_ComponentDataExtensibleFields::NameofObject);
    if (!omo) {
      LOG_AND_THROW("Cannot retrieve ModelObject at objectIndex = " << objectIndex);
    }

    return *omo;
  }

  UUID ComponentData_Impl::createVersionUUID() {
    bool ok = setString(OS_ComponentDataFields::VersionUUID,toString(createUUID()));
    OS_ASSERT(ok);
    ok = setInt(OS_ComponentDataFields::VersionTimestamp,time(nullptr));
    OS_ASSERT(ok);
    return versionUUID();
  }

  bool ComponentData_Impl::registerObject(const ModelObject& object) {
    IdfExtensibleGroup eg = pushExtensibleGroup(StringVector());
    bool result = !eg.empty();
    if (result) {
      ModelExtensibleGroup meg = eg.cast<ModelExtensibleGroup>();
      result = result && meg.setPointer(OS_ComponentDataExtensibleFields::NameofObject,
                                        object.handle());
    }
    return result;
  }

  unsigned ComponentData_Impl::numComponentObjects() const {
    return numExtensibleGroups();
  }

  boost::optional<ModelObject> ComponentData_Impl::primaryComponentObjectAsModelObject() const {
    OptionalModelObject result = primaryComponentObject();
    return result;
  }

} // detail

IddObjectType ComponentData::iddObjectType() {
  IddObjectType result(IddObjectType::OS_ComponentData);
  return result;
}

UUID ComponentData::uuid() const {
  return getImpl<detail::ComponentData_Impl>()->uuid();
}

UUID ComponentData::versionUUID() const {
  return getImpl<detail::ComponentData_Impl>()->versionUUID();
}

boost::optional<int> ComponentData::creationTimestamp() const {
  return getImpl<detail::ComponentData_Impl>()->creationTimestamp();
}

boost::optional<int> ComponentData::versionTimestamp() const {
  return getImpl<detail::ComponentData_Impl>()->versionTimestamp();
}

ModelObject ComponentData::primaryComponentObject() const {
  return getImpl<detail::ComponentData_Impl>()->primaryComponentObject();
}

std::vector<ModelObject> ComponentData::componentObjects() const {
  return getImpl<detail::ComponentData_Impl>()->componentObjects();
}

ModelObject ComponentData::getComponentObject(unsigned objectIndex) const {
  return getImpl<detail::ComponentData_Impl>()->getComponentObject(objectIndex);
}

UUID ComponentData::createVersionUUID() {
  return getImpl<detail::ComponentData_Impl>()->createVersionUUID();
}

unsigned ComponentData::numComponentObjects() const {
  return getImpl<detail::ComponentData_Impl>()->numComponentObjects();
}

/// @cond
ComponentData::ComponentData(const Model& model)
  : ModelObject(iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::ComponentData_Impl>());
  setString(OS_ComponentDataFields::UUID,toString(createUUID()));
  setInt(OS_ComponentDataFields::CreationTimestamp,time(nullptr));
  createVersionUUID();
}

ComponentData::ComponentData(std::shared_ptr<detail::ComponentData_Impl> impl)
  : ModelObject(impl)
{}
/// @endcond

} // model
} // openstudio
