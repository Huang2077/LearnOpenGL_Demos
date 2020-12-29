#pragma once
#ifndef PARTICLE_CLASS_H
#define PARTICLE_CLASS_H 

#include <glm/glm.hpp>
class Particale {
private:
	glm::vec3 m_initposition;//��ʼλ��
	glm::vec3 m_v0;//���ٶ�
	float m_density;//�ܶ�
	float m_quality;//����
	int m_life;//����ֵ
	float m_inittime;//����ʱ��

public:
	Particale(glm::vec3 m_initposition = glm::vec3(0.0, 0.0, 0.0), glm::vec3 m_v0 = glm::vec3(1.0, 0.0, 0.0), int m_life = 10, float m_inittime = 0.0, float m_quality = 1.0, float m_density = 1.0)
	{
		this->m_density = m_density;
		this->m_inittime = m_inittime;
		this->m_v0 = m_v0;
		this->m_life = m_life;
		this->m_quality = m_quality;
		this->m_initposition = m_initposition;
	}
	void ReduceLife(int size) //����ֵ����
	{
		m_life -= size;
	}
	bool IsAlive()//�ж������Ƿ���
	{
		if (m_life > 0)
			return true;
		else
			return false;
	}
	glm::vec3 GetOffset(glm::vec3 a, float nowtime )//�ؼ���ʽ��������ӵ�λ����(�Ժ���Ը�д)
	{
		glm::vec3 offset;
		float t = nowtime - m_inittime;
		offset = m_v0 * t  -  (float)0.5 * a * t * t ; //λ�ƹ�ʽ
		return offset;
	}
	glm::vec3 GetInitPosition()
	{
		return m_initposition;
	}
};
#endif