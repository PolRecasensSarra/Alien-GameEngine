#include "Time.h"
#include "Timer.h"
#include "Application.h"
#include "ModuleObjects.h"

Time::GameState Time::state = Time::GameState::NONE;
float Time::time_since_start = 0.0F;
float Time::game_time = 0.0F;
float Time::delta_time = 0.0F;
float Time::scale_time = 1.0F;
Timer* Time::start_timer = new Timer();
Timer* Time::game_timer = new Timer();


void Time::Start()
{
	start_timer->Start();
}

void Time::Update()
{
	time_since_start = start_timer->ReadSec();

	if (state == GameState::PLAY || state == GameState::PLAY_ONCE) {
		game_time = game_timer->ReadSec(scale_time);
	}
}

void Time::Play()
{
	if (state == GameState::NONE) {
		App->objects->SaveScene("Library/play_scene.alienScene", false);
		App->objects->ignore_cntrlZ = true;
		state = GameState::PLAY;
		game_time = 0.0F;
		game_timer->Start();
	}
	else if (state == GameState::PAUSE) {
		state = GameState::PLAY;
		game_timer->Resume();
	}
	else if (state == GameState::PLAY) {
		state = GameState::NONE;
		game_time = 0.0F;
		App->objects->LoadScene("Library/play_scene.alienScene", false);
		App->objects->ignore_cntrlZ = false;
		remove("Library/play_scene.alienScene");
	}
}

void Time::Pause()
{
	if (state == GameState::PAUSE) {
		Time::Play();
	}
	else if (state == GameState::PLAY || state == GameState::PLAY_ONCE) {
		state = GameState::PAUSE;
		game_timer->Pause();
	}
}

void Time::PlayOnce()
{
	if (state == GameState::PAUSE) {
		game_timer->Resume();
		state = GameState::PLAY_ONCE;
	}
	else if (state == GameState::PLAY) {
		state = GameState::PLAY_ONCE;
	}
}

void Time::CleanUp()
{
	if (start_timer != nullptr)
		delete start_timer;
	if (game_timer != nullptr)
		delete game_timer;
}

void Time::SetScaleTime(const float& scale)
{
	scale_time = scale;
}

void Time::SetDT(const float& dt)
{
	delta_time = dt;
}

float Time::GetDT()
{
	return delta_time * scale_time;
}

bool Time::IsPlaying()
{
	if (state == GameState::PLAY || state == GameState::PLAY_ONCE)
		return true;
	else
		return false;
}

bool Time::IsInGameState()
{
	return state != GameState::NONE;
}

void Time::Stop()
{
	game_time = 0.0F;
	state = GameState::NONE;
}
