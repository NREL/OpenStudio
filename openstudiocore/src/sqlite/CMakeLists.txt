set(src
  sqlite3.c
  #sqlite3.def
  #shell.c
  sqlite3.h
  #sqlite3ext.h
)

add_library(sqlite ${src})
