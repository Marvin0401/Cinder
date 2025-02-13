#ifndef HANDLER_H
#define HANDLER_H
#include <iostream>
#include <map>
#include <set>
#include "stdafx.h"
//#include "../dbms/include/Dbms.h"

using namespace std;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;


class handler
{
public:
	handler();
	handler(utility::string_t url);
	handler(void* pDC, utility::string_t url);

	virtual ~handler();

	pplx::task<void>open() { return m_listener.open(); }
	pplx::task<void>close() { return m_listener.close(); }

protected:

private:
	void init_handlers();
	void set_datacontroller(void* pDC);

	void handle_get(http_request message);
	void handle_put(http_request message);
	void handle_post(http_request message);
	void handle_delete(http_request message);
	void handle_error(pplx::task<void>& t);
	

	http_listener m_listener;
	void* m_pData;
};

#endif // HANDLER_H


