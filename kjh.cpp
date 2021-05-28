void Life(const int x)
{
    for (int i = 0; i < x; i++)
    {
        cout << "��";
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
    cout << "�ð� : " << STime;
    gotoxy(17, 6);
    cout << level << " �ܰ�";
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
    for (int i = 0; i < level; ++i)    //ȭ��ǥ�� ���� (���� ���̵�)
    {
        num = rand() % KEY_NUM;    //ȭ��ǥ ����.
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
    //��������� ����ð�.
    clock_t startTime, endTime, LTime;
    startTime = clock();

    //�����, d a w s
    //����
    vector<int> questionVec;
    string questionStr = "";
    //�����
    vector<int> answerVec;
    string answerStr = "";
    int firstInput = 0;
    int secondInput = 0;
    while (true)
    {
        //������ ����
        SetQuestion(questionVec, level);
        //������ �����ֱ�.
        VectorToString(questionVec, questionStr);
        while (true)
        {
            //1������ ������ ������ Ǭ��.
            LTime = clock();
            STime = static_cast<int>((LTime - startTime) / CLOCKS_PER_SEC);
            int t = time1 - STime;
            DrawStartGame(life, score, combo, t, level, questionStr, answerStr);
            if (life == 0)
            {
                //���� �����϶� ����ð�
                endTime = clock();
                int playTime = static_cast<int>((endTime - startTime) / CLOCKS_PER_SEC);
                string gamer = DrawScoreSet(score, playTime);//append
                GameData gamedata[MAX_DATA];
                gamer.resize(3); //�� �� ���޵Ǵ��� 3���ڸ� ����
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
                int k = i - 1;         //�������� endl �� �־ �ٷ� eof�� ã�����ϰ� �ѹ� �� ������ ���Ƽ� i�� 1��ŭ �� ŭ
                sort(gamedata, gamedata + k);       //������ ����
                if (k <= 5)
                {
                }
                else
                {
                    k = 5;
                }
                fout.open("Board.txt", ios::trunc); // ���� txt ���� �� ������ ������ ����
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
            if (t == 0) // �ð��� ������ ���ӿ����� �ϰ������ lief=0���� ����� ��
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
                        answerStr += "�� ";
                        break;
                    case DOWN:
                        answerStr += "�� ";
                        break;
                    case LEFT:
                        answerStr += "�� ";
                        break;
                    case RIGHT:
                        answerStr += "�� ";
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
                    //��� ����
                    //��� Ȯ��
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
                        //Ʋ�ȴ�.
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