#ifndef OSHGUI_EXCEPTIONS_H_
#define OSHGUI_EXCEPTIONS_H_

#include <exception>
#include "Misc\Strings.h"

#define WIDEN(x) L##x
#define Exception(message) Exception(message, L"OSHGui::Exception", WIDEN(__FILE__), __LINE__)
#define ArgumentException(paramName) ArgumentException(L"Unexpected argument.", paramName, WIDEN(__FILE__), __LINE__)
#define ArgumentException(message, paramName) ArgumentException(message, paramName, WIDEN(__FILE__), __LINE__)
#define ArgumentNullException(paramName) ArgumentNullException(L"Value cannot be null.", paramName, WIDEN(__FILE__), __LINE__)
#define ArgumentNullException(message, paramName) ArgumentNullException(message, paramName, WIDEN(__FILE__), __LINE__)
#define ArgumentOutOfRangeException(paramName) ArgumentOutOfRangeException(L"Value was out of valid range.", paramName, WIDEN(__FILE__), __LINE__)
#define ArgumentOutOfRangeException(message, paramName) ArgumentOutOfRangeException(message, paramName, WIDEN(__FILE__), __LINE__)
#define InvalidOperationException() InvalidOperationException(L"Operation is not valid due to the current state of the object.", WIDEN(__FILE__), __LINE__)
#define InvalidOperationException(message) InvalidOperationException(message, WIDEN(__FILE__), __LINE)

namespace OSHGui
{
	/**
	 * Stellt Fehler dar, die beim Ausf�hren einer Anwendung auftreten.
	 */
	class Exception : public std::exception
	{
	public:
		virtual ~Exception() throw();
		
		/**
		 * Ruft die Meldung ab, die die aktuelle Ausnahme beschreibt.
		 *
		 * @return message
		 */
		const Misc::UnicodeString& GetMessage() const;
		/**
		 * Ruft den Namen der Ausnahme ab.
		 *
		 * @return name
		 */
		const Misc::UnicodeString& GetName() const;
		/**
		 * Ruft den Dateinamen ab, in dem die Ausnahme auftrat.
		 *
		 * @return fileName
		 */
		const Misc::UnicodeString& GetFileName() const;
		/**
		 * Ruft ddie Zeilennummer ab, in der die Ausnahme auftrat.
		 *
		 * @return name
		 */
		int GetLine() const;
		/**
		 * Ruft die Zeichenfolgenentsprechung der aktuellen Ausnahme ab.
		 *
		 * @return
		 */
		const Misc::UnicodeString& ToString() const;

		//overwrite std::exception what()
		const char* what() const throw();
		
	protected:
		/**
		 * Initialisiert eine neue Instanz der Exception-Klasse.
		 *
		 * @param message die Meldung, in der der Fehler beschrieben wird
		 * @param name der Name der Exception
		 * @param file der Name der Datei, in der der Fehler auftrat
		 * @param line die Zeilennummer, in der der Fehler auftrat
		 */
		Exception(const Misc::UnicodeString &message,
				  const Misc::UnicodeString &name = L"OSHGui::Exception",
				  const Misc::UnicodeString &file = L"",
				  int line = 0);
	
		Misc::UnicodeString message;
		Misc::UnicodeString name;
		Misc::UnicodeString file;
		int line;
		Misc::UnicodeString combinedMessage;
	};
	
	/**
	 * Die Ausnahme, die ausgel�st wird, wenn eines der Argumente f�r eine Methode ung�ltig ist.
	 */
	class ArgumentException : public Exception
	{
	public:
		/**
		 * Initialisiert eine neue Instanz der ArgumentException-Klasse.
		 *
		 * @param message die Meldung, in der der Fehler beschrieben wird
		 * @param paramName der Name des Arguments
		 * @param file der Name der Datei, in der der Fehler auftrat
		 * @param line die Zeilennummer, in der der Fehler auftrat
		 */
		ArgumentException(const Misc::UnicodeString &message,
						  const Misc::UnicodeString &paramName = L"",
						  const Misc::UnicodeString &file = L"",
						  int line = 0);
		
		/**
		 * Ruft den Namen des Parameters ab, der die Ausnahme ausl�ste.
		 *
		 * @return paramName
		 */
		const Misc::UnicodeString& GetParamName() const;
	
	protected:
		/**
		 * Initialisiert eine neue Instanz der ArgumentException-Klasse.
		 *
		 * @param message die Meldung, in der der Fehler beschrieben wird
		 * @param paramName der Name des Arguments
		 * @param name der Name der Exception
		 * @param file der Name der Datei, in der der Fehler auftrat
		 * @param line die Zeilennummer, in der der Fehler auftrat
		 */
		ArgumentException(const Misc::UnicodeString &message,
						  const Misc::UnicodeString &paramName = L"",
						  const Misc::UnicodeString &name = L"OSHGui::ArgumentException",
						  const Misc::UnicodeString &file = L"",
						  int line = 0);
	
	private:
		Misc::UnicodeString paramName;
	};
	
	/**
	 * Die Ausnahme, die ausgel�st wird, wenn ein NULL-Verweis an eine Methode
	 * �bergeben wird, f�r die dies kein g�ltiges Argument ist.
	 */
	class ArgumentNullException : public ArgumentException
	{
	public:
		/**
		 * Initialisiert eine neue ArgumentNullException der Exception-Klasse.
		 *
		 * @param message die Meldung, in der der Fehler beschrieben wird
		 * @param paramName der Name des Arguments
		 * @param file der Name der Datei, in der der Fehler auftrat
		 * @param line die Zeilennummer, in der der Fehler auftrat
		 */
		ArgumentNullException(const Misc::UnicodeString &message,
							  const Misc::UnicodeString &paramName,
							  const Misc::UnicodeString &file,
							  int line);
	};
	
	/**
	 * Die Ausnahme, die ausgel�st wird, wenn der Wert eines Arguments nicht in dem Wertebereich
	 * liegt, der durch die aufgerufene Methode als zul�ssig definiert ist.
	 */
	class ArgumentOutOfRangeException : public ArgumentException
	{
	public:
		/**
		 * Initialisiert eine neue Instanz der ArgumentOutOfRangeException-Klasse.
		 *
		 * @param message die Meldung, in der der Fehler beschrieben wird
		 * @param paramName der Name des Arguments
		 * @param file der Name der Datei, in der der Fehler auftrat
		 * @param line die Zeilennummer, in der der Fehler auftrat
		 */
		ArgumentOutOfRangeException(const Misc::UnicodeString &message,
									const Misc::UnicodeString &paramName,
									const Misc::UnicodeString &file,
									int line);
	};
	
	/**
	 * Die Ausnahme, die ausgel�st wird, wenn der Aufruf einer Methode
	 * aufgrund des aktuellen Zustands des Objekts ung�ltig ist.
	 */
	class InvalidOperationException : public Exception
	{
	public:
		/**
		 * Initialisiert eine neue Instanz der InvalidOperationException-Klasse.
		 *
		 * @param message die Meldung, in der der Fehler beschrieben wird
		 * @param file der Name der Datei, in der der Fehler auftrat
		 * @param line die Zeilennummer, in der der Fehler auftrat
		 */
		InvalidOperationException(const Misc::UnicodeString &message,
								  const Misc::UnicodeString &file,
								  int line);
	};
}

#endif