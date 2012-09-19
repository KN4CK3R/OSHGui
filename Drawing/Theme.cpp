#include "Theme.hpp"
#include <json/json.h>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <iterator>
using namespace std;

namespace OSHGui
{
	namespace Drawing
	{
		void Theme::SetControlColorTheme(const Misc::AnsiString &controlClass, const Theme::ControlTheme &controlTheme)
		{
			controlThemes[controlClass] = controlTheme;
		}
		//---------------------------------------------------------------------------
		const Theme::ControlTheme& Theme::GetControlColorTheme(const Misc::AnsiString &controlClass) const
		{
			auto it = controlThemes.find(controlClass);
			if (it == controlThemes.end())
			{
				return DefaultColor;
			}

			return it->second;
		}
		//---------------------------------------------------------------------------
		void Theme::Load(const Misc::AnsiString pathToThemeFile)
		{
			Name = string();
			Author = string();
			controlThemes.clear();
			DefaultColor.ForeColor = Color();
			DefaultColor.BackColor = Color();

			ifstream file(pathToThemeFile);
			if (!file.bad())
			{
				stringstream ss;
				ss << file.rdbuf();

				Json::Value root;
				Json::Reader reader;
				if (reader.parse(ss.str(), root) == false)
				{
					throw "can't parse file";
				}

				Name = root.get("name", "").asString();
				Author = root.get("author", "").asString();

				auto JsonToColor = [](const Json::Value &value) -> Color
				{
					if (value.size() == 4)
					{
						if (value.isArray())
						{
							if (value[0].isInt() && value[1].isInt()
							 && value[2].isInt() && value[3].isInt())
							{
								return Color(value[0].asInt(), value[1].asInt(),
											 value[2].asInt(), value[3].asInt());
							}
						}
						else if (value.isObject() && value.size() == 4)
						{
							if (value["a"].isInt() && value["r"].isInt()
							 && value["g"].isInt() && value["b"].isInt())
							{
								return Color(value["a"].asInt(), value["r"].asInt(),
											 value["g"].asInt(), value["b"].asInt());
							}
						}
					}
					else if (value.isString())
					{
						stringstream ss(value.asString());
						int argb;
						ss >> hex >> argb;
						if (ss.good())
						{
							return Color(argb);
						}
					}
					throw "invalid color";
				};

				if (!root["default"].isObject())
				{
					throw "'default' missing";
				}
				auto &defaultColor = root["default"];
				DefaultColor.ForeColor = JsonToColor(defaultColor["forecolor"]);
				DefaultColor.BackColor = JsonToColor(defaultColor["backcolor"]);

				if (root["themes"].isObject())
				{
					throw "themes missing";
				}
				auto &themes = root["themes"];
				auto member = themes.getMemberNames();
				for (auto it = member.begin(); it != member.end(); ++it)
				{
					ControlTheme theme;
					theme.ForeColor = JsonToColor(themes[*it]["forecolor"]);
					theme.BackColor = JsonToColor(themes[*it]["backcolor"]);
					SetControlColorTheme(*it, theme);
				}
			}
			else
			{
				throw "can't open file";
			}
		}
		//---------------------------------------------------------------------------
		bool Theme::Save(const Misc::AnsiString pathToThemeFile, ColorStyle style) const
		{
			ofstream file(pathToThemeFile);
			if (!file.bad())
			{
				Json::Value root;
				root["name"] = Name;
				root["author"] = Author;

				auto ColorToJson = [](Json::Value &value, const Color &color, ColorStyle style)
				{
					switch (style)
					{
						case Theme::Text:
							value["a"] = color.A;
							value["r"] = color.R;
							value["g"] = color.G;
							value["b"] = color.B;
							break;
						case Theme::Array:
							value.append(color.A);
							value.append(color.R);
							value.append(color.G);
							value.append(color.B);
							break;
						case Theme::Integer:
							stringstream ss;
							ss << setfill('0') << setw(8) << std::hex << color.ARGB;
							value = ss.str();
							break;
					}
				};

				auto &defaultColor = root["default"];
				ColorToJson(defaultColor["forecolor"], DefaultColor.ForeColor, style);
				ColorToJson(defaultColor["backcolor"], DefaultColor.BackColor, style);

				auto &themes = root["themes"];
				for (auto it = controlThemes.begin(); it != controlThemes.end(); ++it)
				{
					Json::Value controlClass;

					ColorToJson(controlClass["forecolor"], it->second.ForeColor, style);
					ColorToJson(controlClass["backcolor"], it->second.BackColor, style);

					themes[it->first] = controlClass;
				}

				Json::StyledWriter writer;
				file << writer.write(root);

				return file.good();
			}

			return false;
		}
		//---------------------------------------------------------------------------
	}
}