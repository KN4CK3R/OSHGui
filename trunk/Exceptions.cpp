#include <iostream>
#include "Exceptions.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Exception::Exception(const Misc::UnicodeString &message,
				  const Misc::UnicodeString &name,
				  const Misc::UnicodeString &file,
				  int line) :
				  message(message),
				  name(name),
				  file(file),
				  line(line),
				  combinedMessage(name + " in file " + file  + "(" + line + ") :\r\n" + message)
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
	const Misc::UnicodeString& Exception::GetMessage() const
	{
		return message;
	}
	//---------------------------------------------------------------------------
	const Misc::UnicodeString& Exception::GetName() const
	{
		return name;
	}
	//---------------------------------------------------------------------------
	const Misc::UnicodeString& Exception::GetFileName() const
	{
		return fileName;
	}
	//---------------------------------------------------------------------------
	int Exception::GetLine() const
	{
		return line;
	}
	//---------------------------------------------------------------------------
	const Misc::UnicodeString& Exception::ToString() const
	{
		return combinedMessage;
	}
	//---------------------------------------------------------------------------
	const char* Exception::what() const throw()
	{
		return Misc::String::UnicodeToAnsi(combinedMessage).c_str();
	}
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ArgumentException::ArgumentException(const Misc::UnicodeString &message,
						  const Misc::UnicodeString &paramName,
						  const Misc::UnicodeString &file,
						  int line) :
						  paramName(paramName),
						  Exception(message + "\r\nParameter name: " + paramName, L"OSHGui::ArgumentException", file, line)
	{
	
	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	const Misc::UnicodeString& Exception::GetParamName() const
	{
		return paramName;
	}
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ArgumentNullException::ArgumentNullException(const Misc::UnicodeString &message,
												 const Misc::UnicodeString &paramName,
												 const Misc::UnicodeString &file,
												 int line) :
												 ArgumentException(message, paramName, L"OSHGui::ArgumentNullException", file, line)
	{
	
	}
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ArgumentOutOfRangeException::ArgumentOutOfRangeException(const Misc::UnicodeString &message,
															 const Misc::UnicodeString &paramName,
															 const Misc::UnicodeString &file,
															 int line) :
															 ArgumentException(message, paramName, L"OSHGui::ArgumentOutOfRangeException", file, line)
	{
	
	}
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	InvalidOperationException::InvalidOperationException(const Misc::UnicodeString &message,
														 const Misc::UnicodeString &file,
														 int line) :
														 Exception(message, L"OSHGui::InvalidOperationException", file, line)
	{
	
	}
	//---------------------------------------------------------------------------
}