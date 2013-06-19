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

namespace openstudio {
namespace rulesengine {

/** \mainpage OpenStudio RulesEngine
 *
 *  \details The OpenStudio RulesEngine library lets users apply StandardsRulesets (see ruleset
 *  documentation) to building models wrapped in an AgnosticInterface (see standardsinterface 
 *  documentation). Most of the functionality is embedded in RulesEngine and built on the 
 *  AgnosticInterface. However, RulesEngine is an abstract class. We provide the concrete class 
 *  OpenStudioRulesEngine, which manages the concrete building models used by the engine, and will 
 *  eventually provide access to local building component libraries. There is a parallel between 
 *  the rulesengine and standardsinterface namespaces: OpenStudioRulesEngine is to RulesEngine as 
 *  OpenStudioStandardsInterface is to AgnosticInterface. Less abstractly, RulesEngine assumes
 *  building models wrapped in AgnosticInterfaces, and OpenStudioRulesEngine assumes OpenStudio
 *  model::Models wrapped in OpenStudioStandardsInterfaces.
 *
 *  The Rules Engine is built on top of a public domain, third party, expert systems library, 
 *  CLIPS (http://clipsrules.sourceforge.net/). The StandardsRuleset prints itself out using 
 *  CLIPS constructs, and the RulesEngine provides additional constructs to identify and get access
 *  to models, objects, and attributes.
 *  
 *  At this time, the RulesEngine only supports applying prescriptive rulesets (object and 
 *  attribute getters only, no setters) to single models. In the future we anticipate supporting
 *  performance compliance, more sophisticated prescriptive rules, more sophisticated rule logic
 *  in general, and baseline model creation. The RulesEngine itself, or an overarching structure
 *  that uses the RulesEngine is expected to handle multiple buildings (climate sweeps, etc.).
 *
 *  The result of a single compliance run is returned in an ApplyRulesetResult object, which is 
 *  available from RulesEngine::result() after a call to OpenStudioRulesEngine::applyRules.
 *
 *  \section quick_links Quick Links
 *
 *  \li ApplyRulesetResult
 *  \li RulesEngine
 *  \li OpenStudioRulesEngine
 */

}
}