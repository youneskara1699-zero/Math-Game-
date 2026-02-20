#include <iostream>  
#include <cstdlib>   
#include <ctime>     
#include <string>
#include <limits>
using namespace std;  

enum enQuestionsLevel { EasyLevel = 1, MedLevel = 2, HardLevel = 3, Mix = 4 };
enum enOperationType { Add = 1, Sub = 2, Mult = 3, Div = 4, MixOp = 5 };
enum enGameMode { QuickMode = 1, CustomMode = 2 };

int RandomNumber(int From, int To)
{
    return rand() % (To - From + 1) + From;
}

string GetOpTypeSymbol(enOperationType OpType)
{
    switch (OpType)
    {
    case enOperationType::Add:
        return "+";
    case enOperationType::Sub:
        return "-";
    case enOperationType::Mult:
        return "x";
    case enOperationType::Div:
        return "/";
    default:
        return "Mix";
    }
}

enOperationType GetRandomOperationType()
{
    return (enOperationType)RandomNumber(1, 4);
}

string GetQuestionLevelText(enQuestionsLevel QuestionLevel)
{
    string arrQuestionLevelText[4] = { "Easy","Medium","Hard","Mixed" };
    return arrQuestionLevelText[QuestionLevel - 1];
}

void SetScreenColor(bool Right)
{
    if (Right)
        system("color 2F"); 
    else
    {
        system("color 4F"); 
        cout << "\a"; 
    }
}

void ResetScreenColor()
{
    system("color 07");
}

int SimpleCalculator(int Number1, int Number2, enOperationType OpType)
{
    switch (OpType)
    {
    case enOperationType::Add:
        return Number1 + Number2;
    case enOperationType::Sub:
        return Number1 - Number2;
    case enOperationType::Mult:
        return Number1 * Number2;
    case enOperationType::Div:
        return (Number2 != 0) ? (Number1 / Number2) : 0;
    default:
        return Number1 + Number2;
    }
}

struct stQuestion
{
    int Number1 = 0;
    int Number2 = 0;
    enOperationType OperationType;
    enQuestionsLevel QuestionLevel;
    int CorrectAnswer = 0;
    int PlayerAnswer = 0;
    bool AnswerResult = false;
};

struct stQuizz
{
    stQuestion QuestionList[100];
    short NumberOfQuestions;
    enQuestionsLevel QuestionsLevel;
    enOperationType OpType;
    short NumberOfWrongAnswers = 0;
    short NumberOfRightAnswers = 0;
    bool isPass = false;
    int Score = 0;
};

int ReadNumberInRange(string Message, int From, int To)
{
    int Number;
    do
    {
        cout << Message;
        cin >> Number;
        
        if(cin.fail())
        {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input! Please enter a number between " << From << " and " << To << ".\n";
            continue;
        }
        
        if(Number < From || Number > To)
            cout << "Please enter a number between " << From << " and " << To << ".\n";
            
    } while (Number < From || Number > To);
    
    return Number;
}

void ShowGameHeader()
{
    cout << "========================================\n";
    cout << "          MATH CHALLENGE GAME          \n";
    cout << "========================================\n\n";
}

void ShowMainMenu()
{
    cout << "MAIN MENU:\n";
    cout << "1. Quick Play (5 Questions - Easy - Mixed Operations)\n";
    cout << "2. Custom Game\n";
    cout << "3. Exit\n";
    cout << "========================================\n";
}

void ShowDifficultyMenu()
{
    cout << "\nSELECT DIFFICULTY LEVEL:\n";
    cout << "1. Easy (Numbers: 1-10)\n";
    cout << "2. Medium (Numbers: 10-50)\n";
    cout << "3. Hard (Numbers: 50-100)\n";
    cout << "4. Mixed (Random Difficulty)\n";
}

void ShowOperationMenu()
{
    cout << "\nSELECT OPERATION TYPE:\n";
    cout << "1. Addition (+)\n";
    cout << "2. Subtraction (-)\n";
    cout << "3. Multiplication (x)\n";
    cout << "4. Division (/)\n";
    cout << "5. Mixed Operations\n";
}

stQuestion GenerateQuestion(enQuestionsLevel QuestionLevel, enOperationType OpType)
{
    stQuestion Question;

    if (QuestionLevel == enQuestionsLevel::Mix)
        QuestionLevel = (enQuestionsLevel)RandomNumber(1, 3);

    if (OpType == enOperationType::MixOp)
        OpType = GetRandomOperationType();

    Question.OperationType = OpType;

    switch (QuestionLevel)
    {
    case enQuestionsLevel::EasyLevel:
        Question.Number1 = RandomNumber(1, 10);
        Question.Number2 = RandomNumber(1, 10);
        break;
    case enQuestionsLevel::MedLevel:
        Question.Number1 = RandomNumber(10, 50);
        Question.Number2 = RandomNumber(10, 50);
        break;
    case enQuestionsLevel::HardLevel:
        Question.Number1 = RandomNumber(50, 100);
        Question.Number2 = RandomNumber(50, 100);
        break;
    }

    if (OpType == enOperationType::Div)
    {
        if (Question.Number2 == 0) Question.Number2 = 1;
        Question.Number1 = Question.Number1 * Question.Number2; 
    }

    Question.CorrectAnswer = SimpleCalculator(Question.Number1, Question.Number2, Question.OperationType);
    Question.QuestionLevel = QuestionLevel;

    return Question;
}

void AskAndCorrectQuestionListAnswers(stQuizz& Quizz)
{
    for (short QuestionNumber = 0; QuestionNumber < Quizz.NumberOfQuestions; QuestionNumber++)
    {
        cout << "\n========================================\n";
        cout << "Question [" << QuestionNumber + 1 << "/" << Quizz.NumberOfQuestions << "]\n";
        cout << "Difficulty: " << GetQuestionLevelText(Quizz.QuestionList[QuestionNumber].QuestionLevel) << "\n";
        cout << Quizz.QuestionList[QuestionNumber].Number1 << " "
            << GetOpTypeSymbol(Quizz.QuestionList[QuestionNumber].OperationType) << " "
            << Quizz.QuestionList[QuestionNumber].Number2 << " = ";

        cin >> Quizz.QuestionList[QuestionNumber].PlayerAnswer;

        if (Quizz.QuestionList[QuestionNumber].PlayerAnswer == Quizz.QuestionList[QuestionNumber].CorrectAnswer)
        {
            SetScreenColor(true);
            cout << "\n✓ Correct!\n";
            Quizz.NumberOfRightAnswers++;
            Quizz.Score += 10;
        }
        else
        {
            SetScreenColor(false);
            cout << "\n✗ Wrong! The correct answer is: " 
                 << Quizz.QuestionList[QuestionNumber].CorrectAnswer << endl;
            Quizz.NumberOfWrongAnswers++;
        }
        
        ResetScreenColor();
    }
    
    Quizz.isPass = (Quizz.NumberOfRightAnswers >= (Quizz.NumberOfQuestions / 2));
}

void DisplayQuizResults(stQuizz Quizz)
{
    cout << "\n========================================\n";
    cout << "            QUIZ RESULTS                \n";
    cout << "========================================\n";
    
    cout << "\nDifficulty Level: " << GetQuestionLevelText(Quizz.QuestionsLevel) << endl;
    cout << "Operation Type: " << GetOpTypeSymbol(Quizz.OpType) << endl;
    cout << "Total Questions: " << Quizz.NumberOfQuestions << endl;
    cout << "----------------------------------------\n";
    
    cout << "Correct Answers: " << Quizz.NumberOfRightAnswers << endl;
    cout << "Wrong Answers: " << Quizz.NumberOfWrongAnswers << endl;
    cout << "Score: " << Quizz.Score << " points\n";
    cout << "----------------------------------------\n";
    
    if (Quizz.isPass)
    {
        cout << "CONGRATULATIONS! You Passed the Quiz! 🎉\n";
    }
    else
    {
        cout << "Keep Practicing! You'll do better next time!\n";
    }
    
    cout << "========================================\n\n";
}

void PlayQuickGame()
{
    stQuizz Quizz;
    Quizz.NumberOfQuestions = 5;
    Quizz.QuestionsLevel = enQuestionsLevel::EasyLevel;
    Quizz.OpType = enOperationType::MixOp;
    
    cout << "\nStarting Quick Game...\n";
    cout << "Questions: " << Quizz.NumberOfQuestions << endl;
    cout << "Difficulty: " << GetQuestionLevelText(Quizz.QuestionsLevel) << endl;
    cout << "Operations: " << GetOpTypeSymbol(Quizz.OpType) << endl;
    cout << "========================================\n";

    for (short QuestionNumber = 0; QuestionNumber < Quizz.NumberOfQuestions; QuestionNumber++)
    {
        Quizz.QuestionList[QuestionNumber] = GenerateQuestion(Quizz.QuestionsLevel, Quizz.OpType);
    }

    AskAndCorrectQuestionListAnswers(Quizz);
    DisplayQuizResults(Quizz);
}

void PlayCustomGame()
{
    stQuizz Quizz;
    
    cout << "\nCUSTOM GAME SETTINGS:\n";
    
    Quizz.NumberOfQuestions = ReadNumberInRange("Enter number of questions (1-20): ", 1, 20);
    
    ShowDifficultyMenu();
    Quizz.QuestionsLevel = (enQuestionsLevel)ReadNumberInRange("Select difficulty (1-4): ", 1, 4);
    
    ShowOperationMenu();
    Quizz.OpType = (enOperationType)ReadNumberInRange("Select operation type (1-5): ", 1, 5);
    
    cout << "\nStarting Custom Game...\n";
    cout << "Questions: " << Quizz.NumberOfQuestions << endl;
    cout << "Difficulty: " << GetQuestionLevelText(Quizz.QuestionsLevel) << endl;
    cout << "Operations: " << GetOpTypeSymbol(Quizz.OpType) << endl;
    cout << "========================================\n";

    for (short QuestionNumber = 0; QuestionNumber < Quizz.NumberOfQuestions; QuestionNumber++)
    {
        Quizz.QuestionList[QuestionNumber] = GenerateQuestion(Quizz.QuestionsLevel, Quizz.OpType);
    }

    AskAndCorrectQuestionListAnswers(Quizz);
    DisplayQuizResults(Quizz);
}

void PlayMathGame()
{
    int Choice;
    bool PlayAgain = true;
    
    while(PlayAgain)
    {
        ShowGameHeader();
        ShowMainMenu();
        
        Choice = ReadNumberInRange("Enter your choice (1-3): ", 1, 3);
        
        switch(Choice)
        {
            case QuickMode:
                PlayQuickGame();
                break;
            case CustomMode:
                PlayCustomGame();
                break;
            case 3:
                cout << "\nThank you for playing Math Challenge Game!\n";
                PlayAgain = false;
                continue;
        }
        
        if(PlayAgain)
        {
            cout << "\nDo you want to play again?\n";
            cout << "1. Yes\n";
            cout << "2. No\n";
            int PlayChoice = ReadNumberInRange("Enter your choice (1-2): ", 1, 2);
            PlayAgain = (PlayChoice == 1);
            
            if(PlayAgain)
            {
                system("cls");
            }
        }
    }
}

int main()
{
    srand((unsigned)time(NULL)); 
    PlayMathGame();  
    
    cout << "\nPress Enter to exit...";
    cin.ignore();
    cin.get();
    
    return 0;  
}