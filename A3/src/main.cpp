//#define GLEW_STATIC
//#include<GL/glew.h>
//#include <GLFW/glfw3.h>
#include "loader.h"
#include "scene_parser.h"
#include "glCanvas.h"
#include "camera.h"
#include "light.h"
#include "material.h"
#include "group.h"
#include "image.h"

char* vertexShaderFile;
char* fragShaderFile;
char* inputFile;
char* texFile;
inline void process_parameters(int argc, char** argv);
inline void loadTexture(char* tex,GLuint p);

Image* img;
float* data;
GLuint texID;

int main(int argc, char** argv)
{
    glutInit(&argc,argv);
    glutInitWindowSize(400,400);
    glutInitWindowPosition(100,100);
    glutCreateWindow("OpenGL Viewer");
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB);
    //glutCreateWindow("GLEW Test");
   //glewInit();
    process_parameters(argc,argv);
    cout<<0<<endl;
    GLuint pO=0;
  pO=  loadShader(vertexShaderFile,fragShaderFile);
   loadTexture(texFile,pO);
    cout<<1<<endl;
    SceneParser* scene=new SceneParser(inputFile);
    GLCanvas* canvas=new GLCanvas();
    scene->getGroup()->applyTransform();
    scene->getGroup()->calEquation();
    canvas->initialize(scene,NULL);
    //unload(pO,vO,fO);
    return 0;
}

inline void process_parameters(int argc, char** argv)
{
    for(int i=1;i<argc;i++){
        if(!strcmp(argv[i], "-input")){
            i++;
            assert(i<argc);
            inputFile=argv[i];
        }
        else if(!strcmp(argv[i],"-vs")){
            i++;
            assert(i<argc);
            vertexShaderFile=argv[i];
         }
        else if(!strcmp(argv[i],"-fs")){
            i++;
            assert(i<argc);
            fragShaderFile=argv[i];
        }
        else if(!strcmp(argv[i],"-texture"))
        {
            i++;
            assert(i<argc);
            texFile=argv[i];
        }
        else{
             printf("whoops error with command line argument %d: '%s'\n",i,argv[i]);
            assert(0);
        }
    }
    assert(inputFile!=NULL);
    return;
}


void loadTexture(char* tex, GLuint p)
{
  img=Image::LoadTGA(tex);
    int w=img->Width();
    int h=img->Height();
    glUniform1f(glGetUniformLocation(p,"w"),w);
    glUniform1f(glGetUniformLocation(p,"h"),h);
    data=new float[w*h*3];
    for(int i=0;i<h;i++)
    for(int j=0;j<w;j++){
        data[3*(i*w+j)]=img->GetPixel(j,i).r();
        data[3*(i*w+j)+1]=img->GetPixel(j,i).g();
        data[3*(i*w+j)+2]=img->GetPixel(j,i).b();
    }
    glActiveTexture(GL_TEXTURE0);
    glGenTextures(1,&texID);
    glBindTexture(GL_TEXTURE_2D,texID);
    glUniform1i(glGetUniformLocation(p,"texture"),0);
   glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, w, h, 0, GL_RGB,  GL_FLOAT, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    float wf=1.0*w;
    float hf=1.0*h;
  glUniform1f(glGetUniformLocation(p,"width"),wf);
 glUniform1f(glGetUniformLocation(p,"height"),hf);
}
