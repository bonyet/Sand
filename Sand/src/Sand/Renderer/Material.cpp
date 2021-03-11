#include "sandpch.h"
#include "Material.h"

namespace Sand
{

    Material::Material(const Ref<Shader>& shader)
        : m_Shader(shader)
    {
    }
    
    Material::~Material()
    {
    }
    
    Ref<Material> Material::Create(const Ref<Shader>& shader)
    {
        return CreateRef<Material>(shader);
    }

}