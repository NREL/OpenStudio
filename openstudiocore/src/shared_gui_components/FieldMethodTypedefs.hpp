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

#ifndef SHAREDGUICOMPONENTS_FIELDMETHODTYPEDEFS_HPP
#define SHAREDGUICOMPONENTS_FIELDMETHODTYPEDEFS_HPP

#include "../model/Model.hpp"
#include "../model/Model_Impl.hpp"
#include "../model/ModelObject.hpp"
#include "../model/ModelObject_Impl.hpp"

#include <boost/optional.hpp>

#include <vector>

namespace openstudio {

typedef std::function<bool ()> BoolGetter;
typedef std::function<double ()> DoubleGetter;
typedef std::function<int ()> IntGetter;
typedef std::function<std::string ()> StringGetter;
typedef std::function<unsigned ()> UnsignedGetter;
typedef std::function<model::ModelObject ()> ModelObjectGetter;

typedef std::function<boost::optional<double> ()> OptionalDoubleGetter;
typedef std::function<boost::optional<int> ()> OptionalIntGetter;
typedef std::function<boost::optional<std::string> ()> OptionalStringGetter;
typedef std::function<boost::optional<std::string> (bool)> OptionalStringGetterBoolArg;
typedef std::function<boost::optional<unsigned> ()> OptionalUnsignedGetter;
typedef std::function<boost::optional<model::ModelObject> ()> OptionalModelObjectGetter;

typedef std::function<void (bool)> BoolSetter;
typedef std::function<bool (double)> DoubleSetter;
typedef std::function<void (double)> DoubleSetterVoidReturn;
typedef std::function<bool (int)> IntSetter;
typedef std::function<bool (std::string)> StringSetter;
typedef std::function<boost::optional<std::string> (const std::string &)> StringSetterOptionalStringReturn;
typedef std::function<bool (unsigned)> UnsignedSetter;
typedef std::function<bool (const model::ModelObject &)> ModelObjectSetter;

typedef std::function<void ()> NoFailAction;
typedef std::function<bool ()> BasicQuery;

}

#endif // SHAREDGUICOMPONENTS_FIELDMETHODTYPEDEFS_HPP
