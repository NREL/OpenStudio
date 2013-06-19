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

#ifndef RULESET_RULESETENUMS_HPP
#define RULESET_RULESETENUMS_HPP

#include <utilities/core/Enum.hpp>

namespace openstudio {
namespace ruleset {

/** \class RulesetNumericalPredicate 
 * 
 *  \brief An enumeration to represent basic numerical predicates like ==, !=, >, <=, etc.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro
 *  call is:
 *
 *  \code
OPENSTUDIO_ENUM(RulesetNumericalPredicate,
    ((Equals)(equals))
    ((NotEquals)(not equals))
    ((GreaterThan)(greater than))
    ((GreaterThanOrEqualTo)(greater than or equal to))
    ((LessThan)(less than))
    ((LessThanOrEqualTo)(less than or equal to))
);
 *  \endcode */
OPENSTUDIO_ENUM(RulesetNumericalPredicate,
    ((Equals)(equals))
    ((NotEquals)(not equals))
    ((GreaterThan)(greater than))
    ((GreaterThanOrEqualTo)(greater than or equal to))
    ((LessThan)(less than))
    ((LessThanOrEqualTo)(less than or equal to))
);

/** \class RulesetStringPredicate 
 *
 *  \brief An enumeration to represent predicates on strings like ==, case insensitive equals,
 *  and matches regular expression.
 *  \details See the OPENSTUDIO_ENUM documentation in utilities/core/Enum.hpp. The actual macro
 *  call is:
 *
 *  \code
OPENSTUDIO_ENUM(RulesetStringPredicate,
    ((Equals)(equals))
    ((NotEquals)(not equals))
    ((IEquals)(insensitive equals))
    ((NotIEquals)(not insensitive equals))
    ((Match)(matches))
    ((NotMatch)(not matches))
);
 *  \endcode */
OPENSTUDIO_ENUM(RulesetStringPredicate,
    ((Equals)(equals))
    ((NotEquals)(not equals))
    ((IEquals)(insensitive equals))
    ((NotIEquals)(not insensitive equals))
    ((Match)(matches))
    ((NotMatch)(not matches))
);

} // ruleset
} // openstudio

#endif // RULESET_RULESETENUMS_HPP
