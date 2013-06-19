/**********************************************************************
*  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

#include <utilities/document/DocumentElement.hpp>
#include <utilities/document/DocumentElement_Impl.hpp>

namespace openstudio {

namespace detail {

} // detail

DocumentElement DocumentElement::clone() const {
  return getImpl<detail::DocumentElement_Impl>()->clone();
}

std::string DocumentElement::print(DocumentFormat fmt) const {
  return getImpl<detail::DocumentElement_Impl>()->print(fmt);
}

std::ostream& DocumentElement::printToStream(std::ostream& os,DocumentFormat fmt) const {
  return getImpl<detail::DocumentElement_Impl>()->printToStream(os,fmt);
}

DocumentElement::DocumentElement(boost::shared_ptr<detail::DocumentElement_Impl> impl)
  : m_impl(impl)
{}

} // openstudio
