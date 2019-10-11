#pragma once

#include <algorithm>
#include <cctype>

#include "include/cef_browser.h"
#include "include/cef_callback.h"
#include "include/cef_frame.h"
#include "include/cef_resource_handler.h"
#include "include/cef_response.h"
#include "include/cef_request.h"
#include "include/cef_scheme.h"
#include "include/wrapper/cef_helpers.h"
#include "data_transfer_control/data_transfer_control.h"

#include "util/StringUtil.h"

#define HANDLER_IAMGE_CONTROLLER				"image_controller"
#define HANDLER_IAMGE_ARRAY_BUFFER_TRANSFER		"image_buffer_transfer"

const static std::string http_image_controller		= std::string("http://") + std::string(HANDLER_IAMGE_CONTROLLER);
const static std::string http_image_buffer_transfer = std::string("http://") + std::string(HANDLER_IAMGE_ARRAY_BUFFER_TRANSFER);


class TimeEllapse
{
public:
	TimeEllapse(std::string name, int count = 100)
		: m_name(name)
		, m_circle_count(count)
		, m_index(1)
		, m_is_printf_once(true)
	{
		begin_time = GetTickCount();
		pre_time = begin_time;
	}
	~TimeEllapse() {}

	void Ellapse()
	{
		if (m_index == 1) {
			begin_time = GetTickCount();
		}
		if (m_index == m_circle_count) {
			end_time = GetTickCount();
			m_index = 1;
			printf("%s ellpase_time :		%ld     %ld			%d\n", m_name.c_str(), end_time, begin_time, end_time - begin_time);
			return;
		}
		if (m_is_printf_once) {
			DWORD current = GetTickCount();
			printf("%s current call  :		%ld     %ld			%d\n", m_name.c_str(), current, pre_time, current - pre_time);
			pre_time = current;
		}
		m_index++;
	}

private:
	int m_circle_count; //
	int m_index;
	DWORD begin_time;
	DWORD end_time;
	DWORD pre_time;
	std::string m_name; 
	bool m_is_printf_once;
};


// Implementation of the schema handler for client:// requests.
class ClientXMLRequestResourceHandler : public CefResourceHandler {
public:
	ClientXMLRequestResourceHandler(CefRefPtr<CefFrame> frame) : offset_(0) , frame_(frame){}

	// ʹ��XMLHttpRequest���н�������3������д�ú��������ڽ���JS��post����
	virtual bool ProcessRequest(CefRefPtr<CefRequest> request,
		CefRefPtr<CefCallback> callback)
		OVERRIDE {
			CEF_REQUIRE_IO_THREAD();
			
			bool handled = false;
			data_ = "";
			
			std::string url = request->GetURL();

			std::vector<std::string> vec_url_elements = SplitString(url, "/");
            std::string method = request->GetMethod();
			// ��web������ʱ��web�˻��ȷ���һ��Option������Ȼ���ٷ���Get�������Ҫ������Option����
            if (method == "OPTIONS")
            {
                handled = true;
                mime_type_ = "application/octet-stream";                
            } 
            else if(url.find(http_image_controller) == 0) 
            {
				if (false) {//ParseImageOperationPostData(request, data_)) {
					//��ȡͼ��ɹ�����Ҫ����handled����ʹ����Ϊtrue
					handled = true;
					// Set the resulting mime type
					mime_type_ = "text";//"image/jpg";					
				} else {
					if (vec_url_elements.size() >= 4 && vec_url_elements[2] == "write_file") {
						static TimeEllapse ellapse("write_file");
						ParseWriteFileOperationPostData(request, data_);
						ellapse.Ellapse();
						//char arraybuffer = 0;
						//if (!DataTransferController::GetInstance()->ParseWriteFileOperationData(
						//	&arraybuffer, data_)) {
						//		//return false;
						//}
					} 
					else if (vec_url_elements.size() >= 4 && vec_url_elements[2] == "read_file") {
						char arraybuffer = 0;
						std::string file_index = vec_url_elements[3];
						static TimeEllapse ellapse("read_file");
						if (!DataTransferController::GetInstance()->ParseReadFileOperationData(
							&arraybuffer, vec_url_elements, data_)) {
								//return false;
						}
						ellapse.Ellapse();
					}
					else if (vec_url_elements.size() >= 3 && vec_url_elements[2] == "clear_files") {
						char arraybuffer = 0;
						static TimeEllapse ellapse("clear_files");
						if (!DataTransferController::GetInstance()->ParseClearFilesOperationData(
							&arraybuffer, vec_url_elements, data_)) {
								//return false;
						}
						ellapse.Ellapse();
					}
					else {
						// ��web������ʱ�����ᷢ��postdata
						char arraybuffer = 0;
						if (!DataTransferController::GetInstance()->ParseDcmOperationData(
							&arraybuffer, data_)) {
								//return false;
						}
					}
					//��ȡͼ��ɹ�����Ҫ����handled����ʹ����Ϊtrue
					handled = true;
					// Set the resulting mime type
					// ֱ�Ӷ�ȡdcm�ļ�����web������,����������Ҫ��application/octet-stream
					mime_type_ = "application/octet-stream";
				}

			} else if(url.find(http_image_buffer_transfer) == 0) {
				if ( HandleArrayBuffrDataFromJS(request, data_)) {
					//��ȡͼ��ɹ�����Ҫ����handled����ʹ����Ϊtrue
					handled = true;
					// Set the resulting mime type
					mime_type_ = "arraybuffer";
				}
			}  

			if (handled) {
				// Indicate the headers are available.
				callback->Continue();
				return true;
			}

			return false;
	};
	bool ParseWriteFileOperationPostData(CefRefPtr<CefRequest> request, std::string& resource_data)
	{
		std::string url = request->GetURL();

		std::vector<std::string> vec_url_elements = SplitString(url, "/");

		CefRefPtr<CefPostData> postData = request->GetPostData();
		if (!postData) {
			return false;
		}
		CefPostData::ElementVector elements;
		postData->GetElements(elements);

		if (elements.size() <= 0) {
			return false;
		}
		std::wstring queryString;
		CefRefPtr<CefPostDataElement> data = elements[0];
		if (data->GetType() != PDE_TYPE_BYTES) {
			return false;
		}
		const unsigned int length = data->GetBytesCount();
		if (length == 0) {
			return false;
		}

		char *arraybuffer = new char[length + 1];
		if (!arraybuffer) {
			return false;
		}

		memset(arraybuffer, 0, length + 1);
		data->GetBytes(length, arraybuffer);

		if (DataTransferController::GetInstance()->ParseWriteFileOperationData(
			arraybuffer, length, vec_url_elements, resource_data)) {
				delete [] arraybuffer;
				arraybuffer = NULL;
				return true;
		}

		delete [] arraybuffer;
		arraybuffer = NULL;

		return false;
	}
	bool ParseImageOperationPostData(CefRefPtr<CefRequest> request, std::string& resource_data)
	{
		CefRefPtr<CefPostData> postData = request->GetPostData();
		if (!postData) {
			return false;
		}
		CefPostData::ElementVector elements;
		postData->GetElements(elements);

		if (elements.size() <= 0) {
			return false;
		}
		std::wstring queryString;
		CefRefPtr<CefPostDataElement> data = elements[0];
		if (data->GetType() != PDE_TYPE_BYTES) {
			return false;
		}
		const unsigned int length = data->GetBytesCount();
		if (length == 0) {
			return false;
		}

		char *arraybuffer = new char[length + 1];
		if (!arraybuffer) {
			return false;
		}

		memset(arraybuffer, 0, length + 1);
		data->GetBytes(length, arraybuffer);

		if (DataTransferController::GetInstance()->ParseImageOperationData(
			arraybuffer, resource_data)) {
				return true;
		}

		return false;
	}

	bool  HandleArrayBuffrDataFromJS(CefRefPtr<CefRequest> request, std::string& resource_data) { 
		CefRefPtr<CefPostData> postData = request->GetPostData();
		if (!postData) {
			return false;
		}

		CefPostData::ElementVector elements;
		postData->GetElements(elements);

		if (elements.size() <= 0) {
			return false;
		}

		std::wstring queryString;
		CefRefPtr<CefPostDataElement> data = elements[0];
		if (data->GetType() != PDE_TYPE_BYTES) {
			return false;
		}

		const unsigned int length = data->GetBytesCount();
		if (length == 0) {
			return false;
		}

		char *arraybuffer = new char[length];
		if (!arraybuffer) {
			return false;
		}

		memset(arraybuffer, 0, length);
		data->GetBytes(length, arraybuffer);
		// �˴���Ҫ��arraybuffer���ݽ���vtk����
		// to do

		resource_data = "";
		return true;
	}

	virtual void GetResponseHeaders(CefRefPtr<CefResponse> response,
		int64& response_length,
		CefString& redirectUrl) OVERRIDE {
			CEF_REQUIRE_IO_THREAD();

			//DCHECK(!data_.empty());

			response->SetMimeType(mime_type_);
			response->SetStatus(200);

			// �����������ok
			CefRequest::HeaderMap headers;
			headers.insert(std::make_pair("Access-Control-Allow-Origin", "*"));

			// ��web������ʱ����Ҫ������µ�Header����-----------------
            // ��Ӧ����
            headers.insert(std::make_pair("Access-Control-Allow-Methods", "*"));
            // ��Ӧͷ����
            headers.insert(std::make_pair("Access-Control-Allow-Headers", "content-type,token,id"));
            headers.insert(std::make_pair("Access-Control-Request-Headers", "Origin, X-Requested-With, content-Type, Accept, Authorization"));
            // ��web������ʱ��Content Length����
            headers.insert(std::make_pair("Content-Length", "15302874"));
			//-------------------------------------------------------------------------

			response->SetHeaderMap(headers);

			// Set the resulting response length
			response_length = data_.length();
	};

	virtual void Cancel() OVERRIDE {
		CEF_REQUIRE_IO_THREAD();
	};

	virtual bool ReadResponse(void* data_out,
		int bytes_to_read,
		int& bytes_read,
		CefRefPtr<CefCallback> callback)
		OVERRIDE 
	{
		CEF_REQUIRE_IO_THREAD();

		bool has_data = false;
  		bytes_read = 0;
  
  		if (data_.length() > 0 && offset_ < data_.length()) {
  			// Copy the next block of data into the buffer.
			int transfer_size = std::min<int>(bytes_to_read, static_cast<int>(data_.length() - offset_));
  			memcpy(data_out, data_.c_str() + offset_, transfer_size);
  			offset_ += transfer_size;
  
  			bytes_read = transfer_size;
  			has_data = true;
  		}

		return has_data;
	};

private:
	std::string data_;
	std::string mime_type_;
	size_t offset_;
	CefRefPtr<CefFrame> frame_;

	IMPLEMENT_REFCOUNTING(ClientXMLRequestResourceHandler);
};



// Implementation of the factory for for creating schema handlers.
class ClientXMLRequestSchemeHandlerFactory : public CefSchemeHandlerFactory {
public:
	// Return a new scheme handler instance to handle the request.
	virtual CefRefPtr<CefResourceHandler> Create(CefRefPtr<CefBrowser> browser,
		CefRefPtr<CefFrame> frame,
		const CefString& scheme_name,
		CefRefPtr<CefRequest> request)
		OVERRIDE 
	{
		CEF_REQUIRE_IO_THREAD();
		// ʹ��XMLHttpRequest���н�������2������Ҫʵ��һ��CefResourceHandler������
		return new ClientXMLRequestResourceHandler(frame);
	}

	IMPLEMENT_REFCOUNTING(ClientXMLRequestSchemeHandlerFactory);
};
