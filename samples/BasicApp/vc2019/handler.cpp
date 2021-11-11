#include "handler.h"
#include "BasicApp.h"

handler::handler()
{
	//ctor
}

handler::handler(utility::string_t url) :m_listener(url)
{
	init_handlers();
}

handler::handler(void* pDC, utility::string_t url) :m_listener(url)
{
	init_handlers();
	set_datacontroller(pDC);
}


handler::~handler()
{
	//dtor
}

void handler::handle_error(pplx::task<void>& t)
{
	try
	{
		t.get();
	}
	catch (...)
	{
		// Ignore the error, Log it if a logger is available
	}
}

void handler::init_handlers()
{
	m_pData = nullptr;
	m_listener.support(methods::GET, std::bind(&handler::handle_get, this, std::placeholders::_1));
	m_listener.support(methods::PUT, std::bind(&handler::handle_put, this, std::placeholders::_1));
	m_listener.support(methods::POST, std::bind(&handler::handle_post, this, std::placeholders::_1));
	m_listener.support(methods::DEL, std::bind(&handler::handle_delete, this, std::placeholders::_1));
}

void handler::set_datacontroller(void* pDC)
{
	BasicApp* pApp = (BasicApp*)pDC;
	if (pApp->MAGIC != U("BASICAPP"))
	{
		m_pData = nullptr;
		return;
	}

	m_pData = pDC;
}

//
// Get Request 
//
void handler::handle_get(http_request message)
{
	ucout << message.to_string() << endl;

	auto paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));
	
	BasicApp* pApp = (BasicApp*)m_pData;

	if (paths.empty()) {
		message.reply(status_codes::OK, pApp->GetPointsJsonArray());
		return;
	}

	string rep = "Hello, rest";
	message.reply(status_codes::OK, rep);
	return;

	message.relative_uri().path();
	//Dbms* d  = new Dbms();
	//d->connect();

	concurrency::streams::fstream::open_istream(U("static/index.html"), std::ios::in).then([=](concurrency::streams::istream is)
	{
		message.reply(status_codes::OK, is, U("text/html"))
			.then([](pplx::task<void> t)
		{
			try {
				t.get();
			}
			catch (...) {
				//
			}
		});
	}).then([=](pplx::task<void>t)
	{
		try {
			t.get();
		}
		catch (...) {
			message.reply(status_codes::InternalError, U("INTERNAL ERROR "));
		}
	});

	return;

};

//
// A POST request
//
void handler::handle_post(http_request message)
{
	ucout << message.to_string() << endl;

	auto paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));

	const json::value data = message.extract_json().get();

	BasicApp* pApp = (BasicApp*)m_pData;
	if (pApp == nullptr || pApp->MAGIC != U("BASICAPP")) {
		message.reply(status_codes::InternalError);
	}

	if (pApp->AddNewPoint(data)) {
		message.reply(status_codes::OK, pApp->GetPointsJsonArray());
		return;
	}

	message.reply(status_codes::BadRequest, message.to_string());
	return;
};

//
// A DELETE request
//
void handler::handle_delete(http_request message)
{
	ucout << message.to_string() << endl;

	string rep = "WRITE YOUR OWN DELETE OPERATION";
	message.reply(status_codes::OK, rep);
	return;
};


//
// A PUT request 
//
void handler::handle_put(http_request message)
{
	ucout << message.to_string() << endl;
	string rep = "WRITE YOUR OWN PUT OPERATION";
	message.reply(status_codes::OK, rep);
	return;
};
