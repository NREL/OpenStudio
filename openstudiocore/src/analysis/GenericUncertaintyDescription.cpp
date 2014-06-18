/**********************************************************************
 *  Copyright (c) 2008-2014, Alliance for Sustainable Energy.
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

#include "GenericUncertaintyDescription.hpp"
#include "UncertaintyDescription_Impl.hpp"

namespace openstudio {
namespace analysis {

GenericUncertaintyDescription::GenericUncertaintyDescription(const UncertaintyDescriptionType& type)
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(type)))
{}

GenericUncertaintyDescription::GenericUncertaintyDescription(const UncertaintyDescriptionType& type,
                                                             const std::vector<Attribute>& attributes)
  : UncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl>(
        new detail::UncertaintyDescription_Impl(type,attributes)))
{}

UncertaintyDescriptionType GenericUncertaintyDescription::type() {
  return UncertaintyDescriptionType(UncertaintyDescriptionType::Generic);
}

UncertaintyDescriptionType GenericUncertaintyDescription::actualType() const {
  return impl()->type();
}

std::vector<Attribute> GenericUncertaintyDescription::attributes() const {
  return impl()->attributes(false);
}

std::vector<AttributeDescription> GenericUncertaintyDescription::attributeDescriptions() const {
  return impl()->attributeDescriptions();
}

Attribute GenericUncertaintyDescription::getAttribute(const std::string& attributeName) const {
  return impl()->getAttribute(attributeName,true);
}

bool GenericUncertaintyDescription::isSet(const std::string& attributeName) const {
  return impl()->isSet(attributeName);
}

bool GenericUncertaintyDescription::isComplete() const {
  return impl()->isComplete();
}

bool GenericUncertaintyDescription::setAttribute(const Attribute& candidate) {
  return impl()->setAttribute(candidate,true);
}

bool GenericUncertaintyDescription::clearAttribute(const std::string& attributeName) {
  return impl()->clearAttribute(attributeName,true);
}

GenericUncertaintyDescription::GenericUncertaintyDescription(std::shared_ptr<detail::UncertaintyDescription_Impl> impl)
  : UncertaintyDescription(impl)
{}

} // analysis
} // openstudio
