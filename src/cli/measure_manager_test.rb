########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
#
#  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
#  following conditions are met:
#
#  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
#  disclaimer.
#
#  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
#  disclaimer in the documentation and/or other materials provided with the distribution.
#
#  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
#  derived from this software without specific prior written permission from the respective party.
#
#  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
#  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
#  written permission from Alliance for Sustainable Energy, LLC.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
#  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
#  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
#  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
#  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
#  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
#  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
#  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
########################################################################################################################

# start a measure manager server with `openstudio measure -s 1234`
# then run this script

require 'rubygems'

require 'rest-client'
require 'json'
require 'openstudio'
require 'fileutils'

@user = 'user'
@pass = 'password'
@host = 'http://localhost:1234'
#@measure_dir = 'D:/openstudio-measures/NREL working measures'
@measure_dir = 'D:/test/measures'

def reset()

  result = {}

  begin
    json_request = JSON.generate({})
    request = RestClient::Resource.new("#{@host}/reset", user: @user, password: @pass)
    response = request.post(json_request, content_type: :json, accept: :json)
    result = JSON.parse(response.body, :symbolize_names => true)
  rescue Exception => e
    puts "reset failed"
    puts e.message
  end

  return result
end

def set(payload)

  result = []

  begin
    json_request = JSON.generate(payload)
    request = RestClient::Resource.new("#{@host}/set", user: @user, password: @pass)
    response = request.post(json_request, content_type: :json, accept: :json)
    result = JSON.parse(response.body, :symbolize_names => true)
  rescue Exception => e
    puts "set #{payload} failed"
    puts e.message
  end

  return result
end

def download_bcl_measure(uid)
  result = []

  begin
    json_request = JSON.generate({:uid => uid})
    request = RestClient::Resource.new("#{@host}/download_bcl_measure", user: @user, password: @pass)
    response = request.post(json_request, content_type: :json, accept: :json)
    result = JSON.parse(response.body, :symbolize_names => true)
  rescue Exception => e
    puts "download_bcl_measure #{uid} failed"
    puts e.message
  end

  return result
end

def bcl_measures

  result = []

  begin
    json_request = JSON.generate({})
    request = RestClient::Resource.new("#{@host}/bcl_measures", user: @user, password: @pass)
    response = request.post(json_request, content_type: :json, accept: :json)
    result = JSON.parse(response.body, :symbolize_names => true)
  rescue Exception => e
    puts "bcl_measures failed"
    puts e.message
  end

  return result
end

def update_measures(measures_dir = nil)

  payload = {}
  if measures_dir
    measures_dir = File.absolute_path(measures_dir)
    payload[:measures_dir] = measures_dir
  end

  result = []

  begin
    json_request = JSON.generate(payload)
    request = RestClient::Resource.new("#{@host}/update_measures", user: @user, password: @pass)
    response = request.post(json_request, content_type: :json, accept: :json)
    result = JSON.parse(response.body, :symbolize_names => true)
  rescue Exception => e
    puts "update_measures(#{measures_dir}) failed"
    puts e.message
  end

  return result
end


def compute_arguments(measure_dir, osm_path = nil)
  measure_dir = File.absolute_path(measure_dir)
  osm_path = File.absolute_path(osm_path) if osm_path

  result = {}

  begin
    json_request = JSON.generate({:measure_dir => measure_dir, :osm_path => osm_path})
    request = RestClient::Resource.new("#{@host}/compute_arguments", user: @user, password: @pass)
    response = request.post(json_request, content_type: :json, accept: :json)
    result = JSON.parse(response.body, :symbolize_names => true)
  rescue Exception => e
    puts "compute_arguments(#{measure_dir}, #{osm_path}) failed"
    puts e.message
  end

  return result
end

def create_measure(measure_dir, display_name, class_name, taxonomy_tag, measure_type, description, modeler_description)
  measure_dir = File.absolute_path(measure_dir)

  result = {}

  begin
    json_request = JSON.generate({:measure_dir => measure_dir, :display_name => display_name, :class_name => class_name, :taxonomy_tag => taxonomy_tag,:measure_type => measure_type, :description => description, :modeler_description => modeler_description})
    request = RestClient::Resource.new("#{@host}/create_measure", user: @user, password: @pass)
    response = request.post(json_request, content_type: :json, accept: :json)
    result = JSON.parse(response.body, :symbolize_names => true)
  rescue Exception => e
    puts "create_measure(#{measure_dir}) failed"
    puts e.message
  end

  return result
end

def duplicate_measure(old_measure_dir, measure_dir, name, class_name, taxonomy_tag, measure_type, description, modeler_description)
  old_measure_dir = File.absolute_path(old_measure_dir)
  measure_dir = File.absolute_path(measure_dir)

  result = {}

  begin
    json_request = JSON.generate({:old_measure_dir => old_measure_dir, :measure_dir => measure_dir, :name => name, :class_name => class_name, :taxonomy_tag => taxonomy_tag,:measure_type => measure_type, :description => description, :modeler_description => modeler_description})
    request = RestClient::Resource.new("#{@host}/duplicate_measure", user: @user, password: @pass)
    response = request.post(json_request, content_type: :json, accept: :json)
    result = JSON.parse(response.body, :symbolize_names => true)
  rescue Exception => e
    puts "duplicate_measure(#{old_measure_dir}, #{measure_dir}) failed"
    puts e.message
  end

  return result
end

if File.exist?('./output/')
  FileUtils.rm_rf('./output/')
end
FileUtils.mkdir_p('./output/')

osm_path = './output/model.osm'
model = OpenStudio::Model::exampleModel
model.save(osm_path, true)

# these will succeed
puts 1
measures = bcl_measures
#download_bcl_measure('05e20922-6fe4-48a4-bddb-411f8167e757')
measures = bcl_measures
measures = update_measures # defaults to 'My Measure' dir
set({:my_measures_dir => @measure_dir})
measures = update_measures # defaults to 'My Measure' dir
measures = update_measures(@measure_dir)
puts measures
puts 
STDOUT.flush

puts 1.1
measures.each do |measure|
  # these will succeed
  info = compute_arguments(measure[:measure_dir])
  info = compute_arguments(measure[:measure_dir], osm_path)
  puts info
end

measures.each do |measure|
  # these will succeed
  info = compute_arguments(measure[:measure_dir])
  info = compute_arguments(measure[:measure_dir], osm_path)
  puts info
end
puts 
STDOUT.flush

# these will fail because measure does not exist
puts 2
info = compute_arguments('./output/MissingMeasure')
puts info
info = compute_arguments('./output/MissingMeasure', osm_path)
puts info
puts 
STDOUT.flush

# this will succeed
puts 3
measure_dir = './output/NewMeasure'
result = create_measure(measure_dir, "NewMeasure", "NewMeasure", "Envelope.Form", "ModelMeasure", "No description", "No modeler description")
puts result
puts
STDOUT.flush

# this will fail because NewMeasure already exists
puts 4
measure_dir = './output/NewMeasure'
result = create_measure(measure_dir, "NewMeasure", "NewMeasure", "Envelope.Form", "ModelMeasure", "No description", "No modeler description")
puts result
puts
STDOUT.flush

# these will succeed
puts 5
info = compute_arguments('./output/NewMeasure')
puts info
puts
STDOUT.flush

puts 6
info = compute_arguments('./output/NewMeasure', osm_path)
puts info
puts
STDOUT.flush

# this will fail because model does not exist
puts 7
info = compute_arguments('./output/NewMeasure', './output/missing.osm')
puts info
puts
STDOUT.flush

# this will succeed
puts 8
new_measure_dir = './output/NewMeasureCopy'
result = duplicate_measure(measure_dir, new_measure_dir, "NewMeasureCopy", "NewMeasureCopy", "Envelope.Form", "ModelMeasure", "No description again", "No modeler description again")
puts result
puts
STDOUT.flush

# this will fail because NewMeasureCopy already exists
puts 9
new_measure_dir = './output/NewMeasureCopy'
result = duplicate_measure(measure_dir, new_measure_dir, "NewMeasureCopy2", "NewMeasureCopy2", "Envelope.Form", "ModelMeasure", "No description again 2", "No modeler description again 2")
puts result
puts
STDOUT.flush

# this will succeed
puts 10
reset()
puts
STDOUT.flush
