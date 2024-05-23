ImFont* litle;
ImFont* medium;
ImFont* large;

float mainColor[3] = { 0.925, 0, 0.15 };

void DrawStyle()
{
    ImGuiIO& io = ImGui::GetIO();

    io.IniFilename = NULL;
    io.LogFilename = NULL;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

    ImFontConfig font;

    litle = io.Fonts->AddFontFromMemoryTTF((ImFontConfig*)Data::font, sizeof(Data::font), 14.5, &font);
    medium = io.Fonts->AddFontFromMemoryTTF((ImFontConfig*)Data::font, sizeof(Data::font), 16.5, &font);
    large = io.Fonts->AddFontFromMemoryTTF((ImFontConfig*)Data::font, sizeof(Data::font), 18.5, &font);

    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowPadding = ImVec2(0, 0);
    style.FramePadding = ImVec2(4, 4);
    style.ItemSpacing = ImVec2(6, 6);
    style.ItemInnerSpacing = ImVec2(6, 6);

    style.IndentSpacing = 24;
    style.ScrollbarSize = 12;
    style.GrabMinSize = 24;

    style.WindowBorderSize = 0;
    style.PopupBorderSize = 4;
    style.FrameBorderSize = 4;
    style.ChildBorderSize = 4;

    style.ScrollbarRounding = 8;
    style.WindowRounding = 4;
    style.ChildRounding = 4;
    style.FrameRounding = 4;
    style.PopupRounding = 4;
    style.GrabRounding = 4;
    style.TabRounding = 4;

    style.AntiAliasedFill = true;
    style.AntiAliasedLines = true;

    ImVec4* colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_Text] = ImVec4(0.8, 0.8, 0.8, 1);

    colors[ImGuiCol_WindowBg] = ImVec4(0.045, 0.045, 0.045, 0.965);
    colors[ImGuiCol_PopupBg] = ImVec4(0.045, 0.045, 0.045, 0.95);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.045, 0.045, 0.045, 0.65);
    colors[ImGuiCol_ChildBg] = ImVec4(0.045, 0.045, 0.045, 0.25);

    colors[ImGuiCol_FrameBg] = ImVec4(0.1, 0.1, 0.1, 0.2);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.1, 0.1, 0.1, 0.3);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.1, 0.1, 0.1, 0.5);

    colors[ImGuiCol_Separator] = ImVec4(0, 0, 0, 0);
    colors[ImGuiCol_MenuBarBg] = ImVec4(0, 0, 0, 0);
    colors[ImGuiCol_Header] = ImVec4(0, 0, 0, 0);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0, 0, 0, 0);
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0, 0, 0, 0);
    colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0);
    colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

    colors[ImGuiCol_Button] = ImVec4(0.045, 0.045, 0.045, 0.3);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.045, 0.045, 0.045, 0.3);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.045, 0.045, 0.045, 0.3);

    colors[ImGuiCol_SliderGrab] = ImVec4(mainColor[0], mainColor[1], mainColor[2], 0.9);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(mainColor[0], mainColor[1], mainColor[2], 1);

    colors[ImGuiCol_ScrollbarGrab] = ImVec4(mainColor[0], mainColor[1], mainColor[2], 0.9);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(mainColor[0], mainColor[1], mainColor[2], 1);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(mainColor[0], mainColor[1], mainColor[2], 1);

    colors[ImGuiCol_CheckMark] = ImVec4(mainColor[0], mainColor[1], mainColor[2], 1);
}

void DrawBase(Entity entity, ImDrawList* drawList)
{
    ImVec4 color = (localPlayer.team == entity.team) ? ImVec4(Vars::Esp::team.x, Vars::Esp::team.y, Vars::Esp::team.z, Vars::Esp::team.w) : ImVec4(Vars::Esp::enemy.x, Vars::Esp::enemy.y, Vars::Esp::enemy.z, Vars::Esp::enemy.w);
    if (!entity.spotted) color.w *= 0.75;

    float height = entity.positionScreen.y - entity.heightScreen.y;

    ImVec2 start = ImVec2(entity.heightScreen.x - height / 3, entity.heightScreen.y);
    ImVec2 end = ImVec2(entity.positionScreen.x + height / 3, entity.positionScreen.y);

    if (Vars::Esp::baseBox)
    {
        drawList->AddRectFilled(ImVec2(start.x + 2, start.y + 2), ImVec2(end.x + 2, end.y + 2), ImGui::ColorConvertFloat4ToU32(ImVec4(0.045, 0.045, 0.045, 0.1)), 2);
        drawList->AddRectFilled(start, end, ImGui::ColorConvertFloat4ToU32(ImVec4(0.045, 0.045, 0.045, 0.015)), 2);

        drawList->AddRect(start, end, ImGui::ColorConvertFloat4ToU32(ImVec4(0.045, 0.045, 0.045, 0.35)), 1);
    }

    if (Vars::Esp::baseGradient)
        drawList->AddRectFilledMultiColor(start, end, ImGui::ColorConvertFloat4ToU32({ 0.045, 0.045, 0.045, 0.2 }), ImGui::ColorConvertFloat4ToU32({ 0.045, 0.045, 0.045, 0.2 }), ImGui::ColorConvertFloat4ToU32({ 0.045, 0.045, 0.045, 0.1 }), 0);

    if (Vars::Esp::baseCorner)
    {
        float cornerDistance = fminf(3, entity.distance / 1512);
        float cornerSize = fminf(12, fmaxf(6, sqrt(pow(end.x - start.x, 2) + pow(end.y - start.y, 2)) / 12 * cornerDistance));

        ImVec4 cornerColor = ImVec4(color.x, color.y, color.z, color.w * (1 - cornerDistance));

        drawList->AddLine(start, ImVec2(start.x + cornerSize, start.y), ImGui::ColorConvertFloat4ToU32(cornerColor), 2);
        drawList->AddLine(start, ImVec2(start.x, start.y + cornerSize), ImGui::ColorConvertFloat4ToU32(cornerColor), 2);

        drawList->AddLine(ImVec2(start.x, end.y), ImVec2(start.x, end.y - cornerSize), ImGui::ColorConvertFloat4ToU32(cornerColor), 2);
        drawList->AddLine(ImVec2(start.x, end.y), ImVec2(start.x + cornerSize, end.y), ImGui::ColorConvertFloat4ToU32(cornerColor), 2);

        drawList->AddLine(end, ImVec2(end.x, end.y - cornerSize), ImGui::ColorConvertFloat4ToU32(cornerColor), 2);
        drawList->AddLine(end, ImVec2(end.x - cornerSize, end.y), ImGui::ColorConvertFloat4ToU32(cornerColor), 2);

        drawList->AddLine(ImVec2(end.x, start.y), ImVec2(end.x, start.y + cornerSize), ImGui::ColorConvertFloat4ToU32(cornerColor), 2);
        drawList->AddLine(ImVec2(end.x, start.y), ImVec2(end.x - cornerSize, start.y), ImGui::ColorConvertFloat4ToU32(cornerColor), 2);
    }
}

void DrawBones(Entity entity, ImDrawList* drawList)
{
    ImVec4 color = (localPlayer.team == entity.team) ? ImVec4(Vars::Esp::team.x, Vars::Esp::team.y, Vars::Esp::team.z, Vars::Esp::team.w) : ImVec4(Vars::Esp::enemy.x, Vars::Esp::enemy.y, Vars::Esp::enemy.z, Vars::Esp::enemy.w);
    if (!entity.spotted) color.w *= 0.75;

    if (Vars::Esp::bonesTrace)
    {
        auto AddBoneLine = [&](Bones from, Bones to) {
            if (static_cast<uint8_t>(from) < entity.bonesScreen.size() && static_cast<uint8_t>(to) < entity.bonesScreen.size()) {
                drawList->AddLine(ImVec2(entity.bonesScreen[static_cast<uint8_t>(from)].x, entity.bonesScreen[static_cast<uint8_t>(from)].y),
                    ImVec2(entity.bonesScreen[static_cast<uint8_t>(to)].x, entity.bonesScreen[static_cast<uint8_t>(to)].y),
                    ImColor(color), 2);
            }
            };

        AddBoneLine(Bones::Waist, Bones::Neck);
        AddBoneLine(Bones::Neck, Bones::Head);

        AddBoneLine(Bones::Neck, Bones::ShoulderLeft);
        AddBoneLine(Bones::ShoulderLeft, Bones::ForeLeft);
        AddBoneLine(Bones::ForeLeft, Bones::HandLeft);

        AddBoneLine(Bones::Neck, Bones::ShoulderRight);
        AddBoneLine(Bones::ShoulderRight, Bones::ForeRight);
        AddBoneLine(Bones::ForeRight, Bones::HandRight);

        AddBoneLine(Bones::Waist, Bones::KneeLeft);
        AddBoneLine(Bones::KneeLeft, Bones::FeetLeft);

        AddBoneLine(Bones::Waist, Bones::KneeRight);
        AddBoneLine(Bones::KneeRight, Bones::FeetRight);
    }

    if (Vars::Esp::bonesDots)
    {
        float distanceFactor = fminf(1.85, entity.distance / 8);

        auto AddBoneDot = [&](Bones bone) {
            if (static_cast<uint8_t>(bone) < entity.bonesScreen.size()) {
                drawList->AddCircleFilled(ImVec2(entity.bonesScreen[static_cast<uint8_t>(bone)].x, entity.bonesScreen[static_cast<uint8_t>(bone)].y),
                    distanceFactor, ImColor(color), 9);
            }
            };

        AddBoneDot(Bones::Waist);
        AddBoneDot(Bones::Neck);

        if (static_cast<uint8_t>(Bones::Head) < entity.bonesScreen.size())
            drawList->AddCircleFilled(ImVec2(entity.bonesScreen[static_cast<uint8_t>(Bones::Head)].x, entity.bonesScreen[static_cast<uint8_t>(Bones::Head)].y), distanceFactor * 1.75, ImColor(color), 18);

        AddBoneDot(Bones::ShoulderLeft);
        AddBoneDot(Bones::ForeLeft);
        AddBoneDot(Bones::HandLeft);

        AddBoneDot(Bones::ShoulderRight);
        AddBoneDot(Bones::ForeRight);
        AddBoneDot(Bones::HandRight);

        AddBoneDot(Bones::KneeLeft);
        AddBoneDot(Bones::FeetLeft);

        AddBoneDot(Bones::KneeRight);
        AddBoneDot(Bones::FeetRight);
    }
}

void DrawMisc(Entity entity, ImDrawList* drawList)
{
    ImVec4 color = (localPlayer.team == entity.team) ? ImVec4(Vars::Esp::team.x, Vars::Esp::team.y, Vars::Esp::team.z, Vars::Esp::team.w) : ImVec4(Vars::Esp::enemy.x, Vars::Esp::enemy.y, Vars::Esp::enemy.z, Vars::Esp::enemy.w);
    if (!entity.spotted) color.w *= 0.75;

    float height = entity.positionScreen.y - entity.heightScreen.y;

    ImVec2 start = ImVec2(screen.x / 2, screen.y);
    ImVec2 end = ImVec2(entity.positionScreen.x, entity.positionScreen.y);

    if (Vars::Esp::miscTrace)
    {
        drawList->AddLine(start, end, ImGui::ColorConvertFloat4ToU32(color), 1.25);
        drawList->AddCircleFilled(end, 2, ImGui::ColorConvertFloat4ToU32(color), 12);
    }

    float boxLeft = entity.heightScreen.x - height / 3;
    float boxRight = entity.positionScreen.x + height / 3;

    float distanceFactor = fminf(3, entity.distance / 1512);
    float barWidth = 0.02 * (boxRight - boxLeft) * (1 + distanceFactor * 5);
    float barHeight = height * (entity.health / 100.0);

    ImVec2 barTopLeft = ImVec2(boxLeft - barWidth - 1, entity.positionScreen.y - barHeight);
    ImVec2 barBottomRight = ImVec2(boxLeft - 1, entity.positionScreen.y);

    std::string healthText = "Hp " + std::to_string(entity.health);
    ImVec2 healthSize = ImGui::CalcTextSize(healthText.c_str());

    float distanceTextFactor = fmaxf(fminf(1 / (1 + distanceFactor * 0.5), 4), 0.75);

    ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
    ImGui::SetWindowFontScale(distanceTextFactor);

    ImVec2 scaledHealthSize = ImVec2(healthSize.x * distanceTextFactor, healthSize.y * distanceTextFactor);
    ImVec2 healthPosition = ImVec2(barTopLeft.x - scaledHealthSize.x * distanceTextFactor * 1.5, barTopLeft.y);

    if (Vars::Esp::miscHealth)
    {
        drawList->AddRectFilled(barTopLeft, barBottomRight, ImGui::ColorConvertFloat4ToU32(color), 8);
        drawList->AddText(healthPosition, ImGui::ColorConvertFloat4ToU32(color), healthText.c_str());
    }

    std::string name = entity.name;
    ImVec2 nameSize = ImGui::CalcTextSize(name.c_str());
    ImVec2 scaledNameSize = ImVec2(nameSize.x * distanceTextFactor, nameSize.y * distanceTextFactor);
    ImVec2 namePosition = ImVec2(entity.positionScreen.x - scaledNameSize.x / 2, entity.positionScreen.y - height - scaledNameSize.y);

    if (Vars::Esp::miscName)
        drawList->AddText(namePosition, ImGui::ColorConvertFloat4ToU32(color), name.c_str());

    std::string weaponText = "Weapon " + entity.weapon;
    ImVec2 weaponSize = ImGui::CalcTextSize(weaponText.c_str());
    ImVec2 scaledWeaponSize = ImVec2(weaponSize.x * distanceTextFactor, weaponSize.y * distanceTextFactor);
    ImVec2 weaponPosition = ImVec2(boxLeft + (boxRight - boxLeft - scaledWeaponSize.x) / 2, entity.positionScreen.y);

    if (Vars::Esp::miscWeapon)
        drawList->AddText(weaponPosition, ImGui::ColorConvertFloat4ToU32(color), weaponText.c_str());

    ImGui::PopFont();
}

void DrawList()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowPos({ 0, 0 }, ImGuiCond_Once);
    ImGui::SetNextWindowSize(io.DisplaySize, ImGuiCond_Once);
    ImGui::Begin("DrawList", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMouseInputs | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing);
    {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        ImGui::SetCursorPos({ 8, 6 });
        ImGui::SetNextWindowSize({ 270, 21 });
        ImGui::PushFont(medium);
        {
            char buffer[64];

            time_t current;
            time(&current);

            tm time;
            localtime_s(&time, &current);

            strftime(buffer, 64, "%d.%m.%Y %H:%M:%S", &time);

            ImGui::TextColored({ 0.045, 0.045, 0.045, 0.9 }, "Wware %s", buffer);
        }
        ImGui::PopFont();

        for (Entity& entity : entities)
        {
            if (Vars::Esp::drawBase) DrawBase(entity, drawList);

            if (Vars::Esp::drawMisc) DrawMisc(entity, drawList);

            if (Vars::Esp::drawBones) DrawBones(entity, drawList);
        }
    }
    ImGui::End();
}

void DrawMenu()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui::SetNextWindowSize({ 835, 480 }, ImGuiCond_Once);
    ImGui::SetNextWindowPos({ 64, 32 }, ImGuiCond_Once);
    ImGui::PushFont(medium);
    {
        ImGui::Begin("DrawMenu", NULL, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings);
        {
            ImDrawList* drawList = ImGui::GetWindowDrawList();

            drawList->AddRectFilledMultiColor(ImGui::GetWindowPos(), ImVec2(ImGui::GetWindowPos().x + ImGui::GetWindowSize().x, ImGui::GetWindowPos().y + ImGui::GetWindowSize().y), ImGui::ColorConvertFloat4ToU32({ 0.045, 0.045, 0.045, 0.05 }), ImGui::ColorConvertFloat4ToU32({ mainColor[0], mainColor[1], mainColor[2], 0.35 }), ImGui::ColorConvertFloat4ToU32({ 0.045, 0.045, 0.045, 0.05 }), ImGui::ColorConvertFloat4ToU32({ mainColor[0], mainColor[1], mainColor[2], 0.35 }));

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(6, 0));
            {
                ImGui::SetCursorPos({ 15, 15 });
                ImGui::BeginChild("FollowUp", { 600, 40 }, true);
                {
                    ImGui::PushFont(large);
                    {
                        ImGui::SetCursorPos({ 12, 12 });
                        ImGui::TextColored({ mainColor[0], mainColor[1], mainColor[2], 1 }, "Wware Counter Strike 2");
                    }
                    ImGui::PopFont();
                }
                ImGui::EndChild();

                ImGui::SetCursorPos({ 620, 15 });
                ImGui::BeginChild("PreviewUp", { 200, 40 }, true);
                {
                    ImGui::PushFont(large);
                    {
                        ImGui::SetCursorPos({ 12, 12 });
                        ImGui::TextColored({ mainColor[0], mainColor[1], mainColor[2], 1 }, "Preview");
                    }
                    ImGui::PopFont();
                }
                ImGui::EndChild();

                ImGui::SetCursorPos({ 620, 60 });
                ImGui::BeginChild("PreviewDown", { 200, 405 }, true);
                {
                    //
                }
                ImGui::EndChild();

                ImGui::SetCursorPos({ 15, 60 });
                ImGui::BeginChild("BaseSetting", { 654 / 3.35, 405 }, true);
                {
                    ImGui::PushFont(large);
                    {
                        ImGui::SetCursorPos({ 12, 12 });
                        ImGui::TextColored({ mainColor[0], mainColor[1], mainColor[2], 1 }, "Base settings");
                    }
                    ImGui::PopFont();

                    ImGui::Separator();
                    ImGui::Checkbox("Enable", &Vars::Esp::drawBase);
                    ImGui::Separator();

                    ImGui::Checkbox("Box", &Vars::Esp::baseBox);
                    ImGui::Checkbox("Gradient", &Vars::Esp::baseGradient);
                    ImGui::Checkbox("Corner", &Vars::Esp::baseCorner);
                }
                ImGui::EndChild();

                ImGui::SetCursorPos({ 218, 60 });
                ImGui::BeginChild("BonesSetting", { 654 / 3.35, 405 }, true);
                {
                    ImGui::PushFont(large);
                    {
                        ImGui::SetCursorPos({ 12, 12 });
                        ImGui::TextColored({ mainColor[0], mainColor[1], mainColor[2], 1 }, "Bones settings");
                    }
                    ImGui::PopFont();

                    ImGui::Separator();
                    ImGui::Checkbox("Enable", &Vars::Esp::drawBones);
                    ImGui::Separator();

                    ImGui::Checkbox("Trace", &Vars::Esp::bonesTrace);
                    ImGui::Checkbox("Dots", &Vars::Esp::bonesDots);
                }
                ImGui::EndChild();

                ImGui::SetCursorPos({ 418, 60 });
                ImGui::BeginChild("MiscSetting", { 654 / 3.35, 405 / 2 }, true);
                {
                    ImGui::PushFont(large);
                    {
                        ImGui::SetCursorPos({ 12, 12 });
                        ImGui::TextColored({ mainColor[0], mainColor[1], mainColor[2], 1 }, "Misc settings");
                    }
                    ImGui::PopFont();

                    ImGui::Separator();
                    ImGui::Checkbox("Enable", &Vars::Esp::drawMisc);
                    ImGui::Separator();

                    ImGui::Checkbox("Trace", &Vars::Esp::miscTrace);
                    ImGui::Checkbox("Health", &Vars::Esp::miscHealth);
                    ImGui::Checkbox("Name", &Vars::Esp::miscName);
                    ImGui::Checkbox("Weapon", &Vars::Esp::miscWeapon);
                }
                ImGui::EndChild();

                ImGui::SetCursorPos({ 418, 264 });
                ImGui::BeginChild("StyleSetting", { 654 / 3.35, 405 / 2 }, true);
                {
                    ImGui::PushFont(large);
                    {
                        ImGui::SetCursorPos({ 12, 12 });
                        ImGui::TextColored({ mainColor[0], mainColor[1], mainColor[2], 1 }, "Style settings");
                    }
                    ImGui::PopFont();

                    ImGui::Separator();

                    ImGui::ColorEdit4("Team", reinterpret_cast<float*>(&Vars::Esp::team), ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoOptions);
                    ImGui::ColorEdit4("Enemy", reinterpret_cast<float*>(&Vars::Esp::enemy), ImGuiColorEditFlags_PickerHueWheel | ImGuiColorEditFlags_AlphaPreview | ImGuiColorEditFlags_AlphaBar | ImGuiColorEditFlags_NoDragDrop | ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoTooltip | ImGuiColorEditFlags_NoOptions);
                }
                ImGui::EndChild();
            }
            ImGui::PopStyleVar();
        }
        ImGui::End();
    }
    ImGui::PopFont();
}

void WindowClamp()
{
    ImGuiIO& io = ImGui::GetIO();

    ImVec2 min = ImVec2(0, 0);
    ImVec2 max = ImVec2(io.DisplaySize.x, io.DisplaySize.y);

    ImVector<ImGuiWindow*>& windows = ImGui::GetCurrentContext()->Windows;

    for (uint16_t i = 0; i < windows.Size; i++)
    {
        ImGuiWindow* window = windows[i];

        window->Pos = ImVec2(ImClamp(window->Pos.x, min.x, max.x - window->Size.x), ImClamp(window->Pos.y, min.y, max.y - window->Size.y));
    }
}