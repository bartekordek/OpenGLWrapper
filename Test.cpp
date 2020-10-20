#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "CUL/STL_IMPORTS/STD_cmath.hpp"
#include "CUL/Math/Angle.hpp"
#include "CUL/Graphics/Color.hpp"

const GLfloat vertices[] = {
    -1.0f, -1.0f, 0.0f,
     1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
     1.0f,  1.0f, 0.0f,
    -1.0f,  1.0f, 0.0f
    //  X,     Y,    Z
};

const GLfloat colors[] = {
    0.0f, 0.0f, 1.0f,
    0.0f, 1.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f
    // R,    G,    B
};

const GLchar* vertex120 = R"END(
#version 120

attribute vec3 inColor;
attribute vec3 inPosition;
uniform mat4 matrix;

varying vec3 outColor;

void main()
{
    outColor = inColor;
    //gl_Position = vec4( inPosition, 1 );// * matrix; // * inPosition
    gl_Position = vec4( inPosition, 1 ) * matrix;
}
)END";

const GLchar* raster120 = R"END(
#version 120

varying vec3 outColor;

void main()
{
    gl_FragColor = vec4(outColor, 1);
}
)END";

using Color = CUL::Graphics::ColorS;

int main( int argc, char** argv )
{
    if( !glfwInit() )
    {
        return 1;
    }

    GLFWwindow* window = glfwCreateWindow( 800, 600, "Hello", 0, 0 );
    if( nullptr == window )
    {
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent( window );

    glfwWindowHint( GLFW_CONTEXT_VERSION_MAJOR, 3 );
    glfwWindowHint( GLFW_CONTEXT_VERSION_MINOR, 3 );
    glfwWindowHint( GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE );
    glfwWindowHint( GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE );

    auto glVersion = glGetString( GL_VERSION );
    glewInit();

    std::cout << "Init :: checking OpenGL version:\n";
    const unsigned char* msg;
    msg = glGetString( GL_VERSION );
    std::cout << msg << "\n Shader language version: \n";
    msg = glGetString( GL_SHADING_LANGUAGE_VERSION );
    std::cout << msg << "\n";

    // Vertex shader.
    GLuint vertexShaderId = glCreateShader( GL_VERTEX_SHADER );
    glShaderSource( vertexShaderId, 1, &vertex120, 0 );
    glCompileShader( vertexShaderId );

    GLint compilationStatus = 0;
    glGetShaderiv( vertexShaderId, GL_COMPILE_STATUS, &compilationStatus );
    if( GL_FALSE == compilationStatus )
    {
        GLchar message[256];
        glGetShaderInfoLog( vertexShaderId, sizeof( message ), 0, message );
        std::cout << message << "\n";
        return 2;
    }

    // Fragment shader.
    GLuint fragmentShaderId = glCreateShader( GL_FRAGMENT_SHADER );
    glShaderSource( fragmentShaderId, 1, &raster120, 0 );
    glCompileShader( fragmentShaderId );

    compilationStatus = 0;
    glGetShaderiv( fragmentShaderId, GL_COMPILE_STATUS, &compilationStatus );
    if( GL_FALSE == compilationStatus )
    {
        GLchar message[256];
        glGetShaderInfoLog( fragmentShaderId, sizeof( message ), 0, message );
        std::cout << message << "\n";
        return 3;
    }

    // Shader program.
    auto shaderProgramId = glCreateProgram();
    glAttachShader( shaderProgramId, vertexShaderId );
    glAttachShader( shaderProgramId, fragmentShaderId );
    glLinkProgram( shaderProgramId );

    glGetProgramiv( shaderProgramId, GL_LINK_STATUS, &compilationStatus );
    if( GL_FALSE == compilationStatus )
    {
        GLchar message[256];
        glGetProgramInfoLog( shaderProgramId, sizeof( message ), 0, message );
        std::cout << message << "\n";
        return 4;
    }

    glUseProgram( shaderProgramId );

    // VBO setup.

    GLuint vertexBuffer = 0;
    glGenBuffers( 1, &vertexBuffer );

    GLuint colorBuffer = 0;
    glGenBuffers( 1, &colorBuffer );


    // Send data to GPU.

    glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( vertices ), vertices, GL_STATIC_DRAW );

    glBindBuffer( GL_ARRAY_BUFFER, colorBuffer );
    glBufferData( GL_ARRAY_BUFFER, sizeof( colors ), colors, GL_STATIC_DRAW );

    GLint attributePosition = glGetAttribLocation( shaderProgramId, "inPosition" );
    glEnableVertexAttribArray( attributePosition );
    glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
    glVertexAttribPointer( attributePosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );

    GLint attributeColor = glGetAttribLocation( shaderProgramId, "inColor" );
    glEnableVertexAttribArray( attributeColor );
    glBindBuffer( GL_ARRAY_BUFFER, colorBuffer );
    glVertexAttribPointer( attributeColor, 3, GL_FLOAT, GL_FALSE, 0, 0 );

    // Uniforms.

    auto attributeMatrix = glGetUniformLocation( shaderProgramId, "matrix" );
    float alpha = 0.0f;

    while( false == glfwWindowShouldClose( window ) )
    {
        glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
        glClear( GL_COLOR_BUFFER_BIT );

        auto sa = 0.5f * std::sin( alpha );
        auto ca = 0.5f * std::cos( alpha );
        alpha += 0.05f;

        const GLfloat matrix[] = {
              sa,  -ca, 0.0f, 0.0f,
              ca,   sa, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f
        };

        glUniformMatrix4fv( attributeMatrix, 1, GL_FALSE, matrix );

        glDrawArrays( GL_TRIANGLES, 0, 6 );

        glfwSwapBuffers( window );
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}