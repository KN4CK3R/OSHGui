/*
 * OldSchoolHack GUI
 *
 * by KN4CK3R http://www.oldschoolhack.me
 *
 * See license in OSHGui.hpp
 */

#include "FontManager.hpp"
#include "../Misc/Exceptions.hpp"
#include "Drawing/FreeTypeFont.hpp"
#include <windows.h>
#include <algorithm>
#include <vector>
#include <sstream>

namespace OSHGui
{
	namespace Drawing
	{
		FontPtr FontManager::LoadFont(Misc::AnsiString name, float pointSize, bool antiAliased)
		{
			if (name.empty())
			{
				throw Misc::ArgumentException();
			}

			std::transform(name.begin(), name.end(), name.begin(), ::tolower);
			std::vector<std::string> parts;
			std::stringstream ss(name);
			for (std::string s; ss >> s; parts.push_back(s));

			FontPtr font = nullptr;

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

					lastError = RegEnumValueA(fontKey, i, const_cast<LPSTR>(valueName.data()), &valueNameLength, nullptr, nullptr, const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(valueData.data())), &valueDataLength);

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

							return LoadFontFromFile(path, pointSize, antiAliased);
						}
					}

					++i;
				} while (lastError != ERROR_NO_MORE_ITEMS);
			}

			RegCloseKey(fontKey);

			throw Misc::ArgumentException();
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
}
