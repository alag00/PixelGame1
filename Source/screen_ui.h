#pragma once
#include "raylib.h"

class ScreenUI
{
private:
public:
	virtual bool Update() { return true; };
	virtual void Render(){};
};