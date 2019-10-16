#pragma once

#include "libopenglwrapper/Import.hpp"
#include "CUL/GenericUtils/SimpleAssert.hpp"
#include "ImportFreeglut.hpp"

NAMESPACE_BEGIN( LOGLW )

class MatrixStack
{
public:
    MatrixStack()
    {

    }

    ~MatrixStack()
    {
        CUL::Assert::simple(
            0 == m_currentMatrix,
            "ERROR PUSH COUNT IS NOT EQUAL TO POP COUNT." );
    }

    void push()
    {
        glPushMatrix();
        ++m_currentMatrix;
    }

    void pop()
    {
        glPopMatrix();
        --m_currentMatrix;
    }

protected:
private:
    unsigned int m_currentMatrix = 0;

};

NAMESPACE_END( LOGLW )