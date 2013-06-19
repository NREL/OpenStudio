######################################################################
#  Copyright (c) 2008-2013, Alliance for Sustainable Energy.  
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

module OpenStudio

  class AsynchProc

    def initialize(delay = 0)
    
      seconds = (delay/1000).to_f
      UI.start_timer(seconds, false) {
        begin
          
          # do the proc
          yield

        # Catch the exception here and post it to the ErrorObserver.
        rescue Exception => e
          $exception = $!
          # Most of this code is duplicated in ErrorObserver.detect_errors.
          backtrace = $exception.backtrace
          # NOTE:  There is a difference in backtrace with different versions of the Ruby Interpreter.
          #        V 1.8.0 returns ["file path:line"] for a file or ["(eval):#"] for Ruby Console command line.
          #        V 1.8.6 returns ["file path:line","(eval):#"] for a file or ["(eval):#"] for Ruby Console command line.
          path_line = backtrace[0].split(':')
          if (path_line.length > 1)
            path = path_line[0] + ':' + path_line[1]  # Colon here is to handle C: in the path
            msg = "\nException in AsynchProc!\n"
            msg += "ERROR:\n"
            msg += $exception.class.to_s + "\n"
            msg += $exception.message + "\n"
            msg += "BACKTRACE:\n"
            $exception.backtrace.each { |stack_call| msg += stack_call + "\n" }
            puts msg
          end
          
        ensure
        
        end
      }
      
    end

  end

end
