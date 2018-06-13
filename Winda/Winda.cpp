#define _USE_MATH_DEFINES
#include<math.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <vector>
#include <deque>

#include"./sdl-2.0.7/include/SDL.h"
#include"./sdl-2.0.7/include/SDL_main.h"


#define SCREEN_WIDTH	1280
#define SCREEN_HEIGHT	720

char text[128];
char btn[32];

void DrawString(SDL_Surface *screen, int x, int y, const char *text,
	SDL_Surface *charset) {
	int px, py, c;
	SDL_Rect s, d;
	s.w = 8;
	s.h = 8;
	d.w = 8;
	d.h = 8;
	while (*text) {
		c = *text & 255;
		px = (c % 16) * 8;
		py = (c / 16) * 8;
		s.x = px;
		s.y = py;
		d.x = x;
		d.y = y;
		SDL_BlitSurface(charset, &s, screen, &d);
		x += 8;
		text++;
	};
};


void DrawSurface(SDL_Surface *screen, SDL_Surface *sprite, int x, int y) {
	SDL_Rect dest;
	dest.x = x - sprite->w / 2;
	dest.y = y - sprite->h / 2;
	dest.w = sprite->w;
	dest.h = sprite->h;
	SDL_BlitSurface(sprite, NULL, screen, &dest);
};

class Button
{

public:
	int x, y, width, height;
	SDL_Surface *screen;
	SDL_Surface *btn;

	Button(SDL_Surface *_screen, SDL_Surface *_btn, int _x, int _y)
	{
		x = _x;
		y = _y;

		screen = _screen;
		btn = _btn;
		width = btn->w;
		height = btn->h;
	}

	void Draw()
	{
		DrawSurface(screen, btn, x + width / 2, y + height / 2);
	}

	bool checkMouse(int mouseX, int mouseY)
	{
		if (mouseX > x && mouseX < x + width && mouseY >y && mouseY < y + height)
			return true;
		else
			return false;
	}
};


class lift
{
public:
	float y, x;	//jej pozycja w xy;
	SDL_Surface *lifts;
	lift(int _y, SDL_Surface *_l)
	{
		y = _y;
		lifts = _l;
	}
	void Draw(SDL_Surface *screen)
	{
		DrawSurface(screen, lifts, SCREEN_WIDTH / 2, y - lifts->h / 2);
	}

	void goTo(float pxY, double dt)
	{
		pxY = SCREEN_HEIGHT - 120 * pxY;
		int dir;

		if (y < pxY)	//bardzo niedopracowane TYLKO TEST - TAK NIE BEDZIE
			dir = 1;
		else
			dir = -1;


		if (abs(y - pxY) > 1)		//bardzo niedopracowane TYLKO TEST - TAK NIE BEDZIE
		{
			y += 150 * dt *dir;
		}
	}
};

int numClicked = 0;  //który przycisk zostal nacisniety


int main(int argc, char **argv) {
	int tick1, tick2, quit, rc;
	double delta;
	SDL_Event event;
	SDL_Surface *screen, *charset;

	SDL_Surface *lifts;
	SDL_Surface *btn1s, *btn2s, *btn3s, *btn4s, *btn5s;

	SDL_Texture *scrtex;
	SDL_Window *window;
	SDL_Renderer *renderer;

	//zeby w konsoli pokazywało printf:
	// project -> szablon2 properties -> Linker -> System -> Subsystem
	// zmienić na "Console"
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		printf("SDL_Init error: %s\n", SDL_GetError());
		return 1;
	}		//inicjalizacja SDL

	rc = SDL_CreateWindowAndRenderer(SCREEN_WIDTH, SCREEN_HEIGHT, 0,
		&window, &renderer);		//tworzenie okna

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
	SDL_RenderSetLogicalSize(renderer, SCREEN_WIDTH, SCREEN_HEIGHT);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

	screen = SDL_CreateRGBSurface(0, SCREEN_WIDTH, SCREEN_HEIGHT, 32,
		0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);

	scrtex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_STREAMING,
		SCREEN_WIDTH, SCREEN_HEIGHT);

	// wczytanie obrazka cs8x8.bmp
	charset = SDL_LoadBMP("./graphics/cs8x8.bmp");
	SDL_SetColorKey(charset, true, 0x000000);

	//wczytanie grafiki windy i przyciskow
	lifts = SDL_LoadBMP("./graphics/lift.bmp");
	btn1s = SDL_LoadBMP("./graphics/btn1.bmp");
	btn2s = SDL_LoadBMP("./graphics/btn2.bmp");
	btn3s = SDL_LoadBMP("./graphics/btn3.bmp");
	btn4s = SDL_LoadBMP("./graphics/btn4.bmp");
	btn5s = SDL_LoadBMP("./graphics/btn5.bmp");

	int black = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	int white = SDL_MapRGB(screen->format, 255, 255, 255);


	//1 pietro przyciski

	Button *btn1 = new Button(screen, btn2s, 350, SCREEN_HEIGHT - 100);
	Button *btn2 = new Button(screen, btn3s, 400, SCREEN_HEIGHT - 100);
	Button *btn3 = new Button(screen, btn4s, 450, SCREEN_HEIGHT - 100);
	Button *btn4 = new Button(screen, btn5s, 500, SCREEN_HEIGHT - 100);

	//2 pietro przyciski

	Button *btn5 = new Button(screen, btn1s, 300, SCREEN_HEIGHT - 220);
	Button *btn6 = new Button(screen, btn3s, 400, SCREEN_HEIGHT - 220);
	Button *btn7 = new Button(screen, btn4s, 450, SCREEN_HEIGHT - 220);
	Button *btn8 = new Button(screen, btn5s, 500, SCREEN_HEIGHT - 220);

	//3 pietro przyciski

	Button *btn9 = new Button(screen, btn1s, 300, SCREEN_HEIGHT - 340);
	Button *btn10 = new Button(screen, btn2s, 350, SCREEN_HEIGHT - 340);
	Button *btn11 = new Button(screen, btn4s, 450, SCREEN_HEIGHT - 340);
	Button *btn12 = new Button(screen, btn5s, 500, SCREEN_HEIGHT - 340);

	//4 pietro przyciski

	Button *btn13 = new Button(screen, btn1s, 300, SCREEN_HEIGHT - 470);
	Button *btn14 = new Button(screen, btn2s, 350, SCREEN_HEIGHT - 470);
	Button *btn15 = new Button(screen, btn3s, 400, SCREEN_HEIGHT - 470);
	Button *btn16 = new Button(screen, btn5s, 500, SCREEN_HEIGHT - 470);

	//5 pietro przyciski

	Button *btn17 = new Button(screen, btn1s, 300, SCREEN_HEIGHT - 600);
	Button *btn18 = new Button(screen, btn2s, 350, SCREEN_HEIGHT - 600);
	Button *btn19 = new Button(screen, btn3s, 400, SCREEN_HEIGHT - 600);
	Button *btn20 = new Button(screen, btn4s, 450, SCREEN_HEIGHT - 600);

	std::vector <Button*> buttons;		//wektor przyciskow by w petli je rysowac i sprawdzac

	buttons.push_back(btn1);
	buttons.push_back(btn2);
	buttons.push_back(btn3);
	buttons.push_back(btn4);
	buttons.push_back(btn5);
	buttons.push_back(btn6);
	buttons.push_back(btn7);
	buttons.push_back(btn8);
	buttons.push_back(btn9);
	buttons.push_back(btn10);
	buttons.push_back(btn11);
	buttons.push_back(btn12);
	buttons.push_back(btn13);
	buttons.push_back(btn14);
	buttons.push_back(btn15);
	buttons.push_back(btn16);
	buttons.push_back(btn17);
	buttons.push_back(btn18);
	buttons.push_back(btn19);
	buttons.push_back(btn20);

	std::deque <int> kolejka;		//kolejkowanie pieter - na razie nic nie robi

	lift *mainLift = new lift(SCREEN_HEIGHT / 2, lifts);

	kolejka.push_back(0);
	kolejka.push_back(1);

	SDL_FillRect(screen, NULL, white); // białe tło
	for (int i = 0; i < 20; i++)		//Rysujemy przyciski przed whilem, zeby nie rysowal co klatka nieruchomych przyciskow
	{
		buttons[i]->Draw();
	}


	SDL_Rect *floorLine = new SDL_Rect;
	floorLine->x = 0;
	floorLine->y = 0;
	floorLine->w = SCREEN_WIDTH / 2;
	floorLine->h = 2;

	for (int i = 2; i < 6; i++)		//rysowanie pięter
	{
		floorLine->y = 120 * i - floorLine->h / 2;
		SDL_FillRect(screen, floorLine, black);
	}

	SDL_Rect *srcrect = new SDL_Rect;	//Obszar za winda do zamalowanie na bialo

	srcrect->x = SCREEN_WIDTH / 2 - mainLift->lifts->w / 2;
	srcrect->y = 0;
	srcrect->w = mainLift->lifts->w;
	srcrect->h = SCREEN_HEIGHT;


	SDL_Rect *textRect = new SDL_Rect;
	textRect->x = 4;
	textRect->y = 4;
	textRect->w = SCREEN_WIDTH-8;
	textRect->h = 36;

	tick1 = SDL_GetTicks();
	quit = 0;

	while (!quit)
	{
		tick2 = SDL_GetTicks();
		delta = (tick2 - tick1) * 0.001;
		tick1 = tick2;



		SDL_FillRect(screen, srcrect, white); //Obszar za winda do zamalowanie na bialo

		mainLift->Draw(screen);
		mainLift->goTo(kolejka.front(), delta);	//funkcja gdzie ma jechac winda - TYLKO DO TESTOW TAK NIE BEDZIE TO WYGLADAC		

		SDL_FillRect(screen, textRect, black);		//czarn y prostokąt i napisy -> POZNIEJ ZMIENIC
		DrawString(screen, screen->w / 2 - strlen(text) * 8 / 2, 10, text, charset);

		SDL_UpdateTexture(scrtex, NULL, screen->pixels, screen->pitch);		//na teksture scrtex rysujemy ekran
		//SDL_RenderClear(renderer);
		SDL_RenderCopy(renderer, scrtex, NULL, NULL);
		SDL_RenderPresent(renderer);

		//obsluga klawiszy

		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_ESCAPE)
					quit = 1;

				else if (event.key.keysym.sym == SDLK_UP)
				{

				}
				else if (event.key.keysym.sym == SDLK_DOWN)
				{

				}
				break;
			case SDL_KEYUP:
				break;
			case SDL_QUIT:
				quit = 1;
				break;
			};

			if (event.type == SDL_MOUSEBUTTONDOWN)		//myszka nacisnieta
			{
				for (int i = 0; i < 20; i++)
				{
					if (buttons[i]->checkMouse(event.button.x, event.button.y))		//sprawdzenie przyciskania dla kazdego przycisku
					{
						sprintf_s(text, "nacisnieto przycisk o ID : %d", i);
						break;
					}
				}
			}
		};
	};

	SDL_FreeSurface(charset);
	SDL_FreeSurface(screen);
	SDL_DestroyTexture(scrtex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);

	SDL_Quit();
	return 0;
};
