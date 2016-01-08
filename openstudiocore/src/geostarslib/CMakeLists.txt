set(target_name geostarslib)

remove_definitions(/WX)

# source files
set(${target_name}_SRC
  #geoAstro.c
  geoEfg2Llh.c
  geoEfg2Llh_fast.c
  geoEllips.c
  #geoMag.c
  geoPoint.c
  geoStars.h
  geoSun.c
)

add_library(${target_name} STATIC
  ${${target_name}_SRC}
)

