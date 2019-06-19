#include "Window_Main.h"

Window_Main::Window_Main(QPoint Start1_isd, QPoint End1_isd, QPoint Start2_isd, QPoint End2_isd, QPoint Start3_isd, QPoint End3_isd, QWidget *parent) : QWidget(parent)
{
	T2 = new Thread_Timer();
	ui.setupUi(this);
	setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);//��ֹ���
	setFixedSize(this->width(), this->height());//��ֹ�ı䴰�ڴ�С
	setWindowIcon(QIcon(":/Zhihuishu_Aided/main_ico.ico"));//������ͼ��
	setWindowTitle(QStringLiteral("�ǻ�������"));//����������
	Start1 = Start1_isd, End1 = End1_isd, Start2 = Start2_isd, End2 = End2_isd, Start3 = Start3_isd, End3 = End3_isd;//�������������뱾��ĳ�Ա
	//����Start End 1 2���㴰�����������
	Start1 -= Start3, End1 = Start1 + (End1_isd - Start1_isd);
	Start2 -= Start3, End2 = Start2 + (End2_isd - Start2_isd);
	//==================================

	//׼��SendMessage
	GoalWindowName_Str = "�γ�ѧϰ_�ǻ��� - Google Chrome";
	WCHAR LPCWSTR[256];
	memset(LPCWSTR, 0, sizeof(LPCWSTR));
	MultiByteToWideChar(CP_ACP, 0, GoalWindowName_Str, strlen(GoalWindowName_Str) + 1, LPCWSTR, sizeof(LPCWSTR) / sizeof(LPCWSTR[0]));
	GoalWindow_HD = FindWindow(NULL, LPCWSTR);
	if (GoalWindow_HD == 0)
	{
		QMessageBox::information(NULL, QStringLiteral("��ʾ"), QStringLiteral("δ��⵽Ŀ�����������"), QMessageBox::Yes, QMessageBox::Yes);
	}

	//���ƶ�����̼߳����봫�������
	MiddlePoint.setX((Start1.x() + End1.x()) / 2), MiddlePoint.setY((Start1.y() + End1.y()) / 2);
	T1.MiddlePoint = this->MiddlePoint;
	T1.GoalWindow_HD = GoalWindow_HD;

	T3.Start1 = Start1, T3.End1 = End1, T3.Start2 = Start2, T3.End2 = End2;//����Ļ����̴߳��������
	T3.GoalWindow_HD = GoalWindow_HD;

	handle = (HWND)(winId());// ��ȡ��ǰ���ھ�� 
	strcpy(name_str, "Zhihuishu_Aided_STOP");
	MultiByteToWideChar(CP_ACP, 0, name_str, -1, name_tchar, 100);
	HotKeyId = GlobalAddAtom(name_tchar);//�õ��ȼ���Ψһ��ʶ
	RegisterHotKey(handle, HotKeyId, 0, VK_F8);//ע��ȫ���ȼ�F8
	qApp->installNativeEventFilter(this);//�뵱ǰ���ڹ���

	//��ʼ�߳����б�ʶ����
	T1.flag_Thread_MoveArrow = 0;
	T2->flag_Thread_Timer = 0;
	T3.flag_Thread_ScreenAnalyze = 0;

	connect(ui.pushButton_1, SIGNAL(clicked()), this, SLOT(Start(void)));//������ʼ��ť
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(close(void)));//�����˳���ť
	connect(T2, SIGNAL(Touch(void)), this, SLOT(Judge(void)));//�̼߳�����
}

Window_Main::~Window_Main()
{

}

void Window_Main::paintEvent(QPaintEvent *event)
{

}

void Window_Main::ToClick(int mode)
{
	if (T1.flag_Thread_MoveArrow == 1)
	{
		T1.flag_Thread_MoveArrow = 0;
	}
	switch (mode)
	{
	//��Ƶ�����Զ���һ��
	case 1:
	{
		//SetCursorPos(Start1.x() + 165, End1.y() + 30);
		SendMessage(GoalWindow_HD, WM_MOUSEMOVE, 0, (((End1.y() + 30) << 16) + (Start1.x() + 165)));

		//mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
		SendMessage(GoalWindow_HD, WM_LBUTTONDOWN, 0, (((End1.y() + 30) << 16) + (Start1.x() + 165)));
		SendMessage(GoalWindow_HD, WM_LBUTTONUP, 0, (((End1.y() + 30) << 16) + (Start1.x() + 165)));
	}break;
	//�����Զ��ر�
	case 2:
	{
		//ѡһ��ѡ�� ѡ��λ�ô��¶�Ӧ������Ϊ�Ե�������Ͻ�Ϊԭ�㣬ˮƽ385����ֱ200
		SendMessage(GoalWindow_HD, WM_MOUSEMOVE, 0, (((Start2.y() + 200) << 16) + (Start2.x() + 385)));
		SendMessage(GoalWindow_HD, WM_LBUTTONDOWN, 0, (((Start2.y() + 200) << 16) + (Start2.x() + 385)));
		SendMessage(GoalWindow_HD, WM_LBUTTONUP, 0, (((Start2.y() + 200) << 16) + (Start2.x() + 385)));
		Sleep(1000);
		//����ر� �رհ�ť��Ӧ������Ϊ�Ե�������Ͻ�Ϊԭ�㣬ˮƽ385����ֱ425
		SendMessage(GoalWindow_HD, WM_MOUSEMOVE, 0, (((Start2.y() + 425) << 16) + (Start2.x() + 385)));
		SendMessage(GoalWindow_HD, WM_LBUTTONDOWN, 0, (((Start2.y() + 425) << 16) + (Start2.x() + 385)));
		SendMessage(GoalWindow_HD, WM_LBUTTONUP, 0, (((Start2.y() + 425) << 16) + (Start2.x() + 385)));
	}break;
	default:break;
	}
	if (T1.flag_Thread_MoveArrow == 0)
	{
		T1.flag_Thread_MoveArrow = 1;
	}
}

void Window_Main::Start(void)
{
	T3.IfVideoEnd = 0;//��Ƶ������ʶ����
	T3.IfQuestionShow = 0;//�����ʶ����
	hide();
	MoveArrow();
	TimeBegin();
	ScreenAnalyze();
}

void Window_Main::Judge(void)
{
	if (T3.IfVideoEnd == 1)
	{
		ToClick(1);
		T3.IfVideoEnd = 0;
	}
	if (T3.IfQuestionShow == 1)
	{
		ToClick(2);
		T3.IfQuestionShow = 0;
	}
}

//���̲߳���
void Window_Main::ScreenAnalyze(void)
{
	T3.flag_Thread_ScreenAnalyze = 1;//���б�ʶ��Ϊ1
	T3.start();
}

void Window_Main::MoveArrow(void)
{
	T1.flag_Thread_MoveArrow = 1;
	T1.start();
}

void Window_Main::TimeBegin(void)
{
	T2->flag_Thread_Timer = 1;
	T2->start();
}

void Thread_Timer::run(void)
{
	while (flag_Thread_Timer)
	{
		emit Touch();
		sleep(1);
	}
}

void Thread_ScreenAnalyze::run(void)
{
	while (flag_Thread_ScreenAnalyze)
	{
		//��ͼ
		QScreen *VideoArea_Screen = QGuiApplication::primaryScreen();//���Ű�ť�����ͼ
		//TODO
		//δ֪ԭ��win10�Ͻ�ͼ���� 2018.10.10
		//�Ѷ�λ��chrome��ԭ�� 2018.11.26
		//��֤����chromeӲ������������ʱ�ر�chromeӲ������ѡ��� 2019.06.19
		VideoArea_Pix = VideoArea_Screen->grabWindow((WId)GoalWindow_HD, Start1.x() + 20, End1.y() - 235, 100, 100);

		QScreen *QuestionArea_Screen = QGuiApplication::primaryScreen();//���������ͼ
		QuestionArea_Pix = QuestionArea_Screen->grabWindow((WId)GoalWindow_HD, Start2.x(), Start2.y(), 770, 450);

		VideoArea_Img = VideoArea_Pix.toImage(), QuestionArea_Img = QuestionArea_Pix.toImage();

		//�򵥴ֱ������������ͼ�ж�RGB��>=200�����ص�ռ������İٷֱ�>=0.9������Ϊ������
		int PixelSub = 0;
		uchar* imagebits = QuestionArea_Img.bits();
		int RightPixel = 0;
		int TotalPixel = QuestionArea_Img.width()*QuestionArea_Img.height();
		for (int i = 0; i < QuestionArea_Img.height(); i++)
		{
			PixelSub = i*QuestionArea_Img.width() * 4;
			for (int j = 0; j < QuestionArea_Img.width(); j++)
			{
				int R = imagebits[PixelSub + j * 4 + 2];
				int G = imagebits[PixelSub + j * 4 + 1];
				int B = imagebits[PixelSub + j * 4 + 0];
				if ((R >= 200) && (G >= 200) && (B >= 200))
				{
					RightPixel++;
				}
			}
		}
		double QuestionAreaRatio = (double)RightPixel / (double)TotalPixel;
		if (QuestionAreaRatio >= 0.90)
		{
			IfQuestionShow = 1;
		}

		if (!IfQuestionShow)//��������ˣ����Լ�⵽����Ƶ������ʶ
		{
			//VideoArea_Img
			int Pixelsub_VideoArea_Img = 0;
			uchar* imagebits_VideoArea_Img = VideoArea_Img.bits();
			int TwoValue_VideoArea_Img[100][100];
			int RGB_VideoArea_Img = 0;
			for (int i = 0; i < 100; i++)
			{
				Pixelsub_VideoArea_Img = i * 100 * 4;
				for (int j = 0; j < 100; j++)
				{
					int R = imagebits_VideoArea_Img[Pixelsub_VideoArea_Img + j * 4 + 2];
					int G = imagebits_VideoArea_Img[Pixelsub_VideoArea_Img + j * 4 + 1];
					int B = imagebits_VideoArea_Img[Pixelsub_VideoArea_Img + j * 4 + 0];
					RGB_VideoArea_Img = 0.299*R + 0.587*G + 0.114*B;
					if (RGB_VideoArea_Img >= 127.5)
					{
						TwoValue_VideoArea_Img[i][j] = 1;
					}
					else
					{
						TwoValue_VideoArea_Img[i][j] = 0;
					}
				}
			}
			int VideoArea_Img_PixelValue_1 = 0;
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < 100; j++)
				{
					if (TwoValue_VideoArea_Img[i][j] == 1)
					{
						VideoArea_Img_PixelValue_1++;
					}
				}
			}
			double VideoArea_ImgRatio = (double)VideoArea_Img_PixelValue_1 / 10000;
			//ͬ���򵥴ֱ�����ֵ��ͼ����������ռ������69.640%(ʵ����)+-3%�����ڼ�����Ϊ����ͣ��־����
			if ((VideoArea_ImgRatio < 0.69640 + 0.03) && (VideoArea_ImgRatio > 0.69640 - 0.03))
			{
				IfVideoEnd = 1;
			}
		}

		sleep(5);
	}
}

void Thread_MoveArrow::run(void)
{
	while (flag_Thread_MoveArrow)
	{
		SendMessage(GoalWindow_HD, WM_MOUSEMOVE, 0, (((MiddlePoint.y()) << 16) + (MiddlePoint.x())));
		//SetCursorPos(MiddlePoint.x(), MiddlePoint.y());
		sleep(1);
		//SetCursorPos(MiddlePoint.x() + 10, MiddlePoint.y());
		SendMessage(GoalWindow_HD, WM_MOUSEMOVE, 0, (((MiddlePoint.y()) << 16) + (MiddlePoint.x() + 10)));
		sleep(1);
	}
}

void Window_Main::keyPressEvent(QKeyEvent *event)//�ڲ��������
{

}

bool Window_Main::nativeEventFilter(const QByteArray &eventType, void *message, long *result)//��д��Ϣ������-WIN_HOTKEY
{
	if (eventType == "windows_generic_MSG")
	{
		MSG *msg = (MSG*)message;
		if (msg->message == WM_HOTKEY)
		{
			//���߳����б�ʶ����
			T1.flag_Thread_MoveArrow = 0;
			T2->flag_Thread_Timer = 0;
			T3.flag_Thread_ScreenAnalyze = 0;
			show();

			return true;
		}
	}
	return false;
}