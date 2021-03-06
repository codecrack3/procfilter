//
// The MIT License (MIT)
//
// Copyright (c) 2016 GoDaddy Operating Company, LLC.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

#include <Windows.h>

#include "isadmin.hpp"

bool
IsAdmin()
{
	bool rv = false;

	BOOL bIsAdmin = FALSE;
	
	SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
	PSID AdminGroup;
	if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &AdminGroup)) {
		if (CheckTokenMembership(NULL, AdminGroup, &bIsAdmin) && bIsAdmin) {
			rv = true;
		}
		FreeSid(AdminGroup);
	}

	return rv;
}


bool
IsElevated(HANDLE hProcess, bool *lpbIsElevated)
{
	bool rv = false;
	HANDLE hTokenHandle = NULL;
	if (OpenProcessToken(hProcess, TOKEN_QUERY, &hTokenHandle)) {
		TOKEN_ELEVATION teTokenElevation;
		DWORD dwBytesStored = 0;
		if (GetTokenInformation(hTokenHandle, TokenElevation, &teTokenElevation, sizeof(TOKEN_ELEVATION), &dwBytesStored)) {
			if (dwBytesStored >= sizeof(TOKEN_ELEVATION)) {
				rv = true;
				if (lpbIsElevated) *lpbIsElevated = teTokenElevation.TokenIsElevated ? true : false;
			}
		}

		CloseHandle(hTokenHandle);
	}

	return rv;
}
