#######################################################################
#             G E N E R A T O R    E X P R E S S I O N S              #
#######################################################################

set(is_msvc_genex "$<CXX_COMPILER_ID:MSVC>")
set(is_gnu_or_clang_genex "$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>,$<CXX_COMPILER_ID:AppleClang>>")
