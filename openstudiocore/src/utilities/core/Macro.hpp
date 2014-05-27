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

#ifndef UTILITIES_CORE_MACRO_HPP
#define UTILITIES_CORE_MACRO_HPP

/** \file Macro.hpp
 *
 *  Macros for common typedefs we want to use with dynamically allocated and
 *  plain data objects.
 */

#include <vector>
#include <boost/optional.hpp>

/** Typedefs for shared pointers and vector of shared pointers, const and
 *  non-const variants. */
#define COMMON_PTR_TYPEDEFS(__T__) \
  typedef std::shared_ptr<__T__> Ptr; \
  typedef std::weak_ptr<__T__> WeakPtr; \
  typedef std::vector<Ptr> Vec; \
  typedef std::shared_ptr<const __T__> ConstPtr; \
  typedef std::weak_ptr<const __T__> ConstWeakPtr; \
  typedef std::vector<ConstPtr> ConstVec;

#endif // UTILITIES_CORE_MACRO_HPP
