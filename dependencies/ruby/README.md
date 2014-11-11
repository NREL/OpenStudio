# Create Ruby lib from dll

1. Open the Visual Studio Command Prompt
       dumpbin /exports libmsvcrt-ruby200.dll.a > output.txt
2. Copy only the function names and paste them into a new text file `libmsvcrt-ruby200.def` and put the line `EXPORTS` at the top:
       EXPORTS
       _write
       _waitpid
       _wait
       _vm_trace_mark_event_hooks
       _vm_thread_backtrace_locations
       _vm_thread_backtrace
       ...

3. Create the .lib file
       lib /def:libmsvcrt-ruby200.def /OUT:libmsvcrt-ruby200.lib

*Source: http://adrianhenke.wordpress.com/2008/12/05/create-lib-file-from-dll/*
