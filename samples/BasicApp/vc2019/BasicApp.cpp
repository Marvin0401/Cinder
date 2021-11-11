#include "BasicApp.h"
#include "handler.h"
#include "messages.h"


std::unique_ptr<handler> g_httpHandler;


void prepareSettings( BasicApp::Settings* settings )
{
	settings->setMultiTouchEnabled( false );
}

BasicApp::BasicApp()
{
	MAGIC = U("BASICAPP");
	utility::string_t port = U("9899");
	utility::string_t address = U("http://127.0.0.1:");
	address.append(port);
	InitRestAPIServer(address);
}

BasicApp::~BasicApp()
{
	CloseRestAPIServer();
}

void BasicApp::InitRestAPIServer(const string_t& address)
{
	uri_builder uri(address);


	auto addr = uri.to_uri().to_string();
	g_httpHandler = std::unique_ptr<handler>(new handler(this, addr));
	g_httpHandler->open().wait();

	ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;
}

void BasicApp::CloseRestAPIServer()
{
	g_httpHandler->close().wait();
}

void BasicApp::mouseDrag( MouseEvent event )
{
	// Store the current mouse position in the list.
	mPoints.push_back( event.getPos() );
}

void BasicApp::keyDown( KeyEvent event )
{
	if( event.getChar() == 'f' ) {
		// Toggle full screen when the user presses the 'f' key.
		setFullScreen( ! isFullScreen() );
	}
	else if( event.getCode() == KeyEvent::KEY_SPACE ) {
		// Clear the list of points when the user presses the space bar.
		mPoints.clear();
	}
	else if( event.getCode() == KeyEvent::KEY_ESCAPE ) {
		// Exit full screen, or quit the application, when the user presses the ESC key.
		if( isFullScreen() )
			setFullScreen( false );
		else
			quit();
	}
}

void BasicApp::draw()
{
	// Clear the contents of the window. This call will clear
	// both the color and depth buffers. 
	gl::clear( Color::gray( 0.1f ) );

	// Set the current draw color to orange by setting values for
	// red, green and blue directly. Values range from 0 to 1.
	// See also: gl::ScopedColor
	gl::color( 1.0f, 0.5f, 0.25f );

	// We're going to draw a line through all the points in the list
	// using a few convenience functions: 'begin' will tell OpenGL to
	// start constructing a line strip, 'vertex' will add a point to the
	// line strip and 'end' will execute the draw calls on the GPU.
	gl::begin( GL_LINE_STRIP );
	for( const vec2 &point : mPoints ) {
		gl::vertex( point );
	}
	gl::end();
}


web::json::value BasicApp::GetPointsJsonArray()
{
	web::json::value result = web::json::value::array();
	size_t idx = 0;
	for (const vec2& point : mPoints) {
		web::json::value res = web::json::value::object();
		res[POINTX] = web::json::value::number((double)point[0]);
		res[POINTY] = web::json::value::number((double)point[1]);
		result[idx++] = res;
	}
	return result;
}

bool BasicApp::AddNewPoint(web::json::value value)
{
	float fX = value[POINTX].as_integer();
	float fY = value[POINTY].as_integer();

	vec2 newpt = vec2(fX, fY);
	mPoints.push_back(newpt);
	return true;
}

// This line tells Cinder to actually create and run the application.
CINDER_APP( BasicApp, RendererGl, prepareSettings )
