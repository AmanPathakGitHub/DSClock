#include <stdio.h>
#include <nds.h>

#include <time.h>

#include <nf_lib.h>

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

tm* getTime()
{
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);


	return timeinfo;
}

#define TOP_SCREEN 0
#define BOTTOM_SCREEN 1

int main(int argc, char **argv)
{
	 
	consoleDemoInit();
	// Turn on MODE 0 on the Top Screen
	NF_Set2D(0, 0);
	NF_Set2D(1, 0);

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

	NF_LoadTextFont16("fnt/font16", "normal", 256, 256, 0);
	NF_LoadTextFont16("fnt/font16", "bnormal", 256, 256, 0);

	NF_LoadRawSound("sfx/music", 1, 22050, 0);

	NF_CreateTiledBg(0, 3, "ground");
	//NF_CreateTiledBg(1, 3, "ground");

	NF_CreateTextLayer16(0, 0, 0, "normal");
	NF_CreateTextLayer16(1, 1, 0, "bnormal");


	NF_WriteText16(1, 1, 0, 0, "Press A to Pause Sound");
	NF_WriteText16(1, 1, 0, 1, "Press B to Resume Sound");

	NF_UpdateTextLayers();


	char timetext[32];
	char daydatetext[32];

	int sound_id = NF_PlayRawSound(1, 127, 64, true, 0);

	while(1)
	{
		scanKeys();

		tm* time = getTime();

		sprintf(timetext, "%02u : %02u", time->tm_hour, time->tm_min);
		NF_WriteText16(0, 0, 11, 6, timetext);

		sprintf(daydatetext, "%s %02u/%02u/%02u", days[time->tm_wday], time->tm_mday, time->tm_mon + 1, time->tm_year + 1900);
		NF_WriteText16(0, 0, 0, 1, daydatetext);


		if(keysDown() & KEY_A) soundPause(sound_id);

		if(keysDown() & KEY_B) soundResume(sound_id);

		NF_UpdateTextLayers();

		NF_SpriteOamSet(1);
		NF_SpriteOamSet(0);

		swiWaitForVBlank();
		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}

	return 0; 
}
