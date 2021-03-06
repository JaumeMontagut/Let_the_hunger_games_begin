#ifndef __DUMMY_H__
#define __DUMMY_H__

#include <time.h>//To calculate the random positions
#include <iostream>//To calculate the random positions

#include "Module.h"
#include "Globals.h"
#include "../SDL/include/SDL.h"
#include "../SDL/include/SDL_image.h"
#include "../SDL/include/SDL_mixer.h"

#pragma comment (lib,"../SDL/x86/SDL2.lib")
#pragma comment (lib,"../SDL/x86/SDL2main.lib")
#pragma comment (lib,"../SDL/x86/SDL2_image.lib")
#pragma comment (lib,"../SDL/x86/SDL2_mixer.lib")

#define screenWitdh 1280
#define screenHeight 720
#define pushDist 26
#define pushDistMultiplier 5
#define initialTime 7000

class ModuleDummy : public Module
{
	bool playing;
	bool pressingUp, pressingLeft, pressingDown, pressingRight, pressingW, pressingA, pressingS, pressingD;
	bool pushP2 = false, pushP1 = false;
	bool timeFinished = false;
	bool p1IsIn = false, p2IsIn = false;
	bool start = false;
	int characterWitdh = 0, characterHeight = 0;
	int characterSpeed;
	int pushR1Pos = 0;//1 = up, 2 = left, 3 = down, 4 = right
	int pushR2Pos = 0;
	int pushR1Counter = 0, pushR2Counter = 0;
	long int timer = 0;
	long int maxTime;
	SDL_Window * window = nullptr;
	SDL_Renderer* renderer = nullptr;
	SDL_Event event;
	SDL_Rect p1Rect;
	SDL_Rect p2Rect;
	SDL_Rect pushR1;
	SDL_Rect pushR2;
	SDL_Rect p1Screen;
	SDL_Rect p2Screen;
	SDL_Rect objectiveR;
	SDL_Rect continueR;
	SDL_Texture * p1Tx = nullptr;
	SDL_Texture * p2Tx = nullptr;
	SDL_Texture * backgroundTx = nullptr;
	SDL_Texture * winTx = nullptr;
	SDL_Texture * loseTx = nullptr;
	SDL_Texture * objectiveTx = nullptr;
	SDL_Texture * push1Tx = nullptr;
	SDL_Texture * push2Tx = nullptr;
	SDL_Texture * initialTx = nullptr;
	SDL_Texture * continueTx = nullptr;
	Mix_Music * backgroundMusic = nullptr;
	Mix_Chunk * almostTimeSFX = nullptr;
	Mix_Chunk * timeSFX = nullptr;
	Mix_Chunk * pushSFX = nullptr;
	Mix_Chunk * winSFX = nullptr;

	bool Init()
	{
		LOG("Dummy Init!");
		playing = true;
		pressingUp = false;
		pressingLeft = false;
		pressingDown = false;
		pressingRight = false;
		pressingW = false;
		pressingA = false;
		pressingS = false;
		pressingD = false;
		characterSpeed = 6;
		maxTime = initialTime;

		SDL_Init(SDL_INIT_VIDEO);
		window = SDL_CreateWindow("Let the hunger games begin", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWitdh, screenHeight, 0);
		renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

		IMG_Init(IMG_INIT_PNG);
		p1Tx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Character1.png"));
		p2Tx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Character2.png"));
		backgroundTx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Background.png"));
		winTx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Win.png"));
		loseTx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Lose.png"));
		objectiveTx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Objective.png"));
		push1Tx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Push1.png"));
		push2Tx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Push2.png"));
		initialTx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/Initial.png"));
		continueTx = SDL_CreateTextureFromSurface(renderer, IMG_Load("Assets/ContinueMessage.png"));
		SDL_QueryTexture(p1Tx, nullptr, nullptr, &characterWitdh, &characterHeight);
		p1Rect.x = screenWitdh / 2 - characterWitdh / 2;
		p1Rect.y = screenHeight / 2 - characterHeight / 2;
		p1Rect.w = characterWitdh;
		p1Rect.h = characterHeight;
		p2Rect.x = screenWitdh / 2 - characterWitdh / 2;
		p2Rect.y = screenHeight / 2 - characterHeight / 2;
		p2Rect.w = characterWitdh;
		p2Rect.h = characterHeight;
		pushR1.w = 70;
		pushR1.h = 70;
		pushR1.x = (p1Rect.x + p1Rect.h / 2) - (pushR1.h / 2);
		pushR1.y = (p1Rect.y + p1Rect.w / 2) - (pushR1.w / 2);
		pushR2.w = 70;
		pushR2.h = 70;
		pushR2.x = (p2Rect.x + p2Rect.h / 2) - (pushR2.h / 2);
		pushR2.y = (p2Rect.y + p2Rect.w / 2) - (pushR2.w / 2);
		objectiveR.w = 470;
		objectiveR.h = 303;
		objectiveR.x = rand() % (screenWitdh - objectiveR.w);
		objectiveR.y = rand() % (screenHeight - objectiveR.h);
		p1Screen.x = 0;
		p1Screen.y = 0;
		p1Screen.w = screenWitdh / 2;
		p1Screen.h = screenHeight;
		p2Screen.x = screenWitdh / 2;
		p2Screen.y = 0;
		p2Screen.w = screenWitdh / 2;
		p2Screen.h = screenHeight;
		continueR.w = 342;
		continueR.h = 43;
		continueR.x = screenWitdh / 2 - continueR.w / 2;
		continueR.y = screenHeight - continueR.h - 25;
		srand(time(NULL));

		Mix_Init(MIX_INIT_OGG);
		Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
		backgroundMusic = Mix_LoadMUS("Assets/BackgroundMusic.ogg");
		almostTimeSFX = Mix_LoadWAV("Assets/AlmostTime.wav");
		timeSFX = Mix_LoadWAV("Assets/Time.wav");
		pushSFX = Mix_LoadWAV("Assets/Attack.wav");
		winSFX = Mix_LoadWAV("Assets/Win.wav");
		Mix_PlayMusic(backgroundMusic, -1);
		return true;
	}

	update_status PreUpdate()
	{
		//Input
		while (SDL_PollEvent(&event) != 0)
		{
			switch (event.type)
			{
			case SDL_QUIT:
				playing = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					pressingUp = true;
					pushR2Pos = 1;
					break;
				case SDLK_LEFT:
					pressingLeft = true;
					pushR2Pos = 2;
					break;
				case SDLK_DOWN:
					pressingDown = true;
					pushR2Pos = 3;
					break;
				case SDLK_RIGHT:
					pressingRight = true;
					pushR2Pos = 4;
					break;
				case SDLK_w:
					pressingW = true;
					pushR1Pos = 1;
					break;
				case SDLK_a:
					pressingA = true;
					pushR1Pos = 2;
					break;
				case SDLK_s:
					pressingS = true;
					pushR1Pos = 3;
					break;
				case SDLK_d:
					pressingD = true;
					pushR1Pos = 4;
					break;
				case SDLK_g:
					//SFX
					if ((p2Rect.x + p2Rect.w) > pushR1.x && p2Rect.x < (pushR1.x + pushR1.w) && (p2Rect.y + p2Rect.h) > pushR1.y && p2Rect.y < (pushR1.y + pushR1.h))
					{
						Mix_PlayChannel(-1, pushSFX, 0);
						pushP2 = true;
					}
					break;
				case SDLK_0:
				case SDLK_KP_0:
					//SFX
					if ((p1Rect.x + p1Rect.w) > pushR2.x && p1Rect.x < (pushR2.x + pushR2.w) && (p1Rect.y + p1Rect.h) > pushR2.y && p1Rect.y < (pushR2.y + pushR2.h))
					{
						Mix_PlayChannel(-1, pushSFX, 0);
						pushP1 = true;
					}
					break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_UP:
					pressingUp = false;
					break;
				case SDLK_LEFT:
					pressingLeft = false;
					break;
				case SDLK_DOWN:
					pressingDown = false;
					break;
				case SDLK_RIGHT:
					pressingRight = false;
					break;
				case SDLK_w:
					pressingW = false;
					break;
				case SDLK_a:
					pressingA = false;
					break;
				case SDLK_s:
					pressingS = false;
					break;
				case SDLK_d:
					pressingD = false;
					break;
				}
				break;
			}
		}
		return update_status::UPDATE_CONTINUE;
	}

	update_status Update()
	{
		//Logic
		//- Timer
		timer += 1000 / 60;
		if (timer >= maxTime)
		{
			Mix_PlayChannel(-1, timeSFX, 0);
			timeFinished = true;
			timer = 0;
			if (maxTime >= 5000)
			{
				maxTime -= 1000;
			}
		}
		if (timer < (maxTime - 3000) + 1000 / 60 && timer >(maxTime - 3000) - 1000 / 60)
		{
			Mix_PlayChannel(-1, almostTimeSFX, 0);
		}
		SDL_Delay(1000 / 60);
		//- Player 1
		if (pressingW == true)
		{
			p1Rect.y -= characterSpeed;
		}
		if (pressingA == true)
		{
			p1Rect.x -= characterSpeed;
		}
		if (pressingS == true)
		{
			p1Rect.y += characterSpeed;
		}
		if (pressingD == true)
		{
			p1Rect.x += characterSpeed;
		}
		//- Player 2
		if (pressingUp == true)
		{
			p2Rect.y -= characterSpeed;
		}
		if (pressingLeft == true)
		{
			p2Rect.x -= characterSpeed;
		}
		if (pressingDown == true)
		{
			p2Rect.y += characterSpeed;
		}
		if (pressingRight == true)
		{
			p2Rect.x += characterSpeed;
		}
		//- Push rectangle 1 position
		switch (pushR1Pos)
		{
		case 0:
			pushR1.x = (p1Rect.x + p1Rect.h / 2) - (pushR1.h / 2);
			pushR1.y = (p1Rect.y + p1Rect.w / 2) - (pushR1.w / 2);
			break;
		case 1:
			pushR1.y = p1Rect.y - pushR1.h;
			pushR1.x = p1Rect.x;
			break;
		case 2:
			pushR1.x = p1Rect.x - pushR1.w;
			pushR1.y = p1Rect.y;
			break;
		case 3:
			pushR1.y = p1Rect.y + p1Rect.h;
			pushR1.x = p1Rect.x;
			break;
		case 4:
			pushR1.x = p1Rect.x + p1Rect.w;
			pushR1.y = p1Rect.y;
			break;
		}
		//- Push rectangle 2 position
		switch (pushR2Pos)
		{
		case 0:
			pushR2.x = (p2Rect.x + p2Rect.h / 2) - (pushR2.h / 2);
			pushR2.y = (p2Rect.y + p2Rect.w / 2) - (pushR2.w / 2);
			break;
		case 1:
			pushR2.y = p2Rect.y - pushR2.h;
			pushR2.x = p2Rect.x;
			break;
		case 2:
			pushR2.x = p2Rect.x - pushR2.w;
			pushR2.y = p2Rect.y;
			break;
		case 3:
			pushR2.y = p2Rect.y + p2Rect.h;
			pushR2.x = p2Rect.x;
			break;
		case 4:
			pushR2.x = p2Rect.x + p2Rect.w;
			pushR2.y = p2Rect.y;
			break;
		}
		//- Push rectangle 1 action
		if (pushP2 == true)
		{
			switch (pushR1Pos)
			{
			case 1:
				p2Rect.y -= pushDist;
				break;
			case 2:
				p2Rect.x -= pushDist;
				break;
			case 3:
				p2Rect.y += pushDist;
				break;
			case 4:
				p2Rect.x += pushDist;
				break;
			}
			if (pushR1Counter == pushDistMultiplier)
			{
				pushP2 = false;
				pushR1Counter = 0;
			}
			pushR1Counter++;
		}
		//- Push rectangle 2 action
		if (pushP1 == true)
		{
			switch (pushR2Pos)
			{
			case 1:
				p1Rect.y -= pushDist;
				break;
			case 2:
				p1Rect.x -= pushDist;
				break;
			case 3:
				p1Rect.y += pushDist;
				break;
			case 4:
				p1Rect.x += pushDist;
				break;
			}
			if (pushR2Counter == pushDistMultiplier)
			{
				pushP1 = false;
				pushR2Counter = 0;
			}
			pushR2Counter++;
		}
		//- Player 1 screen edges
		if (p1Rect.x + p1Rect.w / 2 < 0)
		{
			p1Rect.x = screenWitdh - p1Rect.w / 2;
		}
		if (p1Rect.x + p1Rect.w / 2 > screenWitdh)
		{
			p1Rect.x = 0 + p1Rect.w / 2;
		}
		if (p1Rect.y + p1Rect.h / 2 < 0)
		{
			p1Rect.y = screenHeight - p1Rect.h / 2;
		}
		if (p1Rect.y + p1Rect.h / 2 > screenHeight)
		{
			p1Rect.y = 0 + p1Rect.h / 2;
		}
		//- Player 2 screen edges
		if (p2Rect.x + p2Rect.w / 2 < 0)
		{
			p2Rect.x = screenWitdh - p2Rect.w / 2;
		}
		if (p2Rect.x + p2Rect.w / 2 > screenWitdh)
		{
			p2Rect.x = 0 + p2Rect.w / 2;
		}
		if (p2Rect.y + p2Rect.h / 2 < 0)
		{
			p2Rect.y = screenHeight - p2Rect.h / 2;
		}
		if (p2Rect.y + p2Rect.h / 2 > screenHeight)
		{
			p2Rect.y = 0 + p2Rect.h / 2;
		}
		//- Reduce objective size
		if (timeFinished == true)
		{
			if (objectiveR.w > (p1Rect.w + p2Rect.w))
			{
				objectiveR.w -= 31;//31 = 10% of the sprite size
				objectiveR.h -= 20;//20 = 10% of the sprite size
			}
		}
		return update_status::UPDATE_CONTINUE;
	}

	update_status PostUpdate()
	{
		//Render
		//-Background
		SDL_RenderCopy(renderer, backgroundTx, NULL, NULL);
		//-Objective
		SDL_RenderCopy(renderer, objectiveTx, NULL, &objectiveR);
		//-Push indicator 1
		switch (pushR1Pos)
		{
		case 1:
			SDL_RenderCopyEx(renderer, push1Tx, NULL, &pushR1, 0, NULL, SDL_FLIP_NONE);
			break;
		case 2:
			SDL_RenderCopyEx(renderer, push1Tx, NULL, &pushR1, 270, NULL, SDL_FLIP_NONE);
			break;
		case 3:
			SDL_RenderCopyEx(renderer, push1Tx, NULL, &pushR1, 180, NULL, SDL_FLIP_NONE);
			break;
		case 4:
			SDL_RenderCopyEx(renderer, push1Tx, NULL, &pushR1, 90, NULL, SDL_FLIP_NONE);
			break;
		}
		//-Push indicator 2
		switch (pushR2Pos)
		{
		case 1:
			SDL_RenderCopyEx(renderer, push2Tx, NULL, &pushR2, 0, NULL, SDL_FLIP_NONE);
			break;
		case 2:
			SDL_RenderCopyEx(renderer, push2Tx, NULL, &pushR2, 270, NULL, SDL_FLIP_NONE);
			break;
		case 3:
			SDL_RenderCopyEx(renderer, push2Tx, NULL, &pushR2, 180, NULL, SDL_FLIP_NONE);
			break;
		case 4:
			SDL_RenderCopyEx(renderer, push2Tx, NULL, &pushR2, 90, NULL, SDL_FLIP_NONE);
			break;
		}
		//-Character 1
		SDL_RenderCopy(renderer, p1Tx, NULL, &p1Rect);
		//-Character 2
		SDL_RenderCopy(renderer, p2Tx, NULL, &p2Rect);

		SDL_RenderPresent(renderer);
		return update_status::UPDATE_CONTINUE;
	}

	bool CleanUp()
	{
		LOG("Dummy CleanUp!");
		Mix_FreeMusic(backgroundMusic);
		Mix_FreeChunk(almostTimeSFX);
		Mix_FreeChunk(timeSFX);
		Mix_FreeChunk(pushSFX);
		Mix_FreeChunk(winSFX);
		Mix_CloseAudio();
		Mix_Quit();
		SDL_DestroyTexture(p1Tx);
		SDL_DestroyTexture(p2Tx);
		SDL_DestroyTexture(backgroundTx);
		SDL_DestroyTexture(winTx);
		SDL_DestroyTexture(loseTx);
		SDL_DestroyTexture(objectiveTx);
		SDL_DestroyTexture(push1Tx);
		SDL_DestroyTexture(push2Tx);
		SDL_DestroyTexture(initialTx);
		SDL_DestroyTexture(continueTx);
		IMG_Quit();
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
		return true;
	}
};

#endif // __DUMMY_H__