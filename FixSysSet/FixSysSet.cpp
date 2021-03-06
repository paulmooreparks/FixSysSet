#include "pch.h"
#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <Windows.h>

void usage(bool showDescription)
{
	const wchar_t description[] =
		L"Changes the default drag sensitivity.\n"
		L"\n";

	const wchar_t parameters[] =
		L"FIXSYSSET [/SWCWD flag] [/?]\n"
		L"\n"
		L"  /SWCWD  Enable or disable setting to show window contents while dragging.\n"
		L"          A flag value of 1 enables the setting, and 0 disables it.\n"
		L"\n"
		L"  /?      Display help.\n";

	const wchar_t version[] =
		L"\n"
		L"Version 1.0\n"
		L"Paul M. Parks\n"
		L"paul@smartsam.com\n"
		L"https://github.com/paulmooreparks/FixSysSet\n";

	if (showDescription) {
		std::wcout << description;
	}

	std::wcout << parameters;

	if (showDescription) {
		std::wcout << version;
	}
}

int wmain(int argc, wchar_t* argv[])
{
	if (argc == 1) {
		usage(true);
		return 0;
	}

	if (_wcsicmp(argv[1], L"/?") == 0 || _wcsicmp(argv[1], L"-?") == 0) {
		usage(true);
		return 0;
	}

	typedef std::vector<std::wstring> Args;
	Args args;
	int i = 1;

	for (; i < argc; ++i) {
		args.push_back(argv[i]);
	}

	bool displaySettings = false;
	bool setSWCWD = false;
	bool flagSWCWD = false;

	std::wstringstream cvt;
	Args::const_iterator it = args.begin();

	while (it != args.end()) {
		cvt.clear();
		cvt.str(L"");

		std::wstring arg = *it;
		wchar_t c = arg[0];

		if (c == L'/' || c == L'-') {
			c = arg[1];
			std::wstring paramName = arg.substr(1, std::wstring::npos);
			std::transform(paramName.begin(), paramName.end(), paramName.begin(), ::toupper);

			if (paramName == L"?") {
				usage(true);
				return 0;
			}
			else if (paramName == L"SWCWD") {
				std::wstring flagValue;
				++it;

				if (it != args.end () && cvt << *it && cvt >> flagValue) {
					std::transform(flagValue.begin(), flagValue.end(), flagValue.begin(), ::toupper);
					setSWCWD = true;

					if (flagValue == L"0" || flagValue == L"FALSE" || flagValue == L"F" || flagValue == L"NO" ) {
						flagSWCWD = false;
					}
					else if (flagValue == L"1" || flagValue == L"TRUE" || flagValue == L"T" || flagValue == L"YES") {
						flagSWCWD = true;
					}
					else {
						setSWCWD = false;
						std::wcout << L"Incorrect parameter: " << *it << std::endl << std::endl;
						usage(false);
						return 2;
					}
				}
				else {
					std::wcout << L"Missing flag parameter for /SWCWD" << std::endl << std::endl;
					usage(false);
					return 2;
				}
			}
			else {
				std::wcout << L"Incorrect parameter: " << arg << std::endl << std::endl;
				usage(false);
				return 2;
			}
		}
		else {
			std::wcout << L"Incorrect parameter: " << arg << std::endl << std::endl;
			usage(false);
			return 2;
		}

		++it;
	}

	BOOL success = FALSE;

	if (setSWCWD) {
		success = SystemParametersInfo(SPI_SETDRAGFULLWINDOWS, flagSWCWD ? TRUE : FALSE, NULL, SPIF_UPDATEINIFILE | SPIF_SENDCHANGE);

		if (!success) {
			DWORD error = GetLastError();
			std::wcout << L"Error " << std::hex << error << std::dec << " while setting 'Set window contents while dragging'." << std::endl;
			return 1;
		}
	}

	return 0;
}

