// Copyright 2006-2016 Coppelia Robotics GmbH. All rights reserved. 
// marc@coppeliarobotics.com
// www.coppeliarobotics.com
// 
// -------------------------------------------------------------------
// THIS FILE IS DISTRIBUTED "AS IS", WITHOUT ANY EXPRESS OR IMPLIED
// WARRANTY. THE USER WILL USE IT AT HIS/HER OWN RISK. THE ORIGINAL
// AUTHORS AND COPPELIA ROBOTICS GMBH WILL NOT BE LIABLE FOR DATA LOSS,
// DAMAGES, LOSS OF PROFITS OR ANY OTHER KIND OF LOSS WHILE USING OR
// MISUSING THIS SOFTWARE.
// 
// You are free to use/modify/distribute this file for whatever purpose!
// -------------------------------------------------------------------
//
// This file was automatically created for V-REP release V3.3.0 on February 19th 2016

#include "v_repExtSDL2.h"

#include <SDL2/SDL.h>

#include <iostream>
#include <vector>

#include "luaFunctionData.h"
#include "v_repLib.h"

// have a look what is really needed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdint.h>
#include <inttypes.h>
#include <stdbool.h>
#include <conio.h>

#ifdef _WIN32
#ifdef QT_COMPIL
#include <direct.h>
#else
#include <shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#endif
#endif /* _WIN32 */
#if defined (__linux) || defined (__APPLE__)
#include <unistd.h>
#endif /* __linux || __APPLE__ */

#ifdef __APPLE__
#define _stricmp strcmp
#endif

#define CONCAT(x,y,z) x y z
#define strConCat(x,y,z)	CONCAT(x,y,z)

#define PLUGIN_VERSION 2 // 2 since version 3.2.1

LIBRARY vrepLib; // the V-REP library that we will dynamically load and bind
// --------------------------------------------------------------------------------------
// Functions of OpenAL
// --------------------------------------------------------------------------------------

class HapticJoystick
{
private:
	//float _deadzoneX;
	//float _deadzoneY;
	//float _deadzoneZ;
	//float _deadzoneT;
	//SDL_Event e;

public:
	//HapticJoystick() : _deadzoneX(0.9f), _deadzoneY(0.01f), _deadzoneZ(0.01f), _deadzoneT(0.01f)  {} //verstehen!!
	//HapticJoystick(float dzX, float dzY, float dzZ, float dzT) : _deadzoneX(dzX), _deadzoneY(dzY), _deadzoneZ(dzX), _deadzoneT(dzY) {} //verstehen!!


	bool init_sdl(bool USE_HAPTIC);
	bool quit_sdl();
	bool refresh();
	bool isPressed(int button);
	int hatPosition();
	SDL_Joystick *joy = NULL;
	SDL_Haptic *haptic;
	float StickX = - 99999;
	float StickY = - 99999;
	float StickZ = -99999;
	float StickT= -99999;
};

bool HapticJoystick::init_sdl(bool USE_HAPTIC)
{
	SDL_ClearError();
	if (SDL_Init(SDL_INIT_JOYSTICK) != 0) {
		fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
		atexit(SDL_Quit);
		return (false);
	};
	if (SDL_NumJoysticks() > 0) {
		joy = SDL_JoystickOpen(0);
		if (joy!=NULL) {
			printf("Open Joystick 0\n");
			printf("Name: %s\n", SDL_JoystickNameForIndex(0));
			printf("Number of Axes: %d\n", SDL_JoystickNumAxes(joy));
			printf("Number of Buttons: %d\n", SDL_JoystickNumButtons(joy));
			printf("Number of Balls: %d\n", SDL_JoystickNumBalls(joy));
			printf("Number of Hats: %d\n", SDL_JoystickNumHats(joy));
		}
		else {
			printf("Could not open Joystick 0\n", SDL_GetError());
			return(false);
		}
	}
	// see if user wants to init haptic and if joystick is capable of haptic
	if (SDL_JoystickIsHaptic(joy) == 1) {
		if (USE_HAPTIC) {
			if (SDL_InitSubSystem(SDL_INIT_HAPTIC) != 0) {
				fprintf(stderr, "Unable to initialize Haptic System: %s\n", SDL_GetError());
				atexit(SDL_Quit);
				return (false);
			}else{
				printf("Haptic system initialized \n");
			};
		};
	}
	else {
		printf("Joystick is not haptic \n");
		return(false);
	};
	haptic = SDL_HapticOpenFromJoystick(joy);
	if (haptic == NULL) {
		fprintf(stderr, "Unable to initialize Haptic Joystick: %s\n", SDL_GetError());
		return(false);
	}
	return(true);
};

bool HapticJoystick::quit_sdl()
{
	if (haptic != NULL) {
		SDL_HapticClose(haptic);
		haptic = NULL;
		printf("Haptic closed \n");
	}

	if (SDL_JoystickGetAttached(joy)) {
		SDL_JoystickClose(joy);
		joy = NULL;
		printf("Joystick closed \n");
	}

	SDL_Quit();
	printf("SDL closed \n");
	return(true);
}
bool HapticJoystick::refresh()
{
	SDL_ClearError();
	SDL_JoystickUpdate();
	if (joy ==  NULL) {
		fprintf(stderr, "Unable to access joystick: %s\n", SDL_GetError());
		return (false);
	}else{
		//printf("Number of Axes: %d\n", SDL_JoystickGetHat(joy,0));
		//float normX = fmaxf(-1, SDL_JoystickGetAxis(joy, 0) / 32767);
		//float normY = fmaxf(-1, SDL_JoystickGetAxis(joy, 1) / 32767);
		//float normZ = fmaxf(-1, SDL_JoystickGetAxis(joy, 2) / 32767);
		//float normT = fmaxf(-1, SDL_JoystickGetAxis(joy, 3) / 32767);
		//StickX = (abs(normX) < _deadzoneX ? 0 : (abs(normX) - _deadzoneX) * (normX / abs(normX)));
		//StickY = (abs(normY) < _deadzoneY ? 0 : (abs(normY) - _deadzoneY) * (normY / abs(normY)));
		//StickZ = (abs(normZ) < _deadzoneX ? 0 : (abs(normZ) - _deadzoneX) * (normZ / abs(normZ)));
		//StickT = (abs(normT) < _deadzoneY ? 0 : (abs(normT) - _deadzoneY) * (normT / abs(normT)));
		//if (_deadzoneX > 0) StickX *= 1 / (1 - _deadzoneX);
		//if (_deadzoneY > 0) StickY *= 1 / (1 - _deadzoneY);
		//if (_deadzoneZ > 0) StickZ *= 1 / (1 - _deadzoneX);
		//if (_deadzoneT > 0) StickT *= 1 / (1 - _deadzoneY);
		StickX = SDL_JoystickGetAxis(joy, 0);
		StickY = SDL_JoystickGetAxis(joy, 1);
		StickZ = SDL_JoystickGetAxis(joy, 2);
		StickT = SDL_JoystickGetAxis(joy, 3);
		return true;
	}
	return false;
}

bool HapticJoystick::isPressed(int button)
{
	SDL_ClearError();
	SDL_JoystickUpdate();
	if (joy == NULL) {
		fprintf(stderr, "Unable to access joystick: %s\n", SDL_GetError());
		return (false);
	}
	return(SDL_JoystickGetButton(joy, button));
}

int HapticJoystick::hatPosition()
{

	SDL_ClearError();
	SDL_JoystickUpdate();
	if (joy == NULL) {
		fprintf(stderr, "Unable to access joystick: %s\n", SDL_GetError());
		return (-99999);
	}
	else {

		return(SDL_JoystickGetHat(joy, 0));
	}
}



HapticJoystick joystick;

#define LUA_INIT_SDL_COMMAND "simExtSDL_init"

const int inArgs_INIT_SDL[] = {
	1,
	sim_lua_arg_bool, 1,
};

void LUA_INIT_SDL_CALLBACK(SLuaCallBack *p)
{
	p->outputArgCount = 0;
	CLuaFunctionData D;
	bool res = false;
	bool useOfHaptic = false;
	if (D.readDataFromLua(p, inArgs_INIT_SDL, inArgs_INIT_SDL[0], LUA_INIT_SDL_COMMAND))
	{
		std::vector<CLuaFunctionDataItem>* inData = D.getInDataPtr();
		useOfHaptic = inData->at(0).boolData[0]; // the first argument
		res = joystick.init_sdl(useOfHaptic);
	}

	D.pushOutData(CLuaFunctionDataItem(res));
	D.writeDataToLua(p);
};

#define LUA_QUIT_SDL_COMMAND "simExtSDL_quit"

const int inArgs_QUIT_SDL[] = {
	0,
};

void LUA_QUIT_SDL_CALLBACK(SLuaCallBack *p)
{
	p->outputArgCount = 0;
	CLuaFunctionData D;
	bool res = false;
	if (D.readDataFromLua(p, inArgs_QUIT_SDL, inArgs_QUIT_SDL[0], LUA_QUIT_SDL_COMMAND))
	{
		// no input to work with
	}
	res = joystick.quit_sdl();
	D.pushOutData(CLuaFunctionDataItem(res));
	D.writeDataToLua(p);
};

#define LUA_REFRESH_STATE_COMMAND "simExtSDL_refreshState"

const int inArgs_REFRESH_STATE[] = {
	0,
};

void LUA_REFRESH_STATE_CALLBACK(SLuaCallBack* p)
{
	p->outputArgCount = 1;
	CLuaFunctionData D;

	if (D.readDataFromLua(p, inArgs_REFRESH_STATE, inArgs_REFRESH_STATE[0], LUA_REFRESH_STATE_COMMAND))
	{
		// no inArgs to work with
	}

	bool refreshStateResult = joystick.refresh();

	D.pushOutData(CLuaFunctionDataItem(refreshStateResult));

	D.writeDataToLua(p);
}

#define LUA_GET_JOYSTICK_COORDS_COMMAND "simExtSDL_getJoystickCoords"

const int inArgs_GET_JOYSTICK_COORDS[] = {
	0,
};

void LUA_GET_JOYSTICK_COORDS_CALLBACK(SLuaCallBack* p)
{
	p->outputArgCount = 1;
	CLuaFunctionData D;

	if (D.readDataFromLua(p, inArgs_GET_JOYSTICK_COORDS, inArgs_GET_JOYSTICK_COORDS[0], LUA_GET_JOYSTICK_COORDS_COMMAND))
	{
		// no inArgs to work with
	}

	D.pushOutData(CLuaFunctionDataItem(std::vector<float>({ joystick.StickX, joystick.StickY, joystick.StickZ, joystick.StickT })));

	D.writeDataToLua(p);
}

#define LUA_IS_BUTTON_PRESSED_COMMAND "simExtSDL_isButtonPressed"

const int inArgs_IS_BUTTON_PRESSED[] = {
	1,
	sim_lua_arg_int, 1,
};

void LUA_IS_BUTTON_PRESSED_CALLBACK(SLuaCallBack* p)
{
	p->outputArgCount = 1;
	CLuaFunctionData D;
	bool buttonState = false;
	if (D.readDataFromLua(p, inArgs_IS_BUTTON_PRESSED, inArgs_IS_BUTTON_PRESSED[0], LUA_IS_BUTTON_PRESSED_COMMAND))
	{
		std::vector<CLuaFunctionDataItem>* inData = D.getInDataPtr();
		int button = inData->at(0).intData[0]; // the first argument
		buttonState = joystick.isPressed(button);
	}
	D.pushOutData(CLuaFunctionDataItem(buttonState));
	D.writeDataToLua(p);
}

#define LUA_HAT_POSITION_COMMAND "simExtSDL_hatPosition"

const int inArgs_HAT_POSITION[] = {
	0,
};

void LUA_HAT_POSITION_CALLBACK(SLuaCallBack* p)
{
	p->outputArgCount = 1;
	CLuaFunctionData D;
	if (D.readDataFromLua(p, inArgs_HAT_POSITION, inArgs_HAT_POSITION[0], LUA_HAT_POSITION_COMMAND))
	{
		// no input
	}
	D.pushOutData(CLuaFunctionDataItem(joystick.hatPosition()));
	D.writeDataToLua(p);
}

#define LUA_GETSENSORDATA_COMMAND "simExtSkeleton_getSensorData" // the name of the new Lua command

const int inArgs_GETSENSORDATA[] = { // Decide what kind of arguments we need
	3, // we want 3 input arguments
	sim_lua_arg_int,0, // first argument is an integer
	sim_lua_arg_float | sim_lua_arg_table,3, // second argument should be a table of at least 3 float values (use 0 instead of 3 for a table of random size)
	sim_lua_arg_int | sim_lua_arg_table,2, // third argument should be a table of at least 2 integer values (use 0 instead of 2 for a table of random size)
};

void LUA_GETSENSORDATA_CALLBACK(SLuaCallBack* p)
{ // the callback function of the new Lua command ("simExtSkeleton_getSensorData")
	p->outputArgCount = 0;
	CLuaFunctionData D;
	// If successful the command will return an interger (result), a float table of size 3 (data), and a float (distance). If the command is not successful, it will not return anything
	bool commandWasSuccessful = false;
	int returnResult;
	std::vector<float> returnData;
	float returnDistance;
	if (D.readDataFromLua(p, inArgs_GETSENSORDATA, inArgs_GETSENSORDATA[0], LUA_GETSENSORDATA_COMMAND))
	{ // above function reads in the expected arguments. If the arguments are wrong, it returns false and outputs a message to the simulation status bar
		std::vector<CLuaFunctionDataItem>* inData = D.getInDataPtr();

		int sensorIndex = inData->at(0).intData[0]; // the first argument
		std::vector<float>& floatParameters = inData->at(1).floatData; // the second argument
		std::vector<int>& intParameters = inData->at(2).intData; // the third argument

		// Now you can do something with above's arguments. For example:
		if ((sensorIndex >= 0) && (sensorIndex < 10))
		{
			commandWasSuccessful = true;
			returnResult = 1;
			returnData.push_back(1.0f);
			returnData.push_back(2.0f);
			returnData.push_back(3.0f);
			returnDistance = 59.0f;
		}
		else
			simSetLastError(LUA_GETSENSORDATA_COMMAND, "Invalid sensor index."); // output an error message to the simulator's status bar
	}
	if (commandWasSuccessful)
	{ // prepare the return values:
		D.pushOutData(CLuaFunctionDataItem(returnResult));
		D.pushOutData(CLuaFunctionDataItem(returnData));
		D.pushOutData(CLuaFunctionDataItem(returnDistance));
	}
	D.writeDataToLua(p);
}
// --------------------------------------------------------------------------------------


// This is the plugin start routine (called just once, just after the plugin was loaded):
VREP_DLLEXPORT unsigned char v_repStart(void* reservedPointer, int reservedInt)
{
	// Dynamically load and bind V-REP functions:
	// ******************************************
	// 1. Figure out this plugin's directory:
	char curDirAndFile[1024];
#ifdef _WIN32
#ifdef QT_COMPIL
	_getcwd(curDirAndFile, sizeof(curDirAndFile));
#else
	GetModuleFileName(NULL, curDirAndFile, 1023);
	PathRemoveFileSpec(curDirAndFile);
#endif
#elif defined (__linux) || defined (__APPLE__)
	getcwd(curDirAndFile, sizeof(curDirAndFile));
#endif

	std::string currentDirAndPath(curDirAndFile);
	// 2. Append the V-REP library's name:
	std::string temp(currentDirAndPath);
#ifdef _WIN32
	temp += "\\v_rep.dll";
#elif defined (__linux)
	temp += "/libv_rep.so";
#elif defined (__APPLE__)
	temp += "/libv_rep.dylib";
#endif /* __linux || __APPLE__ */
	// 3. Load the V-REP library:
	vrepLib = loadVrepLibrary(temp.c_str());
	if (vrepLib == NULL)
	{
		std::cout << "Error, could not find or correctly load the V-REP library. Cannot start 'PluginSkeleton' plugin.\n";
		return(0); // Means error, V-REP will unload this plugin
	}
	if (getVrepProcAddresses(vrepLib) == 0)
	{
		std::cout << "Error, could not find all required functions in the V-REP library. Cannot start 'PluginSkeleton' plugin.\n";
		unloadVrepLibrary(vrepLib);
		return(0); // Means error, V-REP will unload this plugin
	}
	// ******************************************

	// Check the version of V-REP:
	// ******************************************
	int vrepVer;
	simGetIntegerParameter(sim_intparam_program_version, &vrepVer);
	if (vrepVer < 30200) // if V-REP version is smaller than 3.02.00
	{
		std::cout << "Sorry, your V-REP copy is somewhat old. Cannot start 'PluginSkeleton' plugin.\n";
		unloadVrepLibrary(vrepLib);
		return(0); // Means error, V-REP will unload this plugin
	}
	// ******************************************

	std::vector<int> inArgs;

	// Register the new Lua command "simExtSkeleton_getSensorData":
	CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_GETSENSORDATA, inArgs);
	simRegisterCustomLuaFunction(LUA_GETSENSORDATA_COMMAND, strConCat("number result,table data,number distance=", LUA_GETSENSORDATA_COMMAND, "(number sensorIndex,table_3 floatParameters,table_2 intParameters)"), &inArgs[0], LUA_GETSENSORDATA_CALLBACK);

	CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_INIT_SDL, inArgs);
	simRegisterCustomLuaFunction(LUA_INIT_SDL_COMMAND, strConCat("boolean status=", LUA_INIT_SDL_COMMAND, "(boolean Haptic Initialization)"), &inArgs[0], LUA_INIT_SDL_CALLBACK);

	CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_QUIT_SDL, inArgs);
	simRegisterCustomLuaFunction(LUA_QUIT_SDL_COMMAND, strConCat("boolean status=", LUA_QUIT_SDL_COMMAND, "()"), &inArgs[0], LUA_QUIT_SDL_CALLBACK);
	
	CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_REFRESH_STATE, inArgs);
	simRegisterCustomLuaFunction(LUA_REFRESH_STATE_COMMAND,
		strConCat("boolean refreshState=", LUA_REFRESH_STATE_COMMAND, "()"), &inArgs[0], LUA_REFRESH_STATE_CALLBACK);

	CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_GET_JOYSTICK_COORDS, inArgs);
	simRegisterCustomLuaFunction(LUA_GET_JOYSTICK_COORDS_COMMAND,
		strConCat("table_4 leftThumbStickCoords=", LUA_GET_JOYSTICK_COORDS_COMMAND, "()"), &inArgs[0], LUA_GET_JOYSTICK_COORDS_CALLBACK);
	
	CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_IS_BUTTON_PRESSED, inArgs);
	simRegisterCustomLuaFunction(LUA_IS_BUTTON_PRESSED_COMMAND,
		strConCat("boolean isButtonPressed=", LUA_IS_BUTTON_PRESSED_COMMAND, "(int Button)"), &inArgs[0], LUA_IS_BUTTON_PRESSED_CALLBACK);

	CLuaFunctionData::getInputDataForFunctionRegistration(inArgs_HAT_POSITION, inArgs);
	simRegisterCustomLuaFunction(LUA_HAT_POSITION_COMMAND,
		strConCat("integer hatPosition=", LUA_HAT_POSITION_COMMAND, "()"), &inArgs[0], LUA_HAT_POSITION_CALLBACK);

}

// This is the plugin end routine (called just once, when V-REP is ending, i.e. releasing this plugin):
VREP_DLLEXPORT void v_repEnd()
{
	// Here you could handle various clean-up tasks

	unloadVrepLibrary(vrepLib); // release the library
}

// This is the plugin messaging routine (i.e. V-REP calls this function very often, with various messages):
VREP_DLLEXPORT void* v_repMessage(int message, int* auxiliaryData, void* customData, int* replyData)
{ // This is called quite often. Just watch out for messages/events you want to handle
	// Keep following 5 lines at the beginning and unchanged:
	static bool refreshDlgFlag = true;
	int errorModeSaved;
	simGetIntegerParameter(sim_intparam_error_report_mode, &errorModeSaved);
	simSetIntegerParameter(sim_intparam_error_report_mode, sim_api_errormessage_ignore);
	void* retVal = NULL;

	// Here we can intercept many messages from V-REP (actually callbacks). Only the most important messages are listed here.
	// For a complete list of messages that you can intercept/react with, search for "sim_message_eventcallback"-type constants
	// in the V-REP user manual.

	if (message == sim_message_eventcallback_refreshdialogs)
		refreshDlgFlag = true; // V-REP dialogs were refreshed. Maybe a good idea to refresh this plugin's dialog too

	if (message == sim_message_eventcallback_menuitemselected)
	{ // A custom menu bar entry was selected..
		// here you could make a plugin's main dialog visible/invisible
	}

	if (message == sim_message_eventcallback_instancepass)
	{	// This message is sent each time the scene was rendered (well, shortly after) (very often)
		// It is important to always correctly react to events in V-REP. This message is the most convenient way to do so:

		int flags = auxiliaryData[0];
		bool sceneContentChanged = ((flags&(1 + 2 + 4 + 8 + 16 + 32 + 64 + 256)) != 0); // object erased, created, model or scene loaded, und/redo called, instance switched, or object scaled since last sim_message_eventcallback_instancepass message 
		bool instanceSwitched = ((flags & 64) != 0);

		if (instanceSwitched)
		{
			// React to an instance switch here!!
		}

		if (sceneContentChanged)
		{ // we actualize plugin objects for changes in the scene

			//...

			refreshDlgFlag = true; // always a good idea to trigger a refresh of this plugin's dialog here
		}
	}

	if (message == sim_message_eventcallback_mainscriptabouttobecalled)
	{ // The main script is about to be run (only called while a simulation is running (and not paused!))

	}

	if (message == sim_message_eventcallback_simulationabouttostart)
	{ // Simulation is about to start

	}

	if (message == sim_message_eventcallback_simulationended)
	{ // Simulation just ended

	}

	if (message == sim_message_eventcallback_moduleopen)
	{ // A script called simOpenModule (by default the main script). Is only called during simulation.
		if ((customData == NULL) || (_stricmp("PluginSkeleton", (char*)customData) == 0)) // is the command also meant for this plugin?
		{
			// we arrive here only at the beginning of a simulation
		}
	}

	if (message == sim_message_eventcallback_modulehandle)
	{ // A script called simHandleModule (by default the main script). Is only called during simulation.
		if ((customData == NULL) || (_stricmp("PluginSkeleton", (char*)customData) == 0)) // is the command also meant for this plugin?
		{
			// we arrive here only while a simulation is running
		}
	}

	if (message == sim_message_eventcallback_moduleclose)
	{ // A script called simCloseModule (by default the main script). Is only called during simulation.
		if ((customData == NULL) || (_stricmp("PluginSkeleton", (char*)customData) == 0)) // is the command also meant for this plugin?
		{
			// we arrive here only at the end of a simulation
		}
	}

	if (message == sim_message_eventcallback_instanceswitch)
	{ // We switched to a different scene. Such a switch can only happen while simulation is not running

	}

	if (message == sim_message_eventcallback_broadcast)
	{ // Here we have a plugin that is broadcasting data (the broadcaster will also receive this data!)

	}

	if (message == sim_message_eventcallback_scenesave)
	{ // The scene is about to be saved. If required do some processing here (e.g. add custom scene data to be serialized with the scene)

	}

	// You can add many more messages to handle here

	if ((message == sim_message_eventcallback_guipass) && refreshDlgFlag)
	{ // handle refresh of the plugin's dialogs
		// ...
		refreshDlgFlag = false;
	}

	// Keep following unchanged:
	simSetIntegerParameter(sim_intparam_error_report_mode, errorModeSaved); // restore previous settings
	return(retVal);
}