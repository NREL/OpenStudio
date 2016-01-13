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

#include "WindowDataFile.hpp"
#include "WindowDataFile_Impl.hpp"

#include "Material.hpp"
#include "Material_Impl.hpp"
#include "ModelExtensibleGroup.hpp"

#include <utilities/idd/OS_Construction_WindowDataFile_FieldEnums.hxx>
#include <utilities/idd/IddEnums.hxx>

#include "../utilities/core/Assert.hpp"

namespace openstudio {
namespace model {

namespace detail {

  WindowDataFile_Impl::WindowDataFile_Impl(const IdfObject& idfObject, Model_Impl* model, bool keepHandle)
    : ConstructionBase_Impl(idfObject, model, keepHandle)
  {
    OS_ASSERT(idfObject.iddObject().type() == WindowDataFile::iddObjectType());
  }

  WindowDataFile_Impl::WindowDataFile_Impl(const openstudio::detail::WorkspaceObject_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ConstructionBase_Impl(other,model,keepHandle)
  {
    OS_ASSERT(other.iddObject().type() == WindowDataFile::iddObjectType());
  }

  WindowDataFile_Impl::WindowDataFile_Impl(const WindowDataFile_Impl& other,
                                           Model_Impl* model,
                                           bool keepHandle)
    : ConstructionBase_Impl(other,model,keepHandle)
  {}

  const std::vector<std::string>& WindowDataFile_Impl::outputVariableNames() const
  {
    static std::vector<std::string> result;
    if (result.empty()){
    }
    return result;
  }

  IddObjectType WindowDataFile_Impl::iddObjectType() const {
    return WindowDataFile::iddObjectType();
  }

  bool WindowDataFile_Impl::isOpaque() const { return false; }

  bool WindowDataFile_Impl::isFenestration() const { return true; }

  bool WindowDataFile_Impl::isSolarDiffusing() const { return true; }

  bool WindowDataFile_Impl::isModelPartition() const { return false; }

  int WindowDataFile_Impl::renderingColorIndex() const
  {
    return OS_Construction_WindowDataFileFields::SurfaceRenderingName;
  }

} // detail

WindowDataFile::WindowDataFile(const Model& model)
  : ConstructionBase(WindowDataFile::iddObjectType(),model)
{
  OS_ASSERT(getImpl<detail::WindowDataFile_Impl>());
}

IddObjectType WindowDataFile::iddObjectType() {
  IddObjectType result(IddObjectType::OS_Construction_WindowDataFile);
  return result;
}

/// @cond
WindowDataFile::WindowDataFile(std::shared_ptr<detail::WindowDataFile_Impl> impl)
  : ConstructionBase(impl)
{}
/// @endcond

} // model
} // openstudio
