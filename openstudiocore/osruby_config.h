#ifndef INCLUDE_OSRUBY_H
#define INCLUDE_OSRUBY_H 1

// Force RUBY_EXTERN to be defined as "undecorated" extern for static ruby
// We want to get rid of the declspec appended by the ruby build system, because
// it causes a compiler warning that manifests as error
#ifdef RUBY_EXTERN
#undef RUBY_EXTERN
#endif

#define RUBY_EXTERN extern

// Unclear why Ruby defines inline as __inline in config.h, 
// but we want to undo that or embedded ruby and bindings wont compile
#ifdef inline
#undef inline
#endif

#endif
