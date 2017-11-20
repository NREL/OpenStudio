########################################################################################################################
#  OpenStudio(R), Copyright (c) 2008-2017, Alliance for Sustainable Energy, LLC. All rights reserved.
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

require("openstudio")
require("openstudio/sketchup_plugin/lib/AnimationManager")
require("openstudio/sketchup_plugin/lib/CommandManager")
require("openstudio/sketchup_plugin/lib/DialogManager")
require("openstudio/sketchup_plugin/lib/MenuManager")
require("openstudio/sketchup_plugin/lib/ModelManager")
#require("openstudio/sketchup_plugin/lib/SimulationManager")
require("openstudio/sketchup_plugin/lib/ConflictManager")
require("openstudio/sketchup_plugin/lib/UpdateManager")
require("openstudio/sketchup_plugin/lib/WorkspaceObject")
require("openstudio/sketchup_plugin/lib/PluginUserScriptRunner")

require("openstudio/sketchup_plugin/sketchup/UI")
require("openstudio/sketchup_plugin/sketchup/Sketchup")
require("openstudio/sketchup_plugin/sketchup/Geom")

require("fileutils")

$OPENSTUDIO_APPLICATION_DIR
$OPENSTUDIO_SKETCHUPPLUGIN_DIR = File.dirname(__FILE__)

$OPENSTUDIO_SKETCHUPPLUGIN_DEVELOPER_MENU = false # default is false, enable to see developer menu
$OPENSTUDIO_SKETCHUPPLUGIN_PROGRESS_DIALOGS = true # default is true, disable to speed up
$OPENSTUDIO_SKETCHUPPLUGIN_CURRENT_METHOD_NAME = false # default is false, disable to speed up
$OPENSTUDIO_SKETCHUPPLUGIN_LOGGING = false # default is false, disable to speed up
$OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OBSERVERS = true # default is true, disables observers rather than adding/removing as SketchUp does not handle that well
$OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OPERATIONS = false # default is false, enabling operations speeds things up but might introduce problems

if $OPENSTUDIO_SKETCHUPPLUGIN_CURRENT_METHOD_NAME
  # function to return current method name
  def current_method_name
    caller[0]=~/`(.*?)'/
    return "#{self.class}::#{$1}"
    #return "#{self.class}::#{$1}, #{Thread.current}"
  end

  def current_call_stack
    result = ""
    caller.each {|c| result += "#{c}\n"}
    return result
  end
else
  def current_method_name
    return ""
  end

  def current_call_stack
    return ""
  end
end

module OpenStudio

  Platform_Unknown = 0
  Platform_Windows = 1
  Platform_Mac = 2
  
  # PluginManager is an App level class, its members correspond to global variables
  class PluginManager

    attr_reader :name, :version, :dir, :profile_running
    attr_reader :event_queue
    
    attr_accessor :model_manager, :command_manager, :menu_manager, :dialog_manager, :animation_manager, :simulation_manager, :preferences
    attr_accessor :update_manager, :conflict_manager, :load_components, :user_script_runner
  
    def initialize
      @name = $OPENSTUDIO_SKETCHUPPLUGIN_NAME   
      @version = $OPENSTUDIO_SKETCHUPPLUGIN_VERSION
      @dir = File.dirname(__FILE__) + "/.."
      @profile_running = false      
      
      @event_queue = []
      
      @model_manager = ModelManager.new
      
      @user_script_runner = PluginUserScriptRunner.new
      
      # this is needed for profiling results, etc 
      FileUtils.mkdir_p(log_dir)
        
      if $OPENSTUDIO_SKETCHUPPLUGIN_LOGGING
        # set up logging
        OpenStudio::Logger::instance.standardOutLogger.disable
        @log_file = OpenStudio::FileLogSink.new(OpenStudio::Path.new(self.log_path))
        #disable_logging
        
        #@log_file.setLogLevel(OpenStudio::Error)
        #@log_file.setLogLevel(OpenStudio::Warn)
        #@log_file.setLogLevel(OpenStudio::Info)
        #@log_file.setLogLevel(OpenStudio::Debug)
        @log_file.setLogLevel(OpenStudio::Trace)
        #enable_logging # DLM: Todo, comment this out
      end

      self.log(OpenStudio::Info, "OpenStudio Plugin started")
      self.log(OpenStudio::Info, "OpenStudio Plugin Version is #{$OPENSTUDIO_SKETCHUPPLUGIN_VERSION}")
      self.log(OpenStudio::Info, "SketchUp version is #{Sketchup.version}")

      lastversion = read_pref("Plugin Version");
      
      if lastversion.nil? || lastversion == "" || lastversion != @version
        self.log(OpenStudio::Info, "First execution with new OpenStudio version, resetting preferences")
        UI.messagebox("New Plug-in version detected, resetting preferences", MB_OK)
        clear_preferences()
        load_default_preferences()
      end
      
      write_pref("Plugin Version", @version)


    end

    def start
      # 'start' must be separate from 'initialize' because some of the objects below are dependent on the Plugin module constant.
      load_default_preferences

      # Move to ModelInterface
      # DLM: simulations are no longer run from within the plugin
      #@simulation_manager = SimulationManager.new  
      
      # Move to ModelInterface
      @animation_manager = AnimationManager.new

      # Any object containing validation procs that are called by GUIManager must be created before GUIManager is created
      # otherwise this method will fail on the Mac (but is okay on Windows).
      @command_manager = CommandManager.new

      # this will check if linked OpenStudio model and try to open it, otherwise start a new model
      @model_manager.new_from_skp_model(Sketchup.active_model)      
      
      # want to load these after model
      @menu_manager = MenuManager.new
      @dialog_manager = DialogManager.new
      
      # load user scripts, do after creating menus
      @user_script_runner.discover_user_scripts
      
      @update_manager = nil
      if Plugin.read_pref("Check For Update #{self.version}")
        @update_manager = PluginUpdateManager.new("SketchUp Plug-in", false)
      end
      
      @conflict_manager = ConflictManager.new
      proc = Proc.new { @conflict_manager.check_for_conflicts }
      add_event( proc )
      
      if $OPENSTUDIO_SKETCHUPPLUGIN_LAUNCH_GETTING_STARTED_ON_START
        UI.openURL("http://nrel.github.io/OpenStudio-user-documentation/reference/sketchup_plugin_interface/")
      end
      
      @process_events_timer_id = nil
      start_event_processing
    end
    
    def add_event(proc)
      self.log(OpenStudio::Trace, "#{current_method_name}")
      
      self.log(OpenStudio::Debug, "Adding proc #{proc} to event queue")
      
      # DLM: eventually may want to use QCoreApplication::postEvent and QCoreApplication::notify
      # for now maintain two separate event queues
      
      @event_queue << proc
    end
    
    def process_events
      #Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      #if @last_model != Sketchup.active_model
      #  @last_model = Sketchup.active_model
      #  puts "Sketchup.active_model is now #{@last_model}, #{Sketchup.active_model.object_id}, #{Sketchup.active_model.guid}"
      #end
      
      error = false
      error_msg = ""
      
      # loop over each model_interface and delete if no longer valid
      @model_manager.model_interfaces.each do |model_interface|
        if not model_interface.skp_model.valid?
          @model_manager.delete_model_interface(model_interface)
        end
      end
      
      # process events in OpenStudio Model
      # this may add events to the Plugin event_queue
      OpenStudio::Application.instance.processEvents
      
      @model_manager.model_interfaces.each do |model_interface|
        model_interface.model_watcher.processAddedObjects
      end
      
      # process events in SketchUp
      while (not @event_queue.empty?)
      
        # put all @event_queue in current_queue
        current_queue = @event_queue
        @event_queue = []
     
        current_queue.each do |proc| 
        
          begin
            #puts "Calling proc #{proc} in event queue"
            Plugin.log(OpenStudio::Debug, "Calling proc #{proc} in event queue")
            
            # this may add events to @event_queue
            proc.call
            
          rescue StandardError => e
            error = true
            error_msg = e.message
            backtrace = e.backtrace
            
            if !backtrace.empty?
            
              path_line = backtrace[0].split(':')
              if (RUBY_PLATFORM =~ /mswin/ || RUBY_PLATFORM =~ /mingw/)
                if (path_line.length > 1)
                  path = path_line[0] + ':' + path_line[1]  # Colon here is to handle C: in the path
                end
              end
              
              error_msg = "ERROR:\n"
              error_msg += e.class.to_s + "\n"
              error_msg += e.message + "\n"
              error_msg += "BACKTRACE:\n"
              backtrace.each { |stack_call| error_msg += stack_call + "\n" }
            end
            
            SKETCHUP_CONSOLE.show
            Plugin.log(OpenStudio::Error, error_msg)
            puts error_msg
            
          end
        end
        
      end
      
      # loop over each model_interface and paint if needed
      @model_manager.model_interfaces.each do |model_interface|
        if model_interface.paint_requested
          model_interface.paint_now
        end
      end
      
      if error
        msg  = "An error occurred in the OpenStudio SketchUp plug-in.\n\n"
        msg += "It is advised that you save a backup of your current OpenStudio model and restart SketchUp."
        UI.messagebox(msg)
      end

    end
    
    def start_event_processing
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      result = false
      if not @process_events_timer_id
        @process_events_timer_id = UI.start_timer(0.1, true) { self.process_events }
        result = true
      end
      
      return result
    end
    
    def stop_event_processing
      Plugin.log(OpenStudio::Trace, "#{current_method_name}")
      
      result = false
      if @process_events_timer_id
        UI.stop_timer(@process_events_timer_id)
        @process_events_timer_id = nil
        result = true
      end
      
      return result
    end

    def inspect
      return(to_s)
    end
    
    def start_profile
      if not @profile_running
        require("profiler")
        Profiler__::start_profile
        @profile_running = true
      end
    end
    
    def stop_profile
      if @profile_running
        puts "Profiling results in #{log_dir}"
        File.open(log_dir + "/profile.txt", 'w') do |file|
          Profiler__::stop_profile
          Profiler__::print_profile(file)
        end
        @profile_running = false
      end
    end
    
    def mem_profile
      if @mem_hash.nil?
        @mem_hash = Hash.new 
        @mem_hash['index'] = 1
      else
        @mem_hash['index'] = @mem_hash['index'] + 1
      end
      
      ObjectSpace.each_object do |obj|
        if @mem_hash[obj.class].nil?
          @mem_hash[obj.class] = Array.new(@mem_hash['index'], 0)
        elsif @mem_hash[obj.class][@mem_hash['index']-1].nil?
          @mem_hash[obj.class][@mem_hash['index']-1] = 0
        end
        
        @mem_hash[obj.class][@mem_hash['index']-1] = @mem_hash[obj.class][@mem_hash['index']-1] + 1
      end
      
      puts "Memory Profiling results in #{log_dir}"
      File.open(log_dir + "/mem_profile.txt", 'w') do |file|
      
        keys = @mem_hash.keys.sort {|x, y| x.to_s <=> y.to_s}
        
        keys.each do |key|
          next if key == "index"
          file.puts key.to_s + ", " + @mem_hash[key].join(", ") 
        end
      end
    end    
        
    def platform
      if (RUBY_PLATFORM =~ /mswin/ or RUBY_PLATFORM =~ /mingw/)  # Windows
        return(Platform_Windows)
      elsif (RUBY_PLATFORM =~ /darwin/)  # Mac OS X
        return(Platform_Mac)
      else
        return(Platform_Unknown)
      end
    end

    def platform_select(win = nil, mac = win)
      if (RUBY_PLATFORM =~ /mswin/ or RUBY_PLATFORM =~ /mingw/)  # Windows
        return(win)
      elsif (RUBY_PLATFORM =~ /darwin/)  # Mac OS X
        return(mac)
      else
        return(win)
      end
    end

    def read_pref(name)
      return(Sketchup.read_default("OpenStudio", name))
    end

    def write_pref(name, value)
      Sketchup.write_default("OpenStudio", name, value)
    end

    def default_preferences
      hash = Hash.new
      hash["Check For Update #{self.version}"] = true
      hash["New Zone for Space"] = false
      hash["Disable OpenStudio User Scripts"] = false
      hash["Unit System"] = "IP"
      hash['Warn on Idf Export'] = true
      hash['Warn on gbXML Export'] = true
      hash['Warn on SDD Export'] = true
      hash['Show Errors on Idf Translation'] = true
      hash['Show Warnings on Idf Translation'] = true
      hash['Show Errors on gbXML Translation'] = true
      hash['Show Warnings on gbXML Translation'] = true
      hash['Show Errors on SDD Translation'] = true
      hash['Show Warnings on SDD Translation'] = true
      hash['Open Dialogs'] = ""
      hash['Inspector Dialog Visible'] = ""
      
      if (platform == Platform_Windows)
        hash['Text Editor Path'] = "C:/WINDOWS/system32/notepad.exe"
      elsif (platform == Platform_Mac)
        hash['Text Editor Path'] = "/Applications/TextEdit.app"
      end

      return(hash)
    end
    
    # clear all preferences, this should be a complete list
    def clear_preferences
      write_pref("Check For Update #{self.version}", nil)
      write_pref("New Zone for Space", nil)
      write_pref("Disable OpenStudio User Scripts", nil)
      write_pref("Unit System", nil)   
      write_pref("Erase Entities", nil)
      write_pref("Last Construction Sets Import Dir", nil)
      write_pref("Last Constructions Import Dir", nil)
      write_pref("Last Idf Import Dir", nil)
      write_pref("Last OpenStudio Dir", nil)
      write_pref("Last OpenStudio Import Dir", nil)
      write_pref("Last Schedules Import Dir", nil)
      write_pref("Last Space Loads Import Dir", nil)
      write_pref("Open Dialogs", nil)
      write_pref("Text Editor Path", nil)
      write_pref("Warn on Idf Export", nil)
      write_pref("Warn on gbXML Export", nil)
      write_pref("Show Errors on Idf Translation", nil)
      write_pref("Show Warnings on Idf Translation", nil)
      write_pref("Show Errors on gbXML Translation", nil)
      write_pref("Show Warnings on gbXML Translation", nil)
    end

    # Create and set default preferences for any that might not be in the Registry already.
    # For example, the first time the plugin is run, or the first time a new version (with new preferences) is run.
    # Stores values in the Registry at:  HKEY_CURRENT_USER\Software\SketchUp\SketchUp 2016\OpenStudio
    def load_default_preferences
      default_hash = default_preferences
      for key in default_hash.keys
        if (read_pref(key).nil?)
          write_pref(key, default_hash[key])
        end
      end
    end

    def energyplus_version
      return('8.8.0')
    end
    
    def minimal_template_path
      return(Plugin.dir + "/resources/templates/MinimalTemplate.osm")
    end
    
    def empty_template_path
      return(Plugin.dir + "/resources/templates/empty.osm")
    end
    
    def log_dir
      return(OpenStudio::tempDir.to_s + "/OpenStudio/SketchUpPlugin")
    end
    
    def log_path
      return(log_dir + "/SketchUpPlugin.log")
    end
    
    if $OPENSTUDIO_SKETCHUPPLUGIN_LOGGING
      
      def logging_enabled
        return @log_file.isEnabled
      end
        
      def enable_logging
        @log_file.enable    
      end
      
      def disable_logging
        @log_file.disable
      end
    
      def log(level, message)
        if logging_enabled
          #puts "[#{level}] <OpenStudio.SketchUpPlugin> #{message}"
          OpenStudio::logFree(level, "OpenStudio.SketchUpPlugin", message)
        end
      end
      
    else
    
      def logging_enabled
        return false
      end
        
      def enable_logging
      end
      
      def disable_logging
      end
      
      def log(level, message)
      end
      
    end

  end

  if $OPENSTUDIO_SKETCHUPPLUGIN_DEVELOPER_MENU
    # Show the Ruby Console at startup so we can
    # see any programming errors we may make.
    SKETCHUP_CONSOLE.show
  end
  
  # will not get new model on start up
  if $OPENSTUDIO_SKETCHUPPLUGIN_DISABLE_OBSERVERS
    # only ever add one of these, never removed
    Sketchup.add_observer(AppObserver.new) 
  else
    Sketchup.add_observer(AppObserver.new)  
  end
  
  # initialize QApplication
  Application::instance.application(true)
  Application::instance.application.setOrganizationName("NREL")
  Application::instance.application.setOrganizationDomain("nrel.gov")
  Application::instance.application.setApplicationName("OpenStudio")
    
  # get SketchUp Qt Widget if possible
  SketchUpWidget = Application::instance.sketchUpWidget
  SketchUpWidget.hide if SketchUpWidget
  
  # Create a module constant to reference the plugin object anywhere within the module.
  Plugin = PluginManager.new
  Plugin.start
  $p = Plugin

end
