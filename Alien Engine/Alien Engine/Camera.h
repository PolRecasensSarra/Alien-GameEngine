#pragma once
#include <vector>

class ComponentCamera;

typedef unsigned int uint;

class __declspec(dllexport) Camera {
public:

	static std::vector<ComponentCamera*> GetAllCameras();
	static ComponentCamera* GetCurrentCamera();
	static uint GetNumCameras();
};