#include <windows.h>
#include <io.h>
#include <fcntl.h>

#include <string>
#include <regex>

int wmain(int argc, wchar_t** argv) {
	std::wstring text;
	std::wregex wReg(L"^.*\\\\(.*\\.*)$");
	std::wsmatch wMatch;

	wchar_t* buf = NULL;
	int pPos, rPos;
	unsigned short flags = 0;

	for (int i = 1; i < argc; ++i)
		if (wcsncmp(argv[i], L"-h", 2) == 0) {
			wprintf(L"Usage: labudab\t-p <path>\n\t\t-r <regex|.exe|.jpg>\n\t\t-h help\n");
			return 0;
		} else if (wcsncmp(argv[i], L"-p", 2) == 0) {
			flags |= 1;
			pPos = ++i;
		} else if (wcsncmp(argv[i], L"-r", 2) == 0) {
			flags |= 2;
			rPos = ++i;
		}

	if (flags & 1) {
		text = argv[pPos];
	}else {
		wprintf(L"You should specify path. Usage: labudab -p <path>.\n");
		return 1;
	}

	if (flags & 2) {
		//Optimization!12312323748912394 we can call swprintf only once.
		int formattedLength = wcslen(argv[rPos]) + 10; // = swprintf(NULL, 0, L"^.*\\\\(.*%s)$", argv[rPos]);

		//Caution: Size of string is unlimited!

		//self-allocation-deallocation
		buf = (wchar_t*)malloc((formattedLength + 1) * sizeof(wchar_t));

		//automatic destruction and memory control
		//buf = make_unique<wchar_t>(formattedLength + 1);

		swprintf(buf, formattedLength + 1, L"^.*\\\\(.*%s)$", argv[rPos]);

		wReg = buf;

		free(buf);
	}

	if (std::regex_search(text, wMatch, wReg))
		wprintf(L"(1) %s\n(2) %s\n", wMatch.str(0).c_str(), wMatch.str(1).c_str());

	return 0;
}
