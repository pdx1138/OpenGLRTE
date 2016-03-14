#include "Debug.ErrorLog.h"

#define DEBUG_ERRORLOG_MAX_FILE_PATH 25

namespace DEBUG{

	namespace ErrorLog {

		ERROR_LOG::ERROR_LOG() {
			this->filePath = (char*)malloc(sizeof(char) * DEBUG_ERRORLOG_MAX_FILE_PATH);
			memset(this->filePath, 0, sizeof(char) * DEBUG_ERRORLOG_MAX_FILE_PATH);

			this->filePath = "ErrorLog.txt";
			//TODO:: CREATE THE FILE!!!
			//errorLog.open("ErrorLog.txt", ios::app);
		}

		ERROR_LOG::ERROR_LOG(char* filePath) {
			this->filePath = filePath;
		}

		ERROR_LOG::~ERROR_LOG() {
			//free(this->filePath);
			//this->filePath = nullptr;

			for (int i = errorMsgList.length - 1; i >= 0; --i) {
				errorMsgList.RemoveAt(i);
			}
		}

		void ERROR_LOG::PrintErrorLog() {
			//TODO: Add #define check for multi-thearding so we only run this when we are multi-threading
			DebugError_mutex.lock();
			/*ofstream dataLog;*/
			//dataLog.open("DataLog.txt", ios::app);

			ofstream errorLog;
			errorLog.open(filePath, ios::app);
			for (int i = 0; i < errorMsgList.length; ++i) { 

				errorLog << errorMsgList[i];
			}

			errorLog.close();
			DebugError_mutex.unlock();
		} 

		void ERROR_LOG::Message(char* message) {
			char messageBuffer[80] ={0};

			errorMsgList.AddValue(message);
		}

		void ERROR_LOG::Message(char* message, int number) {
			char* msgBuffer = new char[80];
			memset(msgBuffer, 0, 80);
			char* lineNum = new char[4];
			memset(lineNum, 0, 4);
			itoa(number,lineNum, 10);
			
			strcpy(msgBuffer, message);

			strcat(msgBuffer, lineNum);
			strcat(msgBuffer, "\n");
			//errorLog->AddErrorMessage(message);
			errorMsgList.AddValue(msgBuffer);
		}
	}

}