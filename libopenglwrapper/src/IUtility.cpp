#include "libopenglwrapper/IUtility.hpp"

#include "IMPORT_glew.hpp"
#include "ImportFreeglut.hpp"
#include "SDL2Wrapper/IMPORT_SDL_opengl.hpp"

using namespace LOGLW;

IUtility::IUtility()
{
}

void IUtility::setUniformValue( int uniformLocation, const glm::vec2& val )
{
    glUniform2fv( uniformLocation, 1, &val[0] );
}

void IUtility::setUniformValue( int uniformLocation, const glm::vec3& val )
{
    glUniform3fv( uniformLocation, 1, &val[0] );
}

void IUtility::setUniformValue( int uniformLocation, const glm::vec4& val )
{
    glUniform4fv( uniformLocation, 1, &val[0] );
}

void IUtility::setUniformValue( int uniformLocation, const glm::mat2& val )
{
    glUniformMatrix2fv( uniformLocation, 1, GL_FALSE, &val[0][0] );
}

void IUtility::setUniformValue( int uniformLocation, const glm::mat3& val )
{
    glUniformMatrix3fv( uniformLocation, 1, GL_FALSE, &val[0][0] );
}

void IUtility::setUniformValue( int uniformLocation, const glm::mat4& val )
{
    glUniformMatrix4fv( uniformLocation, 1, GL_FALSE, &val[0][0] );
}

void IUtility::setActiveTexture( unsigned id )
{
    glActiveTexture(static_cast<GLenum>(id));
}

IUtility::~IUtility()
{
}
