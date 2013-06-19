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
namespace ruleset {

/** \mainpage OpenStudio %Ruleset 
 *
 *  The OpenStudio %Ruleset sub-project provides classes for building model parameterization, 
 *  manipulation, and queries. There are two methods for carrying out these types of operations: 
 *  free-form \link UserScript UserScripts\endlink and \link Ruleset Rulesets\endlink. The former 
 *  are typically instantiated in Ruby scripts packaged as BCLMeasures (see 
 *  openstudio::BCLMeasure in the utilities sub-project), and are thoroughly supported by the 
 *  OpenStudio team. \link Ruleset Rulesets\endlink are currently at proof-of-concept stage, but 
 *  \link ModelRuleset ModelRulesets\endlink are useful as-is in large-scale scripted analyses, 
 *  providing parametrization with brevity and introspection.
 *
 *  \section user_scripts_sec UserScripts
 *
 *  Every OpenStudio UserScript/BCLMeasure is a class, typically written in Ruby, that derives from
 *  one of the classes listed below (\link ModelUserScript ModelUserScripts\endlink are the most 
 *  common). 
 *
 *  \li UserScript
 *  \li UtilityUserScript - Base class for configuration scripts (do not operate on energy models)
 *  \li ModelUserScript - Base class for scripts that operate on OpenStudio Models
 *  \li WorkspaceUserScript - Base class for scripts that operate on EnergyPlus IDF
 *
 *  \link UserScript UserScripts\endlink are folded into OpenStudio workflows using the classes:
 *  
 *  \li OSArgument - Class for specifying and setting values for script arguments.
 *  \li OSResult - Class for capturing an overall result flag; error, warning, and info messages; and
 *                 the initial and final conditions that occur during a single instance of running 
 *                 a user script (typically on an energy model).
 *  \li OSRunner - Class for running scripts. All of the UserScript classes expect a class derived
 *                 from OSRunner to be passed in to the run method. The base class provies helper 
 *                 methods for checking arguments and aggregating results. There is typically one 
 *                 derived class per application, which can be used to customize application 
 *                 behaviors (finding available measures/scripts, alerting the user when there is 
 *                 an error, etc.)
 *  \li RubyUserScriptArgumentGetter - Standard methods for retrieving UserScript \link OSArgument
 *                 arguments\endlink using embedded or native Ruby.
 *
 *  For more information, see BCLMeasure (in utilities/bcl/), runmanager::RubyJobBuilder, 
 *  analysis::RubyPerturbation, analysis::RubyContinuousVariable, the SketchUp Plug-in (user script
 *  menu), the OpenStudio Application (scripts tab), analysisdriver::SimpleProject, and the 
 *  Parametric Analysis Tool (PAT).
 *
 *  \section ruleset_sec Rulesets
 *
 *  OpenStudio rules and rulesets provide abstractions for building model queries and perturbations. 
 *  The project is split into two parts. \link ModelRule ModelRules \endlink and \link 
 *  ModelRuleset ModelRulesets \endlink work directly with OpenStudio building models, whereas
 *  \link StandardsRule StandardsRules \endlink and \link StandardsRuleset StandardsRulesets 
 *  \endlink reference the standardsinterface and are exercised by the rulesengine.
 *
 *  These two halves share the basic abstraction of a rule into a series of \link FilterClause 
 *  filters \endlink and \link ActionClause actions \endlink. If all of the filters of a rule 
 *  are satisfied, then the actions of that rule will fire, typically operating on data that
 *  matched the filters.
 *
 *  At this time, all \link Clause Clauses \endlink forming a Rule are and-ed together (or and not 
 *  are not yet supported), and only a limited number of FilterClause and ActionClause types are 
 *  provided.
 *
 *  The ModelRuleset infrastructure can be used in the analysis framework (see the analysis 
 *  sub-project, especially analysis::ModelRulesetPerturbation and 
 *  analysis::ModelRulesetContinuousVariable) to modify OpenStudio Models. When applicable, they can 
 *  perform the same operation as an equivalent UserScript with less code, and with the possibility 
 *  of creating a rule-writing UI complete with introspection as to what attributes and relationships 
 *  are available for manipulation within a given model.
 */

}
}
