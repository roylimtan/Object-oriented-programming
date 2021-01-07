// BaseBall.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "baseball_rules.h"

using namespace sf;
using namespace std;

int currentX = 64;
int currentY = 64;
int currentNumPadOffsetY = 300;

//function for display text
int printMsg(Text& textMsg, Font& font, int size, float x, float y,
	const Color& color, const Color& outColor, char* p)
{
	//Init. messages

	textMsg.setFont(font);
	textMsg.setCharacterSize(size);
	textMsg.setPosition(x, y);
	textMsg.setFillColor(color);
	textMsg.setOutlineColor(outColor);
	textMsg.setOutlineThickness(2.f);
	textMsg.setString(p);

	return 0;
}

int main()
{
	Texture fileMainAni, fileNumberPad, fileThrowButton;
	Sprite spriteNumPad[10];
	Sprite spriteThrow;
	int initWidthHeight = 64;
	int numberGrid[5][5] = { 0 };
	int n = 0, keyInCount = 0, throwCount = 0;
	Clock clock;

	Text textHelpMsg, textInputMsg, textBallCount;
	Font font;
	char inputBuf[5] = "", inputBufTmp[5] = "";
	int inputNumbers[5] = { 0, };
	char ballCountBuf[100] = "";

	SoundBuffer thunderSoundBuf, throwSoundBuf, clickSoundBuf;

	thunderSoundBuf.loadFromFile("resources/thunder.wav");
	Sound thunderSound(thunderSoundBuf);

	throwSoundBuf.loadFromFile("resources/throw.wav");
	Sound throwSound(throwSoundBuf);

	clickSoundBuf.loadFromFile("resources/click.wav");
	Sound clickSound(clickSoundBuf);

	generateNumbers();

	RenderWindow app(VideoMode(900, 556), "BaseBall Game");

	fileMainAni.loadFromFile("resources/main_ani.png");
	fileNumberPad.loadFromFile("resources/number9.png");
	fileThrowButton.loadFromFile("resources/throw.png");

	spriteThrow.setTexture(fileThrowButton);

	IntRect rectSourceSprite(0, 0, 900, 300);
	Sprite spriteMainAni(fileMainAni, rectSourceSprite);

	//set sprite for number pad & number grid
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			n++;
			spriteNumPad[n].setTexture(fileNumberPad);
			spriteNumPad[n].setTextureRect(IntRect(j * initWidthHeight, i * initWidthHeight, initWidthHeight, initWidthHeight));
			numberGrid[i + 1][j + 1] = n;
		}
	}

	font.loadFromFile("resources/downfont.ttf");
	printMsg(textHelpMsg, font, 30, 200.f, 500.f,
		Color::Yellow, Color::Black, 
		(char*)"Select 3 digits and press the throw button.");

	while (app.isOpen())
	{
		Event event;
		while (app.pollEvent(event))
		{
			if (event.type == Event::EventType::Closed)
				app.close();
			
			//윈도우 창 크기 조절
			if (event.type == Event::EventType::Resized)
			{
				FloatRect visiableArea(0, 0, event.size.width, event.size.height);
				auto realPos = app.mapCoordsToPixel(spriteThrow.getPosition());

				currentX = event.size.width / 14.06;
				currentY = event.size.height / 8.68;

				currentNumPadOffsetY = event.size.height / 1.8533;
				cout << "currentNumPadOffsety = " << currentNumPadOffsetY << endl;
			}
			if (event.type == Event::MouseButtonPressed)
			{
				if (event.key.code == Mouse::Left)
				{
					Vector2i Pos = Mouse::getPosition(app);
					//키패크&던지키 버튼 클릭
					int x = Pos.x / currentX + 1;
					int y = (Pos.y - currentNumPadOffsetY) / currentY + 1;
					cout << "Pos.x = " << Pos.x;
					cout << "Pos.y = " << Pos.y;
					cout << "current X = " << currentX;
					cout << "corrent Y = " << currentY;
					cout << "currentNumPadOffsety = " << currentNumPadOffsetY << endl;

					float speed = 3;
					int n = numberGrid[x][y];

					cout << "x=" << x << "y=" << y << "n=" << n << endl;

					if ((x >= 1 && x <= 3) && (y == 4))
					{
						cout << "Pressed Throw button." << endl;
						int ballCount = 0, strikeCount = 0;
						getBallCount(inputNumbers, ballCount, strikeCount);

						throwCount++;
						for (int i = 0; i < initWidthHeight - 61; i += speed)
						{
							spriteThrow.move(speed * 1, speed * 1);
							app.draw(spriteThrow);
						}
						if (strikeCount == 3)
						{
							sprintf(ballCountBuf, "[%02d]strike Out ! you win !\n", throwCount);
							printMsg(textBallCount, font, 50, 300.f, 5.f,
								Color::Black, Color::Blue, ballCountBuf);
							generateNumbers();
							throwCount = 0;
						}
						else
						{
							sprintf(ballCountBuf, "[%02d]%d Ball, %d strike !\n",
								throwCount, ballCount, strikeCount);
							printMsg(textBallCount, font, 50, 300.f, 350.f,
								Color::Black, Color::Green, ballCountBuf);
						}

						memset(inputBufTmp, 0, sizeof(inputBufTmp));
					

						while (1)
						{
							if (clock.getElapsedTime().asSeconds() > 0.04f)//애니메이션속도조정
							{
								rectSourceSprite.top += 300;
								spriteMainAni.setTextureRect(rectSourceSprite);
								clock.restart();
								app.draw(spriteMainAni);
								app.display();

								if (rectSourceSprite.top == 1200)
									thunderSound.play();

								if (rectSourceSprite.top == 6000)
									throwSound.play();

								if (rectSourceSprite.top == 10200) {//300*34 frames
									rectSourceSprite.top = 0;
									spriteMainAni.setTextureRect(rectSourceSprite);
									break;

								}




							}
						}//while
					}//if
					else if (n > 0 && n <= 9)
					{
						//pressed Num Key Pad(0~9)
						cout << "Pressed Number Key pad" << endl;
						if (keyInCount == 3)
						{
							keyInCount = 0;
							memset(inputBufTmp, 0, sizeof(inputBufTmp));
						}

						inputNumbers[keyInCount] = n;
						cout << "n = " << n << endl;
						for (int i = 0; i <= keyInCount; i++)
						{
							sprintf(inputBuf, "%d", inputNumbers[i]);
							strcat(inputBufTmp, inputBuf);
						}

						printMsg(textInputMsg, font, 30,
							200.f, 300.f,
							Color::Black, Color{ 0X8ECF3CFF }, inputBufTmp);
						memset(inputBufTmp, 0, sizeof(inputBufTmp));

						keyInCount++;

						clickSound.play();

						for (int i = 0; i < initWidthHeight - 58; i += speed)
						{
							spriteNumPad[n].move(speed * 1, speed * 1);
							app.draw(spriteNumPad[n]);
							app.display();
						}

					}
				}
			}
		} //while
		app.clear(Color::White);

		spriteThrow.setPosition(0, 300 + 193);
		app.draw(spriteThrow);

		//draw number pad
		for(int i = 0; i <3; i++)
			for (int j = 0; j < 3; j++)
			{
				int n = numberGrid[i + 1][j + 1];
				spriteNumPad[n].setPosition(i * initWidthHeight, j * initWidthHeight + 300);//300 is main ani's height
				app.draw(spriteNumPad[n]);
			}

		spriteMainAni.setPosition(0, 0);
		app.draw(spriteMainAni);

		app.draw(textHelpMsg);
		app.draw(textInputMsg);
		app.draw(textBallCount);
		app.display();
	}//while

	return 0;
}


