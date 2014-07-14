/**********************************************************************
*  Copyright (c) 2008-2014, Alliance for Sustainable Energy.  
*  All rights reserved.
*  
*  This library is free software; you can redistribute it and/or
*  modify it under the terms of the GNU Lesser General Public
*  License as published by the Free Software Foundation; either
*  version 2.1 of the License, or (at your option) any later version.
*  
*  This library is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*  Lesser General Public License for more details.
*  
*  You should have received a copy of the GNU Lesser General Public
*  License along with this library; if not, write to the Free Software
*  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
**********************************************************************/

#include <ruby.h>

#ifdef HAVE_RUBY_VERSION_H
#include <ruby/version.h>
#endif


#if RUBY_API_VERSION_MAJOR && (RUBY_API_VERSION_MAJOR==2 || (RUBY_API_VERSION_MAJOR==1 && RUBY_API_VERSION_MINOR==9))
int main(int argc, char **argv)
{
  ruby_sysinit(&argc, &argv);
  {
    RUBY_INIT_STACK;
    ruby_init();
    return ruby_run_node(ruby_options(argc, argv));
  }
}

#else

int main(int argc, char *argv[])
{
  ruby_init();
  ruby_options(argc, argv);
  ruby_run();

  return EXIT_SUCCESS;
}
#endif
