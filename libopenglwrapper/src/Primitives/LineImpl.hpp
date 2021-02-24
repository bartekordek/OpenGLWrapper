#pragma once

#include "libopenglwrapper/Primitives/ILine.hpp"

NAMESPACE_BEGIN( LOGLW )

class LineImpl final:
    public ILine,
    public IUtilityUser
{
public:
    QuadData m_data;

    LineImpl();

    void setColor( const LineColors& colors ) override;
    void setColor( const ColorS& color ) override;

    ~LineImpl();
protected:
private:
    void setValues( const LineData& values ) override;
    void render() override;

    LineColors m_colors;

};

NAMESPACE_END( LOGLW )