// -*- mode: cpp; mode: fold -*-
// Description								/*{{{*/
// $Id: dpkgpm.h,v 1.8 2001/05/07 05:05:13 jgg Exp $
/* ######################################################################

   DPKG Package Manager - Provide an interface to dpkg
   
   ##################################################################### */
									/*}}}*/
#ifndef PKGLIB_DPKGPM_H
#define PKGLIB_DPKGPM_H

#include <apt-pkg/packagemanager.h>
#include <vector>
#include <stdio.h>

using std::vector;

class pkgDPkgPM : public pkgPackageManager
{
   private:
   int dpkgbuf_pos;
   char dpkgbuf[1024];

   protected:

   // used for progress reporting
   struct DpkgState 
   {
      const char *state;     // the dpkg state (e.g. "unpack")
      const char *str;       // the human readable translation of the state
   };
   
   struct Item
   {
      enum Ops {Install, Configure, Remove, Purge} Op;
      string File;
      PkgIterator Pkg;
      Item(Ops Op,PkgIterator Pkg,string File = "") : Op(Op),
            File(File), Pkg(Pkg) {};
      Item() {};
      
   };
   vector<Item> List;

   // Helpers
   bool RunScripts(const char *Cnf);
   bool RunScriptsWithPkgs(const char *Cnf);
   bool SendV2Pkgs(FILE *F);

   // input processing
   void DoStdin(int master);
   void DoTerminalPty(int master, FILE *out);
   void DoDpkgStatusFd(int statusfd);
   void ProcessDpkgStatusLine(char *line);


   // The Actuall installation implementation
   virtual bool Install(PkgIterator Pkg,string File);
   virtual bool Configure(PkgIterator Pkg);
   virtual bool Remove(PkgIterator Pkg,bool Purge = false);
   virtual bool Go(int StatusFd=-1);
   virtual void Reset();
   
   public:

   pkgDPkgPM(pkgDepCache *Cache);
   virtual ~pkgDPkgPM();
};

#endif
