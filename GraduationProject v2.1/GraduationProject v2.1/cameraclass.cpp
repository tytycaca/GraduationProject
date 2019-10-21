////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass()
{
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;

	m_rotationX = 0.0f;
	m_rotationY = 0.0f;
	m_rotationZ = 0.0f;

	m_pos.x = m_positionX;
	m_pos.y = m_positionY;
	m_pos.z = m_positionZ;
	m_lookAt.x = m_rotationX;
	m_lookAt.y = m_rotationY;
	m_lookAt.z = m_rotationZ;
	m_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_positionX = x;
	m_positionY = y;
	m_positionZ = z;

	m_pos.x = m_positionX;
	m_pos.y = m_positionY;
	m_pos.z = m_positionZ;

	return;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	/*m_rotationX = x;
	m_rotationY = y;
	m_rotationZ = z;*/

	if (m_rotationX + x > 90.0f) m_rotationX = 90.0f;
	else if (m_rotationX + x < -90.0f) m_rotationX = -90.0f;
	else m_rotationX += x;
	m_rotationY += y;
	m_rotationZ += z;

	if (m_rotationX > 360.0f) m_rotationX -= 360.0f;
	else if (m_rotationX < -360.0f) m_rotationX += 360.0f;

	if (m_rotationY > 360.0f) m_rotationY -= 360.0f;
	else if (m_rotationY < -360.0f) m_rotationY += 360.0f;

	if (m_rotationZ > 360.0f) m_rotationZ -= 360.0f;
	else if (m_rotationZ < -360.0f) m_rotationZ += 360.0f;

	m_lookAt.x = m_rotationX;
	m_lookAt.y = m_rotationY;
	m_lookAt.z = m_rotationZ;

	return;
}


D3DXVECTOR3 CameraClass::GetPosition()
{
	return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ);
}


D3DXVECTOR3 CameraClass::GetRotation()
{
	return D3DXVECTOR3(m_rotationX, m_rotationY, m_rotationZ);
}


void CameraClass::Render()
{
	D3DXVECTOR3 up, position, lookAt;
	float yaw, pitch, roll;
	D3DXMATRIX rotationMatrix;


	// Setup the vector that points upwards.
	up.x = 0.0f;
	up.y = 1.0f;
	up.z = 0.0f;

	// Setup the position of the camera in the world.
	position.x = m_positionX;
	position.y = m_positionY;
	position.z = m_positionZ;

	// Setup where the camera is looking by default.
	lookAt.x = 0.0f;
	lookAt.y = 0.0f;
	lookAt.z = 1.0f;

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotationX * 0.0174532925f;
	yaw   = m_rotationY * 0.0174532925f;
	roll  = m_rotationZ * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	D3DXMatrixRotationYawPitchRoll(&rotationMatrix, yaw, pitch, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	D3DXVec3TransformCoord(&lookAt, &lookAt, &rotationMatrix);
	D3DXVec3TransformCoord(&up, &up, &rotationMatrix);

	//수정
	m_up = up;

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;
	// 수정
	m_lookAt = lookAt;

	// Finally create the view matrix from the three updated vectors.
	D3DXMatrixLookAtLH(&m_viewMatrix, &position, &lookAt, &up);

	return;
}


void CameraClass::GetViewMatrix(D3DXMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
	return;
}


void CameraClass::FrontWalk(long speed)
{
	D3DXVECTOR3 m_view, Direction;
	// 수정
	D3DXVECTOR3 m_upPos, upDirection;
	m_upPos = m_up; // + D3DXVECTOR3(0,0.5f,0);

	m_view = m_lookAt - m_pos;


	D3DXVec3Normalize(&Direction, &m_view);
	// 수정
	D3DXVec3Normalize(&upDirection, &m_upPos);

	//m_pos += m_dwElapsedTime * Direction * speed / 5.0f;
	m_pos.x += m_dwElapsedTime * Direction.x * speed / 5.0f;
	m_pos.y += m_dwElapsedTime * Direction.y * speed / 5.0f;
	m_pos.z += m_dwElapsedTime * Direction.z* speed / 5.0f;

	// 수정 : 다 필요없었음 카메라 회전부분에서 마우스의 z부분을 안가져온게 잘못이었음
	//m_pos.y += m_dwElapsedTime * upDirection.z * speed / 5.0f * -1.0f;
	/*if (m_upPos.y <= 0.0f)
		m_pos.y += m_dwElapsedTime * upDirection.y * speed / 5.0f * -1.0f;
	else
		m_pos.y += m_dwElapsedTime * upDirection.y * speed / 5.0f * 1.0f;*/

	m_lookAt += m_dwElapsedTime * Direction * speed / 5.0f;

	//CheckRange();

	m_positionX = m_pos.x;
	m_positionY = m_pos.y;
	m_positionZ = m_pos.z;


	return;
}

void CameraClass::SideWalk(long speed)
{
	D3DXVECTOR3 m_view, Direction, upNormal, forwardNormal;
	m_view = m_lookAt - m_pos;

	D3DXVec3Normalize(&upNormal, &m_up);
	D3DXVec3Normalize(&forwardNormal, &m_view);
	D3DXVec3Cross(&Direction, &forwardNormal, &upNormal);

	D3DXVec3Normalize(&Direction, &Direction);

	//m_pos += m_dwElapsedTime * Direction * speed / 5.0f;
	m_pos.x += m_dwElapsedTime * Direction.x * speed / 5.0f;
	//m_pos.y += m_dwElapsedTime * Direction.x * speed / 5.0f;
	m_pos.z += m_dwElapsedTime * Direction.z* speed / 5.0f;
	m_lookAt += m_dwElapsedTime * Direction * speed / 5.0f;

	//CheckRange();

	m_positionX = m_pos.x;
	//m_positionY = m_pos.y;
	m_positionZ = m_pos.z;
	return;
}

void CameraClass::CheckRange()
{
	if (m_pos.x > 500.0f) m_pos.x = 500.0f;
	else if (m_pos.x < -500.0f) m_pos.x = -500.0f;

	if (m_pos.y > 500.0f) m_pos.y = 500.0f;
	else if (m_pos.y < -500.0f) m_pos.y = -500.0f;

	if (m_pos.z > 500.0f) m_pos.z = 500.0f;
	else if (m_pos.z < -500.0f) m_pos.z = -500.0f;
}

void CameraClass::TickUpdate()
{
	DWORD dwCurTime = GetTickCount();
	static DWORD dwOldTime = GetTickCount();
	m_dwElapsedTime = dwCurTime - dwOldTime;
	dwOldTime = dwCurTime;
}