#define _WIN32_WINNT 0x0500

#include <windows.h>
#include <AccCtrl.h>
#include <Aclapi.h>
#include <Sddl.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, INT nShow)
{
	HDESK hDefaultDesktop;
	HDESK hWoWDesktop;
	HWINSTA hDefaultWinSta;

	STARTUPINFOW si;
	PROCESS_INFORMATION pi;

	PSECURITY_DESCRIPTOR pSD;
	PACL pDefaultACL, pNewACL;
	EXPLICIT_ACCESS wowAccessEntry;

	hDefaultDesktop = GetThreadDesktop(GetCurrentThreadId());
	hWoWDesktop = CreateDesktop("WoWDesktop", NULL, NULL, 0, GENERIC_ALL, NULL);
	if (hWoWDesktop) {
		hDefaultWinSta = OpenWindowStation("winsta0", FALSE, GENERIC_ALL);

		GetSecurityInfo(hDefaultWinSta, SE_WINDOW_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &pDefaultACL, NULL, &pSD);
		
		wowAccessEntry.grfAccessMode = SET_ACCESS;
		wowAccessEntry.grfAccessPermissions = GENERIC_READ | GENERIC_WRITE | GENERIC_EXECUTE;
		wowAccessEntry.grfInheritance = NO_INHERITANCE;
		
		wowAccessEntry.Trustee.pMultipleTrustee = NULL;
		wowAccessEntry.Trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
		wowAccessEntry.Trustee.TrusteeForm = TRUSTEE_IS_NAME;
		wowAccessEntry.Trustee.TrusteeType = TRUSTEE_IS_USER;
		wowAccessEntry.Trustee.ptstrName = "CENTURION\\wow";

		SetEntriesInAcl(1, &wowAccessEntry, pDefaultACL, &pNewACL);
		SetSecurityInfo(hDefaultWinSta, SE_WINDOW_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, pNewACL, NULL);
		LocalFree(pDefaultACL);
		LocalFree(pNewACL);

		GetSecurityInfo(hWoWDesktop, SE_WINDOW_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &pDefaultACL, NULL, &pSD);
		wowAccessEntry.grfAccessPermissions = GENERIC_ALL;
		SetEntriesInAcl(1, &wowAccessEntry, pDefaultACL, &pNewACL);
		SetSecurityInfo(hWoWDesktop, SE_WINDOW_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, pNewACL, NULL);
		LocalFree(pDefaultACL);
		LocalFree(pNewACL);
		
		RtlZeroMemory(&si, sizeof(si));
		si.cb = sizeof(si);
		si.lpDesktop = L"winsta0\\WoWDesktop";

		CreateProcessWithLogonW(L"wow", L".", L"buhinrxaglstuyki", LOGON_WITH_PROFILE, L"c:\\windows\\system32\\cmd.exe", NULL, 0, NULL,
			L"c:\\Games\\World of Warcraft", &si, &pi);

		SwitchDesktop(hWoWDesktop);
		WaitForSingleObject(pi.hProcess, INFINITE);
		SwitchDesktop(hDefaultDesktop);
		CloseDesktop(hWoWDesktop);
	}
	/*
	HDESK hDefaultDesktop;
	HDESK hWoWDesktop;
	SECURITY_ATTRIBUTES newDesktopSecurity;
	PSECURITY_DESCRIPTOR newDesktopSecurityDescriptor = NULL;
	EXPLICIT_ACCESS ea;
	PACL desktopACL = NULL;
	STARTUPINFOW si;
	PROCESS_INFORMATION pi;
	DWORD err;

	hDefaultDesktop = GetThreadDesktop(GetCurrentThreadId());

	ea.grfAccessMode = SET_ACCESS;
	ea.grfAccessPermissions = GENERIC_ALL;
	ea.grfInheritance = NO_INHERITANCE;

	RtlZeroMemory(&ea.Trustee, sizeof(ea.Trustee));
	ea.Trustee.TrusteeForm = TRUSTEE_IS_NAME;
	ea.Trustee.ptstrName = "wow";
	ea.Trustee.TrusteeType = TRUSTEE_IS_USER;

	SetEntriesInAcl(1, &ea, NULL, &desktopACL);

	newDesktopSecurityDescriptor = (PSECURITY_DESCRIPTOR)LocalAlloc(LPTR, SECURITY_DESCRIPTOR_MIN_LENGTH);
	InitializeSecurityDescriptor(newDesktopSecurityDescriptor, SECURITY_DESCRIPTOR_REVISION);
	SetSecurityDescriptorDacl(newDesktopSecurityDescriptor, TRUE, desktopACL, FALSE);

	newDesktopSecurity.nLength = sizeof(newDesktopSecurity);
	newDesktopSecurity.lpSecurityDescriptor = newDesktopSecurityDescriptor;
	newDesktopSecurity.bInheritHandle = TRUE;

	hWoWDesktop = CreateDesktop("WoWDesktop", NULL, NULL, 0, GENERIC_ALL, NULL);

	RtlZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	//si.lpDesktop = L"default";

	err = GetLastError();

	/*SwitchDesktop(hWoWDesktop);
	WaitForSingleObject(pi.hProcess, INFINITE);
	SwitchDesktop(hDefaultDesktop);

	LocalFree(newDesktopSecurityDescriptor);
	LocalFree(desktopACL);*/
/*
	PACL pACL, pNewAcl;
	PSECURITY_DESCRIPTOR sd;
	ULONG nACEs;
	PEXPLICIT_ACCESS ea;
	EXPLICIT_ACCESS newExplicitAccess;
	TCHAR acctName[512];
	DWORD acctNameSize = 512;
	TCHAR domName[512];
	DWORD domNameSize = 512;
	SID_NAME_USE foobar;
	LPTSTR sidStr;
	int i;

	STARTUPINFOW si;
	PROCESS_INFORMATION pi;

	HDESK hDesktop = GetThreadDesktop(GetCurrentThreadId());
	
	GetSecurityInfo(hDesktop, SE_WINDOW_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, &pACL, NULL, &sd);
	GetExplicitEntriesFromAcl(pACL, &nACEs, &ea);

	for (i = 0; i < nACEs; i++) {
		acctNameSize = 512;
		domNameSize = 512;
		LookupAccountSid(NULL, ea[i].Trustee.ptstrName, acctName, &acctNameSize, domName, &domNameSize, &foobar);
	}

	newExplicitAccess.grfAccessMode = DENY_ACCESS;
	newExplicitAccess.grfAccessPermissions = DESKTOP_ENUMERATE | DESKTOP_READOBJECTS | DESKTOP_HOOKCONTROL |
											 DESKTOP_JOURNALPLAYBACK | DESKTOP_JOURNALRECORD | DESKTOP_WRITEOBJECTS |
											 DESKTOP_SWITCHDESKTOP;
	newExplicitAccess.grfInheritance = NO_INHERITANCE;
	newExplicitAccess.Trustee.pMultipleTrustee = NULL;
	newExplicitAccess.Trustee.MultipleTrusteeOperation = NO_MULTIPLE_TRUSTEE;
	newExplicitAccess.Trustee.TrusteeForm = TRUSTEE_IS_NAME;
	newExplicitAccess.Trustee.TrusteeType = TRUSTEE_IS_USER;
	newExplicitAccess.Trustee.ptstrName = "CENTURION\\wow";

	SetEntriesInAcl(1, &newExplicitAccess, pACL, &pNewAcl);

	SetSecurityInfo(hDesktop, SE_WINDOW_OBJECT, DACL_SECURITY_INFORMATION, NULL, NULL, pNewAcl, NULL);
	LocalFree(ea);
	LocalFree(sd);

	RtlZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	CreateProcessWithLogonW(L"wow", L".", L"buhinrxaglstuyki", LOGON_WITH_PROFILE, L"c:\\windows\\system32\\cmd.exe", NULL, 0, NULL,
		L"c:\\Games\\World of Warcraft", &si, &pi);
	*/
	return 0;
}