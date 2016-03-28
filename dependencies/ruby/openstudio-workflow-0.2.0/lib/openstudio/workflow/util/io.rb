module OpenStudio
  module Workflow
    module Util
      
      def self.is_windows
        win_patterns = [
            /bccwin/i,
            /cygwin/i,
            /djgpp/i,
            /mingw/i,
            /mswin/i,
            /wince/i,
          ]
	
        case RUBY_PLATFORM
        when *win_patterns
          return true
        else
          return false
        end
      end
      
      def self.popen_command(command)
        result = command
        if is_windows
          result = command.gsub('/', '\\')
        end
        return result
      end
      
    end
  end
end