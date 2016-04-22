######################################################################
#  Copyright (c) 2008-2016, Alliance for Sustainable Energy.  
#  All rights reserved.
#  
#  This library is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public
#  License as published by the Free Software Foundation; either
#  version 2.1 of the License, or (at your option) any later version.
#  
#  This library is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#  
#  You should have received a copy of the GNU Lesser General Public
#  License along with this library; if not, write to the Free Software
#  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
######################################################################

require 'thread'
require 'webrick'

require_relative 'measure_manager'

class MeasureManagerServlet < WEBrick::HTTPServlet::AbstractServlet

  @@instance = nil
  
  def initialize(server)
    super
    @mutex = Mutex.new
    @measure_manager = MeasureManager.new
  end
  
  def self.get_instance(server, *options)
    @@instance = self.new(server, *options) if @@instance.nil?
    return @@instance
  end
  
  def do_GET(request, response)
  
    begin
      @mutex.lock
        
      response.status = 200
      response.content_type = 'application/json'
      
      result = {:status => "running"}
      
      case request.path
      when "/internal_state"
        
        osms = []
        @measure_manager.osms.each_pair do |osm_path, value|  
          if value
            osms << {:osm_path => osm_path, :checksum => value[:checksum]}
          end
        end
        
        measures = []
        @measure_manager.measures.each_pair do |measure_dir, measure|  
          if measure
            measures << @measure_manager.measure_hash(measure_dir, measure)
          end
        end
        
        measure_info = []
        @measure_manager.measure_info.each_pair do |measure_dir, value|  
          measure = @measure_manager.measures[measure_dir]
          if measure && value
            value.each_pair do |osm_path, info|
              if info
                temp = @measure_manager.measure_hash(measure_dir, measure, info)
                measure_info << {:measure_dir => measure_dir, :osm_path => osm_path, :arguments => temp[:arguments]}
              end
            end
          end
        end
        
        result[:osms] = osms
        result[:measures] = measures
        result[:measure_info] = measure_info
      end
      
      response.body = JSON.generate(result)
    ensure
      @mutex.unlock
    end
  end
  
  def do_POST (request, response)
  
    begin
      @mutex.lock
        
      response.status = 200
      response.content_type = 'application/json'
      
      case request.path
      when "/reset"
        @measure_manager.reset      
        
        print_message("Reseting internal state")
        
        response.body = JSON.generate({})
        
      when "/update_measures"
        begin
          result = []
          
          data = JSON.parse(request.body, {:symbolize_names=>true})
          measures_dir = data[:measures_dir]
          force_reload = data[:force_reload] ? data[:force_reload] : false

          # loop over all directories
          Dir.glob("#{measures_dir}/*/") do |measure_dir|
          
            measure_dir = File.expand_path(measure_dir)
            if File.directory?(measure_dir)
           
              measure = @measure_manager.get_measure(measure_dir, force_reload)
              if measure.nil?
                print_message("Directory #{measure_dir} is not a measure")
              else
                result << @measure_manager.measure_hash(measure_dir, measure)
              end
            end
          end

          response.body = JSON.generate(result)
        rescue Exception => e  
          response.body = JSON.generate({:error=>e.message, :backtrace=>e.backtrace.inspect})
          response.status = 400
        end
      
      when "/compute_arguments"
        begin
   
          data = JSON.parse(request.body, {:symbolize_names=>true})
          measure_dir  = data[:measure_dir ]
          osm_path = data[:osm_path]
          force_reload = data[:force_reload] ? data[:force_reload] : false

          measure_dir = File.expand_path(measure_dir)
          measure = @measure_manager.get_measure(measure_dir, force_reload)
          if measure.nil?
            raise "Cannot load measure at '#{measure_dir}'"
          end
          
          model = OpenStudio::Model::OptionalModel.new()
          workspace = OpenStudio::OptionalWorkspace.new()
          if osm_path
            osm_path = File.expand_path(osm_path)
            value = @measure_manager.get_model(osm_path, force_reload)
            if value.nil?
              raise "Cannot load model at '#{osm_path}'"
            else
              model = value[0].clone(true).to_Model
              workspace = value[1].clone(true)
            end
          else
            osm_path = ""
          end
          
          info = @measure_manager.get_measure_info(measure_dir, measure, osm_path, model, workspace)
     
          result = @measure_manager.measure_hash(measure_dir, measure, info)

          response.body = JSON.generate(result)
        rescue Exception => e  
          response.body = JSON.generate({:error=>e.message, :backtrace=>e.backtrace.inspect})
          response.status = 400
        end
        
      when "/create_measure"
        begin
          data = JSON.parse(request.body, {:symbolize_names=>true})
          measure_dir = data[:measure_dir]
          name = data[:name]
          class_name = data[:class_name]
          taxonomy_tag = data[:taxonomy_tag]
          measure_type = data[:measure_type]
          description = data[:description]
          modeler_description = data[:modeler_description]
          
          # creating measure will throw if directory exists but is not empty
          measure_dir = File.expand_path(measure_dir)
          OpenStudio::BCLMeasure.new(name, class_name, measure_dir, taxonomy_tag, measure_type.to_MeasureType, description, modeler_description)

          measure = @measure_manager.get_measure(measure_dir, true)
          result = @measure_manager.measure_hash(measure_dir, measure)
          
          response.body = JSON.generate(result)
        rescue Exception => e  
          response.body = JSON.generate({:error=>e.message, :backtrace=>e.backtrace.inspect})
          response.status = 400
        end

      when "/duplicate_measure"
        begin
          data = JSON.parse(request.body, {:symbolize_names=>true})
          old_measure_dir = data[:old_measure_dir]
          measure_dir = data[:measure_dir]
          name = data[:name]
          class_name = data[:class_name]
          taxonomy_tag = data[:taxonomy_tag]
          measure_type = data[:measure_type]
          description = data[:description]
          modeler_description = data[:modeler_description]
          force_reload = data[:force_reload] ? data[:force_reload] : false
          
          old_measure_dir = File.expand_path(old_measure_dir)
          old_measure = @measure_manager.get_measure(old_measure_dir, force_reload)
          if old_measure.nil?
            raise "Cannot load measure at '#{old_measure_dir}'"
          end
          
          measure_dir = File.expand_path(measure_dir)
          new_measure = old_measure.clone(measure_dir)
          if new_measure.empty?
            raise "Cannot copy measure from '#{old_measure_dir}' to '#{measure_dir}'"
          end
          new_measure = new_measure.get
          
          new_measure.updateMeasureScript(old_measure.measureType, measure_type.to_MeasureType,
                                          old_measure.className, class_name,
                                          name, description, modeler_description)
          
          measure = @measure_manager.get_measure(measure_dir, true)
          result = @measure_manager.measure_hash(measure_dir, measure)
          
          response.body = JSON.generate(result)
        rescue Exception => e  
          response.body = JSON.generate({:error=>e.message, :backtrace=>e.backtrace.inspect})
          response.status = 400
        end

      else
        response.body = "Error"
        response.status = 400
      end
    
    ensure
      @mutex.unlock
    end
  end
  
end
