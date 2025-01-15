########################################################################################################################
#  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
#  See also https://openstudio.net/license
########################################################################################################################

require 'thread'
require 'webrick'

require_relative 'measure_manager'

class MeasureManagerServlet < WEBrick::HTTPServlet::AbstractServlet

  @@instance = nil

  def initialize(server)
    super
    @mutex = Mutex.new
    #print_message("new @mutex = #{@mutex}")
    @measure_manager = MeasureManager.new
    @my_measures_dir = File.join(Dir.home, "OpenStudio/Measures/").to_s
  end

  def print_message(message)
    puts message
  end

  def self.get_instance(server, *options)
    @@instance = self.new(server, *options) if @@instance.nil?
    return @@instance
  end

  def do_GET(request, response)

    begin
      #print_message("waiting for @mutex = #{@mutex}")
      @mutex.lock
      #print_message("locked @mutex = #{@mutex}")

      response.status = 200
      response.content_type = 'application/json'

      result = {:status => "running", :my_measures_dir => @my_measures_dir}

      case request.path
      when "/"

        response.body = JSON.generate(result)

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

        response.body = JSON.generate(result)

      else
        response.body = "Error, unknown path #{request.path}"
        response.status = 400
      end

    rescue Exception => e
      response.body = JSON.generate({:error=>e.message, :backtrace=>e.backtrace.inspect})
      response.status = 400

      print_message(e.message)
      print_message(e.backtrace.inspect)
    ensure
      @mutex.unlock
    end
  end

  def do_POST (request, response)

    begin
      #print_message("waiting for @mutex = #{@mutex}")
      @mutex.lock
      #print_message("locked @mutex = #{@mutex}")

      response.status = 200
      response.content_type = 'application/json'

      case request.path
      when "/reset"
        @measure_manager.reset

        print_message("Reseting internal state")

        response.body = JSON.generate({})

      when "/set"

        result = {}

        data = JSON.parse(request.body || "{}", {:symbolize_names=>true})
        my_measures_dir = data[:my_measures_dir]

        if my_measures_dir
          @my_measures_dir = my_measures_dir.to_s
        end

        response.body = JSON.generate(result)

      when "/download_bcl_measure"

        result = []

        data = JSON.parse(request.body || "{}", {:symbolize_names=>true})
        uid = data[:uid]

        if uid
          # each request is handled on a separate thread, need to construct RemoteBCL here
          remote_bcl = OpenStudio::RemoteBCL.new
          measure = remote_bcl.getMeasure(uid)
          if measure.is_initialized
            measure = measure.get
            measure_dir = measure.directory
            result << @measure_manager.measure_hash(measure_dir, measure)
          else
            raise "Failed to download measure '#{uid}'"
          end
        else
          raise "Missing required argument 'uid'"
        end

        response.body = JSON.generate(result)

      when "/bcl_measures"

        result = []

        force_reload = false
        data = JSON.parse(request.body || "{}", {:symbolize_names=>true})

        # loop over all local BCL measures
        OpenStudio::LocalBCL.instance.measures.each do |local_measure|
          measure_dir = local_measure.directory.to_s
          #print_message("measure_dir = #{measure_dir}")
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

      when "/update_measures"

        result = []

        data = JSON.parse(request.body || "{}", {:symbolize_names=>true})
        measures_dir = data[:measures_dir] ? data[:measures_dir] : @my_measures_dir
        force_reload = data[:force_reload] ? data[:force_reload] : false

        # loop over all directories
        measure_dirs = Dir.glob("#{measures_dir}/*/")
        measure_dirs.each do |measure_dir|

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

      when "/compute_arguments"

        data = JSON.parse(request.body || "{}", {:symbolize_names=>true})
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

      when "/create_measure"

        data = JSON.parse(request.body || "{}", {:symbolize_names=>true})
        measure_dir = data[:measure_dir]

        # name = data[:name] # we do not take name as input
        # the measure's name method actually maps to display name
        display_name = data[:display_name]
        class_name = data[:class_name]
        taxonomy_tag = data[:taxonomy_tag]
        measure_type = data[:measure_type]
        measure_language =  data.fetch(:measure_language, 'Ruby') # Fall back
        description = data[:description]
        modeler_description = data[:modeler_description]

        # creating measure will throw if directory exists but is not empty
        measure_dir = File.expand_path(measure_dir)
        OpenStudio::BCLMeasure.new(display_name, class_name, measure_dir, taxonomy_tag, measure_type.to_MeasureType, description, modeler_description, measure_language.to_MeasureLanguage)

        measure = @measure_manager.get_measure(measure_dir, true)
        result = @measure_manager.measure_hash(measure_dir, measure)

        response.body = JSON.generate(result)

      when "/duplicate_measure"

        data = JSON.parse(request.body || "{}", {:symbolize_names=>true})
        old_measure_dir = data[:old_measure_dir]
        measure_dir = data[:measure_dir]

        # name = data[:name] # we do not take name as input
        # the measure's name method actually maps to display name
        display_name = data[:display_name]
        class_name = data[:class_name]
        taxonomy_tag = data[:taxonomy_tag]
        measure_type = data[:measure_type]
        # If not supplied it's fine, it's use old_measure.measureLanguage which
        # if not found defaults to 'Ruby', so fall back is all good
        measure_language = data[:measure_language]
        description = data[:description]
        modeler_description = data[:modeler_description]
        force_reload = data[:force_reload] ? data[:force_reload] : false

        old_measure_dir = File.expand_path(old_measure_dir)
        old_measure = @measure_manager.get_measure(old_measure_dir, force_reload)
        if old_measure.nil?
          raise "Cannot load measure at '#{old_measure_dir}'"
        end

        display_name = old_measure.displayName if display_name.nil?
        class_name = old_measure.className if class_name.nil?
        taxonomy_tag = old_measure.taxonomyTag if taxonomy_tag.nil?
        measure_type = old_measure.measureType.valueName if measure_type.nil?
        measure_language = old_measure.measureLanguage.valueName if measure_language.nil?
        description = old_measure.description if description.nil?
        modeler_description = old_measure.modelerDescription if modeler_description.nil?
        name = OpenStudio::toUnderscoreCase(class_name)

        measure_dir = File.expand_path(measure_dir)
        new_measure = old_measure.clone(measure_dir)
        if new_measure.empty?
          raise "Cannot copy measure from '#{old_measure_dir}' to '#{measure_dir}'"
        end
        new_measure = new_measure.get

        new_measure.changeUID
        new_measure.incrementVersionId

        new_measure.setName(name)
        new_measure.setDisplayName(display_name)
        new_measure.setClassName(class_name)
        new_measure.setTaxonomyTag(taxonomy_tag)
        new_measure.setMeasureType(measure_type.to_MeasureType)
        new_measure.setMeasureLanguage(measure_language.to_MeasureLanguage)
        new_measure.setDescription(description)
        new_measure.setModelerDescription(modeler_description)

        new_measure.updateMeasureScript(old_measure.measureType, measure_type.to_MeasureType,
                                        old_measure.measureLanguage, measure_language.to_MeasureLanguage,
                                        old_measure.className, class_name,
                                        display_name, description, modeler_description)
        new_measure.updateMeasureTests(old_measure.className, class_name)

        new_measure.save

        measure = @measure_manager.get_measure(measure_dir, true)
        result = @measure_manager.measure_hash(measure_dir, measure)

        response.body = JSON.generate(result)

      else
        response.body = "Error, unknown path #{request.path}"
        response.status = 400
      end

    rescue Exception => e
      response.body = JSON.generate({:error=>e.message, :backtrace=>e.backtrace.inspect})
      response.status = 400

      print_message(e.message)
      print_message(e.backtrace.inspect)

    ensure
      @mutex.unlock
    end
  end

end
