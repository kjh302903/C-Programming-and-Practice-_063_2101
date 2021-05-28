void Life(const int x)
{
    for (int i = 0; i < x; i++)
    {
        cout << "♥";
    }
}

//-----------Draw-----------------


void DrawStartGame(const int life, const int score, const int combo, const int STime, const int level, const string questionStr, const string answerStr)
{
    system("cls");
    gotoxy(2, 1);
    cout << "*******************************************";
    gotoxy(4, 3);
    cout << "Life : "; Life(life);
    gotoxy(4, 4);
    cout << "Score : " << score;
    gotoxy(4, 5);
    cout << "Combo : " << combo;
    gotoxy(4, 6);
    cout << "시간 : " << STime;
    gotoxy(17, 6);
    cout << level << " 단계";
    gotoxy(4, 8);
    cout << "Q : " << questionStr;
    gotoxy(4, 10);
    cout << "A : " << answerStr;
    gotoxy(4, 12);
    cout << "press D or d(KeyBoard) if you want delete.";
    gotoxy(4, 13);
    cout << "press SPACE! after input done.";
    gotoxy(2, 18);
    cout << "*******************************************" << endl;
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

void StartGame()
{
    const char* sound = SoundSet();
    wchar_t text1[30];
    mbstowcs(text1, sound, strlen(sound) + 1);
    LPCWSTR test = text1;
    PlaySound(test, NULL, SND_NODEFAULT | SND_ASYNC | SND_LOOP);
    int life = LIFE;
    int score = 0;
    int combo = 0;
    int STime = 0;
    int time1 = 5;
    int exp = 0;
    int level = 1;
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
            DrawStartGame(life, score, combo, t, level, questionStr, answerStr);
            if (life == 0)
            {
                //게임 오버일때 현재시간
                endTime = clock();
                int playTime = static_cast<int>((endTime - startTime) / CLOCKS_PER_SEC);
                string gamer = DrawScoreSet(score, playTime);//append
                GameData gamedata[MAX_DATA];
                gamer.resize(3); //잘 못 전달되더라도 3글자만 받음
                ofstream fout;
                ifstream fin;
                fout.open("Board.txt", ios::app);
                fout << score << ' ';
                fout << playTime << ' ';
                fout << gamer << endl;
                fout.close();
                fin.open("Board.txt");
                int i = 0;
                while (!fin.eof())
                {
                    fin >> gamedata[i].gamescore;
                    fin >> gamedata[i].gamesec;
                    fin >> gamedata[i].gamename;
                    i++;
                }
                int k = i - 1;         //마지막에 endl 을 넣어서 바로 eof를 찾지못하고 한번 더 루프를 돌아서 i가 1만큼 더 큼
                sort(gamedata, gamedata + k);       //데이터 정렬
                if (k <= 5)
                {
                }
                else
                {
                    k = 5;
                }
                fout.open("Board.txt", ios::trunc); // 새로 txt 생성 후 정렬한 데이터 저장
                int j = 0;
                while (j < k)
                {
                    fout << gamedata[j].gamescore << ' ';
                    fout << gamedata[j].gamesec << ' ';
                    fout << gamedata[j].gamename << endl;
                    j++;
                }
                fout.close();
                fin.close();

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
                if (level <= 3)
                {
                    time1 += 3;
                }
                else if (level <= 7)
                {
                    time1 += 7;
                }
                else
                    time1 += level;
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
                else if (firstInput == Delete) {
                    if (answerStr.size() == 0) {

                    }
                    else {
                        answerStr.pop_back();
                        answerStr.pop_back();
                        answerStr.pop_back();
                        answerVec.pop_back();
                    }
                }

                else if (firstInput == SPACE)
                {
                    //답안 제출
                    //답안 확인
                    if (CheckAnswer(questionVec, answerVec))
                    {
                        score = score + questionVec.size() + combo;
                        combo = combo + questionVec.size();
                        ++exp;
                        if (exp % 3 == 0)
                        {
                            ++level;
                        }
                        if (t > 0)
                        {
                            time1 += 2;
                        }
                    }
                    else
                    {
                        //틀렸다.
                        combo = 0;
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