/* vim: set tabstop=3 expandtab:
**
** This file is in the public domain.
**
** osd.c
**
** $Id: osd.c,v 1.2 2001/04/27 14:37:11 neil Exp $
**
*/

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifdef _MSC_VER
#include <direct.h>
#define  _getcwd getcwd
#else /* !_MSC_VER */
#include <sys/time.h>
#include <unistd.h>
#endif /* !_MSC_VER */

#include <noftypes.h>
#include <config.h>
#include <log.h>
#include <osd.h>
#include <nofrendo.h>
#include <version.h>

/* path must be a valid path, and therefore no longer than PATH_MAX */
static void addSlash(char *path)
{
   int len = strlen(path);
   if(path[len - 1] != '\\' && len + 1 <= PATH_MAX)
   {
      path[len] = '\\';
      path[++len] = 0;
   }
}

/* filename must be a valid filename, and therefore no longer than PATH_MAX */
static void removePath(char *filename)
{
   char temp[PATH_MAX + 1];
   int i;

   i = strlen(filename);
   while(filename[--i] != '\\' && i > 0);

   if(filename[i] == '\\')
   {
      strncpy(temp, (filename + i + 1), PATH_MAX);
      strncpy(filename, temp, PATH_MAX);
   }
}

/* if filename ends in extension, cut it off of filename */
static void removeExtension(char *filename, const char *extension)
{
   int i = strlen(filename);
   int j = strlen(extension);

   if(i <= j) return;

   while(i && j)
   {
      i--;
      j--;
      if(filename[i] != extension[j]) return;
   }
   filename[i] = 0;
}

/* this determines where to store our data files */
static const char *dataDirectory(void)
{
   static char dataPath[PATH_MAX + 1];
   static bool checked = false;

   if (false == checked)
   {
      checked = true;

      /* fall back to using cwd */
      getcwd(dataPath, PATH_MAX);
   }
   
   addSlash(dataPath);
   return dataPath;
}

/* This is os-specific part of main() */
int osd_main(int argc, char *argv[])
{
   static char configfilename[PATH_MAX + 1];

   /* command-line parameters */
   if (argc < 2)
   {
      printf("usage: %s IMAGE\n\n", argv[0]);
      printf("WHAP!\n");
      printf("WHAP!\n");
      printf("WHAP!\n");
      printf("Bad user!\n");
      printf("Run me correctly or you'll get more of the same!\n");
      return -1;
   }

   /* all done */
   return main_loop(argv[1], system_autodetect);
}

/* File system interface */
void osd_fullname(char *fullname, const char *shortname)
{
   strncpy(fullname, shortname, PATH_MAX);
}


/* This gives filenames for storage of saves */
/*
char *osd_newextension(char *string, char *ext)
{
   char name[PATH_MAX + 1];
   char fullpath[PATH_MAX + 1];

   strncpy(name, string, PATH_MAX);
   removePath(name);
   removeExtension(name, ".gz");
   removeExtension(name, ".bz2");

   strncpy(fullpath, dataDirectory(), PATH_MAX);
   strncat(fullpath, name, PATH_MAX - strlen(fullpath));

   strncat(fullpath, ext, PATH_MAX - strlen(fullpath));

//   log_printf("osd_newextension: %s + %s == %s\n", string, ext, fullpath);
   strncpy(string, fullpath, PATH_MAX);

   return string;
}
*/
char *osd_newextension(char *string, char *ext)
{
   if (strrchr(string, '.'))
      strcpy(strrchr(string, '.'), ext);
   else
      strcat(string, ext);
   return string;
}

/* This gives filenames for storage of PCX snapshots */
int osd_makesnapname(char *filename, int len)
{
   char fullpath[PATH_MAX + 1];
   struct stat stat_data;
   int pcx_num = -1;

   /* TODO: check if fullpath has %, \, or anything else special in it */
   strncpy(fullpath, dataDirectory(), PATH_MAX);
   strncat(fullpath, "snap%04d.pcx", PATH_MAX - strlen(fullpath));

   while (++pcx_num < 10000)
   {
      sprintf(filename, fullpath, pcx_num);

      if(stat(filename, &stat_data))
         return pcx_num;
   }

   return -1;
}

/*
** $Log: osd.c,v $
** Revision 1.2  2001/04/27 14:37:11  neil
** wheeee
**
** Revision 1.1.1.1  2001/04/27 07:03:54  neil
** initial
**
** Revision 1.12  2000/10/10 14:38:53  matt
** removed some mingw32 hacks
**
** Revision 1.11  2000/10/10 14:12:00  matt
** make way for bjarne
**
** Revision 1.10  2000/10/10 13:58:18  matt
** stroustrup squeezing his way in the door
**
** Revision 1.9  2000/10/10 13:06:37  matt
** Mr. Clean makes a guest appearance
**
** Revision 1.8  2000/07/31 04:30:29  matt
** one million cleanups
**
** Revision 1.7  2000/07/27 01:20:33  matt
** change to system type
**
** Revision 1.6  2000/07/26 21:36:16  neil
** Big honkin' change -- see the mailing list
**
** Revision 1.5  2000/07/25 02:24:52  matt
** temporary fix to osd_newextension
**
** Revision 1.4  2000/07/24 04:34:37  matt
** added hercules logging
**
** Revision 1.3  2000/07/23 16:47:16  matt
** fixed osd_newextension (partially)
**
** Revision 1.2  2000/07/23 15:07:06  matt
** got it going with visual C
**
** Revision 1.1  2000/07/23 00:48:15  neil
** Win32 SDL
**
**
*/
