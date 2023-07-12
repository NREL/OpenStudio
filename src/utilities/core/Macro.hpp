/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

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
#define COMMON_PTR_TYPEDEFS(__T__)                 \
  typedef std::shared_ptr<__T__> Ptr;              \
  typedef std::weak_ptr<__T__> WeakPtr;            \
  typedef std::vector<Ptr> Vec;                    \
  typedef std::shared_ptr<const __T__> ConstPtr;   \
  typedef std::weak_ptr<const __T__> ConstWeakPtr; \
  typedef std::vector<ConstPtr> ConstVec;

#endif  // UTILITIES_CORE_MACRO_HPP
