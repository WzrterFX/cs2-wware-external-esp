#pragma once

uintptr_t FindDumped(const std::string& site, const std::string& name)
{
    HINTERNET internet = InternetOpenA("", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
    if (!internet) return 0;

    HINTERNET connect = InternetOpenUrlA(internet, site.c_str(), NULL, NULL, INTERNET_FLAG_RELOAD, NULL);
    if (!connect) { InternetCloseHandle(internet); return 0; }

    std::string content;

    char page[512];
    DWORD read;
    while (InternetReadFile(connect, page, sizeof(page), &read) && read > 0)
        content.append(page, read);

    InternetCloseHandle(connect);
    InternetCloseHandle(internet);

    std::istringstream iss(content);
    std::string line;

    while (std::getline(iss, line))
    {
        if (line.find("//") == 0) continue;

        if (line.find(name + " ") == 0 || line.find(" " + name + " ") != std::string::npos)
        {
            uint64_t equal = line.find("=");

            if (equal != std::string::npos)
                return std::stoull(line.substr(equal + 1), nullptr, 0);
        }
    }

    return 0;
}

namespace Offsets
{
    const std::string offsetsUrl = "https://raw.githubusercontent.com/a2x/cs2-dumper/main/output/offsets.hpp";
    const std::string clientUrl = "https://raw.githubusercontent.com/a2x/cs2-dumper/main/output/client.dll.hpp";

    const uintptr_t dwLocalPlayerPawn = FindDumped(offsetsUrl, "dwLocalPlayerPawn");
    const uintptr_t dwEntityList = FindDumped(offsetsUrl, "dwEntityList");
    const uintptr_t dwViewMatrix = FindDumped(offsetsUrl, "dwViewMatrix");

    const uint32_t m_iTeamNum = FindDumped(clientUrl, "m_iTeamNum");
    const uint32_t m_lifeState = FindDumped(clientUrl, "m_lifeState");
    const uint32_t m_iHealth = FindDumped(clientUrl, "m_iHealth");
    const uint32_t m_iszPlayerName = FindDumped(clientUrl, "m_iszPlayerName");
    const uint32_t m_hPlayerPawn = FindDumped(clientUrl, "m_hPlayerPawn");
    const uint32_t m_vOldOrigin = FindDumped(clientUrl, "m_vOldOrigin");
    const uint32_t m_vecViewOffset = FindDumped(clientUrl, "m_vecViewOffset");

    const uint32_t m_entitySpottedState = FindDumped(clientUrl, "m_entitySpottedState");
    const uint32_t m_bSpotted = FindDumped(clientUrl, "m_bSpotted");

    const uint32_t m_pClippingWeapon = FindDumped(clientUrl, "m_pClippingWeapon");
    const uint32_t m_AttributeManager = FindDumped(clientUrl, "m_AttributeManager");
    const uint32_t m_Item = FindDumped(clientUrl, "m_Item");
    const uint32_t m_iItemDefinitionIndex = FindDumped(clientUrl, "m_iItemDefinitionIndex");

    const uint32_t m_pGameSceneNode = FindDumped(clientUrl, "m_pGameSceneNode");
    const uint32_t m_modelState = FindDumped(clientUrl, "m_modelState");
}