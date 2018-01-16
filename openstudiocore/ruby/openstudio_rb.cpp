/***********************************************************************************************************************
 *  OpenStudio(R), Copyright (c) 2008-2018, Alliance for Sustainable Energy, LLC. All rights reserved.
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

#include "init_openstudio.hpp"
#include <RubyAPI.hpp>
#include <QtPlugin>
#include <iostream>
#include <ruby.h>


Q_IMPORT_PLUGIN(QSQLiteDriverPlugin);
#if defined(Q_OS_OSX)
  Q_IMPORT_PLUGIN(QCocoaIntegrationPlugin);
#elif defined(Q_OS_WIN)
  Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin);
#elif defined(Q_OS_LINUX)
  Q_IMPORT_PLUGIN(QXcbIntegrationPlugin);
#endif

inline void initResources() {

  #ifndef SHARED_OS_LIBS
    Q_INIT_RESOURCE(modeleditorlib);
  #endif // SHARED_OS_LIBS

}

extern "C" {

void Init_openstudiomodeleditor();

RUBY_API void Init_openstudio(void) {

  initResources();

  init_openstudio_internal();

  Init_openstudiomodeleditor();
  rb_provide("openstudiomodeleditor");
  rb_provide("openstudiomodeleditor.so");


  // DLM: these files should be embedded in the openstudio.so, currently they are only in openstudio.exe
  rb_provide("openstudio/energyplus/find_energyplus.rb");
  rb_provide("openstudio/measure/ShowRunnerOutput.rb");
  rb_provide("openstudio/ruleset/ShowRunnerOutput.rb");

    std::string show_runner_output_script = R"END(
def show_output(result)

  puts "**MEASURE APPLICABILITY**"
  applicability = result.value.value
  if applicability ==  -1
    puts "#{applicability} = Not Applicable"
  elsif applicability == 0
    puts "#{applicability} = Success"
  elsif applicability == 1
    puts "#{applicability} = Fail"
  end

  puts "**INITIAL CONDITION**"
  if result.stepInitialCondition.empty?
    #do nothing
  else
    puts result.stepInitialCondition.get
  end

  puts "**FINAL CONDITION**"
  if result.stepFinalCondition.empty?
    #do nothing
  else
    puts result.stepFinalCondition.get
  end

  puts "**INFO MESSAGES**"
  result.stepInfo.each do |info_msg|
    puts "#{info_msg}"
  end

  puts "**WARNING MESSAGES**"
  result.stepWarnings.each do |info_msg|
    puts "#{info_msg}"
  end

  puts "**ERROR MESSAGES**"
  result.stepErrors.each do |info_msg|
    puts "#{info_msg}"
  end

  puts "***Machine-Readable Attributes**"
  values = []
  result.stepValues.each do |value|
    values << value.string
  end
  puts "[\n#{values.join(',').strip}\n]"

  puts "***Files Generated**"
  result.stepFiles.each do |file|
    puts "#{file}"
  end

  puts "" #space between measures for readability in output
  puts ""

end
)END";

  evalString(show_runner_output_script);

}

}

