#define _CRT_SECURE_NO_WARNINGS
#include<iostream>
#include <fstream>
#include <algorithm>
#include<Windows.h>
#include<conio.h>
#include<vector>
#include<string>
#include<iomanip>
#include<fstream>
#include<ctime>
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")
using namespace std;

#define MAX_DATA 1000
#define MAGIC_KEY 224
#define SPACE 32
#define KEY_NUM 4
#define LIFE 3
#define MAX_LEVEL 11

enum MENU
{
    GAMESTART = 0,
    INFO,
    QUIT
};
enum SOUND
{
    BOUNCE = 0,
    CHILL,
    DARK,
    ELEC,
    LOFI,
    NERVOUS,
    SLOW
};

enum KEYBOARD
{
    UP = 72,
    LEFT = 75,
    RIGHT = 77,
    DOWN = 80
};

class GameData {//DataSetting
public:
    int gamescore;
    int gamesec;
    string gamename;

    GameData() {}
    GameData(int score, int sec, string gamer) : gamescore(score), gamesec(sec)
    {
        gamename = gamer;
    }

    bool operator <(GameData& gamedata)
    {
        return this->gamescore > gamedata.gamescore;
    }
};

void Life(const int x)
{
    for (int i = 0; i < x; i++)
    {
        cout << "♥";
    }
}

bool desc(int cmp1, int cmp2)//DataSorting
{
    return cmp1 > cmp2;
}

//Cursor move
void gotoxy(int x, int y)
{
    COORD Pos;
    Pos.X = 2 * x;
    Pos.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Pos);
}

//title, console size
void SetConsoleView()
{
    system("mode con:cols=50 lines=20");
    system("title DanceDance");
}

//-----------Draw-----------------
void DrawReadyGame()
{
    system("cls");
    gotoxy(5, 2);
    cout << "******************************";
    gotoxy(5, 3);
    cout << "*        Dance Dance         *";
    gotoxy(5, 4);
    cout << "******************************";
    gotoxy(10, 8);
    cout << "GameStart";
    gotoxy(10, 9);
    cout << "GameInfo";
    gotoxy(10, 10);
    cout << "Quit" << endl;
}

void DrawInfoGame()
{
    system("cls");
    gotoxy(1, 3);
    cout << "*******************************************";
    gotoxy(1, 4);
    cout << "|Developer - BlockDMask";
    gotoxy(1, 5);
    cout << "|Blog - https://blockdmask.tistory.com/";
    gotoxy(1, 8);
    cout << "|Thank you.";
    gotoxy(1, 9);
    cout << "*******************************************";
    gotoxy(1, 10);
    cout << "|Music - https://www.youtube.com/HYPMUSIC";
}

void DrawSoundSet()
{
    system("cls");
    gotoxy(1, 3);
    cout << "**********************************************";
    gotoxy(9, 4);
    cout << "Select Sound";
    gotoxy(1, 5);
    cout << "**********************************************";
    gotoxy(10, 8);
    cout << "BOUNCE";
    gotoxy(10, 9);
    cout << "CHILL";
    gotoxy(10, 10);
    cout << "DARK";
    gotoxy(10, 11);
    cout << "ELEC";
    gotoxy(10, 12);
    cout << "LOFI";
    gotoxy(10, 13);
    cout << "NERVOUS";
    gotoxy(10, 14);
    cout << "SLOW" << endl;
}

void DrawStartGame(const int life, const int score, const int STime, const string questionStr, const string answerStr)
{
    system("cls");
    gotoxy(2, 1);
    cout << "*******************************************";
    gotoxy(4, 3);
    cout << "Life : "; Life(life);
    gotoxy(4, 4);
    cout << "Score : " << score;
    gotoxy(4, 5);
    cout << "시간 제한 : " << STime;
    gotoxy(4, 8);
    cout << "Q : " << questionStr;
    gotoxy(4, 10);
    cout << "A : " << answerStr;
    gotoxy(4, 12);
    cout << "press SPACE! after input done.";
    gotoxy(2, 18);
    cout << "*******************************************" << endl;
}

string DrawScoreSet(const int score, const int playTime)
{
    system("cls");
    gotoxy(2, 1);
    cout << "*******************************************";
    gotoxy(4, 7);
    cout.setf(ios::left);
    cout << setw(10) << "Score" << setw(10) << "Sec" << setw(10) << "Gamer";
    gotoxy(4, 9);
    cout << setw(10) << score << setw(10) << playTime;
    gotoxy(4, 12);
    cout << "Input your three letter initials.";
    gotoxy(4, 14);
    cout << "press ENTER! after input done.";
    gotoxy(2, 18);
    cout << "*******************************************" << endl;
    string gamer;
    gotoxy(14, 9);
    cin >> setw(10) >> gamer;
    return gamer;
}

void DrawScoreBoard(GameData* gamedata)
{
    system("cls");
    gotoxy(2, 1);
    cout << "*******************************************";
    gotoxy(4, 3);
    cout.setf(ios::left);
    cout << setw(10) << "Rank" << setw(10) << "Score" << setw(10) << "Sec" << setw(10) << "Gamer";
    for (int i = 0; i < 5; i++)
    {
        gotoxy(4, 6 + 2 * i);
        if (gamedata[i].gamescore < 0 || gamedata[i].gamescore > 100000)
        {
            gamedata[i].gamescore = 0;
            gamedata[i].gamesec = 0;
            gamedata[i].gamename = '---';

        }
        cout << setw(10) << i + 1 << setw(10) << gamedata[i].gamescore << setw(10) << gamedata[i].gamesec << setw(10) << gamedata[i].gamename;
    }
    gotoxy(4, 16);
    cout << "press SPACE! to quit.";
    gotoxy(2, 18);
    cout << "*******************************************" << endl;
}

//게임 오버 그리기
void DrawGameOver(const int playTime)
{
    gotoxy(8, 8);
    cout << "-------------------";
    gotoxy(8, 9);
    cout << "| G A M E O V E R |";
    gotoxy(8, 10);
    cout << " " << playTime << " sec";
    gotoxy(8, 11);
    cout << "-------------------";
    system("pause>null");
}

//커서 움직이는것 출력
void DrawUserCursor(int& y)
{
    if (y <= 0)
    {
        y = 0;
    }
    else if (y >= 2)
    {
        y = 2;
    }

    gotoxy(9, 8 + y);
    cout << ">";
}

void DrawUserCursorExtend(int& y)
{
    if (y <= 0)
    {
        y = 0;
    }
    else if (y >= 6)
    {
        y = 6;
    }

    gotoxy(9, 8 + y);
    cout << ">";
}

const char* SoundSet()
{
    int y = 0;
    int input = 0;
    while (true)
    {
        DrawSoundSet();
        DrawUserCursorExtend(y);
        input = _getch();
        //→←↑↓
        if (input == MAGIC_KEY)
        {
            switch (_getch())
            {
            case UP:
                --y;
                break;
            case DOWN:
                ++y;
                break;
            }
        }
        else if (input == SPACE)
        {
            switch (y)
            {
            case 0:
                return "BOUNCE.wav";
            case 1:
                return "CHILL.wav";
            case 2:
                return "DARK.wav";
            case 3:
                return "ELEC.wav";
            case 4:
                return "LOFI.wav";
            case 5:
                return "NERVOUS.wav";
            case 6:
                return "SLOW.wav";
            }
        }
    }
}

//-----------Func-----------------
MENU ReadyGame()
{
    int y = 0;
    int input = 0;
    while (true)
    {
        DrawReadyGame();
        DrawUserCursor(y);
        input = _getch();
        //→←↑↓
        if (input == MAGIC_KEY)
        {
            switch (_getch())
            {
            case UP:
                --y;
                break;
            case DOWN:
                ++y;
                break;
            }
        }
        else if (input == SPACE)
        {
            switch (y)
            {
            case 0:
                return GAMESTART;
            case 1:
                return INFO;
            case 2:
                return QUIT;
            }
        }
    }
}

void InfoGame()
{
    DrawInfoGame();
    system("pause>null");
}

void SetQuestion(vector<int>& questionVec, int level)
{
    if (level > MAX_LEVEL)
    {
        level = MAX_LEVEL;
    }

    int num = 0;
    srand((unsigned int)time(NULL));
    for (int i = 0; i < level; ++i)    //화살표의 개수 (문제 난이도)
    {
        num = rand() % KEY_NUM;    //화살표 종류.
        switch (num)
        {
        case 0:
            questionVec.push_back(UP);
            break;
        case 1:
            questionVec.push_back(RIGHT);
            break;
        case 2:
            questionVec.push_back(LEFT);
            break;
        case 3:
            questionVec.push_back(DOWN);
            break;
        }
    }
}

void VectorToString(const vector<int> v, string& str)
{
    for (int i = 0; i < static_cast<int>(v.size()); ++i)
    {
        switch (v[i])
        {
        case UP:
            str += "↑ ";
            break;
        case DOWN:
            str += "↓ ";
            break;
        case LEFT:
            str += "← ";
            break;
        case RIGHT:
            str += "→ ";
            break;
        }
    }
}

bool CheckAnswer(const vector<int> questionVec, const vector<int> answerVec)
{
    //숫자의 배열이 같다.
    //길이 체크
    if (questionVec.size() != answerVec.size())
    {
        //길이 다르네
        return false;
    }

    //내용물 체크
    for (int i = 0; i < static_cast<int>(questionVec.size()); ++i)
    {
        if (questionVec[i] != answerVec[i])
        {
            //다른게 있네.
            return false;
        }
    }
    return true;
}

void StartGame()
{
    const char* sound = SoundSet();
    wchar_t text1[30];
    mbstowcs(text1, sound, strlen(sound) + 1);
    LPCWSTR test = text1;
    PlaySound(test, NULL, SND_NODEFAULT | SND_ASYNC | SND_LOOP);
    int life = LIFE;
    int score = 0;
    int STime = 0;
    int time1 = 5;
    //재생했을때 현재시간.
    clock_t startTime, endTime, LTime;
    startTime = clock();
    //→←↑↓, d a w s
    //문제
    vector<int> questionVec;
    string questionStr = "";
    //답안지
    vector<int> answerVec;
    string answerStr = "";
    int firstInput = 0;
    int secondInput = 0;
    while (true)
    {
        int level = (score / 30) + 1;
        //문제를 세팅
        SetQuestion(questionVec, level);
        //문제를 보여주기.
        VectorToString(questionVec, questionStr);
        while (true)
        {
            //1문제를 가지고 문제를 푼다.
            LTime = clock();
            STime = static_cast<int>((LTime - startTime) / CLOCKS_PER_SEC);
            int t = time1 - STime;
            DrawStartGame(life, score, t, questionStr, answerStr);
            if (life == 0)
            {
                //게임 오버일때 현재시간
                endTime = clock();
                int playTime = static_cast<int>((endTime - startTime) / CLOCKS_PER_SEC);
                string gamer = DrawScoreSet(score, playTime);//append
                cout << gamer << endl;
                GameData gamedata[MAX_DATA];

                ofstream fout;
                ifstream fin;
                fout.open("Board.txt", ios::app);
                fout << score << ' ';
                fout << playTime << ' ';
                fout << gamer << endl;

                fin.open("Board.txt");
                int i = 0;
                while (!fin.eof())
                {
                    fin >> gamedata[i].gamescore;
                    fin >> gamedata[i].gamesec;
                    fin >> gamedata[i].gamename;
                    i++;
                }
                int k = i;
                fout.close();
                fin.close();
                sort(gamedata, gamedata + k);
                DrawScoreBoard(gamedata);
                int input = 0;
                while (true)
                {
                    input = _getch();
                    if (input == SPACE)
                    {
                        break;
                    }
                }
                DrawGameOver(playTime);
                PlaySound(NULL, NULL, 0);
                return;
            }
            if (t == 0) // 시간초 끝날때 게임오버로 하고싶으면 lief=0으로 만들면 됨
            {
                --life;
                time1 += 5;
                questionVec.clear();
                questionStr = "";
                answerVec.clear();
                answerStr = "";
                break;
            }
            if (_kbhit())
            {
                firstInput = _getch();
                if (firstInput == MAGIC_KEY)
                {
                    secondInput = _getch();
                    answerVec.push_back(secondInput);
                    switch (secondInput)
                    {
                    case UP:
                        answerStr += "↑ ";
                        break;
                    case DOWN:
                        answerStr += "↓ ";
                        break;
                    case LEFT:
                        answerStr += "← ";
                        break;
                    case RIGHT:
                        answerStr += "→ ";
                        break;
                    }
                }
                else if (firstInput == SPACE)
                {
                    //답안 제출
                    //답안 확인
                    if (CheckAnswer(questionVec, answerVec))
                    {
                        score += 10;
                        if (t > 0)
                        {
                            time1 += 2;
                        }
                    }
                    else
                    {
                        //틀렸다.
                        --life;
                        score -= 5;
                        if (score < 0)
                        {
                            score = 0;
                        }
                    }
                    questionVec.clear();
                    questionStr = "";
                    answerVec.clear();
                    answerStr = "";
                    break;
                }
            }
        }
    }
}

int main(void)
{
    SetConsoleView();
    while (true)
    {
        switch (ReadyGame())
        {
        case GAMESTART:
            StartGame();
            break;
        case INFO:
            InfoGame();
            break;
        case QUIT:
            return 0;
        }
    }
    return 0;
}