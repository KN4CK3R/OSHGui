#ifndef OSHGUI_EXCEPTIONS_HPP
#define OSHGUI_EXCEPTIONS_HPP

#include <exception>
#include "../Exports.hpp"
#include "Strings.hpp"

#define WIDEN2(x) L ## x
#define WIDEN(x) WIDEN2(x)
#define __WFILE__ WIDEN(__FILE__)

namespace OSHGui
{
	namespace Misc
	{
		/**
		 * Stellt Fehler dar, die beim Ausführen einer Anwendung auftreten.
		 */
		class OSHGUI_EXPORT Exception : public std::exception
		{
		public:
			/**
			 * Initialisiert eine neue Instanz der Exception-Klasse.
			 *
			 * @param message die Meldung, in der der Fehler beschrieben wird
			 */
			Exception(const Misc::AnsiString &message = "");
			/**
			 * Initialisiert eine neue Instanz der Exception-Klasse.
			 *
			 * @param message die Meldung, in der der Fehler beschrieben wird
			 * @param file der Name der Datei, in der der Fehler auftrat
			 * @param line die Zeilennummer, in der der Fehler auftrat
			 */
			Exception(const Misc::AnsiString &message,
					  const Misc::AnsiString &file,
					  int line);
			virtual ~Exception() throw();
		
			/**
			 * Ruft die Meldung ab, die die aktuelle Ausnahme beschreibt.
			 *
			 * @return message
			 */
			const Misc::AnsiString& GetMessage() const;
			/**
			 * Ruft den Namen der Ausnahme ab.
			 *
			 * @return name
			 */
			const Misc::AnsiString& GetName() const;
			/**
			 * Ruft den Dateinamen ab, in dem die Ausnahme auftrat.
			 *
			 * @return fileName
			 */
			const Misc::AnsiString& GetFileName() const;
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
			const Misc::AnsiString& ToString() const;

			//overwrite std::exception what()
			const char* what() const throw();
		
		protected:
			/**
			 * Initialisiert eine neue Instanz der Exception-Klasse.
			 *
			 * @param name der Name der Exception
			 * @param message die Meldung, in der der Fehler beschrieben wird
			 */
			Exception(const Misc::AnsiString &name,
					  const Misc::AnsiString &message);
			/**
			 * Initialisiert eine neue Instanz der Exception-Klasse.
			 *
			 * @param name der Name der Exception
			 * @param message die Meldung, in der der Fehler beschrieben wird
			 * @param file der Name der Datei, in der der Fehler auftrat
			 * @param line die Zeilennummer, in der der Fehler auftrat
			 */
			Exception(const Misc::AnsiString &name,
					  const Misc::AnsiString &message,
					  const Misc::AnsiString &file,
					  int line);

			Misc::AnsiString message;
			Misc::AnsiString name;
			Misc::AnsiString file;
			int line;
			Misc::AnsiString combinedMessage;
	
		private:
			//copying prohibited
			/*Exception(const Exception&);
			void operator = (const Exception&);*/
		};
	
		/**
		 * Die Ausnahme, die ausgelöst wird, wenn eines der Argumente für eine Methode ungültig ist.
		 */
		class OSHGUI_EXPORT ArgumentException : public Exception
		{
		public:
			/**
			 * Initialisiert eine neue Instanz der ArgumentException-Klasse.
			 *
			 * @param paramName der Name des Arguments
			 */
			ArgumentException(const Misc::AnsiString &paramName);
			/**
			 * Initialisiert eine neue Instanz der ArgumentException-Klasse.
			 *
			 * @param paramName der Name des Arguments
			 * @param message die Meldung, in der der Fehler beschrieben wird
			 */
			ArgumentException(const Misc::AnsiString &paramName,
							  const Misc::AnsiString &message);
			/**
			 * Initialisiert eine neue Instanz der ArgumentException-Klasse.
			 *
			 * @param paramName der Name des Arguments
			 * @param file der Name der Datei, in der der Fehler auftrat
			 * @param line die Zeilennummer, in der der Fehler auftrat
			 */
			ArgumentException(const Misc::AnsiString &paramName,
							  const Misc::AnsiString &file,
							  int line);
			/**
			 * Initialisiert eine neue Instanz der ArgumentException-Klasse.
			 *
			 * @param paramName der Name des Arguments
			 * @param message die Meldung, in der der Fehler beschrieben wird
			 * @param file der Name der Datei, in der der Fehler auftrat
			 * @param line die Zeilennummer, in der der Fehler auftrat
			 */
			ArgumentException(const Misc::AnsiString &paramName,
							  const Misc::AnsiString &message,
							  const Misc::AnsiString &file,
							  int line);
		
			/**
			 * Ruft den Namen des Parameters ab, der die Ausnahme auslöste.
			 *
			 * @return paramName
			 */
			const Misc::AnsiString& GetParamName() const;

		protected:
			/**
			 * Initialisiert eine neue Instanz der ArgumentException-Klasse.
			 *
			 * @param name der Name der Exception
			 * @param paramName der Name des Arguments
			 * @param message die Meldung, in der der Fehler beschrieben wird
			 */
			ArgumentException(const Misc::AnsiString &name,
							  const Misc::AnsiString &paramName,
							  const Misc::AnsiString &message);
			/**
			 * Initialisiert eine neue Instanz der ArgumentException-Klasse.
			 *
			 * @param name der Name der Exception
			 * @param paramName der Name des Arguments
			 * @param message die Meldung, in der der Fehler beschrieben wird
			 * @param file der Name der Datei, in der der Fehler auftrat
			 * @param line die Zeilennummer, in der der Fehler auftrat
			 */
			ArgumentException(const Misc::AnsiString &name,
							  const Misc::AnsiString &paramName,
							  const Misc::AnsiString &message,
							  const Misc::AnsiString &file,
							  int line);
	
		private:
			Misc::AnsiString paramName;
		};
	
		/**
		 * Die Ausnahme, die ausgelöst wird, wenn ein NULL-Verweis an eine Methode
		 * übergeben wird, für die dies kein gültiges Argument ist.
		 */
		class OSHGUI_EXPORT ArgumentNullException : public ArgumentException
		{
		public:
			/**
			 * Initialisiert eine neue ArgumentNullException der Exception-Klasse.
			 *
			 * @param paramName der Name des Arguments
			 */
			ArgumentNullException(const Misc::AnsiString &paramName);
			/**
			 * Initialisiert eine neue ArgumentNullException der Exception-Klasse.
			 *
			 * @param paramName der Name des Arguments
			 * @param message die Meldung, in der der Fehler beschrieben wird
			 */
			ArgumentNullException(const Misc::AnsiString &paramName,
								  const Misc::AnsiString &message);
			/**
			 * Initialisiert eine neue ArgumentNullException der Exception-Klasse.
			 *
			 * @param paramName der Name des Arguments
			 * @param file der Name der Datei, in der der Fehler auftrat
			 * @param line die Zeilennummer, in der der Fehler auftrat
			 */
			ArgumentNullException(const Misc::AnsiString &paramName,
								  const Misc::AnsiString &file,
								  int line);
			/**
			 * Initialisiert eine neue ArgumentNullException der Exception-Klasse.
			 *
			 * @param paramName der Name des Arguments
			 * @param message die Meldung, in der der Fehler beschrieben wird
			 * @param file der Name der Datei, in der der Fehler auftrat
			 * @param line die Zeilennummer, in der der Fehler auftrat
			 */
			ArgumentNullException(const Misc::AnsiString &paramName,
								  const Misc::AnsiString &message,
								  const Misc::AnsiString &file,
								  int line);
		};
	
		/**
		 * Die Ausnahme, die ausgelöst wird, wenn der Wert eines Arguments nicht in dem Wertebereich
		 * liegt, der durch die aufgerufene Methode als zulässig definiert ist.
		 */
		class OSHGUI_EXPORT ArgumentOutOfRangeException : public ArgumentException
		{
		public:
			/**
			 * Initialisiert eine neue Instanz der ArgumentOutOfRangeException-Klasse.
			 *
			 * @param paramName der Name des Arguments
			 */
			ArgumentOutOfRangeException(const Misc::AnsiString &paramName);
			/**
			 * Initialisiert eine neue Instanz der ArgumentOutOfRangeException-Klasse.
			 *
			 * @param paramName der Name des Arguments
			 * @param message die Meldung, in der der Fehler beschrieben wird
			 */
			ArgumentOutOfRangeException(const Misc::AnsiString &paramName,
										const Misc::AnsiString &message);
			/**
			 * Initialisiert eine neue Instanz der ArgumentOutOfRangeException-Klasse.
			 *
			 * @param paramName der Name des Arguments
			 * @param file der Name der Datei, in der der Fehler auftrat
			 * @param line die Zeilennummer, in der der Fehler auftrat
			 */
			ArgumentOutOfRangeException(const Misc::AnsiString &paramName,
										const Misc::AnsiString &file,
										int line);
			/**
			 * Initialisiert eine neue Instanz der ArgumentOutOfRangeException-Klasse.
			 *
			 * @param paramName der Name des Arguments
			 * @param message die Meldung, in der der Fehler beschrieben wird
			 * @param file der Name der Datei, in der der Fehler auftrat
			 * @param line die Zeilennummer, in der der Fehler auftrat
			 */
			ArgumentOutOfRangeException(const Misc::AnsiString &paramName,
										const Misc::AnsiString &message,
										const Misc::AnsiString &file,
										int line);
		};
	
		/**
		 * Die Ausnahme, die ausgelöst wird, wenn der Aufruf einer Methode
		 * aufgrund des aktuellen Zustands des Objekts ungültig ist.
		 */
		class OSHGUI_EXPORT InvalidOperationException : public Exception
		{
		public:
			/**
			 * Initialisiert eine neue Instanz der InvalidOperationException-Klasse.
			 *
			 * @param message die Meldung, in der der Fehler beschrieben wird
			 */
			InvalidOperationException(const Misc::AnsiString &message = "Operation is not valid due to the current state of the object.");
			/**
			 * Initialisiert eine neue Instanz der InvalidOperationException-Klasse.
			 *
			 * @param message die Meldung, in der der Fehler beschrieben wird
			 * @param file der Name der Datei, in der der Fehler auftrat
			 * @param line die Zeilennummer, in der der Fehler auftrat
			 */
			InvalidOperationException(const Misc::AnsiString &message,
									  const Misc::AnsiString &file,
									  int line);
		};
	}
}

#endif