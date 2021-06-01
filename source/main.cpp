#include <stdio.h>
#include <nds.h>
#include <nf_lib.h>


#include <time.h>
#include <string>

tm* getTime()
{
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);


	return timeinfo;
}

const char* days[] = 
{
	"Sun",
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat"
};



int main(int argc, char **argv)
{
	 
	// Turn on MODE 0 on the Top Screen
	NF_Set2D(0, 0);
	NF_Set2D(1, 0);

	consoleDemoInit();

	soundEnable();
	
	// Set the Root Folder
	NF_SetRootFolder("NITROFS");
 
	// Initialize the Tiled Backgrounds System on the Top Screen
	NF_InitTiledBgBuffers();
	NF_InitTiledBgSys(0);
	NF_InitTiledBgSys(1);


	NF_InitRawSoundBuffers();


	NF_InitTextSys(0);
	NF_InitTextSys(1);
 
	// Load the Tiled Background
	NF_LoadTiledBg("bg/GrassBackGround", "ground", 256, 256);
	NF_LoadTiledBg("bg/DolphinBackGround", "DolphinBackGround", 256, 256);

	NF_LoadTextFont16("fnt/font16", "normal", 256, 256, 0);
	NF_LoadTextFont16("fnt/font16", "normal1", 256, 256, 0);
	NF_LoadTextFont16("fnt/font16", "bnormal", 256, 256, 0);

	NF_LoadRawSound("sfx/music", 1, 22050, 0);

	NF_CreateTiledBg(0, 3, "ground");
	//NF_CreateTiledBg(1, 3, "ground");

	NF_CreateTextLayer16(0, 0, 0, "normal");
	NF_CreateTextLayer16(0, 1, 0, "normal1");
	NF_CreateTextLayer16(1, 1, 0, "bnormal");

	NF_DefineTextColor(1, 1, 1, 31, 0, 0);		// Bottom Screen layer 1 Red id 1
	NF_DefineTextColor(0, 0, 0, 0, 0, 31);

	NF_SetTextColor(1, 1, 1);

	NF_WriteText16(1, 1, 0, 0, "Press A to Pause Sound");
	NF_WriteText16(1, 1, 0, 1, "Press B to Resume Sound");


	NF_UpdateTextLayers();


	char timetext[32];
	char daydatetext[32];

	int sound_id = NF_PlayRawSound(1, 127, 64, true, 0);

	std::string timetextFormat1 = "%02u : %02u";
	std::string timetextFormat2 = "%02u   %02u";
	std::string currenttimetextFormat = timetextFormat1;

	bool isFormat1 = true;

	float animTime = 0;

	while(1)
	{
		scanKeys();

		tm* time = getTime();


		animTime += 0.01f;

		if(animTime > 0.7f) 
		{
			isFormat1 = !isFormat1;
			animTime = 0;
		}

		if(isFormat1)  currenttimetextFormat = timetextFormat1;
		else currenttimetextFormat = timetextFormat2;



		sprintf(timetext, currenttimetextFormat.c_str(), time->tm_hour, time->tm_min);
		NF_WriteText16(0, 1, 11, 6, timetext);

		NF_SetTextColor(0, 0, 0);
		sprintf(daydatetext, "%s %02u/%02u/%02u", days[time->tm_wday], time->tm_mday, time->tm_mon + 1, time->tm_year + 1900);
		NF_WriteText16(0, 0, 0, 0, daydatetext);


		if(keysDown() & KEY_A) soundPause(sound_id);

		if(keysDown() & KEY_B) soundResume(sound_id);

		if(keysDown() & KEY_Y)
		{
			NF_DeleteTiledBg(0, 3);
			NF_CreateTiledBg(0, 3, "DolphinBackGround");
		}

		if(keysDown() & KEY_X)
		{
			NF_DeleteTiledBg(0, 3);
			NF_CreateTiledBg(0, 3, "ground");
		}


		NF_UpdateTextLayers();

		NF_SpriteOamSet(1);
		NF_SpriteOamSet(0);

		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}

	return 0; 
}
