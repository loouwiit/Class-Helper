#include "Interface.h"
#include <Windows.h>

using Function = Interface::Function;

void* NULL_Function(void* param)
{
	return nullptr;
}

void* default_Sleep(void* null)
{
	sf::sleep(sf::milliseconds(16)); //62.5 fps
	return nullptr;
}

Interface::Interface()
{
	self_Init = NULL_Function;
	self_Event = NULL_Function;
	self_Compute = NULL_Function;
	self_Draw = NULL_Function;
	self_Sleep = default_Sleep;

	self_Texture.create(1920, 1080);
}

Interface::~Interface()
{

}

void Interface::set_Init(Function init_Function)
{
	if (init_Function != nullptr) self_Init = init_Function;
	else self_Init = NULL_Function;
}

void Interface::set_Event(Function event_Function)
{
	if (event_Function != nullptr) self_Event = event_Function;
	else self_Event = NULL_Function;
}

void Interface::set_Compute(Function compute_Function)
{
	if (compute_Function != nullptr) self_Compute = compute_Function;
	else self_Compute = NULL_Function;
}

void Interface::set_Draw(Function draw_Function)
{
	if (draw_Function != nullptr) self_Draw = draw_Function;
	else self_Draw = NULL_Function;
}

void Interface::set_Sleep(Function Sleep_Function)
{
	if (Sleep_Function != nullptr) self_Sleep = Sleep_Function;
	else self_Sleep = default_Sleep;
}

//Function Interface::get_Init_Function()
//{
//	return self_Init != NULL_Function ? self_Init : nullptr;
//}
//
//Function Interface::get_Event_Function()
//{
//	return self_Event != NULL_Function ? self_Event : nullptr;
//}
//
//Function Interface::get_Compute_Function()
//{
//	return self_Compute != NULL_Function ? self_Compute : nullptr;
//}
//
//Function Interface::get_Draw_Function()
//{
//	return self_Draw != NULL_Function ? self_Draw : nullptr;
//}
//
//Function Interface::get_Sleep_Function()
//{
//	return self_Sleep != NULL_Function ? self_Sleep : nullptr;
//}

void Interface::init()
{
	self_Init(this);
}

void* Interface::event(sf::Event& param)
{
	return self_Event(&param);
}

void Interface::compute()
{
	self_Compute(nullptr);
}

void Interface::draw()
{
	self_Draw(nullptr);
}

void Interface::sleep()
{
	self_Sleep(nullptr);
}

sf::RenderTexture& Interface::get_Texture()
{
	return self_Texture;
}

Interface_Dll::Interface_Dll()
{
	if (self_Dll != nullptr) FreeLibrary((HMODULE)self_Dll);
	self_Dll = nullptr;

	self_Is_Running = true;

	if (self_Next_Dll != nullptr)
	{
		delete[] self_Next_Dll;
		self_Next_Dll = nullptr;
	}
}

Interface_Dll::~Interface_Dll()
{
	if (self_Dll != nullptr) FreeLibrary((HMODULE)self_Dll);
	self_Dll = nullptr;

	self_Is_Running = false;

	if (self_Next_Dll != nullptr)
	{
		delete[] self_Next_Dll;
		self_Next_Dll = nullptr;
	}
}

void Interface_Dll::set_Next_Dll(const char path[])
{
	if (path == nullptr)
	{
		if (self_Next_Dll != nullptr)
		{
			delete[] self_Next_Dll;
			self_Next_Dll = nullptr;
		}
		return;
	}

	if (self_Next_Dll != nullptr)
	{
		delete[] self_Next_Dll;
		self_Next_Dll = nullptr;
	}

	size_t size = strlen(path) + 1;
	self_Next_Dll = new char[size];
	strcpy_s(self_Next_Dll, size, path);
}

const char* Interface_Dll::get_Next_Dll()
{
	return self_Next_Dll;
}

bool Interface_Dll::is_Running()
{
	return self_Is_Running;
}

void Interface_Dll::set_Running(bool flag)
{
	self_Is_Running = flag;
}

bool Interface_Dll::load(const char path[])
{
	if (self_Dll != nullptr) FreeLibrary((HMODULE)self_Dll);
	self_Dll = nullptr;

	if (path == nullptr)
	{

		set_Init(nullptr);
		set_Event(nullptr);
		set_Compute(nullptr);
		set_Draw(nullptr);
		set_Sleep(nullptr);

		self_Is_Running = false;

		return true ;
	}
	
	printf("Interface::load:加载%s:", path);

	self_Dll = LoadLibraryA(path); //勿用'/'，要用'\\'

	if (self_Dll == nullptr) //加载失败
	{
		self_Is_Running = false;
		printf("失败\n");
		return false;
	}

	set_Init((Function)GetProcAddress((HMODULE)self_Dll, "init")); //若加载失败将返回null，由set函数进一步处理为NULL_Function
	set_Event((Function)GetProcAddress((HMODULE)self_Dll, "event"));
	set_Compute((Function)GetProcAddress((HMODULE)self_Dll, "compute"));
	set_Draw((Function)GetProcAddress((HMODULE)self_Dll, "draw"));
	set_Sleep((Function)GetProcAddress((HMODULE)self_Dll, "sleep"));

	self_Is_Running = true;
	printf("成功\n");

	return true;
}
