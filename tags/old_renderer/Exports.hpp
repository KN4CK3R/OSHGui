/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2010-2013 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#ifndef OSHGUI_EXPORTS_HPP
#define OSHGUI_EXPORTS_HPP

#ifdef OSHGUI_COMPILE_DLL
	#ifdef __GNUC__
		#define OSHGUI_EXPORT __attribute__((dllexport))
	#else
		#define OSHGUI_EXPORT __declspec(dllexport)
		#pragma warning(disable : 4251)
	#endif
#elif defined OSHGUI_COMPILE_DLL_IMPORT
	#ifdef __GNUC__
		#define OSHGUI_EXPORT __attribute__((dllimport))
	#else
		#define OSHGUI_EXPORT __declspec(dllimport)
		#pragma warning(disable : 4251)
	#endif
#else
	#define OSHGUI_EXPORT
#endif

#endif