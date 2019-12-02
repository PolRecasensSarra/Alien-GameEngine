#pragma once

#ifdef ALIENENGINESCRIPTS_EXPORTS
#define ALIEN_ENGINE_API __declspec(dllexport)
#else
#define ALIEN_ENGINE_API __declspec(dllimport)
#endif

class GameObject;

extern "C" {

	ALIEN_ENGINE_API const char* LogToConsole();
	ALIEN_ENGINE_API void DeleteGameObject(GameObject* obj);
}