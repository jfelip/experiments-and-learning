#ifndef _COMMON_H_
#define _COMMON_H_

#include <iostream>
#include <sstream>
#include <string>

#define _GENERIC_ERROR_(msg) CCommon::OnGenericError(__PRETTY_FUNCTION__,__FILE__,__LINE__,msg);
#define _GENERIC_EXCEPTION_(Exception) CCommon::OnGenericException(Exception,__PRETTY_FUNCTION__,__FILE__,__LINE__);
#define _GENERIC_WARNING_(msg) CCommon::OnGenericWarning(__PRETTY_FUNCTION__,__FILE__,__LINE__,msg);
#define _GENERIC_DEBUG_(Content) CCommon::Message(Content);
#define _GENERIC_MESSAGE_(Content) CCommon::Message(Content);

class CCommon {
public:

	static void OnGenericError(const char* pCallerFunction,
			const char* pCallerFile, const uint SourceCodeLine, const std::string msg = "") {
		std::ostringstream Stream;

		Stream << "[ " << pCallerFunction << " ][ " << pCallerFile << " : "
				<< SourceCodeLine << " ] {"<< msg <<"}";

		std::cerr << "\033[31m" << "[Error] " << Stream.str() << "\033[0m"
				<< std::endl << std::flush;
	}

	static void OnGenericException(std::exception& Exception,
			const char* pCallerFunction, const char* pCallerFile,
			const uint SourceCodeLine) {
		std::ostringstream Stream;

		Stream << "[ " << pCallerFunction << " ][ " << pCallerFile << " : "
				<< SourceCodeLine << " ] {" << Exception.what() << "}";

		std::cerr << "\033[36m" << "[Exception] [ " << Stream.str()
				<< " ] \033[0m" << std::endl << std::flush;
	}

	static void OnGenericWarning(const char* pCallerFunction,
			const char* pCallerFile, const uint SourceCodeLine, const std::string& msg = "") {
		std::ostringstream Stream;

		Stream << "[ " << pCallerFunction << " ][ " << pCallerFile << " : "
				<< SourceCodeLine << " ] {"<< msg <<"}";

		std::cerr << "\033[35m" << "[Warning] " << Stream.str() << "\033[0m"
				<< std::endl << std::flush;
	}

	static void Message(const std::string& Content) {
		std::cout << "\033[35m" << "[Message] [" << Content << "] " << "\033[0m" << std::endl << std::flush;
	}
};

#endif
