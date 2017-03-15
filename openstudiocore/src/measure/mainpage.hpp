/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
 *
 *  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
 *  following conditions are met:
 *
 *  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
 *  disclaimer.
 *
 *  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
 *  following disclaimer in the documentation and/or other materials provided with the distribution.
 *
 *  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
 *  products derived from this software without specific prior written permission from the respective party.
 *
 *  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
 *  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
 *  specific prior written permission from Alliance for Sustainable Energy, LLC.
 *
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
 *  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 *  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 **********************************************************************************************************************/

namespace openstudio {
namespace measure {

/** \mainpage OpenStudio %Measure 
 *
 *  The OpenStudio %Measure sub-project provides classes for building model parameterization, 
 *  manipulation, and queries. \link UserScript UserScripts\endlink are typically instantiated
 *  in Ruby scripts packaged as BCLMeasures (see openstudio::BCLMeasure in the utilities
 *  sub-project).
 *
 *  \section user_scripts_sec UserScripts
 *
 *  Every OpenStudio UserScript/BCLMeasure is a class, typically written in Ruby, that derives from
 *  one of the classes listed below (\link ModelUserScript ModelUserScripts\endlink are the most 
 *  common). 
 *
 *  \li UserScript
 *  \li ModelUserScript - Base class for scripts that operate on OpenStudio Models
 *  \li WorkspaceUserScript - Base class for scripts that operate on EnergyPlus IDF
 *  \li ReportingUserScript - Base class for scripts that operate on EnergyPlus results
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
 */

}
}
