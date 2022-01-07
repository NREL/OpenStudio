# Use ffi to call into libc
require 'ffi'

module LibC
  extend FFI::Library
  ffi_lib FFI::Platform.windows? ? FFI::Library::LIBC : 'm'
  # double sin (double x)
  attach_function("sin", [ :double ], :double)
end
