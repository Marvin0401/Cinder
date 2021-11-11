#pragma once
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cpprest/http_listener.h"

using namespace ci;
using namespace ci::app;
using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

// We'll create a new Cinder Application by deriving from the App class.
class BasicApp : public App {
public:
	BasicApp();
	~BasicApp();
	// Cinder will call 'mouseDrag' when the user moves the mouse while holding one of its buttons.
	// See also: mouseMove, mouseDown, mouseUp and mouseWheel.
	void mouseDrag(MouseEvent event) override;

	// Cinder will call 'keyDown' when the user presses a key on the keyboard.
	// See also: keyUp.
	void keyDown(KeyEvent event) override;

	// Cinder will call 'draw' each time the contents of the window need to be redrawn.
	void draw() override;

	// Get the points array as JSON Array
	web::json::value GetPointsJsonArray();

	bool AddNewPoint(web::json::value value);

	std::wstring MAGIC;
private:
	void InitRestAPIServer(const string_t& address);
	void CloseRestAPIServer();
	// This will maintain a list of points which we will draw line segments between
	std::vector<vec2> mPoints;
};