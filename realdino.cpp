#include <bangtal>
#include <iostream>
#include "Trex.h"
#include "Tree.h"
#include <ctime>


using namespace bangtal;
using namespace std;

ObjectPtr Trex::create(const string& trexImage1, ScenePtr scene, int x, int y, bool shown) {
	auto object = ObjectPtr(new Trex(trexImage1,scene, x, y, shown));
	Object::add(object);

	return object;
}
Trex::Trex(const string& trexImage1, ScenePtr scene, int x, int y, bool shown) : Object(trexImage1, scene, x, y, shown) {
}

bool Trex::onMouse(int x, int y, MouseAction action)
{
	if (!Object::onMouse(x, y, action)) {
		cout << "picked location" << x << " " << y << endl;
	}
	return true;
}


ObjectPtr Tree::create(const string& TreeImage1, const string& TreeImage2, ScenePtr scene, int x, int y, bool choice, bool shown)
{
	auto object = ObjectPtr(new Tree(TreeImage1, TreeImage2, scene, x, y, choice, shown));
	Object::add(object);

	return object;
}

Tree::Tree(const string& TreeImage1, const string& TreeImage2, ScenePtr scene, int x, int y, bool choice, bool shown)
	: Object(choice ? TreeImage1 : TreeImage2, scene, x, y, shown), _choice(choice), _TreeImage1(TreeImage1), _TreeImage2(TreeImage2)
{
}
Tree::~Tree() {
	cout << "just one tree has been gone" << endl;
}
bool Tree::onMouse(int x, int y, MouseAction action)
{
	if (!Object::onMouse(x, y, action)) {
		if (action == MouseAction::MOUSE_CLICK) {
			cout << x << y << endl;
		}
	}

	return true;
}


int main()
{
	unsigned int srand(time(NULL));
	setGameOption(GameOption::GAME_OPTION_INVENTORY_BUTTON, false);
	setGameOption(GameOption::GAME_OPTION_MESSAGE_BOX_BUTTON, false);

	const int TREX_DEFAULT_Y = 125;
	const int TREX_JUMP_Y = 300;
	const int TREX_LEAST = 20;
	const int TREX_HIGH = 120;
	const int TREX_NOT_DIE = 140;

	int trex_X = 40;
	int trex_Y = 125;
	bool dead = false;
	int i = 0, count = 0, count2 = 0, count3 = 0, score = 0, highscore = 0;
	int tree_X = 1200;
	int tree_Y = 125;
	ObjectPtr tree[1000] = { 0 };
	int tree_X2 = tree_X;
	int tree_X3 = tree_X;
	int eraseTree;
	bool restart = false;
	bool out = false;
	bool goingup = true;
	bool ground = true;

	auto scene = Scene::create("T_Rex_Run", "image/background.png");
	auto jumpSound = Sound::create("image/jump_dino.mp3");
	auto trex = Trex::create("image/main-character1.png", scene, trex_X, trex_Y);
	auto trextimer = Timer::create(0.05f);
	auto trextimer2 = Timer::create(0.05f);
	auto gameover = Object::create("image/gameover_text.png", scene, 355, 400, false);
	auto restartB = Object::create("image/replay_button.png", scene, 630, 320, false);
	auto timer = Timer::create(0.001f);
	auto Timer2 = Timer::create(0.05f);
	auto treeMakingTimer = Timer::create(0.01f);
	auto restarttimer = Timer::create(0.005f);
	auto scoretimer = Timer::create(0.02f);
	auto jumptimer = Timer::create(0.01f);
	gameover->setScale(3.0f);
	restartB->setScale(2.0f);
	trex->setScale(2.0f);


	restartB->setOnMouseCallback([&](ObjectPtr object, int x, int y, MouseAction action)-> bool {
		eraseTree = 0;
		gameover->hide();
		restartB->hide();
		trex->locate(scene, trex_X, TREX_DEFAULT_Y);
		restarttimer->start();
		return true;
		});

	restarttimer->setOnTimerCallback([&](TimerPtr restartT)->bool {
		i = 0, tree_X2 = tree_X, tree_X3 = tree_X;
		tree_X = 1200, tree_Y = 125;
		dead = false; //죽은거 살리기 
		if (eraseTree < count) {
			if (eraseTree < count) {
				tree[eraseTree]->hide();
				eraseTree++;
				restartT->set(0.005f);
				restartT->start();
			}
			if (eraseTree == count) {
				count = 0;
				count2 = 0;
				count3 = 1;
	
				trextimer->start(); // 다시 달린다 
				timer->start(); // 선인장 다시 만들기
				scoretimer->start(); // score 측정	
				restart = true;
			}
		}
		return true;
		});

	trextimer->setOnTimerCallback([&](TimerPtr trexT)->bool {
		if (i % 2 == 1) {
			trex->setImage("image/main-character2.png");
			i++;
		}
		else {
			trex->setImage("image/main-character1.png");
			i++;
		}
		if (1) {
			trexT->set(0.05f);
			trexT->start();
		}
		return true;
		}); // 공룡 서있을때 다리 움직이기
	trextimer->start();


	trextimer2->setOnTimerCallback([&](TimerPtr trexT2)->bool {
		if (i % 2 == 1) {
			trex->setImage("image/main-character5.png");
			i++;
		}
		else {
			trex->setImage("image/main-character6.png");
			i++;
		}
		if (1) {
			trexT2->set(0.05f);
			trexT2->start();
		}
		return true;
		}); // 공룡 앉았을때 다리 움직이기




	scoretimer->setOnTimerCallback([&](TimerPtr scoreT)->bool { // 선인장만들기
		score++;
		if (dead == false) {
			scoreT->set(0.02f);
			scoreT->start();
		}
		else {
			if (highscore < score) {
				highscore = score;
				string highscorestr = "Highscore is " + to_string(highscore) + " And Score is " + to_string(score);
				showMessage(highscorestr);
				score = 0;
			}
			else {
				string scorestr = "Score is " + to_string(score);
				showMessage(scorestr);
				score = 0;
			}
		}
		return true;
		}); // 점수 세기 
	scoretimer->start();

	//선인장 만들기
	timer->setOnTimerCallback([&](TimerPtr t)->bool {
		tree_X = 1200;
		tree[count] = Tree::create("image/cactus1.png", "image/cactus2.png", scene, tree_X, tree_Y, rand() % 2 == 1);
		tree[count]->hide();
		count++;
		if (count < 100) {
			t->set(0.001f);
			t->start();
		}
		if (restart == true) {
			t->set(0.001f);
			t->start();
			if (count > 1) {
				treeMakingTimer->start(); // 선인장 움직임 
				Timer2->start(); // 선인장 움직임
			}
		}
		return true;
		});
	timer->start();
	// 선인장 만들기 끝

	//선인장 움직이기
	Timer2->setOnTimerCallback([&](TimerPtr secondtimer)->bool {
		if (count3 >= 1) {
			tree[count3]->show();
			tree_X3 = tree_X3 - 30;
			tree[count3]->locate(scene, tree_X3, tree_Y);
			if (tree_X3 < -40) {
				count3 = count3 + 2;
				tree_X3 = 1200;
			}
		}
		else if (count3 < 1 ) {
			count3++;
		}
		if (dead == false && (tree_X2 > TREX_LEAST && tree_X2 < TREX_HIGH || tree_X3> TREX_LEAST && tree_X3 < TREX_HIGH) && (trex_Y < TREX_NOT_DIE)) {
			gameover->show();
			restartB->show();
			trextimer->stop();
			trextimer2->stop();
			timer->stop();
			Timer2->stop();
			treeMakingTimer->stop();
			dead = true;
			trex->setImage("image/main-character4.png");
		}
		if (dead == false && count3 < 100) {
			if (count3 < 15) {
				secondtimer->set(0.04f);
				secondtimer->start();
			}

			else if (count3 >= 15 && count3 <= 50) {
				secondtimer->set(0.02f);
				secondtimer->start();
			}
			else {
				secondtimer->set(0.01f);
				secondtimer->start();
			}
		}
		if (dead == false && restart == true) {
			restart = false;
			secondtimer->set(0.005f);
			secondtimer->start();
		}
		return true;
		});
	Timer2->start();

	treeMakingTimer->setOnTimerCallback([&](TimerPtr makingTimer)->bool {
		tree[count2]->show();
		tree_X2 = tree_X2 - 20;
		tree[count2]->locate(scene, tree_X2, tree_Y);
		if (tree_X2 < -40) {
			count2 = count2 + 2;
			tree_X2 = 1200;
		}
		if (dead == false && (tree_X2 > TREX_LEAST && tree_X2 < TREX_HIGH || tree_X3> TREX_LEAST && tree_X3 < TREX_HIGH) && (trex_Y < TREX_NOT_DIE)) {
			gameover->show();
			restartB->show();
			trextimer->stop();
			trextimer2->stop();
			Timer2->stop();
			treeMakingTimer->stop();
			timer->stop();
			dead = true;
			trex->setImage("image/main-character4.png");
		}
		if (dead == false && count2 < 100) {
			if (count2 < 15) {
				makingTimer->set(0.05f);
				makingTimer->start();
			}

			else if (count2 >= 15 && count3 <= 50) {
				makingTimer->set(0.03f);
				makingTimer->start();
			}
			else {
				makingTimer->set(0.02f);
				makingTimer->start();
			}
		}
		if (dead == false && restart == true) {
			restart = false;
			makingTimer->set(0.03f);
			makingTimer->start();
			restarttimer->stop();
		}
		return true;
		});
	treeMakingTimer->start();
	//선인장 움직이기 끝

	

	jumptimer->setOnTimerCallback([&](TimerPtr jumpT)->bool {
		if (ground == false && goingup == true && trex_Y < TREX_JUMP_Y) {
			trex_Y = trex_Y + 10;
			trex->locate(scene, trex_X, trex_Y);
		}
		else {
			goingup = false;
			if (trex_Y > TREX_DEFAULT_Y) {
				trex_Y = trex_Y - 10;
				trex->locate(scene, trex_X, trex_Y);
			}
			else {
				out = true;
				goingup = true;
				ground = true;
				jumpSound->stop();
			}
		}
		if (!out) {
			jumpT->set(0.01f);
			jumpT->start();
		}

		return true;
		// 3초동안 올라갓다 내려갓다
		});
	scene->setOnKeyboardCallback([&](ScenePtr scene, int key, bool pressed)->bool {
		if (dead == false && !pressed) {
			trextimer2->stop();
			if (out == true) {
				trex_Y = TREX_DEFAULT_Y;
				trex->setImage("image/main-character1.png");
				trex->locate(scene, trex_X, TREX_DEFAULT_Y);
				trextimer->start();
			}
			else {
				trextimer->start();
			}
		}
		else if (ground == true && dead == false && key == 75) {
			jumpSound->play(true);
			trextimer2->stop();
			trextimer->stop();
			trex->setImage("image/main-character1.png");
			out = false;
			ground = false;
			jumptimer->start();
		}
		else if (dead == false && key == 85) {
			trex_Y = TREX_DEFAULT_Y;
			trextimer->stop();
			trex->setImage("image/main-character5.png");
			trex->locate(scene, trex_X, TREX_DEFAULT_Y);
			trextimer2->start();
			out = true;
		}
		return true;
		});
	startGame(scene);
}


