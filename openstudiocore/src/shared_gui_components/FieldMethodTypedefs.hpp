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

#ifndef OPENSTUDIO_FIELDMETHODTYPEDEFS_HPP
#define OPENSTUDIO_FIELDMETHODTYPEDEFS_HPP

#include <boost/function.hpp>
#include <boost/optional.hpp>

#include <vector>

namespace openstudio {

typedef boost::function<bool ()> BoolGetter;
typedef boost::function<double ()> DoubleGetter;
typedef boost::function<int ()> IntGetter;
typedef boost::function<std::string ()> StringGetter;
typedef boost::function<unsigned ()> UnsignedGetter;

typedef boost::function<boost::optional<double> ()> OptionalDoubleGetter;
typedef boost::function<boost::optional<int> ()> OptionalIntGetter;
typedef boost::function<boost::optional<std::string> ()> OptionalStringGetter;
typedef boost::function<boost::optional<unsigned> ()> OptionalUnsignedGetter;

typedef boost::function<std::vector<std::string> ()> ChoicesGetter;

typedef boost::function<void (bool)> BoolSetter;
typedef boost::function<bool (double)> DoubleSetter;
typedef boost::function<bool (int)> IntSetter;
typedef boost::function<bool (std::string)> StringSetter;
typedef boost::function<bool (unsigned)> UnsignedSetter;

typedef boost::function<void ()> NoFailAction;
typedef boost::function<bool ()> BasicQuery;

}

#endif // OPENSTUDIO_FIELDMETHODTYPEDEFS_HPP
