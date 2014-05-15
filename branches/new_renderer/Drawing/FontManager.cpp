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
#include "R:/OSHFusion/Utils/XorStr.hpp"
namespace OSHGui
{
	namespace Drawing
	{
		std::unordered_map<Misc::AnsiString, std::weak_ptr<Drawing::Font>> FontManager::loadedFonts;
		//---------------------------------------------------------------------------
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
			if (RegOpenKeyExA(HKEY_LOCAL_MACHINE, /*SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Fonts*/XorStr(50, 0xBF, 0xEC71FB68, 0xEC7BEB7D, 0xEB7BDC57, 0xC15DC25F, 0xC95AF17B, 0xC244CD47, 0xD055856A, 0xF77EE255, 0xED6CF872, 0xEF4CFC6A, 0xE47FFA7A, 0xCF54FE7E, 0xFB7D0000).c_str(), 0, KEY_READ, &fontKey) == ERROR_SUCCESS)
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
							ExpandEnvironmentStringsA((std::string(/*%windir%\\fonts\\*/XorStr(15, 0xEE, 0xCB1A8505, 0x8E009A42, 0xBA038B0D, 0x9612BC00)) + valueData).c_str(), path, MAX_PATH);

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
			auto it = loadedFonts.find(filename);
			if (it == std::end(loadedFonts) || it->second.expired())
			{
				auto font = std::make_shared<FreeTypeFont>(filename, pointSize, antiAliased);
				loadedFonts[filename] = font;
				return font;
			}
			return it->second.lock();
		}
		//---------------------------------------------------------------------------
		FontPtr FontManager::LoadFontFromMemory(const Misc::RawDataContainer &data, float pointSize, bool antiAliased)
		{
			return std::make_shared<FreeTypeFont>(data, pointSize, antiAliased);
		}
		//---------------------------------------------------------------------------
	}
}
