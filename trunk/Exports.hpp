#ifndef OSHGUI_EXPORTS_HPP_
#define OSHGUI_EXPORTS_HPP_

#ifdef OSHGUI_COMPILE_DLL
	#ifdef __GNUC__
		#define OSHGUI_EXPORT __attribute__((dllexport))
	#else
		#define OSHGUI_EXPORT __declspec(dllexport)
		#pragma warning(disable : 4251)
	#endif
#elif defined OSHGUI_COMPILE_STATIC
	#define OSHGUI_EXPORT
#else
	#ifdef __GNUC__
		#define OSHGUI_EXPORT __attribute__((dllimport))
	#else
		#define OSHGUI_EXPORT __declspec(dllimport)
		#pragma warning(disable : 4251)
	#endif
#endif

#endif