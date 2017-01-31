ruby 2.0.0 is created from 

https://github.com/ruby/ruby on tag v2_0_0_648

then there are two commits cherry picked on top

2250b195a9fcbbfe752dd6d46a702674ec144272
773248551b8cb46f281d5b6bb4f61477ce17d55d

Apply a patch like this..

diff Ruby/load.c
11a12,15
> int rb_hasFile(const char *t_filename);
>
> int rb_require_embedded(const char *t_filename);
>
1066a1071,1073
>   if( rb_hasFile(fname) ) {
>     return rb_require_embedded(fname);
>   } else {
1069a1077
>   }
diff Ruby/main.c
38a39,46
>
> int rb_hasFile(const char *t_filename) {
>   return 0;
> }
>
> int rb_require_embedded(const char *t_filename) {
>   return 0;
> }

This results in a 2.0.0 build of ruby that builds as static library.

