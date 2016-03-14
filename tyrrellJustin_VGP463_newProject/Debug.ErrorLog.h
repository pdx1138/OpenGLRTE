
#pragma once

#include "Student.List.h"
#include <stdio.h>
#include <time.h>
#include <fstream>
#include <mutex>

//TODO:  Make base Debug.cpp/h and place #defines 
//#define USING_MULTITHREADING
//base I/O class that Error/Data inherits from (File opening, etc...)
// Base debug object that can take new components as added.

namespace DEBUG {

	namespace ErrorLog {

		class ERROR_LOG {
	
			public:
				ERROR_LOG();
				ERROR_LOG(char* filePath);
				~ERROR_LOG();

				void PrintErrorLog();
				void Message(char* message);
				void Message(char* message, int number);

				char* GetFilePath();

			private:

				void SetFilePath(char* path);

				char* filePath;
				mutex DebugError_mutex;
				List<char*> errorMsgList;

		};
	}
}