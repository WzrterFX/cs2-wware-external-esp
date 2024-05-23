#pragma once

namespace Vars
{
	namespace Menu
	{
		bool open = false;
	}

	namespace Esp
	{
		bool drawBase = true;
		bool baseBox = true;
		bool baseGradient = true;
		bool baseCorner = true;

		bool drawBones = true;
		bool bonesTrace = true;
		bool bonesDots = true;

		bool drawMisc = true;
		bool miscTrace = true;
		bool miscHealth = true;
		bool miscName = true;
		bool miscWeapon = true;

		ImVec4 team = ImVec4(0, 0.74, 0.445, 0.9);
		ImVec4 enemy = ImVec4(0.925, 0, 0.15, 0.9);
	}
}