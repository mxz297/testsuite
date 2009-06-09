/*
 * Copyright (c) 1996-2004 Barton P. Miller
 * 
 * We provide the Paradyn Parallel Performance Tools (below
 * described as "Paradyn") on an AS IS basis, and do not warrant its
 * validity or performance.  We reserve the right to update, modify,
 * or discontinue this software at any time.  We shall have no
 * obligation to supply such updates or modifications or any other
 * form of support to you.
 * 
 * This license is for research uses.  For such uses, there is no
 * charge. We define "research use" to mean you may freely use it
 * inside your organization for whatever purposes you see fit. But you
 * may not re-distribute Paradyn or parts of Paradyn, in any form
 * source or binary (including derivatives), electronic or otherwise,
 * to any other organization or entity without our permission.
 * 
 * (for other uses, please contact us at paradyn@cs.wisc.edu)
 * 
 * All warranties, including without limitation, any warranty of
 * merchantability or fitness for a particular purpose, are hereby
 * excluded.
 * 
 * By your use of Paradyn, you understand and agree that we (or any
 * other person or entity with proprietary rights in Paradyn) are
 * under no obligation to provide either maintenance services,
 * update services, notices of latent defects, or correction of
 * defects for Paradyn.
 * 
 * Even if advised of the possibility of such damages, under no
 * circumstances shall we (or any other person or entity with
 * proprietary rights in the software licensed hereunder) be liable
 * to you or any third party for direct, indirect, or consequential
 * damages of any character regardless of type of action, including,
 * without limitation, loss of profits, loss of use, loss of good
 * will, or computer failure or malfunction.  You agree to indemnify
 * us (and any other person or entity with proprietary rights in the
 * software licensed hereunder) for any and all liability it may
 * incur to third parties resulting from your use of Paradyn.
 */

// $Id: test2_1.C,v 1.1 2008/10/30 19:20:09 legendre Exp $
/*
 * #Name: test2_1
 * #Desc: Run an executable that does not exist
 * #Dep: 
 * #Arch: all
 * #Notes:
 */

#include "BPatch.h"
#include "BPatch_Vector.h"
#include "BPatch_thread.h"
#include "BPatch_snippet.h"

#include "test_lib.h"
#include "Callbacks.h"

#include "dyninst_comp.h"
class test2_1_Mutator : public DyninstMutator {
  bool useAttach;
  BPatch *bpatch;

  virtual bool hasCustomExecutionPath() { return true; }
  virtual test_results_t setup(ParameterDict &param);
  virtual test_results_t executeTest();
};
extern "C" DLLEXPORT DLLEXPORT TestMutator *test2_1_factory() {
  return new test2_1_Mutator();
}

//
// Test #1 - run an executable that does not exist
//	Attempt to run a program file that does not exist.  Should return a
//	null values from createProcess (called via mutatorMAIN)
//

// static int mutatorTest(BPatch *bpatch, bool useAttach)
test_results_t test2_1_Mutator::executeTest() {

    if (useAttach) {
	logerror("Skipping test #1 (run an executable that does not exist)\n");
	logerror("    not relevant with -attach option\n");
        return SKIPPED;
    } else {
	// try to run a program that does not exist
        clearError();
	BPatch_thread *ret = startMutateeTest(bpatch, "./noSuchFile", "test2_1", useAttach, NULL, NULL);
        bool gotError = getError();
	if (ret || !gotError) {
	    logerror("**Failed** test #1 (run an executable that does not exist)\n");
	    if (ret)
		logerror("    created a thread handle for a non-existant file\n");
	    if (!gotError)
		logerror("    the error callback should have been called but wasn't\n");
            return FAILED;
	} else {
	    logerror("Passed test #1 (run an executable that does not exist)\n");
            return PASSED;
	}
    }
}

// extern "C" TEST_DLL_EXPORT int test2_1_mutatorMAIN(ParameterDict &param)
test_results_t test2_1_Mutator::setup(ParameterDict &param) {
  useAttach = param["useAttach"]->getInt();
  bpatch = (BPatch *)(param["bpatch"]->getPtr());

  if (useAttach) {
    logerror("Skipping test #1 (run an executable that does not exist)\n");
    logerror("    not relevant with -attach option\n");
    return SKIPPED;
  } else {
    return PASSED;
  }
}