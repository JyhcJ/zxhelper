// ע�����ˣ��ܵ���������
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
			CancelIoEx(hPipe, NULL); // ȡ�� ConnectNamedPipe �ĵȴ�
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
		Call_���������Ϣ("[�����]PipeServer��ʼ��");

		// �������������û����ʵĹܵ�
		SECURITY_ATTRIBUTES sa = { 0 };
		SECURITY_DESCRIPTOR sd;

		// 1. ��ʼ����ȫ������
		InitializeSecurityDescriptor(&sd, SECURITY_DESCRIPTOR_REVISION);
		// 2. ����DACL�����������û����ʣ�
		SetSecurityDescriptorDacl(&sd, TRUE, NULL, FALSE);
		sa.nLength = sizeof(sa);
		sa.lpSecurityDescriptor = &sd;
		sa.bInheritHandle = FALSE;

		hPipe = NULL;
		while (!m_stopFlag) {
			//Call_���������Ϣ("[�����]hPipe = %d", hPipe);
			// ���������ܵ���֧�ֶ�ͻ������ӣ�
			if (hPipe != INVALID_HANDLE_VALUE) {
				hPipe = INVALID_HANDLE_VALUE;
			}
			hPipe = CreateNamedPipeW(
				L"\\\\.\\pipe\\MyPipe",             // �ܵ�����
				PIPE_ACCESS_DUPLEX,                 // ˫��ͨ��
				PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT | PIPE_REJECT_REMOTE_CLIENTS,
				PIPE_UNLIMITED_INSTANCES,           // ������ʵ��
				1024, 1024,                         // ����/�����������С
				5000,                                  // Ĭ�ϳ�ʱ�����룩
				&sa                                // Ĭ�ϰ�ȫ����

			);
	
			if (hPipe == INVALID_HANDLE_VALUE) {
				// ������
				Call_���������Ϣ("[�����]������");
				hPipe = NULL;  // ���þ��
				Sleep(1000);   // ����Ƶ������
				break;
			}
			Call_���������Ϣ("[�����]�ܵ������ɹ�");
			// �ȴ��ͻ�������
			//Call_���������Ϣ("[�����]PipeThread�ȴ��ͻ�������");
			g_hPipeThread = (HANDLE)m_thread.native_handle();
			g_hPipe = hPipe;
			while (!m_stopFlag)
			{
				if (arry.empty())
				{
					Sleep(500);
				}
				else {
					Call_���������Ϣ("[�����]�ɹ���ȡ���ݣ��ȴ�����...");
					break;
				}
			}
		

			BOOL connected = ConnectNamedPipe(hPipe, NULL) ? TRUE : (GetLastError() == ERROR_PIPE_CONNECTED);

			if (connected) {
				Call_���������Ϣ("[�����]PipeThread���ӳɹ�");
				//// ���̴߳���ͻ���ͨ��Lamda��ʽ
				//std::thread t([this]() {
					ProcessClient(hPipe);
					//DisconnectNamedPipe(hPipe);     // �Ͽ���ǰ����
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
				CloseHandle(hPipe);  // ����ʧ��ʱ�رչܵ�
			}

			Sleep(100);
		}
	}


	//void ProcessClient(HANDLE hPipe) {
	//    Data data;
	//    DWORD bytesRead;
	//    
	//    // ��ȡ����
	//    if (ReadFile(hPipe, &data, sizeof(data), &bytesRead, NULL)) {
	//        // ������յ�������
	//        OnDataReceived(data);
	//        
	//        //// ����д����Ӧ�������Ҫ��
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
		//�ܵ��ı����� FIFO���Ƚ��ȳ����ֽ�������֤д��Ͷ�ȡ��˳��һ�¡�
		DWORD bytesWritten;
		// �ȷ���������
	

		size_t count = this->arry.size();
		if (count != 0)
		{
			if (!WriteFile(hPipe, &count, sizeof(count), &bytesWritten, NULL)) {
				Call_���������Ϣ("[�����]countд��ܵ�ʧ�ܣ�������룺%d", GetLastError());
			}
			else {
				Call_���������Ϣ("[�����]count�ɹ����� :%d", count);
			}

			if (!WriteFile(hPipe, this->arry.data(), static_cast<DWORD>(count * sizeof(QWORD)), &bytesWritten, NULL)) {
				Call_���������Ϣ("[�����]arry.data()д��ܵ�ʧ�ܣ�������룺%d", GetLastError());
			}
			else {
				Call_���������Ϣ("[�����]arry.data()�ɹ�����");
			}
		}
		
		//����pid ��dll���
		uint32_t len = 0;
		
		if (ReadFile(hPipe, &len, sizeof(len), &bytesWritten, NULL)) {
			Call_���������Ϣ("[�����]��ȡ len :%d", len);
			std::string buf(len, '\0');
			if (ReadFile(hPipe, &buf[0], len, &bytesWritten, NULL)) {
				auto received_json = nlohmann::json::parse(buf);
				DWORD dllPid = (DWORD)received_json.value("dllPid", 0);
				QWORD dllHwnd = (QWORD)received_json.value("dllHwnd", 0);
				std::pair<DWORD, QWORD> pair = std::make_pair(dllPid, dllHwnd);
				g_dllHwnds.push_back(pair);
			}
			else {
				Call_���������Ϣ("[�����]��ȡ len ʧ�� ������:%d", GetLastError());
			}
			
		}
		else {
			Call_���������Ϣ("[�����]��ȡ string ʧ�� ������:%d", GetLastError());
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

		//std::string name = received_json["name"]; // ֱ�ӷ���(�ַ���)
		//int age = received_json.value("age", 0);  // ��ȫ���ʣ��ṩĬ��ֵ��
	
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