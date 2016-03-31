
message(STATUS "Embedding '${INFILE}' as '${OUTNAME}'")

file(READ ${INFILE} FILEDATA HEX)
string(LENGTH ${FILEDATA} STRLEN)
math(EXPR FILELEN "${STRLEN} / 2")

math(EXPR STRLENEND "${STRLEN} - 1")
set(FILEDATASTR "")
foreach (byte RANGE 0 ${STRLENEND} 2)
  string(SUBSTRING ${FILEDATA} ${byte} 2 bytedata)
  set(FILEDATASTR "${FILEDATASTR} 0x${bytedata},")
endforeach()
math(EXPR EXPECTEDLENGTH "(${FILELEN} * 6) - 1")
string(SUBSTRING ${FILEDATASTR} 0 ${EXPECTEDLENGTH} FILEDATASTR)

file(WRITE ${EMBED_SOURCE_FILE} "  static const uint8_t embedded_file_${FILENUM}[] = {\n    ${FILEDATASTR}\n  };\n" )
file(APPEND ${EMBED_SOURCE_FILE} "  static const char *embedded_file_name_${FILENUM} = \"${OUTNAME}\";\n" )
file(APPEND ${EMBED_SOURCE_FILE} "  static const size_t embedded_file_len_${FILENUM} = ${FILELEN};\n" )
