#include <iostream>
#include "Exceptions.h"

namespace OSHGui
{
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	Exception::Exception(const Misc::UnicodeString &message) :
				  message(message),
				  file(L""),
				  line(0)
	{
		name = L"OSHGui::Exception";
		combinedMessage = name + L" :\r\n" + this->message;

		std::cerr << what() << std::endl;
	}
	//---------------------------------------------------------------------------
	Exception::Exception(const Misc::UnicodeString &message,
				  const Misc::UnicodeString &file,
				  int line) :
				  message(message),
				  file(file),
				  line(line)
	{
		name = L"OSHGui::Exception";
		combinedMessage = name + L" in file " + file  + L"(" + Misc::String::Format(L"%u", line) + L") :\r\n" + message;

		std::cerr << what() << std::endl;
	}
	//---------------------------------------------------------------------------
	Exception::Exception(const Misc::UnicodeString &name,
				  const Misc::UnicodeString &message) :
				  name(name),
				  message(message),
				  file(L""),
				  line(0)
	{
		combinedMessage = name + L" : " + this->message;

		std::cerr << what() << std::endl;
	}
	//---------------------------------------------------------------------------
	Exception::Exception(const Misc::UnicodeString &name,
				  const Misc::UnicodeString &message,
				  const Misc::UnicodeString &file,
				  int line) :
				  name(name),
				  message(message),
				  file(file),
				  line(line)
	{
		combinedMessage = name + L" in file " + file  + L"(" + Misc::String::Format(L"%u", line) + L") : " + message;

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
		return file;
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
	ArgumentException::ArgumentException(const Misc::UnicodeString &paramName) :
						  paramName(paramName),
					   Exception(L"OSHGui::ArgumentException", L"Unexpected argument.\r\nParameter name: " + paramName)
	{

	}
	//----------------------------------------------------------------------------
	ArgumentException::ArgumentException(const Misc::UnicodeString &paramName,
						  const Misc::UnicodeString &message) :
						  paramName(paramName),
					   Exception(L"OSHGui::ArgumentException", message + L"\r\nParameter name: " + paramName)
	{

	}
	//---------------------------------------------------------------------------
	ArgumentException::ArgumentException(const Misc::UnicodeString &paramName,
						  const Misc::UnicodeString &file,
						  int line) :
						  paramName(paramName),
					   Exception(L"OSHGui::ArgumentException", L"Unexpected argument.\r\nParameter name: " + paramName, file, line)
	{

	}
	//---------------------------------------------------------------------------
	ArgumentException::ArgumentException(const Misc::UnicodeString &paramName,
						  const Misc::UnicodeString &message,
						  const Misc::UnicodeString &file,
						  int line) :
						  paramName(paramName),
					   Exception(L"OSHGui::ArgumentException", message + L"\r\nParameter name: " + paramName, file, line)
	{

	}
	//---------------------------------------------------------------------------
	ArgumentException::ArgumentException(const Misc::UnicodeString &name,
						  const Misc::UnicodeString &paramName,
						  const Misc::UnicodeString &message) :
						  paramName(paramName),
					   Exception(name, message + L"\r\nParameter name: " + paramName)
	{

	}
	//---------------------------------------------------------------------------
	ArgumentException::ArgumentException(const Misc::UnicodeString &name,
						  const Misc::UnicodeString &paramName,
						  const Misc::UnicodeString &message,
						  const Misc::UnicodeString &file,
						  int line) :
						  paramName(paramName),
					   Exception(name, message + L"\r\nParameter name: " + paramName, file, line)
	{

	}
	//---------------------------------------------------------------------------
	//Getter/Setter
	//---------------------------------------------------------------------------
	const Misc::UnicodeString& ArgumentException::GetParamName() const
	{
		return paramName;
	}
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ArgumentNullException::ArgumentNullException(const Misc::UnicodeString &paramName) :
						   ArgumentException(L"OSHGui::ArgumentNullException", paramName, L"Value cannot be null.")
	{

	}
	//---------------------------------------------------------------------------
	ArgumentNullException::ArgumentNullException(const Misc::UnicodeString &paramName,
												 const Misc::UnicodeString &message) :
						   ArgumentException(L"OSHGui::ArgumentNullException", paramName, message)
	{

	}
	//---------------------------------------------------------------------------
	ArgumentNullException::ArgumentNullException(const Misc::UnicodeString &paramName,
												 const Misc::UnicodeString &message,
												 const Misc::UnicodeString &file,
												 int line) :
						   ArgumentException(L"OSHGui::ArgumentNullException", paramName, message, file, line)
	{

	}
	//---------------------------------------------------------------------------
	ArgumentNullException::ArgumentNullException(const Misc::UnicodeString &paramName,
												 const Misc::UnicodeString &file,
												 int line) :
						   ArgumentException(L"OSHGui::ArgumentNullException", paramName, L"Value cannot be null.", file, line)
	{

	}
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	ArgumentOutOfRangeException::ArgumentOutOfRangeException(const Misc::UnicodeString &paramName) :
								 ArgumentException(L"OSHGui::ArgumentOutOfRangeException", paramName, L"Value was out of valid range.")
	{

	}
	//---------------------------------------------------------------------------
	ArgumentOutOfRangeException::ArgumentOutOfRangeException(const Misc::UnicodeString &paramName,
															 const Misc::UnicodeString &message) :
								 ArgumentException(L"OSHGui::ArgumentOutOfRangeException", paramName, message)
	{

	}
	//---------------------------------------------------------------------------
	ArgumentOutOfRangeException::ArgumentOutOfRangeException(const Misc::UnicodeString &paramName,
															 const Misc::UnicodeString &file,
															 int line) :
								 ArgumentException(L"OSHGui::ArgumentOutOfRangeException", paramName, L"Value was out of valid range.", file, line)
	{

	}
	//---------------------------------------------------------------------------
	ArgumentOutOfRangeException::ArgumentOutOfRangeException(const Misc::UnicodeString &paramName,
															 const Misc::UnicodeString &message,
															 const Misc::UnicodeString &file,
															 int line) :
								 ArgumentException(L"OSHGui::ArgumentOutOfRangeException", paramName, message, file, line)
	{

	}
	//---------------------------------------------------------------------------
	//Constructor
	//---------------------------------------------------------------------------
	InvalidOperationException::InvalidOperationException(const Misc::UnicodeString &message) :
							   Exception(L"OSHGui::InvalidOperationException", message)
	{

	}
	//---------------------------------------------------------------------------
	InvalidOperationException::InvalidOperationException(const Misc::UnicodeString &message,
														 const Misc::UnicodeString &file,
														 int line) :
							   Exception(L"OSHGui::InvalidOperationException", message, file, line)
	{

	}
	//---------------------------------------------------------------------------
}