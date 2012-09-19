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
		Exception::Exception(const Misc::AnsiString &message) :
					  message(message),
					  file(""),
					  line(0)
		{
			name = "OSHGui::Exception";
			combinedMessage = name + " :\r\n" + this->message;

			std::cerr << what() << std::endl;
		}
		//---------------------------------------------------------------------------
		Exception::Exception(const Misc::AnsiString &message,
					  const Misc::AnsiString &file,
					  int line) :
					  message(message),
					  file(file),
					  line(line)
		{
			name = "OSHGui::Exception";
			combinedMessage = name + " in file " + file  + "(" + Misc::String::Format("%u", line) + ") :\r\n" + message;

			std::cerr << what() << std::endl;
		}
		//---------------------------------------------------------------------------
		Exception::Exception(const Misc::AnsiString &name,
					  const Misc::AnsiString &message) :
					  name(name),
					  message(message),
					  file(""),
					  line(0)
		{
			combinedMessage = name + " : " + this->message;

			std::cerr << what() << std::endl;
		}
		//---------------------------------------------------------------------------
		Exception::Exception(const Misc::AnsiString &name,
					  const Misc::AnsiString &message,
					  const Misc::AnsiString &file,
					  int line) :
					  name(name),
					  message(message),
					  file(file),
					  line(line)
		{
			combinedMessage = name + " in file " + file  + "(" + Misc::String::Format("%u", line) + ") : " + message;

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
		const Misc::AnsiString& Exception::GetName() const
		{
			return name;
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
		const Misc::AnsiString& Exception::ToString() const
		{
			return combinedMessage;
		}
		//---------------------------------------------------------------------------
		const char* Exception::what() const throw()
		{
			return combinedMessage.c_str();
		}
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		ArgumentException::ArgumentException(const Misc::AnsiString &paramName) :
							  paramName(paramName),
						   Exception("OSHGui::ArgumentException", "Unexpected argument.\r\nParameter name: " + paramName)
		{

		}
		//----------------------------------------------------------------------------
		ArgumentException::ArgumentException(const Misc::AnsiString &paramName,
							  const Misc::AnsiString &message) :
							  paramName(paramName),
						   Exception("OSHGui::ArgumentException", message + "\r\nParameter name: " + paramName)
		{

		}
		//---------------------------------------------------------------------------
		ArgumentException::ArgumentException(const Misc::AnsiString &paramName,
							  const Misc::AnsiString &file,
							  int line) :
							  paramName(paramName),
						   Exception("OSHGui::ArgumentException", "Unexpected argument.\r\nParameter name: " + paramName, file, line)
		{

		}
		//---------------------------------------------------------------------------
		ArgumentException::ArgumentException(const Misc::AnsiString &paramName,
							  const Misc::AnsiString &message,
							  const Misc::AnsiString &file,
							  int line) :
							  paramName(paramName),
						   Exception("OSHGui::ArgumentException", message + "\r\nParameter name: " + paramName, file, line)
		{

		}
		//---------------------------------------------------------------------------
		ArgumentException::ArgumentException(const Misc::AnsiString &name,
							  const Misc::AnsiString &paramName,
							  const Misc::AnsiString &message) :
							  paramName(paramName),
						   Exception(name, message + "\r\nParameter name: " + paramName)
		{

		}
		//---------------------------------------------------------------------------
		ArgumentException::ArgumentException(const Misc::AnsiString &name,
							  const Misc::AnsiString &paramName,
							  const Misc::AnsiString &message,
							  const Misc::AnsiString &file,
							  int line) :
							  paramName(paramName),
						   Exception(name, message + "\r\nParameter name: " + paramName, file, line)
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
		ArgumentNullException::ArgumentNullException(const Misc::AnsiString &paramName) :
							   ArgumentException("OSHGui::ArgumentNullException", paramName, "Value cannot be null.")
		{

		}
		//---------------------------------------------------------------------------
		ArgumentNullException::ArgumentNullException(const Misc::AnsiString &paramName,
													 const Misc::AnsiString &message) :
							   ArgumentException("OSHGui::ArgumentNullException", paramName, message)
		{

		}
		//---------------------------------------------------------------------------
		ArgumentNullException::ArgumentNullException(const Misc::AnsiString &paramName,
													 const Misc::AnsiString &message,
													 const Misc::AnsiString &file,
													 int line) :
							   ArgumentException("OSHGui::ArgumentNullException", paramName, message, file, line)
		{

		}
		//---------------------------------------------------------------------------
		ArgumentNullException::ArgumentNullException(const Misc::AnsiString &paramName,
													 const Misc::AnsiString &file,
													 int line) :
							   ArgumentException("OSHGui::ArgumentNullException", paramName, "Value cannot be null.", file, line)
		{

		}
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		ArgumentOutOfRangeException::ArgumentOutOfRangeException(const Misc::AnsiString &paramName) :
									 ArgumentException("OSHGui::ArgumentOutOfRangeException", paramName, "Value was out of valid range.")
		{

		}
		//---------------------------------------------------------------------------
		ArgumentOutOfRangeException::ArgumentOutOfRangeException(const Misc::AnsiString &paramName,
																 const Misc::AnsiString &message) :
									 ArgumentException("OSHGui::ArgumentOutOfRangeException", paramName, message)
		{

		}
		//---------------------------------------------------------------------------
		ArgumentOutOfRangeException::ArgumentOutOfRangeException(const Misc::AnsiString &paramName,
																 const Misc::AnsiString &file,
																 int line) :
									 ArgumentException("OSHGui::ArgumentOutOfRangeException", paramName, "Value was out of valid range.", file, line)
		{

		}
		//---------------------------------------------------------------------------
		ArgumentOutOfRangeException::ArgumentOutOfRangeException(const Misc::AnsiString &paramName,
																 const Misc::AnsiString &message,
																 const Misc::AnsiString &file,
																 int line) :
									 ArgumentException("OSHGui::ArgumentOutOfRangeException", paramName, message, file, line)
		{

		}
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		InvalidOperationException::InvalidOperationException(const Misc::AnsiString &message) :
								   Exception("OSHGui::InvalidOperationException", message)
		{

		}
		//---------------------------------------------------------------------------
		InvalidOperationException::InvalidOperationException(const Misc::AnsiString &message,
															 const Misc::AnsiString &file,
															 int line) :
								   Exception("OSHGui::InvalidOperationException", message, file, line)
		{

		}
		//---------------------------------------------------------------------------
		//Constructor
		//---------------------------------------------------------------------------
		InvalidThemeException::InvalidThemeException(const Misc::AnsiString &message) :
							   Exception("OSHGui::InvalidThemeException", message)
		{

		}
		//---------------------------------------------------------------------------
		InvalidThemeException::InvalidThemeException(const Misc::AnsiString &message,
													 const Misc::AnsiString &file,
													 int line) :
							   Exception("OSHGui::InvalidThemeException", message, file, line)
		{

		}
		//---------------------------------------------------------------------------
	}
}