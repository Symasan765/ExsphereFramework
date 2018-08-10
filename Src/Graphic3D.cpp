#include "Graphic3D.h"

cGraphic3D::cGraphic3D(const cMesh3D& mesh, const cMaterial& material, const cAnimation3D& anim)
{
	m_Mesh = mesh;
	m_Material = material;
	m_Animation3D = anim;
}
