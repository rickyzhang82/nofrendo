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
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
       
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
   if(path[len - 1] != '/' && len + 1 <= PATH_MAX)
   {
      path[len] = '/';
      path[++len] = 0;
   }
}

/* filename must be a valid filename, and therefore no longer than PATH_MAX */
static void removePath(char *filename)
{
   char temp[PATH_MAX + 1];
   int i;

   i = strlen(filename);
   while(filename[--i] != '/' && i > 0);

   if(filename[i] == '/')
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
   char cwd[PATH_MAX + 1];

   if(!checked)
   {
      checked = true;

      /* fall back to using cwd */
      getcwd(cwd, PATH_MAX);
      strncpy(dataPath, cwd, PATH_MAX);

      /* but default to using ~/.nofrendo/ if possible */
      if(getenv("HOME"))
      {
         char temp[PATH_MAX + 1];
         strncpy(temp, getenv("HOME"), PATH_MAX);
         addSlash(temp);
         strncat(temp, ".nofrendo", PATH_MAX - strlen(temp));

         if(!mkdir(temp, 0755) || errno == EEXIST)
         {
            log_printf("Succeeded in choosing HOME-based homeDirectory.\n");
            strncpy(dataPath, temp, PATH_MAX);
         }
      }
      
      /* make sure either path ends in a slash */
      addSlash(dataPath);
      log_printf("Storing data in %s\n", dataPath);
   }

   return dataPath;
}

/* This is os-specific part of main() */
int osd_main(int argc, char *argv[])
{
   static char configfilename[PATH_MAX + 1];

   /* command-line parameters */
   if(argc < 2)
   {
      printf("usage: %s IMAGE\n\n", argv[0]);
      printf("WHAP!\n");
      printf("WHAP!\n");
      printf("WHAP!\n");
      printf("Bad user!\n");
      printf("Run me correctly or you'll get more of the same!\n");
      return -1;
   }

   /* config file */
   strncpy(configfilename, dataDirectory(), PATH_MAX);
   strncat(configfilename, "config", PATH_MAX - strlen(configfilename));
   config.filename = configfilename;

   /* all done */
   return main_loop(argv[1], system_autodetect);
}

/* File system interface */
void osd_fullname(char *fullname, const char *shortname)
{
   strncpy(fullname, shortname, PATH_MAX);
}

/* This gives filenames for storage of saves */
char *osd_newextension(char *string, char *ext)
{
   char name[PATH_MAX + 1];

   removePath(string);
   removeExtension(string, ".gz");
   removeExtension(string, ".bz2");

   strncpy(name, dataDirectory(), PATH_MAX);
   strncat(name, string, PATH_MAX - strlen(name));
   strncat(name, ext, PATH_MAX - strlen(name));

   strcpy(string, name);

   log_printf("osd_newextension: %s\n", string);
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
      snprintf(filename, len, fullpath, pcx_num);

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
** Revision 1.6  2000/10/10 14:35:48  neil
** what's TRUE is not true
**
** Revision 1.5  2000/10/10 13:58:14  matt
** stroustrup squeezing his way in the door
**
** Revision 1.4  2000/10/10 13:04:15  matt
** Mr. Clean makes a guest appearance
**
** Revision 1.3  2000/08/04 15:01:32  neil
** BeOS cleanups
**
** Revision 1.2  2000/08/04 14:39:47  neil
** BeOS is working.. kinda
**
** Revision 1.1  2000/08/04 12:41:04  neil
** Initial commit
**
*/
