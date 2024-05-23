#pragma once

class Memory
{
public:
    Memory() : _processId(NULL), _processHandle(nullptr) { }

    bool CheckProcess(const wchar_t* name)
    {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

        if (snapshot != INVALID_HANDLE_VALUE)
        {
            PROCESSENTRY32W process = { sizeof(process) };

            if (Process32FirstW(snapshot, &process))
            {
                do
                {
                    if (_wcsicmp(process.szExeFile, name) == 0)
                    {
                        CloseHandle(snapshot);
                        return true;
                    }
                } while (Process32NextW(snapshot, &process));
            }

            CloseHandle(snapshot);
        }

        return false;
    }

    bool ModifyProcess(const wchar_t* name)
    {
        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (snapshot != INVALID_HANDLE_VALUE)
        {
            PROCESSENTRY32W process = { sizeof(process) };

            if (Process32FirstW(snapshot, &process))
            {
                do
                {
                    if (_wcsicmp(process.szExeFile, name) == NULL)
                    {
                        _processId = process.th32ProcessID;
                        CloseHandle(snapshot);

                        _processHandle = OpenProcess(PROCESS_VM_READ, FALSE, _processId);
                        return (_processHandle != nullptr);
                    }
                } while (Process32NextW(snapshot, &process));
            }

            CloseHandle(snapshot);
        }

        return FALSE;
    }

    uintptr_t GetModule(const wchar_t* name)
    {
        if (_processHandle == nullptr) return 0;

        HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, _processId);
        if (snapshot != INVALID_HANDLE_VALUE)
        {
            uintptr_t base = NULL;

            MODULEENTRY32W module = { sizeof(module) };
            if (Module32FirstW(snapshot, &module))
            {
                do
                {
                    if (_wcsicmp(module.szModule, name) == 0) { base = reinterpret_cast<uintptr_t>(module.modBaseAddr); break; }
                } while (Module32NextW(snapshot, &module));
            }

            CloseHandle(snapshot);

            return base;
        }
    }

    template <typename T>
    T ReadMemory(const uintptr_t& address)
    {
        T value;

        ReadProcessMemory(_processHandle, reinterpret_cast<LPVOID>(address), &value, sizeof(T), nullptr);

        return value;
    }

    template <typename T>
    void WriteMemory(const uintptr_t&, const T& value)
    {
        WriteProcessMemory(_processHandle, reinterpret_cast<LPVOID>(address), &value, sizeof(T), nullptr);
    }

    std::string ReadString(const uintptr_t& address, const size_t& size)
    {
        char* buffer = new char[size];

        ReadProcessMemory(_processHandle, reinterpret_cast<LPVOID>(address), buffer, size, nullptr);

        std::string result(buffer);
        delete[] buffer;

        return result;
    }

    void WriteString(const uintptr_t& address, const std::string& value)
    {
        WriteProcessMemory(_processHandle, reinterpret_cast<LPVOID>(address), value.c_str(), value.size(), nullptr);
    }

private:
    uint32_t _processId;
    HANDLE _processHandle;
};