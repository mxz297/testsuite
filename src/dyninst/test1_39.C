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

// $Id: test1_39.C,v 1.1 2008/10/30 19:19:28 legendre Exp $
/*
 * #Name: test1_39
 * #Desc: Regex search
 * #Dep: 
 * #Arch: sparc_sun_solaris2_4_test,alpha_dec_osf4_0_test,i386_unknown_solaris2_5_test,,i386_unknown_linux2_0_test,ia64_unknown_linux2_4_test,mips_sgi_irix6_4_test,rs6000_ibm_aix4_1_test,x86_64_unknown_linux2_4_test
 * #Notes:
 */

#include "BPatch.h"
#include "BPatch_Vector.h"

#include "test_lib.h"
#include "dyninst_comp.h"

class test1_39_Mutator : public DyninstMutator {
	virtual test_results_t executeTest();
};

extern "C" DLLEXPORT  TestMutator *test1_39_factory() 
{
	return new test1_39_Mutator();
}

//
//  Test case 39:  verify that regex search is working
//

test_results_t test1_39_Mutator::executeTest() 
{
	//  Note:  regex search by module is covered in test 21
#if defined(sparc_sun_solaris2_4_test) \
	|| defined(alpha_dec_osf4_0_test) \
	|| defined(i386_unknown_solaris2_5_test) \
	|| defined(i386_unknown_linux2_0_test) \
	|| defined(x86_64_unknown_linux2_4_test) /* Blind duplication - Ray */ \
	|| defined(ia64_unknown_linux2_4_test) \
	|| defined(mips_sgi_irix6_4_test) \
	|| defined(rs6000_ibm_aix4_1_test) \
	|| defined(os_linux_test) /* Use OS #define instead of platform - Greg */

	// ^^^ Not Windows

	BPatch_Vector<BPatch_function *> bpmv;

	//  Not meant to be an extensive check of all regex usage, just that
	//  the basic mechanism that deals with regexes is not broken

	//   regex "^func12" should match all functions that begin with "func12"

	if (NULL == appImage->findFunction("^test1_39_", bpmv) || (bpmv.size() != 2)) 
	{

		logerror("**Failed test #39 (regex function search)\n");
		logerror("  Expected 2 functions matching ^test1_39_, got %d\n",
				bpmv.size());
		char buf[128];
		for (unsigned int i = 0; i < bpmv.size(); ++i)
			logerror("  matched function: %s\n",
					bpmv[i]->getName(buf, 128));
		return FAILED;
	}

	bpmv.clear();

	if (NULL == appImage->findFunction("^test1_39_func1", bpmv) 
			|| (bpmv.size() != 1)) 
	{
		logerror("**Failed test #39 (regex function search)\n");
		logerror("  Expected 1 function matching ^test1_39_func1, got %d\n",
				bpmv.size());
		return FAILED;
	}

	if (isMutateeFortran(appImage)) 
	{
		return PASSED;
	}

	//  Now lets look for a pattern that ought to match something
	//  in libc (can't check number of hits since libc may vary,
	//  but can check existence)

	bpmv.clear();
	const char *libc_regex = "^sp";

	if (NULL == appImage->findFunction(libc_regex, bpmv) 
			|| (!bpmv.size())) 
	{
		logerror("**Failed test #39 (regex function search)\n");
		logerror("  Expected function(s) matching %s\n",libc_regex);
		return FAILED;
	}

	BPatch_variableExpr *passedExpr = appImage->findVariable("test1_39_passed");

	if (passedExpr == NULL) 
	{
		logerror("**Failed** test1_39 (regex function search)\n");
		logerror("    Unable to locate test1_39_passed\n");
		return FAILED;
	} 

	int pvalue = 1;
	passedExpr->writeValue(&pvalue);

	return PASSED;
#else // Windows
	return SKIPPED;
#endif
}