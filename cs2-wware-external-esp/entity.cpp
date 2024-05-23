struct Entity
{
    uintptr_t address;

    Vec3 position;
    Vec2 positionScreen;

    Vec3 height;
    Vec2 heightScreen;

    std::string name;

    uint8_t team;
    uint8_t health;

    bool spotted;
    float distance;

    std::string weapon;

    std::vector<Vec3> bones;
    std::vector<Vec2> bonesScreen;
};

std::string GetWeaponName(uint16_t index)
{
    switch (index)
    {
        case 1: return "Deagle";
        case 2: return "Berrettas";
        case 3: return "Fiveseven";
        case 4: return "Glock";
        case 7: return "Ak47";
        case 8: return "Aug";
        case 9: return "Awp";
        case 10: return "Famas";
        case 11: return "G3gg1";
        case 14: return "M249";
        case 13: return "Galilar";
        case 17: return "Mac10";
        case 19: return "P90";
        case 24: return "Ump45";
        case 25: return "Xm1014";
        case 26: return "Bizon";
        case 27: return "Mag7";
        case 28: return "Negev";
        case 29: return "Sawedoff";
        case 30: return "Tec9";
        case 31: return "Zeus";
        case 32: return "P2000";
        case 33: return "Mp7";
        case 34: return "Mp9";
        case 35: return "Nova";
        case 36: return "P250";
        case 38: return "Scar20";
        case 39: return "Sg556";
        case 40: return "Ssg08";
        case 42: return "Knife";
        case 43: return "Flashbang";
        case 44: return "Hegrenade";
        case 45: return "Smokegrenade";
        case 46: return "Molotov";
        case 47: return "Decoy";
        case 48: return "Incgrenade";
        case 49: return "C4";
        case 16: return "M4a4";
        case 61: return "UspS";
        case 60: return "M4a1s";
        case 63: return "Cz75a";
        case 64: return "Revolver";
        case 59: return "Knife";

        default: return "Unknown";
    }
}

Vec2 screen = Vec2(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));

struct ViewMatrix
{
    float m11, m12, m13, m14;
    float m21, m22, m23, m24;
    float m31, m32, m33, m34;
    float m41, m42, m43, m44;
};

Vec2 WorldToScreen(const ViewMatrix& matrix, const Vec3& position, const Vec2& screen)
{
    float screenW = (matrix.m41 * position.x) + (matrix.m42 * position.y) + (matrix.m43 * position.z) + matrix.m44;

    if (screenW > std::numeric_limits<float>::epsilon())
    {
        float screenX = (matrix.m11 * position.x) + (matrix.m12 * position.y) + (matrix.m13 * position.z) + matrix.m14;
        float screenY = (matrix.m21 * position.x) + (matrix.m22 * position.y) + (matrix.m23 * position.z) + matrix.m24;

        float x = (screen.x / 2) + (screen.x / 2) * screenX / screenW;
        float y = (screen.y / 2) - (screen.y / 2) * screenY / screenW;

        return Vec2(x, y);
    }

    else return Vec2(std::numeric_limits<float>::quiet_NaN(), std::numeric_limits<float>::quiet_NaN());
}

enum class Bones : uint8_t
{
    Waist = 0,
    Neck = 5,
    Head = 6,
    ShoulderLeft = 8,
    ForeLeft = 9,
    HandLeft = 11,
    ShoulderRight = 13,
    ForeRight = 14,
    HandRight = 16,
    KneeLeft = 23,
    FeetLeft = 24,
    KneeRight = 26,
    FeetRight = 27
};

std::vector<Vec3> ReadBones(const uintptr_t& address)
{
    std::vector<Vec3> bones(static_cast<size_t>(Bones::FeetRight) + 1);

    for (uint8_t bone = static_cast<uint8_t>(Bones::Waist); bone <= static_cast<uint8_t>(Bones::FeetRight); bone++)
        bones[bone] = memory.ReadMemory<Vec3>(address + bone * 32);

    return bones;
}

std::vector<Vec2> BonesToScreen(const std::vector<Vec3>& bones, const ViewMatrix& matrix, const Vec2& screen)
{
    std::vector<Vec2> bonesScreen;
    bonesScreen.reserve(bones.size());

    for (const Vec3& bone : bones)
        bonesScreen.push_back(WorldToScreen(matrix, bone, screen));

    return bonesScreen;
}

Entity localPlayer;
Entity entity;
std::vector<Entity> entities;

void PushEntity()
{
    const uintptr_t client = memory.GetModule(L"client.dll");

    while (true)
    {
        entities.clear();

        ViewMatrix viewMatrix = memory.ReadMemory<ViewMatrix>(client + Offsets::dwViewMatrix);

        uintptr_t entityList = memory.ReadMemory<uintptr_t>(client + Offsets::dwEntityList);
        if (entityList == NULL) continue;

        uintptr_t listEntry = memory.ReadMemory<uintptr_t>(entityList + 0x10);
        if (listEntry == NULL) continue;

        localPlayer.address = memory.ReadMemory<uintptr_t>(client + Offsets::dwLocalPlayerPawn);
        if (localPlayer.address == NULL) continue;

        localPlayer.position = memory.ReadMemory<Vec3>(localPlayer.address + Offsets::m_vOldOrigin);
        localPlayer.team = memory.ReadMemory<uint8_t>(localPlayer.address + Offsets::m_iTeamNum);

        for (uint8_t i = 0; i < 32; i++)
        {
            uintptr_t currentController = memory.ReadMemory<uintptr_t>(listEntry + i * 0x78);
            if (currentController == NULL) continue;

            uintptr_t pawnHandle = memory.ReadMemory<uintptr_t>(currentController + Offsets::m_hPlayerPawn);
            if (pawnHandle == NULL) continue;

            uintptr_t listShift = memory.ReadMemory<uintptr_t>(entityList + 0x8 * ((pawnHandle & 0x7FFF) >> 9) + 0x10);

            entity.address = memory.ReadMemory<uintptr_t>(listShift + 0x78 * (pawnHandle & 0x1FF));
            if (localPlayer.address == entity.address) continue;

            uint16_t lifeState = memory.ReadMemory<uint16_t>(entity.address + Offsets::m_lifeState);
            if (lifeState != 256) continue;

            uintptr_t gameSceneNode = memory.ReadMemory<uintptr_t>(entity.address + Offsets::m_pGameSceneNode);
            if (gameSceneNode == NULL) continue;

            uintptr_t boneMatrix = memory.ReadMemory<uintptr_t>(gameSceneNode + Offsets::m_modelState + 0x80);
            if (boneMatrix == NULL) continue;

            uintptr_t clippingWeapon = memory.ReadMemory<uintptr_t>(entity.address + Offsets::m_pClippingWeapon);
            if (clippingWeapon == NULL) continue;

            entity.position = memory.ReadMemory<Vec3>(entity.address + Offsets::m_vOldOrigin);
            entity.positionScreen = WorldToScreen(viewMatrix, entity.position, screen);

            entity.height = memory.ReadMemory<Vec3>(entity.address + Offsets::m_vecViewOffset);
            entity.heightScreen = WorldToScreen(viewMatrix, entity.position + entity.height, screen);

            entity.distance = Vec3::Distance(entity.position, localPlayer.position);
            entity.spotted = memory.ReadMemory<bool>(entity.address + Offsets::m_entitySpottedState + Offsets::m_bSpotted);

            entity.name = memory.ReadString(currentController + Offsets::m_iszPlayerName, 128);

            entity.team = memory.ReadMemory<uint8_t>(entity.address + Offsets::m_iTeamNum);
            entity.health = memory.ReadMemory<uint8_t>(entity.address + Offsets::m_iHealth);

            entity.bones = ReadBones(boneMatrix);
            entity.bonesScreen = BonesToScreen(entity.bones, viewMatrix, screen);

            entity.weapon = GetWeaponName(memory.ReadMemory<uint16_t>(clippingWeapon + Offsets::m_AttributeManager + Offsets::m_Item + Offsets::m_iItemDefinitionIndex));

            entities.push_back(entity);
        }

        std::sort(entities.begin(), entities.end(), [](const Entity& a, const Entity& b) { return a.distance < b.distance; });

        std::this_thread::sleep_for(std::chrono::milliseconds(8));
    }
}