#include "pch.h"
#include "Mesh.h"

bool SpaRcle::Graphics::Mesh::Destroy() noexcept {
    if (this->m_is_destroyed) return false;
    else this->m_is_destroyed = true;

    return true;
}

bool SpaRcle::Graphics::Mesh::Draw() {
    if (m_is_destroyed) return false;

    /*
    
        draw geometry...
    
    */

    return true;
}

bool SpaRcle::Graphics::Mesh::Calculate() {
    /*
    
        binding vertex array...
        
    */

    return true;
}
