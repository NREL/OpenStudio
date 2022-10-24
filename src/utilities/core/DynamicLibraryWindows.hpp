#ifndef dynamiclibrary_windows_hpp_INCLUDED
#define dynamiclibrary_windows_hpp_INCLUDED

#include "Path.hpp"

#include <stdexcept>
#include <filesystem>
#include <string>

#if !defined(WIN32_LEAN_AND_MEAN)
#  define WIN32_LEAN_AND_MEAN
#  include <windows.h>
#  undef WIN32_LEAN_AND_MEAN
#else
#  include <windows.h>
#endif

#include <fmt/format.h>

namespace openstudio {

struct DynamicLibrary
{

  template <typename T>
  static [[nodiscard]] std::wstring to_wstring(const T& t_str) {
    return std::wstring(t_str.begin(), t_str.end());
  }

  template <typename T>
  static [[nodiscard]] std::string to_string(const T& t_str) {
    return std::string(t_str.begin(), t_str.end());
  }

#if defined(_UNICODE) || defined(UNICODE)
  template <typename T>
  static std::wstring to_proper_string(const T& t_str) {
    return to_wstring(t_str);
  }
#else
  template <typename T>
  static std::string [[nodiscard]] to_proper_string(const T& t_str) {
    return to_string(t_str);
  }
#endif

  static std::string [[nodiscard]] get_error_message(DWORD t_err) {
    typedef LPTSTR StringType;

#if defined(_UNICODE) || defined(UNICODE)
    std::wstring retval = L"Unknown Error";
#else
    std::string retval = "Unknown Error";
#endif
    StringType lpMsgBuf = nullptr;

    // TODO this reinterpret_cast is suspicious (implementation borrowed from ChaiScript)
    if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS, nullptr, t_err,
                      MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<StringType>(&lpMsgBuf), 0, nullptr)
          != 0
        && lpMsgBuf) {
      retval = lpMsgBuf;
      LocalFree(lpMsgBuf);
    }

    return to_string(retval);
  }

  template <typename Signature>
  [[nodiscard]] Signature* load_symbol(const std::string& name) {
    // reinterpret_cast is necessary here
    const auto symbol = reinterpret_cast<Signature*>(GetProcAddress(m_handle, name.c_str()));

    if (symbol == nullptr) {
      throw std::runtime_error(fmt::format("Unable to load symbol: '{}', reason: '{}'", name, get_error_message(GetLastError())));
    }

    return symbol;
  }

  explicit DynamicLibrary(openstudio::path location)
    : m_location{std::move(location)}, m_handle{LoadLibrary(to_proper_string(m_location.string()).c_str())} {
    if (!m_handle) {
      throw std::runtime_error(fmt::format("Unable to load library '{}', reason: '{}'", m_location.string(), get_error_message(GetLastError())));
    }
  }

  DynamicLibrary() = delete;
  DynamicLibrary(DynamicLibrary&& other) noexcept : m_handle{other.m_handle} {
    other.m_handle = HMODULE{};
  }
  DynamicLibrary(const DynamicLibrary&) = delete;
  DynamicLibrary& operator=(DynamicLibrary&& other) noexcept {
    if (m_handle) {
      FreeLibrary(m_handle);
    }
    m_handle = std::exchange(other.m_handle, HMODULE{});
    return *this;
  }
  DynamicLibrary& operator=(const DynamicLibrary&) = delete;

  ~DynamicLibrary() noexcept {
    if (m_handle) {
      FreeLibrary(m_handle);
    }
  }

  openstudio::path m_location{};
  HMODULE m_handle{};
};

}  // namespace openstudio
#endif  // dynamiclibrary_windows_hpp_INCLUDED
