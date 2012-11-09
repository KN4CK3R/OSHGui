/*
 * OldSchoolHack GUI
 *
 * Copyright (c) 2012 KN4CK3R http://www.oldschoolhack.de
 *
 * See license in OSHGui.hpp
 */

#include <iostream>
#include "Exceptions.hpp"

namespace OSHGui
{
	namespace Misc
	{
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		Exception::Exception(const Misc::AnsiString &message)
			: message(message),
			  line(0)
		{
			std::cerr << what() << std::endl;
		}
		//---------------------------------------------------------------------------
		Exception::Exception(const Misc::AnsiString &message, const Misc::AnsiString &file, int line)
			: message(message),
			  file(file),
			  line(line)
		{
			std::cerr << what() << std::endl;
		}
		//---------------------------------------------------------------------------
		Exception::~Exception()
		{
	
		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		const Misc::AnsiString& Exception::GetMessage() const
		{
			return message;
		}
		//---------------------------------------------------------------------------
		const Misc::AnsiString& Exception::GetFileName() const
		{
			return file;
		}
		//---------------------------------------------------------------------------
		int Exception::GetLine() const
		{
			return line;
		}
		//---------------------------------------------------------------------------
		const char* Exception::what() const throw()
		{
			return message.c_str();
		}
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		ArgumentException::ArgumentException(const Misc::AnsiString &paramName)
			: paramName(paramName),
			  Exception("Unexpected argument.\r\nParameter name: " + paramName)
		{

		}
		//----------------------------------------------------------------------------
		ArgumentException::ArgumentException(const Misc::AnsiString &paramName, const Misc::AnsiString &message)
			: paramName(paramName),
			  Exception(message)
		{

		}
		//---------------------------------------------------------------------------
		ArgumentException::ArgumentException(const Misc::AnsiString &paramName, const Misc::AnsiString &file, int line)
			: paramName(paramName),
			  Exception("Unexpected argument.\r\nParameter name: " + paramName, file, line)
		{

		}
		//---------------------------------------------------------------------------
		ArgumentException::ArgumentException(const Misc::AnsiString &paramName, const Misc::AnsiString &message, const Misc::AnsiString &file, int line)
			: paramName(paramName),
			  Exception(message, file, line)
		{

		}
		//---------------------------------------------------------------------------
		//Getter/Setter
		//---------------------------------------------------------------------------
		const Misc::AnsiString& ArgumentException::GetParamName() const
		{
			return paramName;
		}
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		ArgumentNullException::ArgumentNullException(const Misc::AnsiString &paramName)
			: ArgumentException(paramName, "Value cannot be null.")
		{

		}
		//---------------------------------------------------------------------------
		ArgumentNullException::ArgumentNullException(const Misc::AnsiString &paramName, const Misc::AnsiString &message)
			: ArgumentException(paramName, message)
		{

		}
		//---------------------------------------------------------------------------
		ArgumentNullException::ArgumentNullException(const Misc::AnsiString &paramName, const Misc::AnsiString &file, int line)
			: ArgumentException(paramName, "Value cannot be null.", file, line)
		{

		}
		//---------------------------------------------------------------------------
		ArgumentNullException::ArgumentNullException(const Misc::AnsiString &paramName, const Misc::AnsiString &message, const Misc::AnsiString &file, int line)
			: ArgumentException(paramName, message, file, line)
		{

		}
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		ArgumentOutOfRangeException::ArgumentOutOfRangeException(const Misc::AnsiString &paramName)
			: ArgumentException(paramName, "Value was out of valid range.")
		{

		}
		//---------------------------------------------------------------------------
		ArgumentOutOfRangeException::ArgumentOutOfRangeException(const Misc::AnsiString &paramName, const Misc::AnsiString &message)
			: ArgumentException(paramName, message)
		{

		}
		//---------------------------------------------------------------------------
		ArgumentOutOfRangeException::ArgumentOutOfRangeException(const Misc::AnsiString &paramName, const Misc::AnsiString &file, int line)
			: ArgumentException(paramName, "Value was out of valid range.", file, line)
		{

		}
		//---------------------------------------------------------------------------
		ArgumentOutOfRangeException::ArgumentOutOfRangeException(const Misc::AnsiString &paramName, const Misc::AnsiString &message, const Misc::AnsiString &file, int line)
			: ArgumentException(paramName, message, file, line)
		{

		}
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		InvalidOperationException::InvalidOperationException(const Misc::AnsiString &message)
			: Exception(message)
		{

		}
		//---------------------------------------------------------------------------
		InvalidOperationException::InvalidOperationException(const Misc::AnsiString &message, const Misc::AnsiString &file, int line)
			: Exception(message, file, line)
		{

		}
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		InvalidThemeException::InvalidThemeException(const Misc::AnsiString &message)
			: Exception(message)
		{

		}
		//---------------------------------------------------------------------------
		InvalidThemeException::InvalidThemeException(const Misc::AnsiString &message, const Misc::AnsiString &file, int line)
			: Exception(message, file, line)
		{

		}
		//---------------------------------------------------------------------------
	}
}