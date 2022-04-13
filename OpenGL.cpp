#include "GL\glut.h"
#include <math.h>

bool isdrawline; // 判断是否画线

struct Point {
    double px;
    double py;
};

//定义已经鼠标点击的点的数量
static int pointsum = 0;

//四次的bezier曲线共有五个点
Point points[5];

// 选择移动的点
int select_point = 0;

// 点是否被选中
int click_state = 0; // 0表示未被选中，1表示选中


void init()
{
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 500.0, 0.0, 500.0);
    glMatrixMode(GL_MODELVIEW);
}

//画点
void setPoint(Point p) 
{
    glPointSize(6);
    glBegin(GL_POINTS);
    glVertex2f(p.px, p.py);
    glEnd();
}

// 画直线
void setline(Point* p, int num) {

    glBegin(GL_LINE_STRIP);
    for (int i = 0; i < num; i++) {
        glVertex2f(p[i].px, p[i].py);
    }
    glEnd();
}

// 画曲线
void setcurve(Point p1, Point p2)
{
    glBegin(GL_LINES);
    glVertex2f(p1.px, p1.py);
    glVertex2f(p2.px, p2.py);
    glEnd();
}

// 公式法计算获得bezier曲线
Point Bezier(Point p1, Point p2, Point p3, Point p4, Point p5, double t)
{
    Point p;
    p.px = p1.px * pow((1 - t), 4) + 4 * p2.px * pow((1 - t), 3) * t + 6 * p3.px * (1 - t) * (1 - t) * t * t + 4 * p4.px * (1 - t) * pow(t, 3) + p5.px * pow(t, 4);
    p.py = p1.py * pow((1 - t), 4) + 4 * p2.py * pow((1 - t), 3) * t + 6 * p3.py * (1 - t) * (1 - t) * t * t + 4 * p4.py * (1 - t) * pow(t, 3) + p5.py * pow(t, 4);
    return p;
}

//display函数
void display()
{

    glClearColor(0.0, 0.0, 0.0, 0.0); //设背景为黑色
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //用glClearClolr设定的颜色值清除缓存区


    // 画点
    for (int i = 0; i < 5; i++)
    {
        glColor3f(1.0, 1.0, 0.0);
        setPoint(points[i]);
    }
    if (isdrawline)
    {
        glColor3f(0.0, 1.0, 1.0); // 设定bezier曲线
        setline(points, 5);

        glColor3f(1.0, 1.0, 1.0); // 设定bezier曲线
        Point p_current = points[0]; //设为起点
        for (double t = 0.0; t <= 1.0; t += 0.05)
        {
            Point P = Bezier(points[0], points[1], points[2], points[3], points[4], t);
            setcurve(p_current, P);
            p_current = P;
        }
    }

    glFlush();
}

// 鼠标事件
void MouseFunction(int button, int state, int x, int y)
{
    int distance = 0;
    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
        {
            points[pointsum].px = x;
            points[pointsum].py = 500 - y;
            if (pointsum == 0)
                isdrawline = false;
            if (pointsum == 4) {
                isdrawline = true;
                pointsum = 10; // 表示不能再添加点

            }
            else {
                pointsum++;
            }
        }
    }

    if (button == GLUT_LEFT_BUTTON) // 表示无法新增点，只能移动
    {
        for (int i = 0; i < 5; i++)
        {
            //按控制点依次计算距离
            distance = (x - points[i].px) * (x - points[i].px) + (500 - y - points[i].py) * (500 - y - points[i].py);
            if (distance < 15)//如果距离小于特定值
            {
                click_state = 1;
                select_point = i; // 选中这个点
                break;//跳出循环
            }
        }

        if (state == GLUT_UP)//移动点
        {
            points[select_point].px = x;
            points[select_point].py = 500 - y;
            
            glutPostRedisplay();
            select_point = -1;// 松开鼠标，表示移动结束
            click_state = 0;
        }
    }
 }


void mouse_motion(int x, int y)
{

    if (click_state == 1)
    {
        points[select_point].px = x;
        points[select_point].py = 500 - y;
        glutPostRedisplay();
    }
}

int main(int argc, char* argv[])

{

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);

    glutInitWindowPosition(0, 0);

    glutInitWindowSize(500, 500); //确定显示框的大小

    glutCreateWindow("第一次图形学上机");

    init();

    glutMouseFunc(MouseFunction);// 鼠标事件

    glutMotionFunc(mouse_motion);

    glutDisplayFunc(display);

    glutMainLoop();

    return 0;

}
