#include "Graphic3D.h"

cGraphic3D::cGraphic3D(cMesh3D mesh, cMaterial material, cAnimation3D anim)
{
	m_Mesh = mesh;
	m_Material = material;
	m_Animation3D = anim;
}
