#include<iostream>
#include<GL/glut.h>
#include<stdlib.h>
using namespace std;

int tim=1000/20,ch=1;
const int R=200,C=200,W=700,H=700;
const int gC=16,gR=16;
bool mat[R+2][C+2];
bool tmat[R+2][C+2];
bool imat[gC][gR];
bool paused=true;
bool randmode=false;

void menu();

void clearTmat(){
    for(int i=0;i<=R+1;i++){
        for(int j=0;j<=C+1;j++){
            tmat[i][j]=false;
        }
    }
}

void clearMat(){
    for(int i=0;i<=R+1;i++){
        for(int j=0;j<=C+1;j++){
            mat[i][j]=false;
        }
    }
}

void initMat(){
    for(int i=0;i<=R+1;i++){
        for(int j=0;j<=C+1;j++){
            mat[i][j]=false;
            tmat[i][j]=false;
        }
    }
}

void project(){
    for(int i=1;i<=R;i++){
        for(int j=1;j<=C;j++){
            mat[i][j]=tmat[i][j];
        }
    }
}

void process(){
    if(!paused){
        for(int i=1;i<=R;i++){
            for(int j=1;j<=C;j++){
                int count=0;
                for(int x=i-1;x<=i+1;x++){
                    for(int y=j-1;y<=j+1;y++){
                        if(mat[x][y]){
                            if(x!=i||y!=j){
                                count++;
                            }
                        }
                    }
                }
                if(mat[i][j]){
                    if(count==2||count==3){
                        tmat[i][j]=true;
                    }
                }
                else{
                    if(count==3){
                        tmat[i][j]=true;
                    }
                }
            }
        }
        project();
        clearTmat();
    }
}

void border(){
    float x=(float)C,y=(float)R;
    glColor3f(1.0,1.0,1.0);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(0.0,0.0);
    glVertex2f(x,0.0);
    glVertex2f(x,0.0);
    glVertex2f(x,y);
    glVertex2f(x,y);
    glVertex2f(0.0,y);
    glVertex2f(0.0,y);
    glVertex2f(0.0,0.0);
    glEnd();
}

void mapMatrix(){
    for(int y=1;y<=R;y++){
        for(int x=1;x<=C;x++){
            if(mat[y][x]){
                glColor3f(1.0,1.0,1.0);
                glBegin(GL_QUADS);
                glVertex2f(x,y);
                glVertex2f(x+1,y);
                glVertex2f(x+1,y+1);
                glVertex2f(x,y+1);
                glEnd();
            }
        }
    }
}

void display(void){
    glClear(GL_COLOR_BUFFER_BIT );
    glLoadIdentity();
    mapMatrix();
    border();
    glutSwapBuffers();
}

void keyboard(unsigned char c,int x,int y){
    switch(c){
        case ' ':
            if(paused){
                paused=false;
            }
            else{
                paused=true;
            }
        break;
        case '\r':
        case '\e':
            paused=true;
            clearMat();
            clearTmat();
            glutDestroyWindow(glutGetWindow());
            menu();
    }
}

void reshape(int w, int h){
    float x=(float)C,y=(float)R;
    glutReshapeWindow(W,H);
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,x,0.0,y);
    glMatrixMode(GL_MODELVIEW);
}



void timer(int){
    process();
    glutPostRedisplay();
    glutTimerFunc(tim,timer,0.0);
}

void inShape(int ch){
    int x=gR/2;
    int y=gC/2;
    switch(ch){
        case 1:
            imat[x][y]=true;
            imat[x-1][y]=true;
            imat[x-1][y-1]=true;
            imat[x-2][y-1]=true;
            imat[x][y+1]=true;
            imat[x][y+2]=true;
            imat[x+1][y+2]=true;
        break;
        case 2:
            imat[x][y+1]=true;
            imat[x+1][y+1]=true;
            imat[x-1][y]=true;
            imat[x+1][y]=true;
            imat[x+1][y-1]=true;
        break;
    }
}

void clearImat(){
    for(int i=0;i<gR;i++){
        for(int j=0;j<gC;j++){
            imat[i][j]=false;
        }
    }
}

void mapImat(){
    int x=(R/2)-(gR/2);
    int y=(C/2)-(gC/2);
    for(int i=0;i<gR;i++){
        for(int j=0;j<gC;j++){
            mat[i+x][j+y]=imat[i][j];
        }
    }
}

int getXpos(int x){
    int d=H/gR;
    return (int)x/d;
}

int getYpos(int y){
    int d=W/gC;
    return gR-((int)y/d)-1;
}

void init(){
    initMat();
    mapImat();
    glClearColor(0.0,0.0,0.0,1.0);
}


void game(){
    glutInitWindowSize(W,H);
    glutInitWindowPosition(200,0);
    glutCreateWindow("Life");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(0,timer,0);
    glutMainLoop();    
}

void randInit(){
    int r=rand()%(rand()%100);
    r++;
    for(int i=0;i<=R+1;i++){
        for(int j=0;j<=C+1;j++){
            if(rand()%r==0){
                imat[i][j]=true;
            }
        }
    }
}

void keyboardMe(unsigned char c,int x,int y){
    switch(c){
        case '\r':
            mapImat();
            glutDestroyWindow(glutGetWindow());
            game();
        break;
        case ' ':
            clearImat();
            randInit();
            glutPostRedisplay();
        break;
        case '\e':
            clearImat();
            glutPostRedisplay();
        break;
        case '0':
            clearImat();
            randmode=true;
            randInit();
            mapImat();
            glutDestroyWindow(glutGetWindow());
            game();
        break;
        case '1':
            clearImat();
            inShape(1);
            glutPostRedisplay();
        break;
        case '2':
            clearImat();
            inShape(2);
            glutPostRedisplay();
        break;
        case '3':
            clearImat();
            inShape(3);
            glutPostRedisplay();
        break;
        case '4':
            clearImat();
            inShape(4);
            glutPostRedisplay();
        break;
        case '5':
            clearImat();
            inShape(5);
            glutPostRedisplay();
        break;
        case '6':
            clearImat();
            inShape(6);
            glutPostRedisplay();
        break;
        case '7':
            clearImat();
            inShape(7);
            glutPostRedisplay();
        break;
        case '8':
            clearImat();
            inShape(8);
            glutPostRedisplay();
        break;
        case '9':
            clearImat();
            inShape(9);
            glutPostRedisplay();
        break;
    }
}

void reshapeMe(int w, int h){
    float x=(float)gC,y=(float)gR;
    glutReshapeWindow(W,H);
    glViewport(0,0,(GLsizei)w,(GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0,x,0.0,y);
    glMatrixMode(GL_MODELVIEW);
}

void mapGrid(){
    for(int x=0;x<gR;x++){
        for(int y=0;y<gC;y++){
            if(imat[x][y]){
                glColor3f(1.0,1.0,1.0);
                glBegin(GL_QUADS);
                glVertex2f(x,y);
                glVertex2f(x+1,y);
                glVertex2f(x+1,y+1);
                glVertex2f(x,y+1);
                glEnd();
            }
        }
    }
}

void initMe(){
    clearImat();
    glClearColor(0.0,0.0,0.0,1.0);
}

void borderMe(){
    float x=(float)gR,y=(float)gC;
    glColor3f(1.0,1.0,1.0);
    glLineWidth(2);
    glBegin(GL_LINES);
    glVertex2f(0.0,0.0);
    glVertex2f(x,0.0);
    glVertex2f(x,0.0);
    glVertex2f(x,y);
    glVertex2f(x,y);
    glVertex2f(0.0,y);
    glVertex2f(0.0,y);
    glVertex2f(0.0,0.0);
    glEnd();
}

void displayMe(void){
    glClear(GL_COLOR_BUFFER_BIT );
    glLoadIdentity();
    mapGrid();
    borderMe();
    glutSwapBuffers();
}

void mouseCall(int button,int state,int x,int y){
    if(button==GLUT_LEFT_BUTTON&&state==GLUT_DOWN){
        int i=getXpos(x);
        int j=getYpos(y);
        if(imat[i][j]){
            imat[i][j]=false;
        }
        else{
            imat[i][j]=true;
        }
        glutPostRedisplay();
    }
}

void menu(){
    glutInitWindowSize(W,H);
    initMe();
    glutInitWindowPosition(200,0);
    glutCreateWindow("Life");
    glutDisplayFunc(displayMe);
    glutReshapeFunc(reshapeMe);
    glutKeyboardFunc(keyboardMe);
    glutMouseFunc(mouseCall);
    glutMainLoop();
}

int main(int argc,char** argv){
    std::ios_base::sync_with_stdio(false);
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE );
    menu();
    return 0; 
}
