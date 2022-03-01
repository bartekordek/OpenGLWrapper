#pragma once

#include "libopenglwrapper/IUtility.hpp"
#include "libopenglwrapper/Shader.hpp"
#include "libopenglwrapper/IShaderFactory.hpp"

#include "CUL/STL_IMPORTS/STD_map.hpp"
#include "CUL/STL_IMPORTS/STD_vector.hpp"
#include "CUL/STL_IMPORTS/STD_variant.hpp"
#include "CUL/STL_IMPORTS/STD_deque.hpp"

#include "CUL/IMPORT_GLM.hpp"

#include "CUL/Filesystem/Path.hpp"

NAMESPACE_BEGIN( LOGLW )

using String = CUL::String;
using ShaderList = std::map<String, Shader*>;

class VertexArray;

class LIBOPENGLWRAPPER_API Program final: private IUtilityUser
{
public:
    struct Pair
    {
        String name;
        std::variant<float, unsigned, int> value;
    };
    using ValueToSet = Pair;

    Program();

    void setAttrib( const String& name, const char* value );
    void setAttrib( const String& name, float value );
    void setAttrib( const String& name, unsigned value );
    void setAttrib( const String& name, int value );
    void setAttrib( const String& name, bool value );
    void setAttrib( const String& name, const glm::mat2& mat );
    void setAttrib( const String& name, const glm::mat3& mat );
    void setAttrib( const String& name, const glm::mat4& mat );

    String getAttributeStr( const String& name );
    float getAttributeF( const String& name );
    unsigned int getAttributeUi( const String& name );
    int getAttributeI( const String& name );

    void attachShader( Shader* shader );
    void dettachShader( Shader* shader = nullptr );
    void link();
    void enable();
    void disable();
    void validate();

    void bufferData(
        std::vector<float>& data,
        const BufferTypes type );

    unsigned int getProgramId();
    const ShaderList& getShaderList() const;

    bool initialized() const;
    void initialize();

    void render();

    ~Program();
protected:

private:
    void pushTask( ValueToSet& task );
    void goThroughTasks();
    void processTask( const ValueToSet& task );

    unsigned int m_dataBufferId = 0;
    unsigned int m_id = 0;

    ShaderList m_attachedShaders;
    std::mutex m_operationMutex;

    std::deque<ValueToSet> m_tasks;

private:
    Program( const Program& arg ) = delete;
    Program( Program&& arg ) = delete;
    Program& operator=( const Program& rhv ) = delete;
    Program& operator=( Program&& rhv ) = delete;
};

NAMESPACE_END( LOGLW )