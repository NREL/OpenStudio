/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

#include "WorkspaceObjectDiff.hpp"
#include "WorkspaceObjectDiff_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
  namespace detail {

    WorkspaceObjectDiff_Impl::WorkspaceObjectDiff_Impl(unsigned index, boost::optional<std::string> oldValue, boost::optional<std::string> newValue,
                                                       boost::optional<UUID> oldHandle, boost::optional<UUID> newHandle)
      : IdfObjectDiff_Impl(index, oldValue, newValue),
        m_oldHandle(oldHandle), m_newHandle(newHandle)
    {
    }

    boost::optional<UUID> WorkspaceObjectDiff_Impl::oldHandle() const
    {
      return m_oldHandle;
    }

    boost::optional<UUID> WorkspaceObjectDiff_Impl::newHandle() const
    {
      return m_newHandle;
    }

  }

  WorkspaceObjectDiff::WorkspaceObjectDiff(unsigned index, boost::optional<std::string> oldValue, boost::optional<std::string> newValue,
                                           boost::optional<UUID> oldHandle, boost::optional<UUID> newHandle)
    : IdfObjectDiff(std::shared_ptr<detail::WorkspaceObjectDiff_Impl>(new detail::WorkspaceObjectDiff_Impl(index, oldValue, newValue, oldHandle, newHandle)))
  {
    OS_ASSERT(getImpl<detail::WorkspaceObjectDiff_Impl>());
  }

  WorkspaceObjectDiff::WorkspaceObjectDiff(const std::shared_ptr<detail::WorkspaceObjectDiff_Impl>& impl)
    : IdfObjectDiff(std::move(impl))
  {
    OS_ASSERT(getImpl<detail::WorkspaceObjectDiff_Impl>());
  }

  boost::optional<UUID> WorkspaceObjectDiff::oldHandle() const
  {
    return getImpl<detail::WorkspaceObjectDiff_Impl>()->oldHandle();
  }

  boost::optional<UUID> WorkspaceObjectDiff::newHandle() const
  {
    return getImpl<detail::WorkspaceObjectDiff_Impl>()->newHandle();
  }

}
