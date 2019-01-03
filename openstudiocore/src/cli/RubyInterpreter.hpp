/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) 2008-2019, Alliance for Sustainable Energy, LLC, and other contributors. All rights reserved.
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

#ifndef CLI_RUBYINTERPRETER_HPP
#define CLI_RUBYINTERPRETER_HPP

#include "RubyException.hpp"

#include <vector>
#include <string>
#include <map>
#include <typeinfo>
#include <sstream>

#include <ruby.h>
#include <ruby/encoding.h>
// SWIGRubyRuntime.hxx includes ruby.h which includes ruby/win32.h, which has some brain damaged notions of
// what standard errno values should be.

#ifdef _MSC_VER
#ifdef EWOULDBLOCK
#pragma push_macro("EWOULDBLOCK")
#undef EWOULDBLOCK
#define EWOULDBLOCK_macro_found
#endif


#ifdef EINPROGRESS
#pragma push_macro("EINPROGRESS")
#undef EINPROGRESS
#define EINPROGRESS_macro_found
#endif

#ifdef EALREADY
#pragma push_macro("EALREADY")
#undef EALREADY
#define EALREADY_macro_found
#endif

#ifdef ENOTSOCK
#pragma push_macro("ENOTSOCK")
#undef ENOTSOCK
#define ENOTSOCK_macro_found
#endif

#ifdef EDESTADDRREQ
#pragma push_macro("EDESTADDRREQ")
#undef EDESTADDRREQ
#define EDESTADDRREQ_macro_found
#endif

#ifdef EMSGSIZE
#pragma push_macro("EMSGSIZE")
#undef EMSGSIZE
#define EMSGSIZE_macro_found
#endif

#ifdef EPROTOTYPE
#pragma push_macro("EPROTOTYPE")
#undef EPROTOTYPE
#define EPROTOTYPE_macro_found
#endif

#ifdef ENOPROTOOPT
#pragma push_macro("ENOPROTOOPT")
#undef ENOPROTOOPT
#define ENOPROTOOPT_macro_found
#endif

#ifdef EPROTONOSUPPORT
#pragma push_macro("EPROTONOSUPPORT")
#undef EPROTONOSUPPORT
#define EPROTONOSUPPORT_macro_found
#endif

#ifdef EOPNOTSUPP
#pragma push_macro("EOPNOTSUPP")
#undef EOPNOTSUPP
#define EOPNOTSUPP_macro_found
#endif

#ifdef EAFNOSUPPORT
#pragma push_macro("EAFNOSUPPORT")
#undef EAFNOSUPPORT
#define EAFNOSUPPORT_macro_found
#endif

#ifdef EADDRINUSE
#pragma push_macro("EADDRINUSE")
#undef EADDRINUSE
#define EADDRINUSE_macro_found
#endif

#ifdef EADDRNOTAVAIL
#pragma push_macro("EADDRNOTAVAIL")
#undef EADDRNOTAVAIL
#define EADDRNOTAVAIL_macro_found
#endif

#ifdef ENETDOWN
#pragma push_macro("ENETDOWN")
#undef ENETDOWN
#define ENETDOWN_macro_found
#endif

#ifdef ENETUNREACH
#pragma push_macro("ENETUNREACH")
#undef ENETUNREACH
#define ENETUNREACH_macro_found
#endif

#ifdef ENETRESET
#pragma push_macro("ENETRESET")
#undef ENETRESET
#define ENETRESET_macro_found
#endif

#ifdef ECONNABORTED
#pragma push_macro("ECONNABORTED")
#undef ECONNABORTED
#define ECONNABORTED_macro_found
#endif

#ifdef ECONNRESET
#pragma push_macro("ECONNRESET")
#undef ECONNRESET
#define ECONNRESET_macro_found
#endif

#ifdef ENOBUFS
#pragma push_macro("ENOBUFS")
#undef ENOBUFS
#define ENOBUFS_macro_found
#endif

#ifdef EISCONN
#pragma push_macro("EISCONN")
#undef EISCONN
#define EISCONN_macro_found
#endif

#ifdef ENOTCONN
#pragma push_macro("ENOTCONN")
#undef ENOTCONN
#define ENOTCONN_macro_found
#endif

#ifdef ETIMEDOUT
#pragma push_macro("ETIMEDOUT")
#undef ETIMEDOUT
#define ETIMEDOUT_macro_found
#endif

#ifdef ECONNREFUSED
#pragma push_macro("ECONNREFUSED")
#undef ECONNREFUSED
#define ECONNREFUSED_macro_found
#endif

#ifdef ELOOP
#pragma push_macro("ELOOP")
#undef ELOOP
#define ELOOP_macro_found
#endif

#ifdef EHOSTUNREACH
#pragma push_macro("EHOSTUNREACH")
#undef EHOSTUNREACH
#define EHOSTUNREACH_macro_found
#endif

#ifdef try
#pragma push_macro("try")
#undef try
#define try_macro_found
#endif

#ifdef except
#pragma push_macro("except")
#undef except
#define except_macro_found
#endif

#ifdef finally
#pragma push_macro("finally")
#undef finally
#define finally_macro_found
#endif

#ifdef leave
#pragma push_macro("leave")
#undef leave
#define leave_macro_found
#endif

#ifdef OpenFile
#pragma push_macro("OpenFile")
#undef OpenFile
#define OpenFile_macro_found
#endif

#ifdef CharNext
#pragma push_macro("CharNext")
#undef CharNext
#define CharNext_macro_found
#endif


#ifdef rb_w32_iswinnt
#pragma push_macro("rb_w32_iswinnt")
#undef rb_w
#define rb_w_macro_found32_iswinnt
#endif

#ifdef rb_w32_iswin95
#pragma push_macro("rb_w32_iswin95")
#undef rb_w
#define rb_w_macro_found32_iswin95
#endif

#ifdef getc
#pragma push_macro("getc")
#undef getc
#define getc_macro_found
#endif

#ifdef putc
#pragma push_macro("putc")
#undef putc
#define putc_macro_found
#endif

#ifdef fgetc
#pragma push_macro("fgetc")
#undef fgetc
#define fgetc_macro_found
#endif

#ifdef fputc
#pragma push_macro("fputc")
#undef fputc
#define fputc_macro_found
#endif

#ifdef getchar
#pragma push_macro("getchar")
#undef getchar
#define getchar_macro_found
#endif

#ifdef putchar
#pragma push_macro("putchar")
#undef putchar
#define putchar_macro_found
#endif

#ifdef fgetchar
#pragma push_macro("fgetchar")
#undef fgetchar
#define fgetchar_macro_found
#endif

#ifdef fputchar
#pragma push_macro("fputchar")
#undef fputchar
#define fputchar_macro_found
#endif

#ifdef utime
#pragma push_macro("utime")
#undef utime
#define utime_macro_found
#endif

#ifdef strcasecmp
#pragma push_macro("strcasecmp")
#undef strcasecmp
#define strcasecmp_macro_found
#endif

#ifdef strncasecmp
#pragma push_macro("strncasecmp")
#undef strncasecmp
#define strncasecmp_macro_found
#endif

#ifdef close
#pragma push_macro("close")
#undef close
#define close_macro_found
#endif

#ifdef fclose
#pragma push_macro("fclose")
#undef fclose
#define fclose_macro_found
#endif

#ifdef read
#pragma push_macro("read")
#undef read
#define read_macro_found
#endif

#ifdef write
#pragma push_macro("write")
#undef write
#define write_macro_found
#endif

#ifdef getpid
#pragma push_macro("getpid")
#undef getpid
#define getpid_macro_found
#endif

#ifdef sleep
#pragma push_macro("sleep")
#undef sleep
#define sleep_macro_found
#endif

#ifdef creat
#pragma push_macro("creat")
#undef creat
#define creat_macro_found
#endif

#ifdef eof
#pragma push_macro("eof")
#undef eof
#define eof_macro_found
#endif

#ifdef filelength
#pragma push_macro("filelength")
#undef filelength
#define filelength_macro_found
#endif

#ifdef mktemp
#pragma push_macro("mktemp")
#undef mktemp
#define mktemp_macro_found
#endif

#ifdef tell
#pragma push_macro("tell")
#undef tell
#define tell_macro_found
#endif

#ifdef unlink
#pragma push_macro("unlink")
#undef unlink
#define unlink_macro_found
#endif

#ifdef _open
#pragma push_macro("_open")
#undef _open
#define _open_macro_found
#endif

#ifdef sopen
#pragma push_macro("sopen")
#undef sopen
#define sopen_macro_found
#endif

#ifdef fopen
#pragma push_macro("fopen")
#undef fopen
#define fopen_macro_found
#endif

#ifdef fdopen
#pragma push_macro("fdopen")
#undef fdopen
#define fdopen_macro_found
#endif

#ifdef fsopen
#pragma push_macro("fsopen")
#undef fsopen
#define fsopen_macro_found
#endif

#ifdef fsync
#pragma push_macro("fsync")
#undef fsync
#define fsync_macro_found
#endif

#ifdef stat
#pragma push_macro("stat")
#undef stat
#define stat_macro_found
#endif

#ifdef execv
#pragma push_macro("execv")
#undef execv
#define execv_macro_found
#endif

#ifdef isatty
#pragma push_macro("isatty")
#undef isatty
#define isatty_macro_found
#endif

#ifdef mkdir
#pragma push_macro("mkdir")
#undef mkdir
#define mkdir_macro_found
#endif

#ifdef rmdir
#pragma push_macro("rmdir")
#undef rmdir
#define rmdir_macro_found
#endif

#ifdef isascii
#pragma push_macro("isascii")
#undef isascii
#define isascii_macro_found
#endif

#ifdef vsnprintf
#pragma push_macro("vsnprintf")
#undef vsnprintf
#define vsnprintf_macro_found
#endif

#ifdef snprintf
#pragma push_macro("snprintf")
#undef snprintf
#define snprintf_macro_found
#endif

#ifdef isnan
#pragma push_macro("isnan")
#undef isnan
#define isnan_macro_found
#endif

#ifdef finite
#pragma push_macro("finite")
#undef finite
#define finite_macro_found
#endif

#ifdef copysign
#pragma push_macro("copysign")
#undef copysign
#define copysign_macro_found
#endif

#ifdef scalb
#pragma push_macro("scalb")
#undef scalb
#define scalb_macro_found
#endif

#ifdef accept
#pragma push_macro("accept")
#undef accept
#define accept_macro_found
#endif

#ifdef bind
#pragma push_macro("bind")
#undef bind
#define bind_macro_found
#endif

#ifdef connect
#pragma push_macro("connect")
#undef connect
#define connect_macro_found
#endif

#ifdef FD_SET
#pragma push_macro("FD_SET")
#undef FD_SET
#define FD_SET_macro_found
#endif

#ifdef FD_CLR
#pragma push_macro("FD_CLR")
#undef FD_CLR
#define FD_CLR_macro_found
#endif

#ifdef FD_ISSET
#pragma push_macro("FD_ISSET")
#undef FD_ISSET
#define FD_ISSET_macro_found
#endif

#ifdef select
#pragma push_macro("select")
#undef select
#define select_macro_found
#endif

#ifdef getpeername
#pragma push_macro("getpeername")
#undef getpeername
#define getpeername_macro_found
#endif

#ifdef getsockname
#pragma push_macro("getsockname")
#undef getsockname
#define getsockname_macro_found
#endif

#ifdef getsockopt
#pragma push_macro("getsockopt")
#undef getsockopt
#define getsockopt_macro_found
#endif

#ifdef ioctlsocket
#pragma push_macro("ioctlsocket")
#undef ioctlsocket
#define ioctlsocket_macro_found
#endif

#ifdef listen
#pragma push_macro("listen")
#undef listen
#define listen_macro_found
#endif

#ifdef recv
#pragma push_macro("recv")
#undef recv
#define recv_macro_found
#endif

#ifdef recvfrom
#pragma push_macro("recvfrom")
#undef recvfrom
#define recvfrom_macro_found
#endif

#ifdef send
#pragma push_macro("send")
#undef send
#define send_macro_found
#endif

#ifdef sendto
#pragma push_macro("sendto")
#undef sendto
#define sendto_macro_found
#endif

#ifdef setsockopt
#pragma push_macro("setsockopt")
#undef setsockopt
#define setsockopt_macro_found
#endif

#ifdef shutdown
#pragma push_macro("shutdown")
#undef shutdown
#define shutdown_macro_found
#endif

#ifdef socket
#pragma push_macro("socket")
#undef socket
#define socket_macro_found
#endif

#ifdef gethostbyaddr
#pragma push_macro("gethostbyaddr")
#undef gethostbyaddr
#define gethostbyaddr_macro_found
#endif

#ifdef gethostbyname
#pragma push_macro("gethostbyname")
#undef gethostbyname
#define gethostbyname_macro_found
#endif

#ifdef gethostname
#pragma push_macro("gethostname")
#undef gethostname
#define gethostname_macro_found
#endif

#ifdef getprotobyname
#pragma push_macro("getprotobyname")
#undef getprotobyname
#define getprotobyname_macro_found
#endif

#ifdef getprotobynumber
#pragma push_macro("getprotobynumber")
#undef getprotobynumber
#define getprotobynumber_macro_found
#endif

#ifdef getservbyname
#pragma push_macro("getservbyname")
#undef getservbyname
#define getservbyname_macro_found
#endif

#ifdef getservbyport
#pragma push_macro("getservbyport")
#undef getservbyport
#define getservbyport_macro_found
#endif

#ifdef get_osfhandle
#define OLD_get_osfhandle get_osfhandle
#undef get_osfhandle
#define get_osfhandle_macro_found
#endif

#ifdef getcwd
#pragma push_macro("getcwd")
#undef getcwd
#define getcwd_macro_found
#endif

#ifdef getenv
#pragma push_macro("getenv")
#undef getenv
#define getenv_macro_found
#endif

#ifdef rename
#pragma push_macro("rename")
#undef rename
#define rename_macro_found
#endif

#ifdef times
#pragma push_macro("times")
#undef times
#define times_macro_found
#endif

#ifdef Sleep
#pragma push_macro("Sleep")
#undef Sleep
#define Sleep_macro_found
#endif
#endif



#include <cli/SWIGRubyRuntime.hxx>



class RubyInterpreter
{
  private:
    // dummy arguments passed to ruby_options
    int m_argc;
    char** m_argv;

    static void addIncludePath(std::vector<std::string>& includePaths, const std::string& includePath)
    {
      includePaths.push_back("-I");
      includePaths.push_back(includePath);
    }

  public:
    explicit RubyInterpreter(const std::vector<std::string> &t_includePaths)
    {


      //// set load paths
      //std::vector<std::string> rubyArgs;


      //rubyArgs.emplace_back("-EUTF-8");

      //for (const auto &p : t_includePaths) {
      //  addIncludePath(rubyArgs, p);
      //}

      //// and now give the interpreter something to parse, so that it doesn't sit
      //// waiting on stdin from us
      //rubyArgs.emplace_back("-e");
      //rubyArgs.emplace_back("");


      //m_argc = static_cast<int>(rubyArgs.size());
      //m_argv = new char*[m_argc];

      //for (int i = 0; i < m_argc; ++i){
      //  m_argv[i] = new char[rubyArgs[i].size() + 1];
      //  strcpy(m_argv[i], rubyArgs[i].c_str());
      //}

      //// the return value of ruby_options is the parsed node of our "script"
      //// from the -e "" we passed in. This could be used to actually parse / eval something if we wanted
      //ruby_options(m_argc, m_argv);


      // register some default types that we want to pass in / out of the ruby system
      registerType<int>("int");
      registerType<long>("long");
      registerType<size_t>("size_t");
      registerType<double>("double");
      registerType<std::string>("std::string");
      registerType<float>("float");

      // set the script name if desired
      //ruby_script("script_name");
    }

    ~RubyInterpreter()
    {
      //for (int i = 0; i < m_argc; i++){
      //  delete[] m_argv[i];
      //}
      //delete[] m_argv;
    }

    /// Register a type along with its vector versions so that it can be used
    /// from within the Ruby wrapper. These are necessary because there's no way to divine
    /// the unmangled type name.
    template<typename Type>
      void registerType(const std::string &t_name)
      {
        m_types[typeid(Type).name()] = t_name;
        m_types[typeid(Type *).name()] = t_name + " *";
        m_types[typeid(std::vector<Type>).name()] = "std::vector<" + t_name + ">";
        m_types[typeid(std::vector<Type> *).name()] = "std::vector<" + t_name + "> *";
      }

    // evaluate a ruby statement with no return value. If a ruby exception is raised
    // the description is translated into a C++ exception, which is thrown as an openstudio::RubyException.
    void evalString(const std::string &t_str)
    {

      VALUE val = rb_str_new2(t_str.c_str());
      int error;

      // save and restore the current working directory in case the call to ruby upsets it
//      const auto cwd = openstudio::filesystem::current_dir();
      rb_protect(evaluateSimpleImpl,val,&error);
//      openstudio::filesystem::current_dir(cwd);


      if (error != 0)
      {
        VALUE errval = rb_eval_string("$!.to_s");
        char *str = StringValuePtr(errval);
        std::string err(str);

        VALUE locval = rb_eval_string("$@.to_s");
        str = StringValuePtr(locval);
        std::string loc(str);

        throw RubyException(err, loc);
      }
    }

    /// Execute a function by name with 0 parameters and no return value
    template<typename ReturnType>
      void exec(const std::string &t_functionName)
      {
        std::vector<VALUE> params;

        makeProtectedCall(t_functionName, params);
      }

    /// Execute a function by name with 1 parameter and no return value
    template<typename ReturnType, typename Param1>
      void exec(
          const std::string &t_functionName,
          Param1 t_param1)
      {
        std::vector<VALUE> params;
        params.push_back(newPointerObj(&t_param1));

        makeProtectedCall(t_functionName, params);
      }

    /// Execute a function by name with 2 parameters and no return value
    template<typename ReturnType, typename Param1, typename Param2>
      ReturnType exec(
          const std::string &t_functionName,
          Param1 t_param1,
          Param2 t_param2)
      {
        std::vector<VALUE> params;
        params.push_back(newPointerObj(&t_param1));
        params.push_back(newPointerObj(&t_param2));

        makeProtectedCall(t_functionName, params);
      }

    /// Execute a function by name with 3 parameters and no return value
    template<typename ReturnType, typename Param1, typename Param2, typename Param3>
      ReturnType exec(
          const std::string &t_functionName,
          Param1 t_param1,
          Param2 t_param2,
          Param3 t_param3)
      {
        std::vector<VALUE> params;
        params.push_back(newPointerObj(&t_param1));
        params.push_back(newPointerObj(&t_param2));
        params.push_back(newPointerObj(&t_param3));

        makeProtectedCall(t_functionName, params);
      }

    /// Execute a function by name with 0 parameters and a return value
    template<typename ReturnType>
      ReturnType execWithReturn(const std::string &t_functionName)
      {
        std::vector<VALUE> params;

        VALUE retval = makeProtectedCall(t_functionName, params);
        return asType<ReturnType>(retval);
      }

    /// Execute a function by name with 1 parameter and a return value
    template<typename ReturnType, typename Param1>
      ReturnType execWithReturn(
          const std::string &t_functionName,
          Param1 t_param1)
      {
        std::vector<VALUE> params;
        params.push_back(newPointerObj(&t_param1));

        VALUE retval = makeProtectedCall(t_functionName, params);
        return asType<ReturnType>(retval);
      }

    /// Execute a function by name with 2 parameters and a return value
    template<typename ReturnType, typename Param1, typename Param2>
      ReturnType execWithReturn(
          const std::string &t_functionName,
          Param1 t_param1,
          Param2 t_param2)
      {
        std::vector<VALUE> params;
        params.push_back(newPointerObj(&t_param1));
        params.push_back(newPointerObj(&t_param2));

        VALUE retval = makeProtectedCall(t_functionName, params);
        return asType<ReturnType>(retval);
      }

    /// Execute a function by name with 3 parameters and a return value
    template<typename ReturnType, typename Param1, typename Param2, typename Param3>
      ReturnType execWithReturn(
          const std::string &t_functionName,
          Param1 t_param1,
          Param2 t_param2,
          Param3 t_param3)
      {
        std::vector<VALUE> params;
        params.push_back(newPointerObj(&t_param1));
        params.push_back(newPointerObj(&t_param2));
        params.push_back(newPointerObj(&t_param3));

        VALUE retval = makeProtectedCall(t_functionName, params);
        return asType<ReturnType>(retval);
      }


    /// Add a named global object to the ruby runtime
    template<typename ObjectType>
      void addObject(const std::string &t_objectName, ObjectType *t_obj)
      {
        VALUE r_obj = SWIG_NewPointerObj(t_obj, getTypeInfo<ObjectType *>(), 0);
        rb_define_variable(t_objectName.c_str(), &r_obj);
      }


  private:
    RubyInterpreter &operator=(const RubyInterpreter &) = delete;
    RubyInterpreter(const RubyInterpreter &) = delete;

    std::map<std::string, std::string> m_types;

    // Used for our rb_protect calls.
    static VALUE evaluateSimpleImpl(VALUE arg)
    {
      return rb_eval_string(StringValuePtr(arg));
    }

    // Returns the swig_type_info for the templated type if it's been registered previously
    // with registerType
    template<typename Type>
      swig_type_info *getTypeInfo() const
      {
        auto itr = m_types.find(typeid(Type).name());

        if (itr == m_types.end())
        {
          throw std::runtime_error(std::string("Type has not been registered: ") + typeid(Type).name());
        }

        std::string t_typeName = itr->second;

        swig_type_info *sti = SWIG_TypeQuery(t_typeName.c_str());

        if (sti == nullptr) {
          throw std::runtime_error("Unable to lookup type info for type: " + t_typeName);
        }

        return sti;
      }

    VALUE makeProtectedCall(const std::string &t_functionName, std::vector<VALUE> &t_params)
    {
      // we go through all of this because we cannot rb_protect a call to
      // rb_funcall. At least not in any way I can find.
      std::stringstream params;

      for (size_t i = 0; i < t_params.size(); ++i)
      {
        std::stringstream ss;
        ss << "$embedded_ruby_param_" << i;

        rb_define_variable(ss.str().c_str(), &t_params[i]);

        params << ss.str();
        if (i < t_params.size() - 1)
        {
          params << ", ";
        }
      }

      std::string funcall = "$embedded_ruby_return = " + t_functionName + "(" + params.str() + ")";
      evalString(funcall);

      VALUE retval = rb_gv_get("$embedded_ruby_return");
      return retval;
    }

    template<typename Param>
      VALUE newPointerObj(Param t_param)
      {
        swig_type_info *ti = getTypeInfo<Param>();
        return SWIG_NewPointerObj(t_param, ti, 0);
      }

    VALUE newPointerObj(double *d)
    {
      return rb_float_new(*d);
    }

    VALUE newPointerObj(float *d)
    {
      return rb_float_new(*d);
    }

    VALUE newPointerObj(int *d)
    {
      return rb_to_int(rb_float_new(*d));
    }

    VALUE newPointerObj(long *d)
    {
      return rb_to_int(rb_float_new(*d));
    }

    VALUE newPointerObj(std::string *s)
    {
      return rb_str_new2(s->c_str());
    }


    template<typename Type>
      Type asType(VALUE v)
      {
        Type *ptr = nullptr;

        int res = SWIG_ConvertPtr(v, reinterpret_cast<void **>(&ptr), getTypeInfo<Type *>(), 0);
        if (SWIG_IsOK(res))
        {
          if (!ptr)
          {
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


#ifdef _MSC_VER


#ifdef EWOULDBLOCK_macro_found
#pragma pop_macro("EWOULDBLOCK")
#undef EWOULDBLOCK_macro_found
#endif

#ifdef EINPROGRESS_macro_found
#pragma pop_macro("EINPROGRESS")
#undef EINPROGRESS_macro_found
#endif

#ifdef EALREADY_macro_found
#pragma pop_macro("EALREADY")
#undef EALREADY_macro_found
#endif

#ifdef ENOTSOCK_macro_found
#pragma pop_macro("ENOTSOCK")
#undef ENOTSOCK_macro_found
#endif

#ifdef EDESTADDRREQ_macro_found
#pragma pop_macro("EDESTADDRREQ")
#undef EDESTADDRREQ_macro_found
#endif

#ifdef EMSGSIZE_macro_found
#pragma pop_macro("EMSGSIZE")
#undef EMSGSIZE_macro_found
#endif

#ifdef EPROTOTYPE_macro_found
#pragma pop_macro("EPROTOTYPE")
#undef EPROTOTYPE_macro_found
#endif

#ifdef ENOPROTOOPT_macro_found
#pragma pop_macro("ENOPROTOOPT")
#undef ENOPROTOOPT_macro_found
#endif

#ifdef EPROTONOSUPPORT_macro_found
#pragma pop_macro("EPROTONOSUPPORT")
#undef EPROTONOSUPPORT_macro_found
#endif

#ifdef EOPNOTSUPP_macro_found
#pragma pop_macro("EOPNOTSUPP")
#undef EOPNOTSUPP_macro_found
#endif

#ifdef EAFNOSUPPORT_macro_found
#pragma pop_macro("EAFNOSUPPORT")
#undef EAFNOSUPPORT_macro_found
#endif

#ifdef EADDRINUSE_macro_found
#pragma pop_macro("EADDRINUSE")
#undef EADDRINUSE_macro_found
#endif

#ifdef EADDRNOTAVAIL_macro_found
#pragma pop_macro("EADDRNOTAVAIL")
#undef EADDRNOTAVAIL_macro_found
#endif

#ifdef ENETDOWN_macro_found
#pragma pop_macro("ENETDOWN")
#undef ENETDOWN_macro_found
#endif

#ifdef ENETUNREACH_macro_found
#pragma pop_macro("ENETUNREACH")
#undef ENETUNREACH_macro_found
#endif

#ifdef ENETRESET_macro_found
#pragma pop_macro("ENETRESET")
#undef ENETRESET_macro_found
#endif

#ifdef ECONNABORTED_macro_found
#pragma pop_macro("ECONNABORTED")
#undef ECONNABORTED_macro_found
#endif

#ifdef ECONNRESET_macro_found
#pragma pop_macro("ECONNRESET")
#undef ECONNRESET_macro_found
#endif

#ifdef ENOBUFS_macro_found
#pragma pop_macro("ENOBUFS")
#undef ENOBUFS_macro_found
#endif

#ifdef EISCONN_macro_found
#pragma pop_macro("EISCONN")
#undef EISCONN_macro_found
#endif

#ifdef ENOTCONN_macro_found
#pragma pop_macro("ENOTCONN")
#undef ENOTCONN_macro_found
#endif

#ifdef ETIMEDOUT_macro_found
#pragma pop_macro("ETIMEDOUT")
#undef ETIMEDOUT_macro_found
#endif

#ifdef ECONNREFUSED_macro_found
#pragma pop_macro("ECONNREFUSED")
#undef ECONNREFUSED_macro_found
#endif

#ifdef ELOOP_macro_found
#pragma pop_macro("ELOOP")
#undef ELOOP_macro_found
#endif

#ifdef EHOSTUNREACH_macro_found
#pragma pop_macro("EHOSTUNREACH")
#undef EHOSTUNREACH_macro_found
#endif

#ifdef try_macro_found
#pragma pop_macro("try")
#undef try_macro_found
#endif

#ifdef except_macro_found
#pragma pop_macro("except")
#undef except_macro_found
#endif

#ifdef finally_macro_found
#pragma pop_macro("finally")
#undef finally_macro_found
#endif

#ifdef leave_macro_found
#pragma pop_macro("leave")
#undef leave_macro_found
#endif

#ifdef OpenFile_macro_found
#pragma pop_macro("OpenFile")
#undef OpenFile_macro_found
#endif

#ifdef CharNext_macro_found
#pragma pop_macro("CharNext")
#undef CharNext_macro_found
#endif


#ifdef rb_w_macro_found32_iswinnt
#pragma pop_macro("rb_w32_iswinnt")
#undef rb_w_macro_found32_iswinnt
#endif

#ifdef rb_w_macro_found32_iswin95
#pragma pop_macro("rb_w32_iswin95")
#undef rb_w_macro_found32_iswin95
#endif

#ifdef getc_macro_found
#pragma pop_macro("getc")
#undef getc_macro_found
#endif

#ifdef putc_macro_found
#pragma pop_macro("putc")
#undef putc_macro_found
#endif

#ifdef fgetc_macro_found
#pragma pop_macro("fgetc")
#undef fgetc_macro_found
#endif

#ifdef fputc_macro_found
#pragma pop_macro("fputc")
#undef fputc_macro_found
#endif

#ifdef getchar_macro_found
#pragma pop_macro("getchar")
#undef getchar_macro_found
#endif

#ifdef putchar_macro_found
#pragma pop_macro("putchar")
#undef putchar_macro_found
#endif

#ifdef fgetchar_macro_found
#pragma pop_macro("fgetchar")
#undef fgetchar_macro_found
#endif

#ifdef fputchar_macro_found
#pragma pop_macro("fputchar")
#undef fputchar_macro_found
#endif

#ifdef utime_macro_found
#pragma pop_macro("utime")
#undef utime_macro_found
#endif

#ifdef strcasecmp_macro_found
#pragma pop_macro("strcasecmp")
#undef strcasecmp_macro_found
#endif

#ifdef strncasecmp_macro_found
#pragma pop_macro("strncasecmp")
#undef strncasecmp_macro_found
#endif

#ifdef close_macro_found
#pragma pop_macro("close")
#undef close_macro_found
#endif

#ifdef fclose_macro_found
#pragma pop_macro("fclose")
#undef fclose_macro_found
#endif

#ifdef read_macro_found
#pragma pop_macro("read")
#undef read_macro_found
#endif

#ifdef write_macro_found
#pragma pop_macro("write")
#undef write_macro_found
#endif

#ifdef getpid_macro_found
#pragma pop_macro("getpid")
#undef getpid_macro_found
#endif

#ifdef sleep_macro_found
#pragma pop_macro("sleep")
#undef sleep
#undef sleep_macro_found
#endif

#ifdef creat_macro_found
#pragma pop_macro("creat")
#undef creat_macro_found
#endif

#ifdef eof_macro_found
#pragma pop_macro("eof")
#undef eof_macro_found
#endif

#ifdef filelength_macro_found
#pragma pop_macro("filelength")
#undef filelength_macro_found
#endif

#ifdef mktemp_macro_found
#pragma pop_macro("mktemp")
#undef mktemp_macro_found
#endif

#ifdef tell_macro_found
#pragma pop_macro("tell")
#undef tell_macro_found
#endif

#ifdef unlink_macro_found
#pragma pop_macro("unlink")
#undef unlink_macro_found
#endif

#ifdef _open_macro_found
#pragma pop_macro("_open")
#undef _open_macro_found
#endif

#ifdef sopen_macro_found
#pragma pop_macro("sopen")
#undef sopen_macro_found
#endif

#ifdef fopen_macro_found
#pragma pop_macro("fopen")
#undef fopen_macro_found
#endif

#ifdef fdopen_macro_found
#pragma pop_macro("fdopen")
#undef fdopen_macro_found
#endif

#ifdef fsopen_macro_found
#pragma pop_macro("fsopen")
#undef fsopen_macro_found
#endif

#ifdef fsync_macro_found
#pragma pop_macro("fsync")
#undef fsync_macro_found
#endif

#ifdef stat_macro_found
#pragma pop_macro("stat")
#undef stat_macro_found
#endif

#ifdef execv_macro_found
#pragma pop_macro("execv")
#undef execv_macro_found
#endif

#ifdef isatty_macro_found
#pragma pop_macro("isatty")
#undef isatty_macro_found
#endif

#ifdef mkdir_macro_found
#pragma pop_macro("mkdir")
#undef mkdir_macro_found
#endif

#ifdef rmdir_macro_found
#pragma pop_macro("rmdir")
#undef rmdir_macro_found
#endif

#ifdef isascii_macro_found
#pragma pop_macro("isascii")
#undef isascii_macro_found
#endif

#ifdef vsnprintf_macro_found
#pragma pop_macro("vsnprintf")
#undef vsnprintf_macro_found
#endif

#ifdef snprintf_macro_found
#pragma pop_macro("snprintf")
#undef snprintf_macro_found
#endif

#ifdef isnan_macro_found
#pragma pop_macro("isnan")
#undef isnan_macro_found
#endif

#ifdef finite_macro_found
#pragma pop_macro("finite")
#undef finite_macro_found
#endif

#ifdef copysign_macro_found
#pragma pop_macro("copysign")
#undef copysign_macro_found
#endif

#ifdef scalb_macro_found
#pragma pop_macro("scalb")
#undef scalb_macro_found
#endif

#ifdef accept_macro_found
#pragma pop_macro("accept")
#undef accept_macro_found
#endif

#ifdef bind_macro_found
#pragma pop_macro("bind")
#undef bind_macro_found
#endif

#ifdef connect_macro_found
#pragma pop_macro("connect")
#undef connect_macro_found
#endif

#ifdef FD_SET_macro_found
#pragma pop_macro("FD_SET")
#undef FD_SET_macro_found
#endif

#ifdef FD_CLR_macro_found
#pragma pop_macro("FD_CLR")
#undef FD_CLR_macro_found
#endif

#ifdef FD_ISSET_macro_found
#pragma pop_macro("FD_ISSET")
#undef FD_ISSET_macro_found
#endif

#ifdef select_macro_found
#pragma pop_macro("select")
#undef select_macro_found
#endif

#ifdef getpeername_macro_found
#pragma pop_macro("getpeername")
#undef getpeername_macro_found
#endif

#ifdef getsockname_macro_found
#pragma pop_macro("getsockname")
#undef getsockname_macro_found
#endif

#ifdef getsockopt_macro_found
#pragma pop_macro("getsockopt")
#undef getsockopt_macro_found
#endif

#ifdef ioctlsocket_macro_found
#pragma pop_macro("ioctlsocket")
#undef ioctlsocket_macro_found
#endif

#ifdef listen_macro_found
#pragma pop_macro("listen")
#undef listen_macro_found
#endif

#ifdef recv_macro_found
#pragma pop_macro("recv")
#undef recv_macro_found
#endif

#ifdef recvfrom_macro_found
#pragma pop_macro("recvfrom")
#undef recvfrom_macro_found
#endif

#ifdef send_macro_found
#pragma pop_macro("send")
#undef send_macro_found
#endif

#ifdef sendto_macro_found
#pragma pop_macro("sendto")
#undef sendto_macro_found
#endif

#ifdef setsockopt_macro_found
#pragma pop_macro("setsockopt")
#undef setsockopt_macro_found
#endif

#ifdef shutdown_macro_found
#pragma pop_macro("shutdown")
#undef shutdown_macro_found
#endif

#ifdef socket_macro_found
#pragma pop_macro("socket")
#undef socket_macro_found
#endif

#ifdef gethostbyaddr_macro_found
#pragma pop_macro("gethostbyaddr")
#undef gethostbyaddr_macro_found
#endif

#ifdef gethostbyname_macro_found
#pragma pop_macro("gethostbyname")
#undef gethostbyname_macro_found
#endif

#ifdef gethostname_macro_found
#pragma pop_macro("gethostname")
#undef gethostname_macro_found
#endif

#ifdef getprotobyname_macro_found
#pragma pop_macro("getprotobyname")
#undef getprotobyname_macro_found
#endif

#ifdef getprotobynumber_macro_found
#pragma pop_macro("getprotobynumber")
#undef getprotobynumber_macro_found
#endif

#ifdef getservbyname_macro_found
#pragma pop_macro("getservbyname")
#undef getservbyname_macro_found
#endif

#ifdef getservbyport_macro_found
#pragma pop_macro("getservbyport")
#undef getservbyport_macro_found
#endif

#ifdef get_osfhandle_macro_found
#pragma pop_macro("get_osfhandle")
#undef get_osfhandle_macro_found
#endif

#ifdef getcwd_macro_found
#pragma pop_macro("getcwd")
#undef getcwd_macro_found
#endif

#ifdef getenv_macro_found
#pragma pop_macro("getenv")
#undef getenv_macro_found
#endif

#ifdef rename_macro_found
#pragma pop_macro("rename")
#undef rename_macro_found
#endif

#ifdef times_macro_found
#pragma pop_macro("times")
#undef times_macro_found
#endif

#ifdef Sleep_macro_found
#pragma pop_macro("Sleep")
#undef Sleep_macro_found
#endif
#endif


#endif // CLI_RUBYINTERPRETER_HPP
