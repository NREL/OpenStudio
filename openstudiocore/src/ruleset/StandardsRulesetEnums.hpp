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

#ifndef RULESET_STANDARDSRULESETENUMS_HPP
#define RULESET_STANDARDSRULESETENUMS_HPP

#include <ruleset/RulesetEnums.hpp>

#include <utilities/core/Enum.hpp>

namespace openstudio {
namespace ruleset {

/** \class StandardsRulesetModelType
 *  
 *  \brief Labels provided to differentiate between cloned models with the same name. 
 *
 *  \details The intent is for the Original model to be the model passed in by the user. Its 
 *  input data should not be modified by the RulesEngine. If input data modifications are 
 *  expected, the Original model should be cloned (using a StandardsActionCloneModel), and the
 *  clone should be designated as the Proposed model. For performace-based compliance, it is 
 *  usually necessary to also construct a Budget model, to which the Proposed model is compared. 
 */
OPENSTUDIO_ENUM(StandardsRulesetModelType,
    ((Original))
    ((Proposed))
    ((Budget))
);

/** \class StandardsRulesetResult 
 *
 *  \brief One-word descriptions of what can happen when a StandardsRuleset is applied to a 
 *  building model. 
 *
 *  \details If all operations are successful, the model is said to Pass. If the ruleset was 
 *  expecting to be able to find more information (for instance, a climate zone designation, or
 *  a standards-level description of the construction type), then the model is Incomplete. If
 *  an explicitly written rule fires and fails, the StandardsRulesetResult is Fail, and if 
 *  something unexpected happens during runtime, Error is registered. Obviously, more than one
 *  of these conditions may be applicable for a given model; the rulesengine::RulesEngine returns
 *  the most serious applicable code (Error, then Fail, then Incomplete, then Pass). */
OPENSTUDIO_ENUM(StandardsRulesetResult,
    ((Pass))
    ((Incomplete))
    ((Fail))
    ((Error))
);

/** \class StandardsRulesetPrintMode 
 * 
 *  \brief Styles in which a StandardsRuleset may be printed. 
 *
 *  \details Engine refers to the rulesengine::RulesEngine, which in turn passes the printed 
 *  constructs into the CLIPS engine. */
OPENSTUDIO_ENUM(StandardsRulesetPrintMode,
    ((File))
    ((Engine))
);

} // ruleset
} // openstudio

#endif // RULESET_STANDARDSRULESETENUMS_HPP
