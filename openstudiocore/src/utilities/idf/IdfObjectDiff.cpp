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

#include "IdfObjectDiff.hpp"
#include "IdfObjectDiff_Impl.hpp"

#include "../core/Assert.hpp"

namespace openstudio {
  namespace detail {

    IdfObjectDiff_Impl::IdfObjectDiff_Impl()
    {}

    IdfObjectDiff_Impl::IdfObjectDiff_Impl(boost::optional<unsigned> index, boost::optional<std::string> oldValue, boost::optional<std::string> newValue)
      : m_index(index), m_oldValue(oldValue), m_newValue(newValue)
    {}

    IdfObjectDiff_Impl::~IdfObjectDiff_Impl()
    {}

    boost::optional<unsigned> IdfObjectDiff_Impl::index() const
    {
      return m_index;
    }

    bool IdfObjectDiff_Impl::isNull() const
    {
      bool result = true;
      if (m_oldValue && m_newValue){
        result = (m_oldValue.get() == m_newValue.get());
      }else{
        result = (m_oldValue == m_newValue);
      }
      return result;
    }

    boost::optional<std::string> IdfObjectDiff_Impl::oldValue() const
    {
      return m_oldValue;
    }

    boost::optional<std::string> IdfObjectDiff_Impl::newValue() const
    {
      return m_newValue;
    }

  } // detail

  IdfObjectDiff::IdfObjectDiff()
    : m_impl(std::shared_ptr<detail::IdfObjectDiff_Impl>(new detail::IdfObjectDiff_Impl()))
  {
    OS_ASSERT(m_impl);
  }

  IdfObjectDiff::IdfObjectDiff(boost::optional<unsigned> index, boost::optional<std::string> oldValue, boost::optional<std::string> newValue)
    : m_impl(std::shared_ptr<detail::IdfObjectDiff_Impl>(new detail::IdfObjectDiff_Impl(index, oldValue, newValue)))
  {
    OS_ASSERT(m_impl);
  }

  IdfObjectDiff::IdfObjectDiff(const std::shared_ptr<detail::IdfObjectDiff_Impl>& impl)
    : m_impl(impl)
  {
    OS_ASSERT(m_impl);
  }

  boost::optional<unsigned> IdfObjectDiff::index() const
  {
    return m_impl->index();
  }

  bool IdfObjectDiff::isNull() const
  {
    return m_impl->isNull();
  }

  boost::optional<std::string> IdfObjectDiff::oldValue() const
  {
    return m_impl->oldValue();
  }

  boost::optional<std::string> IdfObjectDiff::newValue() const
  {
    return m_impl->newValue();
  }

} // openstudio
