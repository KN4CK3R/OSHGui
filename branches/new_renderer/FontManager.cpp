/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include "FontManager.hpp"
#include "Misc/Exceptions.hpp"
#include "Drawing/FreeTypeFont.hpp"
#include <windows.h>
#include <algorithm>
#include <vector>
#include <sstream>

namespace OSHGui
{
	using namespace Drawing;

	FontPtr FontManager::LoadFont(const Misc::AnsiString &_name, float pointSize, bool antiAliased)
	{
		if (_name.empty())
		{
			return nullptr;
		}

		auto name(_name);
		std::transform(name.begin(), name.end(), name.begin(), ::tolower);
		std::vector<std::string> parts;
		std::stringstream ss(name);
		for (std::string s; ss >> s; parts.push_back(s));

		FontPtr font;

		std::string valueName(MAX_PATH, '\0');
		std::string valueData(MAX_PATH, '\0');

		HKEY fontKey;
		if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts", 0, KEY_READ, &fontKey) == ERROR_SUCCESS)
		{
			int i = 0;
			int lastError = ERROR_SUCCESS;
			do
			{
				DWORD valueNameLength = MAX_PATH;
				DWORD valueDataLength = MAX_PATH;

				lastError = RegEnumValueA(fontKey, i, const_cast<LPSTR>(valueName.data()), &valueNameLength, nullptr, nullptr, const_cast<LPBYTE>(reinterpret_cast<LPCBYTE>(valueData.data())), &valueDataLength);

				if (lastError == ERROR_SUCCESS)
				{
					std::transform(valueName.begin(), valueName.end(), valueName.begin(), ::tolower);

					auto foundAll = true;
					for (auto &part : parts)
					{
						if (valueName.find(part) == std::string::npos)
						{
							foundAll = false;
							break;
						}
					}

					if (foundAll)
					{
						char path[MAX_PATH] = { };
						ExpandEnvironmentStringsA(("%windir%\\fonts\\" + valueData).c_str(), path, MAX_PATH);

						font = LoadFontFromFile(path, pointSize, antiAliased);
						break;
					}
				}

				++i;
			} while (lastError != ERROR_NO_MORE_ITEMS);
		}

		RegCloseKey(fontKey);

		return font;
	}
	//---------------------------------------------------------------------------
	FontPtr FontManager::LoadFontFromFile(const Misc::AnsiString &filename, float pointSize, bool antiAliased)
	{
		return std::make_shared<FreeTypeFont>(filename, pointSize, antiAliased);
	}
	//---------------------------------------------------------------------------
	FontPtr FontManager::LoadFontFromMemory(const Misc::RawDataContainer &data, float pointSize, bool antiAliased)
	{
		return std::make_shared<FreeTypeFont>(data, pointSize, antiAliased);
	}
	//---------------------------------------------------------------------------
}
