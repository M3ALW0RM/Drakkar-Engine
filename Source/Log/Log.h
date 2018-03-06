#pragma once
#include <cstdio>
#include <iostream>

/*!
*	@file
*/

enum class LoggerVerbosity {
	RELEASE = 0, /*!< Release level verbosity: outputs to file */
	DEBUG = 10, /*!< Debug level verbosity: outputs to file and console */
	FATAL = 20 /*!< Fatal level verbosity: outputs to file and crashes program */
};

inline bool constexpr operator<(LoggerVerbosity& a, LoggerVerbosity& b) {
	return (int)a < (int)b;
}

inline bool constexpr operator>(LoggerVerbosity& a, LoggerVerbosity& b) {
	return (int)a > (int)b;
}


template <typename T>
struct LogCategory {
	static const LoggerVerbosity compileMaxVerbosity;
};

FILE* OpenLogFile();
void CloseLogFile(FILE* file);

#define DEFINE_CATEGORY(name) struct name : public LogCategory<name>{}; 
#define DECLARE_CATEGORY(name, comp) const LoggerVerbosity name::compileMaxVerbosity(comp);

#define DRAK_LOG(category, verbosity, outputString, ...){			\
	if constexpr(verbosity <= category::compileMaxVerbosity){		\
		FILE * logFile = OpenLogFile();								\
		switch (verbosity) {										\
		case LoggerVerbosity::DEBUG:								\
				fprintf(logFile, outputString, __VA_ARGS__);		\
				printf(outputString, __VA_ARGS__);					\
				CloseLogFile(logFile);								\
				break;												\
		case LoggerVerbosity::RELEASE:								\
				fprintf(logFile, outputString, __VA_ARGS__);		\
				CloseLogFile(logFile);								\
				break;												\
		case LoggerVerbosity::FATAL:								\
				fprintf(logFile, outputString, __VA_ARGS__);		\
				CloseLogFile(logFile);								\
				break;												\
		}															\
	}																\
}

/*!
	\enum LoggerVerbosity
	\brief An enum defining logging verbosity.
*/
/*!
	\def DEFINE_CATEGORY(name)
	Defines a log category
*/
/*!
	\def DECLARE_CATEGORY(name, comp)
	Declares a previously defined log category and sets its maximum verbosity
*/
/*!
	\def DRAK_LOG(category, warningLevel, outputString, ...)
	Logs a message to the console or the log file based on verbosity. Does not compile code if the verbosity is higher than the category's max verbosity.
*/