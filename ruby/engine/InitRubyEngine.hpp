/***********************************************************************************************************************
*  OpenStudio(R), Copyright (c) Alliance for Sustainable Energy, LLC.
*  See also https://openstudio.net/license
***********************************************************************************************************************/

#ifndef INITRUBYENGINE_included
#define INITRUBYENGINE_included

extern "C"
{
  void ruby_gc_set_params(void);
  // cf https://bugs.ruby-lang.org/issues/19758
  // I've included the ext/extinit.o and enc/encinit.o in the object files to that should work
  void Init_enc(void);
  void Init_ext(void);
  void rb_call_builtin_inits(void);
}  // extern C

#endif  // INITRUBYENGINE_included
