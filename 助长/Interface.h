#pragma once
#include <SFML/Graphics.hpp>

class Interface;

void* NULL_Function(void* param);
void* default_Sleep(void* null);

class Interface
{
public:
	using Function = void* (*)(void*);

	Interface();
	~Interface();

	void set_Init(Function init_Function);
	void set_Draw(Function draw_Function);
	void set_Compute(Function compute_Function);
	void set_Event(Function event_Function);
	void set_Sleep(Function Sleep_Function);

	//Function get_Init_Function();
	//Function get_Event_Function();
	//Function get_Compute_Function();
	//Function get_Draw_Function();
	//Function get_Sleep_Function();

	void init();
	void* event(sf::Event& event_ptr);
	void compute();
	void draw();
	void sleep();

	sf::RenderTexture& get_Texture();

	void set_Next_Dll(const char path[]);
	const char* get_Next_Dll();
	
	bool is_Running();
	void set_Running(bool flag);

	bool load(const char path[]);

private:
	Function self_Init = NULL_Function;
	Function self_Event = NULL_Function;
	Function self_Compute = NULL_Function;
	Function self_Draw = NULL_Function;
	Function self_Sleep = default_Sleep;

	sf::RenderTexture self_Texture;
	void* self_Dll = nullptr;
	bool self_Is_Running = true;
	char* self_Next_Dll = nullptr;
};
