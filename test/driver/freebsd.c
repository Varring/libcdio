/* -*- C -*-
  Copyright (C) 2010 Rocky Bernstein <rocky@gnu.org>
  
  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/* 
   Unit test for lib/driver/freebsd.c
*/
#ifdef HAVE_CONFIG_H
# include "config.h"
#endif
#include <cdio/cdio.h>
#include <cdio/logging.h>

#ifdef HAVE_STDIO_H
#include <stdio.h>
#endif
#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif
#ifdef HAVE_STDLIB_H
#include <stdlib.h>
#endif
#include <string.h>

int
main(int argc, const char *argv[])
{
  CdIo_t *p_cdio;
  char **ppsz_drives=NULL;
  
  cdio_loglevel_default = (argc > 1) ? CDIO_LOG_DEBUG : CDIO_LOG_INFO;
  /* snprintf(psz_nrgfile, sizeof(psz_nrgfile)-1,
	     "%s/%s", TEST_DIR, cue_file[i]);
  */
  if (!cdio_have_driver(DRIVER_FREEBSD)) return(77);
  ppsz_drives = cdio_get_devices(DRIVER_DEVICE);
  if (!ppsz_drives) {
      printf("Can't find a CD-ROM drive. Skipping test.\n");
      exit(77);
  }
  
  p_cdio = cdio_open_freebsd(ppsz_drives[0]);
  if (p_cdio) {
      const char *psz_source = cdio_get_arg(p_cdio, "source");
      if (0 != strncmp(psz_source, ppsz_drives[0],
		       strlen(ppsz_drives[0]))) {
	  fprintf(stderr, 
		  "Got %s; should get back %s, the name we opened.\n",
		  psz_source, ppsz_drives[0]);
	  exit(1);
      }
  }
  
  
  cdio_destroy(p_cdio);
  {
      const char *access_mode[2] = {"ioctl", "CAM"};
      unsigned int i;
      for (i=0; i<=1; i++) {
	  p_cdio = cdio_open_am_freebsd(ppsz_drives[0], access_mode[i]);
	  if (p_cdio) {
	      const char *psz_access_mode = cdio_get_arg(p_cdio, "access-mode");
	      if (0 != strncmp(psz_access_mode, access_mode[i], 
			       strlen(access_mode[i]))) {
		  fprintf(stderr,
			  "Got %s; Should get back %s, the access mode requested.\n",
			  psz_access_mode, access_mode[i]);
		  exit(2);
	      }
	  }
      }
  }
  
  

  cdio_destroy(p_cdio);
  cdio_free_device_list(ppsz_drives);

  return 0;
}