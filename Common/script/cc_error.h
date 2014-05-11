//=============================================================================
//
// Adventure Game Studio (AGS)
//
// Copyright (C) 1999-2011 Chris Jones and 2011-20xx others
// The full list of copyright holders can be found in the Copyright.txt
// file, which is part of this source code distribution.
//
// The AGS source code is provided under the Artistic License 2.0.
// A copy of this license can be found in the file License.txt and at
// http://www.opensource.org/licenses/artistic-license-2.0.php
//
//=============================================================================
//
// 'C'-style script compiler
//
//=============================================================================

#ifndef __CC_ERROR_H
#define __CC_ERROR_H

#include "script/cc_script.h"

extern void cc_error(const char *, ...);
extern void cc_warning(const char *, ...);

#define ERROR_SIZE  400

// error reporting
extern int ccError;             // set to non-zero if error occurs
extern int ccErrorLine;         // line number of the error
extern char ccErrorString[ERROR_SIZE]; // description of the error
extern char ccErrorCallStack[ERROR_SIZE];
extern bool ccErrorIsUserError;
extern const char *ccCurScriptName; // name of currently compiling script
extern char *ccWarningString; // List of warnings
extern int ccWarningCount; // Number of warnings generated
extern int *ccWarningLine; // Line numbers of warnings generated

#endif // __CC_ERROR_H
