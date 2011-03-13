// Copyright (C) 2008-2011 by Vincent Falco, All rights reserved worldwide.
// This file is released under the MIT License:
// http://www.opensource.org/licenses/mit-license.php

#ifndef __VF_MANUALWORKER_VFHEADER__
#define __VF_MANUALWORKER_VFHEADER__

#include "vf/vf_Worker.h"

//
// A Worker that requires periodic manual calls to process.
//

class ManualWorker : public Worker
{
public:
  explicit ManualWorker (const char* szName = "")
    : Worker (szName)
  {
    // Start out open to solve some RAII issues.
    // This may change in the future. It is still
    // required to call close() appropriately.
    open ();
  }

  void open ()    { Worker::open (); }
  void close ()   { Worker::close (); }
  bool process () { return Worker::process (); }

private:
  void signal ()  { }
  void reset ()   { }
};

#endif
