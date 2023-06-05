#include "glad/include/glad/glad.h"
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <array>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sched.h>
#include <string>
#include <ctime>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

static int CompileShader(unsigned int type, const std::string& source) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();

    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if(result == GL_FALSE) {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char message[length];
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed To Compile " << (type == GL_VERTEX_SHADER ? "Vertex" : "Fragment") << " Shader" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);


    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);
    return program;
}

const std::string readFile(const std::string& path) {
    std::string result;
    std::ifstream file_out;
    file_out.open(path);
    std::string line;
    while(std::getline(file_out, line)) {
        result += line + "\n";
    }
    return result;
}

int main()
{
    auto start = std::chrono::system_clock::now();
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }   

    
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);  
  
    //create shader
    //std::array<float, 12> positions = {
    //    -1.0f, 1.0f,
    //    1.0f, -1.0f,
    //    -1.0f, -1.0f,

    //    1.0f, 1.0f,
    //    1.0f, -1.0f,
    //    -1.0f, 1.0f
    //};

    float positions[] {
        -1.0f, 1.0f,
        1.0f, -1.0f,
        -1.0f, -1.0f,
        1.0f, 1.0f,
    };

    unsigned int indices[] {
        0, 1, 2,
        0, 3, 1
    };


    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, 4*2*sizeof(float), positions, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float)*2, 0);

    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*sizeof(unsigned int), indices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
    
    //parse vertex and fragment shader files
    const std::string vertexShader = readFile("shader.vert");
    const std::string fragmentShader = readFile("shader.frag");

    unsigned int shader = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shader);

    GLint timeUniform = glGetUniformLocation(shader, "time");

    while(!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        
        auto end = std::chrono::system_clock::now();
        std::chrono::duration<double> elapsed_seconds = end-start;

        glUniform1f(timeUniform, elapsed_seconds.count());
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        
        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    glfwTerminate();
    return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
} 

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
