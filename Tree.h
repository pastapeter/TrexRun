
#pragma once
#include <Object.h>
#include <Timer.h>

class Tree : public bangtal::Object
{
public:
	static bangtal::ObjectPtr create(const std::string& TreeImage1, const std::string& TreeImage2 = "", bangtal::ScenePtr scene = nullptr, int x = 0, int y = 0, bool choice = true,
		bool shown = true);

protected:
	Tree(const std::string& TreeImage1, const std::string& TreeImage2, bangtal::ScenePtr scene, int x, int y, bool choice, bool shown);

public:
	virtual bool onMouse(int x, int y, MouseAction action);
	~Tree();
protected:
	bool _choice;
	std::string _TreeImage1;
	std::string _TreeImage2;
};
