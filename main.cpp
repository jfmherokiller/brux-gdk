/*==============================================*\
| PROJECT:	XYG Studio Runtime Environment       |
| AUTHOR:		Nick Kovacs                      |
|	DATE:		8-15-15                          |
|	DESCRIPTION:Runtime environment used for     |
|	  games and applications created using       |
|	  the XYG Studio framework.                  |
|	LICENSE:	You are free to use, modify, and |
|	  redistribute this source code, in part,    |
|	  or in full, provided proper attribution    |
|	  is included and this information is not    |
|	  modified or removed. Please include a      |
|	  link to WWW.XYGSTUDIO.ORG in all           |
|	  projects that use this source code.        |
\*==============================================*/

/*===========*\
| MAIN SOURCE |
\*===========*/

//Main header
#include "main.h"
#include "global.h"
#include "input.h"
#include "graphics.h"
#include "shapes.h"
#include "maths.h"
#include "fileio.h"
#include "binds.h"
#include "text.h"
#include "audio.h"


/////////////////
//MAIN FUNCTION//
/////////////////

int main(int argc, char* args[]){
	//Initiate everything
	if(xyInit() == 0){
		xyPrint(0, "Failed to initiate XYG!");
		xyEnd();
		return 1;
	};

	//Process arguments
	string xygapp = "test.nut";
	string curarg = "";
	for(int i = 0; i < argc; i++){
		//Print each argument and process them
		curarg = args[i];
		xyPrint(0, curarg.c_str());
		if(i == 0){
			gvAppDir = curarg.substr(0, curarg.find_last_of("/\\") + 1);
			xyPrint(0, "This is the app directory: %s", gvAppDir.c_str());
		};

		//Input file
		//If the file is long enough
		if(curarg.length() > 4){
			//If the file ends in '.xyg'
			if(curarg.substr(curarg.find_last_of(".")) == ".xyg" || curarg.substr(curarg.find_last_of(".")) == ".sq" || curarg.substr(curarg.find_last_of(".")) == ".nut"){
				//Check that the file really exists
				if(xyFileExists(curarg.c_str())){
					//All checks pass, assign the file
					xygapp = curarg.c_str();
					gvWorkDir = curarg.substr(0, curarg.find_last_of("/\\") + 1);
					xyPrint(0, "This is the working directory: %s", gvWorkDir.c_str());
				};
			};
		};

		//Other arguments
	};

	
	//Run app
	string strCoreLib = gvAppDir;
	strCoreLib += "xylib/core.nut";
	sqstd_dofile(gvSquirrel, strCoreLib.c_str(), 0, 1);
	sqstd_dofile(gvSquirrel, xygapp.c_str(), 0, 1);

	//End game
	xyEnd();

	return 0;
};

//////////////
//EXCEPTIONS//
//////////////



///////////////////
//OTHER FUNCTIONS//
///////////////////

int xyInit(){
	//Initiate log file
	remove("log.txt");
	gvLog = fopen("log.txt", "w");

	//Print opening message
	xyPrint(0, "\n/========================\\\n| XYG STUDIO RUNTIME LOG |\n\\========================/\n\n");
	xyPrint(0, "Initializing program...\n\n");

	//Initiate SDL
	if(SDL_Init(SDL_INIT_EVERYTHING < 0)){
		xyPrint(0, "Failed to initialize! %s", SDL_GetError());
		return 0;
	};

	//Create window
	gvWindow = SDL_CreateWindow("XYG Runtime Environment", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, gvScrW, gvScrH, SDL_WINDOW_RESIZABLE);
	if(gvWindow == 0){
		xyPrint(0, "Window could not be created! SDL Error: %s\n", SDL_GetError());
		return 0;
	} else {
		//Create renderer for window
		gvRender = SDL_CreateRenderer(gvWindow, -1, SDL_RENDERER_ACCELERATED);
		if(gvRender == 0){
			xyPrint(0, "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			return 0;
		} else {
			//Initialize renderer color
			SDL_SetRenderDrawColor(gvRender, 0xFF, 0xFF, 0xFF, 0xFF);

			//Initialize PNG loading
			if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)){
				xyPrint(0, "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
				return 0;
			};

			//Set up the viewport
			SDL_Rect screensize;
			screensize.x = 0;
			screensize.y = 0;
			screensize.w = gvScrW;
			screensize.h = gvScrH;
			SDL_RenderSetViewport(gvRender, &screensize);
			SDL_RenderSetLogicalSize(gvRender, gvScrW, gvScrH);

		};
	};

	//Initialize TTF
	if(TTF_Init() < 0){
		xyError(0, "SDL_ttf could not initialize! SDL_ttf error: %s\n", TTF_GetError());
		return 0;
	};

	//Initialize audio
	if(Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0){
		xyError(0, "SDL_mixer could not initialize! SDL_mixer error: %s\n", Mix_GetError());
		return 0;
	};

	xyInitInput();

	xyPrint(0, "SDL initialized successfully!");

	//Initiate Squirrel
	gvSquirrel = sq_open(1024);

	sqstd_register_mathlib(gvSquirrel);
	sq_setprintfunc(gvSquirrel, xyPrint, xyError);
	sq_pushroottable(gvSquirrel);

	xyBindAllFunctions(gvSquirrel);

	/*Error handler does not seem to print compile-time errors. I haven't
	been able to figure out why, as the same code works in my other apps,
	and is taken from the sq.c example included with Squirrel.*/
	sqstd_seterrorhandlers(gvSquirrel);

	xyPrint(0, "Squirrel initialized successfully!");

	//Initiate other
	vcTextures.push_back(0);
	vcSprites.push_back(0);
	vcSounds.push_back(0);
	vcMusic.push_back(0);

	fprintf(gvLog, "================\n\n");
	printf("================\n\n");

	//Return success
	return 1;
};

void xyError(HSQUIRRELVM v, const SQChar *s, ...){
	va_list args;
	va_start(args, s);
	fprintf(gvLog, "!ERROR! >:: ");
	vfprintf(gvLog, s, args);
	printf("!ERROR! >:: ");
	vprintf(s, args);
	printf("\n\n");
	fprintf(gvLog, "\n\n");
	va_end(args);
};

void xyEnd(){

	fprintf(gvLog, "================\n\n");
	printf("================\n\n");

	//Cleanup all resources
	xyPrint(0, "Cleaning up all resources...");
	for(int i = 0; i < vcTextures.size(); i++){
		xyDeleteImage(i);
	};

	for(int i = 0; i < vcSprites.size(); i++){
		delete vcSprites[i];
	};

	//Close Squirrel
	xyPrint(0, "Closing Squirrel...");
	int garbage = sq_collectgarbage(gvSquirrel);
	xyPrint(0, "Collected %i junk obects.", garbage);
	sq_pop(gvSquirrel, 1);
	sq_close(gvSquirrel);

	//Close SDL
	xyPrint(0, "Closing SDL...");
	SDL_DestroyRenderer(gvRender);
	SDL_DestroyWindow(gvWindow);
	IMG_Quit();
	TTF_Quit();
	Mix_Quit();
	SDL_Quit();

	//Close log file
	xyPrint(0, "System closed successfully!");
	fclose(gvLog);
};

void xyPrint(HSQUIRRELVM v, const SQChar *s, ...){
	va_list args;
	va_start(args, s);
	fprintf(gvLog, ">:: ");
	vfprintf(gvLog, s, args);
	printf(">:: ");
	vprintf(s, args);
	printf("\n\n");
	fprintf(gvLog, "\n\n");
	va_end(args);
};

void xyBindFunc(HSQUIRRELVM v, SQFUNCTION func, const SQChar *key){
	sq_pushroottable(v);
	sq_pushstring(v, key, -1);
	sq_newclosure(v, func, 0);
	sq_newslot(v, -3, false);
	sq_pop(v, 1);
};

void xyBindFunc(HSQUIRRELVM v, SQFUNCTION func, const SQChar *key, SQInteger nParams, const SQChar* sParams){
	sq_pushroottable(v);
	sq_pushstring(v, key, -1);
	sq_newclosure(v, func, 0);
	sq_setparamscheck(v, nParams, sParams);
	sq_newslot(v, -3, false);
	sq_pop(v, 1);
};

void xyBindAllFunctions(HSQUIRRELVM v){
	//Main
	xyPrint(0, "Embedding main...");
	xyBindFunc(v, sqUpdate, "update");
	xyBindFunc(v, sqGetOS, "getOS");
	xyBindFunc(v, sqGetTicks, "getTicks");
	xyBindFunc(v, sqImport, "import", 2, ".s");

	//Graphics
	xyPrint(0, "Embedding graphics...");
	xyBindFunc(v, sqWait, "wait", 2, ".n");
	xyBindFunc(v, sqSetDrawTarget, "setDrawTarget", 2, ".n");
	xyBindFunc(v, sqClearScreen, "clearScreen");
	xyBindFunc(v, sqResetDrawTarget, "resetDrawTarget");
	xyBindFunc(v, sqDrawImage, "drawImage", 4, ".nnn");
	xyBindFunc(v, sqSetDrawColor, "setDrawColor", 2, ".n");
	xyBindFunc(v, sqUpdateScreen, "updateScreen");
	xyBindFunc(v, sqLoadImage, "loadImage", 2, ".s");
	xyBindFunc(v, sqLoadImageKeyed, "loadImageKey", 3, ".sn");
	xyBindFunc(v, sqDrawImage, "drawImage", 4, ".inn");
	xyBindFunc(v, sqSetBackgroundColor, "setBackgroundColor", 2, ".n");
	xyBindFunc(v, sqSetScalingFilter, "setScalingFilter", 2, ".n|b");

	//Sprites
	xyPrint(0, "Embedding sprites...");
	xyBindFunc(v, sqNewSprite, "newSprite", 9, ".siiiiiii");
	xyBindFunc(v, sqDrawSprite, "drawSprite", 5, ".innn");
	xyBindFunc(v, sqDrawSpriteEx, "drawSpriteEx", 10, ".innnninnn");

	//Input
	xyPrint(0, "Embedding input...");
	xyBindFunc(v, sqUpdateInput, "updateInput");
	xyBindFunc(v, sqKeyPress, "keyPress", 2, ".n");
	xyBindFunc(v, sqKeyRelease, "keyRelease", 2, ".n");
	xyBindFunc(v, sqKeyDown, "keyDown", 2, ".n");
	xyBindFunc(v, sqMouseX, "mouseX", 1, ".");
	xyBindFunc(v, sqMouseY, "mouseY", 1, ".");

	//Maths
	xyPrint(0, "Embedding maths...");
	xyBindFunc(v, sqRandomFloat, "randFloat", 2, ".n");
	xyBindFunc(v, sqRandomInt, "randInt", 2, ".n");
	xyBindFunc(v, sqDistance2, "distance2", 5, ".nnnn");
	xyBindFunc(v, sqWrap, "wrap", 4, ".nnn");
	xyBindFunc(v, sqFloor, "floor", 2, ".n");
	xyBindFunc(v, sqPointAngle, "pointAngle", 5, ".nnnn");

	//Text
	xyBindFunc(v, sqDrawText, "drawText", 8, ".nnsnnnn");
	xyBindFunc(v, sqOpenFont, "openFont", 3, ".sn");
	xyBindFunc(v, sqCloseFont, "closeFont", 2, ".n");

	//File IOxyPrint(0, "Embedding file I/O...");
	xyBindFunc(v, sqFileExists, "fileExists", 2, ".s");
};

int xyGetOS(){
#ifdef _OS_WINDOWS_
	return 0;
#endif
#ifdef _OS_MAC_
	return 1;
#endif
#ifdef _OS_LINUX_
	return 2;
#endif
#ifdef _OS_PANDORA_
	return 3;
#endif
#ifdef _OS_GCWZ_
	return 4;
#endif
#ifdef _OS_ANDROID_
	return 5;
#endif
#ifdef _OS_IOS_
	return 6;
#endif
};