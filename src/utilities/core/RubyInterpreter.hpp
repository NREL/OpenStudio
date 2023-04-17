/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2023, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
*
*  Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
*  following conditions are met:
*
*  (1) Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*  disclaimer.
*
*  (2) Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following
*  disclaimer in the documentation and/or other materials provided with the distribution.
*
*  (3) Neither the name of the copyright holder nor the names of any contributors may be used to endorse or promote products
*  derived from this software without specific prior written permission from the respective party.
*
*  (4) Other than as required in clauses (1) and (2), distributions in any form of modifications or other derivative works
*  may not use the "OpenStudio" trademark, "OS", "os", or any other confusingly similar designation without specific prior
*  written permission from Alliance for Sustainable Energy, LLC.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDER(S) AND ANY CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
*  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER(S), ANY CONTRIBUTORS, THE UNITED STATES GOVERNMENT, OR THE UNITED
*  STATES DEPARTMENT OF ENERGY, NOR ANY OF THEIR EMPLOYEES, BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
*  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
*  USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
*  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
*  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***********************************************************************************************************************/

#ifndef UTILITIES_CORE_RUBYINTERPRETER_HPP
#define UTILITIES_CORE_RUBYINTERPRETER_HPP

#include "RubyException.hpp"

// SWIGRubyRuntime.hxx includes ruby.h which includes ruby/win32.h, which has some brain damaged notions of
// what standard errno values should be. We solved this in systemoutliner with some creative
// compilation firewalls and opaque types and such. We don't have quite as much flexibility here
// because we want to use templates to give us type safety and return types and such.

// So, we're going to go a bit more brute force with it.

#ifdef EWOULDBLOCK
#  define OLD_EWOULDBLOCK EWOULDBLOCK
#  undef EWOULDBLOCK
#endif

#ifdef EINPROGRESS
#  define OLD_EINPROGRESS EINPROGRESS
#  undef EINPROGRESS
#endif

#ifdef EALREADY
#  define OLD_EALREADY EALREADY
#  undef EALREADY
#endif

#ifdef ENOTSOCK
#  define OLD_ENOTSOCK ENOTSOCK
#  undef ENOTSOCK
#endif

#ifdef EDESTADDRREQ
#  define OLD_EDESTADDRREQ EDESTADDRREQ
#  undef EDESTADDRREQ
#endif

#ifdef EMSGSIZE
#  define OLD_EMSGSIZE EMSGSIZE
#  undef EMSGSIZE
#endif

#ifdef EPROTOTYPE
#  define OLD_EPROTOTYPE EPROTOTYPE
#  undef EPROTOTYPE
#endif

#ifdef ENOPROTOOPT
#  define OLD_ENOPROTOOPT ENOPROTOOPT
#  undef ENOPROTOOPT
#endif

#ifdef EPROTONOSUPPORT
#  define OLD_EPROTONOSUPPORT EPROTONOSUPPORT
#  undef EPROTONOSUPPORT
#endif

#ifdef EOPNOTSUPP
#  define OLD_EOPNOTSUPP EOPNOTSUPP
#  undef EOPNOTSUPP
#endif

#ifdef EAFNOSUPPORT
#  define OLD_EAFNOSUPPORT EAFNOSUPPORT
#  undef EAFNOSUPPORT
#endif

#ifdef EADDRINUSE
#  define OLD_EADDRINUSE EADDRINUSE
#  undef EADDRINUSE
#endif

#ifdef EADDRNOTAVAIL
#  define OLD_EADDRNOTAVAIL EADDRNOTAVAIL
#  undef EADDRNOTAVAIL
#endif

#ifdef ENETDOWN
#  define OLD_ENETDOWN ENETDOWN
#  undef ENETDOWN
#endif

#ifdef ENETUNREACH
#  define OLD_ENETUNREACH ENETUNREACH
#  undef ENETUNREACH
#endif

#ifdef ENETRESET
#  define OLD_ENETRESET ENETRESET
#  undef ENETRESET
#endif

#ifdef ECONNABORTED
#  define OLD_ECONNABORTED ECONNABORTED
#  undef ECONNABORTED
#endif

#ifdef ECONNRESET
#  define OLD_ECONNRESET ECONNRESET
#  undef ECONNRESET
#endif

#ifdef ENOBUFS
#  define OLD_ENOBUFS ENOBUFS
#  undef ENOBUFS
#endif

#ifdef EISCONN
#  define OLD_EISCONN EISCONN
#  undef EISCONN
#endif

#ifdef ENOTCONN
#  define OLD_ENOTCONN ENOTCONN
#  undef ENOTCONN
#endif

#ifdef ETIMEDOUT
#  define OLD_ETIMEDOUT ETIMEDOUT
#  undef ETIMEDOUT
#endif

#ifdef ECONNREFUSED
#  define OLD_ECONNREFUSED ECONNREFUSED
#  undef ECONNREFUSED
#endif

#ifdef ELOOP
#  define OLD_ELOOP ELOOP
#  undef ELOOP
#endif

#ifdef EHOSTUNREACH
#  define OLD_EHOSTUNREACH EHOSTUNREACH
#  undef EHOSTUNREACH
#endif

#ifdef try
#  define OLD_try try
#  undef try
#endif

#ifdef except
#  define OLD_except except
#  undef except
#endif

#ifdef finally
#  define OLD_finally finally
#  undef finally
#endif

#ifdef leave
#  define OLD_leave leave
#  undef leave
#endif

#ifdef OpenFile
#  define OLD_OpenFile OpenFile
#  undef OpenFile
#endif

#ifdef CharNext
#  define OLD_CharNext CharNext
#  undef CharNext
#endif

#ifdef rb_w32_iswinnt
#  define OLD_rb_w32_iswinnt rb_w32_iswinnt
#  undef rb_w32_iswinnt
#endif

#ifdef rb_w32_iswin95
#  define OLD_rb_w32_iswin95 rb_w32_iswin95
#  undef rb_w32_iswin95
#endif

#ifdef rb_w32_iswinnt
#  define OLD_rb_w32_iswinnt rb_w32_iswinnt
#  undef rb_w32_iswinnt
#endif

#ifdef rb_w32_iswin95
#  define OLD_rb_w32_iswin95 rb_w32_iswin95
#  undef rb_w32_iswin95
#endif

#ifdef getc
#  define OLD_getc getc
#  undef getc
#endif

#ifdef putc
#  define OLD_putc putc
#  undef putc
#endif

#ifdef fgetc
#  define OLD_fgetc fgetc
#  undef fgetc
#endif

#ifdef fputc
#  define OLD_fputc fputc
#  undef fputc
#endif

#ifdef getchar
#  define OLD_getchar getchar
#  undef getchar
#endif

#ifdef putchar
#  define OLD_putchar putchar
#  undef putchar
#endif

#ifdef fgetchar
#  define OLD_fgetchar fgetchar
#  undef fgetchar
#endif

#ifdef fputchar
#  define OLD_fputchar fputchar
#  undef fputchar
#endif

#ifdef utime
#  define OLD_utime utime
#  undef utime
#endif

#ifdef strcasecmp
#  define OLD_strcasecmp strcasecmp
#  undef strcasecmp
#endif

#ifdef strncasecmp
#  define OLD_strncasecmp strncasecmp
#  undef strncasecmp
#endif

#ifdef close
#  define OLD_close close
#  undef close
#endif

#ifdef fclose
#  define OLD_fclose fclose
#  undef fclose
#endif

#ifdef read
#  define OLD_read read
#  undef read
#endif

#ifdef write
#  define OLD_write write
#  undef write
#endif

#ifdef getpid
#  define OLD_getpid getpid
#  undef getpid
#endif

#ifdef sleep
#  define OLD_sleep sleep
#  undef sleep
#endif

#ifdef creat
#  define OLD_creat creat
#  undef creat
#endif

#ifdef eof
#  define OLD_eof eof
#  undef eof
#endif

#ifdef filelength
#  define OLD_filelength filelength
#  undef filelength
#endif

#ifdef mktemp
#  define OLD_mktemp mktemp
#  undef mktemp
#endif

#ifdef tell
#  define OLD_tell tell
#  undef tell
#endif

#ifdef unlink
#  define OLD_unlink unlink
#  undef unlink
#endif

#ifdef _open
#  define OLD__open _open
#  undef _open
#endif

#ifdef sopen
#  define OLD_sopen sopen
#  undef sopen
#endif

#ifdef fopen
#  define OLD_fopen fopen
#  undef fopen
#endif

#ifdef fdopen
#  define OLD_fdopen fdopen
#  undef fdopen
#endif

#ifdef fsopen
#  define OLD_fsopen fsopen
#  undef fsopen
#endif

#ifdef fsync
#  define OLD_fsync fsync
#  undef fsync
#endif

#ifdef stat
#  define OLD_stat stat
#  undef stat
#endif

#ifdef execv
#  define OLD_execv execv
#  undef execv
#endif

#ifdef isatty
#  define OLD_isatty isatty
#  undef isatty
#endif

#ifdef mkdir
#  define OLD_mkdir mkdir
#  undef mkdir
#endif

#ifdef rmdir
#  define OLD_rmdir rmdir
#  undef rmdir
#endif

#ifdef isascii
#  define OLD_isascii isascii
#  undef isascii
#endif

#ifdef vsnprintf
#  define OLD_vsnprintf vsnprintf
#  undef vsnprintf
#endif

#ifdef snprintf
#  define OLD_snprintf snprintf
#  undef snprintf
#endif

#ifdef isnan
#  define OLD_isnan isnan
#  undef isnan
#endif

#ifdef finite
#  define OLD_finite finite
#  undef finite
#endif

#ifdef copysign
#  define OLD_copysign copysign
#  undef copysign
#endif

#ifdef scalb
#  define OLD_scalb scalb
#  undef scalb
#endif

#ifdef accept
#  define OLD_accept accept
#  undef accept
#endif

#ifdef bind
#  define OLD_bind bind
#  undef bind
#endif

#ifdef connect
#  define OLD_connect connect
#  undef connect
#endif

#ifdef FD_SET
#  define OLD_FD_SET FD_SET
#  undef FD_SET
#endif

#ifdef FD_CLR
#  define OLD_FD_CLR FD_CLR
#  undef FD_CLR
#endif

#ifdef FD_ISSET
#  define OLD_FD_ISSET FD_ISSET
#  undef FD_ISSET
#endif

#ifdef select
#  define OLD_select select
#  undef select
#endif

#ifdef getpeername
#  define OLD_getpeername getpeername
#  undef getpeername
#endif

#ifdef getsockname
#  define OLD_getsockname getsockname
#  undef getsockname
#endif

#ifdef getsockopt
#  define OLD_getsockopt getsockopt
#  undef getsockopt
#endif

#ifdef ioctlsocket
#  define OLD_ioctlsocket ioctlsocket
#  undef ioctlsocket
#endif

#ifdef listen
#  define OLD_listen listen
#  undef listen
#endif

#ifdef recv
#  define OLD_recv recv
#  undef recv
#endif

#ifdef recvfrom
#  define OLD_recvfrom recvfrom
#  undef recvfrom
#endif

#ifdef send
#  define OLD_send send
#  undef send
#endif

#ifdef sendto
#  define OLD_sendto sendto
#  undef sendto
#endif

#ifdef setsockopt
#  define OLD_setsockopt setsockopt
#  undef setsockopt
#endif

#ifdef shutdown
#  define OLD_shutdown shutdown
#  undef shutdown
#endif

#ifdef socket
#  define OLD_socket socket
#  undef socket
#endif

#ifdef gethostbyaddr
#  define OLD_gethostbyaddr gethostbyaddr
#  undef gethostbyaddr
#endif

#ifdef gethostbyname
#  define OLD_gethostbyname gethostbyname
#  undef gethostbyname
#endif

#ifdef gethostname
#  define OLD_gethostname gethostname
#  undef gethostname
#endif

#ifdef getprotobyname
#  define OLD_getprotobyname getprotobyname
#  undef getprotobyname
#endif

#ifdef getprotobynumber
#  define OLD_getprotobynumber getprotobynumber
#  undef getprotobynumber
#endif

#ifdef getservbyname
#  define OLD_getservbyname getservbyname
#  undef getservbyname
#endif

#ifdef getservbyport
#  define OLD_getservbyport getservbyport
#  undef getservbyport
#endif

#ifdef get_osfhandle
#  define OLD_get_osfhandle get_osfhandle
#  undef get_osfhandle
#endif

#ifdef getcwd
#  define OLD_getcwd getcwd
#  undef getcwd
#endif

#ifdef getenv
#  define OLD_getenv getenv
#  undef getenv
#endif

#ifdef rename
#  define OLD_rename rename
#  undef rename
#endif

#ifdef times
#  define OLD_times times
#  undef times
#endif

#ifdef Sleep
#  define OLD_Sleep Sleep
#  undef Sleep
#endif

// Include the SWIGRubyRuntime.hxx generated by the build of openstudio::utilities to let it live
// in a known shared place.
#include <utilities/SWIGRubyRuntime.hxx>

//#include <ruby/encoding.h>

#include "Path.hpp"

#include <vector>
#include <string>

namespace openstudio {
namespace detail  // I like to "hide" things I don't want regular uses to use in detail namespaces
{

  class RubyInterpreter
  {
   private:
    // dummy arguments passed to ruby_options
    int m_argc;
    char** m_argv;

    static void addIncludePath(std::vector<std::string>& includePaths, const openstudio::path& includePath) {
      includePaths.push_back("-I");
      includePaths.push_back(toString(includePath));
    }

   public:
    RubyInterpreter(const openstudio::path& t_moduleSearchPath, const openstudio::path& t_rubyIncludePath,
                    const std::vector<std::string>& t_moduleNames) {

      // set load paths
      std::vector<std::string> rubyArgs;
      openstudio::path rubypath = openstudio::getOpenStudioEmbeddedRubyPath();

      rubyArgs.emplace_back("OpenStudio");
      rubyArgs.emplace_back("-EUTF-8");

      addIncludePath(rubyArgs, t_moduleSearchPath);
      addIncludePath(rubyArgs, t_rubyIncludePath);

      if (!rubypath.empty()) {
#if defined(WIN32)
#  if (defined(_M_X64) || defined(__amd64__))
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/site_ruby/2.0.0"));
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/site_ruby/2.0.0/x64-msvcr100"));  // DLM: remove?
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/site_ruby/2.0.0/x64-msvcrt"));
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/site_ruby"));
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/vendor_ruby/2.0.0"));
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/vendor_ruby/2.0.0/x64-msvcr100"));  // DLM: remove?
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/vendor_ruby/2.0.0/x64-msvcrt"));
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/vendor_ruby"));
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/2.0.0"));
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/2.0.0/x64-mswin64_10"));  // DLM: remove?
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/2.0.0/x64-mingw32"));
#  else
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/site_ruby/2.0.0"));
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/site_ruby/2.0.0/i386-mingw32"));  // DLM: remove?
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/site_ruby/2.0.0/i386-msvcrt"));
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/site_ruby"));
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/vendor_ruby/2.0.0"));
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/vendor_ruby/2.0.0/i386-mingw32"));  // DLM: remove?
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/vendor_ruby/2.0.0/i386-msvcrt"));
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/vendor_ruby"));
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/2.0.0"));
        addIncludePath(rubyArgs, rubypath / openstudio::toPath("lib/ruby/2.0.0/i386-mingw32"));  // DLM: same for both builds
#  endif
#endif
      }

      // and now give the interpreter something to parse, so that it doesn't sit
      // waiting on stdin from us
      rubyArgs.emplace_back("-e");
      rubyArgs.emplace_back("");

      m_argc = static_cast<int>(rubyArgs.size());
      m_argv = new char*[m_argc];

      for (int i = 0; i < m_argc; ++i) {
        m_argv[i] = new char[rubyArgs[i].size() + 1];
        strcpy(m_argv[i], rubyArgs[i].c_str());
      }

      // the return value of ruby_options is the parsed node of our "script"
      // from the -e "" we passed in. This could be used to actually parse / eval something if we wanted
      ruby_options(m_argc, m_argv);

      // load the modules. If an error occurs, an exception will be thrown explaining the problem
      for (auto itr = t_moduleNames.begin(); itr != t_moduleNames.end(); ++itr) {
        evalString("require '" + openstudio::toString(t_moduleSearchPath / openstudio::toPath(*itr)) + "'");
      }

      // "typedefs" for backwards compatibility
      // keep synchronized with \openstudiocore\ruby\init_openstudio.cpp
      std::string ruby_typedef_script = R"END(
module OpenStudio
module Ruleset

  # support for name deprecated as of 0.10.1
  class UserScriptArgument < OpenStudio::Measure::OSArgument
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "UserScriptArgument is deprecated, use OpenStudio::Measure::Argument instead.")
      super
    end
  end

  # support for name deprecated as of 0.10.1
  class OptionalUserScriptArgument < OpenStudio::Measure::OptionalOSArgument
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OptionalUserScriptArgument is deprecated, use OpenStudio::Measure::OptionalOSArgument instead.")
      super
    end
  end

  # support for name deprecated as of 0.10.1
  class UserScriptArgumentVector < OpenStudio::Measure::OSArgumentVector
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "UserScriptArgumentVector is deprecated, use OpenStudio::Measure::OSArgumentVector instead.")
      super
    end
  end

  # support for name deprecated as of 0.10.1
  class UserScriptArgumentMap < OpenStudio::Measure::OSArgumentMap
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "UserScriptArgumentMap is deprecated, use OpenStudio::Measure::OSArgumentMap instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  class UserScript < OpenStudio::Measure::OSMeasure
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "UserScript is deprecated, use OpenStudio::Measure::OSMeasure instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  class ModelUserScript < OpenStudio::Measure::ModelMeasure
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "ModelUserScript is deprecated, use OpenStudio::Measure::ModelMeasure instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  class WorkspaceUserScript < OpenStudio::Measure::EnergyPlusMeasure
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "WorkspaceUserScript is deprecated, use OpenStudio::Measure::EnergyPlusMeasure instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  class ReportingUserScript < OpenStudio::Measure::ReportingMeasure
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "ReportingUserScript is deprecated, use OpenStudio::Measure::ReportingMeasure instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  class OSArgument < OpenStudio::Measure::OSArgument
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSArgument is deprecated, use OpenStudio::Measure::OSArgument instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  def self.convertOSArgumentVectorToMap(argument_vector)
    OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OpenStudio::Ruleset::convertOSArgumentVectorToMap is deprecated, use OpenStudio::Measure::convertOSArgumentVectorToMap instead.")
    return OpenStudio::Measure::convertOSArgumentVectorToMap(argument_vector)
  end

  # support for name deprecated as of 2.0.0
  class OSArgumentVector < OpenStudio::Measure::OSArgumentVector
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSArgumentVector is deprecated, use OpenStudio::Measure::OSArgumentVector instead.")
      super
    end
  end

  # support for name deprecated as of 2.0.0
  class OSArgumentMap < OpenStudio::Measure::OSArgumentMap
    def initialize
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSArgumentMap is deprecated, use OpenStudio::Measure::OSArgumentMap instead.")
      super
    end
  end

  # class was replaced by OpenStudio::WorkflowStepResult
#  # support for name deprecated as of 2.0.0
#  class OSResult < OpenStudio::Measure::OSResult
#    def initialize
#      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSResult is deprecated, use OpenStudio::Measure::OSResult instead.")
#      super
#    end
#  end
#
#  # support for name deprecated as of 2.0.0
#  class OSResultVector < OpenStudio::Measure::OSResultVector
#    def initialize
#      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSResultVector is deprecated, use OpenStudio::Measure::OSResultVector instead.")
#      super
#    end
#  end

  # support for name deprecated as of 2.0.0
  class OSRunner < OpenStudio::Measure::OSRunner
    def initialize(workflow_json = nil)
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "OSRunner is deprecated, use OpenStudio::Measure::OSRunner instead.")
      if workflow_json.nil?
        workflow_json = OpenStudio::WorkflowJSON.new
        OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "No workflow provided, using empty WorkflowJSON.")
      end
      super(workflow_json)
    end
  end

  # support for name deprecated as of 2.0.0
  class RubyUserScriptInfo < OpenStudio::Measure::OSMeasureInfo
    def initialize(*args)
      OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "RubyUserScriptInfo is deprecated, use OpenStudio::Measure::OSMeasureInfo instead.")
      if args.size == 1
        super(args[0])
      elsif args.size == 8
        super(args[0],args[1],args[2],args[3],args[4],args[5],args[6],args[7],args[8])
      end
    end
  end

  # support for name deprecated as of 2.0.0
  def self.infoExtractorRubyFunction
    OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "Ruleset is deprecated, use OpenStudio::Measure::infoExtractorRubyFunction instead.")
    return OpenStudio::Measure.infoExtractorRubyFunction
  end

  # support for name deprecated as of 2.0.0
  def self.getInfo(measure, model, workspace)
    OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "Ruleset is deprecated, use OpenStudio::Measure::getInfo instead.")
    return OpenStudio::Measure.getInfo(measure, model, workspace)
  end

  # support for name deprecated as of 2.0.0
  def self.makeChoiceArgumentOfWorkspaceObjects(name, iddObjectType, workspace, required=true)
    OpenStudio::logFree(OpenStudio::Warn, "OpenStudio.Measure", "Ruleset is deprecated, use OpenStudio::Measure::makeChoiceArgumentOfWorkspaceObjects instead.")
    return OpenStudio::Measure.makeChoiceArgumentOfWorkspaceObjects(name, iddObjectType, workspace, required)
  end

end # module Ruleset
end # module OpenStudio

module OpenStudio
  def self.getSharedResourcesPath()
    OpenStudio::logFree(OpenStudio::Warn, "OpenStudio", "getSharedResourcesPath is deprecated.")
    return OpenStudio::Path.new()
  end
end # module OpenStudio

)END";

      evalString(ruby_typedef_script);

      // register some default types that we want to pass in / out of the ruby system
      registerType<int>("int");
      registerType<long>("long");
      registerType<size_t>("size_t");
      registerType<double>("double");
      registerType<std::string>("std::string");
      registerType<float>("float");
      registerType<openstudio::path>("openstudio::path");

      // set the script name if desired
      //ruby_script("OpenStudio");
    }

    ~RubyInterpreter() {
      /// seems there is nothing to do
      for (int i = 0; i < m_argc; i++) {
        delete[] m_argv[i];
      }
      delete[] m_argv;
    }

    /// Register a type along with its vector and optional versions so that it can be used
    /// from within the Ruby wrapper. These are necessary because there's no way to divine
    /// the unmangled type name.
    template <typename Type>
    void registerType(const std::string& t_name) {
      m_types[typeid(Type).name()] = t_name;
      m_types[typeid(Type*).name()] = t_name + " *";
      m_types[typeid(std::vector<Type>).name()] = "std::vector<" + t_name + ">";
      m_types[typeid(std::vector<Type>*).name()] = "std::vector<" + t_name + "> *";
      m_types[typeid(boost::optional<Type>).name()] = "boost::optional<" + t_name + ">";
      m_types[typeid(boost::optional<Type>*).name()] = "boost::optional<" + t_name + "> *";
    }

    // evaluate a ruby statement with no return value. If a ruby exception is raised
    // the description is translated into a C++ exception, which is thrown as an openstudio::RubyException.
    void evalString(const std::string& t_str) {

      VALUE val = rb_str_new2(t_str.c_str());
      int error;

      // save and restore the current working directory in case the call to ruby upsets it
      const auto cwd = openstudio::filesystem::current_path();
      rb_protect(evaluateSimpleImpl, val, &error);
      openstudio::filesystem::current_path(cwd);

      if (error != 0) {
        VALUE errval = rb_eval_string("$!.to_s");
        char* str = StringValuePtr(errval);
        std::string err(str);

        VALUE locval = rb_eval_string("$@.to_s");
        str = StringValuePtr(locval);
        std::string loc(str);

        throw RubyException(err, loc);
      }
    }

    /// Execute a function by name with 0 parameters and no return value
    template <typename ReturnType>
    void exec(const std::string& t_functionName) {
      std::vector<VALUE> params;

      makeProtectedCall(t_functionName, params);
    }

    /// Execute a function by name with 1 parameter and no return value
    template <typename ReturnType, typename Param1>
    void exec(const std::string& t_functionName, Param1 t_param1) {
      std::vector<VALUE> params;
      params.push_back(newPointerObj(&t_param1));

      makeProtectedCall(t_functionName, params);
    }

    /// Execute a function by name with 2 parameters and no return value
    template <typename ReturnType, typename Param1, typename Param2>
    ReturnType exec(const std::string& t_functionName, Param1 t_param1, Param2 t_param2) {
      std::vector<VALUE> params;
      params.push_back(newPointerObj(&t_param1));
      params.push_back(newPointerObj(&t_param2));

      makeProtectedCall(t_functionName, params);
    }

    /// Execute a function by name with 3 parameters and no return value
    template <typename ReturnType, typename Param1, typename Param2, typename Param3>
    ReturnType exec(const std::string& t_functionName, Param1 t_param1, Param2 t_param2, Param3 t_param3) {
      std::vector<VALUE> params;
      params.push_back(newPointerObj(&t_param1));
      params.push_back(newPointerObj(&t_param2));
      params.push_back(newPointerObj(&t_param3));

      makeProtectedCall(t_functionName, params);
    }

    /// Execute a function by name with 0 parameters and a return value
    template <typename ReturnType>
    ReturnType execWithReturn(const std::string& t_functionName) {
      std::vector<VALUE> params;

      VALUE retval = makeProtectedCall(t_functionName, params);
      return asType<ReturnType>(retval);
    }

    /// Execute a function by name with 1 parameter and a return value
    template <typename ReturnType, typename Param1>
    ReturnType execWithReturn(const std::string& t_functionName, Param1 t_param1) {
      std::vector<VALUE> params;
      params.push_back(newPointerObj(&t_param1));

      VALUE retval = makeProtectedCall(t_functionName, params);
      return asType<ReturnType>(retval);
    }

    /// Execute a function by name with 2 parameters and a return value
    template <typename ReturnType, typename Param1, typename Param2>
    ReturnType execWithReturn(const std::string& t_functionName, Param1 t_param1, Param2 t_param2) {
      std::vector<VALUE> params;
      params.push_back(newPointerObj(&t_param1));
      params.push_back(newPointerObj(&t_param2));

      VALUE retval = makeProtectedCall(t_functionName, params);
      return asType<ReturnType>(retval);
    }

    /// Execute a function by name with 3 parameters and a return value
    template <typename ReturnType, typename Param1, typename Param2, typename Param3>
    ReturnType execWithReturn(const std::string& t_functionName, Param1 t_param1, Param2 t_param2, Param3 t_param3) {
      std::vector<VALUE> params;
      params.push_back(newPointerObj(&t_param1));
      params.push_back(newPointerObj(&t_param2));
      params.push_back(newPointerObj(&t_param3));

      VALUE retval = makeProtectedCall(t_functionName, params);
      return asType<ReturnType>(retval);
    }

    /// Add a named global object to the ruby runtime
    template <typename ObjectType>
    void addObject(const std::string& t_objectName, ObjectType* t_obj) {
      VALUE r_obj = SWIG_NewPointerObj(t_obj, getTypeInfo<ObjectType*>(), 0);
      rb_define_variable(t_objectName.c_str(), &r_obj);
    }

   private:
    // explicitly unimplemented copy and assignment operators
    RubyInterpreter& operator=(const RubyInterpreter&);
    RubyInterpreter(const RubyInterpreter&);

    std::map<std::string, std::string> m_types;

    // Used for our rb_protect calls.
    static VALUE evaluateSimpleImpl(VALUE arg) {
      return rb_eval_string(StringValuePtr(arg));
    }

    // Returns the swig_type_info for the templated type if it's been registered previously
    // with registerType
    template <typename Type>
    swig_type_info* getTypeInfo() const {
      auto itr = m_types.find(typeid(Type).name());

      if (itr == m_types.end()) {
        throw std::runtime_error(std::string("Type has not been registered: ") + typeid(Type).name());
      }

      std::string t_typeName = itr->second;

      swig_type_info* sti = SWIG_TypeQuery(t_typeName.c_str());

      if (sti == nullptr) {
        throw std::runtime_error("Unable to lookup type info for type: " + t_typeName);
      }

      return sti;
    }

    VALUE makeProtectedCall(const std::string& t_functionName, std::vector<VALUE>& t_params) {
      // we go through all of this because we cannot rb_protect a call to
      // rb_funcall. At least not in any way I can find.
      std::stringstream params;

      for (size_t i = 0; i < t_params.size(); ++i) {
        std::stringstream ss;
        ss << "$embedded_ruby_param_" << i;

        rb_define_variable(ss.str().c_str(), &t_params[i]);

        params << ss.str();
        if (i < t_params.size() - 1) {
          params << ", ";
        }
      }

      std::string funcall = "$embedded_ruby_return = " + t_functionName + "(" + params.str() + ")";
      evalString(funcall);

      VALUE retval = rb_gv_get("$embedded_ruby_return");
      return retval;
    }

    template <typename Param>
    VALUE newPointerObj(Param t_param) {
      swig_type_info* ti = getTypeInfo<Param>();
      return SWIG_NewPointerObj(t_param, ti, 0);
    }

    VALUE newPointerObj(double* d) {
      return rb_float_new(*d);
    }

    VALUE newPointerObj(float* d) {
      return rb_float_new(*d);
    }

    VALUE newPointerObj(int* d) {
      return rb_to_int(rb_float_new(*d));
    }

    VALUE newPointerObj(long* d) {
      return rb_to_int(rb_float_new(*d));
    }

    VALUE newPointerObj(std::string* s) {
      return rb_str_new2(s->c_str());
    }

    template <typename Type>
    Type asType(VALUE v) {
      Type* ptr = nullptr;

      int res = SWIG_ConvertPtr(v, reinterpret_cast<void**>(&ptr), getTypeInfo<Type*>(), 0);
      if (SWIG_IsOK(res)) {
        if (!ptr) {
          throw std::runtime_error(std::string("Result is null, trying to get type: ") + typeid(Type).name());
        }

        if (SWIG_IsNewObj(res)) {
          Type obj = *ptr;
          delete ptr;
          return obj;
        } else {
          return *ptr;
        }
      } else {
        throw std::runtime_error(std::string("Unable to convert object to type: ") + typeid(Type).name());
      }
    }
  };
}  // namespace detail
}  // namespace openstudio

#ifdef OLD_EWOULDBLOCK
#  undef EWOULDBLOCK
#  define EWOULDBLOCK OLD_EWOULDBLOCK
#  undef OLD_EWOULDBLOCK
#endif

#ifdef OLD_EINPROGRESS
#  undef EINPROGRESS
#  define EINPROGRESS OLD_EINPROGRESS
#  undef OLD_EINPROGRESS
#endif

#ifdef OLD_EALREADY
#  undef EALREADY
#  define EALREADY OLD_EALREADY
#  undef OLD_EALREADY
#endif

#ifdef OLD_ENOTSOCK
#  undef ENOTSOCK
#  define ENOTSOCK OLD_ENOTSOCK
#  undef OLD_ENOTSOCK
#endif

#ifdef OLD_EDESTADDRREQ
#  undef EDESTADDRREQ
#  define EDESTADDRREQ OLD_EDESTADDRREQ
#  undef OLD_EDESTADDRREQ
#endif

#ifdef OLD_EMSGSIZE
#  undef EMSGSIZE
#  define EMSGSIZE OLD_EMSGSIZE
#  undef OLD_EMSGSIZE
#endif

#ifdef OLD_EPROTOTYPE
#  undef EPROTOTYPE
#  define EPROTOTYPE OLD_EPROTOTYPE
#  undef OLD_EPROTOTYPE
#endif

#ifdef OLD_ENOPROTOOPT
#  undef ENOPROTOOPT
#  define ENOPROTOOPT OLD_ENOPROTOOPT
#  undef OLD_ENOPROTOOPT
#endif

#ifdef OLD_EPROTONOSUPPORT
#  undef EPROTONOSUPPORT
#  define EPROTONOSUPPORT OLD_EPROTONOSUPPORT
#  undef OLD_EPROTONOSUPPORT
#endif

#ifdef OLD_EOPNOTSUPP
#  undef EOPNOTSUPP
#  define EOPNOTSUPP OLD_EOPNOTSUPP
#  undef OLD_EOPNOTSUPP
#endif

#ifdef OLD_EAFNOSUPPORT
#  undef EAFNOSUPPORT
#  define EAFNOSUPPORT OLD_EAFNOSUPPORT
#  undef OLD_EAFNOSUPPORT
#endif

#ifdef OLD_EADDRINUSE
#  undef EADDRINUSE
#  define EADDRINUSE OLD_EADDRINUSE
#  undef OLD_EADDRINUSE
#endif

#ifdef OLD_EADDRNOTAVAIL
#  undef EADDRNOTAVAIL
#  define EADDRNOTAVAIL OLD_EADDRNOTAVAIL
#  undef OLD_EADDRNOTAVAIL
#endif

#ifdef OLD_ENETDOWN
#  undef ENETDOWN
#  define ENETDOWN OLD_ENETDOWN
#  undef OLD_ENETDOWN
#endif

#ifdef OLD_ENETUNREACH
#  undef ENETUNREACH
#  define ENETUNREACH OLD_ENETUNREACH
#  undef OLD_ENETUNREACH
#endif

#ifdef OLD_ENETRESET
#  undef ENETRESET
#  define ENETRESET OLD_ENETRESET
#  undef OLD_ENETRESET
#endif

#ifdef OLD_ECONNABORTED
#  undef ECONNABORTED
#  define ECONNABORTED OLD_ECONNABORTED
#  undef OLD_ECONNABORTED
#endif

#ifdef OLD_ECONNRESET
#  undef ECONNRESET
#  define ECONNRESET OLD_ECONNRESET
#  undef OLD_ECONNRESET
#endif

#ifdef OLD_ENOBUFS
#  undef ENOBUFS
#  define ENOBUFS OLD_ENOBUFS
#  undef OLD_ENOBUFS
#endif

#ifdef OLD_EISCONN
#  undef EISCONN
#  define EISCONN OLD_EISCONN
#  undef OLD_EISCONN
#endif

#ifdef OLD_ENOTCONN
#  undef ENOTCONN
#  define ENOTCONN OLD_ENOTCONN
#  undef OLD_ENOTCONN
#endif

#ifdef OLD_ETIMEDOUT
#  undef ETIMEDOUT
#  define ETIMEDOUT OLD_ETIMEDOUT
#  undef OLD_ETIMEDOUT
#endif

#ifdef OLD_ECONNREFUSED
#  undef ECONNREFUSED
#  define ECONNREFUSED OLD_ECONNREFUSED
#  undef OLD_ECONNREFUSED
#endif

#ifdef OLD_ELOOP
#  undef ELOOP
#  define ELOOP OLD_ELOOP
#  undef OLD_ELOOP
#endif

#ifdef OLD_EHOSTUNREACH
#  undef EHOSTUNREACH
#  define EHOSTUNREACH OLD_EHOSTUNREACH
#  undef OLD_EHOSTUNREACH
#endif

#ifdef try
#  undef try
#endif
#ifdef OLD_try
#  define try OLD_try
#  undef OLD_try
#endif

#ifdef except
#  undef except
#endif
#ifdef OLD_except
#  define except OLD_except
#  undef OLD_except
#endif

#ifdef finally
#  undef finally
#endif
#ifdef OLD_finally
#  define finally OLD_finally
#  undef OLD_finally
#endif

#ifdef leave
#  undef leave
#endif
#ifdef OLD_leave
#  define leave OLD_leave
#  undef OLD_leave
#endif

#ifdef OpenFile
#  undef OpenFile
#endif
#ifdef OLD_OpenFile
#  define OpenFile OLD_OpenFile
#  undef OLD_OpenFile
#endif

#ifdef CharNext
#  undef CharNext
#endif
#ifdef OLD_CharNext
#  define CharNext OLD_CharNext
#  undef OLD_CharNext
#endif

#ifdef rb_w32_iswinnt
#  undef rb_w32_iswinnt
#endif
#ifdef OLD_rb_w32_iswinnt
#  define rb_w32_iswinnt OLD_rb_w32_iswinnt
#  undef OLD_rb_w32_iswinnt
#endif

#ifdef rb_w32_iswin95
#  undef rb_w32_iswin95
#endif
#ifdef OLD_rb_w32_iswin95
#  define rb_w32_iswin95 OLD_rb_w32_iswin95
#  undef OLD_rb_w32_iswin95
#endif

#ifdef rb_w32_iswinnt
#  undef rb_w32_iswinnt
#endif
#ifdef OLD_rb_w32_iswinnt
#  define rb_w32_iswinnt OLD_rb_w32_iswinnt
#  undef OLD_rb_w32_iswinnt
#endif

#ifdef rb_w32_iswin95
#  undef rb_w32_iswin95
#endif
#ifdef OLD_rb_w32_iswin95
#  define rb_w32_iswin95 OLD_rb_w32_iswin95
#  undef OLD_rb_w32_iswin95
#endif

#ifdef getc
#  undef getc
#endif
#ifdef OLD_getc
#  define getc OLD_getc
#  undef OLD_getc
#endif

#ifdef putc
#  undef putc
#endif
#ifdef OLD_putc
#  define putc OLD_putc
#  undef OLD_putc
#endif

#ifdef fgetc
#  undef fgetc
#endif
#ifdef OLD_fgetc
#  define fgetc OLD_fgetc
#  undef OLD_fgetc
#endif

#ifdef fputc
#  undef fputc
#endif
#ifdef OLD_fputc
#  define fputc OLD_fputc
#  undef OLD_fputc
#endif

#ifdef getchar
#  undef getchar
#endif
#ifdef OLD_getchar
#  define getchar OLD_getchar
#  undef OLD_getchar
#endif

#ifdef putchar
#  undef putchar
#endif
#ifdef OLD_putchar
#  define putchar OLD_putchar
#  undef OLD_putchar
#endif

#ifdef fgetchar
#  undef fgetchar
#endif
#ifdef OLD_fgetchar
#  define fgetchar OLD_fgetchar
#  undef OLD_fgetchar
#endif

#ifdef fputchar
#  undef fputchar
#endif
#ifdef OLD_fputchar
#  define fputchar OLD_fputchar
#  undef OLD_fputchar
#endif

#ifdef utime
#  undef utime
#endif
#ifdef OLD_utime
#  define utime OLD_utime
#  undef OLD_utime
#endif

#ifdef strcasecmp
#  undef strcasecmp
#endif
#ifdef OLD_strcasecmp
#  define strcasecmp OLD_strcasecmp
#  undef OLD_strcasecmp
#endif

#ifdef strncasecmp
#  undef strncasecmp
#endif
#ifdef OLD_strncasecmp
#  define strncasecmp OLD_strncasecmp
#  undef OLD_strncasecmp
#endif

#ifdef close
#  undef close
#endif
#ifdef OLD_close
#  define close OLD_close
#  undef OLD_close
#endif

#ifdef fclose
#  undef fclose
#endif
#ifdef OLD_fclose
#  define fclose OLD_fclose
#  undef OLD_fclose
#endif

#ifdef read
#  undef read
#endif
#ifdef OLD_read
#  define read OLD_read
#  undef OLD_read
#endif

#ifdef write
#  undef write
#endif
#ifdef OLD_write
#  define write OLD_write
#  undef OLD_write
#endif

#ifdef getpid
#  undef getpid
#endif
#ifdef OLD_getpid
#  define getpid OLD_getpid
#  undef OLD_getpid
#endif

#ifdef sleep
#  undef sleep
#endif
#ifdef OLD_sleep
#  define sleep OLD_sleep
#  undef OLD_sleep
#endif

#ifdef creat
#  undef creat
#endif
#ifdef OLD_creat
#  define creat OLD_creat
#  undef OLD_creat
#endif

#ifdef eof
#  undef eof
#endif
#ifdef OLD_eof
#  define eof OLD_eof
#  undef OLD_eof
#endif

#ifdef filelength
#  undef filelength
#endif
#ifdef OLD_filelength
#  define filelength OLD_filelength
#  undef OLD_filelength
#endif

#ifdef mktemp
#  undef mktemp
#endif
#ifdef OLD_mktemp
#  define mktemp OLD_mktemp
#  undef OLD_mktemp
#endif

#ifdef tell
#  undef tell
#endif
#ifdef OLD_tell
#  define tell OLD_tell
#  undef OLD_tell
#endif

#ifdef _open
#  undef _open
#endif
#ifdef OLD__open
#  define _open OLD__open
#  undef OLD__open
#endif

#ifdef sopen
#  undef sopen
#endif
#ifdef OLD_sopen
#  define sopen OLD_sopen
#  undef OLD_sopen
#endif

#ifdef fopen
#  undef fopen
#endif
#ifdef OLD_fopen
#  define fopen OLD_fopen
#  undef OLD_fopen
#endif

#ifdef fdopen
#  undef fdopen
#endif
#ifdef OLD_fdopen
#  define fdopen OLD_fdopen
#  undef OLD_fdopen
#endif

#ifdef fsopen
#  undef fsopen
#endif
#ifdef OLD_fsopen
#  define fsopen OLD_fsopen
#  undef OLD_fsopen
#endif

#ifdef fsync
#  undef fsync
#endif
#ifdef OLD_fsync
#  define fsync OLD_fsync
#  undef OLD_fsync
#endif

#ifdef stat
#  undef stat
#endif
#ifdef OLD_stat
#  define stat OLD_stat
#  undef OLD_stat
#endif

#ifdef execv
#  undef execv
#endif
#ifdef OLD_execv
#  define execv OLD_execv
#  undef OLD_execv
#endif

#ifdef isatty
#  undef isatty
#endif
#ifdef OLD_isatty
#  define isatty OLD_isatty
#  undef OLD_isatty
#endif

#ifdef mkdir
#  undef mkdir
#endif
#ifdef OLD_mkdir
#  define mkdir OLD_mkdir
#  undef OLD_mkdir
#endif

#ifdef rmdir
#  undef rmdir
#endif
#ifdef OLD_rmdir
#  define rmdir OLD_rmdir
#  undef OLD_rmdir
#endif

#ifdef unlink
#  undef unlink
#endif
#ifdef OLD_unlink
#  define unlink OLD_unlink
#  undef OLD_unlink
#endif

#ifdef isascii
#  undef isascii
#endif
#ifdef OLD_isascii
#  define isascii OLD_isascii
#  undef OLD_isascii
#endif

#ifdef vsnprintf
#  undef vsnprintf
#endif
#ifdef OLD_vsnprintf
#  define vsnprintf OLD_vsnprintf
#  undef OLD_vsnprintf
#endif

#ifdef snprintf
#  undef snprintf
#endif
#ifdef OLD_snprintf
#  define snprintf OLD_snprintf
#  undef OLD_snprintf
#endif

#ifdef isnan
#  undef isnan
#endif
#ifdef OLD_isnan
#  define isnan OLD_isnan
#  undef OLD_isnan
#endif

#ifdef finite
#  undef finite
#endif
#ifdef OLD_finite
#  define finite OLD_finite
#  undef OLD_finite
#endif

#ifdef copysign
#  undef copysign
#endif
#ifdef OLD_copysign
#  define copysign OLD_copysign
#  undef OLD_copysign
#endif

#ifdef scalb
#  undef scalb
#endif
#ifdef OLD_scalb
#  define scalb OLD_scalb
#  undef OLD_scalb
#endif

#ifdef accept
#  undef accept
#endif
#ifdef OLD_accept
#  define accept OLD_accept
#  undef OLD_accept
#endif

#ifdef bind
#  undef bind
#endif
#ifdef OLD_bind
#  define bind OLD_bind
#  undef OLD_bind
#endif

#ifdef connect
#  undef connect
#endif
#ifdef OLD_connect
#  define connect OLD_connect
#  undef OLD_connect
#endif

#ifdef FD_SET
#  undef FD_SET
#endif
#ifdef OLD_FD_SET
#  define FD_SET OLD_FD_SET
#  undef OLD_FD_SET
#endif

#ifdef FD_CLR
#  undef FD_CLR
#endif
#ifdef OLD_FD_CLR
#  define FD_CLR OLD_FD_CLR
#  undef OLD_FD_CLR
#endif

#ifdef FD_ISSET
#  undef FD_ISSET
#endif
#ifdef OLD_FD_ISSET
#  define FD_ISSET OLD_FD_ISSET
#  undef OLD_FD_ISSET
#endif

#ifdef select
#  undef select
#endif
#ifdef OLD_select
#  define select OLD_select
#  undef OLD_select
#endif

#ifdef getpeername
#  undef getpeername
#endif
#ifdef OLD_getpeername
#  define getpeername OLD_getpeername
#  undef OLD_getpeername
#endif

#ifdef getsockname
#  undef getsockname
#endif
#ifdef OLD_getsockname
#  define getsockname OLD_getsockname
#  undef OLD_getsockname
#endif

#ifdef getsockopt
#  undef getsockopt
#endif
#ifdef OLD_getsockopt
#  define getsockopt OLD_getsockopt
#  undef OLD_getsockopt
#endif

#ifdef ioctlsocket
#  undef ioctlsocket
#endif
#ifdef OLD_ioctlsocket
#  define ioctlsocket OLD_ioctlsocket
#  undef OLD_ioctlsocket
#endif

#ifdef listen
#  undef listen
#endif
#ifdef OLD_listen
#  define listen OLD_listen
#  undef OLD_listen
#endif

#ifdef recv
#  undef recv
#endif
#ifdef OLD_recv
#  define recv OLD_recv
#  undef OLD_recv
#endif

#ifdef recvfrom
#  undef recvfrom
#endif
#ifdef OLD_recvfrom
#  define recvfrom OLD_recvfrom
#  undef OLD_recvfrom
#endif

#ifdef send
#  undef send
#endif
#ifdef OLD_send
#  define send OLD_send
#  undef OLD_send
#endif

#ifdef sendto
#  undef sendto
#endif
#ifdef OLD_sendto
#  define sendto OLD_sendto
#  undef OLD_sendto
#endif

#ifdef setsockopt
#  undef setsockopt
#endif
#ifdef OLD_setsockopt
#  define setsockopt OLD_setsockopt
#  undef OLD_setsockopt
#endif

#ifdef shutdown
#  undef shutdown
#endif
#ifdef OLD_shutdown
#  define shutdown OLD_shutdown
#  undef OLD_shutdown
#endif

#ifdef socket
#  undef socket
#endif
#ifdef OLD_socket
#  define socket OLD_socket
#  undef OLD_socket
#endif

#ifdef gethostbyaddr
#  undef gethostbyaddr
#endif
#ifdef OLD_gethostbyaddr
#  define gethostbyaddr OLD_gethostbyaddr
#  undef OLD_gethostbyaddr
#endif

#ifdef gethostbyname
#  undef gethostbyname
#endif
#ifdef OLD_gethostbyname
#  define gethostbyname OLD_gethostbyname
#  undef OLD_gethostbyname
#endif

#ifdef gethostname
#  undef gethostname
#endif
#ifdef OLD_gethostname
#  define gethostname OLD_gethostname
#  undef OLD_gethostname
#endif

#ifdef getprotobyname
#  undef getprotobyname
#endif
#ifdef OLD_getprotobyname
#  define getprotobyname OLD_getprotobyname
#  undef OLD_getprotobyname
#endif

#ifdef getprotobynumber
#  undef getprotobynumber
#endif
#ifdef OLD_getprotobynumber
#  define getprotobynumber OLD_getprotobynumber
#  undef OLD_getprotobynumber
#endif

#ifdef getservbyname
#  undef getservbyname
#endif
#ifdef OLD_getservbyname
#  define getservbyname OLD_getservbyname
#  undef OLD_getservbyname
#endif

#ifdef getservbyport
#  undef getservbyport
#endif
#ifdef OLD_getservbyport
#  define getservbyport OLD_getservbyport
#  undef OLD_getservbyport
#endif

#ifdef get_osfhandle
#  undef get_osfhandle
#endif
#ifdef OLD_get_osfhandle
#  define get_osfhandle OLD_get_osfhandle
#  undef OLD_get_osfhandle
#endif

#ifdef getcwd
#  undef getcwd
#endif
#ifdef OLD_getcwd
#  define getcwd OLD_getcwd
#  undef OLD_getcwd
#endif

#ifdef getenv
#  undef getenv
#endif
#ifdef OLD_getenv
#  define getenv OLD_getenv
#  undef OLD_getenv
#endif

#ifdef rename
#  undef rename
#endif
#ifdef OLD_rename
#  define rename OLD_rename
#  undef OLD_rename
#endif

#ifdef times
#  undef times
#endif
#ifdef OLD_times
#  define times OLD_times
#  undef OLD_times
#endif

#ifdef Sleep
#  undef Sleep
#endif
#ifdef OLD_Sleep
#  define Sleep OLD_Sleep
#  undef OLD_Sleep
#endif

#endif  // UTILITIES_CORE_RUBYINTERPRETER_HPP
