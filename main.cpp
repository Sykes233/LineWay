//////////////////////
// �������ƣ�һ�ʻ� ��Ϸ
// ��  �ߣ� Smileyan<837764072@qq.com>
// ����������VC6.0 EasyX(2018.07.27)
// ����޸ģ�2019-2-14
/////////////////////
#include <graphics.h>
#include <stdio.h>
#include <conio.h>

BYTE g_aryMap[7][6];											// ��ά����洢��Ϸ��Ҫ��Ϣ
COLORREF color_title = RGB(250,120,100);						// ��ӭ���������ɫ 
COLORREF color_button = RGB(160,225,5);							// ��ť��ɫ
COLORREF color_bk = RGB(232,232,232);							// ������ɫ
COLORREF color_circleButton =  RGB(255,170,1);					// Բ�ΰ�ť��ɫ
COLORREF color_grade_bk = RGB(197,197,197);						// ���������Լ�ѡ��ȼ�����								
COLORREF color_way = RGB(250,120,100);							// �Ѿ�������·������
POINT pt_cir = {245, 400};										// ��ӭ���水ťλ��
IMAGE g_imgGameItemBlank(65,65);								// δ����·������ͼƬ
IMAGE g_imgGameItemWay(65,65);									// ����·������ͼƬ
IMAGE g_imgGameItemPlayer(65,65);								// ��ǰ·������ͼƬ
IMAGE g_imgGradeItem(80,80);									// ѡ��ȼ�ͼƬ
IMAGE g_imgGradeLock;											// δ����ĵȼ�
short g_iGrade=0;												// ��ҵ÷�
short MAX_GRADE=25;												// �ؿ���Ŀ

/**
 * ö��ͼԪ��
 * MAP_WALL ���ɵ��Ԫ��
 * MAP_BLANK δ����·��
 * MAP_WAY �Ѿ�����·��Ԫ��
 * MAP_PLAYER ��ǰλ��
 */
enum MAP_ITEM 
{ 
	MAP_WALL = 1,		
	MAP_BLANK = 2,
	MAP_PLAYER = 4,
	MAP_WAY = 8
};

void WelcomePaint();											// ���ƻ�ӭ����		
void Welcome();													// ��ӭ����ĵ��
void InitImage();												// ����ͼƬ
void ChooseGradePaint();										// ѡ��ȼ�����
int  ChooseGrade();												// ѡ��ȼ�
void GameInit(int grade);										// ��ʼ����Ϸ
void GamePaint();												// ������Ϸ
int Game(int grade);											// ��Ϸ
bool MouseInCircle(int rx,int ry,int r,int x, int y);			// �жϵ�(x,y)�Ƿ���Բr��
bool JudgeWin();												// �ж��Ƿ����
bool JudgeBorder(POINT pt1,POINT pt2);							// �ж������Ƿ�����
POINT GetNowPoint();											// ������ڵĵ�
POINT GetGird(int x, int y);									// �жϵ�(x,y)����������

int main()
{
    initgraph(490, 640);  

	InitImage();
	while(true)
	{
		// ��ӭ����
		WelcomePaint();
		Welcome();
	
		int choose = 1;
		while(true) {	
			ChooseGradePaint();
			choose = ChooseGrade();
			// ������һ��ʱ����
			if(choose < 0)
			{
				break;
			}
			GameInit(choose);
			int game = 1;
			while(true){
				// ��Ϸ����ֵ
				int game_result = Game(choose);
				
				// ���¿�ʼ
				if(game_result == 0)
				{
					GameInit(choose);
				}
				// ������һ��
				else if(game_result<0)
				{
					break;
				}
				
				GamePaint();
				// �ж��Ƿ�ʤ��
				if(JudgeWin())
				{
					// �ӷ�
					if(g_iGrade/100 < choose){
						g_iGrade += 100;
					}
					HWND hwnd = GetHWnd();
					// ͨ������
					if(choose>=MAX_GRADE)
					{
						MessageBox(hwnd, _T("��ϲ��ͨ��ȫ�� !"), _T("ѯ��"),  MB_ICONQUESTION) ;
						break;
					}
					if(MessageBox(hwnd, _T("��ϲ��ͨ�����أ��Ƿ������һ�أ�"), _T("ѯ��"), MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
					{
						choose++;
					
						GameInit(choose);
						// �������굽�����ļ�
						TCHAR s[20];
						_stprintf(s, _T("%d"), g_iGrade);			// vc6 ��д��
						::WritePrivateProfileString(_T("Grade"), _T("grade"), s, _T(".\\game.ini"));
					}
				}
				Sleep(2);
			}
		}
	}

	_getch();              // �����������
	
    closegraph();          // �رջ�ͼ����
	return 0;
}

/**
 * ����ͼƬ
 */
void InitImage()
{
	// �����Ѿ�������·��
	SetWorkingImage(&g_imgGameItemWay);
	setbkcolor(RGB(252,119,102));
	cleardevice();
	SetWorkingImage();

	// ���ƿհ�λ��
	SetWorkingImage(&g_imgGameItemBlank);
	setbkcolor(RGB(197,197,197));
	cleardevice();
	SetWorkingImage();

	// �����Ѿ������Ĺؿ�
	SetWorkingImage(&g_imgGradeItem);
	setbkcolor(RGB(197,197,197));
	cleardevice();
	setfillcolor(RGB(154,213,227));
	setlinecolor(RGB(154,213,227));
	fillroundrect(1,1,79,79,32,32);
	SetWorkingImage();
	
	// ����ͼƬ
	loadimage(&g_imgGameItemPlayer,"./photos/player.gif");
	loadimage(&g_imgGradeLock,"./photos/lock.gif");
}

/**
 * ��ӭ����
 */ 
void WelcomePaint()
{
	short i,t;	

	// ���ñ���ɫ
	setbkcolor(color_bk);
	cleardevice();
	
	// ���ڻ�����Ϸ����
	RECT r = {50,80,450,160};

	// �����ִ�С����б�ȣ���ϸ����ɫ������͸��
	settextstyle(40, 0, _T("verdana"),2,2,FW_HEAVY,true,false,false);
	settextcolor(color_bk);
	setbkmode(TRANSPARENT);
	
	// ���ñ���
	setlinecolor(color_title);
	setfillcolor(color_title);
	
	// ����Բ��
	fillroundrect(48,80,58,160,20,20);
	fillroundrect(435,80,445,160,20,20);
	
	// ���ƻ�ӭ
	for(i=55,t=440; i<=245; i+=2,t-=2) 
	{
		fillrectangle(i,80,i+2,160);
		fillrectangle(t,80,t-2,160);
		drawtext(_T("Line Way"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		Sleep(3);
	}
	
	setlinecolor(color_button);
	setfillcolor(color_button);
	// Բ����չЧ��
	for(i=0; i<=6; i++) 
	{
		fillcircle(pt_cir.x,pt_cir.y,30+i*5);
		Sleep(20);
	}

	// ����������
	setlinecolor(color_bk);
	setfillcolor(color_bk);
	POINT pts[] = { {230, 400-30}, {230, 400+30}, {280, 400} };
	fillpolygon(pts,3);
}

/**
 * ������Ϸ�ĵ��
 */
void Welcome()
{
	short i;
	// �������
	FlushMouseMsgBuffer();
	
	MOUSEMSG m;		// ���������Ϣ
	while(true)
	{
		while (MouseHit())			// ���������Ϣ��ʱ��ִ��
		{
			m = GetMouseMsg();
			switch(m.uMsg)
			{
			case WM_LBUTTONDOWN:
				// �����ʼ������Ƶ��Ч��
				if(MouseInCircle(pt_cir.x,pt_cir.y,60,m.x,m.y))
				{
					setlinecolor(color_bk);
					setfillcolor(color_bk);
					for(i=0; i<=10; i++) 
					{
						circle(pt_cir.x,pt_cir.y,60-i);
						Sleep(5);
					}
					
					setlinecolor(color_button);
					setfillcolor(color_button);
					for(i=0; i<=10; i++) 
					{
						circle(pt_cir.x,pt_cir.y,50+i);
						Sleep(5);
					}	
					return;

				}
				break;
	
			default:
				break;
			}
		
		}
	}

}

/**
 * ѡ��ؿ��������
 */
void ChooseGradePaint()
{
	// ���ñ���ɫ
	setbkcolor(color_bk);
	cleardevice();
	setlinecolor(color_circleButton);
	setfillcolor(color_circleButton);
	fillcircle(50,50,30);
	// ����������ɫ
	settextcolor(color_bk);
	setbkmode(TRANSPARENT);
	RECT r = {20,20,80,80};

	settextstyle(55, 0, _T("����"),0,0,FW_HEAVY,false,false,false);
	// ���Ʒ�����һ��
	drawtext(_T("<"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	// ���ƹ���
	fillcircle(490-50,50,30);
	RECT r5 = {440-30,20,470,80};

	drawtext(_T("i"), &r5, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	setfillcolor(color_grade_bk);
	setlinecolor(color_grade_bk);
	
	// ���Ʒ���
	fillroundrect(165,25,325,75,30,30);
	RECT r4 = {165,20,325,80};
	settextstyle(50, 0, _T("����"));

	// ��ʾ����
	if(g_iGrade==0)
	{
		// ��Ҫ�������ļ��ж�ȡһ��
		g_iGrade = GetPrivateProfileInt(_T("Grade"), _T("grade"), 0, _T(".\\game.ini"));
	}
	// ת��Ϊ�ַ�������ʾ
	char str[8];
	sprintf(str,"%d",g_iGrade);
	drawtext(str, &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

	setorigin(30,135);
	fillroundrect(-10,-15,440,445,20,20);
	settextstyle(35, 0, _T("����"));
	
	int i,t;
	// ����
	for(t=0; t<5; t++) 
	{
		for(i=0; i<5; i++)
		{
			int temp1 = i*80+i*8;
			int temp2 = t*80+t*8;
			int a = t*5+i+1;
			
			// δ����Ĺؿ�
			if(g_iGrade+100<a*100)
			{
				putimage(temp1,temp2,&g_imgGradeLock);
			}
			else
			{
				putimage(temp1,temp2,&g_imgGradeItem);
				// ��ʾ�ؿ�
				RECT r = {temp1,temp2,temp1+80,temp2+80};
				char str[8];
				sprintf(str,"%d",a);
				drawtext(str, &r,  DT_CENTER | DT_VCENTER | DT_SINGLELINE);	
			}	
		}
	}

	setorigin(0,0);
}

/**
 * ѡ��ؿ�
 */
int ChooseGrade()
{
	// �������
	FlushMouseMsgBuffer();
	
	MOUSEMSG m;		// ���������Ϣ
	int x,y;
	int t1,t2;
	COLORREF color;
	while(true)
	{	
		while (MouseHit())			// ���������Ϣ��ʱ��ִ��
		{
			// ��ȡһ�������Ϣ
			m = GetMouseMsg();
			switch(m.uMsg)
			{
			case WM_LBUTTONDOWN:
				// ����
				if(MouseInCircle(440,50,30,m.x,m.y))
				{
					MessageBox(GetHWnd(),"���ߣ� Smileyan\nQQ�� 837764072\n�ջ��Ķ࣬��лEasyX!\n�����Ƽ�! " , _T("About Line Way"), MB_OKCANCEL); 
					break;
				}

				// ������һ��
				if(MouseInCircle(50,50,30,m.x,m.y))
				{
					return -1;
				}

				//�ж��Ƿ����ڿհ״�
				color = getpixel(m.x,m.y);
				if(m.x > 460 || m.x < 40 || m.y > 570 || m.y < 120)
				{
					break;
				}

				// ����
				x = m.x - 30;
				t1 = x / 80;
				x = x - t1 * 8;
				y = m.y - 135;
				t2 = y / 80;
				y -= t2 * 8;

				// ����
				if(x > 0 && y > 0)
				{
					t1 = x/80;
					t2 = y/80;
					int witch = t1 + 1 + t2 * 5;
					// Խ��ѡ��
					if(witch*100-100>g_iGrade)
					{
						MessageBox(GetHWnd(),"���Ļ��ֲ�����˵ȼ���Ϸ���밴˳������ͨ�أ�" , _T("��ʾ"), MB_OKCANCEL);
						break; 
					}
					return witch;
				}
				break;

			default:
				break;
			}
		}
	}

}

/**
 * �ж�(x,y)�ǲ�����Բ��
 * (rx,ry)��ʾԲ��
 * r��ʾ�뾶
 */
bool MouseInCircle(int rx,int ry,int r, int x, int y)
{
	int length = (x - rx) * (x - rx) + (y - ry) * (y - ry);
	if(length <= r * r) 
	{
		return true;
	}
	return false;
}

/**
 * ��Ϸ��ʼ������Ҫ��������Ļ��ƺ���Ϸ�����ļ����ݵĶ�ȡ
 */
void GameInit(int grade)
{
	// ���ñ���ɫ
	setbkcolor(color_bk);
	cleardevice();
	setlinecolor(color_circleButton);
	setfillcolor(color_circleButton);
	fillcircle(50,50,30);
	// ����������ɫ
	settextcolor(color_bk);
	setbkmode(TRANSPARENT);
	RECT r = {20,20,80,80};
	settextstyle(55, 0, _T("����"));
	// ���Ʒ�����һ��
	drawtext(_T("<"), &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
	fillcircle(440,50,30);
	setlinecolor(color_bk);
	setlinestyle(PS_SOLID,4);
	arc(420,30,460,70,1.50,3.14+3.5);
	line(460,40,455,50);
	line(460,40,465,50);

	// ���Ʒ���
	fillroundrect(165,25,325,75,30,30);
	RECT r4 = {165,20,325,80};
	
	// ��ʾ�ؿ�
	char str[8];
	sprintf(str,"%d",grade);
	drawtext(str, &r4, DT_CENTER | DT_VCENTER | DT_SINGLELINE);


	for(int t=0; t<7; t++) 
	{
		char swit[4];
		sprintf(swit,"%d",(t+1));
		char str1[18] = "";
		GetPrivateProfileString(str, swit, _T("111111"),str1,10,_T(".\\game.ini"));
		// д�뵽ȫ�ֱ�����
		for(int i=0; i<6; i++)
		{
			g_aryMap[t][i] = str1[i] - 48;			
		}
	}
 
	GamePaint();
}

/**
 * ��Ϸ������Ļ���
 */ 
void GamePaint()
{
	setorigin(30,100);
	for(int t=0; t<7; t++) 
	{	
		for(int i=0; i<6; i++)
		{
			int temp1 = 10+i*65+i*2;
			int temp2 = 10+t*65+t*2;
			switch(g_aryMap[t][i])
			{
			case MAP_WALL:
				break;
			case MAP_BLANK:
				putimage(temp1,temp2,65,65,&g_imgGameItemBlank,0,0);
				break;
			case MAP_PLAYER:
				putimage(temp1,temp2,65,65,&g_imgGameItemPlayer,0,0);
				break;
			case MAP_WAY:
				putimage(temp1,temp2,65,65,&g_imgGameItemWay,0,0);
				break;
			default:
				break;
			}
		}
	}
	setorigin(0,0);
}

/**
 * ��Ϸ
 */ 
int Game(int grade)
{
	// �������
	FlushMouseMsgBuffer();
	int x,y;
	MOUSEMSG m;		// ���������Ϣ
	COLORREF color;
	setorigin(30,100);
	while(true)
	{	
		while (MouseHit())			// ���������Ϣ��ʱ��ִ��
		{ 
			// ��ȡһ�������Ϣ
			m = GetMouseMsg();
			switch(m.uMsg)
			{
			case WM_LBUTTONDOWN:
				// ������һ��
				if(MouseInCircle(50,50,30,m.x,m.y))
				{
					setorigin(0,0);
					return -1;
				}
				// ����
				if(MouseInCircle(440,50,30,m.x,m.y))
				{
					setorigin(0,0);
					return 0;
				}
			case WM_MOUSEMOVE:
				color = getpixel(m.x-30,m.y-100);
				if(color==color_grade_bk ) 
				{
					POINT pt = GetGird(m.x-30,m.y-100);
					x = pt.x;
					y = pt.y;
					x = 65*x+10+x*2;
					y = 65*y+10+y*2; 
			
					// �ж��Ƿ��뵱ǰ��������
					POINT now = GetNowPoint();
					if(JudgeBorder(now,pt)) 
					{
						g_aryMap[pt.y][pt.x]=MAP_PLAYER;
						g_aryMap[now.y][now.x]=MAP_WAY;
						setorigin(0,0);
						return 1;
					}
				}
				break;

			default:
				break;
			}
		}
	}
}

//�ж����ĸ�����
POINT GetGird(int x, int y)
{
	// ����
	x -= 10;
	y -= 10;
	int temp1 = x/65;	
	x -= (temp1*2);
	x /= 65;
	int temp2 = y/65;	
	y -= (temp2*2);
	y /= 65;
	POINT pt={x,y};
	return pt;
}

/**
 * �ж�ʤ��
 *
 */
bool JudgeWin()
{
	for(int t=0; t<7; t++)
	{
		for(int i=0; i<6; i++)
		{
			if(g_aryMap[t][i]==MAP_BLANK)
			{
				return false;
			}
		}
	}
	return true;
}

/**
 * ��õ�ǰ��
 */
POINT GetNowPoint()
{
	for(int t=0; t<7; t++)
	{
		for(int i=0; i<6; i++)
		{
			if(g_aryMap[t][i]==MAP_PLAYER)
			{
				POINT pt = {i,t};
				return pt;
			}
		}
	}
	POINT pt = {0,0};
	return pt;
}

/**
 * �ж������Ƿ�����
 */
bool JudgeBorder(POINT pt1,POINT pt2)
{
	if(pt1.x-pt2.x==0&&(pt1.y-pt2.y==1 || pt2.y-pt1.y==1))
	{
		return true;
	}
	
	if(pt1.y-pt2.y==0&&(pt1.x-pt2.x==1 || pt2.x-pt1.x==1))
	{
		return true;
	}
	return false;
}