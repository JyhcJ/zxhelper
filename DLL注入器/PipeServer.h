// 注入器端（管道服务器）
#include <atomic>
#include <thread>
#include "mainController.h"
#include <vector>

#include <nlohmann/json.hpp>
extern HANDLE g_hPipeThread;
extern HANDLE g_hPipe;
extern std::vector<std::pair<DWORD, QWORD>>g_dllHwnds;

class PipeServer {
public:


	PipeServer() : m_stopFlag(false) {

		m_thread = std::thread(&PipeServer::Run, this);
	}

	~PipeServer() {
		m_stopFlag = true;

		if (hPipe != INVALID_HANDLE_VALUE) {
			CancelIoEx(hPipe, NULL); // 取消 ConnectNamedPipe 的等待
			DisconnectNamedPipe(hPipe);
			CloseHandle(hPipe);
		}

		if (m_thread.joinable()) {
			m_thread.join();
		}

	

	


	


	}
	void SetDatas(std::vector<QWORD> arry) {
		this->arry = arry;
	}
private:

	void Run() {
		Call_输出调试信息("[服务端]PipeServer初始化");

		// 创建允许所有用户访问的管道
		SECURITY_ATTRIBUTES sa = { 0 };
		SECURITY_DESCRIPTOR sd;

		// 1. 初始化安全描述符
		InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
		// 2. 禁用DACL（允许所有用户访问）
		SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = &sd;
		sa.bInheritHandle = FALSE;

		hPipe = NULL;
		while (!m_stopFlag) {
			//Call_输出调试信息("[服务端]hPipe = %d", hPipe);
			// 创建命名管道（支持多客户端连接）
			if (hPipe != INVALID_HANDLE_VALUE) {
				hPipe = INVALID_HANDLE_VALUE;
			}
			hPipe = CreateNamedPipeW(
				L"\\\\.\\pipe\\MyPipe",             // 管道名称
				PIPE_ACCESS_DUPLEX,                 // 双向通信
				PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT | PIPE_REJECT_REMOTE_CLIENTS,
				PIPE_UNLIMITED_INSTANCES,           // 允许多个实例
				1024, 1024,                         // 输入/输出缓冲区大小
				5000,                                  // 默认超时（毫秒）
				&sa                                // 默认安全属性

			);
	
			if (hPipe == INVALID_HANDLE_VALUE) {
				// 错误处理
				Call_输出调试信息("[服务端]错误处理");
				hPipe = NULL;  // 重置句柄
				Sleep(1000);   // 避免频繁重试
				break;
			}
			Call_输出调试信息("[服务端]管道创建成功");
			// 等待客户端连接
			//Call_输出调试信息("[服务端]PipeThread等待客户端连接");
			g_hPipeThread = (HANDLE)m_thread.native_handle();
			g_hPipe = hPipe;
			while (!m_stopFlag)
			{
				if (arry.empty())
				{
					Sleep(500);
				}
				else {
					Call_输出调试信息("[服务端]成功获取数据，等待连接...");
					break;
				}
			}
		

			BOOL connected = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

			if (connected) {
				Call_输出调试信息("[服务端]PipeThread连接成功");
				//// 新线程处理客户端通信Lamda形式
				//std::thread t([this]() {
					ProcessClient(hPipe);
					//DisconnectNamedPipe(hPipe);     // 断开当前连接
					CloseHandle(hPipe);
				//	});
// 					   m_thread
				//this->hThread = (HANDLE)t.native_handle();
				//this->hThread = ;
				//t.join();


			/*	std::thread t(thread_func);
				this->hThread = (HANDLE)t.native_handle();
				t.join();*/
			}
			else {
				CloseHandle(hPipe);  // 连接失败时关闭管道
			}

			Sleep(100);
		}
	}


	//void ProcessClient(HANDLE hPipe) {
	//    Data data;
	//    DWORD bytesRead;
	//    
	//    // 读取数据
	//    if (ReadFile(hPipe, &data, sizeof(data), &bytesRead, NULL)) {
	//        // 处理接收到的数据
	//        OnDataReceived(data);
	//        
	//        //// 可以写回响应（如果需要）
	//        //DWORD bytesWritten;
	//        //WriteFile(hPipe, &response, sizeof(response), &bytesWritten, NULL);
	//    }
	//}

	void ProcessClient(HANDLE hPipe) {
	/*	while (true)
		{
			if (this->arry.empty())
			{
				Sleep(500);
			}
			else {
				break;
			}
			Sleep(500);
		}*/
		//管道的本质是 FIFO（先进先出）字节流，保证写入和读取的顺序一致。
		DWORD bytesWritten;
		// 先发送数据量
	

		size_t count = this->arry.size();
		if (count != 0)
		{
			if (!WriteFile(hPipe, &count, sizeof(count), &bytesWritten, NULL)) {
				Call_输出调试信息("[服务端]count写入管道失败，错误代码：%d", GetLastError());
			}
			else {
				Call_输出调试信息("[服务端]count成功发送 :%d", count);
			}

			if (!WriteFile(hPipe, this->arry.data(), static_cast<DWORD>(count * sizeof(QWORD)), &bytesWritten, NULL)) {
				Call_输出调试信息("[服务端]arry.data()写入管道失败，错误代码：%d", GetLastError());
			}
			else {
				Call_输出调试信息("[服务端]arry.data()成功发送");
			}
		}
		
		//接收pid 和dll句柄
		uint32_t len = 0;
		
		if (ReadFile(hPipe, &len, sizeof(len), &bytesWritten, NULL)) {
			Call_输出调试信息("[服务端]读取 len :%d", len);
			std::string buf(len, '\0');
			if (ReadFile(hPipe, &buf[0], len, &bytesWritten, NULL)) {
				auto received_json = nlohmann::json::parse(buf);
				DWORD dllPid = (DWORD)received_json.value("dllPid", 0);
				QWORD dllHwnd = (QWORD)received_json.value("dllHwnd", 0);
				std::pair<DWORD, QWORD> pair = std::make_pair(dllPid, dllHwnd);
				g_dllHwnds.push_back(pair);
			}
			else {
				Call_输出调试信息("[服务端]读取 len 失败 错误码:%d", GetLastError());
			}
			
		}
		else {
			Call_输出调试信息("[服务端]读取 string 失败 错误码:%d", GetLastError());
		}
		

		
	

	/*	Returns either a copy of an object's element at the specified key key or a given default value if no element with key key exists.

			The function is basically equivalent to executing

			try {
			return at(key);
		}
		catch (out_of_range) {
			return default_value;}
		*/

		//received_json.value("address", json::object()).value("city", "Unknown")

		//std::string name = received_json["name"]; // 直接访问(字符串)
		//int age = received_json.value("age", 0);  // 安全访问（提供默认值）
	
	}



	HANDLE hThread;
	HANDLE hPipe;
	//virtual void OnDataReceived(const Data& data) = 0;
	std::thread m_thread;
	std::atomic<bool> m_stopFlag;
	//std::map<DWORD, HWND> hwndMap;
	//QWORD* datas;
	std::vector<QWORD> arry;
	bool isExist = false;



};