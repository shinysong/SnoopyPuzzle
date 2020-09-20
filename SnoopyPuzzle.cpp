#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <bangtal.h>
#include <iostream>
using namespace bangtal;
using namespace std;

template<typename ... Args> // 사진 좌표 할당할 때 format 을 사용하기 위한 함수 정의
std::string format_string(const std::string& format, Args ... args)
{
	size_t size = snprintf(nullptr, 0, format.c_str(), args ...) + 1;
	std::unique_ptr<char[]> buffer(new char[size]);
	snprintf(buffer.get(), size, format.c_str(), args ...);
	return std::string(buffer.get(), buffer.get() + size - 1);
}

int point[10][10] = { {426,436},{567,436},{708,436},
					{426,295},{567,295},{708,295},
				{426,154},{567,154},{708,154} };
int blankX;
int blankY;
class P {
public:
	int centerX, centerY, radius;

	//빈칸이 인접해있는지
	bool check_blank(int x, int y) {
		return ((x == centerX - radius) && (y == centerY)) || ((x == centerX + radius) && (y == centerY)) || ((x == centerX) && y == (centerY + radius)) || ((x == centerX) && (y == centerY - radius));
	}

};

P piece[9];
const int checkedX[9] = { 426 ,567,708,426 ,567,708,426 ,567,708 };
const int checkedY[9] = { 436,436,436,295,295,295,154,154,154 };

//랜덤배열 함수
int num[9];
void random() {
	int tmp, x, y;
	srand((unsigned int)time(NULL));
	for (int i = 0; i < 9; i++) {
		num[i] = i;
	}
	for (int i = 0; i < 20; i++) {
		x = rand() % 9;
		y = rand() % 9;

		if (x != y) {
			tmp = num[x];
			num[x] = num[y];
			num[y] = tmp;
		}
	}
}

void swap(int* px, int* py, int* x, int* y) {
	int tmpX, tmpY;
	tmpX = *x;
	*x = *px;
	*px = tmpX;

	tmpY = *y;
	*y = *py;
	*py = tmpY;
}
clock_t starttime, endtime;
int main() {

	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	//장면 생성
	auto scene = Scene::create("스누피와 겨울", "Images/원본.png");
	auto scene1 = Scene::create("배경", "Images/배경.png");
	auto startButton = Object::create("Images/start.png", scene, 590, 70);
	auto endButton = Object::create("Images/end.png", scene, 590, 20);

	random();
	
	ObjectPtr pieces[9];
	for (int i = 0; i < 9; i++) {
		string str1 = format_string("Images/%d.png", num[i]);
		pieces[num[i]] = Object::create(str1, scene1, point[i][0], point[i][1]);
		piece[num[i]] = { point[i][0], point[i][1], 141 };
		if (num[i] == 0) {
			blankX = point[i][0];
			blankY = point[i][1];
		}
	} //pieces[0] 은 공백

	for (int i = 0; i < 9; i++) {
		pieces[i]->setOnMouseCallback([=](ObjectPtr object, int x, int y, MouseAction)->bool {

			if (pieces[0] != pieces[i] && piece[i].check_blank(blankX, blankY)) {
					pieces[0]->locate(scene1, piece[i].centerX, piece[i].centerY);
					pieces[i]->locate(scene1, blankX, blankY);
					swap(&piece[i].centerX, &piece[i].centerY, &blankX, &blankY);
				}

			if (piece[0].centerX == checkedX[0] && piece[0].centerY == checkedY[0] &&
				piece[1].centerX == checkedX[1] && piece[1].centerY == checkedY[1] &&
				piece[2].centerX == checkedX[2] && piece[2].centerY == checkedY[2] &&
				piece[3].centerX == checkedX[3] && piece[3].centerY == checkedY[3] &&
				piece[4].centerX == checkedX[4] && piece[4].centerY == checkedY[4] &&
				piece[5].centerX == checkedX[5] && piece[5].centerY == checkedY[5] &&
				piece[6].centerX == checkedX[6] && piece[6].centerY == checkedY[6] &&
				piece[7].centerX == checkedX[7] && piece[7].centerY == checkedY[7] &&
				piece[8].centerX == checkedX[8] && piece[8].centerY == checkedY[8]) {
				endtime = clock();
				int time = (endtime-starttime)/ CLOCKS_PER_SEC;
				scene->enter();
				showMessage("퍼즐완성!");
				endButton->show();
			}
			return 0;
				});
	}

	startButton->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		startButton->hide();
		endButton->hide();
		scene1->enter();
		starttime = clock();
		return 0;
		});

	endButton->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction)->bool {
		endGame();
		return 0;
		});

	startGame(scene);
	return 0;
}