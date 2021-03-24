#include "ObjLoader.hpp"


#include "CUL/GenericUtils/SimpleAssert.hpp"

using namespace LOGLW;

ObjDef* ObjLoader::loadObj( const CUL::String& path )
{
    auto result = new ObjDef();

    CUL::Assert::simple( tinyobj::LoadObj(
        &result->attrib,
        &result->shapes,
        &result->materials,
        &result->warn.string(),
        &result->error.string(),
        path.cStr() ), "Cannot load obj: " + path );

    return result;
}