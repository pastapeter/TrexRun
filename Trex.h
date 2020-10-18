#pragma once
#include <Object.h>
#include <Scene.h>
#include <iostream>

using namespace std;

class Trex : public bangtal::Object {
public:
	static bangtal::ObjectPtr create(const string& trexImage1, bangtal::ScenePtr scene = nullptr, int x = 0, int y = 0, bool shown = true);

protected:
	Trex(const string& trexImage1, bangtal::ScenePtr scene, int x, int y, bool shown);
public:
	virtual bool onMouse(int x, int y, MouseAction action);
	
};