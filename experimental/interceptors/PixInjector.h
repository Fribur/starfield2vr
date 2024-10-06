#pragma once
#include <filesystem>
#include <shlobj.h>

namespace WindowsDebug {

    inline std::wstring GetLatestWinPixGpuCapturerPath()
    {
        LPWSTR programFilesPath = nullptr;
        SHGetKnownFolderPath(FOLDERID_ProgramFiles, KF_FLAG_DEFAULT, NULL, &programFilesPath);

        std::filesystem::path pixInstallationPath = programFilesPath;
        pixInstallationPath /= "Microsoft PIX";

        std::wstring newestVersionFound;

        for (auto const& directory_entry : std::filesystem::directory_iterator(pixInstallationPath))
        {
            if (directory_entry.is_directory())
            {
                if (newestVersionFound.empty() || newestVersionFound < directory_entry.path().filename().c_str())
                {
                    newestVersionFound = directory_entry.path().filename().c_str();
                }
            }
        }

        if (newestVersionFound.empty())
        {
            // TODO: Error, no PIX installation found
        }

        return pixInstallationPath / newestVersionFound / L"WinPixGpuCapturer.dll";
    }

    inline void init() {
        if (GetModuleHandle("WinPixGpuCapturer.dll") == 0)
        {
            LoadLibraryW(GetLatestWinPixGpuCapturerPath().c_str());
        }
    }


}
