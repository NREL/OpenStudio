########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2016, Alliance for Sustainable Energy, LLC. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
#  following disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote
#  products derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative
#  works may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without
#  specific prior written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER, THE UNITED STATES GOVERNMENT, OR ANY CONTRIBUTORS BE LIABLE FOR
#  ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
#  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
#  AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
#  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

######################################################################
# == Synopsis 
#
#   Create a set of MELs components from metering data.
#
# == Usage
#
#  ruby MakeMELs.rb ARGV[0] 
#
#  ARGV[0] - Path to directory containing 'MELsIndex.csv' and data files
#            for each MEL
#
# == Examples
#
#   ruby MakeMELs.rb 'C:\working\mels'
#
######################################################################

require 'openstudio'
require 'csv'
require 'pathname'
require 'fileutils'

# class to hold results
class MelsData
  attr_accessor :max_power, :schedule  
  
  def initialize(max_power, schedule)
    @max_power = max_power
    @schedule = schedule
  end
end

# read each csv file, e.g. 'VendingMachine.csv'
def process_mels_data(path)

  schedule = []
  CSV.open(path, 'r') do |row|
    schedule << row[1].to_f
  end
  
  # figure out max power
  max_power = schedule.max
  
  # normalize schedule
  schedule.collect! {|v| v = v/max_power}
  
  return MelsData.new(max_power, schedule)
end

# process each row in the index
def process_row(path, row)
  id = row[0].to_s
  name = row[1].to_s
  filename = row[2].to_s
  fraction_latent = row[3].to_f
  fraction_radiant = row[4].to_f
  fraction_lost = row[5].to_f
  end_use_subcategory = row[6].to_s
  
  # make a results dir
  new_dir = path + name
  if File.exists?(new_dir)
    FileUtils.rm_rf(new_dir)
  end
  Dir.mkdir(new_dir)
  
  # get the mels data
  mels_data = process_mels_data(path + "#{filename}")
  max_power = mels_data.max_power
  schedule = mels_data.schedule
  
  # write the schedule
  File.open(new_dir + "#{filename}", 'w') do |file|
    schedule.each {|v| file.puts v}
  end
  
  # text for the equipment object
  equipment_text = <<EQUIPMENT_TEXT
ElectricEquipment,
  #{name},            !- Name
  ,                   !- Zone Name
  #{name} Schedule,   !- Schedule Name
  EquipmentLevel,     !- Design Level Calculation Method
  #{max_power},       !- Design Level
  ,                   !- Watts per Zone Floor Area
  ,                   !- Watts per Person
  #{fraction_latent},     !- Fraction Latent
  #{fraction_radiant},    !- Fraction Radiant
  #{fraction_lost},       !- Fraction Lost  
  #{end_use_subcategory}; ! End-Use Subcategory  
EQUIPMENT_TEXT

  # text for the schedule object
  schedule_text = <<SCHEDULE_TEXT  
Schedule:File,
  #{name} Schedule,    !- Name
  ,                    !- Schedule Type Limits Name
  ./#{filename},       !- File Name
  1,                   !- Column Number
  #{0},                !- Rows to Skip at Top
  #{schedule.length},  !- Number of Hours of Data
  Comma;               !- Column Separator
SCHEDULE_TEXT

  # make a workspace
  workspace = OpenStudio::Workspace.new
  
  # add schedule to the workspace
  workspace.addObject(OpenStudio::IdfObject.load(schedule_text).get)  
  
  # add equipment to the workspace
  workspace.addObject(OpenStudio::IdfObject.load(equipment_text).get)

  # write the workspace
  File.open(new_dir + "#{name}.idf", 'w') do |file|
    file << workspace.toIdfFile.to_s
  end
  
end

# main
path = Pathname.new(ARGV[0])
row_number = 1
CSV.open(path + 'MELsIndex.csv', 'r') do |row|
  if row_number > 1
    process_row(path, row)
  end
  row_number += 1
end
